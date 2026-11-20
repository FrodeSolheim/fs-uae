import heapq
import itertools
import logging
from typing import Callable, ClassVar, Self

import _fsapp

logger = logging.getLogger(__name__)

TimerCallback = Callable[[], None]


class Timer:
    """A function that the timer service calls once, at a time on the clock, unless cancelled."""

    def __init__(self, time_us: int, callback: TimerCallback) -> None:
        self._time_us = time_us
        self._callback = callback
        self._cancelled = False

    @property
    def time_us(self) -> int:
        """When the timer is due, on the clock (microseconds)."""
        return self._time_us

    @property
    def cancelled(self) -> bool:
        return self._cancelled

    def cancel(self) -> None:
        """Cancels the timer, so its function is not called (if it has not been already)."""
        self._cancelled = True

    def fire(self) -> None:
        """Calls the timer's function, unless the timer is cancelled, and only once (cancelling it
        afterwards changes nothing). An exception in the function is logged."""
        if self._cancelled:
            return
        self._cancelled = True
        try:
            self._callback()
        except Exception:
            logger.exception("Exception in timer function %r", self._callback)


class TimerService:
    """Calls functions on the UI thread after a delay or at a time on the clock.

    The clock is the monotonic clock that all of FS-UAE reads, SDL's, in microseconds (`now_us`).
    The UI thread sleeps between UI frames until something wakes it or the next timer is due; due
    timers run at the start of the UI frame they wake, after the file dialogs' results and before
    the events, in the order of their times, and again after the UI frame's work, so that a timer
    that becomes due meanwhile (one due at once, set by an event handler) runs in the same UI
    frame. A timer set
    while timers run waits for the next run (at the end of the UI frame, or the next UI frame). An
    exception in a timer's function is logged, and the other timers still run.
    """

    _instance: ClassVar[Self | None] = None

    @classmethod
    def get(cls) -> Self:
        if cls._instance is None:
            cls._instance = cls()
        return cls._instance

    def __init__(self) -> None:
        # The timers by time, then by the order they were set in (the counter), so that the
        # timers themselves are never compared. Cancelled timers stay until they come first.
        self._timers: list[tuple[int, int, Timer]] = []
        self._counter = itertools.count()

    @staticmethod
    def now_us() -> int:
        """The time now on the clock (microseconds)."""
        return _fsapp.get_monotonic_us()

    def call_at(self, time_us: int, callback: TimerCallback) -> Timer:
        """Calls callback at time_us on the clock (at once, in the next UI frame, when passed)."""
        timer = Timer(time_us, callback)
        heapq.heappush(self._timers, (time_us, next(self._counter), timer))
        return timer

    def call_after(self, delay: float, callback: TimerCallback) -> Timer:
        """Calls callback after delay seconds."""
        return self.call_at(self.now_us() + int(delay * 1_000_000), callback)

    def run_due(self) -> None:
        """Runs the timers that are due now (fsapp does, at the start and the end of each UI
        frame's work)."""
        now_us = self.now_us()
        due: list[Timer] = []
        while self._timers and self._timers[0][0] <= now_us:
            due.append(heapq.heappop(self._timers)[2])
        for timer in due:
            timer.fire()

    def get_next_time_us(self) -> int | None:
        """The time of the next timer that is not cancelled, or None."""
        while self._timers and self._timers[0][2].cancelled:
            heapq.heappop(self._timers)
        return self._timers[0][0] if self._timers else None
