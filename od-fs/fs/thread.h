/*
 * Thread utilities that complement SDL's threading API, and telling the system that the
 * program's timing is latency-critical.
 *
 * This module has no dependencies on other FS modules.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FS_THREAD_H
#define FS_THREAD_H

#include <SDL3/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the name of the current thread.
 *
 * This is SDL_GetCurrentThreadName() for an SDL older than 3.6, which does not have it: with SDL
 * 3.6 or newer, it returns what that returns, and when SDL 3.6 is the minimum, its callers call
 * SDL's function instead. Threads are named when they are created (SDL_CreateThread()'s name); with
 * an older SDL, the name is asked from the operating system on the thread's first call, as SDL 3.6
 * does for threads that SDL did not create (pthread_getname_np() on Linux and macOS,
 * GetThreadDescription() on Windows 10 1607 or newer).
 *
 * With an older SDL on Linux, a thread that was not named has the name of the thread that created
 * it: the program's name, which counts as no name, or the name of the named thread that created
 * it, as Linux does not tell a name that was set from one that was inherited.
 *
 * With an older SDL, the name is UTF-8 cut at its first invalid sequence (such as a character that
 * the operating system's length limit cut in two: Linux keeps 15 bytes of a name), and a name
 * longer than 255 bytes is cut at a character boundary.
 *
 * The returned string belongs to the calling thread and lives until the thread ends. If it needs
 * to be passed to another thread that might outlive the calling thread, make a copy of it.
 *
 * \returns the name of the current thread, in UTF-8, or NULL if it has none.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
const char *FS_GetCurrentThreadName(void);

/**
 * Ask for the current thread to be run as one the user is waiting on.
 *
 * For a thread that takes part in making each frame. On macOS, the thread gets the
 * user-interactive quality of service class, which keeps it on the fast cores and clocks also
 * while the user is not giving input. This fails for a thread that has left the quality of service
 * classes by setting a scheduling policy of its own (SDL_SetCurrentThreadPriority(), for example).
 * On other systems, this does nothing.
 *
 * \returns true on success (also where there is nothing to do) or false on failure; call
 *          SDL_GetError() for more information.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
bool FS_SetCurrentThreadInteractive(void);

/**
 * Tell the system that the program's timing is latency-critical, until the process ends.
 *
 * On macOS, this begins an NSProcessInfo activity (user-initiated, latency-critical), which keeps
 * App Nap and timer coalescing off also while the user is not giving input. Idle system sleep is
 * not held off. Once the activity has begun, later calls do nothing. On other systems, this does
 * nothing.
 *
 * \returns true on success (also where there is nothing to do) or false on failure; call
 *          SDL_GetError() for more information.
 *
 * \threadsafety This function should only be called on the main thread.
 */
bool FS_BeginLatencyCriticalActivity(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FS_THREAD_H */
