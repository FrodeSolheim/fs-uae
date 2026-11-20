UNAME_O = $(shell uname -o)
UNAME_M = $(shell uname -m)
UNAME_S = $(shell uname -s)

ifeq ($(UNAME_S),Linux)
OS = LINUX
else ifeq ($(UNAME_S),Darwin)
OS = MACOS
else ifeq ($(UNAME_O),Msys)
OS = WINDOWS
else
OS = UNKNOWN
endif

# The autoconf build directory: a name (../.build/<name>), or a path (with a "/" in it), relative
# to the project directory when relative, as in od-fs/configure. Use different ones for different
# environments sharing this source tree (for example a path outside .build for a container or a
# virtual machine) so that they can be configured side by side. The od-fs/fs-uae copy is only made
# for the "dev" build, and not on Windows (see below).
#
# Development builds have the dist layout (od-fs/configure configures them with
# --enable-dist-layout): FS-UAE finds the Python and Resources folders at its root, which is the
# folder of fs-uae (on Windows, two levels above Libs/<triplet>/FS-UAE.exe). od-fs is a root with
# the source tree's folders (od-fs/fs-uae, od-fs/Libs/<triplet>/FS-UAE.exe), and so is a build
# directory, where od-fs/configure links them (natively on Windows, build/windows/dev_layout.py
# makes them as junctions).
FSUAE_BUILD ?= dev
# (FSUAE_BUILD as a path, seen from od-fs.)
FSUAE_BUILD_PATH = $(if $(filter /%,$(FSUAE_BUILD)),,../)$(FSUAE_BUILD)
BUILD_DIR = $(if $(findstring /,$(FSUAE_BUILD)),$(FSUAE_BUILD_PATH),../.build/$(FSUAE_BUILD))

# On Windows (MSYS2's MINGW64 shell), the build is laid out like a dist, as the cross-build is
# (build/windows/dev_layout.py): Libs/x86_64-w64-mingw32/ with FS-UAE.exe, the DLLs it needs and
# FS-UAE._pth, which gives Python the standard library in MINGW_PREFIX. The "dev" build is laid out
# in od-fs, with the proxy as od-fs/FS-UAE.exe, which is how to run it; any other build in its
# build directory. The executable is not copied to od-fs/fs-uae: on Windows, that is fs-uae.exe,
# the same file name as the proxy.
MINGW_PREFIX ?= /mingw64
WINDOWS_DEV_LAYOUT = python3 build/windows/dev_layout.py $(MINGW_PREFIX) $(BUILD_DIR)

build:
	$(MAKE) -C $(BUILD_DIR)
ifeq ($(OS),WINDOWS)
	$(WINDOWS_DEV_LAYOUT)$(if $(filter dev,$(FSUAE_BUILD)), .)
else ifeq ($(FSUAE_BUILD),dev)
	$(MAKE) fs-uae
ifeq ($(OS),MACOS)
	$(MAKE) FS-UAE.app
endif
endif

fs-uae: $(BUILD_DIR)/fs-uae
	cp -a $(BUILD_DIR)/fs-uae fs-uae

# --------------------------------------------------------------------------------------------------

all: build build-windows

# --------------------------------------------------------------------------------------------------

ifeq ($(OS),MACOS)

# On macOS, the dev build also makes or updates FS-UAE.app, a thin development app bundle for
# running the executable as an app: the executable is copied into it, and the Python code, the
# resources and the env prefix's Python standard library are linked (see the script).
FS-UAE.app: fs-uae
	sh build/macos-thin-app.sh $(BUILD_DIR)

.PHONY: FS-UAE.app

endif

# --------------------------------------------------------------------------------------------------
# Cross-compiling the Windows version on Linux, in a Docker container, so only Docker is needed on
# the host. The scripts in build/windows/crossbuild do the work (and can also be run directly, with
# the packages from apt-packages.txt there); quasi-msys2 is set up in ../.build/quasi-msys2 on the
# first run. The build goes in ../.build/$(WINDOWS_BUILD) (default: windows; WINDOWS_BUILD can also
# be a path, see build/windows/crossbuild/common.sh). Extra configure arguments can be passed with
# CONFIGURE_ARGS=..., and make's -j option is passed on to the build.

WINDOWS_DOCKER_RUN = build/windows/crossbuild/docker-run

configure-windows:
	$(WINDOWS_DOCKER_RUN) configure $(CONFIGURE_ARGS)

build-windows:
	$(WINDOWS_DOCKER_RUN) build $(filter -j%,$(MAKEFLAGS))

# Runs "make clean" in the Windows build directory (keeps the configuration).
clean-windows:
	$(WINDOWS_DOCKER_RUN) clean

# --------------------------------------------------------------------------------------------------
# Dists, made with the dist entry script (build/dist/dist) in the environment for each platform.
# The scripts write their build, dist and release directories under FSUAE_DIST_BUILD, default
# .build in the project directory (see build/dist/common.sh): the build in <base>/build/<triplet>,
# the dist in <base>/dist/<triplet> and the archive in <base>/release/<version>.

# The Linux and macOS triplets for the host's architecture.
LINUX_TRIPLET = $(UNAME_M:arm64=aarch64)-linux-gnu
MACOS_TRIPLET = $(UNAME_M:aarch64=arm64)-apple-darwin

# The Linux dists, in the multi-arch Ubuntu 24.04 Docker image (build/linux): dist-linux for the
# host's architecture, and dist-linux-x86_64 and dist-linux-aarch64 for either, natively or with the
# cross compiler in the image. The env prefix with SDL3 and Python is set up (or reused) in
# ../.build/env/<triplet> (FSUAE_ENV_DIR); a cross-compiled dist also sets up the host's own prefix
# with the same Python, as the build Python.
dist-linux:
	build/linux/docker-run sh build/dist/dist $(LINUX_TRIPLET) $(filter -j%,$(MAKEFLAGS))

dist-linux-x86_64:
	build/linux/docker-run sh build/dist/dist x86_64-linux-gnu $(filter -j%,$(MAKEFLAGS))

dist-linux-aarch64:
	build/linux/docker-run sh build/dist/dist aarch64-linux-gnu $(filter -j%,$(MAKEFLAGS))

# The Windows dist, cross-compiled in the cross-build's Docker image, with the same pinned MSYS2
# packages as the development cross-build.
dist-windows:
	$(WINDOWS_DOCKER_RUN) dist $(filter -j%,$(MAKEFLAGS))

# The macOS dists, on a Mac, with the env prefix in ../.build/env/<triplet> (FSUAE_ENV_DIR):
# dist-macos for the Mac's architecture, and dist-macos-x86_64 also on an arm64 Mac (with the
# prefix built under Rosetta, and FS-UAE cross-compiled). Signed and notarized when the
# credentials are there (NOTARYTOOL_* and the Developer ID certificate), otherwise unsigned with a
# warning (FSUAE_MACOS_SIGNING, see build/dist/common.sh).
dist-macos:
	sh build/dist/dist $(MACOS_TRIPLET) $(filter -j%,$(MAKEFLAGS))

dist-macos-x86_64:
	sh build/dist/dist x86_64-apple-darwin $(filter -j%,$(MAKEFLAGS))

# --------------------------------------------------------------------------------------------------

dist:
	$(MAKE) -C $(BUILD_DIR) dist

# --------------------------------------------------------------------------------------------------

flatpak: dist
	make -C build/dist/flatpak

# --------------------------------------------------------------------------------------------------

# Makes the dists that can be made on this machine: the macOS ones on a Mac, the Linux and Windows
# ones on Linux (see the dist targets above).

ifeq ($(OS),MACOS)
# The x86_64 dist also on an arm64 Mac (an Intel Mac cannot make the arm64 one).
release:
	$(MAKE) dist-macos
ifeq ($(MACOS_TRIPLET),arm64-apple-darwin)
	$(MAKE) dist-macos-x86_64
endif
else
# One after the other (not in parallel with -j). The native Linux dist first, so that its env
# prefix (with the build Python for the cross-compiled one) is there.
release:
	$(MAKE) dist-linux
ifeq ($(LINUX_TRIPLET),aarch64-linux-gnu)
	$(MAKE) dist-linux-x86_64
else
	$(MAKE) dist-linux-aarch64
endif
	$(MAKE) dist-windows
endif

# --------------------------------------------------------------------------------------------------

format:
	find fsapp -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find fsemu -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find fsuae -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i

	ruff format

# --------------------------------------------------------------------------------------------------

.DEFAULT:
	$(MAKE) -C $(BUILD_DIR) $@

.PHONY: build build-windows clean-windows configure-windows default default-target dist \
	dist-linux dist-linux-aarch64 dist-linux-x86_64 dist-macos dist-macos-x86_64 \
	dist-windows \
	format release windows
