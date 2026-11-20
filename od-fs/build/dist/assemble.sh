#!/bin/sh
#
# Assemble step of a dist: makes the dist for $HOST_TRIPLET in $FSUAE_DIST_BUILD/dist/$HOST_TRIPLET
# from the build that build.sh made in $FSUAE_DIST_BUILD/build/$HOST_TRIPLET, and archives it in
# $FSUAE_DIST_BUILD/release/<version> (see common.sh). The archive's path is written to
# dist-archive.txt in the build directory. Run by dist.

set -e

PROJECT_DIR="$(cd "$(dirname "$0")/../../.." && pwd)"
echo "PROJECT_DIR: $PROJECT_DIR"
. "$PROJECT_DIR/od-fs/build/dist/common.sh"
cd "$PROJECT_DIR"

if [ -z "$HOST_TRIPLET" ]; then
    echo "Error: HOST_TRIPLET is not set; make the dist with od-fs/build/dist/dist" >&2
    exit 1
fi

BUILD_DIR="$DIST_BUILD_DIR/build/$HOST_TRIPLET"
echo "BUILD_DIR: $BUILD_DIR"

DIST_DIR="$DIST_BUILD_DIR/dist/$HOST_TRIPLET"
echo "DIST_DIR: $DIST_DIR"

# The triplet the build was configured for, which FS-UAE uses to find its Python home in
# Libs/<triplet>, must be the one the dist is made for.
check_configured_triplet "$BUILD_DIR"

# Some dist scripts specify the strip command (e.g. for macOS, which has no <triplet>-strip).
if [ -z "$STRIP" ]; then
    STRIP=$HOST_TRIPLET-strip
fi

case "$HOST_TRIPLET" in
    *-apple-darwin)
        OS=MACOS
        ;;
    *mingw*)
        OS=WINDOWS
        ;;
    *-linux-*)
        OS=LINUX
        ;;
esac
echo "Building for OS: $OS"
echo "HOST_TRIPLET:    $HOST_TRIPLET"

# FIXME: Replace PYTHON_MINOR with python command (must consider runtime)
if [ "$OS" = "WINDOWS" ]; then

# Run by od-fs/build/windows/crossbuild/dist, which sets MINGW_PREFIX to the quasi-msys2 MinGW
# prefix and BUILD_PYTHON3 to a build machine Python of the same version as the MSYS2 one, for
# byte-compiling the modules. MSYS2 shells set MINGW_PREFIX (/mingw64) themselves.
MINGW_PREFIX="${MINGW_PREFIX:-/mingw64}"
PYTHON3="${BUILD_PYTHON3:-python3}"

# But we actually want to copy the python libraries themselves from the Windows/mingw version
PYTHON_HOME="$MINGW_PREFIX"

else

# Linux and macOS: Python (and on Linux the SDL3 libraries) come from the env prefix, which dist
# sets up and passes on in BUILD_ENV_PREFIX.
if [ -z "$BUILD_ENV_PREFIX" ]; then
    echo "Error: BUILD_ENV_PREFIX (the env prefix with Python) is not set; make the dist with" \
        "od-fs/build/dist/dist" >&2
    exit 1
fi
PYTHON3=python3
PYTHON_HOME="$BUILD_ENV_PREFIX"

fi

if [ -n "$BUILD_ENV_PREFIX" ]; then
# The Python version bundled (and used for compileall below) is the build env one (in cross mode,
# the run script runs the build Python, which has the same version as the bundled one).
PYTHON_MINOR=$("$BUILD_ENV_PREFIX/run" python3 -c "import sys; print(sys.version_info.minor)")
else
PYTHON_MINOR=$("$PYTHON3" -c "import sys; print(sys.version_info.minor)")
fi

if [ ! -d "$PYTHON_HOME/lib/python3.$PYTHON_MINOR" ]; then
    echo "Error: $PYTHON3 is Python 3.$PYTHON_MINOR, but $PYTHON_HOME has no lib/python3.$PYTHON_MINOR"
    exit 1
fi

BASE_DIR="$DIST_DIR"

# SYSTEM_DIR="$BASE_DIR/System"
# Not doing a portable distribution -- for now
SYSTEM_DIR="$BASE_DIR"

LIBS_DIR="$SYSTEM_DIR/Libs"
LIBS_TRIPLET_DIR="$LIBS_DIR/$HOST_TRIPLET"

# The Python home, where the standard library zip and lib-dynload go (in its lib): Libs/<triplet>,
# so that several platforms' dists can be unpacked into one directory, a combined dist (FS-UAE sets
# the home, fsuae-path.cpp). The macOS app bundle keeps it in Contents/Resources/Python.
# Python/FS-UAE.zip is shared.

if [ "$OS" = "MACOS" ]; then
APP_PARENT_DIR="$SYSTEM_DIR"
APP_DIR="$APP_PARENT_DIR/FS-UAE.app"
CONTENTS_DIR="$APP_DIR/Contents"
MACOS_DIR="$CONTENTS_DIR/MacOS"
FRAMEWORKS_DIR="$CONTENTS_DIR/Frameworks"
RESOURCES_DIR="$CONTENTS_DIR/Resources"
PYTHON_DIR="$RESOURCES_DIR/Python"
PYTHON_LIB_DIR="$PYTHON_DIR/lib"
PLUGINS_DIR="$RESOURCES_DIR/Plugins"
FONTS_DIR="$RESOURCES_DIR/Fonts"
IMAGES_DIR="$RESOURCES_DIR/Images"
else
PLUGINS_DIR="$SYSTEM_DIR/Plugins"
PYTHON_DIR="$SYSTEM_DIR/Python"
PYTHON_LIB_DIR="$LIBS_TRIPLET_DIR/lib"
RESOURCES_DIR="$SYSTEM_DIR/Resources"
FONTS_DIR="$RESOURCES_DIR/Fonts"
IMAGES_DIR="$RESOURCES_DIR/Images"
fi

PYTHON_DYNLOAD_DIR="$PYTHON_LIB_DIR/python3.$PYTHON_MINOR/lib-dynload/"

DATA_DIR="$BASE_DIR/Data"
ROMS_DIR="$DATA_DIR/ROMs"

rm -Rf "$DIST_DIR"
mkdir -p $DIST_DIR

mkdir -p "$BASE_DIR"

mkdir -p "$SYSTEM_DIR"

mkdir -p "$LIBS_TRIPLET_DIR"

# --------------------------------------------------------------------------------------------------

if [ "$OS" = "WINDOWS" ]; then

cp "$BUILD_DIR/proxy.exe" "$SYSTEM_DIR/FS-UAE.exe"

cp "$BUILD_DIR/fs-uae.exe" "$LIBS_TRIPLET_DIR/FS-UAE.exe"

# Copy the MinGW DLLs that FS-UAE.exe and the bundled Python extension modules need (found with
# objdump, so the list follows MSYS2 package changes such as DLL renames).
python3 "$PROJECT_DIR/od-fs/build/windows/copy-windows-dlls.py" "$MINGW_PREFIX" "$LIBS_TRIPLET_DIR" \
    "$LIBS_TRIPLET_DIR/FS-UAE.exe"

fi

# --------------------------------------------------------------------------------------------------

if [ "$OS" = "LINUX" ]; then

cp "$BUILD_DIR/fs-uae" $SYSTEM_DIR/FS-UAE

# The libraries are bundled below, once the Python extension modules are in place.

fi

# --------------------------------------------------------------------------------------------------

if [ "$OS" = "MACOS" ]; then

mkdir -p $CONTENTS_DIR
cp "$BUILD_DIR/od-fs/build/macos/Info.plist" $CONTENTS_DIR

mkdir -p $MACOS_DIR
cp "$BUILD_DIR/fs-uae" $MACOS_DIR/FS-UAE

mkdir -p $FRAMEWORKS_DIR

#cp $BUILD_ENV_PREFIX/lib/libcrypto.3.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libffi.8.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libfreetype.6.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libglib-2.0.0.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libharfbuzz.0.dylib $FRAMEWORKS_DIR

# FIXME: harfbuzz plugins???

# FIXME: libiconv needed?
#cp $BUILD_ENV_PREFIX/lib/libiconv.2.dylib $FRAMEWORKS_DIR

#cp $BUILD_ENV_PREFIX/lib/libmpeg2.0.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libmpeg2convert.0.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libpng16.16.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libportmidi.0.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libpython3.$PYTHON_MINOR.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libSDL3_image.0.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libSDL3_ttf.0.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libSDL3.0.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libsqlite3.dylib $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libssl.3.dylib $FRAMEWORKS_DIR
## cp $BUILD_ENV_PREFIX/lib/libuuid.so.1 $FRAMEWORKS_DIR
#cp $BUILD_ENV_PREFIX/lib/libz.1.dylib $FRAMEWORKS_DIR

# # Some shared objects are marked as executable, some are not - normalize this
# chmod a-x $LIBS_TRIPLET_DIR/*

mkdir -p "$RESOURCES_DIR"
cp "$PROJECT_DIR/od-fs/build/macos/fs-uae.icns" "$RESOURCES_DIR/FS-UAE.icns"

fi

# --------------------------------------------------------------------------------------------------

STDLIB_DIR="$PYTHON_LIB_DIR/python3.$PYTHON_MINOR"
rm -Rf "$STDLIB_DIR"

mkdir -p "$STDLIB_DIR"
cp -r "$PYTHON_HOME/lib/python3.$PYTHON_MINOR"/* "$STDLIB_DIR"
rm -Rf "$STDLIB_DIR/site-packages"

# The purge list: parts of the standard library that FS-UAE does not need are left out of every dist
# (packages, modules with their .pyc files, and extension modules). The dist check has the same list
# (PURGED_MODULES and PURGED_MODULE_PREFIXES in check.py) and fails if any of them is in the dist.
# The last entries are CPython's test and example extension modules (which the env Python is built
# without, but the MSYS2 one has): _ctypes_test, and _test*, xx* and _xx* as patterns, quoted here
# and expanded in the dist's standard library below.
# REVIEW: Not tried on macOS yet: check that the env Python's tkinter, curses, readline and dbm
# modules are gone from the .app (Contents/Resources/Python) and from Contents/Frameworks.
for name in tkinter _tkinter idlelib turtle turtledemo curses _curses _curses_panel readline \
        dbm _dbm _gdbm ensurepip test _ctypes_test '_test*' 'xx*' '_xx*'; do
    # shellcheck disable=SC2086 # Unquoted $name, so that the patterns are expanded.
    rm -Rf "${STDLIB_DIR:?}"/$name "$STDLIB_DIR"/$name.py "$STDLIB_DIR"/$name.pyc \
        "$STDLIB_DIR"/__pycache__/$name.*.pyc "$STDLIB_DIR"/lib-dynload/$name.*
done

rm -Rf $DIST_DIR/lib-dynload-temp
mv $PYTHON_DYNLOAD_DIR $DIST_DIR/lib-dynload-temp

rm -f "$PYTHON_LIB_DIR/python3$PYTHON_MINOR.zip"

cd "$STDLIB_DIR"
# rpzip -r ../python3$PYTHON_MINOR.zip .
zip -r ../python3$PYTHON_MINOR.zip .
cd $PROJECT_DIR

rm -Rf "${STDLIB_DIR:?}"/*

mkdir $PYTHON_DYNLOAD_DIR
mv $DIST_DIR/lib-dynload-temp/* $PYTHON_DYNLOAD_DIR
rm -Rf $DIST_DIR/lib-dynload-temp
# mv $DIST_DIR/lib-dynload-temp $PYTHON_DYNLOAD_DIR

# # Remove Python modules which pull in more shared libraries that we are
# # currently not including.
# # FIXME: Can trim some of these after --disable-test-modules
# LIB_DYNLOAD="dist/Python/lib/python3.$PYTHON_MINOR/lib-dynload"
# # rm -f $LIB_DYNLOAD/_bz2.*
# # rm -f $LIB_DYNLOAD/_ctypes_test.*
# # rm -f $LIB_DYNLOAD/_curses_panel.*
# # rm -f $LIB_DYNLOAD/_curses.*
# # rm -f $LIB_DYNLOAD/_dbm.*
# # rm -f $LIB_DYNLOAD/_decimal.*
# # rm -f $LIB_DYNLOAD/_gdbm.*
# # rm -f $LIB_DYNLOAD/_lzma.*
# # rm -f $LIB_DYNLOAD/_test*
# # rm -f $LIB_DYNLOAD/_tkinter.*
# # rm -f $LIB_DYNLOAD/_uuid.*
# # rm -f $LIB_DYNLOAD/_xx*
# # rm -f $LIB_DYNLOAD/readline.*
# # rm -f $LIB_DYNLOAD/termios.*
# # rm -f $LIB_DYNLOAD/xx*

# FIXME: Later, build from source tarball so everything is clean and we don't copy too much

# Byte-compile a copy of od-fs/Python, so the source tree (which may be shared with other builds) is
# not changed. The copy keeps the od-fs/Python path, which ends up in the .pyc files. Hidden files
# and directories (such as .mypy_cache) and old .pyc files are left out.
PYTHON_STAGE_DIR="$BUILD_DIR/dist-python"
rm -Rf "$PYTHON_STAGE_DIR"
mkdir -p "$PYTHON_STAGE_DIR/od-fs"
# (Through a file rather than a pipe, so that set -e stops on errors on both sides.)
# REVIEW: --exclude with BSD tar (macOS) not tried.
PYTHON_STAGE_TAR="$BUILD_DIR/dist-python.tar"
(cd "$PROJECT_DIR/od-fs" && tar -cf "$PYTHON_STAGE_TAR" --exclude='.*' --exclude=__pycache__ \
    --exclude='*.pyc' Python)
(cd "$PYTHON_STAGE_DIR/od-fs" && tar -xf "$PYTHON_STAGE_TAR")
rm "$PYTHON_STAGE_TAR"
cd "$PYTHON_STAGE_DIR"

if [ -n "$BUILD_ENV_PREFIX" ]; then
# Making sure to use build/env version of Python to compile to correct version! (For a
# cross-compiled Linux dist, the prefix's run script runs the build Python, of the same version.)
"$BUILD_ENV_PREFIX/run" python3 -m compileall od-fs/Python
else
$PYTHON3 -m compileall od-fs/Python
fi

cd "$PYTHON_STAGE_DIR/od-fs/Python"
mkdir -p "$PYTHON_DIR"
rm -f $PYTHON_DIR/FS-UAE.zip
# rpzip -r $PYTHON_DIR/FS-UAE.zip .
zip -r $PYTHON_DIR/FS-UAE.zip .
cd "$PROJECT_DIR"

mkdir -p $FONTS_DIR
cp "$PROJECT_DIR/od-fs/Resources/Fonts"/*.ttf $FONTS_DIR

mkdir -p $IMAGES_DIR
cp -a "$PROJECT_DIR/od-fs/Resources/Images"/* $IMAGES_DIR

# Files a WHDLoad archive needs (WHDLoad, uae-configuration, SKick) and the SVG icons.
for name in WHDLoad UAE SKick src svg-custom; do
    cp -a "$PROJECT_DIR/od-fs/Resources/$name" "$RESOURCES_DIR/$name"
done

mkdir "$BASE_DIR/Extras"
cp "$PROJECT_DIR/od-fs/Portable.ini" "$BASE_DIR/Extras/Portable.ini"

cp "$PROJECT_DIR/od-fs/build/dist/README.txt" "$BASE_DIR/README.txt"
unix2dos $BASE_DIR/README.txt

# --------------------------------------------------------------------------------------------------

# mkdir -p $PLUGINS_DIR
# echo "Put plugins in this directory" > $PLUGINS/README.txt
# unix2dos $PLUGINS_DIR/README.txt

# --------------------------------------------------------------------------------------------------

# mkdir -p "$DATA_DIR"
# mkdir -p "$DATA_DIR/Logs"

# mkdir -p "$ROMS_DIR"
# echo "Put Kickstart ROMs in this directory" > "$ROMS_DIR/README.txt"
# unix2dos "$ROMS_DIR/README.txt"

# echo "# FS-UAE" > "$BASE_DIR/Portable.ini"
# unix2dos "$BASE_DIR/Portable.ini"

# --------------------------------------------------------------------------------------------------

# The version the build was configured with (FSUAE_VERSION, see build.sh), which the archive is
# named for.
VERSION=$(sed -n 's/^FSUAE_VERSION *= *//p' "$BUILD_DIR/Makefile")
if [ -z "$VERSION" ]; then
    echo "Error: no FSUAE_VERSION in $BUILD_DIR/Makefile" >&2
    exit 1
fi
echo "FS-UAE version $VERSION"

# --------------------------------------------------------------------------------------------------

OUT_DIR="$DIST_BUILD_DIR/release/$VERSION"
echo "OUT_DIR: $OUT_DIR"
mkdir -p "$OUT_DIR"

# --------------------------------------------------------------------------------------------------

if [ "$OS" = "WINDOWS" ]; then

$STRIP "$LIBS_TRIPLET_DIR/FS-UAE.exe"
$STRIP "$SYSTEM_DIR/FS-UAE.exe"

DIST_FILE=FS-UAE-${VERSION}-$HOST_TRIPLET.zip
cd "$DIST_DIR"
rm -f "$OUT_DIR/$DIST_FILE"
zip -r "$OUT_DIR/$DIST_FILE" *

fi

# --------------------------------------------------------------------------------------------------

if [ "$OS" = "LINUX" ]; then

# The libraries the executable and the Python extension modules need, directly or through other
# libraries, found by walking their dependencies (linux_libs.py, which reads the ELF files without
# running them): SDL3, SDL3_image, SDL3_ttf and Python from the env prefix, the others from the
# system (Ubuntu 24.04), except those on its exclusion list, which must come from the user's system
# (glibc, libstdc++, GL, X11, audio and so on). It fails if a needed library is in neither.
python3 "$PROJECT_DIR/od-fs/build/dist/linux_libs.py" \
    --search-dir "$BUILD_ENV_PREFIX/lib" \
    --search-dir "/usr/lib/$HOST_TRIPLET" \
    --search-dir "/lib/$HOST_TRIPLET" \
    "$LIBS_TRIPLET_DIR" "$SYSTEM_DIR/FS-UAE" "${PYTHON_DYNLOAD_DIR%/}"/*.so

# The bundled libraries: the files in Libs/<triplet> (not its lib directory, the Python home's).
BUNDLED_LIBRARIES="$(find "$LIBS_TRIPLET_DIR" -maxdepth 1 -type f | sort)"

# Some shared objects are marked as executable, some are not - normalize this
# shellcheck disable=SC2086 # The file names have no spaces (sonames).
chmod a-x $BUNDLED_LIBRARIES

$STRIP "$SYSTEM_DIR/FS-UAE"
# shellcheck disable=SC2086 # As above.
$STRIP $BUNDLED_LIBRARIES
$STRIP "${PYTHON_DYNLOAD_DIR%/}"/*

# The rpaths find the bundled libraries from where each file is in the dist; they also replace the
# RUNPATH into the env prefix that the executable gets from sdl3.pc. The dist check checks them.
patchelf --set-rpath "\$ORIGIN/Libs/$HOST_TRIPLET" "$SYSTEM_DIR/FS-UAE"
# shellcheck disable=SC2086 # As above.
patchelf --set-rpath "\$ORIGIN" $BUNDLED_LIBRARIES
# From Libs/<triplet>/lib/python3.X/lib-dynload.
patchelf --set-rpath "\$ORIGIN/../../.." "${PYTHON_DYNLOAD_DIR%/}"/*

DIST_FILE=FS-UAE-${VERSION}-$HOST_TRIPLET.tar.xz
cd "$DIST_DIR"
rm -f "$OUT_DIR/$DIST_FILE"
tar Jcvf "$OUT_DIR/$DIST_FILE" *

fi

# --------------------------------------------------------------------------------------------------

if [ "$OS" = "MACOS" ]; then

# echo NOT STRIPPING
$STRIP "$MACOS_DIR/FS-UAE"
# strip $FRAMEWORKS_DIR/*
# strip $PYTHON_DIR/lib/python3.$PYTHON_MINOR/lib-dynload/*

# Move Python extensions to MacOS directory so they will be fixed by the standalone.py script,
# and necessary dependencies will be put into Contents/Frameworks.
mv "$PYTHON_DYNLOAD_DIR"/*.so "$MACOS_DIR"

python3 "$PROJECT_DIR/od-fs/build/scripts/standalone.py" "$APP_PARENT_DIR"

# Move the fixed Python extensions back
mv "$MACOS_DIR"/*.so "$PYTHON_DYNLOAD_DIR"

cd "$APP_PARENT_DIR"

# Signed with the Developer ID certificate and notarized when the credentials are there (see
# macos_signing_check in common.sh). Otherwise the app is signed ad hoc (no certificate), since
# arm64 Macs do not run code without a valid signature, and the .dmg is not signed or notarized.
# The positional parameters are the app's extra codesign options.
macos_signing_check
if [ "$MACOS_SIGNING" = 1 ]; then
    IDENTITY="$(macos_signing_identity)"
    TIMESTAMP=--timestamp
    ENTITLEMENTS="$PROJECT_DIR/od-fs/build/dist/macos/entitlements.plist"
    if [ "$HOST_TRIPLET" = "x86_64-apple-darwin" ]; then
        ENTITLEMENTS="$PROJECT_DIR/od-fs/build/dist/macos/entitlements-x86_64.plist"
    fi
    set -- --options runtime --entitlements "$ENTITLEMENTS"
else
    # Ad hoc ("-"), without the hardened runtime: with it, library validation would refuse the
    # bundled libraries, which have no team ID when signed ad hoc.
    # REVIEW: Not run on a Mac yet: check that the unsigned app starts on arm64 and x86_64.
    IDENTITY="-"
    TIMESTAMP=--timestamp=none
    set --
fi

# The codesign --deep command does not find/sign the Python extensions, so we do that manually
# and individually first, and then we deep sign the .app bundle itself. (macos_codesign: codesign
# with the identity from FSUAE_MACOS_KEYCHAIN when it is set, see common.sh.)
find "FS-UAE.app/Contents/Resources/Python" -name "*.so" | \
while IFS= read -r f; do
    macos_codesign --force "$TIMESTAMP" --sign "$IDENTITY" "$f"
done
macos_codesign --deep "$@" --force --verify --verbose --sign "$IDENTITY" FS-UAE.app
codesign --verify --deep --strict --verbose=2 FS-UAE.app

echo
echo "If the following step fails, you might have to go into macOS System Settings and enable"
echo "the App Management permission for your terminal."
echo
rm -f FS-UAE.dmg
# The .dmg has the app, and README.txt and Extras from next to it (see macos/settings.py).
dmgbuild -s "$PROJECT_DIR/od-fs/build/dist/macos/settings.py" -D app=FS-UAE.app "FS-UAE" FS-UAE.dmg

if [ "$MACOS_SIGNING" = 1 ]; then

macos_codesign --force --sign "$IDENTITY" "FS-UAE.dmg"
codesign --verify --verbose=2 "FS-UAE.dmg"

# Submit with JSON output, so the log below is fetched for this submission (by its ID). A rejected
# submission may make notarytool exit with an error, so carry on and fetch the log first.
NOTARY_JSON=$(xcrun notarytool submit --apple-id "$NOTARYTOOL_APPLE_ID" \
    --password "$NOTARYTOOL_PASSWORD" --team-id "$NOTARYTOOL_TEAM_ID" \
    --wait --output-format json FS-UAE.dmg) || true
echo "$NOTARY_JSON"

# Prints a field from the notarytool JSON output (the last line that is a JSON object).
notary_field() {
    printf '%s\n' "$NOTARY_JSON" | python3 -c '
import json, sys
for line in reversed(sys.stdin.read().splitlines()):
    try:
        data = json.loads(line)
    except ValueError:
        continue
    if isinstance(data, dict):
        print(data.get(sys.argv[1], ""))
        break
' "$1"
}

NOTARY_ID=$(notary_field id)
NOTARY_STATUS=$(notary_field status)
if [ -z "$NOTARY_ID" ]; then
    echo "Notarization failed: no submission ID in the notarytool output"
    exit 1
fi
echo "Notarization submission $NOTARY_ID: $NOTARY_STATUS"

xcrun notarytool log "$NOTARY_ID" \
    --apple-id "$NOTARYTOOL_APPLE_ID" --password "$NOTARYTOOL_PASSWORD" \
    --team-id "$NOTARYTOOL_TEAM_ID" > notarization-log.json

if [ "$NOTARY_STATUS" != "Accepted" ]; then
    echo "Notarization failed ($NOTARY_STATUS), see $(pwd)/notarization-log.json"
    exit 1
fi

xcrun stapler staple FS-UAE.dmg
xcrun stapler validate FS-UAE.dmg

fi

DIST_FILE=FS-UAE-${VERSION}-$HOST_TRIPLET.dmg

mv "$APP_PARENT_DIR/FS-UAE.dmg" "$OUT_DIR/$DIST_FILE"

fi

# --------------------------------------------------------------------------------------------------

echo
echo "Created $OUT_DIR/$DIST_FILE"
# For dist, which runs the dist check on it.
echo "$OUT_DIR/$DIST_FILE" > "$BUILD_DIR/dist-archive.txt"
# For dist, which checks the signature: "signed" when signed with the Developer ID certificate (and
# notarized), otherwise "unsigned". Only macOS dists are signed.
if [ "$OS" = "MACOS" ] && [ "$MACOS_SIGNING" = 1 ]; then
    echo signed > "$BUILD_DIR/dist-signing.txt"
else
    echo unsigned > "$BUILD_DIR/dist-signing.txt"
fi
echo
