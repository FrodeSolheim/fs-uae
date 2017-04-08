#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "xwin.h"
#include "uae.h"
#include "custom.h"
#include "drawing.h"
#include "gfxfilter.h"
#include "gui.h"
#include "uae/fs.h"

#include <limits.h>
#include <stdlib.h>

#ifdef PICASSO96
#include "picasso96.h"
#endif

#include <fs/emu/video.h>

int max_uae_width = 3072;
int max_uae_height = 2048;

volatile bool vblank_found_chipset;
volatile bool vblank_found_rtg;

#define MAXBLOCKLINES_MAX INT_MAX;

#define AMIGA_WIDTH (AMIGA_WIDTH_MAX * 2)
//#define AMIGA_HEIGHT (AMIGA_HEIGHT_MAX * 2)
//#define AMIGA_HEIGHT 574
#define AMIGA_HEIGHT 572

uae_s32 tyhrgb[65536];
uae_s32 tylrgb[65536];
uae_s32 tcbrgb[65536];
uae_s32 tcrrgb[65536];

//#define USE_BUFMEM
//#define USE_LINEMEM

struct MultiDisplay Displays[MAX_DISPLAYS + 1] = {};

static int g_picasso_enabled = 0;
static int g_picasso_width = 0;
static int g_picasso_height = 0;
static int g_picasso_depth = 0;
static int g_picasso_format = 0;

static int g_has_flushed_line = 0;
static int g_has_flushed_block = 0;
static int g_has_flushed_screen = 0;

static int g_largest_width = 0;
static int g_largest_height = 0;

struct uae_filter uaefilters[] = {
    { UAE_FILTER_NULL, 0, 1, _T("Null filter"), _T("null"),
            UAE_FILTER_MODE_16_16 | UAE_FILTER_MODE_32_32 },
    { 0 }
};

#define SET_FLAG(x, y) ((x) |= (y))
#define CLEAR_FLAG(x, y) ((x) &= ~(y))
#define SET_OR_CLEAR_FLAG(x, y, z) ((z) ? \
        SET_FLAG((x), (y)) : CLEAR_FLAG((x), (y)))

static volatile bool render_ok, wait_render;

volatile bool thread_vblank_found;
// --- win32gfx.c
int screen_is_picasso = 0;
struct uae_filter *usedfilter;
uae_u32 redc[3 * 256], grec[3 * 256], bluc[3 * 256];

static double remembered_vblank;
static int vblankbasewait, vblankbasefull;
RenderData g_renderdata;
static int g_screen_updated = 0;

static uae_u8 g_linemem[4096 * 4];
static unsigned char* g_bufmem = NULL;

static int g_red_bits;
static int g_green_bits;
static int g_blue_bits;
static int g_alpha_bits;
static int g_red_shift;
static int g_green_shift;
static int g_blue_shift;
static int g_alpha_shift;

int g_amiga_rtg_modes[] = {
        640, 360, // 16:9

        800, 450, // 16:9
        800, 500, // 16:10
        800, 600,

        1024, 576, // 16:9
        1024, 600, // 16:10
        1024, 768,

        1280, 720, // 16:9
        //add_mode (md1, 1280, 800, 32, 50, 1); // 16:10
        //add_mode (md1, 1280, 720, 32, 50, 1);

        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        0, 0,
        -1, -1,
};

#if 0
void uae_line_update(int line, int update) {
    printf("%d %d\n", line, update);
    if (!update) {
        return;
    }
    // mark this line as not needing copy from the previous render buffer
    g_renderdata.line[line] = 0;
    g_screen_updated = 1;
}
#endif

void frame_drawn(void) {

}

void flush_screen (struct vidbuffer *buffer, int first_line, int last_line) {
    //write_log("flush_screen\n");
    g_has_flushed_screen = 1;
}


bool target_graphics_buffer_update (void) {
    write_log("target_graphics_buffer_update - clearing buffer\n");
    memset(g_renderdata.pixels, 0, \
            AMIGA_WIDTH * AMIGA_HEIGHT * g_amiga_video_bpp);
    memset(g_renderdata.line, 0, AMIGA_MAX_LINES);
    return 0;
}

static bool render_frame(bool immediate)
{
// FIXME: immediate is a new parameter

    //write_log("render_screen line: %d block %d screen %d\n",
    //        g_has_flushed_line, g_has_flushed_block, g_has_flushed_screen);
    int flushed = g_has_flushed_line || g_has_flushed_block ||
            g_has_flushed_screen;

    g_renderdata.bpp = g_amiga_video_bpp;
    // printf("%d %d %d\n", g_amiga_video_bpp, AMIGA_WIDTH, AMIGA_HEIGHT);

    static int cx, cy, cw, ch, crealh;
    //printf("g_picasso_enabled %d\n", g_picasso_enabled);
    if (g_picasso_enabled) {
        g_renderdata.width = g_picasso_width;
        g_renderdata.height = g_picasso_height;
        g_renderdata.limit_x = 0;
        g_renderdata.limit_y = 0;
        g_renderdata.limit_w = g_picasso_width;
        g_renderdata.limit_h = g_picasso_height;
        //g_renderdata.updated = g_screen_updated;
        g_renderdata.flags |= AMIGA_VIDEO_RTG_MODE;

#ifdef USE_BUFMEM
        //memcpy(g_renderdata.pixels, g_bufmem, g_picasso_width * g_picasso_height * g_amiga_video_bpp);
#endif
        // FIXME
        memset(g_renderdata.line, 0, AMIGA_MAX_LINES);
    }
    else {
        if (gfxvidinfo.outbuffer) {
            // if gfxvidinfo.outbuffer is not set, get_custom_limits will
            // crash
            if (flushed) {
                get_custom_limits(&cw, &ch, &cx, &cy, &crealh);
                // FIXME: crealh is new - find out what it does
            }
            else {
                // reuse last custom limits
            }
        }
        if (cx < 0) {
            //write_log("WARNING: custom limit x (%d) is < 0 - clamping\n", cx);
            cx = 0;
        }
        if (cy < 0) {
            //write_log("WARNING: custom limit y (%d) is < 0 - clamping\n", cy);
            cy = 0;
        }
        if (cx + cw > AMIGA_WIDTH) {
            //write_log("WARNING: custom limit x (%d) + w (%d) is > "
            //        "AMIGA_WIDTH (%d) - clamping\n", cx, cw, AMIGA_WIDTH);
            cw = AMIGA_WIDTH - cx;
        }
        if (cy + ch > AMIGA_HEIGHT) {
            //write_log("WARNING: custom limit y (%d) + h (%d) is > "
            //        "AMIGA_HEIGHT (%d) - clamping\n", cy, ch, AMIGA_HEIGHT);
            ch = AMIGA_HEIGHT - cy;
        }
        g_renderdata.width = AMIGA_WIDTH;
        g_renderdata.height = AMIGA_HEIGHT;
        g_renderdata.limit_x = cx;
        g_renderdata.limit_y = cy;
        g_renderdata.limit_w = cw;
        g_renderdata.limit_h = ch;
        //g_renderdata.updated = g_screen_updated;
        CLEAR_FLAG(g_renderdata.flags, AMIGA_VIDEO_RTG_MODE);
        SET_OR_CLEAR_FLAG(g_renderdata.flags, AMIGA_VIDEO_LOW_RESOLUTION,
                currprefs.gfx_resolution == 0);
        SET_OR_CLEAR_FLAG(g_renderdata.flags, AMIGA_VIDEO_LINE_DOUBLING,
                currprefs.gfx_vresolution == 1);

#ifdef USE_BUFMEM
        //printf("g_renderdata.pixels %p %p", g_renderdata.pixels, g_bufmem);
        memcpy(g_renderdata.pixels, g_bufmem, AMIGA_WIDTH * AMIGA_HEIGHT * g_amiga_video_bpp);
#endif
    }
    //g_renderdata.line[first_line] = 0;
    //g_renderdata.line[first_line + 1] = 0;
    //for (int y = first_line; y <= last_line; y++) {
    //    g_renderdata.line[y] = 0;
    //}
    g_screen_updated = 0;
    //printf("flush_screen (%d -> %d) %d %d %d %d\n", first_line, last_line,
    //        cx, cy, cw, ch);

    od_fs_update_leds();

    if (currprefs.turbo_emulation) {
        g_renderdata.refresh_rate = -1;
    }
    else {
        g_renderdata.refresh_rate = currprefs.chipset_refreshrate;
    }
#if 0
    printf("%0.2f\n", g_renderdata.refresh_rate);
#endif
    if (g_libamiga_callbacks.render) {
        g_libamiga_callbacks.render(&g_renderdata);
    }

    g_has_flushed_line = 0;
    g_has_flushed_block = 0;
    g_has_flushed_screen = 0;

    if (fse_drivers()) {
        notice_screen_contents_lost();
    }
    return 1;
}

static int minimized;
static int monitor_off;

static int dx_islost(void)
{
        return 0;
}

bool render_screen(bool immediate)
{
    bool v = false;
    int cnt;

    render_ok = false;
    if (minimized || picasso_on || monitor_off || dx_islost ())
            return render_ok;
    cnt = 0;
    while (wait_render) {
            sleep_millis (1);
            cnt++;
            if (cnt > 500)
                    return render_ok;
    }
#if 0
    flushymin = 0;
    flushymax = currentmode->amiga_height;
    //EnterCriticalSection (&screen_cs);
    if (currentmode->flags & DM_D3D) {
            v = D3D_renderframe (immediate);
#endif
    v = render_frame(immediate);
#if 0
    } else if (currentmode->flags & DM_SWSCALE) {
            S2X_render ();
            v = true;
    } else if (currentmode->flags & DM_DDRAW) {
            v = true;
    }
#endif
    render_ok = v;
    //LeaveCriticalSection (&screen_cs);
    return render_ok;
}

bool toggle_rtg (int mode)
{
    UAE_LOG_STUB("mode=%d", mode);
#if 0
        if (mode == 0) {
                if (!picasso_on)
                        return false;
        } else if (mode > 0) {
                if (picasso_on)
                        return false;
        }
        if (currprefs.rtgmem_type >= GFXBOARD_HARDWARE) {
                return gfxboard_toggle (mode);
        } else {
                // can always switch from RTG to custom
                if (picasso_requested_on && picasso_on) {
                        picasso_requested_on = false;
                        return true;
                }
                if (picasso_on)
                        return false;
                // can only switch from custom to RTG if there is some mode act$
                if (picasso_is_active ()) {
                        picasso_requested_on = true;
                        return true;
                }
        }
#endif
        return false;
}

void show_screen (int mode)
{
#ifdef DEBUG_SHOW_SCREEN
    printf("show_screen mode=%d\n\n", mode);
#endif
    if (g_libamiga_callbacks.display) {
        g_libamiga_callbacks.display();
    }
}

bool show_screen_maybe (bool show) {
#ifdef DEBUG_SHOW_SCREEN
    printf("show_screen_maybe %d (picasso_on=%d)\n", show, picasso_on);
#endif

    struct apmode *ap = picasso_on ? &currprefs.gfx_apmode[1] : &currprefs.gfx_apmode[0];
    if (!ap->gfx_vflip || ap->gfx_vsyncmode == 0 || !ap->gfx_vsync) {
        if (show) {
        //if (show && !picasso_on) {
            show_screen (0);
        }
        return false;
    }
#if 0
	if (ap->gfx_vflip < 0) {
		doflipevent ();
		return true;
	}
#endif
    return false;
}

double vblank_calibrate (double approx_vblank, bool waitonly) {
    UAE_LOG_STUB("");
    return -1;
}

// FIXME: What is this?
int extraframewait = 0;
static int frame_missed, frame_counted, frame_errors;
static int frame_usage, frame_usage_avg, frame_usage_total;
//extern int log_vsync;
static bool dooddevenskip;
static volatile frame_time_t vblank_prev_time, thread_vblank_time;
//static bool vblankbaselace;
static int vblankbaselace_chipset;
//static bool vblankthread_oddeven;

int log_vsync = 0, debug_vsync_min_delay = 0, debug_vsync_forced_delay = 0;

void vsync_busywait_start(void) {
    UAE_LOG_STUB("");
    //changevblankthreadmode_fast (VBLANKTH_ACTIVE_START);
    vblank_prev_time = thread_vblank_time;
}

static bool isthreadedvsync (void) {
    return isvsync_chipset () <= -2 || isvsync_rtg () < 0;
}

// FIXME
bool vsync_isdone (void) {
    return vblank_found_chipset || dooddevenskip;
}

int vsync_busywait_do (int *freetime, bool lace, bool oddeven) {
    UAE_LOG_STUB("");
    return false;
#if 0
    bool v;
    static bool framelost;
    int ti;
    frame_time_t t;
    frame_time_t prevtime = vblank_prev_time;

    dooddevenskip = false;

    if (lace)
        vblankbaselace_chipset = oddeven;
    else
        vblankbaselace_chipset = -1;

    t = read_processor_time ();
    ti = t - prevtime;
    //if (ti > 2 * vblankbasefull || ti < -2 * vblankbasefull) {
    if (ti > 1 * vblankbasefull || ti < -1 * vblankbasefull) {
#if 0
        waitvblankstate (false, NULL);
#endif
        t = read_processor_time ();
        vblank_prev_time = t;
        thread_vblank_time = t;
        frame_missed++;
        return true;
    }

    //if (log_vsync) {
    //    console_out_f(_T("F:%8d M:%8d E:%8d %3d%% (%3d%%) %10d\r"), frame_counted, frame_missed, frame_errors, frame_usage, frame_usage_avg, (t - vblank_prev_time) - vblankbasefull);
    //}

    if (freetime)
        *freetime = 0;
    if (currprefs.turbo_emulation) {
        frame_missed++;
        return true;
    }
#if 0
    frame_usage = (t - prevtime) * 100 / vblankbasefull;
    if (frame_usage > 99)
        frame_usage = 99;
    else if (frame_usage < 0)
        frame_usage = 0;
    frame_usage_total += frame_usage;
    if (freetime)
        *freetime = frame_usage;
    if (frame_counted)
        frame_usage_avg = frame_usage_total / frame_counted;
#endif
    v = false;

    if (isthreadedvsync ()) {

        framelost = false;
        v = true;

    } else {
#if 0
        bool doskip = false;

        if (!framelost && t - prevtime > vblankbasefull) {
            framelost = true;
            frame_missed++;
            return true;
        }

        if (vblanklaceskip ()) {
            doskip = true;
            dooddevenskip = true;
        }

        if (!doskip) {
            while (!framelost && read_processor_time () - prevtime < vblankbasewait1) {
                vsync_sleep (false);
            }
            v = vblank_wait ();
        } else {
            v = true;
        }
        framelost = false;
#endif
    }

    if (v) {
        vblank_prev_time = read_processor_time ();
        frame_counted++;
        return true;
    }
    frame_errors++;
    return false;
#endif
}

static void vsync_sleep (bool preferbusy) {
#if 0
    struct apmode *ap = picasso_on ? &currprefs.gfx_apmode[1] : &currprefs.gfx_apmode[0];
    bool dowait;
    if (vsync_busy_wait_mode == 0) {
        dowait = ap->gfx_vflip || !preferbusy;
    } else if (vsync_busy_wait_mode < 0) {
        dowait = true;
    } else {
        dowait = false;
    }
    dowait = true;
    if (dowait && currprefs.m68k_speed >= 0)
        sleep_millis_main (1);
#endif
}


static void vsync_notvblank (void) {
    return;
#if 0
    for (;;) {
        int vp;
        if (!getvblankpos (&vp))
            return;
        if (vp > 0) {
            //write_log (_T("%d "), vpos);
            break;
        }
        vsync_sleep (true);
    }
#endif
}

int target_get_display (const TCHAR *name) {
    return 0;
}
const TCHAR *target_get_display_name (int num, bool friendlyname) {
    // FIXME
    return (TCHAR *) "Dummy Display";
}

// FIXME
extern "C" {
int fs_ml_get_vblank_count();
}

frame_time_t vsync_busywait_end (int *flipdelay) {
#if 0
    printf("vsync_busywait_end\n");
    show_screen ();

    static int last_vblank = 0;
    while (fs_ml_get_vblank_count() == last_vblank) {

    }
    last_vblank++;// = fs_ml_get_vblank_count();

    if (!dooddevenskip) {
#if 0
        vsync_notvblank ();
        while (!vblank_found && vblankthread_mode == VBLANKTH_ACTIVE) {
            vsync_sleep (currprefs.m68k_speed < 0);
        }
#endif
    }
    //changevblankthreadmode_fast (VBLANKTH_ACTIVE_WAIT);
#if 0
    return thread_vblank_time;

    write_log("vsync_busywait_end\n");
#endif
#endif
    return read_processor_time();
}

double getcurrentvblankrate (void)
{
    UAE_LOG_STUB("");
    if (remembered_vblank) {
        return remembered_vblank;
    }
    return 50;
}

static int uae_bits_in_mask (unsigned int mask)
{
    int n = 0;
    while (mask) {
        n += mask & 1;
        mask >>= 1;
    }
    return n;
}

static int uae_mask_shift (unsigned int mask)
{
    int n = 0;
    while (!(mask & 1)) {
        n++;
        mask >>= 1;
    }
    return n;
}

static int init_colors (void)
{
    write_log("init_colors\n");
    alloc_colors64k(g_red_bits, g_green_bits, g_blue_bits, g_red_shift,
            g_green_shift, g_blue_shift, 0, 0, 0, 0);
    return 1;
}

#include <fs/emu/hacks.h>

void getgfxoffset (float *dxp, float *dyp, float *mxp, float *myp)
{
    /* Offset and scale factors used for magic mouse (in order to translate
     * mouse coordinates to Amiga coordinates) */
    *dxp = fs_emu_video_offset_x;
    *dyp = fs_emu_video_offset_y;
    *mxp = fs_emu_video_scale_x;
    *myp = fs_emu_video_scale_y;
}

void toggle_fullscreen (int mode)
{

}

int isfullscreen (void)
{
    return 0;
}

void flush_line (struct vidbuffer *buffer, int line_no)
{
    //printf("- flush_line %d\n", line_no);

    //scrlinebuf
#ifdef USE_LINEMEM
    unsigned char *dst = g_renderdata.pixels + AMIGA_WIDTH * g_amiga_video_bpp * line_no;
    memcpy(dst, g_linemem, AMIGA_WIDTH * g_amiga_video_bpp);
#endif

#ifndef USE_BUFMEM
    // mark this line as not needing copy from the previous render buffer
    g_renderdata.line[line_no] = 0;
#endif
    g_screen_updated = 1;
    g_has_flushed_line = 1;
}

void flush_block (struct vidbuffer *buffer, int first_line, int last_line)
{
    //printf("- flush_block %d %d\n", first_line, last_line);
    //g_screen_updated = 1;
    g_has_flushed_block = 1;
}

int lockscr(struct vidbuffer *buffer, bool fullupdate)
{
    return gfxvidinfo.drawbuffer.lockscr(&gfxvidinfo, buffer);
}

void unlockscr(struct vidbuffer *buffer)
{
    gfxvidinfo.drawbuffer.unlockscr(&gfxvidinfo, buffer);
}

int graphics_setup()
{
    write_log("graphics_setup\n");
    return 1;
}

static void grow_render_buffer(int width, int height)
{
    unsigned char *new_pixels = (unsigned char*) g_renderdata.grow(width, height);
    if (new_pixels != g_renderdata.pixels) {
        //printf("new %p old %p\n", new_pixels, g_renderdata.pixels);
        //printf("grow_render_buffer %d %d\n", width, height);
        g_renderdata.pixels = new_pixels;
        gfxvidinfo.drawbuffer.bufmem = new_pixels;
        init_row_map();
        //printf("grow_render_buffer %d %d done\n", width, height);
    }
}

void amiga_set_render_buffer(void *data, int size, int need_redraw,
        void *(*grow)(int width, int height))
{
    //printf("set render buffer %p\n", data);
    g_renderdata.grow = grow;
    g_renderdata.pixels = (unsigned char *) data;

    //printf("\n\n\n\n\n\n\n\n set buffer %p %d\n", data, size);
    //g_renderdata.pixels = (unsigned char*) data;
    //g_renderdata.pixels = (unsigned char*) data;

#ifndef USE_BUFMEM
    // reset line information
    memset(g_renderdata.line, 1, AMIGA_MAX_LINES);
#ifndef USE_LINEMEM
    //printf("setting bufmem\n");
    gfxvidinfo.drawbuffer.bufmem = (unsigned char*) data;
#endif
    //printf("updating row map\n");

    grow_render_buffer(g_largest_width, g_largest_height);
    init_row_map();
#endif
}

uint8_t *uae_get_render_buffer()
{
    return g_renderdata.pixels;
}

#define RGBA_MASK_R 0x000000ff
#define RGBA_MASK_G 0x0000ff00
#define RGBA_MASK_B 0x00ff0000
#define RGBA_MASK_A 0xff000000

#define R5G6B5_MASK_R 0xf800
#define R5G6B5_MASK_G 0x07e0
#define R5G6B5_MASK_B 0x001f

#define R5G5B5A1_MASK_R 0xf800
#define R5G5B5A1_MASK_G 0x07c0
#define R5G5B5A1_MASK_B 0x003e
#define R5G5B5A1_MASK_A 0x0001

int graphics_init(bool mousecapture)
{
    write_log("graphics_init\n");

    // FIXME: perhaps modify so custom_limits defaults to -1, -1, -1, -1
    set_custom_limits (-1, -1, -1, -1);

    if (g_amiga_video_format == AMIGA_VIDEO_FORMAT_R5G6B5) {
        g_red_bits    = uae_bits_in_mask(R5G6B5_MASK_R);
        g_red_shift   = uae_mask_shift(R5G6B5_MASK_R);
        g_green_bits  = uae_bits_in_mask(R5G6B5_MASK_G);
        g_green_shift = uae_mask_shift(R5G6B5_MASK_G);
        g_blue_bits   = uae_bits_in_mask(R5G6B5_MASK_B);
        g_blue_shift  = uae_mask_shift(R5G6B5_MASK_B);
        g_alpha_bits   = 0;
        g_alpha_shift  = 0;
    }
    else if (g_amiga_video_format == AMIGA_VIDEO_FORMAT_R5G5B5A1) {
        g_red_bits    = uae_bits_in_mask(R5G5B5A1_MASK_R);
        g_red_shift   = uae_mask_shift(R5G5B5A1_MASK_R);
        g_green_bits  = uae_bits_in_mask(R5G5B5A1_MASK_G);
        g_green_shift = uae_mask_shift(R5G5B5A1_MASK_G);
        g_blue_bits   = uae_bits_in_mask(R5G5B5A1_MASK_B);
        g_blue_shift  = uae_mask_shift(R5G5B5A1_MASK_B);
        g_alpha_bits   = uae_bits_in_mask(R5G5B5A1_MASK_A);
        g_alpha_shift  = uae_mask_shift(R5G5B5A1_MASK_A);
    }
    else { // RGBA or BGRA
        if (g_amiga_video_format == AMIGA_VIDEO_FORMAT_RGBA) {
            g_red_bits    = uae_bits_in_mask(RGBA_MASK_R);
            g_red_shift   = uae_mask_shift(RGBA_MASK_R);
            g_blue_bits   = uae_bits_in_mask(RGBA_MASK_B);
            g_blue_shift  = uae_mask_shift(RGBA_MASK_B);
        }
        else { // BGRA
            g_red_bits   = uae_bits_in_mask(RGBA_MASK_B);
            g_red_shift  = uae_mask_shift(RGBA_MASK_B);
            g_blue_bits    = uae_bits_in_mask(RGBA_MASK_R);
            g_blue_shift   = uae_mask_shift(RGBA_MASK_R);
        }
        g_green_bits  = uae_bits_in_mask(RGBA_MASK_G);
        g_green_shift = uae_mask_shift(RGBA_MASK_G);
        g_alpha_bits   = uae_bits_in_mask(RGBA_MASK_A);
        g_alpha_shift  = uae_mask_shift(RGBA_MASK_A);
    }

    //g_renderdata.pixels = (unsigned char*) malloc(AMIGA_WIDTH*AMIGA_HEIGHT*4);

    memset(g_renderdata.line, 0, AMIGA_MAX_LINES);
    gfxvidinfo.maxblocklines = 0;
#ifdef USE_BUFMEM
    g_bufmem = (unsigned char*) malloc(AMIGA_WIDTH * AMIGA_HEIGHT * g_amiga_video_bpp);
    gfxvidinfo.drawbuffer.bufmem = g_bufmem;
    memset(g_bufmem, 0, AMIGA_WIDTH * AMIGA_HEIGHT * g_amiga_video_bpp);
    gfxvidinfo.maxblocklines = MAXBLOCKLINES_MAX;
#endif

#ifdef USE_LINEMEM
    gfxvidinfo.drawbuffer.emergmem = 0;
    gfxvidinfo.drawbuffer.linemem = g_linemem;
#else
    gfxvidinfo.drawbuffer.emergmem = 0; //g_linemem;
    gfxvidinfo.drawbuffer.linemem = 0;
#endif
    gfxvidinfo.drawbuffer.pixbytes = g_amiga_video_bpp;
    gfxvidinfo.drawbuffer.rowbytes = AMIGA_WIDTH * g_amiga_video_bpp;
    gfxvidinfo.drawbuffer.height_allocated = AMIGA_HEIGHT;
    gfxvidinfo.drawbuffer.inheight = AMIGA_HEIGHT;
    gfxvidinfo.drawbuffer.outheight = AMIGA_HEIGHT;
    gfxvidinfo.drawbuffer.width_allocated = AMIGA_WIDTH;
    gfxvidinfo.drawbuffer.inwidth = AMIGA_WIDTH;
    gfxvidinfo.drawbuffer.outwidth = AMIGA_WIDTH;

    //gfxvidinfo.flush_block = libamiga_flush_block;
    //gfxvidinfo.flush_screen = libamiga_flush_screen;
    //SDL_SetColors (display, arSDLColors, 0, 256);
    write_log("calling reset_drawing\n");
    reset_drawing ();
    init_colors ();

//#ifdef USE_BUFMEM
//    init_row_map();
//#endif

    //write_log("FIXME: NOT USING VSYNC TRICK\n");
    // Trick UAE into sending believing we are in vsync / fullscreen
    // so a flush command is sent for each frame update in do_flush_screen.

    if (currprefs.m68k_speed == -1) {
        write_log("currprefs.m68k_speed is -1, not allowing full sync\n");
    }
    else {
        //currprefs.gfx_apmode[0].gfx_fullscreen = GFX_FULLSCREEN;
        currprefs.gfx_apmode[0].gfx_vsync = 1;
    }

    //currprefs.gfx_apmode[0].gfx_fullscreen = GFX_FULLSCREEN;
    //currprefs.gfx_apmode[0].gfx_vsync = 1;

    //currprefs.gfx_apmode[0].gfx_vsyncmode = 1;
    //currprefs.gfx_apmode[1].gfx_fullscreen = GFX_FULLSCREEN;
    //currprefs.gfx_apmode[1].gfx_vsync = 1;

    //amiga_set_option("gfx_vsync", "true");
    //amiga_set_option("gfx_vsyncmode", "busywait");

    return 1;
}

void graphics_leave (void)
{

}

int check_prefs_changed_gfx (void)
{
    //write_log("check_prefs_changed_gfx\n");
    return 0;
}

void gui_fps (int fps, int idle, int color)
{
    UAE_LOG_STUB_MAX(1, "");
}

int gui_update (void)
{
    return 0;
}

void gfx_set_picasso_colors (RGBFTYPE rgbfmt)
{
    write_log("gfx_set_picasso_colors %d\n", rgbfmt);

    alloc_colors_picasso(g_red_bits, g_green_bits, g_blue_bits, g_red_shift,
            g_green_shift, g_blue_shift, rgbfmt);
}

int picasso_palette (void)
{
    int i, changed;

    changed = 0;
    for (i = 0; i < 256; i++) {
        int r = picasso96_state.CLUT[i].Red;
        int g = picasso96_state.CLUT[i].Green;
        int b = picasso96_state.CLUT[i].Blue;
        uae_u32 v = (doMask256 (r, g_red_bits, g_red_shift)
            | doMask256 (g, g_green_bits, g_green_shift)
            | doMask256 (b, g_blue_bits, g_blue_shift))
            | doMask256 (0xff, g_alpha_bits, g_alpha_shift);
        if (v != picasso_vidinfo.clut[i]) {
            //write_log (_T("%d:%08x\n"), i, v);
            picasso_vidinfo.clut[i] = v;
            changed = 1;
        }
    }
    return changed;
}

static bool rtg_locked;

static uae_u8 *gfx_lock_picasso2 (bool fullupdate)
{
    picasso_vidinfo.rowbytes = picasso_vidinfo.width * g_amiga_video_bpp;
    uae_u8 *buffer = uae_get_render_buffer();
    // printf("gfx_lock_picasso2 buffer=%p\n", buffer);
    return buffer;
}

uae_u8 *gfx_lock_picasso (bool fullupdate, bool doclear)
{
    // FIXME: currently ignoring fullupdate (what does it do?)
    static uae_u8 *p;
    if (rtg_locked) {
            return p;
    }
#if 0
    EnterCriticalSection (&screen_cs);
#endif
    p = gfx_lock_picasso2 (fullupdate);
    if (!p) {
#if 0
            LeaveCriticalSection (&screen_cs);
#endif
    } else {
        rtg_locked = true;
        if (doclear) {
            uae_u8 *p2 = p;
            for (int h = 0; h < picasso_vidinfo.height; h++) {
                memset (p2, 0, picasso_vidinfo.width * picasso_vidinfo.pixbytes);
                p2 += picasso_vidinfo.rowbytes;
            }
        }
    }
    return p;
}

void gfx_unlock_picasso (bool dorender)
{
#ifdef DEBUG_SHOW_SCREEN
    printf("gfx_unlock_picasso dorender=%d\n", dorender);
#endif

#if 0
    if (!rtg_locked)
            EnterCriticalSection (&screen_cs);
#endif

    rtg_locked = false;

    if (dorender) {
        //render_screen(1);
        // FIXME: check what mode parameter is supposed to do
        //show_screen(0);
        if (render_frame(false)) {
            show_screen_maybe(true);
        }
    }
}

#if 1

void gfx_set_picasso_state (int on)
{
    write_log("gfx_set_picasso_state %d\n", on);
    g_picasso_enabled = (on != 0);
}

#else

void gfx_set_picasso_state (int on)
{
	printf("gfx_set_picasso_state %d\n", on);
#if 0

	//struct winuae_currentmode wc;
	int mode;

	if (screen_is_picasso == on)
		return;
	screen_is_picasso = on;
	//rp_rtg_switch ();
	//memcpy (&wc, currentmode, sizeof (wc));

	//updatemodes ();
	//update_gfxparams ();
	//clearscreen ();
	if (currprefs.gfx_apmode[0].gfx_fullscreen != currprefs.gfx_apmode[1].gfx_fullscreen || (currprefs.gfx_apmode[0].gfx_fullscreen == GFX_FULLSCREEN && currprefs.gfx_api)) {
		mode = 1;
	} else {
		mode = modeswitchneeded (&wc);
		if (!mode)
			goto end;
	}
	if (mode < 0) {
		open_windows (0);
	} else {
		open_screen (); // reopen everything
	}
	if (on && isvsync_rtg () < 0)
		vblank_calibrate (0, false);
end:
#ifdef RETROPLATFORM
	rp_set_hwnd (hAmigaWnd);
#endif
#endif
}

#endif

#if 1

void gfx_set_picasso_modeinfo (uae_u32 w, uae_u32 h, uae_u32 depth,
        RGBFTYPE rgbfmt) {
    write_log("gfx_set_picasso_modeinfo %d %d %d %d\n", w, h, depth, rgbfmt);
    g_picasso_width = w;
    g_picasso_height = h;
    g_picasso_depth = depth;
    g_picasso_format = rgbfmt;

    // register largest width seen, so render buffers can be adjusted if
    // necessary
    if (g_picasso_width > g_largest_width) {
        g_largest_width = g_picasso_width;
    }
    if (g_picasso_height > g_largest_height) {
        g_largest_height = g_picasso_height;
    }
    grow_render_buffer(g_largest_width, g_largest_height);

    gfx_set_picasso_colors (rgbfmt);

    picasso_vidinfo.width = g_picasso_width;
    picasso_vidinfo.height = g_picasso_height;
}

#else

void gfx_set_picasso_modeinfo (uae_u32 w, uae_u32 h, uae_u32 depth, RGBFTYPE rgbfmt)
{
	printf("gfx_set_picasso_modeinfo %d %d %d %d\n", w, h, depth, rgbfmt);
	exit(1);
#if 0
	int need;
	if (!screen_is_picasso)
		return;
	clearscreen ();
#endif
	gfx_set_picasso_colors (rgbfmt);
#if 0
	updatemodes ();
	need = modeswitchneeded (currentmode);
	update_gfxparams ();
	if (need > 0) {
		open_screen ();
	} else if (need < 0) {
		open_windows (0);
	}
#endif
#ifdef RETROPLATFORM
	rp_set_hwnd (hAmigaWnd);
#endif
}

#endif
