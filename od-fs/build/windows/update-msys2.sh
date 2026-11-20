#!/bin/sh
# Updates the MSYS2 package snapshot in msys2/ to the current MSYS2 packages, and shows which
# package versions changed.
#
# The snapshot pins the packages of the Windows builds (development builds and dists alike) to
# known versions; od-fs/build/windows/crossbuild/quasi-msys2.sh installs them for the cross-build,
# and od-fs/build/prepare in native MSYS2:
#
#   requested_packages.txt  Packages to install (edit this, then run this script)
#   database.mk             The MSYS2 MINGW64 package database, as processed by quasi-msys2
#   packages.txt            The requested packages and their dependencies, with versions; the
#                           builds check that exactly these are installed
#   keyring.asc             The MSYS2 package signing keys
#   msystem.txt             The MSYS2 environment (MINGW64)
#
# Runs quasi-msys2 in an Ubuntu container, so only Docker is needed. The database comes from the
# primary repository (repo.msys2.org) unless MSYS2_MIRROR_URL is set.

set -e

cd "$(dirname "$0")"
DOCKER="${DOCKER:-docker}"

cp msys2/packages.txt msys2/packages.txt.old

"$DOCKER" run --rm -v "$PWD:/docker" \
    -e HOST_UID="$(id -u)" -e HOST_GID="$(id -g)" \
    -e MIRROR_URL="${MSYS2_MIRROR_URL:-https://repo.msys2.org}" \
    ubuntu:24.04 sh -e -c '
apt-get update -qq
DEBIAN_FRONTEND=noninteractive apt-get install -y -qq --no-install-recommends \
    ca-certificates gawk gpg gpgv make tar wget zstd > /dev/null
cp -R /docker/quasi-msys2 /tmp/quasi-msys2
cd /tmp/quasi-msys2
cp /docker/msys2/msystem.txt /docker/msys2/requested_packages.txt .
# Downloads the keyring and the database, verifies the signature and writes database.mk
make database.mk MIRROR_URL="$MIRROR_URL"
make -s get-ver $(make -s get-deps $(cat requested_packages.txt) 2> /dev/null) 2> /dev/null \
    | sort > packages.txt
# prepare (native MSYS2) installs the packages from URLs made from packages.txt by msys2-urls.sh,
# which expects every package file to be <name>-<version>-any.pkg.tar.zst.
tar -xOf database.current_original --wildcards "*/desc" |
    awk "/^%FILENAME%\$/ { getline; print }" | sort > filenames.txt
sed "s/\$/-any.pkg.tar.zst/" packages.txt | sort | comm -23 - filenames.txt > unexpected.txt
if [ -s unexpected.txt ]; then
    echo "Error: no package files with these names in the database (see msys2-urls.sh):" >&2
    cat unexpected.txt >&2
    exit 1
fi
export GNUPGHOME="$(mktemp -d)"
gpg -q --no-default-keyring --keyring ./keyring.kbx --export --armor > keyring.asc
cp database.mk packages.txt keyring.asc /docker/msys2/
chown "$HOST_UID:$HOST_GID" /docker/msys2/database.mk /docker/msys2/packages.txt \
    /docker/msys2/keyring.asc
'

echo
echo "Changes in packages.txt:"
diff msys2/packages.txt.old msys2/packages.txt || true
rm -f msys2/packages.txt.old
