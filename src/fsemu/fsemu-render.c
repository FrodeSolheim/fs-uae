#if 0

#define FSEMU_INTERNAL
#include "fsemu/fsemu-render.h"

#include "fsemu/fsemu-titlebar.h"
#include "fsemu/fsemu-types.h"

// ---------------------------------------------------------------------------

#define FSEMU_RENDER_MAX_ITEMS 64

typedef struct {
    fsemu_rect_t rect;
    bool translucent;
    uint32_t color;
    void *image;
    const char *text;
    void *font;
} fsemu_render_item_t;

static struct {
    fsemu_render_item_t items[FSEMU_RENDER_MAX_ITEMS];
    int num_items;
} fsemu_render;

// ---------------------------------------------------------------------------

void fsemu_render_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_titlebar_init();
}

// ---------------------------------------------------------------------------

static void fsemu_render_rectangle(int x, int y, int w, int h, int c)
{
    // SDL_RenderDrawRect(renderer, rect);
}

static void fsemu_render_init_renderer(fsemu_render_t *render)
{
}

// ---------------------------------------------------------------------------

void fsemu_render_render_early(void)
{
    fsemu_render_t render;
    fsemu_render_init_renderer(&render);
}

// ---------------------------------------------------------------------------

void fsemu_render_render_late(void)
{
    fsemu_render_t render;
    fsemu_render_init_renderer(&render);

    fsemu_render.num_items = 0;

    fsemu_titlebar_render(&render);
}

// ---------------------------------------------------------------------------

#endif