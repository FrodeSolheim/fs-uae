from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

#from .element import LightElement
from .spacer import Spacer

DEBUG = 0

class LayoutChild:
    def __init__(self):
        self.element = None
        self.spacing = 0
        self.expand = False
        self.fill = False
        self.valign = 0.5
        self.margin_left  = 0
        self.margin_right = 0
        self.margin_top = 0
        self.margin_bottom = 0

class Layout:

    def __init__(self, padding):
        #self.min_size = (0, 0)
        self.position = (0, 0)
        self.size = (0, 0)
        self.padding_left = padding
        self.padding_right = padding
        self.padding_top = padding
        self.padding_bottom = padding
        self._skip = 0
        #self.origin = (0, 0)
        self.children = []

    def is_visible(self):
        return True

    def set_padding(self, amount):
        self.padding_left = amount
        self.padding_right = amount
        self.padding_top = amount
        self.padding_bottom = amount

    def get_min_size(self):
        return (self.get_min_width(), self.get_min_height())

    def add(self, element, spacing=0, expand=False, fill=False, valign=0.5,
            margin=0, margin_left=None, margin_right=None,
            margin_top=None, margin_bottom=None):
        child = LayoutChild()
        child.element = element
        child.spacing = spacing
        child.expand = expand
        child.fill = fill
        child.valign = valign
        if margin_left is not None:
            child.margin_left = margin_left
        else:
            child.margin_left = margin
        if margin_right is not None:
            child.margin_right = margin_right
        else:
            child.margin_right = margin
        if margin_top is not None:
            child.margin_top = margin_top
        else:
            child.margin_top = margin
        if margin_bottom is not None:
            child.margin_bottom = margin_bottom
        else:
            child.margin_bottom = margin
        self.children.append(child)

    def remove(self, element):
        for i, child in enumerate(self.children):
            if child.element == element:
                del self.children[i]
                return

    def add_spacer(self, size, size2=None, expand=False):
        self.add(Spacer(size, size2), expand=expand)

    def get_position(self):
        return self.position

    def set_position(self, position):
        self.position = position
        #self.origin = position
        # FIXME: avoid calling update after both set_position and set_size
        self.update()

    def set_size(self, size):
        if DEBUG:
            print("Layout.set_size", size)
        self.size = size
        self.update()

    def set_position_and_size(self, position, size):
        self.position = position
        self.size = size
        self.update()

    def update(self):
        pass

class LinearLayout(Layout):

    def __init__(self, vertical, padding=0):
        Layout.__init__(self, padding)
        self.vertical = vertical

    def update(self):
        available = self.size[self.vertical]
        if self.vertical:
            available = available - self.padding_top - self.padding_bottom
        else:
            available = available - self.padding_left - self.padding_right
        if DEBUG:
            print("update, available =", available)
        expanding = 0

        last_margin = 0

        for child in self.children:
            if not child.element.is_visible():
                continue

            child.min_size = [child.element.get_min_width(),
                              child.element.get_min_height()]

            child.size = child.min_size[self.vertical]

            available -= child.size
            expanding += child.expand

            if self.vertical:
                child._skip = max(last_margin, child.margin_top)
                available-= child._skip
                last_margin = child.margin_bottom
                #available = available - child.margin_top - child.margin_bottom
                #available = available - last_margin + max()
                #last_margin = child.margin_bottom
            else:
                child._skip = max(last_margin, child.margin_left)
                available-= child._skip
                last_margin = child.margin_right
        available -= last_margin

        if DEBUG:
            print("available", available, "expanding", expanding)
        if available > 0 and expanding > 0:
            if DEBUG:
                print("distributing extra pixels:", available)
            available2 = available
            for child in self.children:
                extra = int(available2 * (child.expand / expanding))
                if DEBUG:
                    print(child.expand, expanding, extra)
                child.size += extra
                available -= extra
            # some more pixels could be available due to rounding
            if available > 0:
                #print("distributing extra pixels:", available)
                for child in self.children:
                    if child.expand:
                        child.size += 1
                        available -= 1
                        if available == 0:
                            break
        x = self.padding_left
        y = self.padding_top
        #self_height = self.size[1] - self.padding_top - self.padding_bottom
        fill_size = (self.size[0] - self.padding_left - self.padding_right,
                     self.size[1] - self.padding_top - self.padding_bottom)

        for child in self.children:
            if not child.element.is_visible():
                continue

            size = [child.min_size[0], child.min_size[1]]

            size[self.vertical] = child.size
            if child.fill:
                size[not self.vertical] = fill_size[not self.vertical]
                if self.vertical:
                    size[0] -= child.margin_left + child.margin_right
                else:
                    size[1] -= child.margin_top + child.margin_bottom

            if DEBUG:
                print(child.element, size)

            self_pos = self.get_position()
            position = [self_pos[0] + x, self_pos[1] + y]

            position[self.vertical] += child._skip
            if self.vertical:
                position[0] += child.margin_left
            else:
                position[1] += child.margin_top

            if not child.fill:
                # center child
                if self.vertical:
                    #position[0] += (self.size[0] - size[0]) // 2
                    pass
                else:
                    position[1] += (fill_size[1] \
                            - child.margin_top - child.margin_bottom \
                            - size[1]) * child.valign

            child.element.set_position_and_size(position, size)

            if self.vertical:
                y += size[1] + child._skip
            else:
                x += size[0] + child._skip

class HorizontalLayout(LinearLayout):

    def __init__(self, padding=0):
        LinearLayout.__init__(self, False, padding=padding)

    def get_min_width(self):
        min_width = 0
        last_margin = 0
        for child in self.children:
            min_width += child.element.get_min_width()
            min_width += max(last_margin, child.margin_left)
            last_margin = child.margin_right
        min_width += last_margin
        return min_width + self.padding_left + self.padding_right

    def get_min_height(self):
        min_height = 0
        for child in self.children:
            h = child.element.get_min_height()
            h += child.margin_top + child.margin_bottom
            if h > min_height:
                min_height = h
        return min_height + self.padding_top + self.padding_bottom

class VerticalLayout(LinearLayout):

    def __init__(self, padding=0):
        LinearLayout.__init__(self, True, padding=padding)

    def get_min_width(self):
        min_width = 0
        for child in self.children:
            w = child.element.get_min_width()
            w += child.margin_left + child.margin_right
            if w > min_width:
                min_width = w
        return min_width + self.padding_left + self.padding_right

    def get_min_height(self):
        min_height = 0
        last_margin = 0
        for child in self.children:
            min_height += child.element.get_min_height()
            min_height += max(last_margin, child.margin_top)
            last_margin = child.margin_bottom
        min_height += last_margin
        return min_height + self.padding_top + self.padding_bottom
