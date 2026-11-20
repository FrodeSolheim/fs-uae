"""Opening a macOS dist's .dmg, for the dist check (check.py) and the smoke test (smoke.py).

A .dmg can only be opened on a Mac, with hdiutil: attached() mounts it read-only (without showing
it in Finder) at a mount point of the caller's, and detaches it again afterwards. What the volume
must have is in VOLUME_ITEMS: what dmgbuild puts there (macos/settings.py).
"""

import contextlib
import shutil
import subprocess
from collections.abc import Iterator
from pathlib import Path

# The files at the top of the .dmg's volume: the app, and README.txt and Extras next to it, as in
# the other platforms' archives; Applications is a link to /Applications, for dragging the app
# there. Hidden files (Finder's .background.tiff, .DS_Store and the like) are not listed.
VOLUME_ITEMS = ["Extras", "FS-UAE.app", "README.txt"]
APPLICATIONS_LINK = "Applications"
APPLICATIONS_TARGET = "/Applications"
# The volume's icon, which dmgbuild makes from the app's icon (badge_icon in macos/settings.py).
VOLUME_ICON = ".VolumeIcon.icns"


class DmgError(Exception):
    pass


def visible_items(volume: Path) -> list[Path]:
    """What a user sees on the mounted volume and can copy: not the Applications link (which is for
    dragging the app onto) or hidden files."""
    return [
        item
        for item in sorted(volume.iterdir())
        if not item.name.startswith(".") and item.name != APPLICATIONS_LINK
    ]


def available() -> bool:
    """Whether .dmg files can be opened here (hdiutil, on a Mac)."""
    return shutil.which("hdiutil") is not None


@contextlib.contextmanager
def attached(image: Path, mountpoint: Path) -> Iterator[Path]:
    """Mounts the .dmg image read-only at mountpoint (made if missing) while in the context.

    Raises DmgError (with hdiutil's message) when it cannot be attached.
    """
    mountpoint.mkdir(parents=True, exist_ok=True)
    result = subprocess.run(
        [
            "hdiutil",
            "attach",
            "-nobrowse",
            "-readonly",
            "-noautoopen",
            "-mountpoint",
            str(mountpoint),
            str(image),
        ],
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        message = (result.stderr or result.stdout).strip()
        raise DmgError(f"hdiutil attach failed: {message}")
    try:
        yield mountpoint
    finally:
        detach(mountpoint)


def detach(mountpoint: Path) -> None:
    """Detaches the volume at mountpoint, by force if something still has a file open."""
    command = ["hdiutil", "detach", str(mountpoint)]
    if subprocess.run(command, capture_output=True).returncode != 0:
        subprocess.run(command[:2] + ["-force"] + command[2:], capture_output=True)
