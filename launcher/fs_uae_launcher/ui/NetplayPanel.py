from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from ..Settings import Settings
from ..I18N import _, ngettext
from ..netplay.IRC import IRC
from ..netplay.Netplay import Netplay
from ..netplay.IRCBroadcaster import IRCBroadcaster
from .Skin import Skin

class NetplayPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)
        self.layout = fsui.VerticalLayout()

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True)
        self.layout.add_spacer(0)

        label = fsui.HeadingLabel(self, _("Net Play"))
        hori_layout.add(label, margin=10)

        #self.filters_label = fsui.Label(self, _("Filters:"))
        #hori_layout.add(self.filters_label,
        #        margin=10, margin_top=0, margin_bottom=0)

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True, expand=True)
        
        self.text_area = fsui.TextArea(self, font_family="monospace")
        hori_layout.add(self.text_area, fill=True, expand=True, margin=10,
                margin_right=0)

        #hori_layout.add_spacer(6)

        ver_layout = fsui.VerticalLayout()
        hori_layout.add(ver_layout, fill=True)

        self.channel_list = fsui.ListView(self)
        self.channel_list.set_min_width(200)
        self.channel_list.on_select_item = self.on_select_channel
        ver_layout.add(self.channel_list, fill=True, expand=True, margin=10)

        #ver_layout.add_spacer(6)

        self.nick_list = fsui.ListView(self)
        ver_layout.add(self.nick_list, fill=True, expand=True, margin=10)

        #self.layout.add_spacer(6)

        self.input_field = fsui.TextField(self)
        self.input_field.on_activate = self.on_input
        self.layout.add(self.input_field, fill=True, margin=10, margin_top=0)
        #self.layout.add_spacer(20)

        # FIXME: should not be hardcoded here
        self.active_channel = "#lobby"

        IRCBroadcaster.add_listener(self)

    def on_show(self):
        if not Netplay.is_connected():
            Netplay.connect()
        self.input_field.focus()

    def on_select_channel(self):
        index = self.channel_list.get_index()
        #if index == 0:
        #    channel = ""
        #else:
        channel = self.channel_list.get_item(index)
        IRC.set_active_channel(channel)
        self.input_field.focus()

    def on_input(self):
        command = self.input_field.get_text().strip()
        if not command:
            return
        if Netplay.handle_command(command):
            pass
        else:
            IRC.handle_command(command)
        self.input_field.set_text("")

    def set_active_channel(self, channel):
        if channel == self.active_channel:
            return
        self.text_area.set_text("")
        #self.text_area.append_text(IRC.channel(channel).get_text())
        ch = IRC.channel(channel)
        for i, line in enumerate(ch.lines):
            self.text_area.append_text(line, ch.colors[i])
        self.active_channel = channel
        self.update_nick_list()
        for i in range(self.channel_list.get_item_count()):
            if self.channel_list.get_item(i) == channel:
                self.channel_list.set_index(i)

    def update_channel_list(self):
        items = sorted(IRC.channels.keys())
        #items[0] = u"IRC ({0})".format(Settings.get_irc_server())
        #items[0] = Settings.get_irc_server()
        self.channel_list.set_items(items)

    def update_nick_list(self):
        items = IRC.channel(self.active_channel).get_nick_list()
        self.nick_list.set_items(items)

    def on_irc(self, key, args):
        if key == "active_channel":
            self.set_active_channel(args["channel"]) 
        elif key == "nick_list":
            if args["channel"] == self.active_channel:
                self.update_nick_list()
        elif key == "channel_list":
            self.update_channel_list()
        elif key == "message":
            if args["channel"] == self.active_channel:
                self.text_area.append_text(args["message"],
                        color=args["color"])
