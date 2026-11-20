# CMake toolchain file for cross-compiling to aarch64-linux-gnu with Ubuntu's cross compiler
# (gcc-aarch64-linux-gnu in the multi-arch image, see the Dockerfile). Used by the env system's
# cross mode (od-fs/build/env/setup --host aarch64-linux-gnu).
#
# The cross compiler finds the target's headers and libraries in the multiarch directories
# (/usr/include/aarch64-linux-gnu, /usr/lib/aarch64-linux-gnu), where the target's development
# packages are installed, and CMake searches them (CMAKE_LIBRARY_ARCHITECTURE comes from the
# compiler).
# pkg-config gets the target's .pc files from PKG_CONFIG_LIBDIR (set below and by the cross mode),
# and the env prefix's from PKG_CONFIG_PATH.

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_AR aarch64-linux-gnu-gcc-ar)
set(CMAKE_RANLIB aarch64-linux-gnu-gcc-ranlib)
set(CMAKE_STRIP aarch64-linux-gnu-strip)

# Programs found by CMake run on the build machine.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(ENV{PKG_CONFIG_LIBDIR} "/usr/lib/aarch64-linux-gnu/pkgconfig:/usr/share/pkgconfig")
