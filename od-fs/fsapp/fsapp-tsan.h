/*
 * ThreadSanitizer annotations, for synchronisation that ThreadSanitizer cannot see.
 *
 * GLib's mutexes (GMutex, and with it GAsyncQueue and GCond) are built on futexes on Linux, not on
 * pthread mutexes, and GLib is not built with ThreadSanitizer, so a ThreadSanitizer build
 * (-fsanitize=thread) does not see them synchronise: whatever one thread hands another under such
 * a lock is reported as a data race. Code that still uses them marks the lock for it:
 * FSAPP_TSAN_ACQUIRE(lock) after taking the lock (and after a wait that takes it again), and
 * FSAPP_TSAN_RELEASE(lock) before releasing it. In other builds the macros do nothing.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_TSAN_H
#define FSAPP_TSAN_H

#if defined(__SANITIZE_THREAD__)
#define FSAPP_TSAN 1
#elif defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define FSAPP_TSAN 1
#endif
#endif

#ifdef FSAPP_TSAN

#ifdef __cplusplus
extern "C" {
#endif

// ThreadSanitizer's own interface (sanitizer/tsan_interface.h).
void __tsan_acquire(void *addr);
void __tsan_release(void *addr);

#ifdef __cplusplus
} // extern "C"
#endif

#define FSAPP_TSAN_ACQUIRE(lock) __tsan_acquire((void *)(lock))
#define FSAPP_TSAN_RELEASE(lock) __tsan_release((void *)(lock))

#else

#define FSAPP_TSAN_ACQUIRE(lock) ((void)0)
#define FSAPP_TSAN_RELEASE(lock) ((void)0)

#endif // FSAPP_TSAN

#endif /* FSAPP_TSAN_H */
