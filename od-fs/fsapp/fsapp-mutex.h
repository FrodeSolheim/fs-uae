/*
 * Mutexes.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_MUTEX_H
#define FSAPP_MUTEX_H

#include "fsapp-config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsapp_mutex;
typedef struct fsapp_mutex fsapp_mutex_t;

void fsapp_mutex_init(fsapp_mutex_t *mutex);

fsapp_mutex_t *fsapp_mutex_create(void);

void fsapp_mutex_destroy(fsapp_mutex_t *mutex);

int fsapp_mutex_lock(fsapp_mutex_t *mutex);

int fsapp_mutex_unlock(fsapp_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_MUTEX_H */
