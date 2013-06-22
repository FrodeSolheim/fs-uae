from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import time
import fsui as fsui
from ..Config import Config
from ..Database import Database
from ..Scanner import Scanner
from ..Settings import Settings
from ..I18N import _, ngettext
from .settings.ScanPathsGroup import ScanPathsGroup

TIMER_INTERVAL = 100

class ScanDialog(fsui.Dialog):

    @classmethod
    def refresh_game_database(cls, window):
        return cls(window, minimal=True, interactive=False,
                scan_for_files=False)

    def __init__(self, parent, minimal=False, interactive=True,
            scan_for_files=True):
        fsui.Dialog.__init__(self, parent, _("Scan"))
        self.layout = fsui.VerticalLayout()
        self.layout.padding_left = 10
        self.layout.padding_top = 10
        self.layout.padding_right = 10
        self.layout.padding_bottom = 10

        self.layout.add_spacer(640, 0)

        self.interactive = interactive
        self.scan_for_files = scan_for_files
        if Settings.get("database_feature") == "1":
            self.update_game_database = True
        else:
            self.update_game_database = False

        if not minimal:
            #self.layout.add_spacer(20)

            from .ScanKickstartGroup import ScanKickstartGroup
            self.scan_kickstart_group = ScanKickstartGroup(self)
            self.layout.add(self.scan_kickstart_group, fill=True)

            #self.layout.add_spacer(20)

            label = fsui.HeadingLabel(self,
                    _("Scan for Kickstarts, Files and Configurations"))
            self.layout.add(label, margin=10)

            self.scan_paths_group = ScanPathsGroup(self)
            self.layout.add(self.scan_paths_group, fill=True)

            #self.layout.add_spacer(20)

        from .ScanProgressGroup import ScanProgressGroup
        self.scan_progress_group = ScanProgressGroup(self)
        self.layout.add(self.scan_progress_group, fill=True)

        self.layout.add_spacer(20)
        #self.layout.add_spacer(20)

        hor_layout = fsui.HorizontalLayout()
        self.layout.add(hor_layout, fill=True)

        hor_layout.add_spacer(10, expand=True)
        if interactive:
            self.scan_button = fsui.Button(self, _("Scan"))
            self.scan_button.on_activate = self.on_scan_button
            hor_layout.add(self.scan_button)
            hor_layout.add_spacer(10)
        else:
            self.scan_button = None
        self.stop_button = fsui.Button(self, _("Abort"))
        self.stop_button.on_activate = self.on_stop_button
        hor_layout.add(self.stop_button)
        hor_layout.add_spacer(10)
        self.close_button = fsui.Button(self, _("Close"))
        self.close_button.on_activate = self.on_close_button
        hor_layout.add(self.close_button)
        hor_layout.add_spacer(10)

        self.layout.add_spacer(10)

        self.set_size(self.layout.get_min_size())
        self.center_on_parent()

        self.old_title = ""
        self.old_status = ""
        self.has_started_scan = False

        self.on_timer()
        fsui.call_later(TIMER_INTERVAL, self.on_timer)
        if not self.interactive:
            self.start_scan()

    def on_destroy(self):
        Scanner.stop_flag = True

    def set_scan_title(self, text):
        if not text:
            return
        if text == self.old_title:
            return
        self.old_title = text
        self.scan_progress_group.title_label.set_text(text)

    def set_scan_status(self, text):
        if not text:
            return
        if text == self.old_status:
            return
        self.old_status = text
        self.scan_progress_group.status_label.set_text(text)

    def on_timer(self):
        fsui.call_later(TIMER_INTERVAL, self.on_timer)

        if not Scanner.running:
            if self.has_started_scan:
                if Scanner.error:
                    self.set_scan_title(_("Scan error"))
                    self.set_scan_status(Scanner.error)
                else:
                    if not self.interactive:
                        self.end_modal(True)
                        return
                    self.set_scan_title(_("Scan complete"))
                    self.set_scan_status(
                            _("Click 'Scan' button if you want to re-scan"))
            else:
                self.set_scan_title(_("No scan in progress"))
                self.set_scan_status(_("Click 'Scan' button to start scan"))
            if self.scan_button is not None:
                self.scan_button.enable()
            self.stop_button.disable()
            self.close_button.enable()
            return

        status = Scanner.status
        self.set_scan_title(status[0])
        self.set_scan_status(status[1])

    #def on_rom_found(self, path, sha1):
    #    self.text.append_text("found {0}\n".format(path))

    def on_scan_button(self):
        self.start_scan()

    def start_scan(self):
        if self.scan_button is not None:
            self.scan_button.disable()
        self.has_started_scan = True
        self.set_scan_title(_("Starting scan"))
        self.set_scan_status(_("Please wait..."))
        paths = ScanPathsGroup.get_search_path()

        self.close_button.disable()
        self.stop_button.enable()

        Scanner.start(paths, scan_for_files=self.scan_for_files,
                update_game_database=self.update_game_database,
                purge_other_dirs=True)

    def on_close_button(self):
        self.end_modal(False)

    def on_stop_button(self):
        Scanner.stop_flag = True
        #self.close_button.enable()
