# Raspberry Pi 4

FS-UAE now runs quite well on Raspberry Pi 4, especially if you overclock it
to 2 GHz. Of course, how much raw power you need depend on what you emulate.
Amiga 500 cycle-exact emulation can be quite demanding.

## Recommended setup

* Overclock your Raspberry Pi 4 to 2 Ghz.
* Make sure you have a cooling solution which allows you to run at full
  speed (over time) without the CPU being throttled. For example by using
  a case with passive or active cooling, or just get yourself a
  Raspberry Pi 400 which overclocks very nicely to 2 Ghz.
* Run FS-UAE from the console for best performance. When running under X11,
  tearing can often be observed even when using v-sync.
* Make sure the CPU is running at full speed (and not on demand).
* Connect to an HDMI display capable of 1920x1080@50Hz for smooth scrolling.

## Set performance CPU governor

    sudo apt install cpufrequtils
    sudo cpufreq-set -g performance

You can then confirm with `cpufreq-info` that your Raspberry Pi runs at 2 Ghz.

## Note about v-sync and smooth scrolling

By default, the Raspberry Pi version tries to switch to 1920x1080@50Hz when
running from the console (using the KMSDRM SDL2 backend). It will also
automatically enable v-sync.

The v-sync code isn't completely done, and you might experience tearing
for now.

## Compiling for Raspberry Pi 4

Configure optimized version for Raspberry Pi 4:

    CFLAGS="-g -O2 -mcpu=cortex-a72 -mfloat-abi=hard -mfpu=neon-fp-armv8 -mneon-for-64bits" \
    CXXFLAGS="-g -O2 -mcpu=cortex-a72 -mfloat-abi=hard -mfpu=neon-fp-armv8 -mneon-for-64bits" \
    ./configure

TODO: check if the neon flags actually does any good

TODO: Test with profile-guide optimizations.

## Distribution notes

Needed:

- libSDL2-2.0.so.0: Custom build)
- libSDL2_ttf-2.0.so.0: Missing on default install

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
