#include "fsuae-renderer.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glib.h>

#include "fsapp-surface.h"
#include "fsgui-window.h"
#include "fsgui-windows.h"

// FIXME: For fsemu_opengl_texture_2d, etc (local state changes)
#include "fsemu-opengl.h"

#define FSEMU_INTERNAL 1
#include "fsemu-glvideo.h"
#include "fsemu-video.h"

// FIXME
// extern fsgui_window_t* g_fsgui_window;
extern SDL_Window* g_window;
extern int g_window_width;
extern int g_window_height;
static GList* g_surfaces_to_render;

static SDL_Rect g_uae_rect;

// -------------------------------------------------------------------------------------------------

void fsuae_renderer_clear_window(void) {
    glViewport(0, 0, g_window_width, g_window_height);
    // glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    // glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // fsgui_window_t* window = g_fsgui_window;
    fsgui_window_t* window = g_fsgui_windows_list->window;
    glClearColor(1.0f * window->background_color.r / 255, 1.0f * window->background_color.g / 255,
                 1.0f * window->background_color.b / 255, 1.0f * window->background_color.a / 255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// -------------------------------------------------------------------------------------------------

static void free_deleted_surface_textures(void) {
    // Python UI thread is guaranteed to not be running when this function is called

    GList* surfaces = fsapp_surface_get_opengl_free_list();
    for (GList* item = surfaces; item; item = item->next) {
        fsapp_surface_t* surface = (fsapp_surface_t*)item->data;
        SDL_assert(surface->deleted);

        SDL_Log("Found deleted surface, deleting related texture(s)");
        if (surface->render_texture) {
            SDL_Log("Deleting OpenGL texture %d", surface->render_texture);
            glDeleteTextures(1, &surface->render_texture);
        }

        // Finally unref surface to free struct itself
        fsapp_surface_unref(surface);
    }
    // This list must be freed by the caller
    g_list_free(surfaces);
}

static void create_and_update_textures(void) {
    // Python UI thread is guaranteed to not be running when this function is called

    g_list_free(g_surfaces_to_render);
    g_surfaces_to_render = NULL;

    // struct fsapp_surface_pos render;

    // FIXME: Only update changed textures
    // FIXME: Only update dirty region(s)

    // FIXME: If a surface is deleted, let the python thread (?) just mark it for deletion
    // and then this thread/function can actually free the OpenGL texture *and* remove it
    // from the surfaces list (?) (the previous deletion could already have freed surface memory
    // itself)

    // Important that this function is the only one that destroys surfaces...!
    // (if/when) the have associated opengl textures, at least

    // -and also because the structs
    // are reused for rendering lists. hmm, maybe better top copy the structs..

    // GList* item = surfaces;
    // while (item) {
    GList* surfaces = fsapp_surface_get_list();
    for (GList* item = surfaces; item; item = item->next) {
        fsapp_surface_t* surface = (fsapp_surface_t*)item->data;
        // printf("surface %dx%d\n", surface->width, surface->height);
        SDL_assert(!surface->deleted);

        // if (surface->deleted) {
        //     // FIXME: Free opengl texture and surface
        //     SDL_Log("Found deleted surface, deleting related texture(s)");
        //     glDeleteTextures(1, &surface->render_texture);
        //     // Finally unref surface to free struct itself
        //     fsapp_surface_unref(surface);
        //     // Delete link from list

        //     item = item->next;
        // }

        if (surface->special) {
            // FS-UAE emulator output - this is a bit hackish way to coordinate the threads,
            // but somewhat practical.
            if (surface->special == 0x554145) {
                // fsemu_video_render(g_window_width, g_window_height, surface->x, surface->y,
                //                    surface->width, surface->height);
                g_uae_rect.x = surface->x;
                g_uae_rect.y = surface->y;
                g_uae_rect.w = surface->width;
                g_uae_rect.h = surface->height;
            }
            // Skip special surfaces
            continue;
        }

        // Verify that we don't have any surfaces with negative size
        SDL_assert(surface->width >= 0 && surface->height >= 0);
        // Skip invisible and non-zero surfaces (and also negative sizes to be sure)
        if (!surface->visible || surface->height <= 0 || surface->width <= 0) {
            continue;
        }

        // printf("%d\n", i);
        fsemu_opengl_texture_2d(true);
        if (surface->render_texture == 0) {
            SDL_Log("Create texture %dx%d for surface\n", surface->width, surface->height);
            glGenTextures(1, &surface->render_texture);
        }
        glBindTexture(GL_TEXTURE_2D, surface->render_texture);
        fsemu_opengl_unpack_row_length(0);

        // FIXME: Support updating only damaged region(s)
        // FIXME: Only re-upload texture if changed

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->width, surface->height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, surface->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // We make of copy of the surface here, so information about size and position cannot
        // be modified by the python thread. Alternatively, copy parts of the struct.. hmm,
        // yes,
        surface->render.x = surface->x;
        surface->render.y = surface->y;
        surface->render.width = surface->width;
        surface->render.height = surface->height;
        surface->render.z = surface->z;

        g_surfaces_to_render = g_list_append(g_surfaces_to_render, surface);

        // item = item->next;
    }

    // FIXME: Now, copy textures to
}

void fsuae_renderer_update_textures(void) {
    // Python UI thread is guaranteed to not be running when this function is called
    free_deleted_surface_textures();
    create_and_update_textures();
}

// -------------------------------------------------------------------------------------------------

static gint sort_surface_by_render_order(gconstpointer a, gconstpointer b) {
    const fsapp_surface_t* surface_a = (const fsapp_surface_t*)a;
    const fsapp_surface_t* surface_b = (const fsapp_surface_t*)b;
    return surface_b->render.z - surface_a->render.z;

    // printf("sort %f vs %f\n", surface_a->render.z, surface_b->render.z);
    // return surface_a->render.z - surface_b->render.z;

    // const fsapp_surface_t* z_a = ((const fsapp_surface_t*)a)->render.z;
    // const fsapp_surface_t* z_b = ((const fsapp_surface_t*)b)->render.z;
    // // printf("sort %f vs %f\n", surface_a->render.z, surface_b->render.z);
    // return (z_a > z_b) - (z_a < z_b);
}

static float convert_z_index_to_z_coordinate(int z_index) {
    // We want z values between -1 and 1 - also flip the z coordinate axis
    return -0.001f * z_index;
}

void fsuae_renderer_render_textures() {
    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);

    // printf("----------------\n");

    g_surfaces_to_render = g_list_sort(g_surfaces_to_render, sort_surface_by_render_order);
    for (GList* item = g_surfaces_to_render; item; item = item->next) {
        fsapp_surface_t* surface = (fsapp_surface_t*)item->data;

        float z = convert_z_index_to_z_coordinate(surface->render.z);

        // printf("render surface %dx%d %d -> %f\n", surface->render.width, surface->render.height,
        //       surface->render.z, z);

        fsemu_opengl_texture_2d(true);
        float tx1 = 0.0, ty1 = 1.0, tx2 = 1.0, ty2 = 0.0;
        SDL_FRect dr = {
            .x = (float)(-1.0 + 2.0 * surface->render.x / g_window_width),
            .y = (float)(-1.0 + 2.0 *
                                    (g_window_height - surface->render.height - surface->render.y) /
                                    g_window_height),
            .w = (float)(2.0 * surface->render.width / g_window_width),
            .h = (float)(2.0 * surface->render.height / g_window_height),
        };
        glBindTexture(GL_TEXTURE_2D, surface->render_texture);
        glBegin(GL_QUADS);
        glTexCoord2f(tx1, ty1);
        glVertex3f(dr.x, dr.y, z);
        glTexCoord2f(tx2, ty1);
        glVertex3f(dr.x + dr.w, dr.y, z);
        glTexCoord2f(tx2, ty2);
        glVertex3f(dr.x + dr.w, dr.y + dr.h, z);
        glTexCoord2f(tx1, ty2);
        glVertex3f(dr.x, dr.y + dr.h, z);
        glEnd();
    }

    // Do not clear the g_surfaces_to_render list. If the user interface thread is delayed, we
    // might want to re-render the same surfaces on the frame.

    // // The surfaces themselves are not be deleted here
    // g_list_free(g_surfaces_to_render);
    // g_surfaces_to_render = NULL;
}

// -------------------------------------------------------------------------------------------------

void fsuae_renderer_render_video(void) {
    fsemu_video_render(g_window_width, g_window_height, g_uae_rect.x, g_uae_rect.y, g_uae_rect.w,
                       g_uae_rect.h);
}
