#ifndef FSEMU_OPENGL_H_
#define FSEMU_OPENGL_H_

#include "fsemu-config.h"
#include "fsemu-log.h"

#ifdef FSEMU_OPENGL

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_opengl_init(void);

void fsemu_opengl_blend(bool blend);
void fsemu_opengl_color4f(float r, float g, float b, float a);
void fsemu_opengl_depth_test(bool depth_test);
void fsemu_opengl_texture_2d(bool texture_2d);

void fsemu_opengl_assume_blend(bool blend);
void fsemu_opengl_assume_texture_2d(bool texture_2d);

void fsemu_opengl_forget_blend(void);
void fsemu_opengl_forget_texture_2d(void);

#ifdef __cplusplus
}
#endif

#ifdef FSEMU_INTERNAL
#include <GL/glew.h>
#endif

#endif  // FSEMU_OPENGL

#define fsemu_opengl_log(format, ...) \
    fsemu_log("[FSEMU] [OPNGL] " format, ##__VA_ARGS__)

#endif  // FSEMU_OPENGL_H_
