# LHA archive support for Python

The `lhafile` package is a python extension that handles .lha archives, with
an interfile similar to the zipfile module found in the standard library.

## Notice about the origin of this project

This project is an updated version of the project found at
http://trac.neotitans.net/wiki/lhafile - the original project is no longer
maintained.

Changes include:

- Support for Python 3.x.
- Support file notes (as found in Amiga .lha files).
- Support for archives with ISO-8859-1 filenames.
- Fixed a bug causing crashes in lzhlib.
