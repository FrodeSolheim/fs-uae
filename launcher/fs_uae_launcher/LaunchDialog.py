from __future__ import division, print_function
from __future__ import absolute_import, unicode_literals

import threading
import traceback
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .I18N import _, ngettext

class LaunchDialog(fsui.Dialog):

    def __init__(self, handler):
        fsui.Dialog.__init__(self, None, _("Launching FS-UAE"))
        self.layout = fsui.VerticalLayout()

        self.layout.add_spacer(400, 20)

        hor_layout = fsui.HorizontalLayout()
        self.layout.add(hor_layout, fill=True)

        hor_layout.padding_right = 20
        hor_layout.add_spacer(20)

        image = fsui.Image("fs_uae_launcher:res/fs_uae_group.png")
        self.image_view = fsui.ImageView(self, image)
        hor_layout.add(self.image_view, valign=0.0)
        hor_layout.add_spacer(20)

        ver_layout = fsui.VerticalLayout()
        hor_layout.add(ver_layout, fill=True)
        self.title_label = fsui.HeadingLabel(self, _("Launching FS-UAE"))
        ver_layout.add(self.title_label)

        ver_layout.add_spacer(6)
        self.sub_title_label = fsui.Label(self, _("Preparing..."))
        ver_layout.add(self.sub_title_label)


        self.layout.add_spacer(20)

        hor_layout = fsui.HorizontalLayout()
        self.layout.add(hor_layout, fill=True)

        hor_layout.add_spacer(20, expand=True)
        self.cancel_button = fsui.Button(self, _("Cancel"))
        self.cancel_button.on_activate = self.on_cancel_button
        hor_layout.add(self.cancel_button)
        hor_layout.add_spacer(20)

        self.layout.add_spacer(20)
        self.set_size(self.layout.get_min_size())

        self.closed = False
        self.handler = handler
        self.handler.on_progress = self.on_progress
        self.handler.on_complete = self.on_complete

    def complete(self):
        self.closed = True
        self.end_modal(0)

    def on_progress(self, progress):
        def function():
            self.sub_title_label.set_text(progress)
        fsui.call_after(function)

    def on_complete(self):
        def function():
            self.complete()
        fsui.call_after(function)

    def run(self):
        print("LaunchDialog.run")
        threading.Thread(target=self.handler_thread).start()

    def on_cancel_button(self):
        #self.handler.on_progress = None
        #self.handler.on_complete = None
        self.complete()

    def on_error(self, message):
        self.EndModal(1)
        fsui.show_error(message)

    def handler_thread(self):
        try:
            self._handler_thread()
        except Exception:
            traceback.print_exc()
            message = traceback.format_exc()
            def function():
                self.on_error(message)
            fsui.call_after(function)

    def _handler_thread(self):
        self.handler.run()

