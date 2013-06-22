from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import hashlib
#import fsui as fsui
from fsgs.Archive import Archive
from fsgs.amiga.ROMManager import ROMManager


class ChecksumTool():

    def __init__(self, parent=None):
        self.parent = parent
        #fsui.Window.__init__(self, parent, "Checksumming")
        #self.layout = fsui.VerticalLayout()
        #
        #label = fsui.HeadingLabel(self, "Checksumming file...")
        #self.layout.add(label, fill=True)
        #self.layout.add_spacer(6)
        ##self.center_on_parent()

    def checksum(self, path):
        print("checksum", repr(path))
        archive = Archive(path)
        s = hashlib.sha1()
        f = archive.open(path)
        while True:
            data = f.read(65536)
            if not data:
                break
            s.update(data)
        return s.hexdigest()

    def checksum_rom(self, path):
        print("checksum_rom", repr(path))
        archive = Archive(path)
        return ROMManager.decrypt_archive_rom(archive, path)["sha1"]
