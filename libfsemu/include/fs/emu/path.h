#ifndef FS_EMU_PATH_H
#define FS_EMU_PATH_H

typedef char *(*fs_emu_path_expand_function)(const char *path);

void fs_emu_path_set_expand_function(fs_emu_path_expand_function function);

#ifdef FSEMU_INTERNAL

char *fs_emu_path_expand(const char *path);
char *fs_emu_path_expand_and_free(char *path);

#endif /* FSEMU_INTERNAL*/

#endif /* FS_EMU_PATH_H */
