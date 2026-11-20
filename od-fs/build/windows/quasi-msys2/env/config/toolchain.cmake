# This file contains CMake configuration.

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CROSSCOMPILING FALSE)

# Note! Appending instead of overwriting, to allow the user to specify extra custom paths.
# Because `CMAKE_FIND_ROOT_PATH` is really the only variable that lets us do it sanely.
# Even Android NDK does this! https://github.com/android/ndk/issues/912
list(APPEND CMAKE_FIND_ROOT_PATH "$ENV{MSYSTEM_PREFIX}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Surprised this isn't automatic. Some libraries choke without this.
set(CMAKE_SYSTEM_PROCESSOR x86_64)
