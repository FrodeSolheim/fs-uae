"""The surface API: the public, versioned API of fsapp's surfaces, for fsgui and plugins.

A client asks for the version it is written for, and gets that version's behaviour from every later
FS-UAE: api = surface_api(1). The version's classes, enums and events are in its module
(fsapp.surfaceapi.v1). Everything else in fsapp is private.
"""

from . import v1

__all__ = ["surface_api"]


def surface_api(version: int) -> v1.SurfaceAPI:
    """A new API object of the given version, which must be 1.

    Raises:
        ValueError: For another version.
    """
    if version != 1:
        raise ValueError(f"surface API version {version} does not exist (only 1 does)")
    return v1.SurfaceAPI()
