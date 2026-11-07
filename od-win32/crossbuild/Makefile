# Build winuae64.exe with Microsoft's compiler (under Wine) in a Docker container, and copy it here.
#
#   make -C od-win32/crossbuild          # -> od-win32/crossbuild/winuae64.exe
#
# Running this accepts Microsoft's license for the MSVC toolchain and Windows SDK that are
# downloaded on the first build (https://go.microsoft.com/fwlink/?LinkId=2086102).
#
# Downloads and the build directory are in <repo>/.build/winuae-crossbuild, so later builds are
# incremental. See README.md and build.sh --help.

HERE      := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
SRC       := $(abspath $(HERE)/../..)
TOOLCHAIN ?= msvc
CONFIG    ?= FullRelease
BUILD_DIR ?= $(SRC)/.build/winuae-crossbuild/build-x64-$(TOOLCHAIN)
DOCKER    ?= docker
IMAGE     ?= winuae-crossbuild

export DOCKER
export WINUAE_DOCKER_IMAGE := $(IMAGE)

.PHONY: all image build clean

all: build
	cp -p $(BUILD_DIR)/winuae64.exe $(HERE)/winuae64.exe
	@echo "Copied to $(HERE)/winuae64.exe"

image:
	$(DOCKER) build -t $(IMAGE) - < $(HERE)/Dockerfile

build: image
	$(HERE)/build.sh --docker --accept-license --toolchain $(TOOLCHAIN) \
		--config $(CONFIG) --build-dir $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(HERE)/winuae64.exe
