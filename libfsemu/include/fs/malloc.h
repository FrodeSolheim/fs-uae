#ifndef FS_MALLOC_H
#define FS_MALLOC_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_GLIB

#include <glib.h>

#else

void *fs_malloc0(size_t n_bytes) FS_MALLOC /* G_GNUC_ALLOC_SIZE(1) */;

#define g_malloc0 fs_malloc0

#define g_new(struct_type, n_structs) (struct_type *) \
        malloc(sizeof(struct_type) * (n_structs))

#define g_new0(struct_type, n_structs) (struct_type *) \
        fs_malloc0(sizeof(struct_type) * (n_structs))

#endif

#endif /* FS_MALLOC_H */
