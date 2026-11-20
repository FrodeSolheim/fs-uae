#!/bin/sh
#
# Build step of a dist: configures FS-UAE for the dist layout (--enable-dist-layout) for
# $HOST_TRIPLET and builds it from scratch in $FSUAE_DIST_BUILD/build/$HOST_TRIPLET (see
# common.sh), with the version $FSUAE_DIST_VERSION, origin $FSUAE_DIST_ORIGIN, commit
# $FSUAE_DIST_COMMIT and build environment $FSUAE_DIST_ENVIRONMENT (set by dist). assemble.sh then
# makes the dist from that build. Arguments are passed to make (default: -j<number of CPUs>). Run
# by dist.

set -e

PROJECT_DIR="$(cd "$(dirname "$0")/../../.." && pwd)"
echo "PROJECT_DIR: $PROJECT_DIR"
. "$PROJECT_DIR/od-fs/build/dist/common.sh"
cd "$PROJECT_DIR"

BUILD_DIR="$DIST_BUILD_DIR/build/$HOST_TRIPLET"
echo "BUILD_DIR: $BUILD_DIR"

# The version the program shows (see dist and version.py).
if [ -z "$FSUAE_DIST_VERSION" ]; then
    echo "Error: FSUAE_DIST_VERSION is not set; make the dist with od-fs/build/dist/dist" >&2
    exit 1
fi

rm -Rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

"$PROJECT_DIR/configure" --enable-dist-layout --host=$HOST_TRIPLET \
    FSUAE_VERSION="$FSUAE_DIST_VERSION" FSUAE_BUILD_ORIGIN="${FSUAE_DIST_ORIGIN:-}" \
    FSUAE_BUILD_COMMIT="${FSUAE_DIST_COMMIT:-}" \
    FSUAE_BUILD_ENVIRONMENT="${FSUAE_DIST_ENVIRONMENT:-}"

# Checked here, before the build, and again by assemble.sh.
check_configured_triplet "$BUILD_DIR"

if [ "$(uname -o)" = "Darwin" ]; then
NPROC=$(sysctl -n hw.ncpu)
else
# nproc also exists on msys2!
NPROC=$(nproc)
fi

if [ $# -eq 0 ]; then
    set -- -j"$NPROC"
fi
make "$@"
