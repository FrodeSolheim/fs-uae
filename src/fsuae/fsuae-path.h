#ifndef FSUAE_PATH_H_
#define FSUAE_PATH_H_

#include "fsuae-config.h"

void fsuae_path_init(void);

char *fsuae_path_expand(const char *path);
char *fsuae_path_expand_and_free(char *path);

char *fsuae_path_resolve(const char *name, int type);
char *fsuae_path_resolve_and_free(char *name, int type);

const char *fsuae_path_configs_dir();
const char *fsuae_path_cdroms_dir();
const char *fsuae_path_floppies_dir();
const char *fsuae_path_harddrives_dir();
const char *fsuae_path_kickstarts_dir();
// const char *fs_uae_flash_memory_dir();
// const char *fsuae_path_savestates_dir();
const char *fsuae_path_state_dir();
// const char *fs_uae_floppy_overlays_dir();
const char *fsuae_path_controllers_dir();
const char *fsuae_path_logs_dir();
const char *fsuae_path_moduleripper_dir();
const char *fsuae_path_exe_dir();
const char *fsuae_path_themes_dir();
const char *fsuae_path_cache_dir(void);
const char *fsuae_path_kickstartcache_dir();
const char *fsuae_path_base_dir(void);
const char *fsuae_path_plugins_dir(void);
const char *fsuae_path_data_dir(void);

void fs_uae_init_path_resolver();
void fs_uae_set_uae_paths();
void fs_uae_set_state_base_name(const char *base_name);

#define MAX_PATHS 8
#define ZERO_SUCCESS 0

struct multipath {
    const char *path[MAX_PATHS];
};

extern struct multipath g_paths[5];

#endif  // FSUAE_PATH_H_
