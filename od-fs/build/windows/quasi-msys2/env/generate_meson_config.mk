# Generates the cross-file for meson (like cmake's toolchain file).
# Meson, in their infinite wisdom, decided that you shouldn't be able
# to read environment variables from your build scripts, so apparently
# we have GENERATE this on the fly. Duh.

ifeq ($(origin MSYSTEM),undefined)
$(error Environment variables are missing. Run this from the quasi-msys2 shell, or source `vars.src` first)
endif

override define lf :=
$(strip)
$(strip)
endef

override is32bit := $(if $(filter %32,$(MSYSTEM)),y)

override meson_sys_root := $(if $(PKG_CONFIG_SYSROOT_DIR),$(PKG_CONFIG_SYSROOT_DIR),/)

# Here we only set `exe_wrapper` if Wine is installed.
override define contents :=
[binaries]
$(if $(shell which wine >/dev/null 2>/dev/null)$(filter 0,$(.SHELLSTATUS)),exe_wrapper = 'wine',# exe_wrapper = ??)
# Meson refuses to use those unless we explicitly tell it to. Something else might be missing.
pkg-config = 'pkg-config'
strip = 'strip'
# Unsure about those, copied them from https://github.com/mesonbuild/meson/blob/master/cross/linux-mingw-w64-64bit.txt
ar = 'ar'
windres = 'windres'
cmake = 'cmake'

[host_machine]# Maybe we should
system = 'windows'
# I guess?
cpu_family = '$(if $(is32bit),x86,x86_64)'
cpu = '$(if $(is32bit),i686,x86_64)'
endian = 'little'

[properties]
# This is not `MSYSTEM_PREFIX`, but rather its parent.
# Meson assigns this to `PKG_CONFIG_SYSROOT_DIR`, so we just use the value of that variable.
sys_root = '$(meson_sys_root)'
# What's the difference? `root` seems to be undocumented...
root = '$(meson_sys_root)'
endef

# Note the "native file". It's not usually needed, but if a project tries to find a native library,
#   we need it at least to provide a version of pkg-config that unsets our modified env variables.
# And as a bonus, we can set the native compiler to Clang if we know we have it.
override define contents_nat :=
[binaries]
$(if $(WIN_HOST_CC),c = '$(WIN_HOST_CC)',# c = ??)
$(if $(WIN_HOST_CXX),cpp = '$(WIN_HOST_CXX)',# cpp = ??)
$(if $(WIN_HOST_LD),c_ld = '$(WIN_HOST_LD)',# c_ld = ??)
$(if $(WIN_HOST_LD),cpp_ld = '$(WIN_HOST_LD)',# cpp_ld = ??)
# This one is important. We need a wrapper that unsets our environment variables.
pkg-config = 'win-native-pkg-config'
# The unmodified cmake.
cmake = '$(WIN_NATIVE_CMAKE)'
# For completeness.
ar = 'ar'
strip = 'strip'
endef

target_file := $(dir $(word 1,$(MAKEFILE_LIST)))config/meson_cross_file.ini
target_file_nat := $(dir $(word 1,$(MAKEFILE_LIST)))config/meson_native_file.ini

override old_contents := $(file <$(target_file))
override old_contents_nat := $(file <$(target_file_nat))
override should_overwrite := $(if $(and $(findstring $(contents),$(old_contents)),$(findstring $(old_contents),$(contents))),,y)
override should_overwrite_nat := $(if $(and $(findstring $(contents_nat),$(old_contents_nat)),$(findstring $(old_contents_nat),$(contents_nat))),,y)

.PHONY: all
all: $(target_file) $(target_file_nat)

ifneq ($(should_overwrite),)
.PHONY: $(target_file)
endif
ifneq ($(should_overwrite_nat),)
.PHONY: $(target_file_nat)
endif

$(target_file):
	$(file >$(target_file),$(contents))
	$(info Updated `$(target_file)`.)
	@true

$(target_file_nat):
	$(file >$(target_file_nat),$(contents_nat))
	$(info Updated `$(target_file_nat)`.)
	@true

# $(error $(contents))
# $(error $(contents_nat))
