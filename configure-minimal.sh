#!/bin/sh
set -e
./configure \
--disable-a2065 \
--disable-action-replay \
--disable-aga \
--disable-ahi \
--disable-arcadia \
--disable-builtin-slirp \
--disable-cd32 \
--disable-cdtv \
--disable-cpuemu-0 \
--disable-cpuemu-11 \
--disable-cpuemu-20 \
--disable-cpuemu-22 \
--disable-cpuemu-23 \
--disable-cpuemu-24 \
--disable-cpuemu-25 \
--disable-cpuemu-31 \
--disable-cpuemu-32 \
--disable-cpuemu-33 \
--disable-cpuemu-40 \
--disable-cpuemu-50 \
--disable-fdi2raw \
--disable-gfxboard \
--disable-jit \
--disable-jit-fpu \
--disable-lua \
--disable-netplay \
--disable-parallel-port \
--disable-pearpc-cpu \
--disable-ppc \
--disable-prowizard \
--disable-savestate \
--disable-scp \
--disable-slirp \
--disable-softfloat \
--disable-qemu-cpu \
--disable-qemu-slirp \
--disable-uaenative \
--disable-uaenet \
--disable-uaescsi \
--disable-uaeserial \
--disable-udis86 \
--disable-vpar \
--disable-caps \
--disable-dms \
--disable-drivesound \
--disable-serial-port \
--disable-xml-shader \
--disable-zip

# FIXME:
# --disable-a2091 \
# --disable-cpuemu-13 \
# --disable-cpuemu-21 \
# --disable-cpuboard \
# --disable-debugger \
# --disable-fpuemu \
# --disable-ncr \
# --disable-ncr9x \
# --disable-toccata \
