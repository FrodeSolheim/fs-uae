#ifndef FSEMU_OPENGL_H_
#define FSEMU_OPENGL_H_

#include "fsemu-config.h"
#include "fsemu-log.h"

#ifdef FSEMU_OPENGL

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_opengl_init(void);

void fsemu_opengl_forget_state(void);

void fsemu_opengl_blend(bool blend);
void fsemu_opengl_color3f(float r, float g, float b);
void fsemu_opengl_color4f(float r, float g, float b, float a);
void fsemu_opengl_depth_test(bool depth_test);
void fsemu_opengl_texture_2d(bool texture_2d);

void fsemu_opengl_assume_blend(bool blend);
void fsemu_opengl_assume_texture_2d(bool texture_2d);

void fsemu_opengl_forget_blend(void);
void fsemu_opengl_forget_texture_2d(void);
void fsemu_opengl_forget_unpack_row_length(void);

void fsemu_opengl_unpack_row_length(int row_length);

void fsemu_opengl_log_error(void);

extern bool fsemu_opengl_error_checking;

static inline void fsemu_opengl_log_error_maybe(void)
{
    if (fsemu_opengl_error_checking) {
        fsemu_opengl_log_error();
    }
}

#ifdef FSEMU_INTERNAL

#define fsemu_opengl_log(format, ...) \
    fsemu_log("[FSE] [OGL] " format, ##__VA_ARGS__)

#endif

#ifdef __cplusplus
}
#endif

#ifdef FSEMU_INTERNAL
// #define GLEW_NO_GLU
// #include <GL/glew.h>
#include "../../glad/include/glad/glad.h"
#endif

#endif  // FSEMU_OPENGL

#endif  // FSEMU_OPENGL_H_
