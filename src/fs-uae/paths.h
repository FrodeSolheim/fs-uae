const char* fs_uae_base_dir(void);
const char *fs_uae_plugins_dir(void);
const char *fs_uae_system_dir(void);
const char *fs_uae_data_dir(void);

void fs_uae_set_state_base_name(const char *base_name);

#define MAX_PATHS 8
#define ZERO_SUCCESS 0

struct multipath {
    const char *path[MAX_PATHS];
};

extern struct multipath g_paths[5];
