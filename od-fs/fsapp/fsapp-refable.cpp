/*
 * Reference counting for structs that embed FSAPP_REFABLE.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-refable.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "fsapp-util.h"

// FIXME: Might want to use atomatic incr/decr function to make these
// operations thread-safe as well.

void fsapp_refable_init(fsapp_refable_t *refable)
{
    fsapp_assert(refable != NULL);
    refable->magic[0] = 'F';
    refable->magic[1] = 'R';
    refable->magic[2] = 'E';
    refable->magic[3] = 'F';
    refable->ref_count = 1;
    refable->cleanup_handler = NULL;
}

void fsapp_refable_set_cleanup_handler(fsapp_refable_t *refable, void (*cleanup_handler)(void *))
{
    refable->cleanup_handler = cleanup_handler;
}

void fsapp_refable_init_with_finalizer(fsapp_refable_t *refable, void (*finalizer)(void *))
{
    fsapp_refable_init(refable);
    fsapp_refable_set_cleanup_handler(refable, finalizer);
}

static bool fsapp_refable_check_magic(fsapp_refable_t *refable)
{
    if (refable->magic[0] == 'F' && refable->magic[1] == 'R' && refable->magic[2] == 'E' &&
        refable->magic[3] == 'F') {
        return true;
    } else {
        // FIXME: Critical
        fprintf(stderr, "[FSE] [REF] ERROR: Refable magic check failed\n");
        return false;
    }
}

void fsapp_refable_ref(fsapp_refable_t *refable)
{
    fsapp_assert(refable != NULL);
    fsapp_assert(fsapp_refable_check_magic(refable));
    // if (!fsapp_refable_check_magic(refable)) {
    //     return;
    // }
    refable->ref_count += 1;
}

void fsapp_refable_unref(fsapp_refable_t *refable)
{
    fsapp_assert(refable != NULL);
    if (!fsapp_refable_check_magic(refable)) {
        return;
    }
    // FIXME: Check magic
    refable->ref_count -= 1;
    if (refable->ref_count == 0) {
        if (refable->cleanup_handler) {
            refable->cleanup_handler(refable);
        } else {
            fprintf(stderr, "[FSE] [REF] WARNING: No cleanup handler\n");
        }
    }
}
