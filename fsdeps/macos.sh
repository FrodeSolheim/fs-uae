#!/bin/sh
set -e

. fsdeps/env.sh

# FIXME: Use libharfbuzz for freetype?

# rm -Rf _build
# mkdir -p _build
mkdir -p fsdeps/_sources

# ----------------------------------------------------------------------------
# freetype
# ----------------------------------------------------------------------------

# python3 download.py \
# https://download.savannah.gnu.org/releases/freetype/freetype-2.10.4.tar.xz \
# sha256:86a854d8905b19698bbc8f23b860bc104246ce4854dcea8e3b0fb21284f75784
# rm -Rf _build && mkdir _build && cd _build
# tar xf ../_sources/freetype-2.10.4.tar.xz
# cd freetype-2.10.4
# ./configure --prefix=$FSDEPS_PREFIX \
# --enable-freetype-config \
# --with-brotli=no
# make
# make install
# cd ../..

fsdeps/dep/freetype

# ----------------------------------------------------------------------------
# gettext
# ----------------------------------------------------------------------------

python3 download.py \
https://ftp.gnu.org/pub/gnu/gettext/gettext-0.21.tar.gz \
sha256:c77d0da3102aec9c07f43671e60611ebff89a996ef159497ce8e59d075786b12
rm -Rf _build && mkdir _build && cd _build
tar xf ../_sources/gettext-0.21.tar.gz
cd gettext-0.21
./configure --prefix=$FSDEPS_PREFIX --disable-java
make
make install
cd ../..

# ----------------------------------------------------------------------------
# glib
# ----------------------------------------------------------------------------

python3 download.py \
https://download.gnome.org/sources/glib/2.66/glib-2.66.7.tar.xz \
sha256:09f158769f6f26b31074e15b1ac80ec39b13b53102dfae66cfe826fb2cc65502
rm -Rf _build && mkdir _build && cd _build
tar xf ../_sources/glib-2.66.7.tar.xz
cd glib-2.66.7
# replace meson.build "subdir('po')" ""
meson --prefix $FSDEPS_PREFIX --libdir lib -Dinternal_pcre=true _build
ninja -C _build
ninja -C _build install
cd ../..

# ----------------------------------------------------------------------------
# libmpeg2
# ----------------------------------------------------------------------------

python3 download.py \
https://libmpeg2.sourceforge.io/files/libmpeg2-0.5.1.tar.gz \
sha256:dee22e893cb5fc2b2b6ebd60b88478ab8556cb3b93f9a0d7ce8f3b61851871d4
rm -Rf _build && mkdir _build && cd _build
tar xf ../_sources/libmpeg2-0.5.1.tar.gz
cd libmpeg2-0.5.1
# Using -std=gnu89 to avoid some inline problems causing linker errors
./configure --prefix=$FSDEPS_PREFIX CFLAGS="-std=gnu89"
make
# Make libmpeg2.a build deterministically by sorting objects
mkdir temp
cd temp
ar x ../libmpeg2/.libs/libmpeg2.a
ar cru libmpeg2.a `ls *.o | sort`
ranlib libmpeg2.a
mv libmpeg2.a ../libmpeg2/.libs/libmpeg2.a
cd ..
make install
cd ../..

# ----------------------------------------------------------------------------
# libpng
# ----------------------------------------------------------------------------

python3 download.py \
https://downloads.sourceforge.net/project/libpng/libpng16/\
1.6.37/libpng-1.6.37.tar.xz \
sha256:505e70834d35383537b6491e7ae8641f1a4bed1876dbfe361201fc80868d88ca
rm -Rf _build && mkdir _build && cd _build
tar xf ../_sources/libpng-1.6.37.tar.xz
cd libpng-1.6.37
./configure --prefix=$FSDEPS_PREFIX
make
make install
cd ../..

# ----------------------------------------------------------------------------
# libsamplerate
# ----------------------------------------------------------------------------

python3 download.py \
https://github.com/libsndfile/libsamplerate/releases/download/0.2.1/\
libsamplerate-0.2.1.tar.bz2 \
sha256:f6323b5e234753579d70a0af27796dde4ebeddf58aae4be598e39b3cee00c90a
rm -Rf _build && mkdir _build && cd _build
tar xf ../_sources/libsamplerate-0.2.1.tar.bz2
cd libsamplerate-0.2.1
./configure --prefix=$FSDEPS_PREFIX
make
make install
cd ../..

# ----------------------------------------------------------------------------
# SDL2
# ----------------------------------------------------------------------------

python3 download.py \
https://www.libsdl.org/release/SDL2-2.0.14.tar.gz \
sha256:d8215b571a581be1332d2106f8036fcb03d12a70bae01e20f424976d275432bc
rm -Rf _build && mkdir _build && cd _build
tar xf ../_sources/SDL2-2.0.14.tar.gz
cd SDL2-2.0.14
if [ "`uname`" == "Darwin" ]; then
python3 ../../replace.py \
src/video/cocoa/SDL_cocoaevents.m \
"keyEquivalent:@\"q\"];" \
"keyEquivalent:@\"\"];"
python3 ../../replace.py \
src/video/cocoa/SDL_cocoaevents.m \
"keyEquivalent:@\"h\"];" \
"keyEquivalent:@\"\"];"
python3 ../../replace.py \
src/video/cocoa/SDL_cocoaevents.m \
"keyEquivalent:@\"m\"];" \
"keyEquivalent:@\"\"];"
python3 ../../replace.py \
src/video/cocoa/SDL_cocoaevents.m \
"keyEquivalent:@\"f\"];" \
"keyEquivalent:@\"\"];"
python3 ../../replace.py \
src/video/cocoa/SDL_cocoaevents.m \
"keyEquivalent:@\"w\"];" \
"keyEquivalent:@\"\"];"
python3 ../../replace.py \
src/video/cocoa/SDL_cocoaevents.m \
"keyEquivalent:@\",\"];" \
"keyEquivalent:@\"\"];"
fi
./configure --prefix=$FSDEPS_PREFIX
make
make install
cd ../..

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
