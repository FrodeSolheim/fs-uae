class Drives:
    """binding for floppy and cdrom drives interface"""
    def __init__(self, lua_shell, drive_type):
        self._sh = lua_shell
        self._type = drive_type
        self._num_drives = 0
        self._images = []
        self._prefix = "fsuae." + self._type

    def set_shell(self, sh):
        self._sh = sh

    def _quote_lua_string(self, s):
        s = s.replace('"','\\"')
        return '"' + s + '"'

    def get_num_drives(self):
        """get the number of enabled drives"""
        cmd = "={}.get_num_drives()".format(self._prefix)
        res = self._sh.execute(cmd,1)
        return int(res[0])

    def get_image_file(self, slot):
        """get the attached image file"""
        cmd = "={}.get_file({})".format(self._prefix, slot)
        res = self._sh.execute(cmd,1)
        path = res[0]
        return path

    def set_image_file(self, slot, path):
        """set the image file"""
        cmd = "{}.set_file({},{})".format(self._prefix,
                    slot, self._quote_lua_string(path))
        self._sh.execute(cmd,0)

    def dump(self):
        """dump state"""
        n = self.get_num_drives()
        print(self._type,"#",n)
        for i in range(n):
            print(self.get_image_file(i))

class Floppies(Drives):
    def __init__(self, lua_shell):
        Drives.__init__(self, lua_shell, "floppy")

class CDROMs(Drives):
    def __init__(self, lua_shell):
        Drives.__init__(self, lua_shell, "cdrom")

# quick test
if __name__ == '__main__':
    from luashell import LuaShell
    sh = LuaShell()
    sh.connect()
    f = Floppies(sh)
    f.dump()
    c = CDROMs(sh)
    c.dump()
