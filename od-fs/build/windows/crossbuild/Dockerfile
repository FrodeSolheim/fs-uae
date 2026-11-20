# Build environment for cross-compiling the Windows version of FS-UAE with the scripts in this
# directory. docker-run builds the image and runs a script in a throwaway container, which is what
# "make -C od-fs configure-windows / build-windows / clean-windows / dist-windows" do.
#
# The image contains only the Ubuntu packages in apt-packages.txt and WIN_NATIVE_CLANG_VER. A
# GitHub ubuntu-24.04 runner (or a host) runs the scripts directly with the same two (see "Directly
# on the host" in README.md). quasi-msys2 with the MSYS2 packages is set up on demand by
# quasi-msys2.sh, in the mounted source tree (.build/quasi-msys2), so it lives outside the image and
# is shared with builds that run directly on the host.

FROM ubuntu:24.04

COPY apt-packages.txt /tmp/

RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        $(sed 's/#.*//' /tmp/apt-packages.txt) \
 && rm -rf /var/lib/apt/lists/* /tmp/apt-packages.txt

# Makes quasi-msys2 (env/vars.src) use clang-20, lld-20 and llvm-windres-20 from apt-packages.txt
# rather than guess, and check_tools (common.sh) check for them. Ubuntu 24.04's own Clang 18 is too
# old (see "Ubuntu release" in README.md).
ENV WIN_NATIVE_CLANG_VER=20
