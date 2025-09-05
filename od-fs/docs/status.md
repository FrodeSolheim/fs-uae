# FS-UAE 5

Information about FS-UAE 5 - currently in development.

**TL;DR; You can use FS-UAE 5 right now if you want to enjoy mostly floppy-based content and don’t need a lot of configurability.**

It is currently **not** a drop-in replacement for FS-UAE 3 (or 4) due to missing features. But please read on for more detailed status, and to get an idea of the plans ahead.

## Design goals and status

Design goals for the next major version of FS-UAE include:

- Implement the latest emulation code from WinUAE 6.x.
- Include a keyboard/mouse interface in addition to a gamepad interface and expose more customization options in the UI -but still strive to make it understandable and user-friendly.
- Possibly include the Launcher functionality in FS-UAE itself.
- Build on top of the rewritten input/audio/video layer written for FS-UAE 4 for better performance and more flexibility going forward.
- Increase compatibility with WinUAE and other UAE flavours?

Basic functionality is implemented (basic audio/video output, basic input), and for now, the initial versions are not very configurable. The idea is of course that FS-UAE 5 will over time do everything FS-UAE 3 did, and more. But at this point I am not promising that it will be 100% backwards compatible.

You can now - finally - freely insert any floppy image while the emulation is running via native file browsing dialogs. This has possibly been the most requested feature. Additionally, floppy images are now writable by default.

FS-UAE 3.x/4.x options are not supported yet (no command line arguments, no .fs-uae config files). So while FS-UAE 4 could work as a drop-in replacement for FS-UAE 3 in most cases, FS-UAE 5 cannot yet be used that way!

Ironically, FS-UAE can now already load .uae config files (if given as a single command line parameter), but please note that some options are blacklisted because they will not work in FS-UAE (yet?) - examples are gfx_* options - and some options may not have any effect. But the ones that are core emulation-related generally work.

## User interface

Basic mouse/keyboard GUI via F12 is implemented. Strictly speaking, t's just mouse right now since keyboard navigation and UI keyboard input is not implemented yet.

There is no gamepad/couch interface at the moment, but this is of course planned to be (re-)added to FS-UAE, in a similar manner to FS-UAE 3 and FS-UAE 4.

Fullscreen mode is implemented. Must be toggled with F11 or Alt/Cmd+F. UAE gfx_fullscreen_amiga / gfx_fullscreen_picasso options are not respected at this point.

## Audio, video and input

Video output is working, but with limited configurability. FS-UAE currently only shows a fixed view of the Amiga display displayed at approximate 4:3 display aspect ratio. UAE gfx_ options for setting display output parameters and filters are generally ignored/non-working. 

A few additional features have preliminary implementation: Alt+C toggles auto-crop, Alt+S toggles integer scaling mode.

Adaptive sync (G-Sync/FreeSync) probably works fine. Classic v-sync is not implemented yet.

OpenGL shaders are not supported yet.

Audio output is working, but with limited configurability. UAE options for specifying stereo separation works. FS-UAE decides frequency and output device (using OS default device) by itself. Adaptive buffer management currently disabled. Buffer overruns, under-runs and increased audio lag might occur until this is re-added.

Keyboard input works, but is not configurable (no custom key mapping, no shortcut rebinding).

Joystick: Cursor keys/right ctrl or alt, right shift emulates joystick, or real gamepads can be used. Not configurable except for choosing input devices in the UI.

Gamepads are hot pluggable and should in most cases be recognized / auto-configured. If not, there is not yet any UI to configure the gamepad.

Mouse input works. Alt+G or middle blick to toggle mouse grab. Not configurable yet. Absolute mouse / mousehack / tablet mode is not implemented yet.

Net play is not implemented yet.

# Emulation features

FS-UAE is now built on top of the latest emulation code from WinUAE 6.0.1. Stock Amiga emulation is generally working.

Floppy support: Working (ADF, IPF, DMS -- ADZ?). Currently working as in WinUAE – writable adf file as default (no custom overlay functionality).

HDF images: Not extensively tested, but seems to work. There is no UI to configure .hdf yet, so only way right now is via .uae config file.

JIT is not working right now. Also, A4000 quickstart is not working due to it trying to enable JIT by default.

RTG / UAGFX / Picasso96: Almost certainly not working at the moment.

CDTV/CD32/AmigaCD works:
- Also, CHD support has been added (tested)!
- And mp3 files in .cue files!

Other / more advanced features may or may not work at this point. Many of them probably does not work:
- Directory hard drives
- Clipboard sharing
- bsdsocket.library emulation
- A2065 emulation (Slirp)
- PPC emulation
- Etc.

## Under the hood

FS-UAE is now built with SDL3 which works even better than SDL2 on modern operating systems, and it is easier now to get features like native file dialogs.

The code repository layout and organization has been altered to better match WinUAE’s code repository, to make it easier to merge and compare changes.

## A new UI system

A major change in FS-UAE 5 is that a new bespoke GUI system is integrated into FS-UAE, built with Python (which is also included in FS-UAE itself now). This allows for quite rapid development of a new mouse/keyboard interface.

Quite a bit of work has gone into designing the GUI system. The actual Python/UI code is running in a separate thread, rendering the UI into offscreen images which is then rendered together with UAE video output in the same window. This allows the CPU some time to update and draw the GUI without affecting the emulation thread much, and the GUI can be active while the emulation is running.

The GUI system includes "sub-windows" so the system allows for dialogs and multiple "windows" also in fullscreen modes on all platforms.

The bottom line is that complex UIs - and non-emulation functionality - can be integrated directly into FS-UAE without having to write it in C/C++.

This also means that the Launcher will likely over time "disappear" (as in being integrated directly into FS-UAE instead). But that's not realistic in the very near future. So the focus will instead probably be to be able to replace FS-UAE 3 in the first instance.

## A note about FS-UAE 4 - and version number

FS-UAE 4 was in development, but never saw a "stable release". Most of the FS-UAE 4 improvements are being carried over into FS-UAE 5.

Some have suggested that the next version should be called FS-UAE 6 due to being based on WinUAE 6 emulation code. There has never before been a direct link between these versions, but we’ll see.

## Builds and source code

FS-UAE is mainly developed and tested on Linux - the code is regularly built on macOS and Windows, but I’m actually using it much less often on these systems. So at this point, there could be performance issues or other issues with the Windows and/or macOS versions that I am not aware of yet.

I have the following distribution channels set up already for FS-UAE 5:

- [Ready-made packages](https://fs-uae.net/alpha) for Windows, Linux and macOS.
- Official packages built for your specific Linux distribution via [OBS repositories](https://software.opensuse.org//download.html?project=home%3AFrodeSolheim&package=fs-uae-5).
- Source code is available from https://github.com/FrodeSolheim/fs-uae in the main branch.

Minimum requirements for the builds from [fs-uae.net](https://fs-uae.net/alpha):

- Windows (x86-64) version is built for Windows 8.1 and higher (not tested on Windows 8.1, but presumed to work).
- macOS versions (ARM64 and x86-64) are built for macOS 10.13 and higher (not tested on macOS 10.13, but presumed to work).
- Linux standalone versions (x86-64 + ARM64 + ARM) are built on Debian 12, and should work on all Linux distributions with glibc 2.36 or newer.,

The Linux distro-specific versions on openSUSE build service are being built for Debian 13, Ubuntu 25.05, Fedora 41, Fedora 42 and openSUSE Leap 16.0 (older releases of these distributions lack some libraries).
