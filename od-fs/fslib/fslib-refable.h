#ifndef FSLIB_REFABLE_H_
#define FSLIB_REFABLE_H_

#include "fslib-config.h"

// #include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

// FIXME: Define enum type
#define FSEMU_REF_ASSIGN true

struct fslib_refable_struct {
    char magic[4];
    int ref_count;
    void (*cleanup_handler)(void*);
};

typedef struct fslib_refable_struct fslib_refable_t;

void fslib_refable_init_refable(fslib_refable_t* refable);
void fslib_refable_init_refable_with_finalizer(fslib_refable_t* refable, void (*finalizer)(void*));
void fslib_refable_set_cleanup_handler_refable(fslib_refable_t* refable,
                                               void (*cleanup_handler)(void*));
void fslib_refable_ref_refable(fslib_refable_t* refable);
void fslib_refable_unref_refable(fslib_refable_t* refable);

#define fslib_refable_init(a) fslib_refable_init_refable(&((a)->refable))
#define fslib_refable_init_with_finalizer(a, b) \
    fslib_refable_init_refable_with_finalizer(&((a)->refable), b)
#define fslib_refable_set_cleanup_handler(a, b) \
    fslib_refable_set_cleanup_handler_refable(&((a)->refable), b)
#define fslib_refable_ref(a) fslib_refable_ref_refable(&((a)->refable))
#define fslib_refable_unref(a) fslib_refable_unref_refable(&((a)->refable))

// Macro to insert struct member. Use at top of object.
#define FSLIB_REFABLE fslib_refable_t refable

#ifdef __cplusplus
}
#endif

#endif  // FSLIB_REFABLE_H_
