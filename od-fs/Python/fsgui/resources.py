"""The directories fsgui finds its fonts and images in, which the program gives it at startup."""

from collections.abc import Sequence

_fonts_dirs: list[str] = []
_images_dirs: list[str] = []


def set_resource_dirs(*, fonts: Sequence[str], images: Sequence[str]) -> None:
    """Sets the directories to look for fonts and images in, in order of preference. The program
    calls this before fsgui's first window."""
    _fonts_dirs[:] = fonts
    _images_dirs[:] = images


def get_fonts_dirs() -> list[str]:
    return list(_fonts_dirs)


def get_images_dirs() -> list[str]:
    return list(_images_dirs)
