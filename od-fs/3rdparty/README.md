# Third-party libraries

Libraries from other projects that FS-UAE builds from source, each in its own directory with its
license. Code shared with upstream WinUAE (in the root directory, `od-win32/` and so on) is not
listed here, nor are libraries that come from the system or the build environment.

When adding or updating a library here:

- Name the directory after the upstream project (or the name distributions package it under).
- Keep the library's own license file (and README) in its directory.
- Change the library's files as little as possible, and list local changes in the table, so that the
  next update knows what to carry over.
- Update the table below.

| Directory | Project | Version | License | Used for | Local changes |
| --- | --- | --- | --- | --- | --- |
| `gamemode/` | [GameMode](https://github.com/FeralInteractive/gamemode) (client header only) | Unknown | BSD-3-Clause | Requesting GameMode on Linux (`od-fs/fsemu/fsemu-gamemode.cpp`) | Unknown |
| `inih/` | [inih](https://github.com/benhoyt/inih) | Unknown | BSD-3-Clause | INI files (`od-fs/fsemu/fsemu-inifile.cpp`) | Unknown |
| `lhafile/` | lhafile, updated from the [original project](http://trac.neotitans.net/wiki/lhafile) (see its `README.md`) | Unknown | BSD-3-Clause | `lzhlib.c`, the C part of the `lhafile` Python package in `od-fs/Python/lhafile` | Python 3 support and other fixes (see its `README.md`) |
| `libslirp/` | [libslirp](https://gitlab.freedesktop.org/slirp/libslirp) | 4.6.1 | BSD-3-Clause and others (see `COPYRIGHT`) | User-mode networking (`slirp_uae.cpp`); `--with-libslirp=system` uses the system library instead | `include/libslirp.h` makes `<libslirp.h>` available with `-I od-fs/3rdparty/libslirp/include`, as `pkg-config slirp` does. `src/misc.c` always uses libslirp's own replacement for `g_spawn_async_with_fds` (GLib 2.58), which only exec forwarding uses, and FS-UAE does not. `src/libslirp-version.h` is pre-generated. `Makefile` (from libslirp 4.2) and `build-aux/meson-dist` are leftovers and not used. |
| `lzma-sdk/` | [LZMA SDK](https://www.7-zip.org/sdk.html) | 16.04 | Public domain | 7z and XZ archives, CHD (`zfile*.cpp`, `mame/src/lib/util/chdcodec.cpp`) | The SDK's `C` directory is in `7z/`, for the `"7z/…"` includes in code shared with WinUAE (`-I od-fs/3rdparty/lzma-sdk`). Other changes unknown. |
| `mt32emu/` | [Munt](https://github.com/munt/munt) (libmt32emu) | 2.7.2 | LGPL-2.1-or-later | Roland MT-32 emulation | `include/mt32emu.h` makes `<mt32emu.h>` available with `-I od-fs/3rdparty/mt32emu/include`, as Munt's install does. `include/mt32emu/config.h` is pre-generated (Munt's build generates it from `src/config.h.in`), and `src/mt32emu.h` and `src/globals.h` include it from there. Other changes unknown. |

## Builtin and system libraries

FS-UAE builds the copies here by default, so that official builds are reproducible. Where
distribution packages need it, a library gets a configure option `--with-<library>=builtin|system`
(default `builtin`) to use the system library instead: so far only libslirp (`--with-libslirp`),
which the Debian and RPM packaging in `od-fs/build/dist/obs` use.

## Include paths

Each library is visible only through its own `-I`, or through an explicit path from the including
file. The `-I` points to a directory with the library's public headers only, never to its source
directory, so that including one library does not make the others (or the library's internal
headers) visible. Where the source tree does not have such a directory, a small FS-UAE wrapper
header provides it (listed as a local change), in the layout the library's own install uses, so that
the same `#include` works with the builtin copy and with an installed library.
