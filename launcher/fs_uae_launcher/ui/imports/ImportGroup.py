from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ...I18N import _, ngettext
from ...Signal import Signal
from .ImportDialog import ImportDialog

class ImportGroup(fsui.Group):

    AMIGA_FOREVER = 1

    def __init__(self, parent, type=0):
        fsui.Group.__init__(self, parent)
        self.type = type

        self.layout = fsui.VerticalLayout()
        if self.type == self.AMIGA_FOREVER:
            title = _("Import From Amiga Forever CD/DVD")
        else:
            title = _("Import Kickstarts and ROMs")
        label = fsui.HeadingLabel(self, title)
        self.layout.add(label, margin=10)

        icon_layout = fsui.HorizontalLayout()
        self.layout.add(icon_layout, fill=True)
        icon_layout.add_spacer(20)
        if self.type == self.AMIGA_FOREVER:
            image = fsui.Image("fs_uae_launcher:res/amiga_forever_group.png")
        else:
            image = fsui.Image("fs_uae_launcher:res/kickstart.png")
        self.image_view = fsui.ImageView(self, image)
        icon_layout.add(self.image_view, valign=0.0, margin=10)

        vert_layout = fsui.VerticalLayout()
        icon_layout.add(vert_layout, fill=True, expand=True)

        if self.type == self.AMIGA_FOREVER:
            text = _("If you own Amiga Forever, select the drive/folder "
                    "and click \"{0}\"".format(_("Import")))
        else:
            text = _("Select a folder containing Amiga kickstart files "
                    "and click \"{0}\"".format(_("Import")))
        label = fsui.Label(self, text)
        vert_layout.add(label, margin=10)

        hori_layout = fsui.HorizontalLayout()
        vert_layout.add(hori_layout, fill=True, margin=10)
        self.text_field = fsui.TextField(self, "", read_only=True)
        hori_layout.add(self.text_field, expand=True)#, expand=True, fill=True)
        self.browse_button = fsui.Button(self, _("Browse"))
        self.browse_button.on_activate = self.on_browse
        hori_layout.add(self.browse_button, margin_left=10)
        self.import_button = fsui.Button(self, _("Import"))
        self.import_button.on_activate = self.on_import
        self.import_button.disable()
        hori_layout.add(self.import_button, margin_left=10)

    def set_path(self, path):
        self.path = path
        self.text_field.set_text(path)
        self.import_button.enable()

    def on_browse(self):
        dialog = fsui.DirDialog(self.get_window(),
                _("Select Directory to Import From"))
        if dialog.show_modal():
            self.set_path(dialog.get_path())
        dialog.destroy()

    def on_import(self):
        dialog = ImportDialog(self.get_window(), self.path, self.type)
        dialog.show_modal()
        dialog.destroy()
        Signal.broadcast("scan_done")
