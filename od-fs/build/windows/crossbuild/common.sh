# Settings and helpers shared by the scripts in this directory (sourced, bash).
#
# Environment variables:
#
#   WINDOWS_BUILD     The build directory: a name, .build/<name> in the project directory
#                     (default: windows), or a path (with a "/" in it), relative to the project
#                     directory when relative (see od-fs/build/build-dir.sh)
#   QUASI_MSYS2_DIR   Where quasi-msys2 is set up (default: .build/quasi-msys2 in the project
#                     directory)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OD_FS_DIR="$(cd "$HERE/../../.." && pwd)"
PROJECT_DIR="$(cd "$OD_FS_DIR/.." && pwd)"

WINDOWS_TRIPLET=x86_64-w64-mingw32
WINDOWS_BUILD="${WINDOWS_BUILD:-windows}"
source "$OD_FS_DIR/build/build-dir.sh"
WINDOWS_BUILD_DIR="$(build_dir "$WINDOWS_BUILD")"
# A path without "..", so that docker-run can tell whether it is in the project (left as it is where
# realpath has no -m, as on macOS).
if [[ "$WINDOWS_BUILD" == */* ]] && _dir="$(realpath -ms "$WINDOWS_BUILD_DIR" 2> /dev/null)"; then
    WINDOWS_BUILD_DIR="$_dir"
fi
QUASI_MSYS2_DIR="${QUASI_MSYS2_DIR:-$PROJECT_DIR/.build/quasi-msys2}"
# Make it absolute. docker-run runs this on the host too, and macOS realpath has no -m.
if _dir="$(realpath -m "$QUASI_MSYS2_DIR" 2> /dev/null)"; then
    QUASI_MSYS2_DIR="$_dir"
elif [ "${QUASI_MSYS2_DIR#/}" = "$QUASI_MSYS2_DIR" ]; then
    QUASI_MSYS2_DIR="$PWD/$QUASI_MSYS2_DIR"
fi
unset _dir
export WINDOWS_BUILD QUASI_MSYS2_DIR

# The Clang tools quasi-msys2 uses (env/vars.src): those with the version suffix of
# WIN_NATIVE_CLANG_VER when it is set (the Docker image sets it, see the Dockerfile), as vars.src
# reads it, otherwise the unversioned ones. Without WIN_NATIVE_CLANG_VER, vars.src may also pick
# the highest-sorting clang-<version> that has its clang++-<version> and lld-<version>, but checking
# for the unversioned tools keeps this simple; the image and the GitHub runner set the variable.
clang_tools() {
    local suffix=
    if [[ -v WIN_NATIVE_CLANG_VER && "$WIN_NATIVE_CLANG_VER" != NONE ]]; then
        suffix="-$WIN_NATIVE_CLANG_VER"
    fi
    echo "clang$suffix clang++$suffix ld.lld$suffix llvm-windres$suffix"
}

# Exit with an error if any of the tools needed for cross-compiling are missing.
check_tools() {
    local missing=() tool
    # shellcheck disable=SC2046
    for tool in make wget tar xz zstd gawk gpg gpgv $(clang_tools) \
        x86_64-w64-mingw32-objdump cmake ninja autoreconf pkg-config python3; do
        command -v "$tool" >/dev/null || missing+=("$tool")
    done
    if [ ${#missing[@]} -gt 0 ]; then
        echo "Error: missing tools: ${missing[*]}" >&2
        echo "Install the packages listed in $HERE/apt-packages.txt" >&2
        echo "(and set WIN_NATIVE_CLANG_VER as in README.md), or build in Docker with" >&2
        echo "$HERE/docker-run." >&2
        exit 1
    fi
}

# Exit with an error unless the Clang that quasi-msys2 selected (WIN_NATIVE_CLANG_CC, set by
# env/vars.src) is CLANG_MIN_VERSION or newer. Older ones cannot compile the libstdc++ headers of
# the MSYS2 GCC in the snapshot (see "Ubuntu release" in README.md), which fails much later with
# errors in stl_iterator.h.
CLANG_MIN_VERSION=19
check_clang_version() {
    local version
    if [ -z "${WIN_NATIVE_CLANG_CC:-}" ]; then
        echo "Error: quasi-msys2 found no usable Clang (see its output above)" >&2
        exit 1
    fi
    version="$("$WIN_NATIVE_CLANG_CC" -dumpversion)"
    if [ "${version%%.*}" -lt "$CLANG_MIN_VERSION" ]; then
        echo "Error: Clang $version ($WIN_NATIVE_CLANG_CC) is too old:" \
            "$CLANG_MIN_VERSION or newer is needed." >&2
        echo "Install a newer clang-<version> with its lld-<version> and llvm-<version>, and set" \
            "WIN_NATIVE_CLANG_VER=<version> (see $HERE/README.md)." >&2
        exit 1
    fi
}

# Set up the environment for cross-compiling with quasi-msys2 in the current shell.
#
# Only env/vars.src is sourced, never env/all.src: all.src registers Wine for .exe files via sudo,
# and if Wine is installed it puts Wine wrappers for the MSYS2 tools (python, gcc, ...) first in
# PATH, so configure ends up running Windows programs in Wine.
windows_env() {
    if [ ! -f "$QUASI_MSYS2_DIR/env/vars.src" ]; then
        echo "Error: $QUASI_MSYS2_DIR is not set up (run $HERE/configure)" >&2
        exit 1
    fi
    # vars.src runs make itself, so hide the flags of a make we may be running under. It does not
    # expect to run with set -e either.
    local saved_makeflags="${MAKEFLAGS-}" status=0 output
    unset MAKEFLAGS MFLAGS MAKELEVEL
    output="$(mktemp)"
    set +e
    QUASI_MSYS2_QUIET=1 source "$QUASI_MSYS2_DIR/env/vars.src" > "$output" 2>&1 || status=1
    set -e
    # Leave out three warnings that do not apply here: the /mingw64 link is not used (creating it
    # needs root, see quasi-msys2.sh), Wine is not needed for building, and the MSYS2 CMake in the
    # snapshot (for native MSYS2 builds) is not run: the build uses the distribution's CMake.
    awk '/Didn.t find symlink/ { skip = 3; next }
        /MSYS2 CMake is installed/ { skip = 1; next }
        skip > 0 { skip--; next }
        /Can.t find Wine/ { next }
        { print }' "$output"
    rm -f "$output"
    if [ $status != 0 ]; then
        echo "Error: could not set up the quasi-msys2 environment" >&2
        exit 1
    fi
    check_clang_version
    if [ -n "$saved_makeflags" ]; then
        export MAKEFLAGS="$saved_makeflags"
    fi
    export WINDRES=windres
}
