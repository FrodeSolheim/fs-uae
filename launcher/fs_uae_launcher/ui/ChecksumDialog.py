from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import hashlib
import fsui as fsui
from ..I18N import _


class ChecksumDialog(fsui.Window):

    def __init__(self, parent, path):
        fsui.Window.__init__(self, parent, _("Checksumming"))
        self.layout = fsui.VerticalLayout()

        label = fsui.HeadingLabel(self, _("Checksumming file..."))
        self.layout.add(label, fill=True)
        self.layout.add_spacer(6)
        #self.center_on_parent()

    def checksum(self, path):
        #raise Exception(".")
        s = hashlib.sha1()
        with open(path, "rb") as f:
            data = f.read()
            while data:
                s.update(data)
                data = f.read()
        #aa = bb
        return s.hexdigest()
