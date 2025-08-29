#!/bin/sh
# 
# This script installs the required packages for building FS-UAE on Raspberry PI, including
# compiling SDL3 / SDL3_ttf / SDL3_image from source. At the time of writing, SDL3 is not present
# in the Raspberry Pi OS distribution.
#
# --------------------------------------------------------------------------------------------------

set -e

# --------------------------------------------------------------------------------------------------
# Build tools
# --------------------------------------------------------------------------------------------------

# First, install packages needed for running ./bootstrap, configure and make

sudo apt-get install -y autoconf automake gcc make libtool pkgconf

# Other tools that may be required when building or packaging FS-UAE

sudo apt-get install -y dos2unix gettext zip unzip

# --------------------------------------------------------------------------------------------------
# Libraries (except SDL3, SDL3_ttf, SDL3_image)
# --------------------------------------------------------------------------------------------------

# FIXME: libpng-dev might become an indirect (only) dependency of sdl image?
sudo apt-get install -y libflac-dev libgl-dev libglib2.0-dev libmpeg2-4-dev libmpg123-dev \
    libpng-dev libz-dev libzstd-dev python3-dev 

# --------------------------------------------------------------------------------------------------
# SDL3
# --------------------------------------------------------------------------------------------------

# From https://wiki.libsdl.org/SDL3/README-linux
# Ubuntu 18.04, all available features enabled:

sudo apt-get install -y build-essential git make \
    pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
    libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
    libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
    libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
    libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev

# Ubuntu 22.04+ can also add libpipewire-0.3-dev libwayland-dev libdecor-0-dev liburing-dev to that
# command line.

sudo apt-get install -y libpipewire-0.3-dev libwayland-dev libdecor-0-dev liburing-dev

wget https://github.com/libsdl-org/SDL/releases/download/release-3.2.20/SDL3-3.2.20.tar.gz
tar xf SDL3-3.2.20.tar.gz
cd SDL3-3.2.20
cmake -S . -B _build
cmake --build _build
sudo cmake --install _build
cd ..
rm -Rf SDL3-3.2.20.tar.gz SDL3-3.2.20

# --------------------------------------------------------------------------------------------------
# SDL3_ttf
# --------------------------------------------------------------------------------------------------

sudo apt-get install -y libfreetype-dev libharfbuzz-dev 

wget https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz
tar xf SDL3_ttf-3.2.2.tar.gz
cd SDL3_ttf-3.2.2
cmake -S . -B _build
cmake --build _build
sudo cmake --install _build
cd ..
rm -Rf SDL3_ttf-3.2.2.tar.gz SDL3_ttf-3.2.2

# --------------------------------------------------------------------------------------------------
# SDL3_image
# --------------------------------------------------------------------------------------------------

sudo apt-get install -y libpng-dev libjpeg-dev

wget https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-3.2.4.tar.gz
tar xf SDL3_image-3.2.4.tar.gz
cd SDL3_image-3.2.4
cmake -S . -B _build
cmake --build _build
sudo cmake --install _build
cd ..
rm -Rf SDL3_image-3.2.4.tar.gz SDL3_image-3.2.4

# --------------------------------------------------------------------------------------------------

echo
echo "Done! You can now run ./bootstrap"
