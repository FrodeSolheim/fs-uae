#ifndef LIBAMIGA_INTERNAL_H_
#define LIBAMIGA_INTERNAL_H_

#include "uae/uae.h"

typedef struct _libamiga_callbacks {
    display_function display;
    event_function event;
    render_function render;
    init_function init;
    log_function log;
} libamiga_callbacks;

extern log_function g_amiga_gui_message_function;

extern libamiga_callbacks g_libamiga_callbacks;
extern char *g_libamiga_save_image_path;
extern int g_amiga_paused;

int amiga_handle_input_event (int nr, int state, int max,
        int autofire, bool canstopplayback, bool playbackevent);
const char *get_libamiga_base_version();

void amiga_configure_port_from_input_event(int input_event);

extern int g_amiga_video_format;
extern int g_amiga_rtg_modes[];

#endif // LIBAMIGA_INTERNAL_H_
