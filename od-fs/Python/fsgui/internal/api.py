"""fsgui's own surface API object (fsapp.surfaceapi, version 1)."""

from fsapp.surfaceapi import surface_api
from fsapp.surfaceapi.v1 import SurfaceAPI

_api: SurfaceAPI | None = None


def get_api() -> SurfaceAPI:
    global _api
    if _api is None:
        _api = surface_api(1)
    return _api
