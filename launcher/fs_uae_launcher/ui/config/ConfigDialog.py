from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from ...I18N import _, ngettext
from ..PagedDialog import PagedDialog
from .CustomOptionsPage import CustomOptionsPage
from ..settings.CustomSettingsPage import CustomSettingsPage

class ConfigDialog(PagedDialog):

    CUSTOM_OPTIONS = 0

    def __init__(self, parent, index):
        PagedDialog.__init__(self, parent, _("Custom Configuration"))

        self.add_page(_("Custom Options"), CustomOptionsPage)
        self.add_page(_("Custom Settings"), CustomSettingsPage)

        self.list_view.set_index(index)
        self.set_size((900, 540))
        self.center_on_parent()

    @classmethod
    def run(cls, parent, index):
        dialog = cls(parent, index)
        dialog.show_modal()
        dialog.close()
