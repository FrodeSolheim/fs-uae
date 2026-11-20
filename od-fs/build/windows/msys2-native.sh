# shellcheck shell=sh
# Helpers for native MSYS2 builds (sourced, POSIX sh): the default compiler, and the check that the
# MinGW packages are the pinned ones from the MSYS2 snapshot (od-fs/build/windows/msys2). Sourced by
# od-fs/configure (development builds), the dist entry script (od-fs/build/dist/dist) and
# od-fs/build/prepare.

# msys2_use_default_compiler [<configure argument>...]: in MSYS2's MinGW shells, when neither CC nor
# CXX is set (in the environment, or as a configure argument), uses Clang and LLD from the snapshot,
# like the cross-build: exports CC=clang, CXX=clang++ and -fuse-ld=lld in LDFLAGS. Set CC and CXX to
# use another compiler, for example CC=gcc CXX=g++ for GCC (possible, but not tested). Does nothing
# elsewhere.
#
# The MINGW64 clang package uses GCC's libstdc++ and libgcc by default, as the cross-build does, but
# links with GNU ld unless given -fuse-ld=lld (seen with clang -### under Wine). Unlike the
# cross-build (env/vars.src in quasi-msys2), it uses native TLS rather than -femulated-tls; GCC 16's
# libstdc++ exports no emulated TLS variables, so both work with it.
# REVIEW: Not run in an MSYS2 shell yet: check that configure finds clang and clang++, and that the
# link lines (make V=1) have -fuse-ld=lld.
msys2_use_default_compiler() {
    case "$(uname -s)" in
        MINGW*) ;;
        *) return 0 ;;
    esac
    if [ -n "$CC" ] || [ -n "$CXX" ]; then
        return 0
    fi
    for msys2_arg in "$@"; do
        case "$msys2_arg" in
            CC=* | CXX=*) return 0 ;;
        esac
    done
    for msys2_arg in "$@"; do
        case "$msys2_arg" in
            # Configure takes LDFLAGS from the argument rather than the environment.
            LDFLAGS=*)
                echo "Warning: LDFLAGS is given as an argument, so add -fuse-ld=lld to it to link" \
                    "with LLD" >&2
                ;;
        esac
    done
    CC=clang
    CXX=clang++
    LDFLAGS="-fuse-ld=lld${LDFLAGS:+ $LDFLAGS}"
    export CC CXX LDFLAGS
    echo "Using Clang and LLD (CC=$CC CXX=$CXX LDFLAGS=$LDFLAGS; set CC and CXX for another" \
        "compiler)"
}

# msys2_unpinned_package_urls <od-fs directory>: prints the URLs of the snapshot's packages that are
# not installed in their pinned version (none when everything is as pinned), from pacman -Q and
# msys2-urls.sh. MSYS2_MIRROR_URL is the mirror (default: https://mirror.msys2.org).
msys2_unpinned_package_urls() {
    msys2_installed_list=$(mktemp)
    msys2_status=0
    pacman -Q > "$msys2_installed_list" || msys2_status=$?
    if [ "$msys2_status" = 0 ]; then
        sh "$1/build/windows/msys2-urls.sh" --installed "$msys2_installed_list" \
            "$1/build/windows/msys2/packages.txt" \
            "${MSYS2_MIRROR_URL:-https://mirror.msys2.org}" || msys2_status=$?
    fi
    rm -f "$msys2_installed_list"
    return "$msys2_status"
}
