# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([FS-UAE], [2.5.7dev],
        [frode@fs-uae.net], [fs-uae], [http://fs-uae.net])
AC_CONFIG_AUX_DIR([.])
AC_CANONICAL_SYSTEM
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
# AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([tar-ustar subdir-objects])

AC_SUBST([PACKAGE_SERIES], [devel])

AC_DEFINE_UNQUOTED([UAE_BASE_VERSION], ["WinUAE 2.8.2 beta 8"],
[Which UAE core this version is based on.])

PACKAGE_MAJOR=`echo ${PACKAGE_VERSION} | sed -e 's,\(\.*\)\..*\..*,\1,g'`
PACKAGE_MINOR=`echo ${PACKAGE_VERSION} | sed -e 's,.*\.\(.*\)\..*,\1,g'`
PACKAGE_REVISION=`echo ${PACKAGE_VERSION} | sed -e 's,.*\..*\.\([[0-9]]*\).*,\1,g'`
AC_SUBST(PACKAGE_MAJOR)
AC_SUBST(PACKAGE_MINOR)
AC_SUBST(PACKAGE_REVISION)

# Checks for programs.

AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
# AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AM_PROG_CC_C_O
# AC_PATH_TOOL(OBJCOPY, objcopy)
AC_PATH_PROG([MSGFMT], [msgfmt])
AC_PATH_PROG([ZIP], [zip])

# Misc system checks.

AC_SYS_LARGEFILE
# AC_LTDL_SHLIBEXT

# Checks for libraries.

AC_SUBST([CARBON_LIBS])
AS_CASE([$target_os],
[darwin*], [
	AC_MSG_NOTICE([Using Carbon.framework])
	CARBON_LIBS="-framework Carbon"
])

AC_CHECK_LIB([compat], [ftime])

AC_CHECK_LIB([dl], [dlopen])

AC_DEFINE([USE_FREETYPE], [1], [Define to 1 to use FreeType])
PKG_CHECK_MODULES([FREETYPE], [freetype2])

AC_CHECK_LIB([gdi32], [main])

AC_DEFINE([USE_GLIB], [1], [Define to 1 to use GLib])
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.32 gthread-2.0])

AC_CHECK_LIB([Iphlpapi], [main])

PKG_CHECK_MODULES([LIBMPEG2], [libmpeg2 libmpeg2convert])

# The built-in Lua has been sandboxed
# AC_DEFINE([USE_LUA], [1], [Define to 1 to use LUA])
AC_SUBST([LUA_CFLAGS])
AS_CASE([$target_os],
	[mingw*], [],
	[LUA_CFLAGS="-DLUA_USE_POSIX"]
)

AC_SUBST([IOKIT_LIBS])
AS_CASE([$target_os],
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
AC_DEFINE([USE_OPENAL], [1], [Define to 1 to use OpenAL])
PKG_CHECK_MODULES([OPENAL], [openal], [
], [
	AS_CASE([$target_os],
	[darwin*], [
		AC_MSG_NOTICE([Using OpenAL.framework])
		#OPENAL_CFLAGS="-I/System/Library/Frameworks/OpenAL.framework/Headers"
		OPENAL_CFLAGS="-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/OpenAL.framework/Headers"
		OPENAL_LIBS="-framework OpenAL"

	],
	[
		AC_MSG_NOTICE([Assuming -lopenal works])
		OPENAL_LIBS = -lopenal
	])
	AC_SUBST([OPENAL_CFLAGS])
	AC_SUBST([OPENAL_LIBS])

])

# AC_SUBST([USE_OPENGL], [1])
AC_DEFINE([USE_OPENGL], [1], [Define to 1 to use OpenGL])
PKG_CHECK_MODULES([OPENGL], [gl glu], [
], [
	AS_CASE([$target_os],
	[darwin*], [
		AC_MSG_NOTICE([Using OpenGL.framework])
		OPENGL_LIBS="-framework OpenGL"

	],
	[
		AC_CHECK_LIB([GL], [glViewport])
		AC_CHECK_LIB([GLU], [gluPerspective])
		AC_CHECK_LIB([GLU32], [main])
		AC_CHECK_LIB([OpenGL32], [main])
	])
	AC_SUBST([OPENGL_CFLAGS])
	AC_SUBST([OPENGL_LIBS])
])

AC_DEFINE([USE_PNG], [1], [Define to 1 to use libpng])
PKG_CHECK_MODULES([PNG], [libpng])

AC_DEFINE([USE_SDL2], [1], [Define to 1 to use SDL 2.0])
PKG_CHECK_MODULES([SDL2], [sdl2])
AC_DEFINE([USE_SDL_VIDEO], [1], [Define to 1 to use SDL for video])
AC_DEFINE([USE_SDL_INPUT], [1], [Define to 1 to use SDL for input])

AS_CASE([$target_os],
[mingw*], [],
[darwin*], [],
[
	X11_LIBS="-lX11"
	AC_DEFINE([USE_X11], [1], [Define to 1 to use X11])
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
AS_CASE([$target_os],
[mingw*], [
	OS_NAME="windows"
	OS_CPPFLAGS="-DWINDOWS"
	LOADABLE_MODULE_EXT=".dll"
],
[linux*], [
	OS_NAME="linux"
	OS_CPPFLAGS="-DLINUX"
	LOADABLE_MODULE_EXT=".so"
],
[darwin*], [
	OS_NAME="macosx"
	OS_CPPFLAGS="-DMACOSX"
	LOADABLE_MODULE_EXT=".so"
	OS_LDFLAGS="-headerpad_max_install_names -pagezero_size 0x2000"
],
[freebsd*], [
	OS_NAME="freebsd"
	OS_CPPFLAGS="-DFREEBSD"
	LOADABLE_MODULE_EXT=".so"
],
[openbsd*], [
	OS_NAME="openbsd"
	OS_CPPFLAGS="-DOPENBSD"
	LOADABLE_MODULE_EXT=".so"
],
[
	OS_NAME="unknown"
	LOADABLE_MODULE_EXT=".unknown"
])

# Not using libtool right now, but use name LT_MODULE_EXT anyway in case
# transitioning to libtool later.
AC_DEFINE_UNQUOTED([LT_MODULE_EXT], ["$LOADABLE_MODULE_EXT"],
[Define to the extension used for runtime loadable modules, say, ".so".])

AC_DEFINE_UNQUOTED([OS_NAME], ["$OS_NAME"],
[Define to a name identifier for the operating system, "linux".])


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
AC_FUNC_MALLOC
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

AS_CASE([$target_os],
	[mingw*], [],
	[
		AC_DEFINE([POSIX_SERIAL], [1],
			[Define to 1 if you have POSIX serial support])
	]
)
#AC_CHECK_FUNC(ioctl, [
#	AC_DEFINE(POSIX_SERIAL)
#])


# JIT support

AC_ARG_ENABLE([jit], [AS_HELP_STRING([--enable-jit],
        [enable support for JIT compilation @<:@default=check@:>@])
    ], [], [enable_jit=check])

AS_IF([test "x$enable_jit" != xno], [
    AC_TRY_COMPILE([
    ], [
        #ifndef __i386__
        #error JIT is only supported on x86
        #endif
        #if defined(__OpenBSD__) || defined(__FreeBSD__)
        #error no JIT on OpenBSD/FreeBSD right now
        #endif
    ], [
        AC_DEFINE([JIT], [1], [Define to 1 to enable JIT compilation])
    ], [
        if test "x$enable_jit" != xcheck; then
            AC_MSG_FAILURE(
                [--enable-jit was given, but arch is not supported])
        fi
    ]) 
], [])


# Udis86

#AC_ARG_ENABLE(
#    [udis86],
#    [AS_HELP_STRING(
#            [--enable-udis86],
#            [enable support for JIT compilation @<:@default=no@:>@]
#    )],
#    [],
#    [enable_udis86=no]
#)
#AS_IF(
#    [test "x$enable_udis86" != xno],
#    [
#        AC_CHECK_LIB([udis86], [ud_init])
#    ],
#    []
#)

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

AC_DEFUN([OPT_FEATURE], [

# AS_IF([test "x$5" = "xyes"], [
#	AC_ARG_ENABLE([$3],
#	[AS_HELP_STRING([--disable-$3], [$4])],
#	[enable_$2=no],
#	[enable_$2=yes])
# ], [
#	AC_ARG_ENABLE([$3],
#	[AS_HELP_STRING([--enable-$3], [$4])],
#	[enable_$2=yes],
#	[enable_$2=no])
# ])
AC_ARG_ENABLE([$3],
	[AS_HELP_STRING([--enable-$3], [$4 @<:@default=$5@:>@])],
	[], [enable_$2=$5])
AS_IF([test x$enable_$2 = xyes], [
	AC_DEFINE([$1], [1], [Define to 1 to $4])
])
AM_CONDITIONAL([$1], [test x$enable_$2 = xyes])
])

OPT_FEATURE([A2065], [a2065], [a2065],
	[enable A2065 ethernet card], [yes])
OPT_FEATURE([A2091], [a2091], [a2091],
	[enable A590/A2091 SCSI], [yes])
OPT_FEATURE([ACTION_REPLAY], [action_replay], [action-replay],
	[enable Action Replay 1/2/3 support], [yes])
OPT_FEATURE([CAPS], [caps], [caps],
	[enable IPF support via libcapsimage], [yes])
OPT_FEATURE([CD32], [cd32], [cd32],
	[enable CD32 emulation], [yes])
OPT_FEATURE([CDTV], [cdtv], [cdtv],
	[enable CDTV emulation], [yes])
OPT_FEATURE([DRIVESOUND], [drivesound], [drivesound],
	[enable drive sound emulation], [yes])
OPT_FEATURE([FPUEMU], [fpuemu], [fpuemu],
	[enable FPU emulation], [yes])
OPT_FEATURE([WITH_LUA], [lua], [lua],
	[enable LUA scripting], [no])
OPT_FEATURE([WITH_NETPLAY], [netplay], [netplay],
	[enable netplay support], [yes])
OPT_FEATURE([NCR9X], [ncr9x], [ncr9x],
	[enable 53C9X SCSI], [yes])
OPT_FEATURE([WITH_SLIRP], [slirp], [slirp],
	[enable SLIRP (user mode NAT)], [yes])
OPT_FEATURE([WITH_UAENATIVE], [uaenative], [uaenative],
	[enable uaenative.library], [yes])
OPT_FEATURE([SANA2], [uaenet], [uaenet],
	[enable uaenet.device], [yes])
OPT_FEATURE([WITH_UDIS86], [udis86], [udis86],
	[enable UDIS86 x86/x86-64 disassembler], [yes])
OPT_FEATURE([WITH_XML_SHADER], [xml_shader], [xml-shader],
	[enable XML shader support], [yes])
OPT_FEATURE([A_ZIP], [zip], [zip],
	[enable ZIP archive support], [yes])


AC_DEFINE([FPU_UAE], [1], [Define to 1])


# Write Makefile

AC_CONFIG_FILES([
    build/linux-dist/Makefile
    build/steamos-dist/Makefile
    macosx/Info.plist
    macosx/Makefile
    Makefile
    windows/fs-uae.iss
    windows/Makefile
])
AC_OUTPUT
