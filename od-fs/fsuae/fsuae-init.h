#ifndef FSUAE_INIT_H
#define FSUAE_INIT_H

#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

bool fsuae_init(int argc, char *argv[], int *error);

// FS-UAE's data and cache directories: Data and Cache in the portable directory (the closest one
// with a Portable.ini), or else in SDL's pref path for FS-UAE. NULL before fsuae_init.
const char *fsuae_init_get_data_dir(void);
const char *fsuae_init_get_cache_dir(void);

#ifdef __cplusplus
}
#endif

#endif /* FSUAE_INIT_H */
