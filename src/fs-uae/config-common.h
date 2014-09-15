#ifndef FS_UAE_CONFIG_COMMON_H
#define FS_UAE_CONFIG_COMMON_H

enum {
    CONFIG_A500,
    CONFIG_A500P,
    CONFIG_A600,
    CONFIG_CD32,
    CONFIG_CDTV,
    CONFIG_A1200,
    CONFIG_A1200_020,
    CONFIG_SUPER,
    CONFIG_A4000,
    CONFIG_A4000_040,
    CONFIG_A1000,
    CONFIG_A3000,
    CONFIG_CD32_FMV,
    CONFIG_LAST,
};

enum {
    MODEL_INVALID,
    MODEL_A500,
    MODEL_A1200,
    MODEL_CD32,
    MODEL_CDTV,
    MODEL_A500P,
    MODEL_A600,
    MODEL_A4000,
    MODEL_A1000,
    MODEL_A3000,
};

typedef struct amiga_config {
    const char *id;
    const char *name;
    int model;
    int quickstart_model;
    int quickstart_config;
    int allow_z3_memory;
    int z3mem_size;
    const char *cpu_model;
    int cpu_32bit_addressing;
    int fast;
    int fast_on_accuracy_level;
    int no_accuracy_adjustment;
    const char *warning;
    int enhanced_audio_filter;
} amiga_config;

extern amiga_config g_fs_uae_amiga_configs[];
extern int g_fs_uae_amiga_config;
extern int g_fs_uae_amiga_model;
extern int g_fs_uae_ntsc_mode;

#endif /* FS_UAE_CONFIG_COMMON_H */
