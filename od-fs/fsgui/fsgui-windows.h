#ifndef FSGUI_WINDOWS_H
#define FSGUI_WINDOWS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fsgui-window.h"

struct fsgui_windows_list {
    fsgui_window_t* window;
    struct fsgui_windows_list* next;
};

typedef struct fsgui_windows_list fsgui_windows_list_t;

extern fsgui_windows_list_t* g_fsgui_windows_list;

#ifdef __cplusplus
}
#endif

#endif  // FSGUI_WINDOWS_H
