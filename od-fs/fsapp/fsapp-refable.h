/*
 * Reference counting for structs that embed FSAPP_REFABLE.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_REFABLE_H
#define FSAPP_REFABLE_H

#include "fsapp-config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsapp_refable_struct {
    char magic[4];
    int ref_count;
    void (*cleanup_handler)(void *);
};

typedef struct fsapp_refable_struct fsapp_refable_t;

// The functions take the object's refable member: fsapp_refable_ref(&image->refable). The
// cleanup handler (or finalizer) is called with the refable when the last reference is released,
// which is the object itself since FSAPP_REFABLE is its first member.
void fsapp_refable_init(fsapp_refable_t *refable);
void fsapp_refable_init_with_finalizer(fsapp_refable_t *refable, void (*finalizer)(void *));
void fsapp_refable_set_cleanup_handler(fsapp_refable_t *refable, void (*cleanup_handler)(void *));
void fsapp_refable_ref(fsapp_refable_t *refable);
void fsapp_refable_unref(fsapp_refable_t *refable);

// Macro to insert struct member. Use at top of object.
#define FSAPP_REFABLE fsapp_refable_t refable

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_REFABLE_H */
