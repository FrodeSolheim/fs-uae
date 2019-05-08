#include "sysconfig.h"
#include "sysdeps.h"

#include "autoconf.h"
#include "debug.h"
#include "enforcer.h"
#include "filesys.h"
#include "fsdb.h"
#include "gui.h"
#include "newcpu.h"
#include "options.h"
#include "rommgr.h"
#include "sampler.h"
#include "savestate.h"
#include "scsidev.h"
#include "uae.h"
#include "xwin.h"
#include "sana2.h"
#include "ethernet.h"

void graphics_reset(bool force)
{
    LOG_STUB("force=%d", force);
}

void gui_lock (void)
{

}

void gui_unlock (void)
{

}

void gui_display (int shortcut)
{
    STUB("shortcut=%d", shortcut);
}

int gui_init()
{
    LOG_STUB("");
    return 1;
}

void gui_exit()
{
    LOG_STUB("");
}

int get_guid_target (uae_u8 *out) {
    return 0;
}

uae_u8 *save_log (int bootlog, int *len) {
    STUB("");
    return NULL;
}

int GetDriveType(TCHAR* vol) {
    // FIXME:
    return 0;
}

uae_u32 emulib_target_getcpurate (uae_u32 v, uae_u32 *low) {
    STUB("v=%d", v);
    // FIXME:
    return 0;
}

void update_debug_info(void) {
    // used to update debug info in debugger UI , currently FS only supports
    // using console debugging on Linux/Mac OS X (The Windows version is
    // not a console application).
}

void debugger_change (int mode) {
    STUB("mode=%d", mode);
}

void screenshot (int mode, int doprepare) {
    STUB("mode=%d doprepare=%d", mode, doprepare);
}

void write_dlog (const TCHAR *format, ...) {
    STUB("format=\"%s\"", format);
}

void target_addtorecent (const TCHAR *name, int t) {
    STUB("name=\"%s\" t=%d", name, t);
}

uae_u8 sampler_getsample (int) {
    return 0;
}

int sampler_init (void) {
    return 0;
}

void sampler_free (void) {
}

void sampler_vsync (void) {
}

#include "include/zfile.h"

// --- win32gui.cpp ---

int target_cfgfile_load (struct uae_prefs *p, const TCHAR *filename, int type, int isdefault)
{
    LOG_STUB("");
    return 1;
}

void target_save_options (struct zfile *f, struct uae_prefs *p) {
    LOG_STUB("zfile=%p p=%p", f, p);
}

int target_parse_option (struct uae_prefs *p, const TCHAR *option, const TCHAR *value) {
    STUB("p=%p\n, option=\"%s\"", p, option);
    return 0;
}

uae_u8 *target_load_keyfile (struct uae_prefs *p, const TCHAR *path, int *sizep, TCHAR *name) {
    STUB("");
    return NULL;
}

bool vsync_switchmode (int hz) {
    STUB("hz=%d", hz);
    return 0;
}

#ifndef AHI

#include "uae/ahi.h"

void ahi_hsync (void)
{
    VERBOSE_STUB("");
}

int enforcer_disable(void)
{
    STUB("");
    return 1;
}

#endif

void refreshtitle (void) {
    STUB("");
}

void updatedisplayarea (void) {
    LOG_STUB("");
}

void filesys_addexternals(void) {
    LOG_STUB("");
}

void machdep_free (void) {
    LOG_STUB("");
}

void target_run (void) {
    LOG_STUB("");
}

void target_reset (void) {
    LOG_STUB("");
}

void target_restart (void) {
    STUB("");
}

void target_quit (void)
{
    write_log("UAE: Stopping\n");
    printf("UAE: Stopping\n");
}

void target_fixup_options (struct uae_prefs *p) {
    LOG_STUB("");
}

int debuggable (void) {
    return 0;
}

void logging_init(void) {
    LOG_STUB("");
}

void flush_log(void) {
    STUB("");
}

#if 0
void machdep_save_options (FILE *f, const struct uae_prefs *p) {
    LOG_STUB("");
}

int machdep_parse_option (struct uae_prefs *p, const char *option, const char *value) {
    STUB("");
    return 0;
}

void machdep_default_options (struct uae_prefs *p) {
    STUB("");
}
#endif

//void fpux_save (int *v) {
//    STUB("");
//}

void fpux_restore (int *v) {
    LOG_STUB("");
}

bool my_issamepath(const TCHAR *path1, const TCHAR *path2)
{
        LOG_STUB_MAX(3, "");
        return false;
}

static struct netdriverdata *ndd[MAX_TOTAL_NET_DEVICES + 1];
static int net_enumerated;

struct netdriverdata **target_ethernet_enumerate(void)
{
	STUB("");
	if (net_enumerated)
		return ndd;
	ethernet_enumerate(ndd, 0);
	net_enumerated = 1;
	return ndd;
}

bool gui_ask_disk(int drv, TCHAR *name)
{
    STUB("");
    return false;
}

void target_inputdevice_unacquire(void)
{
    STUB("");
}
void target_inputdevice_acquire(void)
{
    STUB("");
}

void desktop_coords (int *dw, int *dh, int *ax, int *ay, int *aw, int *ah)
{
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

int is_touch_lightpen(void) {
    return 0;
}

void target_getdate(int *y, int *m, int *d)
{
    STUB("");
    *y = 2019;
    *m = 5;
    *d = 8;
}
