#define FSEMU_INTERNAL
#include "fsemu-opengl.h"

struct {
    bool blend;
    bool depth_test;
    struct {
        float r;
        float g;
        float b;
        float a;
    } color4f;
    bool texture_2d;
} fsemu_opengl;

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
    static bool blend_func_set;
    if (fsemu_opengl.blend != blend) {
        fsemu_opengl_enable(GL_BLEND, blend);
        fsemu_opengl.blend = blend;
        if (!blend_func_set) {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            blend_func_set = true;
        }
    }
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
        fsemu_opengl_enable(GL_BLEND, depth_test);
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
