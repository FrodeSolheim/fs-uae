// FIXME: consider using autotools for this

#if defined(WINDOWS)

#elif defined(MACOSX)

#else
#define HAVE_POSIX_MEMALIGN
#endif
