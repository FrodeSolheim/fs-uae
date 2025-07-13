# Compiling FS-UAE

## Configuration options

The configure script can be used to enable some additional features
(but they might not work or even compile) and disable default features.

Please note, the only *supported* configuration is to build with the
default option set or the ones listed below.  Other options are provided
for testing and debugging purposes - and also for your convenience, in
the cases where it happens to work ;-).

Supported options:

 * --with-libmpeg=builtin    For systems lacking libmpeg2.
 * --disable-jit             For platforms without UAE JIT support.

## Building on Ubuntu or Debian

Install dependencies for running bootstrap:

    sudo apt-get install autoconf automake libtool gettext pkgconf

Run bootstrap (only when building from git repo):

    ./bootstrap

Install dependencies for building FS-UAE:

    sudo apt-get install libsdl2-dev libglib2.0-dev libpng-dev gettext \
    libfreetype6-dev build-essential libopenal-dev libxi-dev zip \
    libmpeg2-4-dev

Configure and build:

    ./configure
    make

You can now run fs-uae from the source folder without installation:

    ./fs-uae

## Building on openSUSE

Install dependencies for running bootstrap:

    sudo zypper install autoconf automake libtool gettext

Install dependencies for building FS-UAE:

    sudo zypper install SDL2-devel glib2-devel libpng-devel gettext \
    freetype2-devel gcc gcc-c++ openal-devel libXi-devel zip
