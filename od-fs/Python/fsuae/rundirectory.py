"""Run directories: where they live, and deleting them again.

A run directory holds the generated media for one emulation session (for example the hard drive
for a WHDLoad archive). Run directories live in `Run/` in the cache directory, as
`Run/<name>-<random suffix>`. The current run directory is deleted when the session has ended,
after emulation has stopped.

Each run directory shows whether its owner (the FS-UAE process that created it) is still running:
it has a lock file (`LOCK_FILE_NAME`), which the owner holds locked with an OS file lock for as
long as the run directory exists (`fcntl.flock` on Linux and macOS, `msvcrt.locking` on Windows).
The operating system releases the lock when the process ends, also when it crashes. A run directory
whose lock can be taken is stale, and is removed at the next start (`remove_stale_run_dirs`). This
does not depend on the single instance lock: a run directory of another running FS-UAE is never
removed, and neither is one of this process.

Deleting never follows symbolic links (or Windows junctions) out of the directory being deleted:
a link is removed, not what it points to. Failures are logged, never raised.

No GUI or emulator dependencies, so it can be tested with a plain Python interpreter.
"""

import logging
import os
import shutil
import stat
import sys
import tempfile
import time
from typing import BinaryIO

logger = logging.getLogger(__name__)

RUN_DIR_NAME = "Run"

# The lock file in each run directory, locked by the process owning the run directory. Not on the
# emulated hard drive (that is a directory of its own in the run directory).
LOCK_FILE_NAME = "FS-UAE.lock"

# Entries in Run/ younger than this (in seconds) are never removed. A new run directory exists for
# a moment before its owner has locked its lock file, and must not be taken for a stale one then.
STALE_MIN_AGE = 60.0

# The open (and locked) lock files of the run directories this process owns, by path. Kept open
# until the run directory is deleted, or until the process ends.
_lock_files: dict[str, BinaryIO] = {}


def run_root(cache_dir: str) -> str:
    """The directory holding all run directories: `Run/` in the cache directory."""
    return os.path.join(cache_dir, RUN_DIR_NAME)


def create_run_dir(cache_dir: str, name: str) -> str:
    """Create a new run directory `Run/<name>-<random suffix>` and return its path.

    The run directory is empty apart from its lock file, which this process holds locked until the
    run directory is deleted with `delete_run_dir`, or the process ends.
    """
    root = run_root(cache_dir)
    os.makedirs(root, exist_ok=True)
    path = os.path.abspath(tempfile.mkdtemp(prefix=f"{name}-", dir=root))
    lock_file = open(os.path.join(path, LOCK_FILE_NAME), "wb")
    try:
        locked = _try_lock(lock_file.fileno())
    except OSError as e:
        locked = False
        logger.warning("Cannot lock %s: %s", lock_file.name, e)
    if not locked:
        # Nothing else knows this directory yet, so this should not happen. Without the lock,
        # the run directory looks stale to other FS-UAE processes once it is old enough.
        logger.warning("Run directory %s is not locked", path)
    _lock_files[path] = lock_file
    return path


def remove_stale_run_dirs(cache_dir: str, min_age: float = STALE_MIN_AGE) -> None:
    """Delete the stale run directories in `Run/` in the cache directory: those whose owner has
    ended (the lock file is not locked), and other entries (no lock file).

    Entries younger than `min_age` seconds are kept, as are run directories whose lock state
    cannot be determined. Failures are logged.
    """
    root = run_root(cache_dir)
    try:
        entries = sorted(os.listdir(root))
    except FileNotFoundError:
        return
    except OSError as e:
        logger.warning("Cannot list run directories in %s: %s", root, e)
        return
    now = time.time()
    for entry in entries:
        path = os.path.join(root, entry)
        try:
            if not _is_stale(path, now, min_age):
                continue
        except OSError as e:
            logger.warning("Cannot check run directory %s: %s", path, e)
            continue
        if delete_tree(path):
            logger.info("Removed stale run directory %s", path)


def _is_stale(path: str, now: float, min_age: float) -> bool:
    st = os.lstat(path)
    if now - st.st_mtime < min_age:
        return False
    if os.path.abspath(path) in _lock_files:
        # Our own. Not checked through the lock: where flock is emulated with POSIX record locks
        # (NFS), closing another descriptor of the file would release the process's own lock.
        return False
    if _is_link(path, st) or not stat.S_ISDIR(st.st_mode):
        return True
    try:
        lock_file = open(os.path.join(path, LOCK_FILE_NAME), "rb")
    except FileNotFoundError:
        # Not made by create_run_dir (an older FS-UAE, or a crash before the lock file was made).
        return True
    with lock_file:
        if not _try_lock(lock_file.fileno()):
            return False
        _unlock(lock_file.fileno())
    return True


if sys.platform == "win32":
    import msvcrt

    def _try_lock(fd: int) -> bool:
        """Lock the file without waiting. False if another process (or handle) has locked it."""
        os.lseek(fd, 0, os.SEEK_SET)
        try:
            msvcrt.locking(fd, msvcrt.LK_NBLCK, 1)
        except OSError:
            # A lock conflict is EACCES, but Wine reports it with errno 0. Any failure counts as
            # locked, so the run directory is kept.
            return False
        return True

    def _unlock(fd: int) -> None:
        os.lseek(fd, 0, os.SEEK_SET)
        msvcrt.locking(fd, msvcrt.LK_UNLCK, 1)

else:
    import fcntl

    def _try_lock(fd: int) -> bool:
        """Lock the file without waiting. False if another process (or open file) has locked it."""
        try:
            fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            return False
        return True

    def _unlock(fd: int) -> None:
        fcntl.flock(fd, fcntl.LOCK_UN)


def delete_run_dir(path: str) -> None:
    """Delete a run directory (on exit), releasing its lock first. Failures are logged."""
    lock_file = _lock_files.pop(os.path.abspath(path), None)
    if lock_file is not None:
        # Closing releases the lock. (On Windows, an open file cannot be deleted.)
        lock_file.close()
    if delete_tree(path):
        logger.info("Removed run directory %s", path)


def delete_tree(path: str) -> bool:
    """Delete a directory tree, file or link, without following links. Logs failures.

    Files and directories without write permission (the emulated Amiga can change protection
    bits) are made writable and the deletion is retried. Returns True if `path` is gone.
    """
    try:
        st = os.lstat(path)
    except FileNotFoundError:
        return True
    except OSError as e:
        logger.warning("Cannot delete %s: %s", path, e)
        return False

    if _is_link(path, st) or not stat.S_ISDIR(st.st_mode):
        try:
            _remove_entry(path, st)
        except OSError as e:
            logger.warning("Cannot delete %s: %s", path, e)
            return False
        return True

    errors: list[tuple[str, BaseException]] = []

    def on_error(_function: object, error_path: str, error: BaseException) -> None:
        errors.append((error_path, error))

    shutil.rmtree(path, onexc=on_error)
    if os.path.lexists(path):
        # Most likely missing permissions; make everything writable and try again.
        _make_writable(path)
        errors.clear()
        shutil.rmtree(path, onexc=on_error)
    if not os.path.lexists(path):
        return True
    for error_path, error in errors[:10]:
        logger.warning("Cannot delete %s: %s", error_path, error)
    if len(errors) > 10:
        logger.warning("... and %d more errors", len(errors) - 10)
    logger.warning("Could not delete %s completely", path)
    return False


def _is_link(path: str, st: os.stat_result) -> bool:
    if stat.S_ISLNK(st.st_mode):
        return True
    isjunction = getattr(os.path, "isjunction", None)  # Python 3.12+, only True on Windows
    return bool(isjunction and isjunction(path))


def _remove_entry(path: str, st: os.stat_result) -> None:
    """Remove a file or a link (not what it points to)."""
    try:
        os.unlink(path)
        return
    except IsADirectoryError:
        pass
    except PermissionError:
        # Windows: a read-only file, or a directory link/junction (removed with rmdir).
        if stat.S_ISLNK(st.st_mode) or stat.S_ISDIR(st.st_mode):
            pass
        else:
            os.chmod(path, stat.S_IMODE(st.st_mode) | stat.S_IWRITE)
            os.unlink(path)
            return
    os.rmdir(path)


def _make_writable(path: str) -> None:
    """Give the owner write access (and read/search access to directories) in the tree at `path`,
    without following links."""
    try:
        st = os.lstat(path)
        if _is_link(path, st):
            return
        if stat.S_ISDIR(st.st_mode):
            os.chmod(path, stat.S_IMODE(st.st_mode) | stat.S_IRWXU)
            with os.scandir(path) as entries:
                children = [entry.path for entry in entries]
            for child in children:
                _make_writable(child)
        else:
            os.chmod(path, stat.S_IMODE(st.st_mode) | stat.S_IWUSR)
    except OSError:
        pass
