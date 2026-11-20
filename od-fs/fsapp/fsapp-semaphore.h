/*
 * Semaphores.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_SEMAPHORE_H
#define FSAPP_SEMAPHORE_H

#include <stdint.h>

#include "fsapp-config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSAPP_SEMAPHORE_TIMEOUT 1

struct fsapp_semaphore_t;
typedef struct fsapp_semaphore_t fsapp_semaphore_t;

void fsapp_semaphore_init(fsapp_semaphore_t *semaphore, int value);

fsapp_semaphore_t *fsapp_semaphore_create(int value);

void fsapp_semaphore_destroy(fsapp_semaphore_t *semaphore);

int fsapp_semaphore_post(fsapp_semaphore_t *semaphore);

int fsapp_semaphore_wait(fsapp_semaphore_t *semaphore);

int fsapp_semaphore_try_wait(fsapp_semaphore_t *semaphore);

// Waits until the semaphore can be taken, or timeout_ns has passed (FSAPP_SEMAPHORE_TIMEOUT). SDL
// waits whole milliseconds: the timeout is rounded up. A negative timeout waits for ever.
int fsapp_semaphore_wait_timeout_ns(fsapp_semaphore_t *semaphore, int64_t timeout_ns);

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_SEMAPHORE_H */
