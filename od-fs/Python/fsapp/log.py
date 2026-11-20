"""Python's output in the log: the logging module's records, sys.stdout and sys.stderr.

Everything goes through _fsapp.log(priority, text), which logs the text with SDL's logging, so that
Python's lines take the same way into the log as the C side's (fs/log.h), with their thread's name
and in order with them. The log splits a text with newlines into one log line per line.

install() is called by fsapp (fsapp_main_install_python_log) right after Python is initialised,
before any other of the program's Python code is imported. sys.__stdout__ and sys.__stderr__ stay
the real streams.
"""

import io
import logging
import os
import sys
import threading
import traceback

import _fsapp

# The format of a logging record's text: its logger's name before the message. The log line shows
# the time, the thread and the level.
LOG_RECORD_FORMAT = "%(name)s: %(message)s"


def priority_for_level(level: int) -> int:
    """SDL's log priority for a logging level: a level between two of logging's named levels has
    the lower one's priority."""
    if level >= logging.CRITICAL:
        return _fsapp.LOG_PRIORITY_CRITICAL
    if level >= logging.ERROR:
        return _fsapp.LOG_PRIORITY_ERROR
    if level >= logging.WARNING:
        return _fsapp.LOG_PRIORITY_WARN
    if level >= logging.INFO:
        return _fsapp.LOG_PRIORITY_INFO
    return _fsapp.LOG_PRIORITY_DEBUG


class LogHandler(logging.Handler):
    """Hands each logging record to the log, with its level as SDL's priority. A record of
    several lines (a traceback with exc_info) is several log lines."""

    def __init__(self) -> None:
        super().__init__()
        self.setFormatter(logging.Formatter(LOG_RECORD_FORMAT))

    def emit(self, record: logging.LogRecord) -> None:
        try:
            _fsapp.log(priority_for_level(record.levelno), self.format(record))
        except Exception:
            self.handleError(record)


class LogStream(io.TextIOBase):
    """A text stream that hands each complete line written to it to the log, at the priority
    given. A partial line is kept until its newline, or until the stream is flushed."""

    encoding = "utf-8"
    errors = "backslashreplace"

    def __init__(self, priority: int, name: str) -> None:
        super().__init__()
        self._priority = priority
        self._name = name
        self._partial = ""
        # Reentrant, so that a write from a signal handler on the same thread cannot deadlock.
        self._lock = threading.RLock()

    @property
    def name(self) -> str:
        return self._name

    def writable(self) -> bool:
        return True

    def isatty(self) -> bool:
        return False

    def write(self, text: str) -> int:
        if not isinstance(text, str):
            raise TypeError(f"write() argument must be str, not {type(text).__name__}")
        with self._lock:
            lines, newline, self._partial = (self._partial + text).rpartition("\n")
            if newline:
                _fsapp.log(self._priority, lines)
        return len(text)

    def flush(self) -> None:
        with self._lock:
            if self._partial:
                partial, self._partial = self._partial, ""
                _fsapp.log(self._priority, partial)


def install() -> None:
    """Sends Python's output to the log: sys.stdout at priority info, sys.stderr at priority warn,
    and the logging module's records through the root logger's handler (at level info)."""
    sys.stdout = LogStream(_fsapp.LOG_PRIORITY_INFO, "<stdout>")
    sys.stderr = LogStream(_fsapp.LOG_PRIORITY_WARN, "<stderr>")
    root = logging.getLogger()
    root.addHandler(LogHandler())
    root.setLevel(logging.INFO)


def print_test_traceback_maybe() -> None:
    """A test hook: with FSAPP_TEST_PYTHON_TRACEBACK=1, prints a traceback to sys.stderr, as
    an unhandled exception's would be (the log test checks that it is in the log)."""
    if os.environ.get("FSAPP_TEST_PYTHON_TRACEBACK") != "1":
        return
    try:
        raise RuntimeError("A test traceback (FSAPP_TEST_PYTHON_TRACEBACK)")
    except RuntimeError:
        traceback.print_exc()
