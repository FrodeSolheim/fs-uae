#!/bin/sh
set -e

. fsdeps/env.sh

# FIXME: Use libharfbuzz for freetype?



fsdeps/dep/freetype
fsdeps/dep/gettext
fsdeps/dep/glib
fsdeps/dep/libmpeg2
fsdeps/dep/libpng
fsdeps/dep/libsamplerate
fsdeps/dep/SDL2
fsdeps/dep/SDL2_ttf

# ----------------------------------------------------------------------------
# SDL2_ttf
# ----------------------------------------------------------------------------

python3 download.py \
https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.15.tar.gz \
sha256:a9eceb1ad88c1f1545cd7bd28e7cbc0b2c14191d40238f531a15b01b1b22cd33
rm -Rf _build && mkdir _build && cd _build
tar xf ../_sources/SDL2_ttf-2.0.15.tar.gz
cd SDL2_ttf-2.0.15
./configure --prefix=$FSDEPS_PREFIX
# FT2_CONFIG=$FSDEPS_PREFIX/bin/freetype-config
make
make install
cd ../..
