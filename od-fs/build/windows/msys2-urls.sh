#!/bin/sh
#
# Prints the repository URLs of the MSYS2 packages in the snapshot's package list
# (msys2/packages.txt, one <name>-<version>-<release> per line), for installing exactly those
# versions with pacman -U in native MSYS2 (od-fs/build/prepare).
#
# Usage: msys2-urls.sh [--installed <file>] <packages.txt> <mirror URL>
#
#   --installed <file>  The installed packages, as pacman -Q lists them ("<name> <version>" per
#                       line). Packages installed in the pinned version are left out.

set -e

installed_file=/dev/null
if [ "$1" = "--installed" ]; then
    installed_file="$2"
    shift 2
fi
if [ $# -ne 2 ]; then
    echo "Usage: $0 [--installed <file>] <packages.txt> <mirror URL>" >&2
    exit 2
fi
packages_file="$1"
mirror_url="$2"

awk -v mirror="$mirror_url" '
    # The installed packages, as <name>-<version>, like the entries in packages.txt. These (and the
    # package file names) write an epoch as "1~1.2-3", where pacman writes "1:1.2-3".
    FILENAME == ARGV[1] {
        if (NF == 2) {
            version = $2
            sub(/:/, "~", version)
            installed[$1 "-" version] = 1
        }
        next
    }
    NF && !($1 in installed) {
        print mirror "/mingw/mingw64/" $1 "-any.pkg.tar.zst"
    }
' "$installed_file" "$packages_file"
