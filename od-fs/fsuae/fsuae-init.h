#ifndef FSUAE_INIT_H
#define FSUAE_INIT_H

#include <stdbool.h>
#include <stdio.h>

#include "fsuae-config.h"

#ifdef __cplusplus
extern "C" {
#endif

// extern FILE* g_fsuae_init_log_file;

bool fsuae_init(int argc, char* argv[], int* error);

void fsuae_init_close_log_file(void);

#ifdef __cplusplus
}
#endif

#endif  // FSUAE_INIT_H
