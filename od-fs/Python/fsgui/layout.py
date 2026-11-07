from typing import Any

from fsgui.label import Widget
from fsgui.parentstack import ParentStack
from fsgui.widgetorlayout import WidgetOrLayout


class Layout(WidgetOrLayout):
    # expand = False
    # fill = True

    def __init__(self, *, parent: Any = None) -> None:
        if parent is None:
            parent = ParentStack.parent()
            auto_add = True
        else:
            auto_add = False

        super().__init__(parent)

        if isinstance(parent, Layout):
            # add_to_layout = parent
            parent.add(self)
        else:
            # add_to_layout = parent.layout
            if auto_add:
                if parent.layout is not None:
                    parent.layout.add(self)
                else:
                    parent.layout = self

        # if add_to_layout is not None:
        #     add_to_layout.add(self)

        # Layouts defaults to fill
        self._fill = True

        self.items: list[WidgetOrLayout] = []

    def add(self, item: WidgetOrLayout) -> None:
        # print("Layout.add", item)
        self.items.append(item)

    def calculate_min_height(self, width: int) -> int:
        height = 0
        for item in self.items:
            height += item.calculate_min_height(width)
        return height

    def calculate_min_width(self) -> int:
        width = 0
        for item in self.items:
            width += item.calculate_min_width()
        return width

    def destroy(self):
        # Widgets themselves are destroyed by the parent widget, so we only
        # need to make sure that the layout hierarchy clears all references to
        # child widgets and layouts.
        for item in self.items:
            if not isinstance(item, Widget):
                item.destroy()
        self.items.clear()
        # FIXME: Not sure if this is needed (edit: yes, for now at least)
        self.parent = None


class LinearLayout(Layout):
    def __init__(self, *, gap: int = 0, horizontal: bool):
        super().__init__()
        self.horizontal = horizontal
        # self.vertical = not horizontal
        self.gap = gap

    def calculate_min_height(self, width: int) -> int:
        height = 0
        for i, item in enumerate(self.items):
            if self.horizontal:
                height = max(height, item.calculate_min_height(width))
            else:
                if i > 0:
                    height += self.gap
                height += item.calculate_min_height(width)
        return height

    def calculate_min_width(self) -> int:
        width = 0
        for i, item in enumerate(self.items):
            if self.horizontal:
                if i > 0:
                    width += self.gap
                width += item.calculate_min_width()
            else:
                width = max(width, item.calculate_min_width())
        return width

    def layout_children(self) -> None:
        # FIXME: wrapping labels in a horizontal container will "fail"
        # because height is not recalculated after final width of child has
        # been determined. Also, layout's get_min_height is probably wrong in
        # this scenario for the same reason.

        horizontal = int(self.horizontal)
        vertical = int(not self.horizontal)
        layout_size = (self.width, self.height)
        # print("\nlayout_children layout_size =", layout_size)
        available = layout_size[vertical]

        # padding = self.padding
        # if self.vertical:
        #     available -= self.padding[0] + padding[2]
        # else:
        #     available -= self.padding[1] + padding[3]

        expanding = 0.0

        for i, item in enumerate(self.items):
            # if not item.visible:
            #     continue

            item_min_width = item.calculate_min_width()
            item.layout_data.min_size = (
                item_min_width,
                item.calculate_min_height(item_min_width),
            )
            item.layout_data.size = item.layout_data.min_size[vertical]

            available -= item.layout_data.size
            if i > 0:
                available -= self.gap
            expanding += item._expand

        # print(
        #     "layout size",
        #     layout_size,
        #     "available",
        #     available,
        #     "expanding",
        #     expanding,
        # )
        if available > 0 and expanding > 0:
            available2 = available
            for item in self.items:
                extra = int(available2 * (abs(item._expand) / expanding))
                # if DEBUG:
                #     print(child.expand, expanding, extra)
                item.layout_data.size += extra
                available -= extra

            # some more pixels could be available due to rounding
            while available > 0:
                # print("distributing extra pixels:", available)
                for item in self.items:
                    if abs(item._expand):
                        item.layout_data.size += 1
                        available -= 1
                        if available == 0:
                            break

        # if horizontal

        x = self.x
        y = self.y

        # fill_size = (
        #     self.width - self.padding[1] - self.padding[3],
        #     self.height - self.padding[0] - self.padding[1],
        # )

        for item in self.items:
            # if not item.visible:
            #     continue
            size = [item.layout_data.min_size[0], item.layout_data.min_size[1]]
            size[vertical] = item.layout_data.size

            if item._fill:
                size[horizontal] = layout_size[horizontal]
                # print("fill", item, size)

                # if vertical:
                #     size[0] -= child.margin_left + child.margin_right
                # else:
                #     size[1] -= child.margin_top + child.margin_bottom

            # from fsgui.label import Label
            # if isinstance(item, Label) and item.wrap:
            #     print("LABEL", size)
            # size[1] = item.calculate_min_height(size[0])
            # if isinstance(item, Label) and item.wrap:
            #     print("LABEL", size)

            # position[self.vertical] += child._skip
            # if self.vertical:
            #     position[0] += child.margin_left
            # else:
            #     position[1] += child.margin_top

            position = [x, y]

            if not item._fill:
                # Maybe center item
                # if vertical:
                #     # position[0] += (self.size[0] - size[0]) // 2
                #     pass
                # else:
                position[horizontal] += round(
                    (
                        layout_size[horizontal]
                        # - child.margin_top
                        # - child.margin_bottom
                        - size[horizontal]
                    )
                    * item._align[horizontal]
                )

            # print(x, y, position, item, size)
            item.move((position[0], position[1]))
            item.set_size_internal((size[0], size[1]))

            # FIXME: Could/should be moved to on_resize on item??+
            item.layout_children()

            if vertical:
                y += size[1] + self.gap  # + child._skip
            else:
                x += size[0] + self.gap  # + child._skip

        # for item in self.items:
        #     if self.horizontal:
        #         width = item.calculate_min_width()
        #         height = item.calculate_min_height(width)

        #         # FIXME: Currently only vertical center alignment
        #         dy = (self.height - height) // 2
        #     else:
        #         width = self.width  # FIXME??
        #         height = item.calculate_min_height(width)
        #         dy = 0

        #     item.move((x, y + dy))
        #     item.set_size_internal((width, height))
        #     # print("->", width, height)
        #     item.layout_children()

        #     if self.horizontal:
        #         x += width + self.gap
        #     else:
        #         y += height + self.gap


class HorizontalLayout(LinearLayout):
    def __init__(self, *, gap: int = 0):
        super().__init__(gap=gap, horizontal=True)


class VerticalLayout(LinearLayout):
    def __init__(self, *, gap: int = 0):
        super().__init__(gap=gap, horizontal=False)
