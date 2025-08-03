#define FSEMU_INTERNAL
#include "fslib-refable.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "fsemu-util.h"

// FIXME: Might want to use atomatic incr/decr function to make these
// operations thread-safe as well.

void fslib_refable_init_refable(fslib_refable_t* refable)
{
    fsemu_assert(refable != NULL);
    refable->magic[0] = 'F';
    refable->magic[1] = 'R';
    refable->magic[2] = 'E';
    refable->magic[3] = 'F';
    refable->ref_count = 1;
    refable->cleanup_handler = NULL;
}

void fslib_refable_set_cleanup_handler_refable(
    fslib_refable_t* refable, void (*cleanup_handler)(void*)
)
{
    refable->cleanup_handler = cleanup_handler;
}

void fslib_refable_init_refable_with_finalizer(
    fslib_refable_t* refable, void (*finalizer)(void*)
)
{
    fslib_refable_init_refable(refable);
    fslib_refable_set_cleanup_handler_refable(refable, finalizer);
}

static bool fslib_refable_check_magic(fslib_refable_t* refable)
{
    if (refable->magic[0] == 'F' && refable->magic[1] == 'R'
        && refable->magic[2] == 'E' && refable->magic[3] == 'F') {
        return true;
    } else {
        // FIXME: Critical
        fprintf(stderr, "[FSE] [REF] ERROR: Refable magic check failed\n");
        return false;
    }
}

void fslib_refable_ref_refable(fslib_refable_t* refable)
{
    fsemu_assert(refable != NULL);
    fsemu_assert(fslib_refable_check_magic(refable));
    // if (!fslib_refable_check_magic(refable)) {
    //     return;
    // }
    refable->ref_count += 1;
}

void fslib_refable_unref_refable(fslib_refable_t* refable)
{
    fsemu_assert(refable != NULL);
    if (!fslib_refable_check_magic(refable)) {
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
