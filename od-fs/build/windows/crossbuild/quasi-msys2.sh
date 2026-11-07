# Setting up quasi-msys2 (sourced after common.sh, bash).
#
# setup_quasi_msys2 sets up quasi-msys2 (od-fs/dist/docker/quasi-msys2) in $QUASI_MSYS2_DIR
# (default: .build/quasi-msys2) with the MSYS2 (MINGW64) packages FS-UAE needs, plus libmpeg2 and
# PortMidi cross-compiled from source. This is the same approach as the Docker release build in
# od-fs/dist/docker. Needs no root.
#
# It does nothing when the packages are already installed, so the configure and build scripts call
# it first. Remove $QUASI_MSYS2_DIR/fs-uae-packages.txt to run the package installation again.
#
# Environment variables (see also common.sh):
#
#   MSYS2_MIRROR_URL  MSYS2 package mirror (default: https://mirror.msys2.org)

source "$OD_FS_DIR/build/windows/extra-libs.sh"

# MSYS2 (MINGW64) packages installed into quasi-msys2. GCC is needed for the C/C++ headers and
# runtime libraries used by Clang when cross-compiling.
MSYS2_CROSS_PACKAGES="
    mingw-w64-x86_64-flac
    mingw-w64-x86_64-freetype
    mingw-w64-x86_64-gcc
    mingw-w64-x86_64-glib2
    mingw-w64-x86_64-harfbuzz
    mingw-w64-x86_64-libpng
    mingw-w64-x86_64-mpg123
    mingw-w64-x86_64-python3
    mingw-w64-x86_64-sdl3
    mingw-w64-x86_64-sdl3-image
    mingw-w64-x86_64-sdl3-ttf
    mingw-w64-x86_64-zlib
    mingw-w64-x86_64-zstd
"

setup_quasi_msys2() {
    local mirror_url="${MSYS2_MIRROR_URL:-https://mirror.msys2.org}"
    local root="$QUASI_MSYS2_DIR/root"
    local stamp="$QUASI_MSYS2_DIR/fs-uae-packages.txt"
    local packages src
    # shellcheck disable=SC2086
    packages="$(echo $MSYS2_CROSS_PACKAGES)"

    if [ "$(cat "$stamp" 2>/dev/null)" = "$packages" ] &&
        [ -f "$root/mingw64/lib/pkgconfig/libmpeg2.pc" ] &&
        [ -f "$root/mingw64/lib/pkgconfig/portmidi.pc" ]; then
        return 0
    fi

    check_tools

    echo "Setting up Windows cross-compilation environment in $QUASI_MSYS2_DIR"

    mkdir -p "$QUASI_MSYS2_DIR"
    if [ ! -f "$QUASI_MSYS2_DIR/Makefile" ]; then
        # Copy only the tracked files, not any package state from the source tree.
        src="$OD_FS_DIR/dist/docker/quasi-msys2"
        cp -R "$src/Makefile" "$src/LICENSE.md" "$src/README.md" "$src/env" \
            "$src/msys2_pacmake_base_dir" "$src/.gitignore" "$QUASI_MSYS2_DIR/"
        echo MINGW64 > "$QUASI_MSYS2_DIR/msystem.txt"
    fi

    if [ "$(cat "$QUASI_MSYS2_DIR/msystem.txt" 2>/dev/null)" != "MINGW64" ]; then
        echo "Error: $QUASI_MSYS2_DIR is not set up for MINGW64 (see msystem.txt)" >&2
        exit 1
    fi

    echo
    echo "Installing MSYS2 packages into $QUASI_MSYS2_DIR..."
    (
        unset MAKEFLAGS MFLAGS MAKELEVEL
        # shellcheck disable=SC2086
        make -C "$QUASI_MSYS2_DIR" install MIRROR_URL="$mirror_url" $packages
    )

    # Remove any Wine wrappers left from an earlier env/all.src (see windows_env in common.sh),
    # since vars.src adds env/fake_bin to PATH as well.
    rm -Rf "$QUASI_MSYS2_DIR/env/fake_bin"

    # libmpeg2 (CD32 FMV) and PortMidi (serial MIDI) have been removed from MSYS2.
    #
    # There is no /mingw64 symlink (creating it would need root), so quasi-msys2 uses absolute
    # paths and PKG_CONFIG_SYSROOT_DIR=$QUASI_MSYS2_DIR/root. Like the MSYS2 packages, these are
    # built with prefix /mingw64 and installed with DESTDIR, so their .pc files work the same way.
    if [ ! -f "$root/mingw64/lib/pkgconfig/libmpeg2.pc" ]; then
        echo
        echo "Cross-compiling libmpeg2..."
        (
            windows_env
            export DESTDIR="$root"
            in_temp_dir build_libmpeg2 /mingw64 --host="$WINDOWS_TRIPLET"
        )
    fi
    if [ ! -f "$root/mingw64/lib/pkgconfig/portmidi.pc" ]; then
        echo
        echo "Cross-compiling PortMidi..."
        (
            windows_env
            export DESTDIR="$root"
            # shellcheck disable=SC2086
            in_temp_dir build_portmidi $WIN_CMAKE_FLAGS -DCMAKE_INSTALL_PREFIX=/mingw64
        )
    fi

    echo "$packages" > "$stamp"
    echo
    echo "quasi-msys2 is ready in $QUASI_MSYS2_DIR"
}
