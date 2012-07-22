/* src/sysconfig.h.  Generated from sysconfig.h.in by configure.  */
/* src/sysconfig.h.in.  Generated from configure.in by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* CPU is 64bit */
/* #undef CPU_64_BIT */

/* we want ecs_denise */
/* #undef ECS_DENISE */

/* CPU supports 3DNOW */
/* #undef HAVE_3DNOW */

/* Define to 1 if you have the `alarm' function. */
#define HAVE_ALARM 1

/* Define to 1 if you have the 'bswap_16' function. */
//#define HAVE_BSWAP_16 1

/* Define to 1 if you have the 'bswap_32' function. */
//#define HAVE_BSWAP_32 1

/* Define to 1 if you have the <byteswap.h> header file. */
//#define HAVE_BYTESWAP_H 1

/* Define to 1 if you have the <caps/capsimage.h> header file. */
#define HAVE_CAPS_CAPSIMAGE_H 1

/* Define to 1 if you have the <curses.h> header file. */
#define HAVE_CURSES_H 1

/* Define to 1 if you have the <cybergraphx/cybergraphics.h> header file. */
/* #undef HAVE_CYBERGRAPHX_CYBERGRAPHICS_H */

/* Define to 1 if you have the <devices/ahi.h> header file. */
/* #undef HAVE_DEVICES_AHI_H */

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#define HAVE_DIRENT_H 1

/* "Define to 1 if you have 'dlopen' function */
#define HAVE_DLOPEN 1

/* Define to 1 if you have the <dustat.h> header file. */
/* #undef HAVE_DUSTAT_H */

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the CAPS framework. */
/* #undef HAVE_FRAMEWORK_CAPSIMAGE */

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `gmtime_r' function. */
#define HAVE_GMTIME_R 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if <inttypes.h> exists, doesn't clash with <sys/types.h>, and
   declares uintmax_t. */
#define HAVE_INTTYPES_H_WITH_UINTMAX 1

/* Define to 1 if you have the `isinf' function. */
#define HAVE_ISINF 1

/* Define to 1 if you have the `isnan' function. */
#define HAVE_ISNAN 1

/* Define to 1 if you have the <libraries/cybergraphics.h> header file. */
/* #undef HAVE_LIBRARIES_CYBERGRAPHICS_H */

/* Define to 1 if you have the `localtime_r' function. */
#define HAVE_LOCALTIME_R 1

/* Define to 1 if you have the <machine/joystick.h> header file. */
/* #undef HAVE_MACHINE_JOYSTICK_H */

/* Define to 1 if you have the <machine/soundcard.h> header file. */
/* #undef HAVE_MACHINE_SOUNDCARD_H */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* CPU supports MMX */
/* #undef HAVE_MMX */

/* Define to 1 if you have the `nanosleep' function. */
#ifndef WINDOWS
#define HAVE_NANOSLEEP 1
#endif

/* Define to 1 if you have the <ncurses.h> header file. */
#define HAVE_NCURSES_H 1

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define if you have POSIX threads libraries and header files. */
/* #undef HAVE_PTHREAD */

/* Define to 1 if you have the `readdir_r' function. */
#define HAVE_READDIR_R 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `setitimer' function. */
#define HAVE_SETITIMER 1

/* Define to 1 if you have the `sigaction' function. */
#define HAVE_SIGACTION 1

/* Define to 1 if you have the `sleep' function. */
#define HAVE_SLEEP 1

/* CPU supports SSE */
/* #undef HAVE_SSE */

/* CPU supports SSE2 */
/* #undef HAVE_SSE2 */

/* CPU supports SSE4_1 */
/* #undef HAVE_SSE4_1 */

/* CPU supports SSSE3 */
/* #undef HAVE_SSSE3 */

/* Define to 1 if stdbool.h conforms to C99. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define if <stdint.h> exists, doesn't clash with <sys/types.h>, and declares
   uintmax_t. */
#define HAVE_STDINT_H_WITH_UINTMAX 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strcmpi' function. */
/* #undef HAVE_STRCMPI */

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the `stricmp' function. */
/* #undef HAVE_STRICMP */

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if `st_blocks' is a member of `struct stat'. */
#ifndef WINDOWS
#define HAVE_STRUCT_STAT_ST_BLOCKS 1
#endif

/* Define to 1 if your `struct stat' has `st_blocks'. Deprecated, use
   `HAVE_STRUCT_STAT_ST_BLOCKS' instead. */
#ifndef WINDOWS
#define HAVE_ST_BLOCKS 1
#endif

/* Define to 1 if you have the <sun/audioio.h> header file. */
/* #undef HAVE_SUN_AUDIOIO_H */

/* Define to 1 if you have the `sync' function. */
#define HAVE_SYNC 1

/* Define to 1 if you have the <sys/audioio.h> header file. */
/* #undef HAVE_SYS_AUDIOIO_H */

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/filio.h> header file. */
/* #undef HAVE_SYS_FILIO_H */

/* Define to 1 if you have the <sys/filsys.h> header file. */
/* #undef HAVE_SYS_FILSYS_H */

/* Define to 1 if you have the <sys/fs/s5param.h> header file. */
/* #undef HAVE_SYS_FS_S5PARAM_H */

/* Define to 1 if you have the <sys/fs_types.h> header file. */
/* #undef HAVE_SYS_FS_TYPES_H */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/ipc.h> header file. */
#define HAVE_SYS_IPC_H 1

/* Define to 1 if you have the <sys/mman.h> header file. */
#define HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/mount.h> header file. */
#ifndef WINDOWS
#define HAVE_SYS_MOUNT_H 1
#endif

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/shm.h> header file. */
#define HAVE_SYS_SHM_H 1

/* Define to 1 if you have the <sys/soundcard.h> header file. */
#define HAVE_SYS_SOUNDCARD_H 1

/* Define to 1 if you have the <sys/statfs.h> header file. */
#ifndef WINDOWS
#ifndef MACOSX
#define HAVE_SYS_STATFS_H 1
#endif
#endif

/* Define to 1 if you have the <sys/statvfs.h> header file. */
#if !defined(WINDOWS) && !defined(ANDROID)
#define HAVE_SYS_STATVFS_H 1
#endif

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/termios.h> header file. */
#define HAVE_SYS_TERMIOS_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/vfs.h> header file. */
#ifndef WINDOWS
#ifndef MACOSX
#define HAVE_SYS_VFS_H 1
#endif
#endif

/* Define to 1 if you have the `timegm' function. */
#define HAVE_TIMEGM 1

/* Define if you have the 'uintmax_t' type in <stdint.h> or <inttypes.h>. */
#define HAVE_UINTMAX_T 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define if you have the 'unsigned long long' type. */
#define HAVE_UNSIGNED_LONG_LONG 1

/* Define to 1 if you have the `usleep' function. */
#define HAVE_USLEEP 1

/* Define to 1 if you have the <utime.h> header file. */
#define HAVE_UTIME_H 1

/* Define to 1 if `utime(file, NULL)' sets file's timestamp to the present. */
#define HAVE_UTIME_NULL 1

/* Define to 1 if you have the <values.h> header file. */
//#ifdef _MAC
//#else
//#define HAVE_VALUES_H 1
//#endif

/* Define to 1 if you have the `vfprintf' function. */
#define HAVE_VFPRINTF 1

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the `vsprintf' function. */
#define HAVE_VSPRINTF 1

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Define to 1 if your CPU profitably supports multiplication. */
#define MULTIPLICATION_PROFITABLE 1

/* Name of package */
#define PACKAGE "puae"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "PUAE"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "PUAE 2.3.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "puae"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.3.1"

/* Define to the necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* The size of `char', as computed by sizeof. */
#define SIZEOF_CHAR 1

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* The size of `void *', as computed by sizeof. */

#if __LP64__
#define SIZEOF_VOID_P 8
#else
#define SIZEOF_VOID_P 4
#endif

/* The size of `__int64', as computed by sizeof. */
#define SIZEOF___INT64 0

/* Define if the block counts reported by statfs may be truncated to 2GB and
   the correct values may be stored in the f_spare array. (SunOS 4.1.2, 4.1.3,
   and 4.1.3_U1 are reported to have this problem. SunOS 4.1.1 seems not to be
   affected.) */
/* #undef STATFS_TRUNCATES_BLOCK_COUNTS */

/* Define if there is no specific function for reading filesystems usage
   information and you have the <sys/filsys.h> header file. (SVR2) */
/* #undef STAT_READ_FILSYS */

/* Define if statfs takes 2 args and struct statfs has a field named f_bsize.
   (4.3BSD, SunOS 4, HP-UX, AIX PS/2) */
#define STAT_STATFS2_BSIZE 1

/* Define if statfs takes 2 args and struct statfs has a field named f_fsize.
   (4.4BSD, NetBSD) */
/* #undef STAT_STATFS2_FSIZE */

/* Define if statfs takes 2 args and the second argument has type struct
   fs_data. (Ultrix) */
/* #undef STAT_STATFS2_FS_DATA */

/* Define if statfs takes 3 args. (DEC Alpha running OSF/1) */
/* #undef STAT_STATFS3_OSF1 */

/* Define if statfs takes 4 args. (SVR3, Dynix, Irix, Dolphin) */
/* #undef STAT_STATFS4 */

/* Define if there is a function named statvfs. (SVR4) */
/* #undef STAT_STATVFS */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Version number of package */
#define VERSION "2.3.1"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Number of bits in a file offset, on hosts where this is settable. */
#define _FILE_OFFSET_BITS 64

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to `long int' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Substitute for socklen_t */
/* #undef socklen_t */

/* Define to unsigned long or unsigned long long if <stdint.h> and
   <inttypes.h> don't define. */
/* #undef uintmax_t */

/* Define to empty if the keyword `volatile' does not work. Warning: valid
   code using `volatile' can become incorrect without. Disable with care. */
/* #undef volatile */

#include "defines.h"
#include "libamiga_internal.h"
