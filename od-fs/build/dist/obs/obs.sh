#!/bin/sh
#
# Makes the Debian source package (deb-src.sh) and copies it, with the RPM spec for the same
# package version, into the local checkout of the OBS package.

# REVIEW: Paths updated for the move to od-fs/build/dist/obs; not yet run against OBS itself (osc).

set -e

PROJECT_DIR="$(cd "$(dirname "$0")/../../../.." && pwd)"
. "$PROJECT_DIR/od-fs/build/dist/common.sh"
cd "$PROJECT_DIR"

od-fs/build/dist/obs/deb-src.sh

cd "$DIST_BUILD_DIR/deb"
PACKAGE_VERSION=$(cat package-version.txt)

OBS_PROJECT="home:FrodeSolheim"
OBS_PACKAGE_DIR=~/obs/"$OBS_PROJECT"/fs-uae-5

rm -f "$OBS_PACKAGE_DIR"/*
cp fs-uae-5_$PACKAGE_VERSION-0.debian.tar.xz "$OBS_PACKAGE_DIR"/
cp fs-uae-5_$PACKAGE_VERSION-0.dsc "$OBS_PACKAGE_DIR"/
cp fs-uae-5_$PACKAGE_VERSION.orig.tar.xz "$OBS_PACKAGE_DIR"/

# The spec with the package version (the one in the file is only a placeholder).
sed "s/^%define version .*/%define version $PACKAGE_VERSION/" \
    "$PROJECT_DIR"/od-fs/build/dist/obs/fs-uae.spec > "$OBS_PACKAGE_DIR"/fs-uae-5.spec
cp "$PROJECT_DIR"/od-fs/build/dist/obs/obs-origin.sh "$OBS_PACKAGE_DIR"/

echo "Now you can:"
echo ""
echo "cd \"$OBS_PACKAGE_DIR\""
echo "osc addremove . && EDITOR=nano osc commit -m Updated"
echo ""
