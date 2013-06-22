from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from fsui import Panel, Color


class StatusElement(Panel):

    def __init__(self, parent):
        """

        :type parent: fsui.Widget
        """
        Panel.__init__(self, parent, paintable=True)
        #self.set_background_color(Color(0xd0, 0xd0, 0xd0))
        self.set_background_color(parent.get_background_color())
        self.icon = None
        self.right_icon = None
        self.right_icon_disabled = None
        self.text = ""
        self.active = True

    def on_paint(self):
        dc = self.create_dc()
        from .StatusBar import StatusBar
        StatusBar.draw_element_background(self, dc)
        self.paint_element(dc)

    def get_draw_operations(self):
        operations = []
        x = 0
        h = self.get_min_height()
        if self.icon:
            x += 6
            if self.active:
                image = self.icon
            else:
                image = self.icon.grey_scale()
            operations.append(("image", image, x, (h - 16) // 2))
            x += 16
        if self.text:
            x += 6
            tw, th = self.measure_text(self.text)
            #tw = 200
            #th = 20
            operations.append(("text", self.text, x, (h - th) // 2))
            x += tw
        if self.right_icon:
            x += 6
            if self.active:
                image = self.right_icon
            elif self.right_icon_disabled:
                image = self.right_icon_disabled
            else:
                image = self.right_icon.grey_scale()
            operations.append(("image", image, x, (h - 16) // 2))
            x += 16
        x += 6
        operations.append(("null", None, x, 0))
        return operations

    def get_min_height(self):
        return 28

    def get_min_width(self):
        operations = self.get_draw_operations()
        return operations[-1][2]

    def paint_element(self, dc):
        # orig_text_color = dc.get_text_color()
        if not self.active:
            dc.set_text_color(Color(0x80, 0x80, 0x80))
        operations = self.get_draw_operations()
        for type, object, x, y in operations:
            if type == "image":
                dc.draw_image(object, x, y)
            elif type == "text":
                # if not self.active:
                #     dc.set_text_color(Color(0x80, 0x80, 0x80))
                dc.draw_text(object, x, y)
                # if not self.active:
                #     dc.set_text_color(orig_text_color)
