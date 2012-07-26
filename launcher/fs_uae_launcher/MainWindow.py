from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import sys
import uuid
import fs_uae_launcher.fsui as fsui
import fs_uae_launcher.fs as fs
from .Amiga import Amiga
from .Config import Config
from .Settings import Settings
from .IRC import IRC
#from .FSUAE import FSUAE
#from .ConfigWriter import ConfigWriter
from .ConfigurationsGroup import ConfigurationsGroup
from .Netplay import Netplay
from .Database import Database
from .GameHandler import GameHandler
from .Separator import Separator
from .I18N import _, ngettext

def set_min_height(button):
    #bottom_button_height = 32
    #if fs.linux:
    #    button.set_min_height(bottom_button_height)
    pass

class MainWindow(fsui.Window):

    def __init__(self, icon):
        fsui.Window.__init__(self, None, "FS-UAE Launcher")
        #self.set_size((1000, 600))
        if icon:
            self.set_icon_from_path(icon)

        self.layout = fsui.VerticalLayout()
        hlayout = fsui.HorizontalLayout()
        self.layout.add(hlayout, expand=True, fill=True)

        self.left_side = fsui.Panel(self)

        self.create_left_side(self.left_side)
        hlayout.add(self.left_side, expand=0, fill=True)

        #self.layout.add_spacer(20)
        self.right_side = fsui.Panel(self)
        self.create_right_side(self.right_side)
        hlayout.add(self.right_side, expand=2.5, fill=True)
        hlayout.add_spacer(20)

        if self.is_editor_enabled():
            from .editor.EditorGroup import EditorGroup
            self.editor = EditorGroup(self)
            hlayout.add(self.editor, expand=5.0, fill=True)
            hlayout.add_spacer(20)

        if self.is_editor_enabled():
            #self.layout.add_spacer(20)
            layout = fsui.HorizontalLayout()
            layout.padding_bottom = 20
            layout.padding_left = 20
            layout.padding_right = 20
            self.layout.add(layout, fill=True, expand=False)

            from .ScreenshotsPanel import ScreenshotsPanel
            self.screenshots_panel = ScreenshotsPanel(self)
            self.screenshots_panel.set_min_screenshots(6)
            layout.add(self.screenshots_panel, fill=True, expand=True)

        self.layout.set_size(self.get_size())
        self.layout.update()

        self.init_net_play()

        self.configurations_group.text_field.focus()
        was_maximized = Settings.get("maximized") == "1"

        self.set_size(self.layout.get_min_size())
        fsui.call_later(500, self.on_timer)

        #IRC.add_listener(self)
        #self.irc.start()
        self.center_on_screen()
        if was_maximized:
            self.maximize()

    def is_editor_enabled(self):
        return '--editor' in sys.argv

    def on_resize(self):
        print("on_resize, size =", self.get_size(), self.is_maximized())
        if self.is_maximized():
            Settings.set("maximized", "1")
        else:
            Settings.set("maximized", "0")
        fsui.Window.on_resize(self)

    def init_net_play(self):
        #from fs_uae_launcher.IRCHandler import IRCHandler
        #self.irc = IRCHandler(self)
        #IRC.irc = self.irc # self.lobby_panel.irc
        #self.irc.lobby = self.lobby_panel
        #self.irc.game = self.game_panel
        #self.irc.start()
        pass

    def separator(self, parent):
        parent.layout.add(Separator(parent), fill=True)
        #parent.layout.add_spacer(20)

    def create_left_side(self, parent):
        from .ConfigGroup import ConfigGroup
        self.config_group = ConfigGroup(parent)

        from .RemovableMediaGroup import RemovableMediaGroup
        self.removable_media_group = RemovableMediaGroup(parent, 2)

        from .configui.InputGroup import InputGroup
        self.input_group = InputGroup(parent)

        from .configui.ModelGroup import ModelGroup
        self.model_group = ModelGroup(parent)

        parent.layout = fsui.VerticalLayout()
        parent.layout.add_spacer(528, 20)
        parent.layout.add(self.model_group, fill=True)
        self.separator(parent)
        parent.layout.add(self.removable_media_group, fill=True)
        self.separator(parent)
        parent.layout.add(self.input_group, fill=True)
        self.separator(parent)
        parent.layout.add(self.config_group, fill=True)
        #parent.layout.add_spacer(20)
        #self.separator(parent)

        parent.layout.add_spacer(20)
        #parent.layout.add_spacer(20, expand=1)
        parent.layout.add_spacer(0, expand=1)

        layout = fsui.HorizontalLayout()
        parent.layout.add(layout, fill=True)

        layout.add_spacer(20)

        self.settings_button = fsui.Button(parent, _("Settings"))
        #button.disable()
        #set_min_height(button)
        self.settings_button.on_activate = self.on_settings_button
        layout.add(self.settings_button)

        layout.add_spacer(10)

        layout.add_spacer(0, expand=True)

        #self.join_game_button = fsui.Button(parent, "   Join Game   ")
        #layout.add(self.join_game_button)
        #layout.add_spacer(10)
        #self.create_game_button = fsui.Button(parent, "   Create Game   ")
        #layout.add(self.create_game_button)
        #layout.add_spacer(10, expand=True)

        self.netplay_button = fsui.Button(parent, _("Net Play"))
        self.netplay_button.disable()
        set_min_height(self.netplay_button)
        layout.add(self.netplay_button)
        layout.add_spacer(10)

        #button = fsui.Button(parent, "Options")
        #button.disable()
        #set_min_height(button)
        #layout.add(button)
        #layout.add_spacer(10)

        window_modes = [_("Window"), _("Fullscreen")]
        self.window_mode_choice = fsui.Choice(parent, window_modes)
        if Settings.get("fullscreen") == "1":
            self.window_mode_choice.set_index(1)
        self.window_mode_choice.on_change = self.on_window_mode_change
        layout.add(self.window_mode_choice)

        layout.add_spacer(10)

        self.start_button = fsui.Button(parent, _("Start"))
        set_min_height(self.start_button)
        self.start_button.on_activate = self.on_start_button
        layout.add(self.start_button)

        layout.add_spacer(20)

        parent.layout.add_spacer(20)

        if fsui.get_screen_size()[1] >= 768:
            hor_layout = fsui.HorizontalLayout()
            hor_layout.padding_left = 20
            hor_layout.padding_right = 20
            parent.layout.add(hor_layout, fill=True)
            # add bottom panel with game information
            from .GameInfoPanel import GameInfoPanel
            self.game_panel = GameInfoPanel(parent)
            hor_layout.add(self.game_panel, fill=True, expand=True)
            parent.layout.add_spacer(20)


    def create_right_side(self, parent):
        parent.layout = fsui.VerticalLayout()
        right_width = 460
        extra_screen_width = 222 + 5 + 1
        need_width = 1280
        if self.is_editor_enabled():
            need_width += extra_screen_width
        else:
            if fsui.get_screen_size()[0] >= need_width:
                # make room for one more screenshot
                right_width += extra_screen_width
        parent.layout.add_spacer(right_width, 20)

        self.configurations_group = ConfigurationsGroup(parent)
        parent.layout.add(self.configurations_group, fill=True)

        parent.layout.add_spacer(14)

        #layout2 = fsui.HorizontalLayout()
        #layout2.add_spacer(20)

        from .ConfigurationsBrowser import ConfigurationsBrowser
        self.configurations_browser = ConfigurationsBrowser(parent)
        #layout2.add(self.configurations_browser, fill=True, expand=True)
        #parent.layout.add(layout2, fill=True, expand=True)
        parent.layout.add(self.configurations_browser, fill=True, expand=True)

        if not self.is_editor_enabled():
            parent.layout.add_spacer(20)
            from .ScreenshotsPanel import ScreenshotsPanel
            self.screenshots_panel = ScreenshotsPanel(parent)
            parent.layout.add(self.screenshots_panel, fill=True, expand=False)

        parent.layout.add_spacer(20)

        """
        from .LobbyPanel import LobbyPanel
        self.lobby_panel = LobbyPanel(parent)
        parent.layout.add(self.lobby_panel, expand=True, fill=True)

        parent.layout.add_spacer(20)

        from .GamePanel import GamePanel
        self.game_panel = GamePanel(parent)
        parent.layout.add(self.game_panel, expand=True, fill=True)

        parent.layout.add_spacer(20)

        layout = fsui.HorizontalLayout()
        parent.layout.add(layout, fill=True)

        layout.add_spacer(20, expand=1)

        self.ready_button = fsui.Button(parent, "   Ready   ")
        self.ready_button.on_activate = self.on_ready_button
        layout.add(self.ready_button)

        layout.add_spacer(10)

        self.leave_game_button = fsui.Button(parent, "   Leave Game   ")
        layout.add(self.leave_game_button)

        #layout.add_spacer(10)
        #self.join_game_button = fsui.Button(parent, "   Join Game   ")
        #layout.add(self.join_game_button)

        layout.add_spacer(10)

        self.create_join_button = fsui.Button(parent, "   Create/Join Game   ")
        self.create_join_button.on_activate = self.on_join_button
        layout.add(self.create_join_button)

        #layout.add_spacer(10)
        #self.create_game_button = fsui.Button(parent, "   Create Game   ")
        #layout.add(self.create_game_button)

        #layout.add_spacer(10)
        #self.start_button = fsui.Button(parent, "   Start Game   ")
        #layout.add(self.start_button)

        parent.layout.add_spacer(20)
        """

    def on_ready_button(self):
        Config.set("x_ready", "1")

    def on_join_button(self):
        from .JoinDialog import JoinDialog
        dialog = JoinDialog()
        result = dialog.show_modal()
        if result:
            name = dialog.get_game_name()
            IRC.irc.join(name)
            #print(name)
        dialog.destroy()

    def on_settings_button(self):
        from .settingsui.SettingsDialog import SettingsDialog
        dialog = SettingsDialog(self)
        dialog.show_modal()
        dialog.destroy()

    def on_window_mode_change(self):
        index = self.window_mode_choice.get_index()
        if index == 0:
            Settings.set("fullscreen", "0")
        else:
            Settings.set("fullscreen", "1")
        #if index == 2:
        #    Config.set("fullscreen_mode", "window")
        #else:
        #    Config.set("fullscreen_mode", "fullscreen")

    def on_start_button(self):
        # check if local or online game here
        net_play = False

        if net_play: # net play
            Config.set("x_ready", "1")
            players = []
            if not Netplay.check_config("x_ready", players):
                message = u"The following players are not ready: " + \
                        repr(players)
                Netplay.notice(message)
                return
            # all players were ready
            Netplay.config_version = str(uuid.uuid4())
            message = "__check {0} {1}".format(Netplay.config_version,
                    Config.checksum())
            Netplay.message(message)
        else:
            self.start_local_game()

    def start_local_game(self):
        if not Config.get("kickstart_file"):# or not \
                #os.path.exists(Config.get("kickstart_file")):
            fsui.show_error(_("No kickstart found for this model. " +
                    "Try 'scan' function."))
            return
        cs = Amiga.get_model_config(Config.get("amiga_model"))["ext_roms"]
        if len(cs) > 0:
            # extended kickstart ROM is needed
            if not Config.get("kickstart_ext_file"):
                fsui.show_error(_("No extended kickstart found for this "
                        "model. Try 'scan' function."))
                return

        from .LaunchHandler import LaunchHandler
        handler = LaunchHandler(Settings.get("config_name"), Config,
                GameHandler.current())
        from .LaunchDialog import LaunchDialog
        dialog = LaunchDialog(handler)
        dialog.run()
        dialog.show_modal()
        dialog.destroy()

    def on_destroy(self):
        print("MainWindow.destroy")
        #from .DeviceManager import DeviceManager
        #DeviceManager.stop()
        #self.irc.stop()

    def on_timer(self):
        fsui.call_later(500, self.on_timer)
        #print("timer")
        if Netplay.config_version:
            for player in Netplay.players.values():
                if player.config_version != Netplay.config_version:
                    break
            else:
                # everyone has acked!
                self.create_server()

    def create_server(self):
        pass
