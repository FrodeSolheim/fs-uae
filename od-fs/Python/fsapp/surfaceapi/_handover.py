"""Surfaces handed between the C side and version 1's handles (private and unstable: for the top,
such as the emulation surface's hand-over to the emulator window)."""

import _fsapp_surface

from ._capsule import SurfaceCapsule
from .v1 import Role, ScaleSupport, Surface, _surface_capsule


def adopt_child(capsule: SurfaceCapsule, parent: Surface, scale_support: ScaleSupport) -> Surface:
    """Makes a surface created elsewhere, without a buffer (its content is drawn by fsapp, such as
    the emulation surface's), a child of parent, and returns a version 1 handle of it. The handle
    takes its size as 0 x 0 until it is resized, and is hidden until it is shown."""
    _fsapp_surface.set_role(capsule, Role.CHILD, _surface_capsule(parent))
    return Surface._wrap(capsule, (0, 0), scale_support, False)


def capsule_of(surface: Surface) -> SurfaceCapsule:
    """The surface's capsule, for fsapp's private calls that take one (such as the file dialog's
    parent)."""
    return _surface_capsule(surface)
