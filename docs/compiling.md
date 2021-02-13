# Compiling FS-UAE

These instructions are for you if you want to build your own version of FS-UAE
for any reason.

Please note that the resulting build may not be built in a way that's suitable
for distribution to other users. Often, you need to take additional steps to
ensure that the packages work on systems with older operating system versions.

## Setting up a build environment

First, you need to prepare your build environment. Instructions for several
operating systems are listed below.

### Debian / Ubuntu

Install dependencies for bootstrapping and building FS-UAE:

    sudo apt install autoconf automake build-essential gettext \
    libfreetype6-dev libglew-dev libglib2.0-dev libjpeg-dev libmpeg2-4-dev \
    libopenal-dev libpng-dev libsdl2-dev libsdl2-ttf-dev libtool libxi-dev \
    libxtst-dev zip zlib1g-dev

### openSUSE

Install dependencies for bootstrapping and building FS-UAE:

    sudo zypper install autoconf automake freetype2-devel gcc gcc-c++ gettext \
    glib2-devel libpng-devel libtool libXi-devel openal-devel SDL2-devel zip

### Fedora

Install dependencies for running bootstrap:

    sudo dnf install autoconf automake libtool gettext

Install dependencies for building FS-UAE:

    sudo dnf install SDL2-devel glib2-devel libpng-devel gcc gcc-c++ openal-devel libXi-devel zip libmpeg2-devel SDL2_ttf-devel glew-devel zlib-devel freetype-devel hicolor-icon-theme

### Windows

Install latest msys2-x86_64 from www.msys2.org/ and follow the installation
instructions to get an updated MSYS2 installation. Then start
`MSYS2 MinGW 64-bit`.

Install dependencies for bootstrapping and building FS-UAE:

    pacman -S autoconf automake gettext libtool make \
    mingw-w64-x86_64-freetype mingw-w64-x86_64-gcc mingw-w64-x86_64-glew \
    mingw-w64-x86_64-glib2 mingw-w64-x86_64-libmpeg2 mingw-w64-x86_64-libpng \
    mingw-w64-x86_64-openal mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf \
    pkg-config zip

## macOS

Install homebrew from https://brew.sh (follow their instructions) and then
open a new terminal.

Install dependencies for bootstrapping and building FS-UAE:

    brew install autoconf automake freetype gettext glew glib jpeg libmpeg2 \
    libpng libtool openal-soft pkg-config sdl2 sdl2_ttf zlib

In order build FS-UAE itself, you need to have Apple's Xcode installed and
command line tools for Xcode enabled.

## Configuring and building FS-UAE

If you are using the git repository, you need to bootstrap with
(skip if using source tarball):

    ./bootstrap

Run the configuration script (see the next section for options):

    ./configure

And finally, compile the fs-uae executable:

    make

You can now run fs-uae from the source folder without installation:

    ./fs-uae

### Configuration options

The configure script can be used to enable some additional features
(but they might not work or even compile) and disable default features.

Please note, the only _supported_ configuration is to build with the
default option set or the ones listed below. Other options are provided
for testing and debugging purposes - and also for your convenience, in
the cases where it happens to work ;-).

Supported options:

- `--prefix=...`: Installation prefix (defaults to /usr/local).
- `--disable-jit`: For platforms without UAE JIT support.
- `--disable-x86`: Disable x86 bridgeboard emulation.
