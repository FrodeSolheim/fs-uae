/*
 * A queue of pointers between threads, which SDL does not have.
 *
 * Items are pushed at the back and popped from the front, under the queue's own lock, and a pop
 * can wait for an item. Items are never NULL, which a pop returns for no item. The functions with
 * Unlocked in their names are for a caller that holds the lock (FS_LockAsyncQueue()), to do
 * several operations as one.
 *
 * This module has no dependencies on other FS modules.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FS_ASYNCQUEUE_H
#define FS_ASYNCQUEUE_H

#include <SDL3/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A queue of pointers between threads.
 */
typedef struct FS_AsyncQueue FS_AsyncQueue;

/**
 * Create an empty queue.
 *
 * \returns the new queue on success or NULL on failure; call SDL_GetError() for more information.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
FS_AsyncQueue *FS_CreateAsyncQueue(void);

/**
 * Destroy a queue.
 *
 * Items still in the queue are not freed: they are the caller's. No thread may use the queue any
 * more.
 *
 * \param queue the queue to destroy, or NULL.
 */
void FS_DestroyAsyncQueue(FS_AsyncQueue *queue);

/**
 * Lock the queue, for the functions with Unlocked in their names.
 *
 * The functions without Unlocked must not be called while holding it. SDL's mutexes are
 * recursive, but a pop that waits releases the lock with SDL_WaitCondition(), which needs it held
 * exactly once.
 *
 * \param queue the queue to lock.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
void FS_LockAsyncQueue(FS_AsyncQueue *queue);

/**
 * Unlock the queue, locked with FS_LockAsyncQueue().
 *
 * \param queue the queue to unlock.
 */
void FS_UnlockAsyncQueue(FS_AsyncQueue *queue);

/**
 * Push an item at the back of the queue, and wake a thread waiting to pop.
 *
 * \param queue the queue.
 * \param item the item, not NULL (a pop returns NULL for no item): a NULL item fails an assertion
 *             and is not pushed.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
void FS_PushAsyncQueue(FS_AsyncQueue *queue, void *item);

/**
 * Push an item at the back of the queue, while holding its lock.
 *
 * \param queue the queue, locked by the caller.
 * \param item the item, not NULL (see FS_PushAsyncQueue()).
 */
void FS_PushAsyncQueueUnlocked(FS_AsyncQueue *queue, void *item);

/**
 * Push an item at the front of the queue, while holding its lock, so that it is popped next.
 *
 * \param queue the queue, locked by the caller.
 * \param item the item, not NULL (see FS_PushAsyncQueue()).
 */
void FS_PushFrontAsyncQueueUnlocked(FS_AsyncQueue *queue, void *item);

/**
 * Pop the item at the front of the queue, without waiting.
 *
 * \param queue the queue.
 * \returns the item, or NULL if the queue is empty.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
void *FS_TryPopAsyncQueue(FS_AsyncQueue *queue);

/**
 * Pop the item at the front of the queue, without waiting, while holding its lock.
 *
 * \param queue the queue, locked by the caller.
 * \returns the item, or NULL if the queue is empty.
 */
void *FS_TryPopAsyncQueueUnlocked(FS_AsyncQueue *queue);

/**
 * Pop the item at the front of the queue, waiting up to a time for one to be pushed.
 *
 * \param queue the queue.
 * \param timeout_ms how long to wait, in milliseconds: 0 to not wait, or negative (such as -1) to
 *                   wait until an item is pushed.
 * \returns the item, or NULL if none was pushed before the timeout.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
void *FS_PopAsyncQueueTimeout(FS_AsyncQueue *queue, int32_t timeout_ms);

/**
 * Pop the item at the front of the queue, waiting up to a time, while holding its lock.
 *
 * As FS_PopAsyncQueueTimeout(); the lock is released while waiting, and held again when this
 * returns. The caller must hold the lock exactly once (see FS_LockAsyncQueue()).
 *
 * \param queue the queue, locked by the caller.
 * \param timeout_ms how long to wait, in milliseconds: 0 to not wait, or negative (such as -1) to
 *                   wait until an item is pushed.
 * \returns the item, or NULL if none was pushed before the timeout.
 */
void *FS_PopAsyncQueueUnlockedTimeout(FS_AsyncQueue *queue, int32_t timeout_ms);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FS_ASYNCQUEUE_H */
