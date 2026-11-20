#!/usr/bin/env python3
"""Dist smoke test: checks that a Linux or macOS dist starts.

Usage: smoke.py [--out DIR] [--timeout SECONDS] [--exit-timeout SECONDS] [--origin ORIGIN]
                <archive or dist directory> <triplet>

For example, for the archive the dist entry script made (see common.sh for the directories):

  python3 od-fs/build/dist/smoke.py \
      .build/release/<version>/FS-UAE-<version>-x86_64-linux-gnu.tar.xz x86_64-linux-gnu
  python3 od-fs/build/dist/smoke.py \
      .build/release/<version>/FS-UAE-<version>-arm64-apple-darwin.dmg arm64-apple-darwin

Unpacks the dist archive (.tar.xz), copies what is on a macOS .dmg (mounted read-only with hdiutil,
see dmg.py), or copies the dist directory, into a new directory, sets it up as a portable directory
(Extras/Portable.ini copied to the top, next to FS-UAE.app on macOS), and starts FS-UAE there
without arguments (FS-UAE.app/Contents/MacOS/FS-UAE on macOS), headless where it can be: on Linux
with SDL's offscreen video driver (EGL and OpenGL with Mesa llvmpipe), on macOS with its normal
driver, since the offscreen driver has no OpenGL there (a window shows while it runs); with the
dummy audio driver, and a home directory of its own, so that it finds no Kickstart ROM and uses
the built-in AROS ROM. After the last GL grab (GL_GRAB_FRAMES; --timeout is the time limit for
getting there, 120 s by default), it sends SIGTERM. The test passes when FS-UAE ran until then and
exited with code 0 after SIGTERM; the log has no Python traceback or other error
(ERROR_LOG_MARKERS), shows the portable directory's cache directory, the Python home in the dist
(Libs/<triplet>, or the app's Contents/Resources/Python) and the standard library from there on
sys.path, and no Kickstart ROM loaded; the log file in the dist (LOG_FILE) starts with the build
header, with the dist's triplet and an origin (--origin, if given); and at least one GL grab is not
blank. Otherwise it exits with 1 and a list of what is wrong.

The unpacked dist, its home directory (home), the log (run.log) and the GL grabs go to the --out
directory (what an earlier run left there is removed first). Without --out, they go to a new
temporary directory, which is removed when the test passes and kept for inspection when it fails.

Needs only the archive, Python 3.12+ (the standard library) and what a dist needs from the system;
no Kickstart ROMs or other data. On Ubuntu 24.04, that is python3, libgl1, libegl1 (with Mesa's EGL
and llvmpipe) and libasound2t64 on top of the base system; on macOS, a logged-in desktop session
(GitHub's macOS runners have one) for the window. Run it on a machine that runs the dist's
architecture (an x86_64 macOS dist also under Rosetta). The dist check (check.py) checks the
archive without running it; the dist entry script does not run this test.
"""

import argparse
import ast
import collections
import os
import re
import shutil
import signal
import subprocess
import sys
import tarfile
import tempfile
import time
import zlib
from pathlib import Path

import dmg
from png_reader import read_png

# The executable in a Linux dist and in a macOS dist, and the Portable.ini to copy to the top.
EXECUTABLE = "FS-UAE"
MACOS_EXECUTABLE = "FS-UAE.app/Contents/MacOS/FS-UAE"
PORTABLE_INI = "Extras/Portable.ini"
# The Python home in a macOS dist (on Linux, Libs/<triplet>).
MACOS_PYTHON_HOME = "FS-UAE.app/Contents/Resources/Python"

# The GL grabs to request (buffer swap numbers, see FSUAE_GL_GRAB_FRAMES in fsapp-mainthread.cpp).
GL_GRAB_FRAMES = [300, 600, 1000]

# The part of a GL grab (the whole window) where the Amiga screen is, as fractions of the width and
# height: below the title bar and inside the border. A grab is blank when at most
# BLANK_MAX_FRACTION of the pixels there differ from the most common colour, as when FS-UAE draws
# its window but the emulation shows nothing.
EMULATION_AREA = ((0.05, 0.95), (0.10, 0.95))
BLANK_MAX_FRACTION = 0.001

# FS-UAE's log file in a portable dist (open_log_file in fsuae-init.cpp), which has the log's
# lines with their prefix (LOG_PREFIX) as on stdout, and starts with the build header
# (fsuae-buildinfo.cpp): the version line, then indented "<name>: <value>" lines.
LOG_FILE = "Cache/Log/FS-UAE.log.txt"
BUILD_HEADER_START = "FS-UAE version "

# The XDG base directory variables, which are not passed on to FS-UAE (it gets its own HOME).
XDG_DIRECTORIES = ["XDG_DATA_HOME", "XDG_CONFIG_HOME", "XDG_CACHE_HOME", "XDG_STATE_HOME"]

# Text in log lines that show an error (a Python traceback is an error even when FS-UAE carries
# on). The dynamic loader's error names a library that is neither in the dist nor on the system;
# without a window (for example without EGL, which SDL's offscreen driver needs), there are no GL
# grabs. A crash shows in the exit code (a signal), not in the log.
ERROR_LOG_MARKERS = [
    "error while loading shared libraries",
    "Could not create window",
    "Traceback (most recent call last)",
    "Fatal Python error",
]


# What FS-UAE logs (with SDL_Log) starts with the log line's time, thread and level, as in
# "    12.345678 main   I " (printf and Python's print do not).
LOG_PREFIX = re.compile(r"^ *\d+\.\d{6} \S+ +[CEWIDVT] ")


class SmokeRun:
    """A smoke test run: its output directory (with the dist in dist), and what is wrong."""

    def __init__(self, out_dir: Path, triplet: str) -> None:
        self.out_dir = out_dir
        self.dist_dir = out_dir / "dist"
        self.log_path = out_dir / "run.log"
        self.triplet = triplet
        self.macos = is_macos(triplet)
        self.failures: list[str] = []

    @property
    def executable(self) -> str:
        """The executable to start, in the dist."""
        return MACOS_EXECUTABLE if self.macos else EXECUTABLE

    @property
    def python_home(self) -> str:
        """The bundled Python's home, in the dist."""
        return MACOS_PYTHON_HOME if self.macos else f"Libs/{self.triplet}"

    def fail(self, message: str) -> None:
        self.failures.append(message)

    def grab_path(self, frame: int) -> Path:
        return self.out_dir / f"GLGrab_{frame:05d}.png"

    def clean(self) -> None:
        """Removes what an earlier run left in the output directory (a reused --out)."""
        shutil.rmtree(self.dist_dir, ignore_errors=True)
        shutil.rmtree(self.out_dir / "home", ignore_errors=True)
        shutil.rmtree(self.out_dir / "volume", ignore_errors=True)
        self.log_path.unlink(missing_ok=True)
        for grab in self.out_dir.glob("GLGrab_*.png"):
            grab.unlink()


def is_macos(triplet: str) -> bool:
    """Whether the triplet is a macOS dist's (the others the smoke test runs are Linux dists)."""
    return triplet.endswith("-apple-darwin")


def set_up_dist(smoke: SmokeRun, path: Path) -> bool:
    """Unpacks the archive at path, copies what is on the .dmg, or copies the dist directory, and
    makes it a portable directory.

    Returns False (after reporting why) if it cannot be run.
    """
    if path.is_dir():
        shutil.copytree(path, smoke.dist_dir, symlinks=True)
    elif path.name.endswith(".dmg"):
        try:
            with dmg.attached(path, smoke.out_dir / "volume") as volume:
                copy_volume(volume, smoke.dist_dir)
        except dmg.DmgError as e:
            smoke.fail(f"Cannot open the .dmg ({e})")
            return False
    else:
        smoke.dist_dir.mkdir(parents=True)
        with tarfile.open(path) as tar:
            tar.extractall(smoke.dist_dir, filter="tar")
    ok = True
    for name in [smoke.executable, PORTABLE_INI]:
        if not (smoke.dist_dir / name).is_file():
            smoke.fail(f"{name} is missing")
            ok = False
    if ok:
        shutil.copy(smoke.dist_dir / PORTABLE_INI, smoke.dist_dir / "Portable.ini")
    return ok


def copy_volume(volume: Path, dist_dir: Path) -> None:
    """Copies what is on a .dmg's volume (the app, README.txt and Extras; not the link to
    /Applications or Finder's hidden files) to dist_dir, as a user drags the app off it."""
    dist_dir.mkdir(parents=True)
    for item in dmg.visible_items(volume):
        if item.is_dir():
            shutil.copytree(item, dist_dir / item.name, symlinks=True)
        else:
            shutil.copy2(item, dist_dir / item.name)


def run_fsuae(smoke: SmokeRun, timeout: float, exit_timeout: float) -> None:
    """Runs FS-UAE until the last GL grab, then sends SIGTERM; the output goes to run.log."""
    env = dict(os.environ)
    # Python must find its standard library in the dist by itself, and FS-UAE must not find
    # anything in the user's directories (it also looks for Kickstart ROMs there): it gets a home
    # directory of its own.
    for name in ["PYTHONHOME", "PYTHONPATH"] + XDG_DIRECTORIES:
        env.pop(name, None)
    home = smoke.out_dir / "home"
    home.mkdir()
    # SDL's offscreen driver has OpenGL only through EGL, which macOS does not have.
    env.pop("SDL_VIDEODRIVER", None)
    if not smoke.macos:
        env["SDL_VIDEODRIVER"] = "offscreen"
    env.update(
        {
            "HOME": str(home),
            "SDL_AUDIODRIVER": "dummy",
            "FSEMU_SCREENSHOTS_DIR": str(smoke.out_dir),
            "FSUAE_GL_GRAB_FRAMES": ",".join(str(frame) for frame in GL_GRAB_FRAMES),
        }
    )
    last_grab = smoke.grab_path(GL_GRAB_FRAMES[-1])
    with open(smoke.log_path, "wb") as log:
        try:
            process = subprocess.Popen(
                [str(smoke.dist_dir / smoke.executable)],
                cwd=smoke.dist_dir,
                env=env,
                stdin=subprocess.DEVNULL,
                stdout=log,
                stderr=subprocess.STDOUT,
            )
        except OSError as e:
            smoke.fail(f"FS-UAE could not be started: {e}")
            return
        deadline = time.monotonic() + timeout
        try:
            while not last_grab.exists() and process.poll() is None and time.monotonic() < deadline:
                time.sleep(0.2)
        except KeyboardInterrupt:
            # FS-UAE allows one instance at a time: one left running would block the next.
            process.kill()
            process.wait()
            raise
        exit_code = process.poll()
        if exit_code is not None:
            smoke.fail(f"FS-UAE {exit_description(exit_code)} at start")
            return
        process.send_signal(signal.SIGTERM)
        try:
            exit_code = process.wait(timeout=exit_timeout)
        except subprocess.TimeoutExpired:
            smoke.fail(f"FS-UAE did not exit within {exit_timeout:g} s after SIGTERM (killed)")
            process.kill()
            process.wait()
            return
    if exit_code != 0:
        smoke.fail(f"FS-UAE {exit_description(exit_code)} after SIGTERM")


def is_blank(width: int, height: int, rows: list[bytes]) -> bool:
    """True if the emulation area of a GL grab is (almost) all one colour."""
    (left, right), (top, bottom) = EMULATION_AREA
    x0, x1 = int(width * left), int(width * right)
    colours: collections.Counter[int] = collections.Counter()
    for row in rows[int(height * top) : int(height * bottom)]:
        colours.update(memoryview(row).cast("I")[x0:x1])
    total = colours.total()
    if total == 0:
        return True
    return total - colours.most_common(1)[0][1] <= total * BLANK_MAX_FRACTION


def check_grabs(smoke: SmokeRun) -> None:
    """Every GL grab must be written, and at least one must show something in the window."""
    shown = False
    for frame in GL_GRAB_FRAMES:
        path = smoke.grab_path(frame)
        if not path.exists():
            smoke.fail(f"GL grab {path.name} is missing")
            continue
        try:
            png = read_png(path)
            if not is_blank(png.width, png.height, png.rows):
                shown = True
        except (ValueError, zlib.error) as e:
            smoke.fail(f"GL grab {path.name} cannot be read: {e}")
    if not shown:
        smoke.fail("All GL grabs are blank (or missing): nothing is shown in the window")


def check_log(smoke: SmokeRun) -> None:
    # The lines as they are (a traceback's stack lines are indented), and without the log's prefix,
    # the indentation and trailing whitespace.
    raw_lines = smoke.log_path.read_text(errors="replace").splitlines()
    lines = [LOG_PREFIX.sub("", line).strip() for line in raw_lines]
    # The cache directory (init_data_and_cache_directories in fsuae-init.cpp) is in the portable
    # directory. Paths are compared resolved: macOS may log /var/... for /private/var/....
    cache_dir = real_path(smoke.dist_dir / "Cache")
    if not any(
        line.startswith("Cache dir: ") and real_path(line.removeprefix("Cache dir: ")) == cache_dir
        for line in lines
    ):
        smoke.fail("FS-UAE did not start in portable mode (its cache directory is not in the dist)")
    # The Python home (fsuae-path.cpp and init_python), and the standard library on sys.path (which
    # FS-UAE prints when Python is running).
    python_home = real_path(smoke.dist_dir / smoke.python_home)
    prefix = "Setting Python Home to "
    if not any(
        line.startswith(prefix) and real_path(line.removeprefix(prefix)) == python_home
        for line in lines
    ):
        smoke.fail(f"The log does not show the Python home in {smoke.python_home}")
    sys_path = [
        entry
        for line in lines
        if line.startswith("sys.path =")
        for entry in parse_sys_path(line.removeprefix("sys.path ="))
    ]
    if not any(real_path(entry).startswith(f"{python_home}/lib/python3") for entry in sys_path):
        smoke.fail(f"sys.path in the log does not have the standard library in {smoke.python_home}")
    # WinUAE's ROM loader (memory.cpp) logs a Kickstart ROM it knows; the built-in AROS ROM is not
    # logged like that.
    for line in lines:
        if "Known ROM '" in line:
            smoke.fail(f"FS-UAE loaded a Kickstart ROM, not the built-in AROS ROM: {line}")
            break
    for marker in ERROR_LOG_MARKERS:
        for i, line in enumerate(lines):
            if marker in line:
                smoke.fail(f"The log has an error: {line}{exception_after(raw_lines, i)}")
                break


def real_path(path: str | Path) -> str:
    """The path with links resolved (also where it does not exist), without a trailing slash."""
    return os.path.realpath(str(path).rstrip("/") or "/")


def parse_sys_path(text: str) -> list[str]:
    """The entries of sys.path as FS-UAE logs it (the list's repr), or none if it cannot be read."""
    try:
        value = ast.literal_eval(text.strip())
    except (ValueError, SyntaxError):
        return []
    return [entry for entry in value if isinstance(entry, str)] if isinstance(value, list) else []


def check_log_file(smoke: SmokeRun, origin: str | None) -> None:
    """FS-UAE.log.txt starts with the build header, which has the dist's triplet and an origin
    (origin, if given)."""
    path = smoke.dist_dir / LOG_FILE
    try:
        lines = [LOG_PREFIX.sub("", line) for line in path.read_text(errors="replace").splitlines()]
    except OSError as e:
        smoke.fail(f"FS-UAE did not write {LOG_FILE}: {e}")
        return
    if not lines or not lines[0].startswith(BUILD_HEADER_START):
        smoke.fail(f"{LOG_FILE} does not start with the build header ({BUILD_HEADER_START}...)")
        return
    fields: dict[str, str] = {}
    for line in lines[1:]:
        if not line.startswith("  "):
            break
        name, _, value = line.strip().partition(": ")
        fields[name] = value
    triplet = fields.get("Triplet")
    if triplet != smoke.triplet:
        smoke.fail(f"The build header in {LOG_FILE} has the triplet {triplet}, not {smoke.triplet}")
    found = fields.get("Origin")
    if not found:
        smoke.fail(f"The build header in {LOG_FILE} has no origin")
    elif origin is not None and found != origin:
        smoke.fail(f"The build header in {LOG_FILE} has the origin {found}, not {origin}")


def exception_after(lines: list[str], index: int) -> str:
    """The exception line of a Python traceback starting at lines[index], as " (...)"."""
    if "Traceback" not in lines[index]:
        return ""
    for line in lines[index + 1 :]:
        # The stack lines are indented; the exception line that ends the traceback is not.
        if line and not line[0].isspace():
            return f" ({line.strip()})"
    return ""


def exit_description(exit_code: int) -> str:
    """How a process ended, from its exit code (negative for a signal, as in subprocess)."""
    if exit_code < 0:
        try:
            name = signal.Signals(-exit_code).name
        except ValueError:
            name = f"signal {-exit_code}"
        return f"crashed ({name})"
    return f"exited with code {exit_code}"


def main() -> int:
    parser = argparse.ArgumentParser(description="Checks that a Linux or macOS dist starts.")
    parser.add_argument("--out", type=Path, help="directory for the unpacked dist, log and grabs")
    parser.add_argument("--timeout", type=float, default=120, help="seconds to the last GL grab")
    parser.add_argument("--exit-timeout", type=float, default=20, help="seconds after SIGTERM")
    parser.add_argument("--origin", help="the origin the build header must have (default: any)")
    parser.add_argument(
        "path", type=Path, help="dist archive (.tar.xz, or .dmg on a Mac) or dist directory"
    )
    parser.add_argument("triplet", help="the triplet the dist is for, e.g. x86_64-linux-gnu")
    args = parser.parse_args()
    if "-linux-" not in args.triplet and not is_macos(args.triplet):
        parser.error(f"the smoke test is for Linux and macOS dists only, not {args.triplet}")
    if not args.path.exists():
        parser.error(f"no such archive or directory: {args.path}")
    if args.path.is_file() and args.path.name.endswith(".dmg") != is_macos(args.triplet):
        parser.error(
            f"a .dmg is a macOS dist, and a macOS dist is a .dmg or a directory: {args.path} is "
            f"not for {args.triplet}"
        )

    out_dir = (args.out or Path(tempfile.mkdtemp(prefix="fs-uae-dist-smoke-"))).resolve()
    out_dir.mkdir(parents=True, exist_ok=True)
    smoke = SmokeRun(out_dir, args.triplet)
    smoke.clean()
    if set_up_dist(smoke, args.path):
        run_fsuae(smoke, args.timeout, args.exit_timeout)
        check_log(smoke)
        check_log_file(smoke, args.origin)
        check_grabs(smoke)

    if smoke.failures:
        print(f"Smoke test failed for {args.path}:")
        for failure in smoke.failures:
            print(f"  {failure}")
        print(f"The dist, its log (run.log) and the GL grabs are in {out_dir}")
        return 1
    print(f"Smoke test passed for {args.path}")
    if args.out is None:
        shutil.rmtree(out_dir)
    return 0


if __name__ == "__main__":
    sys.exit(main())
