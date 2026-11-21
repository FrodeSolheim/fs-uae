/*
 * A queue of pointers between threads. See asyncqueue.h.
 *
 * This module has no dependencies on other FS modules.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fs/asyncqueue.h"

#include <SDL3/SDL.h>

#include <deque>
#include <new>

struct FS_AsyncQueue {
    SDL_Mutex *mutex;
    // Signalled for each item pushed.
    SDL_Condition *pushed;
    std::deque<void *> items;
};

FS_AsyncQueue *FS_CreateAsyncQueue(void)
{
    FS_AsyncQueue *queue;
    // The deque's constructor can allocate too, and no exception may leave the C API.
    try {
        queue = new FS_AsyncQueue();
    } catch (const std::bad_alloc &) {
        SDL_OutOfMemory();
        return NULL;
    }
    queue->mutex = SDL_CreateMutex();
    queue->pushed = SDL_CreateCondition();
    if (queue->mutex == NULL || queue->pushed == NULL) {
        FS_DestroyAsyncQueue(queue);
        return NULL;
    }
    return queue;
}

void FS_DestroyAsyncQueue(FS_AsyncQueue *queue)
{
    if (queue == NULL) {
        return;
    }
    SDL_DestroyCondition(queue->pushed);
    SDL_DestroyMutex(queue->mutex);
    delete queue;
}

void FS_LockAsyncQueue(FS_AsyncQueue *queue)
{
    SDL_LockMutex(queue->mutex);
}

void FS_UnlockAsyncQueue(FS_AsyncQueue *queue)
{
    SDL_UnlockMutex(queue->mutex);
}

// noexcept: a failed allocation in the deque ends the program (std::terminate()), as no exception
// may leave the C API.
static void push_item(FS_AsyncQueue *queue, void *item, bool at_front) noexcept
{
    SDL_assert(item != NULL);
    if (item == NULL) {
        return;
    }
    if (at_front) {
        queue->items.push_front(item);
    } else {
        queue->items.push_back(item);
    }
    SDL_SignalCondition(queue->pushed);
}

void FS_PushAsyncQueueUnlocked(FS_AsyncQueue *queue, void *item)
{
    push_item(queue, item, false);
}

void FS_PushFrontAsyncQueueUnlocked(FS_AsyncQueue *queue, void *item)
{
    push_item(queue, item, true);
}

void FS_PushAsyncQueue(FS_AsyncQueue *queue, void *item)
{
    SDL_LockMutex(queue->mutex);
    FS_PushAsyncQueueUnlocked(queue, item);
    SDL_UnlockMutex(queue->mutex);
}

void *FS_TryPopAsyncQueueUnlocked(FS_AsyncQueue *queue)
{
    if (queue->items.empty()) {
        return NULL;
    }
    void *item = queue->items.front();
    queue->items.pop_front();
    return item;
}

void *FS_TryPopAsyncQueue(FS_AsyncQueue *queue)
{
    SDL_LockMutex(queue->mutex);
    void *item = FS_TryPopAsyncQueueUnlocked(queue);
    SDL_UnlockMutex(queue->mutex);
    return item;
}

void *FS_PopAsyncQueueUnlockedTimeout(FS_AsyncQueue *queue, int32_t timeout_ms)
{
    if (timeout_ms < 0) {
        while (queue->items.empty()) {
            SDL_WaitCondition(queue->pushed, queue->mutex);
        }
        return FS_TryPopAsyncQueueUnlocked(queue);
    }
    Uint64 deadline_ns = SDL_GetTicksNS() + SDL_MS_TO_NS(timeout_ms);
    while (queue->items.empty()) {
        Uint64 now_ns = SDL_GetTicksNS();
        if (now_ns >= deadline_ns) {
            return NULL;
        }
        // The time left, rounded up so that the wait does not end early. Woken without an item
        // (spuriously, or another thread popped it), the loop waits again for what is left.
        SDL_WaitConditionTimeout(queue->pushed, queue->mutex,
                                 (int32_t)SDL_NS_TO_MS(deadline_ns - now_ns + SDL_NS_PER_MS - 1));
    }
    return FS_TryPopAsyncQueueUnlocked(queue);
}

void *FS_PopAsyncQueueTimeout(FS_AsyncQueue *queue, int32_t timeout_ms)
{
    SDL_LockMutex(queue->mutex);
    void *item = FS_PopAsyncQueueUnlockedTimeout(queue, timeout_ms);
    SDL_UnlockMutex(queue->mutex);
    return item;
}
