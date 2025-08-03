#ifndef FSUAE_PATH_H
#define FSUAE_PATH_H

#include "fsuae-config.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsuae_path_init(void);

const char* fsuae_path_appdata_dir(void);

const char* fsuae_path_base_dir(void);
// const char* fsuae_path_data_dir(void);
// const char* fsuae_path_plugins_dir(void);
const char* fsuae_path_python_dir(void);
const char* fsuae_path_system_dir(void);

#ifdef __cplusplus
}
#endif

#endif // FSUAE_PATH_H
