#ifndef UAE_OD_FS_SYSCONFIG_H
#define UAE_OD_FS_SYSCONFIG_H

// FIXME: this file was created by configure for PUAE and has been
// manually altered with reasonable values.
// New macros that are explicitly used in code are added to config.h for
// now, to separate them from this messy file. Have to eventually consider
// using autotools...

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define SUPPORT_THREADS

#define GFXFILTER

#define UAE_FILESYS_THREADS
#define NATMEM_OFFSET natmem_offset

#define ENFORCER /* UAE Enforcer */
#define ECS_DENISE /* ECS DENISE new features */
#define MMUEMU /* Aranym 68040 MMU */
#define FULLMMU /* Aranym 68040 MMU */

// Sound boards support
#define WITH_SNDBOARD

// Special Monitors support
#define WITH_SPECIALMONITORS

#define A_ZIP
// #define A_RAR
#define A_7Z
#define A_LHA
#define A_LZX
#define A_DMS
#define A_WRP

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <dustat.h> header file. */
/* #undef HAVE_DUSTAT_H */

/* Define to 1 if you have the CAPS framework. */
/* #undef HAVE_FRAMEWORK_CAPSIMAGE */

/* Define to 1 if you have the `gmtime_r' function. */
#define HAVE_GMTIME_R 1

/* Define to 1 if you have the `isinf' function. */
#define HAVE_ISINF 1

/* Define to 1 if you have the `isnan' function. */
#define HAVE_ISNAN 1

/* Define to 1 if you have the `nanosleep' function. */
#ifndef WINDOWS
#define HAVE_NANOSLEEP 1
#endif

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/filio.h> header file. */
/* #undef HAVE_SYS_FILIO_H */

/* Define to 1 if you have the <sys/filsys.h> header file. */
/* #undef HAVE_SYS_FILSYS_H */

/* Define to 1 if you have the <sys/fs/s5param.h> header file. */
/* #undef HAVE_SYS_FS_S5PARAM_H */

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/statvfs.h> header file. */
#if !defined(WINDOWS) && !defined(ANDROID)
#define HAVE_SYS_STATVFS_H 1
#endif

/* Define to 1 if you have the <sys/termios.h> header file. */
#define HAVE_SYS_TERMIOS_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `usleep' function. */
#define HAVE_USLEEP 1

/* Define to 1 if you have the <utime.h> header file. */
#define HAVE_UTIME_H 1

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

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

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

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* Enable GNU extensions on systems that have them.  */
// #ifndef _GNU_SOURCE
// # define _GNU_SOURCE 1
// #endif

#include "defines.h"

#endif /* UAE_OD_FS_SYSCONFIG_H */
