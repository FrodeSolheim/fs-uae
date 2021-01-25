# Compiling FS-UAE

First, you need to prepare your build environment

## Setting up a build environment

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
