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

SUBDIR = _build

all: build fs-uae

build:
	$(MAKE) -C $(SUBDIR)

fs-uae: _build/fs-uae
	echo fs-uae
	cp -a _build/fs-uae fs-uae

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
	ln -s "$$(pwd)/resources/Fonts/" FS-UAE.app/Contents/Resources/Fonts
	ln -s "$$(pwd)/resources/Images/" FS-UAE.app/Contents/Resources/Images
	mkdir -p FS-UAE.app/Contents/Resources/Python
	ln -s "$$(pwd)/python/" FS-UAE.app/Contents/Resources/Python/FS-UAE.zip
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

bindist:
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

format:
	find fsapp -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find fsemu -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find fsgui -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find fslib -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i
	find fsuae -name '*.h' -o -name '*.c' -o -name '*.cpp' | xargs clang-format --verbose -i

	ruff format

# --------------------------------------------------------------------------------------------------

.DEFAULT:
	$(MAKE) -C $(SUBDIR) $@

.PHONY: bindist build default default-target format
