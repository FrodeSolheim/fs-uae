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

# Name of the autoconf build directory (../.build/<name>). Use different names for different
# environments sharing this source tree (e.g. FSUAE_BUILD=claude in the Claude sandbox) so that
# they can be configured side by side. The od-fs/fs-uae copy is only made for the "dev" build.
FSUAE_BUILD ?= dev
BUILD_DIR = ../.build/$(FSUAE_BUILD)

build:
	$(MAKE) -C $(BUILD_DIR)
ifeq ($(FSUAE_BUILD),dev)
	$(MAKE) fs-uae
endif

fs-uae: $(BUILD_DIR)/fs-uae
	cp -a $(BUILD_DIR)/fs-uae fs-uae

# --------------------------------------------------------------------------------------------------

all: build build-windows winuae winuae-unix

# --------------------------------------------------------------------------------------------------

ifeq ($(OS),MACOS)

# On macOS, create a thin development app bundle for testing the executable as as bundle. In order
# for this to happen, we override the all target. Python code and resources such as images and
# fonts are symlinked into the app.

FS-UAE.app:
	mkdir -p FS-UAE.app/Contents
	ln -s "$$(pwd)/macos/Info.plist" FS-UAE.app/Contents/Info.plist
	mkdir -p FS-UAE.app/Contents/MacOS
	mkdir -p FS-UAE.app/Contents/Resources
	ln -s "$$(pwd)/macos/fs-uae.icns" FS-UAE.app/Contents/Resources/FS-UAE.icns
	ln -s "$$(pwd)/Resources/Fonts/" FS-UAE.app/Contents/Resources/Fonts
	ln -s "$$(pwd)/Resources/Images/" FS-UAE.app/Contents/Resources/Images
	mkdir -p FS-UAE.app/Contents/Resources/Python
	ln -s "$$(pwd)/Python/" FS-UAE.app/Contents/Resources/Python/FS-UAE.zip
	mkdir -p FS-UAE.app/Contents/Resources/Python/lib
#	ln -s "$$(pwd)/build/env/_prefix/lib" FS-UAE.app/Contents/Resources/Python/lib
#	ln -s "$$(pwd)/build/env/_prefix/lib/python3.13/" \
#		FS-UAE.app/Contents/Resources/Python/lib/python313.zip
	ln -s "$$(pwd)/build/env/_prefix/lib/python3.13/" \
		FS-UAE.app/Contents/Resources/Python/lib/python3.13

FS-UAE.app/Contents/MacOS/FS-UAE: FS-UAE.app fs-uae
	cp fs-uae FS-UAE.app/Contents/MacOS/FS-UAE

# Override the all target

all: build fs-uae FS-UAE.app/Contents/MacOS/FS-UAE

endif

# --------------------------------------------------------------------------------------------------
# Cross-compiling the Windows version on Linux, in a Docker container, so only Docker is needed on
# the host. The scripts in build/windows/crossbuild do the work (and can also be run directly, with
# the packages from the Dockerfile there); quasi-msys2 is set up in ../.build/quasi-msys2 on the
# first run. The build goes in ../.build/$(WINDOWS_BUILD) (default: windows). Extra configure
# arguments can be passed with CONFIGURE_ARGS=..., and make's -j option is passed on to the build.

WINDOWS_DOCKER_RUN = build/windows/crossbuild/docker-run

configure-windows:
	$(WINDOWS_DOCKER_RUN) configure $(CONFIGURE_ARGS)

build-windows:
	$(WINDOWS_DOCKER_RUN) build $(filter -j%,$(MAKEFLAGS))

# Runs "make clean" in the Windows build directory (keeps the configuration).
clean-windows:
	$(WINDOWS_DOCKER_RUN) clean

# --------------------------------------------------------------------------------------------------
# Builds the native Windows WinUAE (winuae64.exe) on Linux with ../od-win32/crossbuild (in Docker,
# see its README.md), and copies it here instead of to od-win32/crossbuild. TOOLCHAIN (default:
# msvc) and CONFIG are passed on; running this accepts Microsoft's license for the downloaded MSVC
# toolchain and Windows SDK, as "make -C od-win32/crossbuild" does.

WINUAE_BUILD_DIR ?= $(abspath ../.build/winuae-crossbuild/build-x64-$(or $(TOOLCHAIN),msvc))

winuae:
	$(MAKE) -C ../od-win32/crossbuild build BUILD_DIR=$(WINUAE_BUILD_DIR)
	cp -p $(WINUAE_BUILD_DIR)/winuae64.exe winuae64.exe

# --------------------------------------------------------------------------------------------------
# Builds the Unix WinUAE (od-unix, see ../README_unix.md) with CMake, in
# ../.build/winuae_cmake_build, and copies the winuae executable here together with plugins/ (the
# drive sounds and the FloppyBridge.so real-drive plugin), which it looks for next to the
# executable.

WINUAE_UNIX_BUILD_DIR ?= ../.build/winuae_cmake_build

winuae-unix:
	cmake -S .. -B $(WINUAE_UNIX_BUILD_DIR) -DCMAKE_BUILD_TYPE=RelWithDebInfo
	cmake --build $(WINUAE_UNIX_BUILD_DIR) --target winuae_unix winuae_unix_floppybridge_plugin -j
	cp -p $(WINUAE_UNIX_BUILD_DIR)/winuae winuae
	cp -pR $(WINUAE_UNIX_BUILD_DIR)/plugins .
	cp -p $(WINUAE_UNIX_BUILD_DIR)/FloppyBridge.so plugins/FloppyBridge.so

# --------------------------------------------------------------------------------------------------

bindist:
	./bootstrap
ifeq ($(OS),LINUX)
	make -C dist/x86_64-linux-gnu
else ifeq ($(OS),MACOS)
	make -C dist/arm64-apple-darwin
else ifeq ($(OS),WINDOWS)
	make -C dist/x86_64-w64-mingw32
else
	echo "Unknown platform"
	exit 1
endif

# --------------------------------------------------------------------------------------------------

dist:
	$(MAKE) -C $(BUILD_DIR) dist

# --------------------------------------------------------------------------------------------------

flatpak: dist
	make -C dist/flatpak

# --------------------------------------------------------------------------------------------------

release:
	cd dist/env && make arm64-apple-darwin x86_64-apple-darwin
	cd dist/docker && make x86_64-linux-gnu aarch64-linux-gnu arm-linux-gnueabihf
	cd dist/docker && make x86_64-w64-mingw32

# --------------------------------------------------------------------------------------------------

format:
	find fsapp -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find fsemu -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find fsgui -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find fslib -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find fsuae -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i

	ruff format

# --------------------------------------------------------------------------------------------------

.DEFAULT:
	$(MAKE) -C $(BUILD_DIR) $@

.PHONY: bindist build build-windows clean-windows configure-windows default default-target dist \
	format windows winuae winuae-unix
