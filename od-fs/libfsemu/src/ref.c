#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/ref.h>
#include <fs/log.h>
#include <stdio.h>

void fs_ref_initialize(void *ptr, void (*destroy_function)(void *ptr))
{
    struct fs_ref *ref = (struct fs_ref *) ptr;
    ref->magic = FS_REF_MAGIC;
    ref->count = 1;
    ref->destroy = destroy_function;
}

int fs_ref(void *ptr)
{
    struct fs_ref *ref = (struct fs_ref *) ptr;
    ref->count += 1;
    return ref->count;
}

int fs_unref(void *ptr)
{
    struct fs_ref* ref = (struct fs_ref *) ptr;
    if (ref->magic != FS_REF_MAGIC) {
        fs_log("ERROR: not UNREF'ING fs-ref struct\n");
        return -1;
    }
    ref->count -= 1;
    if (ref->count == 0) {
        ref->magic = 0x00000000;
        ref->destroy(ptr);
        return 0;
    }
    return ref->count;
}
