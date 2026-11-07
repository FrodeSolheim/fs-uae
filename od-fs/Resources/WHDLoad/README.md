# WHDLoad

WHDLoad executables, one file per version, named after the version, starting with WHDLoad 18.5, the
FS-UAE Launcher's default version.

| File   | SHA-1                                      | Source                    |
|--------|--------------------------------------------|---------------------------|
| `18.5` | `d6b706bfbfe637bd98cd657114eea630b7d2dcc7` | WHDLoad 18.5, `C/WHDLoad` |

The executables are identical to `C/WHDLoad` in the official release archives from whdload.de,
e.g. `WHDLoad_18.5_usr.lzx` in `https://whdload.de/whdload/old-whdload/`.

FS-UAE Launcher installs a few more files for WHDLoad games (`binaries` and `support_files` in
`fsgs/amiga/whdload.py`):

- `C/uae-configuration`: see `../UAE/uae-configuration`.
- `Devs/Kickstarts/*.RTB`: the Kickstart relocation tables WHDLoad combines with the user's own
  Kickstart ROM images; see `../SKick/skick346.lha`.
- `C/OSEmu.400` (from JST): left out here, since the WHDLoad documentation does not require it, and
  only some old slaves use it.

## License

WHDLoad is copyright 1995-2019 by Bert Jahn. From its license: "Commercial use is only allowed with
an explicit, written permission by the author. It is strictly prohibited to modify any part of the
WHDLoad package, except the source files which may be modified for reuse. Redistributing is only
allowed if it is absolutely free of charge."
