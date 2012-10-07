from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import traceback
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..Database import Database
from ..I18N import _, ngettext

class ConfigurationsBrowser(fsui.VerticalItemView):

    def __init__(self, parent):
        fsui.VerticalItemView.__init__(self, parent)
        self.items = []
        self.icon = fsui.Image("fs_uae_launcher:res/16/fsuae_config.png")

        Settings.add_listener(self)
        self.update_search()

    def on_destroy(self):
        Settings.remove_listener(self)

    def on_select_item(self, index):
        #index = self.get_index()
        #print(index)
        self.load_configuration(self.items[index][0])

    def on_activate_item(self, index):
        from ..LaunchHandler import LaunchHandler
        LaunchHandler.start_game()

    def on_setting(self, key, value):
        if key == "config_search":
            self.update_search()
        if key == "config_refresh":
            self.update_search()

    def set_items(self, items):
        self.items = items
        #self.set_item_count(len(self.items))
        self.update()

    def get_item_count(self):
        return len(self.items)

    def search_for_text(self, text):
        text = text.lower()
        start = 0
        stop = len(self.items) -1
        while start <= stop:
            if self.items[start][1].lower().startswith(text):
                return start
            mid = (start + stop) // 2
            midval = self.items[mid][1].lower()
            #print(start, stop, mid, midval, text)
            if midval > text:
                stop = mid
            elif not midval.startswith(text):
                start = mid + 1
        return None

    #def get_item_text(self, index):
    #    text =  u" " + self.items[index][1]
    #    text = text.replace(u"\nAmiga \u00b7 ", "\n")
    #    return text

    #def on_get_item_tooltip(self, row, column):
    #    return self.items[row][1]
    #    #text = text.replace(u"\nAmiga \u00b7 ", "\n")

    def update_search(self):
        self.search = Settings.get("config_search").strip().lower()
        print("search for", self.search)

        database = Database.get_instance()
        items = database.search_configurations(self.search)
        #print(items)
        self.set_items(items)
        #self.set_items([list(x) for x in items])

    def load_configuration(self, configuration_id):
        database = Database.get_instance()
        config_info = database.get_config(configuration_id)
        if config_info["data"]:
            Config.load_data(config_info["data"])
        else:
            Config.load_file(config_info["path"])

    def render_item (self, index, rd):
        # FIXME: remove direct dependency on wx from this method
        import wx
        dc = rd.dc
        dc.SetFont(self.GetFont())

        c = dc.GetTextForeground()
        old_foreground = wx.Colour(c.Red(),c.Green(),c.Blue())
        reset_color = False

        if rd.selected:
            flags = wx.CONTROL_SELECTED
            flags |= wx.CONTROL_FOCUSED
            wx.RendererNative_Get().DrawItemSelectionRect(self, dc,
                    rd.itemrect, flags=flags)
            #dc.SetTextForeground(wx.SystemSettings.GetColour(
            #        wx.SYS_COLOUR_HIGHLIGHTTEXT))
            dc.SetTextForeground(wx.WHITE)
            reset_color = True

        x, y, w, h = rd.itemrect
        bitmap = self.icon.bitmap
        if bitmap is not None and bitmap.Ok():
            dc.DrawBitmap(bitmap, x + 6, y + (h - 16) // 2, True)
            
            x += 16 + 6 + 6
            w -= 16 + 6 + 6

        primary = self.items[index][1]
        primary = primary.replace(u"\nAmiga \u00b7 ", "\n")
        secondary = ""
        SPLIT = u"\n"
        if SPLIT in primary:
            primary, secondary = primary.split(SPLIT, 1)
            primary = primary.rstrip()
            secondary = u"   " + secondary.lstrip()

        itemRect = wx.Rect(x, y, w, h)
        dw, dh = dc.GetTextExtent(primary)
        dc.DrawText(primary, x, y + (h - dh) // 2)
        x += dw
        w -= dw

        if secondary:
            if not rd.selected:
                dc.SetTextForeground(wx.Colour(0xa0, 0xa0, 0xa0))
            dc.DrawText(secondary, x, y + (h - dh) // 2)
            reset_color = True
        if reset_color:
            dc.SetTextForeground(old_foreground)
