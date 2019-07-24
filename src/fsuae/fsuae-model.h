#ifndef FS_UAE_CONFIG_MODEL_H
#define FS_UAE_CONFIG_MODEL_H

enum {
    CONFIG_A500,
    CONFIG_A500_512K,
    CONFIG_A500P,
    CONFIG_A600,
    CONFIG_CD32,
    CONFIG_CDTV,
    CONFIG_A1200,
    CONFIG_A1200_3_0,
    CONFIG_A1200_020,
    CONFIG_A1200_1230,
    CONFIG_A1200_1240,
    CONFIG_A1200_1260,
    CONFIG_A1200_PPC,
    CONFIG_SUPER,
    CONFIG_A4000,
    CONFIG_A4000_040,
    CONFIG_A1000,
    CONFIG_A3000,
    CONFIG_CD32_FMV,
    CONFIG_A4000_PPC,
    CONFIG_A4000_OS4,
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
    int can_use_ecs_agnus;
    int quickstart_model;
    int quickstart_config;
    int allow_z3_memory;
    //int z3mem_size;
    //const char *cpu_model;
    int cpu_32bit_addressing;
    int fast;
    int no_accuracy_adjustment;
    const char *warning;
    int enhanced_audio_filter;
    const char *wb_disk;
    const char *default_hd_controller;
    const char *default_cd_controller;
    const char *default_graphics_card;
    int cpu_idle;
    int z3realmapping;
    const char *accelerator;
    int default_floppy_drive_type;
    int default_floppy_drive_count;
    const char *default_cpu;
    const char *default_fpu;
    const char *default_fpu_noninternal;
    const char *default_mmu;

    /* This will (possibly) be filled by configure_accelerator. */
    const char *accelerator_cpu;
} amiga_config;

extern amiga_config g_fs_uae_amiga_configs[];
extern int g_fs_uae_amiga_config;
extern int g_fs_uae_amiga_model;

extern amiga_config *g_fs_uae_config;
#define cfg g_fs_uae_config

void fs_uae_init_configs(void);

#define NEW_ACCURACY_SYSTEM

#endif /* FS_UAE_CONFIG_MODEL_H */
