// FIXME: consider using autotools for this

#if defined(WINDOWS)

#elif defined(MACOSX)
#define POSIX_SERIAL
#else
#define HAVE_POSIX_MEMALIGN
#define POSIX_SERIAL
#endif
