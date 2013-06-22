"""
The goals of this class is:
1. To be able to use ISO-8859-1 encoding instead of CP43, and
2. Also support zip files with UTF-8 encoding, and
2. To work with unicode strings on both Python 2 and 3.

Python 3 assumes strings must be converted to CP437 for non-ASCII characters.
This is why there'll be decoding/encoding from/to CP437 even for ISO-8859-1
and UTF-8 name lookup (with Python 3). Complications arise because Python
seems to detect UTF-8 in some zip files (based on a flag) and then
automatically convert to/from unicode.
"""

import zipfile

import fs_uae_launcher.six as six

class ZipFile(zipfile.ZipFile):

    def __init__(self, path, mode="r"):
        zipfile.ZipFile.__init__(self, path, mode)

    #def read(self, name):
    # read uses getinfo, so don't also "fix" read..
    #    name = six.text_type(name)
    #    try:
    #        return zipfile.ZipFile.read(self, name.encode("ISO-8859-1"))
    #    except Exception: # UnicodeEncodeError, KeyError
    #        return zipfile.ZipFile.read(self, name.encode("UTF-8"))

    def getinfo(self, name):
        if six.PY3:
            try:
                n = name.encode("ISO-8859-1").decode("CP437")
                return zipfile.ZipFile.getinfo(self, n)
            except Exception:
                pass
            try:
                n = name.encode("UTF-8").decode("CP437")
                return zipfile.ZipFile.getinfo(self, n)
            except Exception:
                pass
            return zipfile.ZipFile.getinfo(self, name)
        else:
            name = unicode(name)
            try:
                n = name.encode("ISO-8859-1")
                return zipfile.ZipFile.getinfo(self, n)
            except Exception as e: # UnicodeEncodeError, KeyError
                #print(repr(e))
                pass
            try:
                n = name.encode("UTF-8")
                return zipfile.ZipFile.getinfo(self, n)
            except Exception as e:
                #print(repr(e))
                pass
            #raise KeyError(name)
            return zipfile.ZipFile.getinfo(self, name)

    def namelist(self):
        names = zipfile.ZipFile.namelist(self)
        for i, name in enumerate(names):
            print("...", repr(name))
            if six.PY3:
                try:
                    name = name.encode("CP437").decode("UTF-8")
                except Exception:
                    try:
                        name = name.encode("CP437").decode("ISO-8859-1")
                    except Exception:
                        pass
            else:
                if isinstance(name, unicode):
                    # already converted from UTF-8
                    pass
                else:
                    try:
                        name = name.decode("UTF-8")
                    except UnicodeDecodeError:
                        name = name.decode("ISO-8859-1")
            names[i] = name
        return names
