/*
 * The compositor: draws the surfaces, the emulation surface and the adornments into the host window
 * with OpenGL, from the frame plan (the steps and the GL state are described in
 * fsapp-compositor.h).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-compositor.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <unordered_map>
#include <vector>

#include "fsapp-frame-plan.h"
#include "fsapp-gl.h"
#include "fsapp-gldraw.h"
#include "fsapp-mainthread.h"
#include "fsapp-streamed.h"

// The drawing side's textures, one for each surface with a buffer that has been uploaded, by the
// surface's id in the layout.
static std::unordered_map<const void *, GLuint> g_textures;

// The drawing side's texture for an adornment with content, and the size of the content in it.
struct uploaded_adornment {
    GLuint texture;
    int width;
    int height;
};

// The drawing side's textures for the adornments with content, by the adornment's id.
static std::unordered_map<const void *, uploaded_adornment> g_adornment_textures;

// The frame plan, and its input with what each entry of the input stands for, for the frame being
// drawn: worked out in drawing below and used until drawing above. The vectors keep their storage
// from frame to frame.
static struct {
    fsapp_frame_plan_t *plan;
    std::vector<fsapp_frame_plan_surface_t> surfaces;
    std::vector<fsapp_frame_plan_adornment_t> adornments;
    // Parallel to surfaces: the snapshot's surface, and its texture (0 for a surface with streamed
    // content).
    std::vector<const fsapp_snapshot_surface_t *> snapshot_surfaces;
    std::vector<GLuint> surface_textures;
    // Parallel to adornments: the snapshot's adornment and its texture.
    std::vector<const fsapp_snapshot_adornment_t *> snapshot_adornments;
    std::vector<GLuint> adornment_textures;
    fsapp_frame_plan_steps_t steps;
    // Whether step 1 left the emulation surface's picture unfilled (its depth-only entry), and
    // where: the picture rectangle before the wait, clipped (window coordinates).
    bool picture_unfilled;
    fsapp_frame_plan_rectangle_t unfilled_picture;
    bool dropped_logged;
} g_frame;

// -------------------------------------------------------------------------------------------------

// The viewport covers the whole window.
// REVIEW: The viewport was the size in window coordinates (g_window_width/height) before the
// snapshot; it is now the size in pixels, which covers the whole drawable. The two are the same
// unless the window has high pixel density, which it never asks for (no
// SDL_WINDOW_HIGH_PIXEL_DENSITY), so nothing changes today.
static void set_viewport(const fsapp_snapshot_t *snapshot)
{
    glViewport(0, 0, snapshot->window.pixel_width, snapshot->window.pixel_height);
}

void fsapp_compositor_clear_window(const fsapp_snapshot_t *snapshot)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    // Errors since the previous frame's drawing above: the swap or the drawing hooks.
    fsapp_gldraw_check_errors("the swap or the drawing hooks");
    set_viewport(snapshot);
    // The clear writes every channel and the depth, in the whole window.
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    // The background colour with alpha 1, whatever its alpha: the window is always opaque.
    SDL_Color background = snapshot->window.background_color;
    glClearColor(1.0f * background.r / 255, 1.0f * background.g / 255, 1.0f * background.b / 255,
                 1.0f);
    // The far value, behind every place's depth.
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fsapp_gldraw_check_errors("the clear");
}

// -------------------------------------------------------------------------------------------------

// The unpack state for uploading content with row_length pixels per row (0: the content's width),
// from its first pixel. Set in full before each upload: the streamed content's uploads set it too
// (fsapp-streamed.cpp).
static void set_unpack_state(int row_length)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, row_length);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
}

static void delete_textures_of_destroyed_surfaces(const fsapp_snapshot_layout_t *layout)
{
    for (int i = 0; i < layout->destroyed_count; i++) {
        SDL_Log("Found destroyed surface, deleting related texture(s)");
        // The textures of its streamed content, if it has any.
        fsapp_streamed_destroy(layout->destroyed[i]);
        auto texture = g_textures.find(layout->destroyed[i]);
        if (texture == g_textures.end()) {
            continue;
        }
        SDL_Log("Deleting OpenGL texture %d", texture->second);
        glDeleteTextures(1, &texture->second);
        g_textures.erase(texture);
    }
}

// Uploads the content of the record the frame takes in, straight from the client's buffer (which is
// the drawing side's until the record is taken in): the whole buffer into a new texture or one
// whose size changes, else each damaged rectangle, read with the buffer's width as the row length
// from the rectangle's first pixel, so nothing is repacked and nothing outside the damage is read.
// REVIEW: One texture per surface, updated in place: the glFinish() before the swap (rendering
// finished) means the GPU has finished reading every texture before the next frame's UI upload
// starts. Without it (a fence, a render thread), an upload could change a texture the GPU still
// reads for the frame in flight, and this must be looked at again.
static void upload_record_content(const fsapp_snapshot_layout_t *layout)
{
    fsapp_gl.ActiveTexture(GL_TEXTURE0);
    for (int i = 0; i < layout->content_count; i++) {
        const fsapp_snapshot_content_t *content = &layout->contents[i];
        GLuint &texture = g_textures[content->id];
        if (texture == 0) {
            SDL_Log("Create texture %dx%d for surface\n", content->width, content->height);
            glGenTextures(1, &texture);
        }
        glBindTexture(GL_TEXTURE_2D, texture);
        // The buffer is premultiplied BGRA (fsapp-surface.h), uploaded as it is.
        if (content->size_changed) {
            set_unpack_state(0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, content->width, content->height, 0, GL_BGRA,
                         GL_UNSIGNED_INT_8_8_8_8_REV, content->buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        } else if (content->damage.whole) {
            set_unpack_state(0);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, content->width, content->height, GL_BGRA,
                            GL_UNSIGNED_INT_8_8_8_8_REV, content->buffer);
        } else {
            const uint32_t *pixels = (const uint32_t *)content->buffer;
            for (int j = 0; j < content->damage.count; j++) {
                const SDL_Rect *rect = &content->damage.rects[j];
                set_unpack_state(content->width);
                glTexSubImage2D(GL_TEXTURE_2D, 0, rect->x, rect->y, rect->w, rect->h, GL_BGRA,
                                GL_UNSIGNED_INT_8_8_8_8_REV,
                                pixels + (size_t)rect->y * content->width + rect->x);
            }
        }
    }
}

void fsapp_compositor_update_textures(const fsapp_snapshot_t *snapshot)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    // Deleted first: a new surface can have the id (the address) of one destroyed since the
    // previous record, and must get a texture of its own.
    delete_textures_of_destroyed_surfaces(&snapshot->layout);
    upload_record_content(&snapshot->layout);
}

void fsapp_compositor_update_adornment_textures(const fsapp_snapshot_t *snapshot)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    const fsapp_snapshot_adornments_t *adornments = &snapshot->adornments;
    // Deleted first, as for the surfaces.
    for (int i = 0; i < adornments->destroyed_count; i++) {
        auto texture = g_adornment_textures.find(adornments->destroyed[i]);
        if (texture != g_adornment_textures.end()) {
            glDeleteTextures(1, &texture->second.texture);
            g_adornment_textures.erase(texture);
        }
    }
    fsapp_gl.ActiveTexture(GL_TEXTURE0);
    for (int i = 0; i < adornments->count; i++) {
        const fsapp_snapshot_adornment_t *adornment = &adornments->adornments[i];
        if (adornment->content == NULL) {
            continue;
        }
        uploaded_adornment &texture = g_adornment_textures[adornment->id];
        bool allocate = texture.texture == 0 || texture.width != adornment->content_width ||
                        texture.height != adornment->content_height;
        if (texture.texture == 0) {
            glGenTextures(1, &texture.texture);
        }
        glBindTexture(GL_TEXTURE_2D, texture.texture);
        // The content's rows are content_stride bytes apart.
        set_unpack_state(adornment->content_stride / 4);
        if (allocate) {
            // A new texture, or content of another size: (re)allocated and filled from the whole
            // content.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, adornment->content_width,
                         adornment->content_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         adornment->content);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            texture.width = adornment->content_width;
            texture.height = adornment->content_height;
        } else if (adornment->content_damage_height > 0) {
            // Only the damaged rows, from the first damaged row's first pixel.
            const uint8_t *rows = (const uint8_t *)adornment->content +
                                  (size_t)adornment->content_damage_y * adornment->content_stride;
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, adornment->content_damage_y,
                            adornment->content_width, adornment->content_damage_height, GL_RGBA,
                            GL_UNSIGNED_BYTE, rows);
        }
    }
    fsapp_gldraw_check_errors("the UI upload");
}

// -------------------------------------------------------------------------------------------------
// The frame plan.

// The texture of an adornment that has a rectangle and content to draw, else 0.
static GLuint adornment_texture(const fsapp_snapshot_adornment_t *adornment)
{
    if (adornment->width <= 0 || adornment->height <= 0) {
        return 0;
    }
    auto texture = g_adornment_textures.find(adornment->id);
    if (texture == g_adornment_textures.end()) {
        // No content committed yet.
        return 0;
    }
    return texture->second.texture;
}

// The picture rectangle of streamed content in window coordinates (the surface at its place), and
// the source rectangle shown in it, both cut to the part of the source that was uploaded. When the
// upload region was larger than the GPU's largest texture, only its top left part is in the
// texture: the picture keeps the scale of the whole source and shows that part, and the bars cover
// the rest of it. Returns false when nothing of the source was uploaded (or the picture is empty).
static bool get_streamed_picture(const fsapp_snapshot_surface_t *surface,
                                 const fsapp_streamed_content_t *content,
                                 fsapp_frame_plan_rectangle_t *picture,
                                 fsapp_frame_plan_source_t *source)
{
    const SDL_Rect &given = content->source;
    fsapp_frame_plan_rectangle_t whole = {surface->x + content->picture.x,
                                          surface->y + content->picture.y, content->picture.w,
                                          content->picture.h};
    fsapp_frame_plan_source_t whole_source = {(double)given.x, (double)given.y, (double)given.w,
                                              (double)given.h};
    if (given.x >= 0 && given.y >= 0 && given.x + given.w <= content->region_width &&
        given.y + given.h <= content->region_height) {
        *picture = whole;
        *source = whole_source;
        return true;
    }
    if (given.w <= 0 || given.h <= 0) {
        return false;
    }
    // The uploaded region in window coordinates, at the picture's scale, to whole pixels.
    double scale_x = (double)whole.width / given.w;
    double scale_y = (double)whole.height / given.h;
    int left = whole.x + (int)SDL_lround(-given.x * scale_x);
    int top = whole.y + (int)SDL_lround(-given.y * scale_y);
    int right = whole.x + (int)SDL_lround((content->region_width - given.x) * scale_x);
    int bottom = whole.y + (int)SDL_lround((content->region_height - given.y) * scale_y);
    fsapp_frame_plan_rectangle_t uploaded = {left, top, right - left, bottom - top};
    return fsapp_frame_plan_clip_picture(&uploaded, &whole, &whole_source, picture, source);
}

// Works out the frame's plan from the snapshot, into g_frame. The input is what there is something
// to draw for: the surfaces with a texture (a buffer uploaded at least once) and those with
// streamed content (the emulation surface, which always has its fill), and the adornments with
// content. Streamed content is taken as it is before the wait: whether it has content, and its
// picture rectangle (in window coordinates, cut to what was uploaded), for the bars.
static void plan_frame(const fsapp_snapshot_t *snapshot)
{
    if (g_frame.plan == NULL) {
        g_frame.plan = fsapp_frame_plan_create();
    }
    g_frame.surfaces.clear();
    g_frame.snapshot_surfaces.clear();
    g_frame.surface_textures.clear();
    const fsapp_snapshot_layout_t *layout = &snapshot->layout;
    for (int i = 0; i < layout->surface_count; i++) {
        const fsapp_snapshot_surface_t *surface = &layout->surfaces[i];
        // Only visible surfaces are in the layout.
        SDL_assert(surface->visible);
        GLuint texture = 0;
        if (!surface->streamed) {
            auto found = g_textures.find(surface->id);
            if (found == g_textures.end()) {
                continue;
            }
            texture = found->second;
        }
        fsapp_frame_plan_surface_t input = {};
        input.id = surface->id;
        input.rectangle = {surface->x, surface->y, surface->width, surface->height};
        input.translucent = surface->translucent;
        if (surface->streamed) {
            fsapp_streamed_content_t content;
            fsapp_streamed_get_content(surface->id, &content);
            fsapp_frame_plan_source_t source;
            input.streamed = true;
            input.has_content = content.has_content &&
                                get_streamed_picture(surface, &content, &input.picture, &source);
        }
        g_frame.surfaces.push_back(input);
        g_frame.snapshot_surfaces.push_back(surface);
        g_frame.surface_textures.push_back(texture);
    }

    g_frame.adornments.clear();
    g_frame.snapshot_adornments.clear();
    g_frame.adornment_textures.clear();
    const fsapp_snapshot_adornments_t *adornments = &snapshot->adornments;
    for (int i = 0; i < adornments->count; i++) {
        const fsapp_snapshot_adornment_t *adornment = &adornments->adornments[i];
        GLuint texture = adornment_texture(adornment);
        if (texture == 0) {
            continue;
        }
        fsapp_frame_plan_adornment_t input = {};
        input.id = adornment->id;
        input.surface =
            adornment->place == FSAPP_ADORNMENT_IN_FRONT_OF_SURFACE ? adornment->surface : NULL;
        input.rectangle = {adornment->x, adornment->y, adornment->width, adornment->height};
        input.translucent = !adornment->opaque;
        g_frame.adornments.push_back(input);
        g_frame.snapshot_adornments.push_back(adornment);
        g_frame.adornment_textures.push_back(texture);
    }

    fsapp_frame_plan_input_t input = {};
    input.surfaces = g_frame.surfaces.data();
    input.surface_count = (int)g_frame.surfaces.size();
    input.adornments = g_frame.adornments.data();
    input.adornment_count = (int)g_frame.adornments.size();
    input.pacer_set = snapshot->pacer.set;
    input.paced_surface = snapshot->pacer.set ? snapshot->pacer.pacer.surface : NULL;
    fsapp_frame_plan_update(g_frame.plan, &input, &g_frame.steps);
    g_frame.picture_unfilled = false;
    for (int i = 0; i < g_frame.steps.opaque_count; i++) {
        const fsapp_frame_plan_entry_t *entry = &g_frame.steps.opaque[i];
        if (entry->kind == FSAPP_FRAME_PLAN_DEPTH_ONLY) {
            g_frame.picture_unfilled = true;
            g_frame.unfilled_picture = entry->rectangle;
        }
    }
    if (g_frame.steps.dropped_count > 0 && !g_frame.dropped_logged) {
        SDL_Log("The compositor has more to draw than the depth buffer has depths for: the %d "
                "at the back are not drawn (logged once)",
                g_frame.steps.dropped_count);
        g_frame.dropped_logged = true;
    }
}

// The window depth of a place in the plan: place / 65535, the frontmost (place 0) the nearest. In a
// 16-bit depth buffer it is stored exactly as the value place, below the far value (65535) that the
// clear leaves; with more depth bits the values stay distinct and in the same order.
// REVIEW: Exactness relies on the conversion to the depth buffer's fixed point rounding to nearest
// (as GL asks for, and as Mesa does). The fill and the streamed content store the same value
// whatever the rounding, since both are drawn with glDepthRange(depth, depth). The compositor test
// (od-fs/tests/compositor) checks it on a real GPU, run as its docstring says.
static double place_depth(int place)
{
    SDL_assert(place >= 0 && place < FSAPP_FRAME_PLAN_PLACE_COUNT);
    return (double)place / FSAPP_FRAME_PLAN_PLACE_COUNT;
}

// -------------------------------------------------------------------------------------------------
// Drawing the plan's entries.

// Fills a rectangle with a surface's fill colour at a depth, with the fill program begun. Alpha 1,
// whatever the fill colour's alpha: the fill is opaque.
static void fill_opaque(const SDL_FRect *rect, double depth, SDL_Color color)
{
    fsapp_gldraw_fill_rectangle(rect, depth, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
                                1.0f);
}

// Draws the entries in their order, each at the depth of its place, with the step's state set:
// surfaces and adornments with texture_program, the fill with the fill program, and the
// depth-only entry with the fill program and colour writes off.
static void draw_entries(const fsapp_snapshot_t *snapshot, const fsapp_frame_plan_entry_t *entries,
                         int count, fsapp_gldraw_program_t texture_program)
{
    // The program begun, if any.
    bool program_begun = false;
    fsapp_gldraw_program_t begun = texture_program;
    for (int i = 0; i < count; i++) {
        const fsapp_frame_plan_entry_t *entry = &entries[i];
        bool fill =
            entry->kind == FSAPP_FRAME_PLAN_FILL || entry->kind == FSAPP_FRAME_PLAN_DEPTH_ONLY;
        fsapp_gldraw_program_t program = fill ? FSAPP_GLDRAW_PROGRAM_FILL : texture_program;
        if (!program_begun || program != begun) {
            if (program_begun) {
                fsapp_gldraw_end_program();
            }
            fsapp_gldraw_begin_program(program, snapshot->window.width, snapshot->window.height);
            program_begun = true;
            begun = program;
        }
        const fsapp_frame_plan_rectangle_t *r = &entry->rectangle;
        SDL_FRect rect = {(float)r->x, (float)r->y, (float)r->width, (float)r->height};
        double depth = place_depth(entry->place);
        switch (entry->kind) {
        case FSAPP_FRAME_PLAN_SURFACE:
            // The surface's filter (linear) was set when its buffer was uploaded.
            fsapp_gldraw_draw_texture(g_frame.surface_textures[entry->index], &rect, depth);
            break;
        case FSAPP_FRAME_PLAN_ADORNMENT: {
            const fsapp_snapshot_adornment_t *adornment = g_frame.snapshot_adornments[entry->index];
            GLuint texture = g_frame.adornment_textures[entry->index];
            // The adornment's own filter.
            glBindTexture(GL_TEXTURE_2D, texture);
            GLint filter =
                adornment->filter == FSAPP_ADORNMENT_FILTER_NEAREST ? GL_NEAREST : GL_LINEAR;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
            fsapp_gldraw_draw_texture(texture, &rect, depth);
            break;
        }
        case FSAPP_FRAME_PLAN_FILL:
            fill_opaque(&rect, depth, g_frame.snapshot_surfaces[entry->index]->fill_color);
            break;
        case FSAPP_FRAME_PLAN_DEPTH_ONLY:
            // The emulation surface's depth over its picture, which step 4 draws after the wait:
            // nothing behind it is drawn there. Only depth is written (step 1 writes depth).
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            fsapp_gldraw_fill_rectangle(&rect, depth, 0.0f, 0.0f, 0.0f, 1.0f);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            break;
        }
    }
    if (program_begun) {
        fsapp_gldraw_end_program();
    }
}

// The GL state of the steps that draw the plan's entries (1, 2 and 5), set in full where each
// begins. The viewport covers the window. The depth test is on with GL_LESS, so that what lies
// under an opaque entry is skipped and opaque entries in front hide translucent ones, and every
// channel is written (the opaque programs write alpha 1). Opaque entries are drawn without blending
// and with depth writes; translucent ones with premultiplied blending (the content's colours are
// already multiplied by its alpha) that leaves the window's alpha at 1, and without depth writes,
// so that they hide nothing.
static void set_entry_state(const fsapp_snapshot_t *snapshot, bool translucent)
{
    set_viewport(snapshot);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    if (translucent) {
        glEnable(GL_BLEND);
        fsapp_gl.BlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
        glDepthMask(GL_FALSE);
    } else {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
}

// Steps 2 and 5: the translucent entries, back to front.
static void draw_translucent(const fsapp_snapshot_t *snapshot,
                             const fsapp_frame_plan_entry_t *entries, int count)
{
    if (count == 0) {
        return;
    }
    set_entry_state(snapshot, true);
    draw_entries(snapshot, entries, count, FSAPP_GLDRAW_PROGRAM_TEXTURE_TRANSLUCENT);
}

// -------------------------------------------------------------------------------------------------
// The phases.

void fsapp_compositor_draw_below(const fsapp_snapshot_t *snapshot)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    plan_frame(snapshot);

    // Step 1, opaque, front to back. When the UI has no finished frame, this draws the same layout
    // with the same textures again.
    set_entry_state(snapshot, false);
    draw_entries(snapshot, g_frame.steps.opaque, g_frame.steps.opaque_count,
                 FSAPP_GLDRAW_PROGRAM_TEXTURE_OPAQUE);

    // Step 2, translucent behind the emulation surface, back to front. Nothing is flushed before
    // the wait.
    draw_translucent(snapshot, g_frame.steps.translucent_below,
                     g_frame.steps.translucent_below_count);
    fsapp_gldraw_check_errors("drawing below");
}

static bool same_rectangle(const fsapp_frame_plan_rectangle_t *a,
                           const fsapp_frame_plan_rectangle_t *b)
{
    return a->x == b->x && a->y == b->y && a->width == b->width && a->height == b->height;
}

// Step 4 for streamed content: the front texture's source rectangle drawn into the picture
// rectangle as they are after the wait (with a commit made in it, the new content), both cut to
// what was uploaded and clipped to the surface's rectangle with the frame plan's arithmetic, at the
// surface's depth with the streamed-content program and the content's filter. When step 1 left a
// picture unfilled that is not the one drawn now (a commit in the wait changed the picture
// rectangle, or the content is gone), the bars it filled no longer fit: the surface's whole
// rectangle is filled again first, at the same depth, with the fill colour. A first commit after no
// content needs no second fill, since step 1 filled the whole rectangle. The depth test is on with
// GL_LEQUAL (step 1 wrote the surface's depth over the whole rectangle), depth writes, blending and
// alpha writes are off (the programs write alpha 1 anyway; the window stays opaque). With no
// content and nothing to fill again, nothing.
static void draw_streamed_content(const fsapp_snapshot_t *snapshot,
                                  const fsapp_snapshot_surface_t *surface, int place)
{
    fsapp_streamed_content_t content;
    fsapp_streamed_get_content(surface->id, &content);
    fsapp_frame_plan_rectangle_t rectangle = {surface->x, surface->y, surface->width,
                                              surface->height};
    fsapp_frame_plan_rectangle_t picture = {};
    fsapp_frame_plan_source_t source = {};
    fsapp_frame_plan_rectangle_t clipped_picture = {};
    fsapp_frame_plan_source_t clipped_source = {};
    bool draws_picture = content.has_content &&
                         get_streamed_picture(surface, &content, &picture, &source) &&
                         fsapp_frame_plan_clip_picture(&rectangle, &picture, &source,
                                                       &clipped_picture, &clipped_source);
    bool fills_again =
        g_frame.picture_unfilled &&
        (!draws_picture || !same_rectangle(&clipped_picture, &g_frame.unfilled_picture));
    if (!draws_picture && !fills_again) {
        return;
    }
    double depth = place_depth(place);
    set_viewport(snapshot);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glDisable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    if (fills_again) {
        fsapp_gldraw_begin_program(FSAPP_GLDRAW_PROGRAM_FILL, snapshot->window.width,
                                   snapshot->window.height);
        SDL_FRect rect = {(float)rectangle.x, (float)rectangle.y, (float)rectangle.width,
                          (float)rectangle.height};
        fill_opaque(&rect, depth, surface->fill_color);
        fsapp_gldraw_end_program();
    }
    if (draws_picture) {
        fsapp_gldraw_begin_program(FSAPP_GLDRAW_PROGRAM_STREAMED, snapshot->window.width,
                                   snapshot->window.height);
        glBindTexture(GL_TEXTURE_2D, content.texture);
        GLint filter =
            content.filter == FSAPP_SURFACE_STREAM_FILTER_NEAREST ? GL_NEAREST : GL_LINEAR;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        SDL_FRect rect = {(float)clipped_picture.x, (float)clipped_picture.y,
                          (float)clipped_picture.width, (float)clipped_picture.height};
        // The source rectangle in texture coordinates (texel row 0, the region's top row, at t 0).
        SDL_FRect part = {(float)(clipped_source.x / content.texture_width),
                          (float)(clipped_source.y / content.texture_height),
                          (float)(clipped_source.width / content.texture_width),
                          (float)(clipped_source.height / content.texture_height)};
        // The whole source rectangle (cut to what was uploaded) inset by half a texel (see the
        // streamed-content program).
        SDL_FRect clamp = {(float)((source.x + 0.5) / content.texture_width),
                           (float)((source.y + 0.5) / content.texture_height),
                           (float)((source.width - 1.0) / content.texture_width),
                           (float)((source.height - 1.0) / content.texture_height)};
        fsapp_gldraw_draw_texture_part(content.texture, &rect, depth, &part, &clamp);
        fsapp_gldraw_end_program();
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void fsapp_compositor_draw_paced_surface(const fsapp_snapshot_t *snapshot)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    // Errors since drawing below: the pacer's wait hook.
    fsapp_gldraw_check_errors("the wait");
    // Step 4, from the plan worked out in drawing below.
    const fsapp_frame_plan_entry_t *entry = g_frame.steps.emulation;
    if (entry == NULL) {
        return;
    }
    const fsapp_snapshot_surface_t *surface = g_frame.snapshot_surfaces[entry->index];
    SDL_assert(surface->streamed);
    draw_streamed_content(snapshot, surface, entry->place);
    fsapp_gldraw_check_errors("drawing the paced surface");
}

void fsapp_compositor_draw_above(const fsapp_snapshot_t *snapshot)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    // Step 5, translucent in front of the emulation surface, back to front, over the emulated frame
    // just drawn.
    draw_translucent(snapshot, g_frame.steps.translucent_above,
                     g_frame.steps.translucent_above_count);
    fsapp_gldraw_check_errors("drawing above");
}
