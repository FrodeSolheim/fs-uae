#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "uae.h"
#include "xwin.h"
#include "drawing.h"
#include "picasso96_win.h"
#include "win32gfx.h"
#include "gui.h"
#include "gfxfilter.h"
#include "gfxboard.h"
#include "devices.h"

#include "uae/fs.h"
#include <limits.h>
#include <stdlib.h>

#ifdef PICASSO96
#include "picasso96.h"
#endif

#include <fs/emu/video.h>

// #define DEBUG_PICASSO96
// #define DEBUG_SHOW_SCREEN

int max_uae_width = 3072;
int max_uae_height = 2048;

int busywait = 0;
bool beamracer_debug = 0;

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

struct AmigaMonitor AMonitors[MAX_AMIGAMONITORS] = {};
// struct AmigaMonitor *amon = NULL;

static int g_picasso_enabled = 0;
static int g_picasso_width = 0;
static int g_picasso_height = 0;
static int g_picasso_depth = 0;
static int g_picasso_format = 0;

# if 0
static int g_has_flushed_line = 0;
static int g_has_flushed_block = 0;
static int g_has_flushed_screen = 0;
#endif

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

int vsync_activeheight = 0, vsync_totalheight = 0;
float vsync_vblank = 0, vsync_hblank = 0;

static int minimized;
static int monitor_off;

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
		// 640, 360, // 16:9
		// 800, 450, // 16:9
		// 800, 500, // 16:10
		// 672, 540,
		692, 540,
		800, 600,
		960, 540,
		1024, 576, // 16:9
		// 1024, 600, // 16:10
		1024, 768,
		1280, 720, // 16:9
		1280, 800, // 16:10
		// 1344, 1080, // 672 * 2, 540 * 2,
		1384, 1080, // 692 * 2, 540 * 2,
		1920, 1080,
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

static int dx_islost(void)
{
	return 0;
}

bool fs_render_frame(int monid, int mode, bool immediate);

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

#if 0
double vblank_calibrate (double approx_vblank, bool waitonly) {
	UAE_LOG_STUB("");
	return -1;
}
#endif

// FIXME: What is this?
int extraframewait = 0;
int extraframewait2 = 0;
static int frame_missed, frame_counted, frame_errors;
static int frame_usage, frame_usage_avg, frame_usage_total;
//extern int log_vsync;
static bool dooddevenskip;
static volatile frame_time_t vblank_prev_time, thread_vblank_time;
//static bool vblankbaselace;
static int vblankbaselace_chipset;
//static bool vblankthread_oddeven;

int log_vsync = 0, debug_vsync_min_delay = 0, debug_vsync_forced_delay = 0;

#if 0
void vsync_busywait_start(void)
{
	UAE_LOG_STUB("");
	//changevblankthreadmode_fast (VBLANKTH_ACTIVE_START);
	vblank_prev_time = thread_vblank_time;
}

static bool isthreadedvsync (void) {
	return isvsync_chipset () <= -2 || isvsync_rtg () < 0;
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
}

static void vsync_notvblank (void) {
	return;
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
}
#endif

// FIXME
extern "C" {
int fs_ml_get_vblank_count();
}

#if 0
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
#endif

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

#include <fs/emu/hacks.h>

void gfx_lock (void)
{
#if 0
	EnterCriticalSection (&screen_cs);
#endif
}
void gfx_unlock (void)
{
#if 0
	LeaveCriticalSection (&screen_cs);
#endif
}

int WIN32GFX_IsPicassoScreen(struct AmigaMonitor *mon)
{
	return mon->screen_is_picasso ? 1 : 0;
}

static int isscreen(struct AmigaMonitor *mon)
{
	return mon->monitor_id == 0;
}

static void clearscreen (void)
{
	STUB("");
	// FIXME: Schedule a clear (draw black) on both front and back buffers?
}

void desktop_coords (int monid, int *dw, int *dh, int *ax, int *ay, int *aw, int *ah)
{
	STUB("");
#if 0
		struct MultiDisplay *md = getdisplay (&currprefs);

		*dw = md->rect.right - md->rect.left;
		*dh = md->rect.bottom - md->rect.top;
		*ax = amigawin_rect.left;
		*ay = amigawin_rect.top;
		*aw = amigawin_rect.right - *ax;
		*ah = amigawin_rect.bottom - *ay;
#endif
}

int target_get_display (const TCHAR *name)
{
	if (strcmp(name, "Dummy Display") == 0) {
		return 0;
	}
	return -1;
}

int target_get_display_scanline(int displayindex)
{
	return -2;
}

void target_cpu_speed(void)
{
#ifdef FSUAE
	STUB("");
#else
	display_vblank_thread(&AMonitors[0]);
#endif
}

const TCHAR *target_get_display_name (int num, bool friendlyname)
{
	static TCHAR name[] = "Dummy Display";
	return name;
}

void getgfxoffset(int monid, float *dxp, float *dyp, float *mxp, float *myp)
{
	/* Offset and scale factors used for magic mouse (in order to translate
	 * mouse coordinates to Amiga coordinates) */
	*dxp = fs_emu_video_offset_x;
	*dyp = fs_emu_video_offset_y;
	*mxp = fs_emu_video_scale_x;
	*myp = fs_emu_video_scale_y;
}

int isfullscreen (void)
{
	return 0;
}

static void grow_render_buffer(int width, int height)
{
	struct vidbuf_description *vidinfo = &adisplays[0].gfxvidinfo;
	unsigned char *new_pixels = (unsigned char*) g_renderdata.grow(width, height);
	if (new_pixels != g_renderdata.pixels) {
		//printf("new %p old %p\n", new_pixels, g_renderdata.pixels);
		//printf("grow_render_buffer %d %d\n", width, height);
		g_renderdata.pixels = new_pixels;
		vidinfo->drawbuffer.bufmem = new_pixels;
		init_row_map();
		//printf("grow_render_buffer %d %d done\n", width, height);
	}
}

void amiga_set_render_buffer(void *data, int size, int need_redraw,
		void *(*grow)(int width, int height))
{
	struct vidbuf_description *vidinfo = &adisplays[0].gfxvidinfo;

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
	vidinfo->drawbuffer.bufmem = (unsigned char*) data;
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

void gui_fps (int fps, int idle, int color)
{
	UAE_LOG_STUB_MAX(1, "");
}

int gui_update (void)
{
	return 0;
}

#if 0

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

void flush_screen (struct vidbuffer *buffer, int first_line, int last_line)
{
	write_log("flush_screen\n");
	g_has_flushed_screen = 1;
}

#endif

bool render_screen(int monid, int mode, bool immediate)
{
	struct AmigaMonitor *mon = &AMonitors[monid];
	struct amigadisplay *ad = &adisplays[monid];
	bool v = false;
	int cnt;

	render_ok = false;
	if (minimized || ad->picasso_on || monitor_off || dx_islost ())
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
#endif
	gfx_lock();
#ifdef FSUAE
	v = fs_render_frame(monid, mode, immediate);
#else
	if (mon->currentmode.flags & DM_D3D) {
		v = D3D_renderframe(monid, mode, immediate);
	} else if (mon->currentmode.flags & DM_SWSCALE) {
		S2X_render(monid, -1, -1);
		v = true;
	} else if (mon->currentmode.flags & DM_DDRAW) {
		v = true;
	}
#endif
	mon->render_ok = v;
	gfx_unlock();
	return mon->render_ok;
}

bool show_screen_maybe(int monid, bool show)
{
	struct AmigaMonitor *mon = &AMonitors[monid];
	struct amigadisplay *ad = &adisplays[monid];
	struct apmode *ap = ad->picasso_on ? &currprefs.gfx_apmode[1] : &currprefs.gfx_apmode[0];

#ifdef DEBUG_SHOW_SCREEN
	printf("show_screen_maybe monid=%d show=%d (picasso_on=%d)\n",
		   monid, show, ad->picasso_on);
#endif

	if (!ap->gfx_vflip || ap->gfx_vsyncmode == 0 || !ap->gfx_vsync) {
		if (show) {
		//if (show && !picasso_on) {
			show_screen(monid, 0);
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

float target_adjust_vblank_hz(int monid, float hz)
{
#ifdef FSUAE
	/* FIXME: Can make small adjustments to vblank hertz here if rate
	 * almost matches native vblank, but not quite? */
#else
	struct AmigaMonitor *mon = &AMonitors[monid];
	int maxrate;
	if (!currprefs.lightboost_strobo)
		return hz;
	if (isfullscreen() > 0) {
		maxrate = mon->currentmode.freq;
	} else {
		maxrate = deskhz;
	}
	double nhz = hz * 2.0;
	if (nhz >= maxrate - 1 && nhz < maxrate + 1)
		hz -= 0.5;
#endif
	return hz;
}

void show_screen(int monid, int mode)
{
	struct AmigaMonitor *mon = &AMonitors[monid];
	struct amigadisplay *ad = &adisplays[monid];

#ifdef DEBUG_SHOW_SCREEN
	printf("show_screen mode=%d\n\n", mode);
#endif

	if (g_libamiga_callbacks.display) {
		g_libamiga_callbacks.display();
	}
}

int lockscr(struct vidbuffer *vb, bool fullupdate, bool first)
{
	struct AmigaMonitor *mon = &AMonitors[vb->monitor_id];
	int ret = 0;

	if (!isscreen(mon))
		return ret;
#if 0
	flushymin = mon->currentmode.amiga_height;
	flushymax = 0;
#endif
	ret = 1;
#ifdef FSUAE
#else
	if (mon->currentmode.flags & DM_D3D) {
#ifdef D3D
		if (mon->currentmode.flags & DM_SWSCALE) {
			ret = 1;
		} else {
			ret = 0;
			vb->bufmem = D3D_locktexture(vb->monitor_id, &vb->rowbytes, NULL, fullupdate);
			if (vb->bufmem) {
				if (first)
					init_row_map();
				ret = 1;
			}
		}
#endif
	} else if (mon->currentmode.flags & DM_SWSCALE) {
		ret = 1;
	} else if (mon->currentmode.flags & DM_DDRAW) {
		ret = ddraw_dolock() != 0;
	}
#endif
	return ret;
}

void unlockscr(struct vidbuffer *vb, int y_start, int y_end)
{
#ifdef FSUAE
#else
	struct AmigaMonitor *mon = &AMonitors[vb->monitor_id];
	if (mon->currentmode.flags & DM_D3D) {
		if (mon->currentmode.flags & DM_SWSCALE) {
			S2X_render(vb->monitor_id, y_start, y_end);
		} else {
			vb->bufmem = NULL;
		}
		D3D_unlocktexture(vb->monitor_id, y_start, y_end);
	} else if (mon->currentmode.flags & DM_SWSCALE) {
		return;
	} else if (mon->currentmode.flags & DM_DDRAW) {
		DirectDraw_SurfaceUnlock();
		vb->bufmem = NULL;
	}
#endif
}

#if 0
void flush_clear_screen (struct vidbuffer *vb)
{
	STUB("");
}
#endif

static uae_u8 *gfx_lock_picasso2(int monid, bool fullupdate)
{
#ifdef DEBUG_PICASSO96
	printf("gfx_lock_picasso2\n");
#endif
	struct picasso_vidbuf_description *vidinfo = &picasso_vidinfo[monid];
	if (true) { // if (currprefs.gfx_api) {
		int pitch = vidinfo->width * g_amiga_video_bpp;
		// FIXME: Make sure the render buffer is big enough!!
		// FIXME: Can grow here if necessary?
		// Or return NULL or use temp buffer if not?
		// This is currently done in gfx_set_picasso_modeinfo, maybe it should be
		// moved here...
		uae_u8 *p = uae_get_render_buffer();
		vidinfo->rowbytes = pitch;
		vidinfo->rowbytes = pitch;
		return p;
	}
}

uae_u8 *gfx_lock_picasso(int monid, bool fullupdate, bool doclear)
{
#ifdef DEBUG_PICASSO96
	printf("gfx_lock_picasso\n");
#endif
	struct AmigaMonitor *mon = &AMonitors[monid];
	struct picasso_vidbuf_description *vidinfo = &picasso_vidinfo[monid];
	static uae_u8 *p;
	if (mon->rtg_locked) {
		return p;
	}
	gfx_lock();
	p = gfx_lock_picasso2(monid, fullupdate);
	if (!p) {
		gfx_unlock();
	} else {
		mon->rtg_locked = true;
		if (doclear) {
			uae_u8 *p2 = p;
			for (int h = 0; h < vidinfo->height; h++) {
				memset (p2, 0, vidinfo->width * vidinfo->pixbytes);
				p2 += vidinfo->rowbytes;
			}
		}
	}
#ifdef DEBUG_PICASSO96
	printf("gfx_lock_picasso -> %p\n", p);
#endif
	return p;
}

void gfx_unlock_picasso(int monid, bool dorender)
{
#ifdef DEBUG_PICASSO96
	printf("gfx_unlock_picasso monid %d dorender %d\n", monid, dorender);
#endif
	struct AmigaMonitor *mon = &AMonitors[monid];
	struct picasso_vidbuf_description *vidinfo = &picasso_vidinfo[monid];

#if 0
	if (!rtg_locked)
			EnterCriticalSection (&screen_cs);
#endif

	mon->rtg_locked = false;

	if (dorender) {
		//render_screen(1);
		// FIXME: check what mode parameter is supposed to do. 1?
		//show_screen(0);
		if (fs_render_frame(monid, 1, false)) {
			show_screen_maybe(monid, true);
		}
	}
}

static void closeblankwindows (void)
{
	STUB("");
}
static void createblankwindows (void)
{
	STUB("");
}

static void close_hwnds(struct AmigaMonitor *mon)
{
	STUB("");
}

static bool canmatchdepth(void)
{
	return false;
}

static void updatemodes(struct AmigaMonitor *mon)
{
#if 0
	struct uae_filter *usedfilter = mon->usedfilter;
	DWORD flags;

	mon->currentmode.fullfill = 0;
	flags = DM_DDRAW;
	if (isfullscreen () > 0)
		flags |= DM_DX_FULLSCREEN;
	else if (isfullscreen () < 0)
		flags |= DM_W_FULLSCREEN;
#if defined (GFXFILTER)
	if (usedfilter) {
		flags |= DM_SWSCALE;
		if (mon->currentmode.current_depth < 15)
			mon->currentmode.current_depth = 16;
	}
#endif
	if (currprefs.gfx_api) {
		flags |= DM_D3D;
		if (flags & DM_DX_FULLSCREEN) {
			flags &= ~DM_DX_FULLSCREEN;
			flags |= DM_D3D_FULLSCREEN;
		}
		flags &= ~DM_DDRAW;
	}
	mon->currentmode.flags = flags;
	if (flags & DM_SWSCALE)
		mon->currentmode.fullfill = 1;
	if (flags & DM_W_FULLSCREEN) {
		RECT rc = getdisplay(&currprefs, mon->monitor_id)->rect;
		mon->currentmode.native_width = rc.right - rc.left;
		mon->currentmode.native_height = rc.bottom - rc.top;
		mon->currentmode.current_width = mon->currentmode.native_width;
		mon->currentmode.current_height = mon->currentmode.native_height;
	} else {
		mon->currentmode.native_width = mon->currentmode.current_width;
		mon->currentmode.native_height = mon->currentmode.current_height;
	}
#endif
}

static void update_gfxparams(struct AmigaMonitor *mon)
{
	struct picasso96_state_struct *state = &picasso96_state[mon->monitor_id];

	updatewinfsmode(mon->monitor_id, &currprefs);
#ifdef PICASSO96
	mon->currentmode.vsync = 0;
	if (mon->screen_is_picasso) {
		mon->currentmode.current_width = (int)(state->Width * currprefs.rtg_horiz_zoom_mult);
		mon->currentmode.current_height = (int)(state->Height * currprefs.rtg_vert_zoom_mult);
		currprefs.gfx_apmode[1].gfx_interlaced = false;
		if (currprefs.win32_rtgvblankrate == 0) {
			currprefs.gfx_apmode[1].gfx_refreshrate = currprefs.gfx_apmode[0].gfx_refreshrate;
			if (currprefs.gfx_apmode[0].gfx_interlaced) {
				currprefs.gfx_apmode[1].gfx_refreshrate *= 2;
			}
		} else if (currprefs.win32_rtgvblankrate < 0) {
			currprefs.gfx_apmode[1].gfx_refreshrate = 0;
		} else {
			currprefs.gfx_apmode[1].gfx_refreshrate = currprefs.win32_rtgvblankrate;
		}
		if (currprefs.gfx_apmode[1].gfx_vsync)
			mon->currentmode.vsync = 1 + currprefs.gfx_apmode[1].gfx_vsyncmode;
	} else {
#endif
		mon->currentmode.current_width = currprefs.gfx_monitor[mon->monitor_id].gfx_size.width;
		mon->currentmode.current_height = currprefs.gfx_monitor[mon->monitor_id].gfx_size.height;
		if (currprefs.gfx_apmode[0].gfx_vsync)
			mon->currentmode.vsync = 1 + currprefs.gfx_apmode[0].gfx_vsyncmode;
#ifdef PICASSO96
	}
#endif
#if FORCE16BIT
	mon->currentmode.current_depth = 16;
#else
	mon->currentmode.current_depth = currprefs.color_mode < 5 ? 16 : 32;
#endif
	if (mon->screen_is_picasso && canmatchdepth() && isfullscreen () > 0) {
		int pbits = state->BytesPerPixel * 8;
		if (pbits <= 8) {
			if (mon->currentmode.current_depth == 32)
				pbits = 32;
			else
				pbits = 16;
		}
		if (pbits == 24)
			pbits = 32;
		mon->currentmode.current_depth = pbits;
	}
	mon->currentmode.amiga_width = mon->currentmode.current_width;
	mon->currentmode.amiga_height = mon->currentmode.current_height;

	mon->scalepicasso = 0;
	if (mon->screen_is_picasso) {
		if (isfullscreen () < 0) {
			if ((currprefs.gf[1].gfx_filter_autoscale == RTG_MODE_CENTER || currprefs.gf[1].gfx_filter_autoscale == RTG_MODE_SCALE || currprefs.win32_rtgallowscaling) && (state->Width != mon->currentmode.native_width || state->Height != mon->currentmode.native_height))
				mon->scalepicasso = 1;
			if (currprefs.gf[1].gfx_filter_autoscale == RTG_MODE_CENTER)
				mon->scalepicasso = currprefs.gf[1].gfx_filter_autoscale;
			if (!mon->scalepicasso && currprefs.win32_rtgscaleaspectratio)
				mon->scalepicasso = -1;
		} else if (isfullscreen () > 0) {
			if (!canmatchdepth()) { // can't scale to different color depth
				if (mon->currentmode.native_width > state->Width && mon->currentmode.native_height > state->Height) {
					if (currprefs.gf[1].gfx_filter_autoscale)
						mon->scalepicasso = 1;
				}
				if (currprefs.gf[1].gfx_filter_autoscale == RTG_MODE_CENTER)
					mon->scalepicasso = currprefs.gf[1].gfx_filter_autoscale;
				if (!mon->scalepicasso && currprefs.win32_rtgscaleaspectratio)
					mon->scalepicasso = -1;
			}
		} else if (isfullscreen () == 0) {
			if (currprefs.gf[1].gfx_filter_autoscale == RTG_MODE_INTEGER_SCALE) {
				mon->scalepicasso = RTG_MODE_INTEGER_SCALE;
				mon->currentmode.current_width = currprefs.gfx_monitor[mon->monitor_id].gfx_size.width;
				mon->currentmode.current_height = currprefs.gfx_monitor[mon->monitor_id].gfx_size.height;
			} else if (currprefs.gf[1].gfx_filter_autoscale == RTG_MODE_CENTER) {
				if (currprefs.gfx_monitor[mon->monitor_id].gfx_size.width < state->Width || currprefs.gfx_monitor[mon->monitor_id].gfx_size.height < state->Height) {
					if (!currprefs.win32_rtgallowscaling) {
						;
					} else if (currprefs.win32_rtgscaleaspectratio) {
						mon->scalepicasso = -1;
						mon->currentmode.current_width = currprefs.gfx_monitor[mon->monitor_id].gfx_size.width;
						mon->currentmode.current_height = currprefs.gfx_monitor[mon->monitor_id].gfx_size.height;
					}
				} else {
					mon->scalepicasso = 2;
					mon->currentmode.current_width = currprefs.gfx_monitor[mon->monitor_id].gfx_size.width;
					mon->currentmode.current_height = currprefs.gfx_monitor[mon->monitor_id].gfx_size.height;
				}
			} else if (currprefs.gf[1].gfx_filter_autoscale == RTG_MODE_SCALE) {
				if (currprefs.gfx_monitor[mon->monitor_id].gfx_size.width > state->Width || currprefs.gfx_monitor[mon->monitor_id].gfx_size.height > state->Height)
					mon->scalepicasso = 1;
				if ((currprefs.gfx_monitor[mon->monitor_id].gfx_size.width != state->Width || currprefs.gfx_monitor[mon->monitor_id].gfx_size.height != state->Height) && currprefs.win32_rtgallowscaling) {
					mon->scalepicasso = 1;
				} else if (currprefs.gfx_monitor[mon->monitor_id].gfx_size.width < state->Width || currprefs.gfx_monitor[mon->monitor_id].gfx_size.height < state->Height) {
					// no always scaling and smaller? Back to normal size and set new configured max size
					mon->currentmode.current_width = changed_prefs.gfx_monitor[mon->monitor_id].gfx_size_win.width = state->Width;
					mon->currentmode.current_height = changed_prefs.gfx_monitor[mon->monitor_id].gfx_size_win.height = state->Height;
				} else if (currprefs.gfx_monitor[mon->monitor_id].gfx_size.width == state->Width || currprefs.gfx_monitor[mon->monitor_id].gfx_size.height == state->Height) {
					;
				} else if (!mon->scalepicasso && currprefs.win32_rtgscaleaspectratio) {
					mon->scalepicasso = -1;
				}
			} else {
				if ((currprefs.gfx_monitor[mon->monitor_id].gfx_size.width != state->Width || currprefs.gfx_monitor[mon->monitor_id].gfx_size.height != state->Height) && currprefs.win32_rtgallowscaling)
					mon->scalepicasso = 1;
				if (!mon->scalepicasso && currprefs.win32_rtgscaleaspectratio)
					mon->scalepicasso = -1;
			}
		}

		if (mon->scalepicasso > 0 && (currprefs.gfx_monitor[mon->monitor_id].gfx_size.width != state->Width || currprefs.gfx_monitor[mon->monitor_id].gfx_size.height != state->Height)) {
			mon->currentmode.current_width = currprefs.gfx_monitor[mon->monitor_id].gfx_size.width;
			mon->currentmode.current_height = currprefs.gfx_monitor[mon->monitor_id].gfx_size.height;
		}
	}
}

static int open_windows(struct AmigaMonitor *mon, bool mousecapture, bool started)
{
	bool recapture = false;
	int ret;
#if 0
	mon->screen_is_initialized = 0;
#endif

	updatewinfsmode(mon->monitor_id, &currprefs);
#if 0
	int init_round = 0;
	ret = -2;
	do {
		if (ret < -1) {
			updatemodes(mon);
			update_gfxparams(mon);
		}
		ret = doInit(mon);
		init_round++;
		if (ret < -9) {
			DirectDraw_Release();
			if (!DirectDraw_Start())
				return 0;
		}
	} while (ret < 0);
#endif
	ret = 0;
	return ret;
}

void graphics_reset(bool force)
{
	LOG_STUB("force=%d", force);
}

int check_prefs_changed_gfx (void)
{
	//write_log("check_prefs_changed_gfx\n");
	return 0;
}

static int init_colors (int monid)
{
	write_log("init_colors\n");
	bool use_yuv = false;
	alloc_colors64k(monid, g_red_bits, g_green_bits, g_blue_bits, g_red_shift,
			g_green_shift, g_blue_shift, 0, 0, 0, 0, use_yuv);
	return 1;
}

int picasso_palette(struct MyCLUTEntry *CLUT, uae_u32 *clut)
{
	int changed = 0;

	for (int i = 0; i < 256; i++) {
		int r = CLUT[i].Red;
		int g = CLUT[i].Green;
		int b = CLUT[i].Blue;
		uae_u32 v = (doMask256 (r, g_red_bits, g_red_shift)
			| doMask256 (g, g_green_bits, g_green_shift)
			| doMask256 (b, g_blue_bits, g_blue_shift))
			| doMask256 (0xff, g_alpha_bits, g_alpha_shift);
		if (v != clut[i]) {
			//write_log (_T("%d:%08x\n"), i, v);
			clut[i] = v;
			changed = 1;
		}
	}
	return changed;
}

static void open_screen(struct AmigaMonitor *mon)
{
	STUB("");
	close_windows(mon);
	open_windows(mon, true, true);
}

bool vsync_switchmode(int monid, int hz)
{
	STUB("monid=%d mhz=%d", monid, hz);
	return 0;
}

void vsync_clear(void)
{
	// vsync_active = false;
}

// FIXME
int vsync_isdone(frame_time_t *dt)
{
	if (isvsync() == 0)
		return -1;
	// if (dt)
	//	*dt = wait_vblank_timestamp;
	// return vsync_active ? 1 : 0;
	return 1;
	// FIXME ??
	// return vblank_found_chipset || dooddevenskip;
}

static int modeswitchneeded(struct AmigaMonitor *mon, struct winuae_currentmode *wc)
{
	return 0;
}

void gfx_set_picasso_state(int monid, int on)
{
	write_log(_T("gfx_set_picasso_state monid=%d on=%d\n"), monid, on);
	g_picasso_enabled = (on != 0);

	struct AmigaMonitor *mon = &AMonitors[monid];
	struct winuae_currentmode wc;
	struct apmode *newmode, *oldmode;
	struct gfx_filterdata *newf, *oldf;
	int mode;

	if (mon->screen_is_picasso == on)
		return;
	mon->screen_is_picasso = on;
	// rp_rtg_switch ();
	memcpy (&wc, &mon->currentmode, sizeof (wc));

	newmode = &currprefs.gfx_apmode[on ? 1 : 0];
	oldmode = &currprefs.gfx_apmode[on ? 0 : 1];

	newf = &currprefs.gf[on ? 1 : 0];
	oldf = &currprefs.gf[on ? 0 : 1];

	updatemodes(mon);
	update_gfxparams(mon);
	// clearscreen();
}

void gfx_set_picasso_modeinfo(int monid, RGBFTYPE rgbfmt)
{
	struct AmigaMonitor *mon = &AMonitors[monid];
	int need;
#ifdef DEBUG_PICASSO96
	write_log(_T("gfx_set_picasso_modeinfo rgbfmt=%d mon->screen_is_picasso=%d\n"),
			  rgbfmt, mon->screen_is_picasso);
#endif

#ifdef DEBUG_PICASSO96
	if (!mon->screen_is_picasso) {
		printf("!mon->screen_is_picasso, RETURNING\n");
	}
#endif

	if (!mon->screen_is_picasso)
		return;

	g_picasso_format = rgbfmt;

#ifdef DEBUG_PICASSO96
	write_log("====================== rgbfmt = %d\n ===============\n", g_picasso_format);
#endif

	// FIXME: Clean up mess

	struct picasso_vidbuf_description *vidinfo = &picasso_vidinfo[monid];
	write_log("gfx_set_picasso_modeinfo %dx%d:%d rgbfmt=%d/%d\n",
	vidinfo->width, vidinfo->height, vidinfo->depth, vidinfo->selected_rgbformat,
	rgbfmt);
#if 1
	g_picasso_width = vidinfo->width;
	g_picasso_height = vidinfo->height;
	g_picasso_depth = vidinfo->depth;
	g_picasso_format = vidinfo->selected_rgbformat;
/*
	g_picasso_width = picasso96_state_uaegfx.Width;
	g_picasso_height = picasso96_state_uaegfx.Height;
	g_picasso_depth = picasso96_state_uaegfx.GC_Depth;
	g_picasso_format = picasso96_state_uaegfx.RGBFormat;
	*/
/*
	if((picasso_vidinfo.width == ) &&
		(picasso_vidinfo.height == ) &&
		(picasso_vidinfo.depth == ( >> 3)) &&
		(picasso_vidinfo.selected_rgbformat == ))
		*/
#else
	g_picasso_width = w;
	g_picasso_height = h;
	g_picasso_depth = depth;
	g_picasso_format = rgbfmt;
#endif

	clearscreen();
	gfx_set_picasso_colors(monid, rgbfmt);
	updatemodes(mon);
	need = modeswitchneeded(mon, &mon->currentmode);
	update_gfxparams(mon);
	if (need > 0) {
		open_screen(mon);
	} else if (need < 0) {
		open_windows(mon, true, true);
	}
#ifdef RETROPLATFORM
	rp_set_hwnd(mon->hAmigaWnd);
#endif

	// register largest width seen, so render buffers can be adjusted if
	// necessary
	if (g_picasso_width > g_largest_width) {
		g_largest_width = g_picasso_width;
	}
	if (g_picasso_height > g_largest_height) {
		g_largest_height = g_picasso_height;
	}
	grow_render_buffer(g_largest_width, g_largest_height);

	vidinfo->width = g_picasso_width;
	vidinfo->height = g_picasso_height;
}

void gfx_set_picasso_colors(int monid, RGBFTYPE rgbfmt)
{
#ifdef DEBUG_PICASSO96
	write_log(_T("gfx_set_picasso_colors monid=%d rgbfmt=%d\n"), monid, rgbfmt);
#endif
	alloc_colors_picasso(g_red_bits, g_green_bits, g_blue_bits, g_red_shift,
			g_green_shift, g_blue_shift, rgbfmt, p96_rgbx16);
}

int machdep_init(void)
{
	for (int i = 0; i < MAX_AMIGAMONITORS; i++) {
		struct AmigaMonitor *mon = &AMonitors[i];
		struct amigadisplay *ad = &adisplays[i];
		mon->monitor_id = i;
		ad->picasso_requested_on = 0;
		ad->picasso_on = 0;
		mon->screen_is_picasso = 0;
		memset(&mon->currentmode, 0, sizeof(*&mon->currentmode));
	}
#ifdef LOGITECHLCD
	lcd_open ();
#endif

	// We call libamiga_callbacks.init here because machdep_init is called
	// very early in real_main2, just after default configuration and before
	// it is too late to change the configuration

	built_in_prefs(&currprefs, 1, 0, 0, 0);

	if (g_libamiga_callbacks.init) {
		g_libamiga_callbacks.init();
	}

	return 1;
}

void machdep_free(void)
{
#ifdef LOGITECHLCD
	lcd_close ();
#endif
}

int graphics_init(bool mousecapture)
{
	write_log("graphics_init\n");

	int monid = 0;
	struct AmigaMonitor *mon = &AMonitors[monid];
	struct amigadisplay *ad = &adisplays[monid];
	struct vidbuf_description *avidinfo = &adisplays[0].gfxvidinfo;

	// FIXME: perhaps modify so custom_limits defaults to -1, -1, -1, -1
	set_custom_limits (-1, -1, -1, -1);

	if (g_amiga_video_format == AMIGA_VIDEO_FORMAT_R5G6B5) {
		currprefs.color_mode = 2;
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
		currprefs.color_mode = 2;
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
		currprefs.color_mode = 5;
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
	// FIXME: ??
	// avidinfo->maxblocklines = 0;
#ifdef USE_BUFMEM
	g_bufmem = (unsigned char*) malloc(AMIGA_WIDTH * AMIGA_HEIGHT * g_amiga_video_bpp);
	avidinfo->drawbuffer.bufmem = g_bufmem;
	memset(g_bufmem, 0, AMIGA_WIDTH * AMIGA_HEIGHT * g_amiga_video_bpp);
	avidinfo->maxblocklines = MAXBLOCKLINES_MAX;
#endif

#ifdef USE_LINEMEM
	avidinfo->drawbuffer.emergmem = 0;
	avidinfo->drawbuffer.linemem = g_linemem;
#else
	avidinfo->drawbuffer.emergmem = 0; //g_linemem;
	avidinfo->drawbuffer.linemem = 0;
#endif
	avidinfo->drawbuffer.pixbytes = g_amiga_video_bpp;
	avidinfo->drawbuffer.rowbytes = AMIGA_WIDTH * g_amiga_video_bpp;
	avidinfo->drawbuffer.height_allocated = AMIGA_HEIGHT;
	avidinfo->drawbuffer.inheight = AMIGA_HEIGHT;
	avidinfo->drawbuffer.outheight = AMIGA_HEIGHT;
	avidinfo->drawbuffer.width_allocated = AMIGA_WIDTH;
	avidinfo->drawbuffer.inwidth = AMIGA_WIDTH;
	avidinfo->drawbuffer.outwidth = AMIGA_WIDTH;

	//gfxvidinfo.flush_block = libamiga_flush_block;
	//gfxvidinfo.flush_screen = libamiga_flush_screen;
	//SDL_SetColors (display, arSDLColors, 0, 256);
	write_log("calling reset_drawing\n");
	reset_drawing ();
	init_colors(monid);

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

int graphics_setup(void)
{
#ifdef FSUAE
	write_log("graphics_setup\n");
#else
	if (!screen_cs_allocated) {
		InitializeCriticalSection(&screen_cs);
		screen_cs_allocated = true;
	}
#endif
#ifdef PICASSO96
	InitPicasso96(0);
#endif
	return 1;
}

void graphics_leave (void)
{
#ifdef FSUAE
	STUB("");
#else
	for (int i = 0; i < MAX_AMIGAMONITORS; i++) {
		close_windows(&AMonitors[i]);
	}
#endif
}

void close_windows(struct AmigaMonitor *mon)
{
	STUB("");
}

float target_getcurrentvblankrate(int monid)
{
	UAE_LOG_STUB("");
	if (remembered_vblank) {
		return remembered_vblank;
	}
	return 50;
}

static int oldtex_w, oldtex_h, oldtex_rtg;

bool target_graphics_buffer_update(int monid)
{
	// FS-UAE:

	write_log("target_graphics_buffer_update - clearing buffer\n");
	memset(g_renderdata.pixels, 0, \
			AMIGA_WIDTH * AMIGA_HEIGHT * g_amiga_video_bpp);
	memset(g_renderdata.line, 0, AMIGA_MAX_LINES);

	// WinUAE:

	struct AmigaMonitor *mon = &AMonitors[monid];
	struct picasso_vidbuf_description *vidinfo = &picasso_vidinfo[monid];
	struct vidbuf_description *avidinfo = &adisplays[0].gfxvidinfo;
	struct picasso96_state_struct *state = &picasso96_state[monid];

	static bool	graphicsbuffer_retry;
	int w, h;
	
	graphicsbuffer_retry = false;
	if (mon->screen_is_picasso) {
		w = state->Width > vidinfo->width ? state->Width : vidinfo->width;
		h = state->Height > vidinfo->height ? state->Height : vidinfo->height;
#ifdef FSUAE
		w = state->Width;
		h = state->Height;
		vidinfo->width = w;
		vidinfo->height = w;
		g_picasso_width = w;
		g_picasso_height = h;
#endif
	} else {
		struct vidbuffer *vb = avidinfo->drawbuffer.tempbufferinuse ? &avidinfo->tempbuffer : &avidinfo->drawbuffer;
		avidinfo->outbuffer = vb;
		w = vb->outwidth;
		h = vb->outheight;
		printf("outwidth/height = %dx%d\n", w, h);
	}
	
	if (oldtex_w == w && oldtex_h == h && oldtex_rtg == mon->screen_is_picasso)
		return false;

	if (!w || !h) {
		oldtex_w = w;
		oldtex_h = h;
		oldtex_rtg = mon->screen_is_picasso;
		return false;
	}

#if 0
	S2X_free(mon->monitor_id);
	if (mon->currentmode.flags & DM_D3D) {
		if (!D3D_alloctexture(mon->monitor_id, w, h)) {
			graphicsbuffer_retry = true;
			return false;
		}
	} else {
		DirectDraw_ClearSurface (NULL);
	}
#endif

	oldtex_w = w;
	oldtex_h = h;
	oldtex_rtg = mon->screen_is_picasso;

	write_log (_T("Buffer size (%d*%d) %s\n"), w, h, mon->screen_is_picasso ? _T("RTG") : _T("Native"));
#if 0
	if ((mon->currentmode.flags & DM_SWSCALE) && !mon->screen_is_picasso) {
		if (!S2X_init(mon->monitor_id, mon->currentmode.native_width, mon->currentmode.native_height, mon->currentmode.native_depth))
			return false;
	}
#endif
	return true;
}

void updatewinfsmode(int monid, struct uae_prefs *p)
{
	STUB("");
#if 0
	struct MultiDisplay *md;

	fixup_prefs_dimensions (p);
	if (isfullscreen_2 (p) != 0) {
		p->gfx_monitor[monid].gfx_size = p->gfx_monitor[monid].gfx_size_fs;
	} else {
		p->gfx_monitor[monid].gfx_size = p->gfx_monitor[monid].gfx_size_win;
	}
	md = getdisplay(p, monid);
	set_config_changed ();
#endif
}

int rtg_index = -1;

// -2 = default
// -1 = prev
// 0 = chipset
// 1..4 = rtg
// 5 = next

bool toggle_rtg (int monid, int mode)
{
#ifdef DEBUG_PICASSO96
	printf("toggle_rtg monid=%d mode=%d\n", monid, mode);
#endif
	struct amigadisplay *ad = &adisplays[monid];

	int old_index = rtg_index;

	if (monid > 0) {
		return true;
	}

	if (mode < -1 && rtg_index >= 0)
		return true;

	for (;;) {
		if (mode == -1) {
			rtg_index--;
		} else if (mode >= 0 && mode <= MAX_RTG_BOARDS) {
			rtg_index = mode - 1;
		} else {
			rtg_index++;
		}
		if (rtg_index >= MAX_RTG_BOARDS) {
			rtg_index = -1;
		} else if (rtg_index < -1) {
			rtg_index = MAX_RTG_BOARDS - 1;
		}
		if (rtg_index < 0) {
			if (ad->picasso_on) {
				gfxboard_rtg_disable(monid, old_index);
				ad->picasso_requested_on = false;
				// statusline_add_message(STATUSTYPE_DISPLAY, _T("Chipset display"));
				set_config_changed();
				return false;
			}
			return false;
		}
		struct rtgboardconfig *r = &currprefs.rtgboards[rtg_index];
		if (r->rtgmem_size > 0 && r->monitor_id == monid) {
			if (r->rtgmem_type >= GFXBOARD_HARDWARE) {
				int idx = gfxboard_toggle(r->monitor_id, rtg_index, mode >= -1);
				if (idx >= 0) {
					rtg_index = idx;
					return true;
				}
				if (idx < -1) {
					rtg_index = -1;
					return false;
				}
			} else {
				gfxboard_toggle(r->monitor_id, -1, -1);
				if (mode < -1)
					return true;
				devices_unsafeperiod();
				gfxboard_rtg_disable(monid, old_index);
				// can always switch from RTG to custom
				if (ad->picasso_requested_on && ad->picasso_on) {
					ad->picasso_requested_on = false;
					rtg_index = -1;
					set_config_changed();
					return true;
				}
				if (ad->picasso_on)
					return false;
				// can only switch from custom to RTG if there is some mode active
				if (picasso_is_active(r->monitor_id)) {
					picasso_enablescreen(r->monitor_id, 1);
					ad->picasso_requested_on = true;
					// statusline_add_message(STATUSTYPE_DISPLAY, _T("RTG %d: %s"), rtg_index + 1, _T("UAEGFX"));
					set_config_changed();
					return true;
				}
			}
		}
		if (mode >= 0 && mode <= MAX_RTG_BOARDS) {
			rtg_index = old_index;
			return false;
		}
	}
	return false;

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

void close_rtg(int monid)
{
	STUB("");
}

void toggle_fullscreen(int monid, int mode)
{
	STUB("");
}

bool fs_render_frame(int monid, int mode, bool immediate)
{
	struct AmigaMonitor *mon = &AMonitors[monid];
	struct amigadisplay *ad = &adisplays[monid];
	struct vidbuf_description *avidinfo = &adisplays[0].gfxvidinfo;
	// FIXME: immediate is a new parameter
	// FIXME: mode is a new parameter

#if 0
	//write_log("render_screen line: %d block %d screen %d\n",
	//        g_has_flushed_line, g_has_flushed_block, g_has_flushed_screen);
	int flushed = g_has_flushed_line || g_has_flushed_block ||
			g_has_flushed_screen;
#endif

	g_renderdata.bpp = g_amiga_video_bpp;
	// printf("%d %d %d\n", g_amiga_video_bpp, AMIGA_WIDTH, AMIGA_HEIGHT);

	static int cx, cy, cw, ch, crealh;
#if 0
	printf("g_picasso_enabled %d\n", g_picasso_enabled);
#endif
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
		if (avidinfo->outbuffer) {
			// if gfxvidinfo.outbuffer is not set, get_custom_limits will
			// crash
			// if (flushed) {
			if (true) {
				get_custom_limits(&cw, &ch, &cx, &cy, &crealh);
				// FIXME: crealh is new - find out what it does
			} else {
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

	// FIXME: Need to do this right now to fix rendering, this
	// causes some slowdown, most likely
	memset(g_renderdata.line, 0, AMIGA_MAX_LINES);

	if (g_libamiga_callbacks.render) {
#if 0
		printf("rendering with %p\n", g_renderdata.pixels);
		printf("%dx%d (flags 0x%x)\n", g_renderdata.width, g_renderdata.height, g_renderdata.flags);
#endif
		g_libamiga_callbacks.render(&g_renderdata);
	}

#if 0
	g_has_flushed_line = 0;
	g_has_flushed_block = 0;
	g_has_flushed_screen = 0;
#endif

	if (fse_drivers()) {
		notice_screen_contents_lost(monid);
	} else {
		// FIXME: Need to do this right now to fix rendering, this
		// causes some slowdown, most likely
		notice_screen_contents_lost(monid);
	}
	return 1;
}
