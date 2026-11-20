# Build environment for the Linux dists: Ubuntu 24.04 (glibc 2.39), so the dists run on every
# Linux with glibc 2.39 or newer. docker-run (in this directory) builds the image and runs a command
# in a throwaway container, which is what "make -C od-fs dist-linux" does.
#
# The image is multi-arch: on an amd64 host it makes the x86_64-linux-gnu dist natively and the
# aarch64-linux-gnu dist with a cross toolchain, and on an arm64 host the other way round. It holds
# only Ubuntu packages, from apt-packages.txt: the build tools for the image's own architecture, the
# development packages for both architectures, and the cross compiler for the other one. SDL3,
# SDL3_image, SDL3_ttf and Python are not in the image: the dist entry script
# (od-fs/build/dist/dist) builds them with the env system at dist time, into the env prefix in the
# mounted project (.build/env/<triplet>), where later runs reuse them. A cross-compiled dist uses
# the toolchain files and config.site in this directory.

FROM ubuntu:24.04

COPY apt-packages apt-packages.txt /tmp/apt/

# Ubuntu serves amd64 from archive.ubuntu.com and security.ubuntu.com, and arm64 from
# ports.ubuntu.com, so each source is limited to its architecture (whichever the host is).
RUN printf '%s\n' \
        'Types: deb' \
        'URIs: http://archive.ubuntu.com/ubuntu/' \
        'Suites: noble noble-updates noble-backports' \
        'Components: main restricted universe multiverse' \
        'Architectures: amd64' \
        'Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg' \
        '' \
        'Types: deb' \
        'URIs: http://security.ubuntu.com/ubuntu/' \
        'Suites: noble-security' \
        'Components: main restricted universe multiverse' \
        'Architectures: amd64' \
        'Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg' \
        '' \
        'Types: deb' \
        'URIs: http://ports.ubuntu.com/ubuntu-ports/' \
        'Suites: noble noble-updates noble-backports noble-security' \
        'Components: main restricted universe multiverse' \
        'Architectures: arm64' \
        'Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg' \
        > /etc/apt/sources.list.d/ubuntu.sources

# The other architecture: its Debian name, its triplet, and the name in its cross compiler's
# package names. (The lists are read first, so that an error in the file stops the build.)
#
# libportmidi-dev cannot be installed for two architectures (it is not Multi-Arch: same), so the
# other architecture gets the library package and the development links by hand (the headers from
# the image's own libportmidi-dev are the same for both).
#
# apt installs qemu-user too: libglib2.0-dev for the other architecture needs "python3 | qemu-user",
# and the other architecture's python3 cannot be installed next to the image's own.
#
# REVIEW: Only built on an amd64 host so far. On an arm64 host (the x86_64 dist cross-compiled),
# check that the image builds and that "make -C od-fs dist-linux-x86_64" passes the dist check.
RUN case "$(dpkg --print-architecture)" in \
        amd64) foreign=arm64 triplet=aarch64-linux-gnu cross=aarch64-linux-gnu ;; \
        arm64) foreign=amd64 triplet=x86_64-linux-gnu cross=x86-64-linux-gnu ;; \
        *) echo "Error: no Linux dists for $(dpkg --print-architecture)" >&2; exit 1 ;; \
    esac \
 && sh /tmp/apt/apt-packages tools > /tmp/apt/tools \
 && sh /tmp/apt/apt-packages libraries > /tmp/apt/libraries \
 && sed -e 's/^libportmidi-dev$/libportmidi0/' -e "s/\$/:$foreign/" /tmp/apt/libraries \
        > /tmp/apt/foreign-libraries \
 && dpkg --add-architecture "$foreign" \
 && apt-get update \
 && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get install -y --no-install-recommends \
        $(cat /tmp/apt/tools /tmp/apt/libraries /tmp/apt/foreign-libraries) \
        "gcc-$cross" "g++-$cross" \
 && ln -s libportmidi.so.0 "/usr/lib/$triplet/libportmidi.so" \
 && ln -s libporttime.so.0 "/usr/lib/$triplet/libporttime.so" \
 && rm -rf /var/lib/apt/lists/* /tmp/apt
