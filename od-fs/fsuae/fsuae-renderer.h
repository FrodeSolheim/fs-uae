#ifndef FSUAE_RENDERER_H
#define FSUAE_RENDERER_H

#include "fsuae-config.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsuae_renderer_clear_window(void);
// void fsuae_render_free_deleted_surface_textures(void);
// void fsuae_render_create_and_update_textures(void);
void fsuae_renderer_update_textures(void);
void fsuae_renderer_render_textures();
void fsuae_renderer_render_video(void);

#ifdef __cplusplus
}
#endif

#endif  // FSUAE_RENDERER_H
