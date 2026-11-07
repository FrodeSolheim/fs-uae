from typing import Any, Self


class Item:
    """A class used to represent items in menus, list views, choice widgets, etc."""

    def __init__(self, text: str, data: Any = None) -> None:
        self.index: int = 0
        self.text = text
        self.enabled = True
        self.data = data

    # def disable(self) -> None:
    #     self.enabled = False

    def set_enabled(self, enabled: bool = True) -> Self:
        self.enabled = enabled
        return self
