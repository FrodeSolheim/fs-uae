# Compiling FS-UAE

First, you need to prepare your build environment

## Set up an build environment on Ubuntu or Debian

Install dependencies for running bootstrap:

    sudo apt install autoconf automake libtool gettext

Run bootstrap (only when building from git repo):

    ./bootstrap

Install dependencies for building FS-UAE:

    sudo apt install build-essential libsdl2-dev libglib2.0-dev \
    libopenal-dev gettext libfreetype6-dev libxi-dev libpng-dev zlib1g-dev \
    libmpeg2-4-dev zip libjpeg-dev libglew-dev libxtst-dev libsdl2-ttf-dev

## Set up a build environment on openSUSE

Install dependencies for running bootstrap:

    sudo zypper install autoconf automake libtool gettext

Install dependencies for building FS-UAE:

    sudo zypper install SDL2-devel glib2-devel libpng-devel gettext \
    freetype2-devel gcc gcc-c++ openal-devel libXi-devel zip

## Set up a build environment on Fedora

Install dependencies for running bootstrap:

    sudo dnf install autoconf automake libtool gettext

Install dependencies for building FS-UAE:

    sudo dnf install SDL2-devel glib2-devel libpng-devel gettext   gcc gcc-c++ openal-devel libXi-devel zip libmpeg2-devel SDL2_ttf-devel.x86_64 glew-devel

## Set up a build environment on Windows

Install latest msys2-x86_64 from www.msys2.org/ and follow the installation
instructions to get an updated MSYS2 installation. Then start
`MSYS2 MinGW 64-bit`.

Install dependencies for bootstrapping from repository as well as checking
out the repository from github (optional).

    pacman -S autoconf automake gettext libtool nano openssh

Run bootstrap (only when building from git repo):

    ./bootstrap

Install dependencies for building FS-UAE:

    pacman -S gettext make mingw-w64-x86_64-freetype mingw-w64-x86_64-gcc \
    mingw-w64-x86_64-glew mingw-w64-x86_64-glib2 mingw-w64-x86_64-openal \
    mingw-w64-x86_64-libmpeg2 mingw-w64-x86_64-libpng mingw-w64-x86_64-SDL2 \
    mingw-w64-x86_64-SDL2_ttf pkg-config zip

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

## Configuration options

The configure script can be used to enable some additional features
(but they might not work or even compile) and disable default features.

Please note, the only _supported_ configuration is to build with the
default option set or the ones listed below. Other options are provided
for testing and debugging purposes - and also for your convenience, in
the cases where it happens to work ;-).

Supported options:

- `--prefix=...`: Installation prefix (defaults to /usr/local).
- `--with-libmpeg=builtin`: For systems lacking libmpeg2.
- `--disable-jit`: For platforms without UAE JIT support.
- `--disable-x86`: Disable x86 bridgeboard emulation.
