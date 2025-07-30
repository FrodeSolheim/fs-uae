#define FSEMU_INTERNAL
#include "fsemu-refable.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "fsemu-util.h"

// FIXME: Might want to use atomatic incr/decr function to make these
// operations thread-safe as well.

void fsemu_refable_init_refable(fsemu_refable_t *refable)
{
    fsemu_assert(refable != NULL);
    refable->magic[0] = 'F';
    refable->magic[1] = 'R';
    refable->magic[2] = 'E';
    refable->magic[3] = 'F';
    refable->ref_count = 1;
    refable->cleanup_handler = NULL;
}

void fsemu_refable_set_cleanup_handler_refable(fsemu_refable_t *refable,
                                               void (*cleanup_handler)(void *))
{
    refable->cleanup_handler = cleanup_handler;
}

void fsemu_refable_init_refable_with_finalizer(fsemu_refable_t *refable,
                                               void (*finalizer)(void *))
{
    fsemu_refable_init_refable(refable);
    fsemu_refable_set_cleanup_handler_refable(refable, finalizer);
}

static bool fsemu_refable_check_magic(fsemu_refable_t *refable)
{
    if (refable->magic[0] == 'F' && refable->magic[1] == 'R' &&
        refable->magic[2] == 'E' && refable->magic[3] == 'F') {
        return true;
    } else {
        // FIXME: Critical
        fprintf(stderr, "[FSE] [REF] ERROR: Refable magic check failed\n");
        return false;
    }
}

void fsemu_refable_ref_refable(fsemu_refable_t *refable)
{
    fsemu_assert(refable != NULL);
    fsemu_assert(fsemu_refable_check_magic(refable));
    // if (!fsemu_refable_check_magic(refable)) {
    //     return;
    // }
    refable->ref_count += 1;
}

void fsemu_refable_unref_refable(fsemu_refable_t *refable)
{
    fsemu_assert(refable != NULL);
    if (!fsemu_refable_check_magic(refable)) {
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
