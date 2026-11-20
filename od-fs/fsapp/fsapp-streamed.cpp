/*
 * Streamed content on the drawing side (see fsapp-streamed.h, and the client's calls in
 * fsapp-surface.h).
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-streamed.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <unordered_map>

#include "fsapp-frame-plan.h"
#include "fsapp-gl.h"
#include "fsapp-mainthread.h"

namespace
{

struct streamed {
    // The two textures (0 until the first frame is begun): textures[front] is drawn, the other is
    // the back texture, which the slices go into. Each has its own size (0 x 0 until it is first
    // the back texture), the largest upload region it has been given, rounded up by the frame
    // plan's size rule (fsapp_frame_plan_grow_texture).
    GLuint textures[2];
    fsapp_frame_plan_size_t sizes[2];
    int front;
    // Whether a frame has been committed, its source and picture rectangles, and the size of its
    // upload region as uploaded (cut to the texture's largest size).
    bool has_content;
    SDL_Rect source;
    SDL_Rect picture;
    fsapp_frame_plan_size_t region;
    fsapp_surface_stream_filter_t filter;
    // The frame begun and not committed yet, if any, and its upload region's size.
    bool begun;
    int region_width;
    int region_height;
};

// How the pixels of a format are uploaded: the texture's internal format, and the format and type
// of the pixels given to glTexImage2D and glTexSubImage2D.
struct upload_format {
    GLint internal_format;
    GLenum format;
    GLenum type;
};

} // namespace

// By the surface's id (the surface).
static std::unordered_map<const void *, streamed> g_streamed;

static streamed &get_streamed(fsapp_surface_t *surface)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    SDL_assert(surface->streamed);
    return g_streamed[surface];
}

static int back_index(const streamed &content)
{
    return 1 - content.front;
}

// The largest texture the GPU takes (GL_MAX_TEXTURE_SIZE), asked once.
static int get_max_texture_size(void)
{
    static GLint max_size = 0;
    if (max_size == 0) {
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
        // At least 64, which OpenGL 2.1 guarantees, should the query fail.
        max_size = SDL_max(max_size, 64);
        SDL_Log("Streamed content: textures of at most %d x %d", max_size, max_size);
    }
    return max_size;
}

int fsapp_surface_get_stream_pixel_size(fsapp_surface_stream_format_t format)
{
    return format == FSAPP_SURFACE_STREAM_RGB565 ? 2 : 4;
}

// The internal formats are unsized (GL_RGBA, GL_RGB): never GL_RGBA8, with which Mesa no longer
// matches the GL_BGRA uploads and converts every upload (on the CPU in llvmpipe); and macOS's GL
// has no GL_RGB565.
static upload_format get_upload_format(fsapp_surface_stream_format_t format)
{
    if (format == FSAPP_SURFACE_STREAM_RGB565) {
        return {GL_RGB, GL_RGB, GL_UNSIGNED_SHORT_5_6_5};
    }
    // GL_UNSIGNED_INT_8_8_8_8_REV: the same bytes as GL_UNSIGNED_BYTE on little-endian hosts, and
    // Apple's documented fast path.
    return {GL_RGBA, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV};
}

// Creates the texture (glGenTextures when it is 0) again with no data at the size, in the format.
static void create_texture(GLuint *texture, fsapp_frame_plan_size_t size,
                           fsapp_surface_stream_format_t format)
{
    upload_format upload = get_upload_format(format);
    if (*texture == 0) {
        glGenTextures(1, texture);
    }
    fsapp_gl.ActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, upload.internal_format, size.width, size.height, 0,
                 upload.format, upload.type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // The filter is set when the content is drawn (the content's filter).
}

void fsapp_surface_begin_streamed_frame(fsapp_surface_t *surface, int width, int height)
{
    streamed &content = get_streamed(surface);
    // The back texture is created again, with no data, only when a larger region comes. What lies
    // in it outside the upload region is never sampled (the streamed-content program's clamp).
    int back = back_index(content);
    fsapp_frame_plan_size_t region = {SDL_max(width, 0), SDL_max(height, 0)};
    int max_size = get_max_texture_size();
    if (fsapp_frame_plan_grow_texture(&content.sizes[back], &region, max_size)) {
        create_texture(&content.textures[back], content.sizes[back], surface->stream_format);
    }
    if (region.width < width || region.height < height) {
        // REVIEW: What to do with a region larger than the GPU allows is open; for now the part
        // that fits is shown (fsapp_surface_begin_streamed_frame).
        static bool warned = false;
        if (!warned) {
            warned = true;
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "Streamed content: an upload region of %d x %d is cut to %d x %d, the "
                        "largest texture the GPU takes (logged once)",
                        width, height, region.width, region.height);
        }
    }
    // A frame half uploaded is abandoned: its slices are overwritten, and it is never committed.
    content.begun = true;
    content.region_width = region.width;
    content.region_height = region.height;
}

void fsapp_surface_upload_streamed_slice(fsapp_surface_t *surface, int y, int height,
                                         const void *pixels, int stride)
{
    streamed &content = get_streamed(surface);
    SDL_assert(content.begun);
    upload_format upload = get_upload_format(surface->stream_format);
    int pixel_size = fsapp_surface_get_stream_pixel_size(surface->stream_format);
    SDL_assert(stride % pixel_size == 0);
    // The rows in the upload region (and the texture).
    int first = SDL_max(y, 0);
    int end = SDL_min(y + height, content.region_height);
    if (!content.begun || first >= end || content.region_width <= 0) {
        return;
    }
    const uint8_t *rows = (const uint8_t *)pixels + (size_t)(first - y) * stride;
    // No state is kept from an earlier call (see fsapp-streamed.h): the back texture on fsapp's
    // texture unit, and the whole unpack state.
    fsapp_gl.ActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, content.textures[back_index(content)]);
    // Rows stride bytes apart: the row length in pixels, and an alignment that adds nothing to it.
    glPixelStorei(GL_UNPACK_ALIGNMENT, pixel_size);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, stride / pixel_size);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, first, content.region_width, end - first, upload.format,
                    upload.type, rows);
}

void fsapp_surface_commit_streamed_frame(fsapp_surface_t *surface, const SDL_Rect *source,
                                         const SDL_Rect *picture)
{
    streamed &content = get_streamed(surface);
    SDL_assert(content.begun);
    if (!content.begun) {
        return;
    }
    content.begun = false;
    content.front = 1 - content.front;
    content.has_content = true;
    content.source = *source;
    content.picture = *picture;
    content.region = {content.region_width, content.region_height};
}

void fsapp_surface_set_streamed_filter(fsapp_surface_t *surface,
                                       fsapp_surface_stream_filter_t filter)
{
    get_streamed(surface).filter = filter;
}

void fsapp_surface_set_streamed_picture(fsapp_surface_t *surface, const SDL_Rect *picture)
{
    get_streamed(surface).picture = *picture;
}

void fsapp_surface_drop_streamed_content(fsapp_surface_t *surface)
{
    // The textures are kept for the next commit.
    get_streamed(surface).has_content = false;
}

bool fsapp_surface_get_frame_rectangle(fsapp_surface_t *surface, SDL_Rect *rect)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    const fsapp_snapshot_layout_t *layout = &fsapp_mainthread_get_drawing_snapshot()->layout;
    for (int i = 0; i < layout->surface_count; i++) {
        const fsapp_snapshot_surface_t *item = &layout->surfaces[i];
        if (item->id == surface) {
            *rect = {item->x, item->y, item->width, item->height};
            return true;
        }
    }
    return false;
}

void fsapp_streamed_get_content(const void *id, fsapp_streamed_content_t *content)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    *content = {};
    auto found = g_streamed.find(id);
    if (found == g_streamed.end() || !found->second.has_content) {
        return;
    }
    const streamed &streamed = found->second;
    content->has_content = true;
    content->texture = streamed.textures[streamed.front];
    content->texture_width = streamed.sizes[streamed.front].width;
    content->texture_height = streamed.sizes[streamed.front].height;
    content->source = streamed.source;
    content->picture = streamed.picture;
    content->region_width = streamed.region.width;
    content->region_height = streamed.region.height;
    content->filter = streamed.filter;
}

void fsapp_streamed_destroy(const void *id)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    auto found = g_streamed.find(id);
    if (found == g_streamed.end()) {
        return;
    }
    // glDeleteTextures ignores 0 (a texture never created).
    glDeleteTextures(2, found->second.textures);
    g_streamed.erase(found);
}
