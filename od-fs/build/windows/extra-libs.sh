# Helpers for building libraries that are no longer packaged by MSYS2 from source (same sources as
# od-fs/build/env/packages and the docker build in od-fs/dist/docker/Dockerfile). Sourced by
# od-fs/build/prepare and od-fs/build/windows/crossbuild/quasi-msys2.sh (POSIX sh).

LIBMPEG2_URL=https://fs-uae.net/files/libmpeg2-0.5.1.tar.gz
LIBMPEG2_SHA256=dee22e893cb5fc2b2b6ebd60b88478ab8556cb3b93f9a0d7ce8f3b61851871d4
PORTMIDI_URL=https://github.com/PortMidi/portmidi/archive/refs/tags/v2.0.6.tar.gz
PORTMIDI_SHA256=81d22b34051621cd56c8d5ef12908ef2a59764c9cdfba6dae47aabddb71ac914

# Download URL ($1) to file ($2) and verify its SHA-256 checksum ($3).
download() {
    if command -v curl >/dev/null 2>&1; then
        curl -fL --retry 3 -o "$2" "$1"
    else
        wget -O "$2" "$1"
    fi
    echo "$3  $2" | sha256sum -c -
}

# Run a command in a new temporary directory, which is removed afterwards. The command runs in a
# subshell with set -e, so a failing step in one of the build functions below stops it (do not call
# this as an if/&&/|| condition, which disables set -e).
in_temp_dir() {
    temp_dir=$(mktemp -d)
    (
        trap 'rm -Rf "$temp_dir"' EXIT
        set -e
        cd "$temp_dir"
        "$@"
    )
}

# Build and install libmpeg2. $1 is the configure prefix; extra arguments are passed to configure.
# Installs into $DESTDIR (if set) + prefix.
# The 2008 autotools files do not know newer platforms (or newer libtool), so regenerate them.
build_libmpeg2() {
    prefix="$1"
    shift
    download "$LIBMPEG2_URL" libmpeg2-0.5.1.tar.gz "$LIBMPEG2_SHA256"
    tar xf libmpeg2-0.5.1.tar.gz
    cd libmpeg2-0.5.1
    autoreconf -fi
    # Using -std=gnu89 to avoid some inline problems causing linker errors
    ./configure --prefix="$prefix" "$@" CFLAGS="-std=gnu89 -O2"
    make -C libmpeg2
    make -C libmpeg2 install
    make -C include install
}

# Build and install PortMidi. Arguments are passed to cmake. Installs into $DESTDIR (if set) +
# install prefix.
build_portmidi() {
    download "$PORTMIDI_URL" portmidi-2.0.6.tar.gz "$PORTMIDI_SHA256"
    tar xf portmidi-2.0.6.tar.gz
    cd portmidi-2.0.6
    CFLAGS="-Wno-incompatible-pointer-types" cmake -S . -B build "$@" -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    cmake --install build
}
