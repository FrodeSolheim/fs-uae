/*
 * Stacking (fsapp-stacking.h, which describes the rules).
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-stacking.h"

#include <algorithm>
#include <vector>

namespace
{

// The surfaces and the order being written.
struct stacking {
    const fsapp_stacking_surface_t *surfaces;
    int count;
    fsapp_stacking_entry_t *order;
    int written;
};

// The surfaces that pass the filter, back to front: by raise order, then creation order.
template <typename Filter> std::vector<int> find_siblings(const stacking &stack, Filter filter)
{
    std::vector<int> siblings;
    for (int i = 0; i < stack.count; i++) {
        if (filter(stack.surfaces[i])) {
            siblings.push_back(i);
        }
    }
    const fsapp_stacking_surface_t *surfaces = stack.surfaces;
    std::stable_sort(siblings.begin(), siblings.end(),
                     [surfaces](int a, int b) { return surfaces[a].raise < surfaces[b].raise; });
    return siblings;
}

// The position in the order, from first up to end, of the surface with the id, or -1.
int find_written(const stacking &stack, int first, int end, const void *id)
{
    for (int i = first; i < end; i++) {
        if (stack.surfaces[stack.order[i].index].id == id) {
            return i;
        }
    }
    return -1;
}

// Writes the surface, then its children depth first.
void write_children(stacking &stack, int index, bool parent_shown)
{
    int position = stack.written++;
    bool shown = parent_shown && stack.surfaces[index].visible;
    stack.order[position] = {index, shown};
    const void *id = stack.surfaces[index].id;
    for (int child : find_siblings(stack, [id](const fsapp_stacking_surface_t &surface) {
             return surface.role == FSAPP_STACKING_ROLE_CHILD && surface.parent == id;
         })) {
        write_children(stack, child, shown);
    }
}

// Writes the tree of a top-level, an overlay or a popup: the surface and its children, then the
// popups of any of them, each with its own tree.
void write_tree(stacking &stack, int index, bool parent_shown)
{
    int first = stack.written;
    write_children(stack, index, parent_shown);
    int end = stack.written;
    for (int popup : find_siblings(stack, [&](const fsapp_stacking_surface_t &surface) {
             return surface.role == FSAPP_STACKING_ROLE_POPUP &&
                    find_written(stack, first, end, surface.parent) >= 0;
         })) {
        int parent = find_written(stack, first, end, stack.surfaces[popup].parent);
        write_tree(stack, popup, stack.order[parent].shown);
    }
}

} // namespace

int fsapp_stacking_compute_order(const fsapp_stacking_surface_t *surfaces, int count,
                                 fsapp_stacking_entry_t *order)
{
    static const fsapp_stacking_band_t bands[] = {
        FSAPP_STACKING_BAND_BACKGROUND,
        FSAPP_STACKING_BAND_NORMAL,
        FSAPP_STACKING_BAND_PANEL,
    };
    stacking stack = {surfaces, count, order, 0};
    for (fsapp_stacking_band_t band : bands) {
        for (int top_level : find_siblings(stack, [band](const fsapp_stacking_surface_t &surface) {
                 return surface.role == FSAPP_STACKING_ROLE_TOP_LEVEL && surface.band == band;
             })) {
            write_tree(stack, top_level, true);
        }
    }
    for (int overlay : find_siblings(stack, [](const fsapp_stacking_surface_t &surface) {
             return surface.role == FSAPP_STACKING_ROLE_OVERLAY;
         })) {
        write_tree(stack, overlay, true);
    }
    return stack.written;
}
