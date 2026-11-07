# Settings and helpers shared by the scripts in this directory (sourced, bash).
#
# Environment variables:
#
#   WINDOWS_BUILD     Name of the build directory, .build/<name> in the project directory (default:
#                     windows)
#   QUASI_MSYS2_DIR   Where quasi-msys2 is set up (default: .build/quasi-msys2 in the project
#                     directory)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OD_FS_DIR="$(cd "$HERE/../../.." && pwd)"
PROJECT_DIR="$(cd "$OD_FS_DIR/.." && pwd)"

WINDOWS_TRIPLET=x86_64-w64-mingw32
WINDOWS_BUILD="${WINDOWS_BUILD:-windows}"
WINDOWS_BUILD_DIR="$PROJECT_DIR/.build/$WINDOWS_BUILD"
QUASI_MSYS2_DIR="$(realpath -m "${QUASI_MSYS2_DIR:-$PROJECT_DIR/.build/quasi-msys2}")"
export WINDOWS_BUILD QUASI_MSYS2_DIR

# Exit with an error if any of the tools needed for cross-compiling are missing.
check_tools() {
    local missing=() tool
    for tool in make wget tar xz zstd gawk gpg gpgv clang ld.lld llvm-ar \
        x86_64-w64-mingw32-objdump cmake ninja autoreconf pkg-config python3; do
        command -v "$tool" >/dev/null || missing+=("$tool")
    done
    if [ ${#missing[@]} -gt 0 ]; then
        echo "Error: missing tools: ${missing[*]}" >&2
        echo "Install the packages listed in $HERE/Dockerfile," >&2
        echo "or build in Docker with $HERE/docker-run." >&2
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
    # Leave out two warnings that do not apply here: the /mingw64 link is not used (creating it
    # needs root, see quasi-msys2.sh), and Wine is not needed for building.
    awk '/Didn.t find symlink/ { skip = 3; next }
        skip > 0 { skip--; next }
        /Can.t find Wine/ { next }
        { print }' "$output"
    rm -f "$output"
    if [ $status != 0 ]; then
        echo "Error: could not set up the quasi-msys2 environment" >&2
        exit 1
    fi
    if [ -n "$saved_makeflags" ]; then
        export MAKEFLAGS="$saved_makeflags"
    fi
    export WINDRES=windres
}
