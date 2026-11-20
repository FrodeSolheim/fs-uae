# shellcheck shell=sh
# Where a development build is configured and built (sourced, POSIX sh). Used by od-fs/configure
# for FSUAE_BUILD and by the Windows cross-build (od-fs/build/windows/crossbuild) for
# WINDOWS_BUILD; od-fs/Makefile does the same in make. Expects PROJECT_DIR to be set.

# build_dir <name or path>: prints the build directory. A name is .build/<name> in the project
# directory (FSUAE_BUILD=dev is .build/dev). With a "/" in it, it is a path, relative to the project
# directory when relative (for example a path outside .build for another environment sharing the
# source tree).
build_dir() {
    case "$1" in
        /*) echo "$1" ;;
        */*) echo "$PROJECT_DIR/$1" ;;
        *) echo "$PROJECT_DIR/.build/$1" ;;
    esac
}
