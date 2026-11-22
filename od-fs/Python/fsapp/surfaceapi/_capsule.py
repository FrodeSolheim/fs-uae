"""A surface as the C side hands it to Python (private: for fsapp and the top)."""

from typing import NewType

# An _fsapp_surface capsule (fsapp_surface_t).
SurfaceCapsule = NewType("SurfaceCapsule", object)
