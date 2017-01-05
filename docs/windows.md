# Notes for Windows Users

This document has some information specific to the Windows version of FS-UAE.

## Amiga Forever

FS-UAE Launcher should automatically find Amiga [kickstarts](kickstarts.md)
from [Amiga Forever](amiga-forever.md) if you have that installed. If not,
please report it. Remember that you may have to start the Amiga Forever
Player at least one in order for the `rom.key` file to be written to disk.

## OpenGL Drivers

FS-UAE requires working and accelerated OpenGL drivers (it does not use
DirectX/Direct3D). OpenGL drivers are usually bundled with the display
drivers for your graphics card - so they should already be present on your
system.

FS-UAE will start and run without hardware-accelerated drivers, but slowly
and with parts of the user interface missing. You really need to have
proper OpenGL display drivers for your card installed.

If you suspect problems with your OpenGL graphics driver, you can download
and run [GLView](http://www.realtech-vr.com/glview/). This program will list
information about your OpenGL driver and its capabilities, and can also
run some tests. Reinstalling your display driver is also a good idea to try.

There has been reported a case where FS-UAE only starts briefly, with the
window disappearing immediately after appear. The problem turned out to be
an issue with the OpenGL drivers. The aforementioned GLView program didn't
even start (aborted with a System.AccessViolationException error). A
reinstall of current display drivers fixed the problem for both GLView and
FS-UAE.

## Conflicting Software

The following software has been reported by users to cause problems for
FS-UAE:

* Trillian overlay - It has been reported that using FS-UAE with Trillian
  chat overlay can cause FS-UAE to crash. You should try to disable the
  Trillian overlay if that is the case.
* Outpost Security Suite Pro 8.0 (4164.639.1856) might cause system to
  freeze when used with FS-UAE ("It’s just a matter of enabling 'Auto-Learn'
  mode just before running FS-UAE for the first time, and all will be fine"
  http://eab.abime.net/showthread.php?t=69080).
* Avast anti-virus has been reported to interfere with thumbnail downloads
  in FS-UAE Launcher, causing the downloader to "hang" on a thumbnail.
  The visible effect to the user is that thumbnails suddenly stop appearing
  and a launcher restart is needed to make thumbnails appear again. If you
  suspect Avast may be interfering, try disabling it before running
  FS-UAE Launcher and see if the problem goes away.
