# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])

AC_INIT([FS-UAE], [5.0.8], [frode@fs-uae.net], [fs-uae], [https://fs-uae.net])

AC_CONFIG_AUX_DIR([od-fs/build/build-aux])
AC_CANONICAL_HOST
AC_CONFIG_SRCDIR([./main.cpp])
AC_CONFIG_HEADERS([od-fs/config.h])

PACKAGE_VERSION_MAJOR=${PACKAGE_VERSION%.*.*}
PACKAGE_VERSION_MINOR=${PACKAGE_VERSION%.*}; PACKAGE_VERSION_MINOR=${PACKAGE_VERSION_MINOR#*.}
PACKAGE_VERSION_SUBMINOR=${PACKAGE_VERSION#*.*.}
AC_SUBST([PACKAGE_VERSION_MAJOR])
AC_SUBST([PACKAGE_VERSION_MINOR])
AC_SUBST([PACKAGE_VERSION_SUBMINOR])

#AC_DEFINE([FSBUILD_VERSION], ["fsbuild_version"], [Version])
#AC_DEFINE_UNQUOTED([FSBUILD_VERSION_MAJOR], [fsbuild_version_major], [Major])
#AC_DEFINE_UNQUOTED([FSBUILD_VERSION_MINOR], [fsbuild_version_minor], [Minor])
#AC_DEFINE_UNQUOTED(
#    [FSBUILD_VERSION_REVISION], [fsbuild_version_revision], [Revision])
#AC_DEFINE_UNQUOTED([FSBUILD_COMMIT], ["fsbuild_commit"], [Commit])

#AC_SUBST(FSBUILD_VERSION)
#AC_SUBST(FSBUILD_VERSION_MAJOR)
#AC_SUBST(FSBUILD_VERSION_MINOR)
#AC_SUBST(FSBUILD_VERSION_REVISION)
#AC_SUBST(FSBUILD_COMMIT)

AC_CONFIG_MACRO_DIRS([od-fs/build/m4])

AM_INIT_AUTOMAKE([tar-ustar subdir-objects])

AC_SUBST([PACKAGE_SERIES], [devel])

AC_DEFINE_UNQUOTED([UAE_BASE_VERSION], ["WinUAE 6.0.1"],
[Which UAE core this version is based on.])

PACKAGE_MAJOR=`echo ${PACKAGE_VERSION} | sed -e 's,\(\.*\)\..*\..*,\1,g'`
PACKAGE_MINOR=`echo ${PACKAGE_VERSION} | sed -e 's,.*\.\(.*\)\..*,\1,g'`
PACKAGE_REVISION=`echo ${PACKAGE_VERSION} | sed -e 's,.*\..*\.\([[0-9]]*\).*,\1,g'`
AC_SUBST(PACKAGE_MAJOR)
AC_DEFINE_UNQUOTED([PACKAGE_MAJOR], [$PACKAGE_MAJOR], [Package major version])
AC_SUBST(PACKAGE_MINOR)
AC_DEFINE_UNQUOTED([PACKAGE_MINOR], [$PACKAGE_MINOR], [Package minor version])
AC_SUBST(PACKAGE_REVISION)
AC_DEFINE_UNQUOTED([PACKAGE_REVISION], [$PACKAGE_REVISION], [Package revision])


# Checks for programs

# Python first, since the version and commit below come from od-fs/build/dist/version.py.
AM_PATH_PYTHON([3.12])

# The version FS-UAE shows (in the log and the About window, and in the Windows version
# information), and the dists and source tarballs are named for. Unless FSUAE_VERSION is set (as the
# dist scripts do), it is:
# - in a source tarball, the version it was made for (.tarball-version, written by make dist);
# - in a git checkout, the version od-fs/build/dist/version.py makes from git: <version> for the
#   commit tagged v<version>, otherwise a development version with the commit's hash, so that
#   development builds identify themselves (the same in a shallow clone); or PACKAGE_VERSION, with a
#   warning, when that fails (no git, a wrong tag), since a development build should not fail for
#   it;
# - otherwise PACKAGE_VERSION (the version above).
# It is the version when configure ran: make does not update it after a commit (run configure
# again). It must start with PACKAGE_VERSION, and only have letters, digits and ".+-".
AC_ARG_VAR([FSUAE_VERSION],
    [the version FS-UAE shows (default: from .tarball-version or git, else the package version)])
FSUAE_VERSION_FROM_GIT=no
AS_IF([test -z "$FSUAE_VERSION" && test -f "$srcdir/.tarball-version"], [
    FSUAE_VERSION=$(cat "$srcdir/.tarball-version")
    AC_MSG_NOTICE([the version from .tarball-version: $FSUAE_VERSION])
])
AS_IF([test -z "$FSUAE_VERSION" && test -e "$srcdir/.git"], [
    AC_MSG_CHECKING([for the version from git])
    AS_IF([fsuae_git_version=$("$PYTHON" "$srcdir/od-fs/build/dist/version.py" "$srcdir" \
            2> conftest.err)], [
        FSUAE_VERSION="$fsuae_git_version"
        FSUAE_VERSION_FROM_GIT=yes
        AC_MSG_RESULT([$FSUAE_VERSION])
    ], [
        AC_MSG_RESULT([none])
        AC_MSG_WARN([$(sed 's/^Error: //' conftest.err)])
        AC_MSG_WARN([using the version $PACKAGE_VERSION])
    ])
    rm -f conftest.err
])
# Whether the version came from git, so that make dist can check that it is still the checkout's.
AC_SUBST([FSUAE_VERSION_FROM_GIT])
AS_IF([test -z "$FSUAE_VERSION"], [FSUAE_VERSION="$PACKAGE_VERSION"])
case "$FSUAE_VERSION" in
    *[[!A-Za-z0-9.+-]]*)
        AC_MSG_ERROR([FSUAE_VERSION ($FSUAE_VERSION) may only have letters, digits and ".+-"])
        ;;
    "$PACKAGE_VERSION" | "$PACKAGE_VERSION"-* | "$PACKAGE_VERSION"+*)
        ;;
    *)
        AC_MSG_ERROR([FSUAE_VERSION ($FSUAE_VERSION) must start with the version $PACKAGE_VERSION])
        ;;
esac

# The build's identity, which FS-UAE logs in the build header at the top of FS-UAE.log.txt (see
# od-fs/fsuae/fsuae-buildinfo.cpp), and shows in the About window (the version and origin).
# They go into od-fs/fsuae-build.h, not config.h, so that configuring again (with a new version
# after a commit, and a new build date) only rebuilds the file that uses them.
#
# FSUAE_BUILD_ORIGIN says who made the build. A build that does not set it is "Unofficial build",
# so that no build claims to be official by accident; the official dists and packages set it (see
# od-fs/build/dist/dist and od-fs/build/dist/obs/obs-origin.sh). FSUAE_BUILD_COMMIT is the commit
# it is made from, when known (from git in a git checkout, unless set), and FSUAE_BUILD_ENVIRONMENT
# what it is made in (for example a Docker image or a GitHub runner image), when known; the dist
# scripts set both. The build date is the time configure ran (dists are configured and built from
# scratch), or SOURCE_DATE_EPOCH when set, so that builds can be reproducible.
FSUAE_IDENTITY_CHARACTERS='A-Za-z0-9 .,:;()/+_#@=~-'
AC_DEFUN([FSUAE_CHECK_IDENTITY], [
    case "$$1" in
        *[[!A-Za-z0-9\ .,:\;\(\)/+_#@=~-]]*)
            AC_MSG_ERROR([$1 ($$1) may only have the characters $FSUAE_IDENTITY_CHARACTERS])
            ;;
    esac
    AS_IF([test ${#$1} -gt 200], [AC_MSG_ERROR([$1 is longer than 200 characters])])
])
AC_ARG_VAR([FSUAE_BUILD_ORIGIN],
    [who made the build, as FS-UAE shows it (default: Unofficial build)])
AS_IF([test -z "$FSUAE_BUILD_ORIGIN"], [FSUAE_BUILD_ORIGIN="Unofficial build"])
FSUAE_CHECK_IDENTITY([FSUAE_BUILD_ORIGIN])
AC_ARG_VAR([FSUAE_BUILD_COMMIT],
    [the commit FS-UAE is built from (default: from git, else unknown)])
# (Quietly "unknown" when git fails: a checkout without git has had a warning about the version
# above, unless FSUAE_VERSION was set.)
AS_IF([test -z "$FSUAE_BUILD_COMMIT" && test ! -f "$srcdir/.tarball-version" &&
        test -e "$srcdir/.git"], [
    FSUAE_BUILD_COMMIT=$("$PYTHON" "$srcdir/od-fs/build/dist/version.py" --commit "$srcdir" \
        2> /dev/null)
])
AS_IF([test -z "$FSUAE_BUILD_COMMIT"], [FSUAE_BUILD_COMMIT="unknown"])
case "$FSUAE_BUILD_COMMIT" in
    unknown) ;;
    *[[!0-9a-f]]* | ?????? | ????? | ???? | ??? | ?? | ?)
        AC_MSG_ERROR(
            [FSUAE_BUILD_COMMIT ($FSUAE_BUILD_COMMIT) must be a commit hash (7 or more hex digits)])
        ;;
esac
FSUAE_CHECK_IDENTITY([FSUAE_BUILD_COMMIT])
AC_ARG_VAR([FSUAE_BUILD_ENVIRONMENT],
    [what FS-UAE is built in, such as a Docker or runner image (default: unknown)])
AS_IF([test -z "$FSUAE_BUILD_ENVIRONMENT"], [FSUAE_BUILD_ENVIRONMENT="unknown"])
FSUAE_CHECK_IDENTITY([FSUAE_BUILD_ENVIRONMENT])
AS_IF([test -n "$SOURCE_DATE_EPOCH"], [
    case "$SOURCE_DATE_EPOCH" in
        *[[!0-9]]*) AC_MSG_ERROR([SOURCE_DATE_EPOCH ($SOURCE_DATE_EPOCH) must be a number]) ;;
    esac
    # GNU date, or BSD date (macOS).
    FSUAE_BUILD_DATE=$(date -u -d "@$SOURCE_DATE_EPOCH" "+%Y-%m-%d %H:%M:%S UTC" 2>/dev/null ||
        date -u -r "$SOURCE_DATE_EPOCH" "+%Y-%m-%d %H:%M:%S UTC")
], [
    FSUAE_BUILD_DATE=$(date -u "+%Y-%m-%d %H:%M:%S UTC")
])
AC_SUBST([FSUAE_BUILD_DATE])

# The other programs

AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
# AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AM_PROG_AR
AM_PROG_CC_C_O
# AC_PATH_TOOL(OBJCOPY, objcopy)
AC_PATH_PROG([MSGFMT], [msgfmt], [not found])
if test "$MSGFMT" = "not found"; then
  AC_MSG_ERROR([msgfmt not found on path])
fi
AC_PATH_PROG([ZIP], [zip], [not found])
if test "$ZIP" = "not found"; then
  AC_MSG_ERROR([zip not found on path])
fi
PKG_PROG_PKG_CONFIG

AC_CHECK_TOOL([WINDRES], [windres], [:])

# Needed for x86_64-w64-mingw32 cross compilation and std::mutex support via <mutex>
AX_PTHREAD

# Feature test macros

AC_DEFUN([OPT_FEATURE], [
AC_ARG_ENABLE([$3], [AS_HELP_STRING([--disable-$3], [$4])])
AS_IF([test x$enable_$2 != xno], [AC_DEFINE([$1], [1], [$4])])
AM_CONDITIONAL([$1], [test x$enable_$2 != xno])
])

AC_DEFUN([DIS_FEATURE], [
AC_ARG_ENABLE([$3], [AS_HELP_STRING([--enable-$3], [$4])])
AS_IF([test x$enable_$2 = xyes], [AC_DEFINE([$1], [1], [$4])])
AM_CONDITIONAL([$1], [test x$enable_$2 = xyes])
])

AC_DEFUN([REQ_FEATURE], [
AC_DEFINE([$1], [1], [$4])
AM_CONDITIONAL([$1], [true])
])


# Misc system checks

AC_SYS_LARGEFILE
# AC_LTDL_SHLIBEXT


# Static build

DIS_FEATURE([STATIC_BUILD], [static], [static], [static build (works on Windows)])

AS_IF([test "x$enable_static" = xyes], [PKG_CONFIG="$PKG_CONFIG --static"])

# The dist layout: FS-UAE finds its Python code, Python standard library, resources and plugins in
# folders relative to its executable, as a dist has them (od-fs/fsuae/fsuae-path.cpp).
# od-fs/configure (every development build) and the dist build (od-fs/build/dist/build.sh) turn it
# on. Off by default, so that a plain "./configure && make install" (Linux distributions, make
# distcheck) gets the system install layout: <bindir>/../share/<share dir name>, where
# install-data-local (Makefile.am) puts the files. Portable mode (Portable.ini, the data and cache
# directories) is decided at run time and has nothing to do with it.

DIS_FEATURE([DIST_LAYOUT], [dist_layout], [dist-layout],
    [find the Python code and resources relative to the executable, as in a dist])

# Host triplet in the form the dists use for Libs/<triplet> and their archive names (the same names
# as od-fs/build/triplet.sh gives): config.sub's canonical $host has a vendor part on Linux
# (x86_64-pc-linux-gnu), turns arm64 into aarch64 and adds a version to darwin. FS-UAE uses it to
# find its Python home in the dist layout (Libs/<triplet>, fsuae-path.cpp), and the dist scripts
# read it from the configured build (HOST_TRIPLET in the Makefile). Other hosts keep $host.
# REVIEW: The arm-linux-gnueabihf and darwin mappings are untested (no 32-bit ARM build yet, not
# run on a Mac; native configures on a Mac add a Darwin version, e.g. aarch64-apple-darwin23.1.0).

AC_MSG_CHECKING([the triplet for the dist and the Python home])
AS_CASE([$host_cpu-$host_os],
[x86_64-linux-gnu], [fsuae_host_triplet=x86_64-linux-gnu],
[aarch64-linux-gnu], [fsuae_host_triplet=aarch64-linux-gnu],
[arm*-linux-gnueabihf], [fsuae_host_triplet=arm-linux-gnueabihf],
[x86_64-darwin*], [fsuae_host_triplet=x86_64-apple-darwin],
[aarch64-darwin* | arm64-darwin*], [fsuae_host_triplet=arm64-apple-darwin],
[x86_64-mingw32*], [fsuae_host_triplet=x86_64-w64-mingw32],
[fsuae_host_triplet=$host])
AC_MSG_RESULT([$fsuae_host_triplet])
AC_DEFINE_UNQUOTED([HOST_TRIPLET], ["$fsuae_host_triplet"], [Host triplet, as the dists name it])
AC_SUBST([HOST_TRIPLET], [$fsuae_host_triplet])

# Share directory name

AC_ARG_WITH([share-dir-name], AS_HELP_STRING([--with-share-dir-name=NAME], [set share directory name (default: fs-uae)]))
AS_IF([test "x$with_share_dir_name" = "x"], [with_share_dir_name="fs-uae"])
AC_SUBST([SHARE_DIR_NAME], ["$with_share_dir_name"])
AC_DEFINE_UNQUOTED([SHARE_DIR_NAME], ["$with_share_dir_name"], [Share directory name])

# Compiler / linker feature checks

AX_CHECK_COMPILE_FLAG(-fno-strict-overflow,
[f_no_strict_overflow=yes],
[f_no_strict_overflow=no])
AM_CONDITIONAL([F_NO_STRICT_OVERFLOW],
[test x$f_no_strict_overflow = xyes])

AX_CHECK_COMPILE_FLAG(-fno-strict-aliasing,
[f_no_strict_aliasing=yes],
[f_no_strict_aliasing=no])
AM_CONDITIONAL([F_NO_STRICT_ALIASING],
[test x$f_no_strict_aliasing = xyes])

AX_CHECK_COMPILE_FLAG(-Wmissing-declarations,
[w_missing_declarations=yes],
[w_missing_declarations=no])
AM_CONDITIONAL([W_MISSING_DECLARATIONS],
[test x$w_missing_declarations = xyes])

AX_CHECK_COMPILE_FLAG(-Wpointer-arith,
[w_pointer_arith=yes],
[w_pointer_arith=no])
AM_CONDITIONAL([W_POINTER_ARITH],
[test x$w_pointer_arith = xyes])

AX_CHECK_COMPILE_FLAG(-Werror=missing-declarations,
[w_error_missing_declarations=yes],
[w_error_missing_declarations=no])
AM_CONDITIONAL([W_ERROR_MISSING_DECLARATIONS],
[test x$w_error_missing_declarations = xyes])

AX_CHECK_COMPILE_FLAG(-Wno-format-truncation,
[w_no_format_truncation=yes],
[w_no_format_truncation=no])
AM_CONDITIONAL([W_NO_FORMAT_TRUNCATION],
[test x$w_no_format_truncation = xyes])

AX_CHECK_COMPILE_FLAG(-Waddress,
[w_address=yes],
[w_address=no])
AM_CONDITIONAL([W_ADDRESS],
[test x$w_address = xyes])

AX_CHECK_COMPILE_FLAG(-Wmissing-format-attribute,
[w_missing_format_attribute=yes],
[w_missing_format_attribute=no])
AM_CONDITIONAL([W_MISSING_FORMAT_ATTRIBUTE],
[test x$w_missing_format_attribute = xyes])

AX_CHECK_COMPILE_FLAG(-Wformat-overflow,
[w_format_overflow=yes],
[w_format_overflow=no])
AM_CONDITIONAL([W_FORMAT_OVERFLOW],
[test x$w_format_overflow = xyes])

AX_CHECK_COMPILE_FLAG(-Wc++11-narrowing,
[w_cpp11_narrowing=yes],
[w_cpp11_narrowing=no])
AM_CONDITIONAL([W_CPP11_NARROWING],
[test x$w_cpp11_narrowing = xyes])

AX_CHECK_COMPILE_FLAG(-fno-pie,
[fno_pie=yes],
[fno_pie=no])
AM_CONDITIONAL([FNO_PIE],
[test x$fno_pie = xyes])

AX_CHECK_COMPILE_FLAG(-fno-PIE,
[fno_pie_cap=yes],
[fno_pie_cap=no])
AM_CONDITIONAL([FNO_PIE_CAP],
[test x$fno_pie_cap = xyes])

AX_CHECK_LINK_FLAG(-no-pie,
[no_pie=yes],
[no_pie=no])
AM_CONDITIONAL([NO_PIE],
[test x$no_pie = xyes])

# AX_CHECK_LINK_FLAG([-Wl,-Tdata=0x10000000],
# [wl_tdata=yes],
# [wl_tdata=no])
# AM_CONDITIONAL([WL_TDATA],
# [test x$wl_tdata = xyes])


# Checks for libraries.

AC_SUBST([CARBON_LIBS])
AS_CASE([$host_os],
[darwin*], [
    AC_MSG_NOTICE([Using Carbon.framework])
    CARBON_LIBS="-framework Carbon -framework Cocoa"
])

AC_ARG_WITH(alsa, AS_HELP_STRING([--with-alsa], [use ALSA library]))
AS_IF([test "x$with_alsa" = xyes], [
    PKG_CHECK_MODULES([ALSA], [alsa])
    AC_DEFINE([WITH_ALSA], [1], [Define to 1 to use ALSA])
    AC_DEFINE([FSEMU_ALSA], [1], [Define to 1 to use ALSA])
])

AC_CHECK_LIB([compat], [ftime])

AC_CHECK_LIB([dl], [dlopen])

# not using freetype right now
# AC_DEFINE([USE_FREETYPE], [1], [Define to 1 to use FreeType])
# PKG_CHECK_MODULES([FREETYPE], [freetype2])
AC_SUBST([FREETYPE_CFLAGS])
AC_SUBST([FREETYPE_LIBS])

AC_DEFINE([USE_GLIB], [1], [Define to 1 to use GLib])
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.32 gthread-2.0])

AC_ARG_WITH(libmpeg2, AS_HELP_STRING([--without-libmpeg2], [Disable libmpeg2 usage]))
AS_IF([test "x$with_libmpeg2" != xno], [
    PKG_CHECK_MODULES([LIBMPEG2], [libmpeg2 libmpeg2convert])
    AC_DEFINE([WITH_LIBMPEG2], [1], [Define to 1 to use libmpeg2])
])
AM_CONDITIONAL([WITH_LIBMPEG2], [test x$with_libmpeg2 != xno])

AM_CONDITIONAL([BUILTIN_MANYMOUSE], [true])

AC_SUBST([IOKIT_LIBS])
AS_CASE([$host_os],
[darwin*], [
    AC_MSG_NOTICE([Using IOKit.framework])
    IOKIT_LIBS="-framework IOKit"
])

AC_CHECK_LIB([m], [floor])

# Workaround for bug in libsdl2-dev / ubuntu, SDL headers
# depend on mirclient/mircommon headers.
# https://bugs.launchpad.net/ubuntu/+source/libsdl2/+bug/1306629
PKG_CHECK_MODULES([MIR], [mirclient], [
], [
    AC_SUBST([MIR_CFLAGS])
])

AC_DEFINE([USE_OPENGL], [1], [Define to 1 to use OpenGL])
AS_CASE([$host_os],
[darwin*], [
    AC_MSG_NOTICE([Using OpenGL.framework])
    OPENGL_LIBS="-framework OpenGL"
    AC_SUBST([OPENGL_CFLAGS])
    AC_SUBST([OPENGL_LIBS])
],
[mingw*], [
    AC_MSG_NOTICE([Linking with -lopengl32 for OpenGL])
    AC_CHECK_LIB([opengl32], [main], [OPENGL_LIBS=-lopengl32])
    AC_SUBST([OPENGL_CFLAGS])
    AC_SUBST([OPENGL_LIBS])
],
[
    PKG_CHECK_MODULES([OPENGL], [gl])
])

# midi support via PortMidi
AC_ARG_WITH([midi],
    [AS_HELP_STRING([--without-midi], [serial Midi support via PortMidi])])
AS_IF([test "x$with_midi" != xno], [
    AC_CHECK_LIB([portmidi], [Pm_Initialize],[FOUND_PORTMIDI_LIB=yes])
    AC_CHECK_HEADERS([portmidi.h porttime.h],[FOUND_PORTMIDI_HDR=yes])
    AS_IF([test "x$FOUND_PORTMIDI_LIB$FOUND_PORTMIDI_HDR" = xyesyes], [
        AC_MSG_NOTICE([enabling midi support])
        MIDI_LIBS="-lportmidi"
        AC_SUBST([MIDI_LIBS])
        AC_DEFINE([WITH_MIDI], [1], [Define to 1 to use midi])
    ])
])

AC_DEFINE([USE_PNG], [1], [Define to 1 to use libpng])
AC_DEFINE([USE_SDL3], [1], [Define to 1 to use SDL3])
AC_DEFINE([USE_SDL_VIDEO], [1], [Define to 1 to use SDL for video])
AC_DEFINE([USE_SDL_INPUT], [1], [Define to 1 to use SDL for input])

PKG_CHECK_MODULES([FLAC], [flac])
PKG_CHECK_MODULES([MPG123], [libmpg123])
PKG_CHECK_MODULES([PNG], [libpng])
PKG_CHECK_MODULES([PYTHON3], [python3-embed >= 3.12])
PKG_CHECK_MODULES([SDL3_IMAGE], [sdl3-image])
PKG_CHECK_MODULES([SDL3_TTF], [sdl3-ttf])
PKG_CHECK_MODULES([SDL3], [sdl3])
PKG_CHECK_MODULES([ZSTD], [libzstd])

AS_CASE([$host_os],
[mingw*], [],
[darwin*], [],
[
    AC_ARG_WITH(x11, AS_HELP_STRING(
        [--without-x11], [Disable X11 video usage]))
    AS_IF([test "x$with_x11" != xno], [
        X11_CFLAGS=""
        # X11_LIBS="-lX11"
        X11_LIBS=""
        AC_DEFINE([USE_X11], [1], [Define to 1 to use X11])
        # X11_LIBS="$X11_LIBS -lXtst"
        # AC_DEFINE([USE_X11_XTEST], [1], [Define to 1 to use X11 XTEST extension])
    ], [
        X11_CFLAGS="-DSUPPORT_XINPUT2=0"
        X11_LIBS=""
    ])
])
AC_SUBST([X11_CFLAGS])
AC_SUBST([X11_LIBS])

AC_DEFINE([USE_ZLIB], [1], [Define to 1 to use zlib])
PKG_CHECK_MODULES([ZLIB], [zlib], [
], [
    AC_CHECK_LIB([z], [compress])
    AC_SUBST([ZLIB_CFLAGS])
    AC_SUBST([ZLIB_LIBS])
])

# Operating system

AC_SUBST([OS_CPPFLAGS])
AC_SUBST([OS_LDFLAGS])
AC_SUBST([OS_NAME])
AC_SUBST([OS_NAME_2])
AC_SUBST([OS_NAME_3])
AC_SUBST([PACKAGE_ARCH])
AC_SUBST([SLIRP_LIBS])
AC_SUBST([HOST_OS], $host_os)
AC_SUBST([HOST_CPU], $host_cpu)

AS_CASE([$host_os],
[dummy*], [

# --------------------------------------------------------------------------------------------------
], [mingw*], [

OS_NAME="windows"
OS_NAME_2="Windows"
OS_NAME_3="Windows"
OS_CPPFLAGS="-DWINDOWS"

# When cross-compiling on a case-sensitive file system (e.g. Linux), mixed-case includes such as
# <Windows.h> in shared WinUAE/86Box code do not match MinGW's lowercase headers. If so, add
# forwarding headers, searched after the system directories (see the directory's README.md).
# This tests for the problem directly instead of checking $build_os, since cross environments
# like quasi-msys2 set build_alias to the MinGW triplet as well.
AC_MSG_CHECKING([whether <Windows.h> can be included with mixed case])
AC_PREPROC_IFELSE([AC_LANG_SOURCE([[#include <Windows.h>]])], [
    AC_MSG_RESULT([yes])
], [
    AC_MSG_RESULT([no])
    MINGW_CROSS_INCLUDE_DIR="$(cd "$srcdir" && pwd)/od-fs/mingw-cross-include"
    AC_MSG_NOTICE([Adding $MINGW_CROSS_INCLUDE_DIR (-idirafter) for mixed-case Windows headers])
    CPPFLAGS="$CPPFLAGS -idirafter $MINGW_CROSS_INCLUDE_DIR"
])

AS_CASE([$host_cpu],
[x86_64], [
    OS_LDFLAGS="-mwindows -Wl,--image-base=0x400000"
    PACKAGE_ARCH="x86-64"
], [
    OS_LDFLAGS="-mwindows -Wl,--large-address-aware"
    PACKAGE_ARCH="x86"
])
LOADABLE_MODULE_EXT=".dll"

# FIXME: Check if needed
AC_CHECK_LIB([winmm], [main])

AC_CHECK_LIB([ws2_32], [main])
AC_CHECK_LIB([gdi32], [main])
AC_CHECK_LIB([iphlpapi], [main])

# --------------------------------------------------------------------------------------------------
], [linux*], [

OS_NAME="linux"
OS_NAME_2="Linux"
OS_NAME_3="Linux"
OS_CPPFLAGS="-DLINUX"
LOADABLE_MODULE_EXT=".so"

# --------------------------------------------------------------------------------------------------
], [darwin*], [

OS_NAME="macos"
OS_NAME_2="macOS"
OS_NAME_3="macOS"
OS_CPPFLAGS="-DMACOSX -DMACOS"
LOADABLE_MODULE_EXT=".so"
AS_CASE([$host_cpu],
[x86_64], [
    OS_LDFLAGS="-headerpad_max_install_names -pagezero_size 0x2000 -rpath @executable_path/../Frameworks"
],
[
    OS_LDFLAGS="-headerpad_max_install_names -rpath @executable_path/../Frameworks"
])
SLIRP_LIBS="-lresolv"

# --------------------------------------------------------------------------------------------------
], [freebsd*], [

OS_NAME="freebsd"
OS_NAME_2="FreeBSD"
OS_NAME_3="FreeBSD"
OS_CPPFLAGS="-DFREEBSD"
LOADABLE_MODULE_EXT=".so"

# --------------------------------------------------------------------------------------------------
], [openbsd*], [

OS_NAME="openbsd"
OS_NAME_2="OpenBSD"
OS_NAME_3="OpenBSD"
OS_CPPFLAGS="-DOPENBSD"
LOADABLE_MODULE_EXT=".so"

# --------------------------------------------------------------------------------------------------
], [dragonfly*], [

OS_NAME="dragonflybsd"
OS_NAME_2="DragonFlyBSD"
OS_NAME_3="DragonFlyBSD"
OS_CPPFLAGS="-DDRAGONFLYBSD"
LOADABLE_MODULE_EXT=".so"

# --------------------------------------------------------------------------------------------------
], [

OS_NAME="unknown"
OS_NAME_2="Unknown OS"
OS_NAME_3="Unknown"
LOADABLE_MODULE_EXT=".unknown"

# --------------------------------------------------------------------------------------------------

])

AM_CONDITIONAL([LINUX], [test x$OS_NAME = xlinux])
AM_CONDITIONAL([MACOS], [test x$OS_NAME = xmacos])
AM_CONDITIONAL([WINDOWS], [test x$OS_NAME = xwindows])

# Native packet networking for A2065 and uaenet.device (optional, auto-detected).
# Windows: od-win32/win32_uaenet.cpp loads Npcap (wpcap.dll/packet.dll) at
# runtime, so only the Npcap SDK headers are needed at build time.
# Other platforms: od-unix/uaenet_unix.cpp links against libpcap (and uses
# TAP/TUN on Linux).
AC_ARG_WITH([pcap],
    [AS_HELP_STRING([--without-pcap], [native pcap/TAP/TUN networking])])
with_uaenet_pcap=no
AS_IF([test "x$with_pcap" != xno], [
    AS_IF([test "x$OS_NAME" = xwindows], [
        AC_CHECK_HEADER([pcap.h], [
            AC_CHECK_HEADER([Packet32.h], [with_uaenet_pcap=yes], [],
                [#include <winsock2.h>
#include <pcap.h>])
        ], [], [#include <winsock2.h>])
    ], [
        PKG_CHECK_MODULES([PCAP], [libpcap], [with_uaenet_pcap=yes], [
            AC_CHECK_HEADER([pcap/pcap.h], [
                AC_CHECK_LIB([pcap], [pcap_open_live], [
                    PCAP_CFLAGS=""
                    PCAP_LIBS="-lpcap"
                    with_uaenet_pcap=yes
                ])
            ])
        ])
    ])
    AS_IF([test "x$with_uaenet_pcap" = xyes], [
        AC_MSG_NOTICE([enabling native pcap networking])
        AC_DEFINE([WITH_UAENET_PCAP], [1],
            [Define to 1 for native pcap/TAP/TUN networking])
    ], [
        AS_IF([test "x$with_pcap" = xyes],
            [AC_MSG_ERROR([pcap requested but not found])])
    ])
])
AC_SUBST([PCAP_CFLAGS])
AC_SUBST([PCAP_LIBS])
AM_CONDITIONAL([WITH_UAENET_PCAP], [test x$with_uaenet_pcap = xyes])

# Not using libtool right now, but use name LT_MODULE_EXT anyway in case
# transitioning to libtool later.
AC_DEFINE_UNQUOTED([LT_MODULE_EXT], ["$LOADABLE_MODULE_EXT"],
[Define to the extension used for runtime loadable modules, say, ".so".])

AC_DEFINE_UNQUOTED([OS_NAME], ["$OS_NAME"],
[Define to a name identifier for the operating system, "linux".])

AC_DEFINE_UNQUOTED([OS_NAME_2], ["$OS_NAME_2"],
[Define to a nicely formatted name for the operating system, "Linux".])

AC_DEFINE_UNQUOTED([OS_NAME_3], ["$OS_NAME_3"],
[New name identifier for the operating system, "Linux".])


# Checks for builtins

AX_GCC_BUILTIN(__builtin_bswap16)
AX_GCC_BUILTIN(__builtin_bswap32)
AX_GCC_BUILTIN(__builtin_bswap64)
AX_GCC_BUILTIN(__builtin_expect)


# Checks for header files.

AC_CHECK_HEADERS([AL/al.h])
AC_CHECK_HEADERS([AL/alc.h])
AC_CHECK_HEADERS([arpa/inet.h])
AC_CHECK_HEADERS([byteswap.h])
AC_CHECK_HEADERS([dlfcn.h])
AC_CHECK_HEADERS([endian.h])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([fenv.h])
AC_CHECK_HEADERS([float.h])
AC_CHECK_HEADERS([inttypes.h])
# AC_CHECK_HEADERS([sys/ipc.h])
AC_CHECK_HEADERS([libkern/OSByteOrder.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([netdb.h])
AC_CHECK_HEADERS([netinet/in.h])
# AC_CHECK_HEADERS([OpenAL/al.h])
# AC_CHECK_HEADERS([OpenAL/alc.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADERS([stdint.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([strings.h])
AC_CHECK_HEADERS([sys/endian.h])
AC_CHECK_HEADERS([sys/filsys.h])
AC_CHECK_HEADERS([sys/fs/s5param.h])
AC_CHECK_HEADERS([sys/ioctl.h])
# AC_CHECK_HEADERS([sys/mman.h])
AC_CHECK_HEADERS([sys/mount.h])
AC_CHECK_HEADERS([sys/param.h])
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([sys/statfs.h])
AC_CHECK_HEADERS([sys/statvfs.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([sys/timeb.h])
AC_CHECK_HEADERS([sys/vfs.h])
AC_CHECK_HEADERS([termios.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([utime.h])
AC_CHECK_HEADERS([values.h])
AC_CHECK_HEADERS([wchar.h])


# Checks for typedefs, structures, and compiler characteristics.

AC_C_BIGENDIAN
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT
AC_C_VOLATILE

AX_GCC_FUNC_ATTRIBUTE(always_inline)
AX_GCC_FUNC_ATTRIBUTE(fastcall)
AX_GCC_FUNC_ATTRIBUTE(format)
AX_GCC_FUNC_ATTRIBUTE(noinline)
AX_GCC_FUNC_ATTRIBUTE(noreturn)
AX_GCC_FUNC_ATTRIBUTE(regparm)
AX_GCC_FUNC_ATTRIBUTE(unused)

AX_GCC_VAR_ATTRIBUTE(unused)

# FIXME: not found on the mingw box
# AC_CHECK_HEADER_STDBOOL

AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_CHECK_MEMBERS([struct stat.st_mtim.tv_nsec])
AC_CHECK_MEMBERS([struct stat.st_mtimespec.tv_nsec])
AC_CHECK_MEMBERS([struct stat.st_mtime_nsec])
AC_CHECK_MEMBERS([struct in_addr.S_un])
AC_CHECK_MEMBERS([struct ucontext.uc_mcontext.gregs],
    [], [], [[#include <sys/ucontext.h>]])
AC_CHECK_MEMBERS([struct ucontext_t.uc_mcontext.gregs],
    [], [], [[#include <sys/ucontext.h>]])

AC_CHECK_TYPES([ptrdiff_t])
AC_STRUCT_ST_BLOCKS
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([__uint128_t])
AC_TYPE_LONG_LONG_INT

AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(long long int)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(void *)

# Checks for library functions.

AC_FUNC_ALLOCA
AC_FUNC_CHOWN
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_FSEEKO
AC_FUNC_MKTIME
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STRCOLL
AC_FUNC_STRTOD
AC_CHECK_FUNCS([atexit])
AC_CHECK_FUNCS([bzero])

# AC_CHECK_FUNCS([bswap_16])
# AC_CHECK_FUNCS([bswap_32])

AC_CHECK_FUNCS([dlopen])
AC_CHECK_FUNCS([dup2])
AC_CHECK_FUNCS([floor])
AC_CHECK_FUNCS([fseeko64])
AC_CHECK_FUNCS([ftello64])
AC_CHECK_FUNCS([ftime])
AC_CHECK_FUNCS([ftruncate])
AC_CHECK_FUNCS([getcwd])
AC_CHECK_FUNCS([gethostbyaddr])
AC_CHECK_FUNCS([gethostbyname])
AC_CHECK_FUNCS([gethostname])
AC_CHECK_FUNCS([getpagesize])
AC_CHECK_FUNCS([gmtime_r])
AC_CHECK_FUNCS([index])
AC_CHECK_FUNCS([inet_aton])
AC_CHECK_FUNCS([inet_ntoa])
AC_CHECK_FUNCS([localeconv])
AC_CHECK_FUNCS([localtime_r])
AC_CHECK_FUNCS([memchr])
AC_CHECK_FUNCS([memmove])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([mkdir])
AC_CHECK_FUNCS([munmap])
AC_CHECK_FUNCS([pow])
AC_CHECK_FUNCS([putenv])
AC_CHECK_FUNCS([rmdir])
AC_CHECK_FUNCS([posix_memalign])
AC_CHECK_FUNCS([select])
AC_CHECK_FUNCS([setenv])
AC_CHECK_FUNCS([setlocale])
AC_CHECK_FUNCS([socket])
AC_CHECK_FUNCS([sqrt])
AC_CHECK_FUNCS([stpcpy])
AC_CHECK_FUNCS([strcasecmp])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strcspn])
AC_CHECK_FUNCS([strdup])
AC_CHECK_FUNCS([strerror])
AC_CHECK_FUNCS([strlcpy])
AC_CHECK_FUNCS([strncasecmp])
AC_CHECK_FUNCS([strpbrk])
AC_CHECK_FUNCS([strrchr])
AC_CHECK_FUNCS([strspn])
AC_CHECK_FUNCS([strstr])
AC_CHECK_FUNCS([strtol])
AC_CHECK_FUNCS([strtoul])
AC_CHECK_FUNCS([strtoull])
AC_CHECK_FUNCS([tzset])
AC_CHECK_FUNCS([utime])


# Check for time functions and whether we have support for monotonic clock.

AC_CHECK_FUNCS([clock_gettime])
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_DECLS([CLOCK_MONOTONIC], [], [], [#include <time.h>])


# RTLD_DEEPBIND is not supported by all C libraries.

AC_CHECK_DECLS([RTLD_DEEPBIND], [], [], [#include <dlfcn.h>])


# Check for POSIX support.

AS_CASE([$host_os],
    [mingw*], [],
    [
        AC_DEFINE([POSIX_SERIAL], [1],
            [Define to 1 if you have POSIX serial support])
    ]
)
#AC_CHECK_FUNC(ioctl, [
#	AC_DEFINE(POSIX_SERIAL)
#])


# Byte swapping

AC_MSG_CHECKING(for bswap_16)
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
    #if HAVE_BYTESWAP_H
    # include <byteswap.h>
    #endif
]], [[
    bswap_16(0x12);
]])],[
    AC_DEFINE(HAVE_BSWAP_16, 1, [Define to 1 if you have the 'bswap_16' function.])
    AC_MSG_RESULT(yes)
],[
    AC_MSG_RESULT(no)
])
AC_MSG_CHECKING(for bswap_32)
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
    #if HAVE_BYTESWAP_H
    # include <byteswap.h>
    #endif
]], [[
    bswap_32(0x1234);
]])],[
    AC_DEFINE(HAVE_BSWAP_32, 1, [Define to 1 if you have the 'bswap_32' function.])
    AC_MSG_RESULT(yes)
],[
    AC_MSG_RESULT(no)
])

AC_CHECK_SIZEOF([void *])
AS_IF([test "$ac_cv_sizeof_void_p" -eq 8], [
    # AC_DEFINE([IS_64BIT], [1], [Define to 1 if the target is 64-bit])
], [
    # AC_DEFINE([IS_32BIT], [1], [Define to 1 if the target is 32-bit])
])
AM_CONDITIONAL([IS_64BIT], [test "$ac_cv_sizeof_void_p" -eq 8])

# More feature macros. Many of these are generally always enabled, and
# compiling without them may not work (without further changes to
# source code).

OPT_FEATURE([A2065], [a2065], [a2065], [A2065 ethernet card])
REQ_FEATURE([A2091], [a2091], [a2091], [A590/A2091 SCSI])
OPT_FEATURE([ACTION_REPLAY], [action_replay], [action-replay], [Action Replay 1/2/3 support])
OPT_FEATURE([AGA], [aga], [aga], [AGA chipset emulation])
DIS_FEATURE([AHI], [ahi], [ahi], [builtin AHI device (unsupported)])
OPT_FEATURE([ARCADIA], [arcadia], [arcadia], [Arcadia arcade system])
REQ_FEATURE([AUTOCONFIG], [autoconfig], [autoconfig], [autoconfig support])
OPT_FEATURE([BSDSOCKET], [bsdsocket], [bsdsocket], [built-in bsdsocket.library])
OPT_FEATURE([CAPS], [caps], [caps], [IPF support via libcapsimage])
OPT_FEATURE([CD32], [cd32], [cd32], [CD32 emulation])
OPT_FEATURE([CDTV], [cdtv], [cdtv], [CDTV emulation])
OPT_FEATURE([WITH_CHD], [chd], [chd], [CHD disk/CD image support])
OPT_FEATURE([WITH_CLIPBOARD], [clipboard], [clipboard], [clipboard integration])
OPT_FEATURE([CODEGEN], [codegen], [codegen], [code generation (disable for cross-compilation)])
OPT_FEATURE([CPUEMU_0], [cpuemu_0], [cpuemu-0], [generic 680x0 emulation])
OPT_FEATURE([CPUEMU_11], [cpuemu_11], [cpuemu-11], [68000/68010 prefetch emulation])
REQ_FEATURE([CPUEMU_13], [cpuemu_13], [cpuemu-13], [68000/68010 cycle-exact cpu&blitter])
OPT_FEATURE([CPUEMU_20], [cpuemu_20], [cpuemu-20], [68020 prefetch])
OPT_FEATURE([CPUEMU_21], [cpuemu_21], [cpuemu-21], [68020 "cycle-exact" + blitter])
OPT_FEATURE([CPUEMU_22], [cpuemu_22], [cpuemu-22], [68030 prefetch])
OPT_FEATURE([CPUEMU_23], [cpuemu_23], [cpuemu-23], [68030 "cycle-exact" + blitter])
OPT_FEATURE([CPUEMU_24], [cpuemu_24], [cpuemu-24], [68060 "cycle-exact" + blitter])
OPT_FEATURE([CPUEMU_25], [cpuemu_25], [cpuemu-25], [68040 "cycle-exact" + blitter (needs cpuemu-24)])
OPT_FEATURE([CPUEMU_31], [cpuemu_31], [cpuemu-31], [Aranym 68040 MMU])
OPT_FEATURE([CPUEMU_32], [cpuemu_32], [cpuemu-32], [Previous 68030 MMU])
OPT_FEATURE([CPUEMU_33], [cpuemu_33], [cpuemu-33], [68060 MMU])
OPT_FEATURE([CPUEMU_34], [cpuemu_34], [cpuemu-34], [MMU + cache])
OPT_FEATURE([CPUEMU_35], [cpuemu_35], [cpuemu-35], [MMU + CE + cache])
OPT_FEATURE([CPUEMU_40], [cpuemu_40], [cpuemu-40], [generic 680x0 with JIT direct memory access])
OPT_FEATURE([CPUEMU_50], [cpuemu_50], [cpuemu-50], [generic 680x0 with indirect memory access])
REQ_FEATURE([WITH_CPUBOARD], [cpuboard], [cpuboard], [cpu board emulation])
OPT_FEATURE([DEBUGGER], [debugger], [debugger], [debugger support (cannot be disabled right now)])
# OPT_FEATURE([A_DMS], [dms], [dms], [DMS support])
OPT_FEATURE([DRIVESOUND], [drivesound], [drivesound], [drive sound emulation])
OPT_FEATURE([FDI2RAW], [fdi2raw], [fdi2raw], [FDI 1.0 and 2.x image support])
REQ_FEATURE([FILESYS], [filesys], [filesys], [directory hard drives])
REQ_FEATURE([FPUEMU], [fpuemu], [fpuemu], [FPU emulation])
DIS_FEATURE([FSE_DRIVERS], [drivers], [drivers], [multiple driver backends (experimental)])
OPT_FEATURE([FLOPPYBRIDGE], [floppybridge], [floppybridge], [FloppyBridge plugin support])
REQ_FEATURE([FSEMU], [fsemu], [fsemu], [new FSEMU backend (experimental)])
OPT_FEATURE([GFXBOARD], [gfxboard], [gfxboard], [GFX hardware boards])

AC_ARG_ENABLE([jit], [AS_HELP_STRING([--disable-jit], [JIT compiler])])
AS_IF([test "x$enable_jit" != xno], [
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    ]], [[
        #if !defined(__i386__) && !defined(__x86_64__) && !defined(__aarch64__)
        #error JIT is only supported on x86/x86-64 and ARM64
        #endif
        #if defined(__OpenBSD__) || defined(__FreeBSD__)
        #error no JIT on OpenBSD/FreeBSD right now
        #endif
    ]])],[
        AC_DEFINE([JIT], [1], [Define to 1 to enable JIT compilation])
    ],[
        if test "x$enable_jit" = xyes; then
            AC_MSG_FAILURE(
                [JIT is not supported on $host])
        else
            enable_jit=no
        fi
    ])
])
AM_CONDITIONAL([JIT],
[test x$enable_jit != xno])

host_aarch64=no
AS_CASE([$host_cpu], [aarch64|arm64], [host_aarch64=yes])
AM_CONDITIONAL([HOST_AARCH64], [test x$host_aarch64 = xyes])

# JIT compilation of FPU instructions requires the JIT
AS_IF([test "x$enable_jit" = xno], [enable_jit_fpu=no])
OPT_FEATURE([USE_JIT_FPU], [jit_fpu], [jit-fpu], [JIT compilation of FPU instructions])
OPT_FEATURE([WITH_NETPLAY], [netplay], [netplay], [netplay support])
OPT_FEATURE([NCR], [ncr], [ncr], [A4000T/A4091, 53C710/53C770 SCSI])
OPT_FEATURE([NCR9X], [ncr9x], [ncr9x], [53C9X SCSI])
OPT_FEATURE([PARALLEL_PORT], [parallel_port], [parallel-port], [parallell port (printer) emu])
REQ_FEATURE([PICASSO96], [picasso96], [picasso96], [Picasso 96 (uaegfx.card)])
OPT_FEATURE([WITH_PPC], [ppc], [ppc], [PPC (requires cpu board)])
OPT_FEATURE([PROWIZARD], [prowizard], [prowizard], [prowizard])
OPT_FEATURE([SAVESTATE], [savestate], [savestate], [save state support])
OPT_FEATURE([SCP], [scp], [scp], [SuperCardPro floppy image support])
OPT_FEATURE([WITH_SEGTRACKER], [segtracker], [segtracker], [segtracker support in debugger])
OPT_FEATURE([SERIAL_PORT], [serial_port], [serial-port], [serial port emulation])
OPT_FEATURE([WITH_SLIRP], [slirp], [slirp], [SLIRP (user mode NAT)])
OPT_FEATURE([WITH_SOFTFLOAT], [softfloat], [softfloat], [use softfloat library])
OPT_FEATURE([WITH_QEMU_CPU], [qemu_cpu], [qemu-cpu], [use QEMU plugin for PPC emulation])
REQ_FEATURE([WITH_THREADED_CPU], [threaded_cpu], [threaded-cpu], [Threaded CPU emulation])
REQ_FEATURE([WITH_TOCCATA], [toccata], [toccata], [Toccata sound card emulation])
OPT_FEATURE([WITH_UAENATIVE], [uaenative], [uaenative], [uaenative.library])
OPT_FEATURE([SANA2], [uaenet], [uaenet], [uaenet.device])
OPT_FEATURE([SCSIEMU], [uaescsi], [uaescsi], [uaescsi.device])
OPT_FEATURE([UAESERIAL], [uaeserial], [uaeserial], [uaeserial.device])
OPT_FEATURE([WITH_VPAR], [vpar], [vpar], [vpar virtual parallel port])

# libslirp (for WITH_SLIRP): the builtin copy in od-fs/3rdparty/libslirp, which is the default so
# that official builds are reproducible, or the system library (for distribution packages).

AC_ARG_WITH([libslirp],
    [AS_HELP_STRING([--with-libslirp=builtin|system],
        [use the builtin libslirp (default) or the system library])],
    [], [with_libslirp=builtin])
AS_CASE([$with_libslirp],
    [builtin|system], [],
    [AC_MSG_ERROR([--with-libslirp must be builtin or system (use --disable-slirp to build without slirp)])])
AS_IF([test "x$enable_slirp" != xno], [
    AS_IF([test "x$with_libslirp" = xsystem], [
        PKG_CHECK_MODULES([LIBSLIRP], [slirp >= 4.6.1])
    ], [
        LIBSLIRP_CFLAGS='-I$(top_srcdir)/od-fs/3rdparty/libslirp/include'
        LIBSLIRP_LIBS=''
    ])
])
AC_SUBST([LIBSLIRP_CFLAGS])
AC_SUBST([LIBSLIRP_LIBS])
AM_CONDITIONAL([WITH_BUILTIN_LIBSLIRP],
    [test "x$enable_slirp" != xno && test "x$with_libslirp" = xbuiltin])

AC_ARG_ENABLE([x86], [AS_HELP_STRING([--disable-x86], [x86 bridgeboard support])])
AS_IF([test "x$enable_x86" != xno], [
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    ]], [[
        // FIXME: REMOVE THIS TEST NOW...
        #if 0
        #error Bridgeboard is only supported on x86/x86-64
        #endif
    ]])],[
        AC_DEFINE([WITH_X86], [1], [Define to 1 to enable x86 bridgeboard support])
    ],[
        if test "x$enable_x86" = xyes; then
            AC_MSG_FAILURE([x86 bridgeboard is not supported on $host])
        else
            enable_x86=no
        fi
    ])
])
AM_CONDITIONAL([WITH_X86],
[test x$enable_x86 != xno])

# 86Box x86 recompiler (dynarec) for the bridgeboards; only available for
# x86-64 and arm64 hosts.
x86_dynarec_x86_64=no
x86_dynarec_arm64=no
AS_IF([test x$enable_x86 != xno], [
    AS_CASE([$host_cpu],
    [x86_64|amd64], [x86_dynarec_x86_64=yes],
    [aarch64|arm64], [x86_dynarec_arm64=yes])
])
AM_CONDITIONAL([X86_DYNAREC_X86_64], [test x$x86_dynarec_x86_64 = xyes])
AM_CONDITIONAL([X86_DYNAREC_ARM64], [test x$x86_dynarec_arm64 = xyes])
AM_CONDITIONAL([X86_DYNAREC], [test x$x86_dynarec_x86_64 = xyes -o x$x86_dynarec_arm64 = xyes])

OPT_FEATURE([WITH_XML_SHADER], [xml_shader], [xml-shader], [XML shader support])
# OPT_FEATURE([A_ZIP], [zip], [zip], [ZIP archive support])

AC_DEFINE([FPU_UAE], [1], [Define to 1])

# Write Makefile

AC_CONFIG_FILES([
    Makefile
    od-fs/fsuae-build.h
    od-fs/build/macos/Info.plist
    od-fs/windows/fs-uae.rc
])
AC_OUTPUT

# Summary

echo ""
echo "Configuration summary for FS-UAE:"
echo ""
echo "  Version:           $FSUAE_VERSION"
echo "  Origin:            $FSUAE_BUILD_ORIGIN"
echo "  Commit:            $FSUAE_BUILD_COMMIT"
echo "  Build environment: $FSUAE_BUILD_ENVIRONMENT"
echo "  Building for OS:   $host_os (\"$OS_NAME\")"
echo "  Building for CPU:  $host_cpu"
echo "  Module extension:  $LOADABLE_MODULE_EXT"
echo ""
echo "  LIBMPEG2_CFLAGS:   $LIBMPEG2_CFLAGS"
echo "  LIBMPEG2_LIBS:     $LIBMPEG2_LIBS"
# echo "  OPENAL_CFLAGS:     $OPENAL_CFLAGS"
# echo "  OPENAL_LIBS:       $OPENAL_LIBS"
echo "  OPENGL_CFLAGS:     $OPENGL_CFLAGS"
echo "  OPENGL_LIBS:       $OPENGL_LIBS"
echo "  OS_CPPFLAGS:       $OS_CPPFLAGS"
echo "  OS_LDFLAGS:        $OS_LDFLAGS"
echo "  SDL3_CFLAGS:       $SDL3_CFLAGS"
echo "  SDL3_LIBS:         $SDL3_LIBS"
echo ""
echo "  Python interpreter: $PYTHON"
echo "  PYTHON3_CFLAGS:     $PYTHON3_CFLAGS"
echo "  PYTHON3_LIBS:       $PYTHON3_LIBS"
echo ""
