# Raspberry Pi 4

FS-UAE now runs quite well on Raspberry Pi 4, especially if you overclock it
to 2 GHz. Of course, how much raw power you need depend on what you emulate.
Amiga 500 cycle-exact emulation can be quite demanding.

## Recommended setup

* Overclock your Raspberry Pi 4 to 2 Ghz.
* Use a cooling solution which allows you to run at full speed over time.
* Make sure the CPU is running at full speed always, and not on demand.
* Run FS-UAE from the console for best performance.
* Connect to an HDMI display capable of 1920x1080@50Hz for smooth scrolling.

## Overclocking to 2 Ghz

By overclocking your Pi to 2 Ghz, you can run most Amiga 500 (and 68000)
software in cycle exact mode without slowdown. You can of course use the Pi
with a lower clock speed if it runs fast enough for you.

## Make sure the Raspberry Pi 4 is sufficiently cool

If the Raspberry Pi 4 gets too hot, it will automatically throttle down the
system and this will kill performance. There are tools you can use to monitor
the temperature and see if your RPI4 is being throttled.

The Raspberry Pi 400 can in my experience run at 2 Ghz constantly without
overheating. And even better; the cooling is passive, so there is no fan noise!

## Set performance CPU governor

If the CPU is allowed to throttle up on demand only, the timing of FS-UAE can
be affected (threads not responding quickly enough after going to sleep). So I
recommend switching to the `performance` CPU governor while running FS-UAE.

    sudo apt install cpufrequtils
    sudo cpufreq-set -g performance

You can then confirm with `cpufreq-info` that your Raspberry Pi runs at 2 Ghz.
This is not a permanent setting, a reboot will clear it. So remember to
re-apply the setting before running FS-UAE after a reboot.

This is mostly important if you want stable frame rates (and especially for
vsync).

## X11 vs console (KMSDRM)

Run FS-UAE from the console for best performance. When running under X11, 
tearing can be observed even when using v-sync, and there are also stuttering
(missed v-blanks) occurring quite often with X11 (about every second?).

## Note about v-sync and smooth scrolling

By default, the Raspberry Pi version tries to switch to 1920x1080@50Hz when
running from the console (using the KMSDRM SDL2 backend). It will also
automatically enable vsync in this case.

## Warning about SDL 2.0.14

The vblank code in the KMSDRM SDL2 backend seems to be slightly broken in SDL
2.0.14, causing page flips to occur at the wrong time and visible tearing.
FS-UAE is distributed with a newer version without this bug.

## Compiling for Raspberry Pi 4

Configure optimized version for Raspberry Pi 4:

    CFLAGS="-g -O3 -march=armv8-a" \
    CXXFLAGS="-g -O3 -march=armv8-a" \
    ./configure

TODO: Test with profile-guide optimizations.

## Distribution notes

This is note-to-self- about which libraries to include (and which to skip) when
distributing builds for the Raspberry Pi 4.

Needed:

- libSDL2-2.0.so.0 (Using custom build for KMSDRM support)
- libSDL2_ttf-2.0.so.0: (Not included in default install)

Currently bundled libraries, but not strictly needed, since they seem to be
present on the default Raspberry OS (debian-based) install:

- libbsd.so.0
- libfreetype.so.6
- libglib-2.0.so.0
- libgthread-2.0.so.0
- libmpeg2convert.so.0
- libmpeg2.so.0
- libopenal.so.1
- libpcre.so.3
- libpng16.so.16
- libportmidi.so.0
- libsndio.so.7.0
- libz.so.1

Maybe these libraries are not included in the minimal Raspberry OS install.
Could be an idea to check that before removing any of these from the
distribution.

TODO: Might want to include a freetype library with nicer rendering enabled.
The font rendering looks a bit crappy compared to FS-UAE running on other
systems.
