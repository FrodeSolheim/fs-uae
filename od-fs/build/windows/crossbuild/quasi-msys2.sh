# Setting up quasi-msys2 (sourced after common.sh, bash).
#
# setup_quasi_msys2 sets up quasi-msys2 (od-fs/build/windows/quasi-msys2) in $QUASI_MSYS2_DIR
# (default: .build/quasi-msys2) with the MSYS2 (MINGW64) packages FS-UAE needs, plus libmpeg2 and
# PortMidi cross-compiled from source (install_extra_libs in od-fs/build/windows/extra-libs.sh,
# which native MSYS2 uses too). Needs no root. Used by development builds and the Windows release
# dist alike.
#
# The MSYS2 packages are pinned by the snapshot in od-fs/build/windows/msys2 (see update-msys2.sh
# there): requested_packages.txt lists the packages to install (GCC is needed for the C/C++
# headers and runtime libraries used by Clang when cross-compiling; Clang, LLD, pkgconf, CMake,
# Ninja, GDB and uv are there for native MSYS2 builds, see od-fs/build/prepare, and are installed
# but not used here), database.mk is the package database they are installed from, and
# packages.txt lists the exact package versions that this results in.
#
# It does nothing when the packages in packages.txt are already installed, so the configure and
# build scripts call it first. When the snapshot changes, the next run installs, upgrades or
# downgrades packages to match it. Remove $QUASI_MSYS2_DIR/fs-uae-packages.txt to run the package
# installation again. Downloaded packages are kept in $QUASI_MSYS2_DIR/cache.
#
# Environment variables (see also common.sh):
#
#   MSYS2_MIRROR_URL  MSYS2 package mirror (default: https://mirror.msys2.org)

source "$OD_FS_DIR/build/windows/extra-libs.sh"

MSYS2_SNAPSHOT_DIR="$OD_FS_DIR/build/windows/msys2"

setup_quasi_msys2() {
    local mirror_url="${MSYS2_MIRROR_URL:-https://mirror.msys2.org}"
    local root="$QUASI_MSYS2_DIR/root"
    local stamp="$QUASI_MSYS2_DIR/fs-uae-packages.txt"
    local src file

    if cmp -s "$stamp" "$MSYS2_SNAPSHOT_DIR/packages.txt" &&
        extra_libs_installed "$root" /mingw64; then
        return 0
    fi

    check_tools

    echo "Setting up Windows cross-compilation environment in $QUASI_MSYS2_DIR"

    mkdir -p "$QUASI_MSYS2_DIR"
    if [ ! -f "$QUASI_MSYS2_DIR/Makefile" ]; then
        # Copy only the tracked files, not any package state from the source tree.
        src="$OD_FS_DIR/build/windows/quasi-msys2"
        cp -R "$src/Makefile" "$src/LICENSE.md" "$src/README.md" "$src/env" \
            "$src/msys2_pacmake_base_dir" "$src/.gitignore" "$QUASI_MSYS2_DIR/"
        cp "$MSYS2_SNAPSHOT_DIR/msystem.txt" "$QUASI_MSYS2_DIR/"
    fi

    if [ "$(cat "$QUASI_MSYS2_DIR/msystem.txt" 2>/dev/null)" != "MINGW64" ]; then
        echo "Error: $QUASI_MSYS2_DIR is not set up for MINGW64 (see msystem.txt)" >&2
        exit 1
    fi

    echo
    echo "Installing the MSYS2 packages from $MSYS2_SNAPSHOT_DIR into $QUASI_MSYS2_DIR..."
    for file in requested_packages.txt database.mk packages.txt keyring.asc; do
        cp "$MSYS2_SNAPSHOT_DIR/$file" "$QUASI_MSYS2_DIR/"
    done
    gpg --batch --yes -o "$QUASI_MSYS2_DIR/keyring.kbx" --dearmor "$QUASI_MSYS2_DIR/keyring.asc"
    (
        unset MAKEFLAGS MFLAGS MAKELEVEL
        make -C "$QUASI_MSYS2_DIR" apply-delta MIRROR_URL="$mirror_url"
        # make reports dependency cycles between some packages on stderr
        if ! make -s -C "$QUASI_MSYS2_DIR" list-ins 2> /dev/null | sort |
            cmp -s - "$MSYS2_SNAPSHOT_DIR/packages.txt"; then
            echo "Error: the installed MSYS2 packages do not match" \
                "$MSYS2_SNAPSHOT_DIR/packages.txt" >&2
            exit 1
        fi
    )

    # Remove any Wine wrappers left from an earlier env/all.src (see windows_env in common.sh),
    # since vars.src adds env/fake_bin to PATH as well.
    rm -Rf "$QUASI_MSYS2_DIR/env/fake_bin"

    # libmpeg2 (CD32 FMV) and PortMidi (serial MIDI) have been removed from MSYS2.
    #
    # There is no /mingw64 symlink (creating it would need root), so quasi-msys2 uses absolute
    # paths and PKG_CONFIG_SYSROOT_DIR=$QUASI_MSYS2_DIR/root. Like the MSYS2 packages, these are
    # built with prefix /mingw64 and installed with DESTDIR, so their .pc files work the same way.
    (
        windows_env
        # shellcheck disable=SC2086
        install_extra_libs "$WINDOWS_TRIPLET" "$root" /mingw64 $WIN_CMAKE_FLAGS
    )

    cp "$MSYS2_SNAPSHOT_DIR/packages.txt" "$stamp"
    echo
    echo "quasi-msys2 is ready in $QUASI_MSYS2_DIR"
}
