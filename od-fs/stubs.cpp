#include "sysconfig.h"
#include "sysdeps.h"

#include "autoconf.h"
#include "debug.h"
#include "disk.h"
#include "drawing.h"
#include "enforcer.h"
#include "ethernet.h"
#include "filesys.h"
#include "fsdb.h"
#include "gui.h"
#include "inputdevice.h"
#include "newcpu.h"
#include "options.h"
#include "rommgr.h"
#include "sampler.h"
#include "sana2.h"
#include "savestate.h"
#include "scsidev.h"
#include "uae.h"
#include "xwin.h"

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

uae_u8 *save_log(int bootlog, size_t *len)
{
    UAE_LOG_STUB("");
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

void screenshot(int monid, int mode, int doprepare) {
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

// Cfgfile callback to save target-specific options to config file
void target_save_options (struct zfile *f, struct uae_prefs *p) {
    // LOG_STUB("zfile=%p p=%p", f, p);
}

int target_parse_option(struct uae_prefs *p, const TCHAR *option, const TCHAR *value, int type)
{
    STUB("p=%p\n, option=\"%s\"", p, option);
    return 0;
}

uae_u8 *target_load_keyfile (struct uae_prefs *p, const TCHAR *path, int *sizep, TCHAR *name) {
    STUB("");
    return NULL;
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

void filesys_addexternals(void) {
    LOG_STUB("");
}

void target_run (void) {
    LOG_STUB("");
#if 0
	shellexecute (currprefs.win32_commandpathstart);
#endif
}

void target_restart (void) {
    STUB("");
}

void target_quit (void)
{
	uae_log("target_quit\n");
#if 0
	shellexecute (currprefs.win32_commandpathend);
#endif
}

// FIXME: No longer a stub
void target_fixup_options(struct uae_prefs *p)
{
    uae_log("target_fixup_options\n");
    // We set gfx_api here so we can keep some if-statement code similar
    // to WinUAE (in WinUAE gfx_api non-null means Direct3D, as opposed to
    // older DirectDraw code). FS-UAE code looks more similar to Direct3D
    // code.
    uae_log("Set gfx_api = 1\n");
    p->gfx_api = 1;
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

void target_inputdevice_unacquire(bool full)
{
    STUB("");
}

void target_inputdevice_acquire(void)
{
    STUB("");
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

uae_u8 *save_screenshot(int monid, size_t *len)
{
    STUB("");
    return NULL;
}

/* Stub function for avioutput */
bool frame_drawn (int monid)
{
    return false;
}

void systray (HWND hwnd, int remove)
{

}

#include "uae/compat/windows2.h"
#include "../od-win32/registry.h"

int regsetint(UAEREG *root, const TCHAR *name, int val)
{
    return 0;
}