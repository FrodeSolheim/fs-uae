# _fsapp_surface.pyi
"""Typing stubs for the `_fsapp_surface` C extension (fsapp/fsapp-surface.h).

A surface's setters (and resize) change its client state, which goes to the compositor with the
record of the UI frame they are called in, together with everything else the UI frame changed. What
is drawn in a surface's buffer shows only once the surface is committed, and a new size with the
content committed for it. Call them on the UI thread, inside a UI frame.
"""

from collections.abc import Sequence

def create(size: tuple[int, int], special: int | None = None) -> object: ...
def commit(surface: object, damage: Sequence[tuple[int, int, int, int]] | None = None) -> None:
    """Commits what was drawn in the surface inside the damaged rectangles ((x, y, width, height)
    in surface pixels), or in the whole surface with none (or an empty sequence): it shows with the
    UI frame's record. What is drawn outside the damage shows only when a later commit's damage
    covers it. The rectangles are clipped to the surface and empty ones dropped; several commits in
    one UI frame are combined, many rectangles merged into their bounding box. A surface without a
    buffer has nothing to commit."""

def resize(surface: object, size: tuple[int, int]) -> None:
    """Resizes the surface. A surface with a buffer gets a new one at once (draw it all again), and
    its new size shows with its next commit, which counts as damage to the whole surface (a commit
    before the resize in the same UI frame is dropped); until then its old content shows at the old
    size. A surface without a buffer changes size with the UI frame's record."""

def destroy(surface: object) -> None:
    """Destroys the surface: its buffer is freed at once, and it disappears with the UI frame's
    record, together with the UI frame's other changes. Freeing the capsule does the same."""
