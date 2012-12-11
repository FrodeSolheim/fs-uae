// FIXME: consider using autotools for this

#if defined(WINDOWS)
#elif defined(MACOSX) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1060
#else
#define HAVE_POSIX_MEMALIGN 1
#endif

#if !defined(WINDOWS)
#define POSIX_SERIAL 1
#endif

#if defined(WINDOWS) || defined(LINUX)
#define HAVE_FSEEKO64 1
#endif

#if defined(WINDOWS) || defined(LINUX)
#define HAVE_FTELLO64 1
#endif

//#if defined(FREEBSD) || defined(OPENBSD)
#if !defined(WINDOWS)
#define HAVE_SYS_MOUNT_H 1
#endif

//#if defined(FREEBSD) || defined(OPENBSD)
#if !defined(WINDOWS)
#define HAVE_SYS_PARAM_H 1
#endif

#if defined(LINUX)
#define HAVE_SYS_VFS_H 1
#endif

#if defined(LINUX)
#define HAVE_SYS_STATFS_H 1
#endif
