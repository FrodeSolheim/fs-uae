# FS-UAE

This is the README.md file for the in-development version 5 of FS-UAE. See further down for instructions on building FS-UAE from source code.

FS-UAE 5 builds on the work on FS-UAE 4 which never saw a stable release, but it inherits the following features (and more) from the work done on FS-UAE 4:

- Rewritten video/audio/input system for better performance and more flexibility in extending with new features going forward.
- Includes support for hot-pluggable input devices.
- Making better use of high-resolution timers to ensure the emulation runs smoothly also in fastest-possible CPU mode. This allows for v-sync in fastest possible mode and emulated CPU cycle count is much more stable.
- Updated emulation code from WinUAE 4.2.1. Allows for use of UAE expansions with AmigaOS 4.1 (for example directory hard drives). Also enabled x86 bridgeboard emulation (tested and works, not officially supported yet).
- Integrated MIDI output support via portmidi (tested on Linux).

FS-UAE 5 further enhances this with:

- A new bespoke GUI system built on Python for integrating detailed UIs into FS-UAE (see below for more information).
- Built with SDL3 which works even better than SDL2 on modern operating systems.
- You can now finally freely insert floppy images while the emulation is running via native file browsing dialogs (possibly the most requested feature).
- Floppy images will now be writable by default (perhaps the second-most requested feature).
- A mouse/keyboard interface, no longer just a couch-friendly UI.
- (Soon) Emulation based on WinUAE 6.x emulation code.

The (current) limitations - will have to be reimplemented over time:

- Almost no configurability (yet) - no settings to speak of. What you see is what you get (this will change, of course).
- No v-sync mode right now. It may or may not work nicely with G-Sync/FreeSync.
- No shader support, no functionality to change zoom, or aspect ratio.
- Custom window decorations (which I find nice, but might not be to everyones taste) - an option to use native window borders will likely come later.
- It does at this point not even support loading .fs-uae config files. (Ironically, it can run many .uae config files from command line).
- No custom input mapping.
- No net play support.
- No gamepad/couch-based user interface, only the mouse/keyboard interface for now. A gamepad UI similar the the ones in FS-UAE 3 and 4 will of course be reimplemented in Python and appear later.
- And more...

The idea is of course that FS-UAE 5 will over time do everything FS-UAE 3 did, and more. But at this point I am not promising that it will be 100% backwards compatible.

## New UI System

The major change in FS-UAE 5 is that a new bespoke GUI system is integrated into FS-UAE, built with Python (which is also included in FS-UAE itself now). This allows for quite rapid development of a new mouse/keyboard interface. Quite a bit of work has gone into designing the GUI system. The actual Python/UI code is running in a separate thread, rendering the UI into offscreen images which is then rendered together with UAE video output in the same window. This allows the CPU some time to update and draw the GUI without affecting the emulation thread much, and the GUI can be active while the emulation is running. The GUI system includes "sub-windows" so the system allows for dialogs and multiple "windows" also in fullscreen modes on all platforms.

The bottom line is that complex UIs - and non-emulation functionality - can be integrated directly into FS-UAE without having to write it in C/C++. This also means that the Launcher will likely "disappear" (as in being integrated directly into FS-UAE instead).

## Building FS-UAE

On Ubuntu or Debian, from the project directory (the one above `od-fs`):

```sh
od-fs/build/prepare      # installs the build dependencies
od-fs/bootstrap          # makes configure (in a git checkout)
od-fs/configure          # configures the build directory, .build/dev
make -C od-fs            # builds it
od-fs/fs-uae             # runs it
```

The documentation for developers is in [Docs/Development](Docs/Development) in the git repository (https://github.com/FrodeSolheim/fs-uae; it is not in the source tarball):

- [Building FS-UAE.md](Docs/Development/Building%20FS-UAE.md): what to install and run to build FS-UAE on Linux, Windows (MSYS2, or cross-compiled on Linux) and macOS.
- [Environment.md](Docs/Development/Environment.md): the build environments (the host, the env prefix, the Linux and Windows Docker environments, the GitHub runners) and which to use.
- [Distributions.md](Docs/Development/Distributions.md): the dists (the portable Linux, Windows and macOS downloads), the source tarball and the OBS packages, and how each is made.
- [Versions.md](Docs/Development/Versions.md): the version, file name and origin of every kind of build, and the build header FS-UAE logs.
- [Releasing.md](Docs/Development/Releasing.md): the release cycle, making a release, the `dev` pre-release, and signing the macOS dists.
- [Automated tests.md](Docs/Development/Automated%20tests.md): the tests (essential and extra), how to run them (`od-fs/tests/run.py`) and where new tests go.

## Downloads

- Releases: [GitHub Releases](https://github.com/FrodeSolheim/fs-uae/releases).
- The latest development build: the [`dev` pre-release](https://github.com/FrodeSolheim/fs-uae/releases/tag/dev), updated for every change to `main`.
- Packages for Linux distributions: [OBS repositories](https://software.opensuse.org//download.html?project=home%3AFrodeSolheim&package=fs-uae-5).
