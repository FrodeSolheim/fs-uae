#!/bin/sh
#
# Makes the Debian source package for OBS in $FSUAE_DIST_BUILD/deb (see ../common.sh), from the
# source tarball (make dist), for the version configure takes from git (see configure.ac): the
# release version for a commit tagged v<version>, otherwise a development version. The package
# version is that version with "~" for "-" (5.0.8~dev.<date>T<time>+g<hash>), so that a development
# version sorts before its release in dpkg and RPM; it is written to package-version.txt there, for
# obs.sh. The tarball has the version itself in .tarball-version, which configure reads in the
# package build. It fails when configure could not make the version from git (no git, or a wrong
# tag).

set -e

PROJECT_DIR="$(cd "$(dirname "$0")/../../../.." && pwd)"
. "$PROJECT_DIR/od-fs/build/dist/common.sh"
cd "$PROJECT_DIR"

./bootstrap

rm -Rf "$DIST_BUILD_DIR/deb"
mkdir -p "$DIST_BUILD_DIR/deb"
cd "$DIST_BUILD_DIR/deb"

"$PROJECT_DIR"/configure
make dist

# The version make dist named the tarball for (FSUAE_VERSION, see configure.ac), and the package
# version made from it. (FSUAE_VERSION only has letters, digits and ".+-", so the package version is
# a valid Debian upstream version and RPM version.)
VERSION=$(sed -n 's/^FSUAE_VERSION *= *//p' Makefile)
# A development build falls back to the version in configure.ac when git cannot make the version
# (no git, or a wrong tag), which would name the packages of a development commit like the release.
if [ "$(sed -n 's/^FSUAE_VERSION_FROM_GIT *= *//p' Makefile)" != yes ]; then
    echo "Error: configure could not make the version from git (see its warning)" >&2
    exit 1
fi
PACKAGE_VERSION=$(echo "$VERSION" | tr - '~')
echo "$PACKAGE_VERSION" > package-version.txt

tar xf fs-uae-$VERSION.tar.gz
rm fs-uae-$VERSION.tar.gz

mv fs-uae-$VERSION fs-uae-5-$PACKAGE_VERSION
tar cfJ fs-uae-5-$PACKAGE_VERSION.tar.xz fs-uae-5-$PACKAGE_VERSION
cp fs-uae-5-$PACKAGE_VERSION.tar.xz fs-uae-5_$PACKAGE_VERSION.orig.tar.xz
# mv fs-uae-5-$PACKAGE_VERSION fs-uae-$PACKAGE_VERSION

cd fs-uae-5-$PACKAGE_VERSION
mkdir debian
mkdir debian/source
# A changelog entry for this version, dated now: dpkg-buildpackage sets SOURCE_DATE_EPOCH from it,
# which is the build date FS-UAE shows (see configure.ac).
cat > debian/changelog << EOF
fs-uae-5 ($PACKAGE_VERSION-0) unstable; urgency=low

  * FS-UAE $VERSION.

 -- Frode Solheim <frode@fs-uae.net>  $(date -R)
EOF
cp "$PROJECT_DIR"/od-fs/build/dist/obs/debian/compat debian/
cp "$PROJECT_DIR"/od-fs/build/dist/obs/debian/control debian/
cp "$PROJECT_DIR"/od-fs/build/dist/obs/debian/copyright debian/
cp "$PROJECT_DIR"/od-fs/build/dist/obs/debian/rules debian/
cp "$PROJECT_DIR"/od-fs/build/dist/obs/obs-origin.sh debian/
cp "$PROJECT_DIR"/od-fs/build/dist/obs/debian/source/format debian/source/

dpkg-buildpackage -S -us -uc

cd ..
rm -Rf fs-uae-$PACKAGE_VERSION
