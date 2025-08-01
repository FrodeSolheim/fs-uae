/*
  * UAE - The Un*x Amiga Emulator
  *
  * Try to include the right system headers and get other system-specific
  * stuff right & other collected kludges.
  *
  * If you think about modifying this, think twice. Some systems rely on
  * the exact order of the #include statements. That's also the reason
  * why everything gets included unconditionally regardless of whether
  * it's actually needed by the .c file.
  *
  * Copyright 1996, 1997 Bernd Schmidt
  */
#ifndef UAE_SYSDEPS_H
#define UAE_SYSDEPS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "sysconfig.h"

#ifndef UAE
#define UAE
#endif

#ifdef __cplusplus
#include <string>
using namespace std;
#else
#include <string.h>
#include <ctype.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>

#ifndef UAE
#define UAE
#endif

#if defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__)
#define CPU_arm 1
#define ARM_ASSEMBLY 1
#define CPU_64_BIT 1
#elif defined(__arm__) || defined(_M_ARM)
#define CPU_arm 1
#define ARM_ASSEMBLY 1
#elif defined(__x86_64__) || defined(_M_AMD64)
#define CPU_x86_64 1
#define CPU_64_BIT 1
#define X86_64_ASSEMBLY 1
#ifdef FSUAE
// FIXME: Cleanup this (?)
#define SAHF_SETO_PROFITABLE
#endif
#elif defined(__i386__) || defined(_M_IX86)
#define CPU_i386 1
#define X86_ASSEMBLY 1
#define SAHF_SETO_PROFITABLE
#elif defined(__powerpc__) || defined(_M_PPC)
#define CPU_powerpc 1
#else
#error unrecognized CPU type
#endif

#ifdef _WIN32
/* Parameters are passed in ECX, EDX for both x86 and x86-64 (RCX, RDX).
 * For x86-64, __fastcall is the default, so it isn't really required. */
#define JITCALL __fastcall
#elif defined(CPU_x86_64)
/* Parameters are passed in RDI, RSI by default (System V AMD64 ABI). */
#define JITCALL
#elif defined(HAVE_FUNC_ATTRIBUTE_REGPARM)
/* Parameters are passed in EAX, EDX on x86 with regparm(2). */
#define JITCALL __attribute__((regparm(2)))
/* This was originally regparm(3), but as far as I can see only two register
 * params are supported by the JIT code. It probably just worked anyway
 * if all functions used max two arguments. */
#elif !defined(JIT)
#define JITCALL
#endif
#define REGPARAM
#define REGPARAM2 JITCALL
#define REGPARAM3 JITCALL

#include "uae/tchar.h"

#if CPU_64_BIT
#define addrdiff(a, b) ((int)((a) - (b)))
#else
#define addrdiff(a, b) ((a) - (b))
#endif

#ifndef __STDC__
#ifndef _MSC_VER
#error "Your compiler is not ANSI. Get a real one."
#endif
#endif

#include <stdarg.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_UTIME_H
#include <utime.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#if HAVE_DIRENT_H
# include <dirent.h>
#else
# define dirent direct
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#ifdef HAVE_SYS_UTIME_H
# include <sys/utime.h>
#endif

#include <errno.h>
#include <assert.h>

#ifdef __NeXT__
#define S_IRUSR S_IREAD
#define S_IWUSR S_IWRITE
#define S_IXUSR S_IEXEC
#define S_ISDIR(val) (S_IFDIR & val)
struct utimbuf
{
    time_t actime;
    time_t modtime;
};
#endif

#ifdef FSUAE

#include "uae/types.h"

#define VAL64(a) (a ## ll)
#define UVAL64(a) (a ## ull)

#else

#include <stdint.h>

typedef int8_t uae_s8;
typedef uint8_t uae_u8;

typedef int16_t uae_s16;
typedef uint16_t uae_u16;

typedef int32_t uae_s32;
typedef uint32_t uae_u32;

typedef int64_t uae_s64;
typedef uint64_t uae_u64;

typedef char uae_char;

typedef struct { uae_u8 RGB[3]; } RGB;

typedef uae_u32 uaecptr;

#if SIZEOF_LONG == 8
#define VAL64(a) (a ## l)
#define UVAL64(a) (a ## ul)
#elif SIZEOF_LONG_LONG == 8
#define VAL64(a) (a ## LL)
#define UVAL64(a) (a ## uLL)
#elif SIZEOF___INT64 == 8
#define VAL64(a) (a)
#define UVAL64(a) (a)
#endif

#endif

#ifdef FSUAE
#include "uae/atomic.h"
#else
uae_atomic atomic_and(volatile uae_atomic *p, uae_u32 v);
uae_atomic atomic_or(volatile uae_atomic *p, uae_u32 v);
uae_atomic atomic_inc(volatile uae_atomic *p);
uae_atomic atomic_dec(volatile uae_atomic *p);
uae_u32 atomic_bit_test_and_reset(volatile uae_atomic *p, uae_u32 v);
#endif

#ifdef HAVE_STRDUP
#define my_strdup _tcsdup
#else
extern TCHAR *my_strdup (const TCHAR*s);
#endif
extern TCHAR *my_strdup_ansi (const char*);
extern void my_trim (TCHAR*);
extern TCHAR *my_strdup_trim (const TCHAR*);
extern TCHAR *au (const char*);
extern char *ua (const TCHAR*);
extern TCHAR *aucp (const char *s, unsigned int cp);
extern char *uacp (const TCHAR *s, unsigned int cp);
extern TCHAR *au_fs (const char*);
extern char *ua_fs (const TCHAR*, int);
extern char *ua_copy (char *dst, int maxlen, const TCHAR *src);
extern TCHAR *au_copy (TCHAR *dst, int maxlen, const char *src);
extern char *ua_fs_copy (char *dst, int maxlen, const TCHAR *src, int defchar);
extern TCHAR *au_fs_copy (TCHAR *dst, int maxlen, const char *src);
extern char *uutf8 (const TCHAR *s);
extern TCHAR *utf8u (const char *s);
extern void unicode_init (void);
extern void to_lower (TCHAR *s, int len);
extern void to_upper (TCHAR *s, int len);
#ifdef FSUAE
// Defined in uae/string.h instead
#else
extern int uaestrlen(const char*);
extern int uaetcslen(const TCHAR*);
#endif

#define ENUMDECL typedef enum
#define ENUMNAME(name) name

/*
 * Porters to weird systems, look! This is the preferred way to get
 * filesys.c (and other stuff) running on your system. Define the
 * appropriate macros and implement wrappers in a machine-specific file.
 *
 * I guess the Mac port could use this (Ernesto?)
 */

#undef DONT_HAVE_POSIX
#undef DONT_HAVE_REAL_POSIX /* define if open+delete doesn't do what it should */
#undef DONT_HAVE_STDIO
#undef DONT_HAVE_MALLOC

#if defined(WARPUP)
#define DONT_HAVE_POSIX
#endif

#if !defined(FSUAE) && defined _WIN32

//#ifdef FSUAE
//#error _WIN32 should not be defined here
//#endif
#if defined __WATCOMC__

#define O_NDELAY 0
#include <direct.h>
#define dirent direct
#define mkdir(a,b) mkdir(a)
#define strcasecmp stricmp

#elif defined __MINGW32__

#include <winsock.h>

#define O_NDELAY 0

#define FILEFLAG_DIR     0x1
#define FILEFLAG_ARCHIVE 0x2
#define FILEFLAG_WRITE   0x4
#define FILEFLAG_READ    0x8
#define FILEFLAG_EXECUTE 0x10
#define FILEFLAG_SCRIPT  0x20
#define FILEFLAG_PURE    0x40

#define mkdir(a,b) mkdir(a)

#elif defined _MSC_VER

#ifdef HAVE_GETTIMEOFDAY
#include <winsock.h> // for 'struct timeval' definition
extern void gettimeofday( struct timeval *tv, void *blah );
#endif

#define O_NDELAY 0

#define FILEFLAG_DIR     0x1
#define FILEFLAG_ARCHIVE 0x2
#define FILEFLAG_WRITE   0x4
#define FILEFLAG_READ    0x8
#define FILEFLAG_EXECUTE 0x10
#define FILEFLAG_SCRIPT  0x20
#define FILEFLAG_PURE    0x40

#include <io.h>
#define O_BINARY _O_BINARY
#define O_WRONLY _O_WRONLY
#define O_RDONLY _O_RDONLY
#define O_RDWR   _O_RDWR
#define O_CREAT  _O_CREAT
#define O_TRUNC  _O_TRUNC
#define strcasecmp _tcsicmp 
#define strncasecmp _tcsncicmp 
#define W_OK 0x2
#define R_OK 0x4
#define STAT struct stat
#define DIR struct DIR
struct direct
{
    TCHAR d_name[1];
};
#include <sys/utime.h>
#define utimbuf __utimbuf64
#define USE_ZFILE

#undef S_ISDIR
#undef S_IWUSR
#undef S_IRUSR
#undef S_IXUSR
#define S_ISDIR(a) (a&FILEFLAG_DIR)
#define S_ISARC(a) (a&FILEFLAG_ARCHIVE)
#define S_IWUSR FILEFLAG_WRITE
#define S_IRUSR FILEFLAG_READ
#define S_IXUSR FILEFLAG_EXECUTE

#endif

#endif /* _WIN32 */

#ifdef DONT_HAVE_POSIX

#define access posixemu_access
extern int posixemu_access (const TCHAR *, int);
#define open posixemu_open
extern int posixemu_open (const TCHAR *, int, int);
#define close posixemu_close
extern void posixemu_close (int);
#define read posixemu_read
extern int posixemu_read (int, TCHAR *, int);
#define write posixemu_write
extern int posixemu_write (int, const TCHAR *, int);
#undef lseek
#define lseek posixemu_seek
extern int posixemu_seek (int, int, int);
#define stat(a,b) posixemu_stat ((a), (b))
extern int posixemu_stat (const TCHAR *, STAT *);
#define mkdir posixemu_mkdir
extern int mkdir (const TCHAR *, int);
#define rmdir posixemu_rmdir
extern int posixemu_rmdir (const TCHAR *);
#define unlink posixemu_unlink
extern int posixemu_unlink (const TCHAR *);
#define truncate posixemu_truncate
extern int posixemu_truncate (const TCHAR *, long int);
#define rename posixemu_rename
extern int posixemu_rename (const TCHAR *, const TCHAR *);
#define chmod posixemu_chmod
extern int posixemu_chmod (const TCHAR *, int);
#define tmpnam posixemu_tmpnam
extern void posixemu_tmpnam (TCHAR *);
#define utime posixemu_utime
extern int posixemu_utime (const TCHAR *, struct utimbuf *);
#define opendir posixemu_opendir
extern DIR * posixemu_opendir (const TCHAR *);
#define readdir posixemu_readdir
extern struct dirent* readdir (DIR *);
#define closedir posixemu_closedir
extern void closedir (DIR *);

/* This isn't the best place for this, but it fits reasonably well. The logic
 * is that you probably don't have POSIX errnos if you don't have the above
 * functions. */
extern long dos_errno (void);

#endif

#ifdef DONT_HAVE_STDIO

extern FILE *stdioemu_fopen (const TCHAR *, const TCHAR *);
#define fopen(a,b) stdioemu_fopen(a, b)
extern int stdioemu_fseek (FILE *, int, int);
#define fseek(a,b,c) stdioemu_fseek(a, b, c)
extern int stdioemu_fread (TCHAR *, int, int, FILE *);
#define fread(a,b,c,d) stdioemu_fread(a, b, c, d)
extern int stdioemu_fwrite (const TCHAR *, int, int, FILE *);
#define fwrite(a,b,c,d) stdioemu_fwrite(a, b, c, d)
extern int stdioemu_ftell (FILE *);
#define ftell(a) stdioemu_ftell(a)
extern int stdioemu_fclose (FILE *);
#define fclose(a) stdioemu_fclose(a)

#endif

#ifdef DONT_HAVE_MALLOC

#define malloc(a) mallocemu_malloc(a)
extern void *mallocemu_malloc (int size);
#define free(a) mallocemu_free(a)
extern void mallocemu_free (void *ptr);

#endif

#ifdef X86_ASSEMBLY
//#define ASM_SYM_FOR_FUNC(a) __asm__(a)
#define ASM_SYM_FOR_FUNC(a)
#else
#define ASM_SYM_FOR_FUNC(a)
#endif

#include "target.h"

#ifdef UAE_CONSOLE
#undef write_log
#define write_log write_log_standard
#endif

#if __GNUC__ - 1 > 1 || __GNUC_MINOR__ - 1 > 6
extern void write_log(const TCHAR *, ...);
extern void write_logx(const TCHAR *, ...);
extern void write_log(const char *, ...) __attribute__ ((format (printf, 1, 2)));
#else
extern void write_log(const TCHAR *, ...);
extern void write_logx(const TCHAR *, ...);
extern void write_log(const char *, ...);
#endif
extern void write_dlog (const TCHAR *, ...);
extern int read_log(void);

extern void flush_log (void);
extern TCHAR *setconsolemode (TCHAR *buffer, int maxlen);
extern void close_console (void);
extern void open_console(void);
extern void reopen_console(void);
extern void activate_console(void);
extern void deactivate_console(void);
extern void console_out (const TCHAR *);
extern void console_out_f (const TCHAR *, ...);
extern void console_flush (void);
extern int console_get (TCHAR *, int);
extern bool console_isch (void);
extern TCHAR console_getch (void);
extern void f_out (void *, const TCHAR *, ...);
extern TCHAR* buf_out (TCHAR *buffer, int *bufsize, const TCHAR *format, ...);
extern void gui_message (const TCHAR *,...);
extern int gui_message_multibutton (int flags, const TCHAR *format,...);
#define write_log_err write_log
extern void logging_init (void);
extern FILE *log_open (const TCHAR *name, int append, int bootlog, TCHAR*);
extern void log_close (FILE *f);
extern TCHAR *write_log_get_ts(void);
extern bool is_console_open(void);

extern bool use_long_double;

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifdef FSUAE
#include "uae/inline.h"
#else
#ifndef STATIC_INLINE
#if __GNUC__ - 1 > 2 || (__GNUC__ - 1 == 2 && __GNUC_MINOR__ - 1 >= 0)
#define STATIC_INLINE static __inline__ __attribute__ ((always_inline))
#define NOINLINE __attribute__ ((noinline))
#define NORETURN __attribute__ ((noreturn))
#elif _MSC_VER
#define STATIC_INLINE static __forceinline
#define NOINLINE __declspec(noinline)
#define NORETURN __declspec(noreturn)
#else
#define STATIC_INLINE static __inline__
#define NOINLINE
#define NORETURN
#endif
#endif
#endif

/* Every Amiga hardware clock cycle takes this many "virtual" cycles.  This
   used to be hardcoded as 1, but using higher values allows us to time some
   stuff more precisely.
   512 is the official value from now on - it can't change, unless we want
   _another_ config option "finegrain2_m68k_speed".

   We define this value here rather than in events.h so that gencpu.c sees
   it.  */
#define CYCLE_UNIT 512

/* This one is used by cfgfile.c.  We could reduce the CYCLE_UNIT back to 1,
   I'm not 100% sure this code is bug free yet.  */
#define OFFICIAL_CYCLE_UNIT 512

/*
 * You can specify numbers from 0 to 5 here. It is possible that higher
 * numbers will make the CPU emulation slightly faster, but if the setting
 * is too high, you will run out of memory while compiling.
 * Best to leave this as it is.
 */
#define CPU_EMU_SIZE 0

#ifndef __cplusplus

#define xmalloc(T, N) malloc(sizeof (T) * (N))
#define xcalloc(T, N) calloc(sizeof (T), N)
#define xfree(T) free(T)
#define xrealloc(T, TP, N) realloc(TP, sizeof (T) * (N))

#if 0
extern void *xmalloc (size_t);
extern void *xcalloc (size_t, size_t);
extern void xfree (const void*);
#endif

#else

#define xmalloc(T, N) static_cast<T*>(malloc (sizeof (T) * (N)))
#define xcalloc(T, N) static_cast<T*>(calloc (sizeof (T), N))
#define xrealloc(T, TP, N) static_cast<T*>(realloc (TP, sizeof (T) * (N)))
#define xfree(T) free(T)

#endif

#define DBLEQU(f, i) (abs ((f) - (i)) < 0.000001)

#ifdef HAVE_VAR_ATTRIBUTE_UNUSED
#define NOWARN_UNUSED(x) __attribute__((unused)) x
#else
#define NOWARN_UNUSED(x) x
#endif

#include "uae/inline.h"
#include "uae/io.h"
#include "uae/types.h"
#include "uae/likely.h"

#ifdef FSUAE

#include "uae/byteswap.h"
#include "uae/declarations.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int fsemu;

#ifdef __cplusplus
}
#endif

#endif

#endif /* UAE_SYSDEPS_H */
