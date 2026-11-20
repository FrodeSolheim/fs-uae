/*
 * fsapp's OpenGL: the context it asks for, what it logs about the context it got, and the loader of
 * the GL functions it uses beyond GL 1.1.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-gl.h"

#include "fsapp-mainthread.h"

fsapp_gl_functions_t fsapp_gl;

// The context is the same kind everywhere: the legacy 2.1 profile on macOS, the compatibility
// profiles of Mesa and the vendors elsewhere, at most 3.1 on the Raspberry Pi. The depth buffer is
// for the compositor's depth order (16 bits is SDL's default, and the depth values used are exact
// in 16 bits; more is fine). The back buffer's alpha is left at SDL's default of 8 bits.
void fsapp_gl_set_context_attributes(void)
{
    SDL_assert(SDL_IsMainThread());
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_Log("OpenGL context requested: version 2.1, no profile, double buffered, depth size 16");
}

static void log_attribute(const char *name, SDL_GLAttr attribute)
{
    int value = 0;
    if (SDL_GL_GetAttribute(attribute, &value)) {
        SDL_Log("OpenGL %s: %d", name, value);
    } else {
        SDL_Log("OpenGL %s: unknown (%s)", name, SDL_GetError());
    }
}

void fsapp_gl_log_information(void)
{
    const char *str;
    str = (const char *)glGetString(GL_VENDOR);
    if (str) {
        SDL_Log("OpenGL vendor: %s\n", str);
    }
    str = (const char *)glGetString(GL_RENDERER);
    if (str) {
        SDL_Log("OpenGL renderer: %s\n", str);
    }
    str = (const char *)glGetString(GL_VERSION);
    if (str) {
        SDL_Log("OpenGL version: %s\n", str);
    }
    str = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (str) {
        SDL_Log("OpenGL shading language version: %s\n", str);
    }
    str = (const char *)glGetString(GL_EXTENSIONS);
    if (str) {
        SDL_Log("OpenGL extensions: %s\n", str);
    }
    int max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    SDL_Log("OpenGL reported max texture size: %dx%d\n", max_texture_size, max_texture_size);
    // What was obtained, which can differ from what was asked for (more depth bits are fine).
    log_attribute("depth size", SDL_GL_DEPTH_SIZE);
    log_attribute("alpha size", SDL_GL_ALPHA_SIZE);
}

// The version fsapp draws with.
#define NEEDED_MAJOR_VERSION 2
#define NEEDED_MINOR_VERSION 1

char *fsapp_gl_check_version(void)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    const char *version = (const char *)glGetString(GL_VERSION);
    int major = 0;
    int minor = 0;
    // "<major>.<minor>[.<release>][ <vendor's information>]". An OpenGL ES context (which fsapp
    // does not ask for) has "OpenGL ES " in front, and is refused as one of unknown version.
    if (version == NULL || SDL_sscanf(version, "%d.%d", &major, &minor) != 2) {
        major = 0;
        minor = 0;
    }
    if (major > NEEDED_MAJOR_VERSION ||
        (major == NEEDED_MAJOR_VERSION && minor >= NEEDED_MINOR_VERSION)) {
        return NULL;
    }
    char *message = NULL;
    if (SDL_asprintf(&message, "OpenGL %d.%d is needed, but the OpenGL version is %s",
                     NEEDED_MAJOR_VERSION, NEEDED_MINOR_VERSION,
                     version ? version : "unknown") < 0) {
        return SDL_strdup("OpenGL 2.1 is needed");
    }
    return message;
}

// REVIEW: With Mesa (GLX or EGL), SDL_GL_GetProcAddress returns a dispatch stub for any name that
// starts with gl, also one the driver does not have, so a missing function is only found where the
// platform returns NULL for it: WGL on Windows (where an old driver, or Microsoft's GDI renderer
// with GL 1.1, lacks them) and macOS. Elsewhere the version check (fsapp_gl_check_version) is the
// guard.
const char *fsapp_gl_load_functions(void)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    fsapp_gl_functions_t functions;
#define FSAPP_GL_LOAD_FUNCTION(type, name)                                                         \
    functions.name = (type)SDL_GL_GetProcAddress("gl" #name);                                      \
    if (functions.name == NULL) {                                                                  \
        return "gl" #name;                                                                         \
    }
    FSAPP_GL_FUNCTIONS(FSAPP_GL_LOAD_FUNCTION)
#undef FSAPP_GL_LOAD_FUNCTION
    fsapp_gl = functions;
    SDL_Log("OpenGL functions loaded (%d)", (int)(sizeof(fsapp_gl) / sizeof(void *)));
    return NULL;
}
