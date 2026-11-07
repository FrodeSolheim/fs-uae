# Build environment for od-win32/crossbuild/build.sh, for both toolchains.
#
# The image contains only Ubuntu packages. Everything that build.sh downloads (xwin, the MSVC
# CRT/SDK, MSVC under Wine, winuaeinclibs) goes into the mounted source tree under
# .build/winuae-crossbuild, so it lives outside the image and is shared with builds that run
# directly on the host.
#
# build.sh --docker builds this image and runs the build in a throwaway container. To do the same by
# hand:
#
#   docker build -t winuae-crossbuild - < od-win32/crossbuild/Dockerfile
#   docker run --rm --init -u "$(id -u):$(id -g)" -v "$PWD:$PWD" -w "$PWD" \
#       -e HOME="$PWD/.build/winuae-crossbuild/home" \
#       winuae-crossbuild od-win32/crossbuild/build.sh --accept-license
#
# Mount the tree at the same path inside the container, because build.ninja and the msvc-wine
# install contain absolute paths.

FROM ubuntu:26.04

RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        ca-certificates curl git python3 nasm ninja-build \
        clang lld llvm \
        wine wine64 msitools winbind \
 && rm -rf /var/lib/apt/lists/*
