#ifndef FSEMU_REFABLE_H_
#define FSEMU_REFABLE_H_

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

// FIXME: Define enum type
#define FSEMU_REF_ASSIGN true

struct fsemu_refable_struct {
    char magic[4];
    int ref_count;
    void (*cleanup_handler)(void *);
};

typedef struct fsemu_refable_struct fsemu_refable_t;

void fsemu_refable_init_refable(fsemu_refable_t *refable);
void fsemu_refable_init_refable_with_finalizer(fsemu_refable_t *refable,
                                               void (*finalizer)(void *));
void fsemu_refable_set_cleanup_handler_refable(
    fsemu_refable_t *refable, void (*cleanup_handler)(void *));
void fsemu_refable_ref_refable(fsemu_refable_t *refable);
void fsemu_refable_unref_refable(fsemu_refable_t *refable);

#define fsemu_refable_init(a) fsemu_refable_init_refable(&((a)->refable))
#define fsemu_refable_init_with_finalizer(a, b) \
    fsemu_refable_init_refable_with_finalizer(&((a)->refable), b)
#define fsemu_refable_set_cleanup_handler(a, b) \
    fsemu_refable_set_cleanup_handler_refable(&((a)->refable), b)
#define fsemu_refable_ref(a) fsemu_refable_ref_refable(&((a)->refable))
#define fsemu_refable_unref(a) fsemu_refable_unref_refable(&((a)->refable))

// Macro to insert struct member. Use at top of object.
#define FSEMU_REFABLE fsemu_refable_t refable

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_REFABLE_H_
