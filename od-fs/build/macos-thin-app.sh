#!/bin/sh
#
# Makes or updates the thin development app bundle od-fs/FS-UAE.app on macOS, for running the
# development build as an app (make -C od-fs runs it for the dev build). Only the executable is
# copied (from od-fs/fs-uae); the rest is linked: Info.plist from the build directory (configure
# makes it), the icon, the Python code and every folder in od-fs/Resources (Fonts, Images, WHDLoad,
# ..., directly in Contents/Resources, where the macOS dist has them) from the source tree, and the
# env prefix's Python standard library (.build/env/<triplet>/lib/python3.X, see
# od-fs/build/env/scripts/paths.sh, the prefix od-fs/configure builds against) as
# Contents/Resources/Python/lib/python3.X, where a dist has its own. The links are made again on every run, so they follow a prefix that has moved
# (such as the old od-fs/build/env/_prefix).
#
# Usage: macos-thin-app.sh <build directory>

# REVIEW: Not run on a Mac. Before, the Makefile made the bundle once, with Info.plist linked from
# od-fs/macos (where only an in-tree configure puts it) and the standard library from
# od-fs/build/env/_prefix.

set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 <build directory>" >&2
    exit 2
fi

PROJECT_DIR="$(cd "$(dirname "$0")/../.." && pwd)"
OD_FS_DIR="$PROJECT_DIR/od-fs"
BUILD_DIR="$(cd "$1" && pwd)"
APP="$OD_FS_DIR/FS-UAE.app"
CONTENTS="$APP/Contents"
PYTHON_LIB_DIR="$CONTENTS/Resources/Python/lib"

ENV_PREFIX="$(
    BUILD_ENV_DIR="$OD_FS_DIR/build/env"
    . "$BUILD_ENV_DIR/scripts/paths.sh"
    real_dir "$BUILD_ENV_PREFIX" 2> /dev/null
)" || ENV_PREFIX=""

if [ ! -f "$OD_FS_DIR/fs-uae" ]; then
    echo "Error: $OD_FS_DIR/fs-uae is missing (build the dev build first)" >&2
    exit 1
fi
if [ ! -f "$BUILD_DIR/od-fs/build/macos/Info.plist" ]; then
    echo "Error: $BUILD_DIR/od-fs/build/macos/Info.plist is missing (run od-fs/configure first)" >&2
    exit 1
fi

# Links $2 to $1, replacing an earlier link (never a directory: -n). Directory targets end with a
# slash.
link() {
    ln -sfn "$1" "$2"
}

mkdir -p "$CONTENTS/MacOS" "$CONTENTS/Resources/Python/lib"
link "$BUILD_DIR/od-fs/build/macos/Info.plist" "$CONTENTS/Info.plist"
link "$OD_FS_DIR/build/macos/fs-uae.icns" "$CONTENTS/Resources/FS-UAE.icns"
# Every folder in od-fs/Resources, so that a new one is linked too. Links to folders that are no
# longer there are removed.
for old_link in "$CONTENTS/Resources"/*; do
    if [ -L "$old_link" ] && [ ! -e "$old_link" ]; then
        case "$(readlink "$old_link")" in
            "$OD_FS_DIR/Resources/"*) rm "$old_link" ;;
        esac
    fi
done
for dir in "$OD_FS_DIR/Resources"/*/; do
    name="$(basename "$dir")"
    link "$OD_FS_DIR/Resources/$name/" "$CONTENTS/Resources/$name"
done
link "$OD_FS_DIR/Python/" "$CONTENTS/Resources/Python/FS-UAE.zip"

# The standard library: remove the old links first (another Python version, another prefix).
for old_link in "$PYTHON_LIB_DIR"/python3.*; do
    if [ -L "$old_link" ]; then
        rm "$old_link"
    fi
done
stdlib_dir=""
if [ -n "$ENV_PREFIX" ]; then
    for dir in "$ENV_PREFIX"/lib/python3.*; do
        if [ -f "$dir/os.py" ]; then
            if [ -n "$stdlib_dir" ]; then
                echo "Error: more than one Python standard library in $ENV_PREFIX/lib" >&2
                exit 1
            fi
            stdlib_dir="$dir"
        fi
    done
fi
if [ -n "$stdlib_dir" ]; then
    link "$stdlib_dir/" "$PYTHON_LIB_DIR/$(basename "$stdlib_dir")"
else
    echo "Warning: no Python standard library in the env prefix ${ENV_PREFIX:-(none)}, so" \
        "$APP has none either (run od-fs/build/env/setup)" >&2
fi

cp "$OD_FS_DIR/fs-uae" "$CONTENTS/MacOS/FS-UAE"
echo "Updated $APP"
