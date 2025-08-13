#!/bin/sh

set -e

pacman -S --needed --noconfirm \
    autoconf \
    automake \
    dos2unix \
    gettext \
    libtool \
    make \
    pkgconf \
    zip

pacman -S --needed --noconfirm \
    mingw-w64-x86_64-gcc \
    mingw-w64-x86_64-flac \
    mingw-w64-x86_64-freetype \
    mingw-w64-x86_64-glib2 \
    mingw-w64-x86_64-harfbuzz \
    mingw-w64-x86_64-libmpeg2 \
    mingw-w64-x86_64-mpg123 \
    mingw-w64-x86_64-portmidi \
    mingw-w64-x86_64-python3 \
    mingw-w64-x86_64-sdl3 \
    mingw-w64-x86_64-sdl3-image \
    mingw-w64-x86_64-sdl3-ttf \
    mingw-w64-x86_64-zstd \

# The Python extensions won't load unless they can find DLL dependencies
# FIXED: By using SetDllDirectory in development mode
# cp /mingw64/bin/libffi-8.dll .
# cp /mingw64/bin/zlib1.dll .
