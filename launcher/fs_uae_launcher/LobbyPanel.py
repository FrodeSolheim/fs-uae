from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from .IRCPanel import IRCPanel
from .Netplay import Netplay

class LobbyPanel(IRCPanel):

    def __init__(self, parent):
        IRCPanel.__init__(self, parent)

        self.channel = Netplay.lobby

        #self.layout = fsui.VerticalLayout()

        """
        label = fsui.HeadingLabel(self, "Lobby (and Server Messages)")
        self.layout.add(label, fill=True)
        self.layout.add_spacer(6)

        self.main = fsui.Panel(self)
        self.main.layout = fsui.HorizontalLayout()

        self.output = fsui.TextArea(self.main)
        self.main.layout.add(self.output, expand=4, fill=True)

        self.main.layout.add_spacer(6)
        self.users = fsui.TextArea(self.main)
        self.main.layout.add(self.users, expand=1, fill=True)

        self.layout.add(self.main, expand=True, fill=True)

        self.layout.add_spacer(6)
        self.input = fsui.TextField(self)
        self.input.on_activate = self.on_input
        self.layout.add(self.input, fill=True)

        #self.browse_button = fsui.Button(self, "Browse")
        #self.layout.add(self.browse_button)
        #self.browse_button = fsui.Button(self, "Browse")
        #self.layout.add(self.browse_button)

        #self.channel = "#FS-UAE"
        """

    #def on_destroy(self):
    #    print("on_destroy")
    #    self.irc.stop()

    """
    def append_text(self, message):
        self.output.append_text(message + "\n")

    def on_input(self):
        message = self.input.get_text()
        self.input.set_text("")
        if message.startswith("/"):
            #message = message[1:]
            #parts = message.split(" ")
            #command = parts[0].lower()
            #args = parts[1:]
            #name = "command_" + command
            #try:
            #    method = getattr(self, name)
            #except AttributeError:
            #    self.append_text("Unknown command: " + command)
            #else:
            #    method(args)
            if not self.irc.handle_command_string(message):
                self.append_text("Unknown command: " + message)
        else:
            self.append_text(u"<{0}> {1}".format(IRC.my_nick, message))
            self.irc.privmsg(Netplay.lobby, message)
"""