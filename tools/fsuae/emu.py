from .luashell import LuaShell, LuaShellError
from .drives import CDROMs, Floppies

class Emu:
    def __init__(self):
        self.sh = None
        self.connected = False
        self.floppies = None
        self.cdroms = None
        self.error = None

    def connect(self, host="localhost", port=6800):
        if self.connected:
            return
        try:
            self.sh = LuaShell(host, port)
            self.sh.connect()
            self.floppies = Floppies(self.sh)
            self.cdroms = CDROMs(self.sh)
            self.num_floppies = self.floppies.get_num_drives()
            self.num_cdroms = self.cdroms.get_num_drives()
            self.connected = True
            self.error = None
            return True
        except LuaShellError as e:
            self.error = "LuaShellError: " + e.msg
            return False

    def disconnect(self):
        if not self.connected:
            return
        try:
            self.sh.disconnect()
            self.sh = None
            self.floppies = None
            self.drives = None
            self.connected = False
            return True
        except LuaShellError as e:
            self.error = "LuaShellError: " + e.msg
            return False

    def isConnected(self):
        return self.connected

    def getError(self):
        return self.error

    def getNumFloppyDrives(self):
        return self.num_floppies

    def getNumCDROMDrives(self):
        return self.num_cdroms

    def getFloppyImagePath(self, slot):
        return self.floppies.get_image_file(slot)

    def setFloppyImagePath(self, slot, path):
        self.floppies.set_image_file(slot, path)

    def getCDROMImagePath(self, slot):
        return self.cdroms.get_image_file(slot)

    def setCDROMImagePath(self, slot, path):
        self.cdroms.set_image_file(slot, path)


if __name__ == '__main__':
    emu = Emu()
    if not emu.connect():
        print("ERROR", emu.getError())
    else:
        print("Drives", emu.getNumDrives())
        emu.disconnect()
