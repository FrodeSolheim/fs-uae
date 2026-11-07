# Build environment for cross-compiling the Windows version of FS-UAE with the scripts in this
# directory. docker-run builds the image and runs a script in a throwaway container, which is what
# "make -C od-fs configure-windows / build-windows / clean-windows" do.
#
# The image contains only Ubuntu packages; install the same ones to run the scripts directly on the
# host. quasi-msys2 with the MSYS2 packages is set up on demand by quasi-msys2.sh, in the mounted
# source tree (.build/quasi-msys2), so it lives outside the image and is shared with builds that
# run directly on the host.
#
#   make, wget, tar, xz-utils, zstd, gawk, gpg, gpgv:  quasi-msys2 package manager
#   clang, lld, llvm:                                  compiler, linker and llvm-windres/ar/ranlib
#   binutils-mingw-w64-x86-64:                         x86_64-w64-mingw32-strip/objdump and friends
#   autoconf, automake, libtool, cmake, ninja-build:   bootstrap, and building libmpeg2/PortMidi
#   gettext, pkgconf, python3, zip, file:              FS-UAE configure and build
#
# Keep the package list in sync with the apt-get command in README.md.

FROM ubuntu:26.04

RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        ca-certificates curl wget git build-essential \
        make tar xz-utils zstd gawk gpg gpgv \
        clang lld llvm binutils-mingw-w64-x86-64 \
        autoconf automake libtool cmake ninja-build \
        gettext pkgconf python3 zip unzip file \
 && rm -rf /var/lib/apt/lists/*
