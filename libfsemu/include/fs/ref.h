#ifndef FS_REF_H
#define FS_REF_H

#define FS_REF_MAGIC 0x00c8a74d

struct fs_ref {
    int magic;
    int count;
    void (*destroy)(void* ptr);
};

void fs_ref_initialize(void *ptr, void (*destroy_function)(void *ptr));
int fs_ref(void* ptr);
int fs_unref(void* ptr);

#endif /* FS_REF_H */
