#define FSEMU_INTERNAL
#include "fsemu-opengl.h"

#include "fsemu-log.h"
#include "fsemu-thread.h"

struct {
    int blend;
    // bool blend_func_set;
    struct {
        float r;
        float g;
        float b;
        float a;
    } color4f;
    bool depth_test;
    int texture_2d;
    int unpack_row_length;
} fsemu_opengl;

bool fsemu_opengl_error_checking = false;

void fsemu_opengl_unpack_row_length(int row_length)
{
    if (fsemu_opengl.unpack_row_length != row_length) {
        glPixelStorei(GL_UNPACK_ROW_LENGTH, row_length);
        fsemu_opengl.unpack_row_length = row_length;
    }
}

void fsemu_opengl_log_error(void)
{
    int error = glGetError();
    if (error) {
        fsemu_log_error("glGetError() = %d\n\n", error);
    }
}

static inline void fsemu_opengl_enable(int what, bool enable)
{
    if (enable) {
        glEnable(what);
    } else {
        glDisable(what);
    }
}

void fsemu_opengl_blend(bool blend)
{
    fsemu_thread_assert_video();
    // fsemu_opengl_enable(GL_BLEND, blend);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    static bool blend_func_set;
    // fsemu_opengl.blend = -1;
    if (fsemu_opengl.blend != blend) {
        // printf("fsemu_opengl_blend %d\n", blend);
        fsemu_opengl_enable(GL_BLEND, blend);
        fsemu_opengl.blend = blend;
        // FIXME...
        if (!blend_func_set || true) {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            blend_func_set = true;
        }
    } else {
        // printf("fsemu_opengl_blend %d (ignore)\n", blend);
    }
}

void fsemu_opengl_assume_blend(bool blend)
{
    fsemu_opengl.blend = blend;
}

void fsemu_opengl_assume_texture_2d(bool texture_2d)
{
    fsemu_opengl.texture_2d = texture_2d;
}

void fsemu_opengl_forget_blend(void)
{
    fsemu_opengl.blend = -1;
}

void fsemu_opengl_forget_texture_2d(void)
{
    fsemu_opengl.texture_2d = -1;
}

void fsemu_opengl_color3f(float r, float g, float b)
{
    fsemu_opengl_color4f(r, g, b, 1.0);
}

void fsemu_opengl_color4f(float r, float g, float b, float a)
{
    if (fsemu_opengl.color4f.r != r || fsemu_opengl.color4f.g != g ||
        fsemu_opengl.color4f.b != b || fsemu_opengl.color4f.a != a) {
        glColor4f(r, g, b, a);
        fsemu_opengl.color4f.r = r;
        fsemu_opengl.color4f.g = g;
        fsemu_opengl.color4f.b = b;
        fsemu_opengl.color4f.a = a;
    }
}

void fsemu_opengl_depth_test(bool depth_test)
{
    if (fsemu_opengl.depth_test != depth_test) {
        fsemu_opengl_enable(GL_DEPTH_TEST, depth_test);
        fsemu_opengl.depth_test = depth_test;
    }
}

void fsemu_opengl_texture_2d(bool texture_2d)
{
    if (fsemu_opengl.texture_2d != texture_2d) {
        fsemu_opengl_enable(GL_TEXTURE_2D, texture_2d);
        fsemu_opengl.texture_2d = texture_2d;
    }
}

void fsemu_opengl_forget_unpack_row_length(void)
{
    fsemu_opengl.unpack_row_length = -1;
}

void fsemu_opengl_forget_state(void)
{
    fsemu_opengl.blend = -1;
    fsemu_opengl.color4f.r = -1;
    fsemu_opengl.color4f.g = -1;
    fsemu_opengl.color4f.b = -1;
    fsemu_opengl.color4f.a = -1;
    fsemu_opengl.depth_test = -1;
    fsemu_opengl.texture_2d = -1;
    fsemu_opengl.unpack_row_length = -1;
}

void fsemu_opengl_init(void)
{
    // FIXME: Not called yet
    fsemu_return_if_already_initialized();
    fsemu_opengl_log("Init\n");

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0, 0.0, 0.0, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    fsemu_opengl.blend = -1;
    fsemu_opengl.texture_2d = -1;
}
