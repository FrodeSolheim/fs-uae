# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([FS-UAE], [2.9.8dev3],
        [frode@fs-uae.net], [fs-uae], [https://fs-uae.net])
AC_CONFIG_AUX_DIR([.])
AC_CANONICAL_HOST
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

# AC_CONFIG_MACRO_DIRS requires newer autoconf/automake than is found in
# Debian 7.0
# AC_CONFIG_MACRO_DIRS([m4])
m4_include([m4/ax_c___builtin_expect.m4])
m4_include([m4/ax_check_compile_flag.m4])
m4_include([m4/ax_check_link_flag.m4])
m4_include([m4/ax_gcc_var_attribute.m4])
m4_include([m4/ax_gcc_func_attribute.m4])

AM_INIT_AUTOMAKE([tar-ustar subdir-objects])

AC_SUBST([PACKAGE_SERIES], [devel])

AC_DEFINE_UNQUOTED([UAE_BASE_VERSION], ["WinUAE 3300b13"],
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

AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
# AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
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

DIS_FEATURE([STATIC_BUILD], [static], [static],
            [static build (works on Windows)])

AS_IF([test "x$enable_static" = xyes],
      [PKG_CONFIG="$PKG_CONFIG --static"])


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

AX_C___BUILTIN_EXPECT

# Checks for libraries.

AC_SUBST([CARBON_LIBS])
AS_CASE([$host_os],
[darwin*], [
	AC_MSG_NOTICE([Using Carbon.framework])
	CARBON_LIBS="-framework Carbon"
])

AC_ARG_WITH(cef, AS_HELP_STRING(
    [--with-cef], [use Chromium Embedded Framework (experimental)]))
AM_CONDITIONAL([BUILTIN_CEF], [test x$with_cef = xyes])
AS_IF([test "x$with_cef" = xyes], [
    AC_DEFINE([WITH_CEF], [1], [Define to 1 to use CEF])
])

AC_CHECK_LIB([compat], [ftime])

AC_CHECK_LIB([dl], [dlopen])

# not using freetype right now
# AC_DEFINE([USE_FREETYPE], [1], [Define to 1 to use FreeType])
# PKG_CHECK_MODULES([FREETYPE], [freetype2])
AC_SUBST([FREETYPE_CFLAGS])
AC_SUBST([FREETYPE_LIBS])

AC_CHECK_LIB([gdi32], [main])

AC_DEFINE([USE_GLIB], [1], [Define to 1 to use GLib])
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.32 gthread-2.0])

AC_CHECK_LIB([Iphlpapi], [main])

AC_ARG_WITH(glad, AS_HELP_STRING(
    [--without-glad], [use GLAD OpenGL loader]))
AS_IF([test "x$with_glad" != xno], [
    AC_DEFINE([WITH_GLAD], [1], [Define to 1 to use GLAD])
])
AM_CONDITIONAL([WITH_GLAD], [test x$with_glad != xno])

AC_ARG_WITH(glew, AS_HELP_STRING(
    [--with-glew], [use GLEW library]))
AS_IF([test "x$with_glew" = xyes], [
    PKG_CHECK_MODULES([GLEW], [glew])
    AC_DEFINE([WITH_GLEW], [1], [Define to 1 to use GLEW])
])

AC_ARG_WITH(libmpeg2, AS_HELP_STRING(
    [--without-libmpeg2], [or --with-libmpeg=builtin to use included version]))
AM_CONDITIONAL([BUILTIN_LIBMPEG2], [test x$with_libmpeg2 = xbuiltin])
AS_IF([test "x$with_libmpeg2" != xno], [
    AS_IF([test "x$with_libmpeg2" = xbuiltin], [
    ], [
        PKG_CHECK_MODULES([LIBMPEG2], [libmpeg2 libmpeg2convert])
    ])
    AC_DEFINE([WITH_LIBMPEG2], [1], [Define to 1 to use libmpeg2])
])

AM_CONDITIONAL([BUILTIN_MANYMOUSE], [true])

# The built-in Lua has been sandboxed, which is why we do not
# want to pick up the system lua, if present.

AC_SUBST([LUA_CFLAGS])
AS_CASE([$host_os],
	[mingw*], [],
	[LUA_CFLAGS="-DLUA_USE_POSIX"]
)

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

# AC_SUBST([USE_OPENAL], [1])
AC_DEFINE([WITH_OPENAL], [1], [Define to 1 to use OpenAL])
PKG_CHECK_MODULES([OPENAL], [openal], [
], [
        AS_CASE([$host_os],
	[darwin*], [
		AC_MSG_NOTICE([Using OpenAL.framework])
		OPENAL_LIBS="-framework OpenAL"
	],
	[
		AC_MSG_NOTICE([Assuming -lopenal works])
		OPENAL_LIBS="-lopenal"
	])
	AC_SUBST([OPENAL_CFLAGS])
	AC_SUBST([OPENAL_LIBS])

])

# AC_SUBST([USE_OPENGL], [1])
AC_DEFINE([USE_OPENGL], [1], [Define to 1 to use OpenGL])
PKG_CHECK_MODULES([OPENGL], [gl], [
], [
        AS_CASE([$host_os],
	[darwin*], [
		AC_MSG_NOTICE([Using OpenGL.framework])
		OPENGL_LIBS="-framework OpenGL"

	],
	[
		AC_CHECK_LIB([GL], [glViewport])
		AC_CHECK_LIB([OpenGL32], [main], [OPENGL_LIBS=-lOpenGL32])
	])
	AC_SUBST([OPENGL_CFLAGS])
	AC_SUBST([OPENGL_LIBS])
])

AC_DEFINE([USE_PNG], [1], [Define to 1 to use libpng])
PKG_CHECK_MODULES([PNG], [libpng])

AC_ARG_WITH(qt, AS_HELP_STRING(
    [--with-qt], [use QT]))
AS_IF([test "x$with_qt" = xyes], [
    AC_DEFINE([WITH_QT], [1], [Define to 1 to use QT])
    PKG_CHECK_MODULES([QT], [Qt5Gui])
])

AC_DEFINE([USE_SDL2], [1], [Define to 1 to use SDL 2.0])
PKG_CHECK_MODULES([SDL2], [sdl2])
AC_DEFINE([USE_SDL_VIDEO], [1], [Define to 1 to use SDL for video])
AC_DEFINE([USE_SDL_INPUT], [1], [Define to 1 to use SDL for input])

AS_CASE([$host_os],
[mingw*], [],
[darwin*], [],
[
    X11_LIBS="-lX11"
    AC_DEFINE([USE_X11], [1], [Define to 1 to use X11])

#    X11_LIBS="$X11_LIBS -lXtst"
#    AC_DEFINE([USE_X11_XTEST], [1], [Define to 1 to use X11 XTEST extension])
])
AC_SUBST([X11_LIBS])

AC_DEFINE([USE_ZLIB], [1], [Define to 1 to use zlib])
PKG_CHECK_MODULES([ZLIB], [zlib], [
], [
	AC_CHECK_LIB([z], [compress])
	AC_SUBST([ZLIB_CFLAGS])
	AC_SUBST([ZLIB_LIBS])
])

AC_CHECK_LIB([Winmm], [main])

AC_CHECK_LIB([Ws2_32], [main])


# Operating system

AC_SUBST([OS_CPPFLAGS])
AC_SUBST([OS_LDFLAGS])
AC_SUBST([OS_NAME])
AC_SUBST([OS_NAME_2])
AC_SUBST([OS_NAME_3])
AC_SUBST([PACKAGE_ARCH])
AS_CASE([$host_os],
[mingw*], [
    OS_NAME="windows"
    OS_NAME_2="Windows"
    OS_NAME_3="Windows"
    OS_CPPFLAGS="-DWINDOWS"
    AS_CASE([$host_cpu],
    [x86_64], [
        OS_LDFLAGS="-mwindows"
        PACKAGE_ARCH="x86-64"
    ], [
        OS_LDFLAGS="-mwindows -Wl,--large-address-aware"
        PACKAGE_ARCH="x86"
    ])
    LOADABLE_MODULE_EXT=".dll"
],
[linux*], [
    OS_NAME="linux"
    OS_NAME_2="Linux"
    OS_NAME_3="Linux"
    OS_CPPFLAGS="-DLINUX"
    LOADABLE_MODULE_EXT=".so"
],
[darwin*], [
    OS_NAME="macos"
    OS_NAME_2="macOS"
    OS_NAME_3="macOS"
    OS_CPPFLAGS="-DMACOSX -DMACOS"
    LOADABLE_MODULE_EXT=".so"
    OS_LDFLAGS="-headerpad_max_install_names -pagezero_size 0x2000 -rpath @executable_path/../Frameworks"
],
[freebsd*], [
    OS_NAME="freebsd"
    OS_NAME_2="FreeBSD"
    OS_NAME_3="FreeBSD"
    OS_CPPFLAGS="-DFREEBSD"
    LOADABLE_MODULE_EXT=".so"
],
[openbsd*], [
    OS_NAME="openbsd"
    OS_NAME_2="OpenBSD"
    OS_NAME_3="OpenBSD"
    OS_CPPFLAGS="-DOPENBSD"
    LOADABLE_MODULE_EXT=".so"
],
[dragonfly*], [
    OS_NAME="dragonflybsd"
    OS_NAME_2="DragonFlyBSD"
    OS_NAME_3="DragonFlyBSD"
    OS_CPPFLAGS="-DDRAGONFLYBSD"
    LOADABLE_MODULE_EXT=".so"
],
[
    OS_NAME="unknown"
    OS_NAME_2="Unknown OS"
    OS_NAME_3="Unknown"
    LOADABLE_MODULE_EXT=".unknown"
])

AM_CONDITIONAL([LINUX], [test x$OS_NAME = xlinux])
AM_CONDITIONAL([MACOS], [test x$OS_NAME = xmacos])
AM_CONDITIONAL([WINDOWS], [test x$OS_NAME = xwindows])

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
AC_CHECK_HEADERS([OpenAL/al.h])
AC_CHECK_HEADERS([OpenAL/alc.h])
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
AC_TRY_LINK([
	#if HAVE_BYTESWAP_H
	# include <byteswap.h>
	#endif
], [
	bswap_16(0x12);
], [
	AC_DEFINE(HAVE_BSWAP_16, 1, [Define to 1 if you have the 'bswap_16' function.])
	AC_MSG_RESULT(yes)
], [
	AC_MSG_RESULT(no)
])
AC_MSG_CHECKING(for bswap_32)
AC_TRY_LINK([
	#if HAVE_BYTESWAP_H
	# include <byteswap.h>
	#endif
], [
	bswap_32(0x1234);
], [
	AC_DEFINE(HAVE_BSWAP_32, 1, [Define to 1 if you have the 'bswap_32' function.])
	AC_MSG_RESULT(yes)
], [
	AC_MSG_RESULT(no)
])


# More feature macros. Many of these are generally always enabled, and
# compiling without them may not work (without further changes to
# source code).

OPT_FEATURE([A2065], [a2065], [a2065],
            [A2065 ethernet card])
REQ_FEATURE([A2091], [a2091], [a2091],
            [A590/A2091 SCSI])
OPT_FEATURE([ACTION_REPLAY], [action_replay], [action-replay],
            [Action Replay 1/2/3 support])
OPT_FEATURE([AGA], [aga], [aga],
            [AGA chipset emulation])
DIS_FEATURE([AHI], [ahi], [ahi],
            [builtin AHI device (unsupported)])
OPT_FEATURE([ARCADIA], [arcadia], [arcadia],
            [Arcadia arcade system])
REQ_FEATURE([AUTOCONFIG], [autoconfig], [autoconfig],
            [autoconfig support, fast ram, harddrives etc...])
DIS_FEATURE([BSDSOCKET], [bsdsocket], [bsdsocket],
            [built-in bsdsocket.library])
DIS_FEATURE([WITH_BUILTIN_SLIRP], [builtin_slirp], [builtin-slirp],
            [built-in SLIRP implementation (unsupported)])
OPT_FEATURE([CAPS], [caps], [caps],
            [IPF support via libcapsimage])
OPT_FEATURE([CD32], [cd32], [cd32],
            [CD32 emulation])
OPT_FEATURE([CDTV], [cdtv], [cdtv],
            [CDTV emulation])
DIS_FEATURE([WITH_CLIPBOARD], [clipboard], [clipboard],
            [clipboard integration])
OPT_FEATURE([CODEGEN], [codegen], [codegen],
            [code generation (disable for cross-compilation)])
OPT_FEATURE([CPUEMU_0], [cpuemu_0], [cpuemu-0],
            [generic 680x0 emulation])
OPT_FEATURE([CPUEMU_11], [cpuemu_11], [cpuemu-11],
            [68000/68010 prefetch emulation])
REQ_FEATURE([CPUEMU_13], [cpuemu_13], [cpuemu-13],
            [68000/68010 cycle-exact cpu&blitter])
OPT_FEATURE([CPUEMU_20], [cpuemu_20], [cpuemu-20],
            [68020 prefetch])
OPT_FEATURE([CPUEMU_21], [cpuemu_21], [cpuemu-21],
            [68020 "cycle-exact" + blitter])
OPT_FEATURE([CPUEMU_22], [cpuemu_22], [cpuemu-22],
            [68030 prefetch])
OPT_FEATURE([CPUEMU_23], [cpuemu_23], [cpuemu-23],
            [68030 "cycle-exact" + blitter])
OPT_FEATURE([CPUEMU_24], [cpuemu_24], [cpuemu-24],
            [68060 "cycle-exact" + blitter])
OPT_FEATURE([CPUEMU_25], [cpuemu_25], [cpuemu-25],
            [68040 "cycle-exact" + blitter (needs cpuemu-24)])
OPT_FEATURE([CPUEMU_31], [cpuemu_31], [cpuemu-31],
            [Aranym 68040 MMU])
OPT_FEATURE([CPUEMU_32], [cpuemu_32], [cpuemu-32],
            [Previous 68030 MMU])
OPT_FEATURE([CPUEMU_33], [cpuemu_33], [cpuemu-33],
            [68060 MMU])
OPT_FEATURE([CPUEMU_40], [cpuemu_40], [cpuemu-40],
            [generic 680x0 with JIT direct memory access])
OPT_FEATURE([CPUEMU_50], [cpuemu_50], [cpuemu-50],
            [generic 680x0 with indirect memory access])
REQ_FEATURE([WITH_CPUBOARD], [cpuboard], [cpuboard],
            [cpu board emulation])
OPT_FEATURE([DEBUGGER], [debugger], [debugger],
            [debugger support (cannot be disabled right now)])
OPT_FEATURE([A_DMS], [dms], [dms],
            [DMS support])
OPT_FEATURE([DRIVESOUND], [drivesound], [drivesound],
            [drive sound emulation])
OPT_FEATURE([FDI2RAW], [fdi2raw], [fdi2raw],
            [FDI 1.0 and 2.x image support])
REQ_FEATURE([FILESYS], [filesys], [filesys],
            [directory hard drives])
REQ_FEATURE([FPUEMU], [fpuemu], [fpuemu],
            [FPU emulation])
DIS_FEATURE([FSE_DRIVERS], [drivers], [drivers],
            [multiple driver backends (experimental)])
OPT_FEATURE([GFXBOARD], [gfxboard], [gfxboard],
            [GFX hardware boards])
AC_ARG_ENABLE([jit], [AS_HELP_STRING([--disable-jit], [JIT compiler])])
AS_IF([test "x$enable_jit" != xno], [
    AC_TRY_COMPILE([
    ], [
        #if !defined(__i386__) && !defined(__x86_64__)
        #error JIT is only supported on x86/x86-64
        #endif
        #if defined(__OpenBSD__) || defined(__FreeBSD__)
        #error no JIT on OpenBSD/FreeBSD right now
        #endif
    ], [
        AC_DEFINE([JIT], [1], [Define to 1 to enable JIT compilation])
    ], [
        if test "x$enable_jit" != xyes; then
            AC_MSG_FAILURE(
                [JIT is not supported on $host])
        fi
    ])
])
AM_CONDITIONAL([JIT],
[test x$enable_jit != xno])

OPT_FEATURE([USE_JIT_FPU], [jit_fpu], [jit-fpu],
            [JIT compilation of FPU instructions])
DIS_FEATURE([WITH_LUA], [lua], [lua],
            [LUA scripting (unfinished)])
OPT_FEATURE([WITH_NETPLAY], [netplay], [netplay],
            [netplay support])
OPT_FEATURE([NCR], [ncr], [ncr],
            [A4000T/A4091, 53C710/53C770 SCSI])
OPT_FEATURE([NCR9X], [ncr9x], [ncr9x],
            [53C9X SCSI])
OPT_FEATURE([PARALLEL_PORT], [parallel_port], [parallel-port],
            [parallell port (printer) emulation])
DIS_FEATURE([WITH_PEARPC_CPU], [pearpc_cpu], [pearpc-cpu],
            [use PearPC for PPC emulation (deprecated)])
REQ_FEATURE([PICASSO96], [picasso96], [picasso96],
            [Picasso 96 (uaegfx.card)])
OPT_FEATURE([WITH_PPC], [ppc], [ppc],
            [PPC (requires cpu board)])
OPT_FEATURE([PROWIZARD], [prowizard], [prowizard],
            [prowizard])
OPT_FEATURE([SAVESTATE], [savestate], [savestate],
            [save state support])
OPT_FEATURE([SCP], [scp], [scp],
            [SuperCardPro floppy image support])
OPT_FEATURE([WITH_SEGTRACKER], [segtracker], [segtracker],
            [segtracker support in debugger])
OPT_FEATURE([SERIAL_PORT], [serial_port], [serial-port],
            [serial port emulation])
OPT_FEATURE([WITH_SLIRP], [slirp], [slirp],
            [SLIRP (user mode NAT)])
OPT_FEATURE([WITH_SOFTFLOAT], [softfloat], [softfloat],
            [use softfloat library])
OPT_FEATURE([WITH_QEMU_CPU], [qemu_cpu], [qemu-cpu],
            [use QEMU plugin for PPC emulation])
OPT_FEATURE([WITH_QEMU_SLIRP], [qemu_slirp], [qemu-slirp],
            [use QEMU plugin for SLIRP implementation])
REQ_FEATURE([WITH_TOCCATA], [toccata], [toccata],
            [Toccata sound card emulation])
OPT_FEATURE([WITH_UAENATIVE], [uaenative], [uaenative],
            [uaenative.library])
OPT_FEATURE([SANA2], [uaenet], [uaenet],
            [uaenet.device])
OPT_FEATURE([SCSIEMU], [uaescsi], [uaescsi],
            [uaescsi.device])
OPT_FEATURE([UAESERIAL], [uaeserial], [uaeserial],
            [uaeserial.device])
OPT_FEATURE([WITH_UDIS86], [udis86], [udis86],
            [UDIS86 x86/x86-64 disassembler])
OPT_FEATURE([WITH_VPAR], [vpar], [vpar],
            [vpar virtual parallel port])
OPT_FEATURE([WITH_XML_SHADER], [xml_shader], [xml-shader],
            [XML shader support])
OPT_FEATURE([A_ZIP], [zip], [zip],
            [ZIP archive support])

AC_DEFINE([FPU_UAE], [1], [Define to 1])

AC_CONFIG_SUBDIRS([libmpeg2])

# Write Makefile

AC_CONFIG_FILES([
    dist/linux/Makefile
    dist/macos/Info.plist
    dist/macos/Makefile
    dist/steamos/Makefile
    dist/windows/fs-uae.iss
    dist/windows/Makefile
    src/fs-uae/fs-uae.rc
    Makefile
])
AC_OUTPUT

# Summary

echo ""
echo "Configuration summary for FS-UAE:"
echo ""
echo "  Building for OS:   $host_os (\"$OS_NAME\")"
echo "  Building for CPU:  $host_cpu"
echo "  Module extension:  $LOADABLE_MODULE_EXT"
echo ""
echo "  LIBMPEG2_CFLAGS:   $LIBMPEG2_CFLAGS"
echo "  LIBMPEG2_LIBS:     $LIBMPEG2_LIBS"
echo "  OPENAL_CFLAGS:     $OPENAL_CFLAGS"
echo "  OPENAL_LIBS:       $OPENAL_LIBS"
echo "  OPENGL_CFLAGS:     $OPENGL_CFLAGS"
echo "  OPENGL_LIBS:       $OPENGL_LIBS"
echo "  OS_CPPFLAGS:       $OS_CPPFLAGS"
echo "  OS_LDFLAGS:        $OS_LDFLAGS"
echo "  SDL2_CFLAGS:       $SDL2_CFLAGS"
echo "  SDL2_LIBS:         $SDL2_LIBS"
echo ""
