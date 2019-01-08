#!/bin/sh

set -e

if [ `uname` = "Darwin" ]; then
  export CC="clang" CXX="clang++"
fi

./configure \
  --without-libmpeg2 \
  --enable-debugger \
  --disable-jit \
  --disable-prowizard \
  --enable-segtracker \
  $@
