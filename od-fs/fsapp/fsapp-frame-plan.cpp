/*
 * The frame plan (fsapp-frame-plan.h, which describes the rules and the steps).
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-frame-plan.h"

#include <algorithm>
#include <vector>

// An adornment attached to a surface.
struct attachment {
    // The surface's index in the input.
    int surface;
    // The adornment's index in the input.
    int adornment;
};

// An entry in the frame's list.
struct list_item {
    fsapp_frame_plan_entry_t entry;
    bool translucent;
};

struct fsapp_frame_plan {
    // The adornments attached to a surface in the input, by the surface's index and then in
    // creation order.
    std::vector<attachment> attachments;
    // The frame's list, from back to front.
    std::vector<list_item> list;
    // The steps.
    std::vector<fsapp_frame_plan_entry_t> opaque;
    std::vector<fsapp_frame_plan_entry_t> translucent_below;
    std::vector<fsapp_frame_plan_entry_t> translucent_above;
    fsapp_frame_plan_entry_t emulation;
};

fsapp_frame_plan_t *fsapp_frame_plan_create(void)
{
    return new fsapp_frame_plan;
}

void fsapp_frame_plan_destroy(fsapp_frame_plan_t *plan)
{
    delete plan;
}

static bool is_empty(const fsapp_frame_plan_rectangle_t *rectangle)
{
    return rectangle->width <= 0 || rectangle->height <= 0;
}

static list_item make_surface_item(const fsapp_frame_plan_input_t *input, int index)
{
    const fsapp_frame_plan_surface_t *surface = &input->surfaces[index];
    list_item item = {};
    item.entry.kind = FSAPP_FRAME_PLAN_SURFACE;
    item.entry.index = index;
    item.entry.id = surface->id;
    item.entry.rectangle = surface->rectangle;
    item.translucent = surface->translucent;
    return item;
}

static list_item make_adornment_item(const fsapp_frame_plan_input_t *input, int index)
{
    const fsapp_frame_plan_adornment_t *adornment = &input->adornments[index];
    list_item item = {};
    item.entry.kind = FSAPP_FRAME_PLAN_ADORNMENT;
    item.entry.index = index;
    item.entry.id = adornment->id;
    item.entry.rectangle = adornment->rectangle;
    item.translucent = adornment->translucent;
    return item;
}

// The index of the emulation surface in the input, or -1: the frontmost surface with streamed
// content that is the paced surface while a pacer is set.
static int find_emulation_surface(const fsapp_frame_plan_input_t *input)
{
    for (int i = input->surface_count - 1; i >= 0; i--) {
        const fsapp_frame_plan_surface_t *surface = &input->surfaces[i];
        if (!surface->streamed || is_empty(&surface->rectangle)) {
            continue;
        }
        if (!input->pacer_set || surface->id == input->paced_surface) {
            return i;
        }
    }
    return -1;
}

// The index of the surface in the input, or -1.
static int find_surface(const fsapp_frame_plan_input_t *input, const void *id)
{
    for (int i = 0; i < input->surface_count; i++) {
        if (input->surfaces[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Finds the adornments attached to a surface in the input, and sorts them by the surface's index
// and then in creation order.
static void find_attachments(fsapp_frame_plan_t *plan, const fsapp_frame_plan_input_t *input)
{
    plan->attachments.clear();
    for (int i = 0; i < input->adornment_count; i++) {
        const fsapp_frame_plan_adornment_t *adornment = &input->adornments[i];
        if (adornment->surface == nullptr || is_empty(&adornment->rectangle)) {
            continue;
        }
        // A linear search for each adornment attached to a surface: there are a few of them (the
        // performance graph's sidebars).
        int surface = find_surface(input, adornment->surface);
        if (surface != -1) {
            plan->attachments.push_back({surface, i});
        }
    }
    // std::sort sorts in place, with no allocation.
    std::sort(plan->attachments.begin(), plan->attachments.end(),
              [](const attachment &a, const attachment &b) {
                  return a.surface != b.surface ? a.surface < b.surface : a.adornment < b.adornment;
              });
}

// Adds an entry of the emulation surface's in step 1 over a rectangle, unless it is empty.
static void add_emulation_entry(fsapp_frame_plan_t *plan, fsapp_frame_plan_entry_t entry,
                                fsapp_frame_plan_entry_kind_t kind,
                                const fsapp_frame_plan_rectangle_t &rectangle)
{
    entry.kind = kind;
    entry.rectangle = rectangle;
    if (!is_empty(&entry.rectangle)) {
        plan->opaque.push_back(entry);
    }
}

// Adds the emulation surface's entries in step 1 at its place: with content and a clipped picture
// that is not empty, fill entries over the bars (above, below, left and right of the picture) and
// a depth-only entry over the picture; else one fill entry over the whole rectangle.
static void add_emulation_entries(fsapp_frame_plan_t *plan, const fsapp_frame_plan_input_t *input,
                                  const fsapp_frame_plan_entry_t &entry)
{
    const fsapp_frame_plan_surface_t *surface = &input->surfaces[entry.index];
    const fsapp_frame_plan_rectangle_t &rectangle = entry.rectangle;
    fsapp_frame_plan_rectangle_t picture;
    if (!surface->has_content ||
        !fsapp_frame_plan_clip_picture(&rectangle, &surface->picture, nullptr, &picture, nullptr)) {
        add_emulation_entry(plan, entry, FSAPP_FRAME_PLAN_FILL, rectangle);
        return;
    }
    int right = rectangle.x + rectangle.width;
    int bottom = rectangle.y + rectangle.height;
    int picture_right = picture.x + picture.width;
    int picture_bottom = picture.y + picture.height;
    // Above and below the picture, the whole width.
    add_emulation_entry(plan, entry, FSAPP_FRAME_PLAN_FILL,
                        {rectangle.x, rectangle.y, rectangle.width, picture.y - rectangle.y});
    add_emulation_entry(plan, entry, FSAPP_FRAME_PLAN_FILL,
                        {rectangle.x, picture_bottom, rectangle.width, bottom - picture_bottom});
    // Left and right of the picture, between them.
    add_emulation_entry(plan, entry, FSAPP_FRAME_PLAN_FILL,
                        {rectangle.x, picture.y, picture.x - rectangle.x, picture.height});
    add_emulation_entry(plan, entry, FSAPP_FRAME_PLAN_FILL,
                        {picture_right, picture.y, right - picture_right, picture.height});
    // REVIEW: The depth-only entry, so that nothing behind the emulation surface is drawn where its
    // picture will be. Whether that saves more than the extra entry costs is open, to be measured
    // on a Raspberry Pi once the performance graph can show it.
    add_emulation_entry(plan, entry, FSAPP_FRAME_PLAN_DEPTH_ONLY, picture);
}

void fsapp_frame_plan_update(fsapp_frame_plan_t *plan, const fsapp_frame_plan_input_t *input,
                             fsapp_frame_plan_steps_t *steps)
{
    plan->list.clear();
    plan->opaque.clear();
    plan->translucent_below.clear();
    plan->translucent_above.clear();

    // The frame's list, back to front: the surfaces, each followed by the adornments attached to
    // it, and then the adornments in front of everything.
    find_attachments(plan, input);
    int emulation_surface = find_emulation_surface(input);
    // The emulation surface's index in the list, or -1.
    int emulation = -1;
    // The next attachment to look at.
    size_t next = 0;
    for (int i = 0; i < input->surface_count; i++) {
        const fsapp_frame_plan_surface_t *surface = &input->surfaces[i];
        bool in_list = false;
        if (i == emulation_surface) {
            emulation = (int)plan->list.size();
            list_item item = make_surface_item(input, i);
            item.translucent = false;
            plan->list.push_back(item);
            in_list = true;
        } else if (!surface->streamed && !is_empty(&surface->rectangle)) {
            plan->list.push_back(make_surface_item(input, i));
            in_list = true;
        }
        for (; next < plan->attachments.size() && plan->attachments[next].surface == i; next++) {
            if (in_list) {
                plan->list.push_back(make_adornment_item(input, plan->attachments[next].adornment));
            }
        }
    }
    for (int i = 0; i < input->adornment_count; i++) {
        const fsapp_frame_plan_adornment_t *adornment = &input->adornments[i];
        if (adornment->surface == nullptr && !is_empty(&adornment->rectangle)) {
            plan->list.push_back(make_adornment_item(input, i));
        }
    }

    // A list longer than there are places drops the entries at the back: the list starts at first.
    int count = (int)plan->list.size();
    int first = count > FSAPP_FRAME_PLAN_PLACE_COUNT ? count - FSAPP_FRAME_PLAN_PLACE_COUNT : 0;
    if (emulation < first) {
        emulation = -1;
    }

    // Places, counted from the front.
    for (int i = first; i < count; i++) {
        plan->list[i].entry.place = count - 1 - i;
    }

    // Step 1, front to back.
    for (int i = count - 1; i >= first; i--) {
        if (i == emulation) {
            add_emulation_entries(plan, input, plan->list[i].entry);
        } else if (!plan->list[i].translucent) {
            plan->opaque.push_back(plan->list[i].entry);
        }
    }
    // Steps 2 and 5, back to front.
    for (int i = first; i < count; i++) {
        if (plan->list[i].translucent) {
            if (emulation != -1 && i > emulation) {
                plan->translucent_above.push_back(plan->list[i].entry);
            } else {
                plan->translucent_below.push_back(plan->list[i].entry);
            }
        }
    }
    // Step 4.
    if (emulation != -1) {
        plan->emulation = plan->list[emulation].entry;
    }

    *steps = {};
    steps->opaque = plan->opaque.data();
    steps->opaque_count = (int)plan->opaque.size();
    steps->translucent_below = plan->translucent_below.data();
    steps->translucent_below_count = (int)plan->translucent_below.size();
    steps->emulation = emulation != -1 ? &plan->emulation : nullptr;
    steps->translucent_above = plan->translucent_above.data();
    steps->translucent_above_count = (int)plan->translucent_above.size();
    steps->dropped_count = first;
}

bool fsapp_frame_plan_clip_picture(const fsapp_frame_plan_rectangle_t *surface,
                                   const fsapp_frame_plan_rectangle_t *picture,
                                   const fsapp_frame_plan_source_t *source,
                                   fsapp_frame_plan_rectangle_t *clipped_picture,
                                   fsapp_frame_plan_source_t *clipped_source)
{
    if (is_empty(surface) || is_empty(picture)) {
        return false;
    }
    int left = std::max(surface->x, picture->x);
    int top = std::max(surface->y, picture->y);
    int right = std::min(surface->x + surface->width, picture->x + picture->width);
    int bottom = std::min(surface->y + surface->height, picture->y + picture->height);
    if (right <= left || bottom <= top) {
        return false;
    }
    *clipped_picture = {left, top, right - left, bottom - top};
    if (source != nullptr) {
        // Source texels per window pixel, across and down.
        double scale_x = source->width / picture->width;
        double scale_y = source->height / picture->height;
        clipped_source->x = source->x + (left - picture->x) * scale_x;
        clipped_source->y = source->y + (top - picture->y) * scale_y;
        clipped_source->width = (right - left) * scale_x;
        clipped_source->height = (bottom - top) * scale_y;
    }
    return true;
}

// The size of a texture in one direction for a region of the given size, as
// fsapp_frame_plan_grow_texture says.
static int grow_texture_size(int texture, int region, int max_size)
{
    // REVIEW: A multiple of 64 (a choice, not anything GL asks for), so that a texture is not
    // created again for every few rows or columns that auto-crop or an RTG mode adds.
    const int multiple = 64;
    int rounded = (region + multiple - 1) / multiple * multiple;
    return std::min(std::max(texture, rounded), max_size);
}

bool fsapp_frame_plan_grow_texture(fsapp_frame_plan_size_t *texture,
                                   fsapp_frame_plan_size_t *region, int max_size)
{
    // REVIEW: A region larger than the GPU allows is cut to it, so that the part that fits is
    // shown; whether something better can be done with such a region is open.
    region->width = std::min(region->width, max_size);
    region->height = std::min(region->height, max_size);
    fsapp_frame_plan_size_t grown = {
        grow_texture_size(texture->width, region->width, max_size),
        grow_texture_size(texture->height, region->height, max_size),
    };
    bool changed = grown.width != texture->width || grown.height != texture->height;
    *texture = grown;
    return changed;
}
