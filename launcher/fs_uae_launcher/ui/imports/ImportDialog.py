from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ...I18N import _, ngettext
from .ImportTask import ImportTask

TIMER_INTERVAL = 100

class ImportDialog(fsui.Dialog):

    AMIGA_FOREVER = 1

    def __init__(self, parent, path, type):
        if type == self.AMIGA_FOREVER:
            title = _("Import From Amiga Forever CD/DVD")
        else:
            title = _("Import Kickstarts and ROMs")
        fsui.Dialog.__init__(self, parent, title)
        self.layout = fsui.VerticalLayout()
        self.layout.padding_left = 20
        self.layout.padding_top = 20
        self.layout.padding_right = 20
        self.layout.padding_bottom = 20

        self.text_area = fsui.TextArea(self, read_only=True)
        self.text_area.set_min_width(600)
        self.text_area.set_min_height(300)
        self.layout.add(self.text_area)

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True, margin_top=20)
        hori_layout.add_spacer(0, expand=True)
        self.close_button = fsui.Button(self, _("Close"))
        self.close_button.on_activate = self.on_close_button
        self.close_button.disable()
        hori_layout.add(self.close_button)

        self.line_count = 0
        self.task = ImportTask(path, type)
        self.task.start()

        self.set_size(self.layout.get_min_size())
        self.center_on_parent()
        fsui.call_later(TIMER_INTERVAL, self.on_timer)

    def on_timer(self):
        if self.task.done:
            self.close_button.enable()
        else:
            fsui.call_later(TIMER_INTERVAL, self.on_timer)
        lines = self.task.get_new_log_lines(self.line_count)
        for line in lines:
            self.text_area.append_text(line + "\n")
        self.line_count += len(lines)

    def on_close_button(self):
        self.end_modal(False)
