# shellcheck shell=sh
# Where the env system puts its output (sourced by setup, clean, shell and the dist entry script,
# POSIX sh). Expects BUILD_ENV_DIR (this od-fs/build/env directory, absolute). Sets:
#
#   BUILD_ENV_BASE     The base directory, absolute (from FSUAE_ENV_DIR).
#   BUILD_ENV_TRIPLET  The triplet the prefix is for: the host's (see od-fs/build/triplet.sh),
#                      unless the sourcing script has set it.
#   BUILD_ENV_PREFIX   The prefix, <base>/<triplet>, unless set in the environment (exported).
#   PACKAGE_SOURCES    Downloaded sources, <base>/sources (exported).
#
# Nothing is created here. A development build and a dist for the same triplet share the prefix.
#
# Environment variables:
#
#   FSUAE_ENV_DIR     Base directory (default: .build/env). A relative path is relative to the
#                     project directory. Use another one (for example a path outside .build) to
#                     leave the default prefixes and sources alone.
#   BUILD_ENV_PREFIX  Use this prefix instead of <base>/<triplet> (also set by the prefix's "run"
#                     script, so running setup from a "run" shell uses that prefix).

# Resolves a directory to its real path, with the case stored on disk. On macOS (case-insensitive
# file system), a path with the wrong case (e.g. after cd-ing into .../git/ instead of .../Git/)
# otherwise ends up in the package builds, while Python resolves its own location with the on-disk
# case. Building Python 3.14 then fails with "No module named '_sysconfigdata__darwin_darwin'".
real_dir() {
    realpath "$1" 2> /dev/null || (cd "$1" && pwd)
}

. "$BUILD_ENV_DIR/../triplet.sh"

BUILD_ENV_PROJECT_DIR="$(real_dir "$BUILD_ENV_DIR/../../..")"

FSUAE_ENV_DIR="${FSUAE_ENV_DIR:-.build/env}"
case "$FSUAE_ENV_DIR" in
    /*) BUILD_ENV_BASE="$FSUAE_ENV_DIR" ;;
    *) BUILD_ENV_BASE="$BUILD_ENV_PROJECT_DIR/$FSUAE_ENV_DIR" ;;
esac

if [ -z "$BUILD_ENV_TRIPLET" ]; then
    BUILD_ENV_TRIPLET="$(host_triplet)"
fi
# Never an empty triplet: the prefix would be the base directory (and clean would delete it all).
if [ -z "$BUILD_ENV_TRIPLET" ]; then
    echo "Error: could not tell the host's triplet" >&2
    exit 1
fi

if [ -z "$BUILD_ENV_PREFIX" ]; then
    BUILD_ENV_PREFIX="$BUILD_ENV_BASE/$BUILD_ENV_TRIPLET"
fi
export BUILD_ENV_PREFIX

PACKAGE_SOURCES="$BUILD_ENV_BASE/sources"
export PACKAGE_SOURCES
