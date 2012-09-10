#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "scsidev.h"
//#include <memory.h>
#include <glib.h>

void clipboard_vsync (void) {

}

void gui_flicker_led (int, int, int) {

}

void gui_lock (void) {

}

void gui_unlock (void) {

}

void gui_display (int shortcut) {
    STUB("shortcut=%d", shortcut);
}

int gui_init() {
    STUB("");
    return 1;
}

void gui_exit() {
    STUB("");
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

void notify_user (int msg) {
    STUB("msg=%d", msg);
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

void target_save_options (struct zfile *f, struct uae_prefs *p) {
    STUB("zfile=? p=%p", p);
}

int target_parse_option (struct uae_prefs *p, const TCHAR *option, const TCHAR *value) {
    STUB("p=%p\n, option=\"%s\"", p, option);
    return 0;
}

void target_startup_sequence (struct uae_prefs *p) {
    STUB("p=%p\n", p);
}

void notify_user_parms (int msg, const TCHAR *parms, ...) {
    STUB("msg=%d parms=\"%s\"", msg, parms);
}

void target_reset (void) {
    STUB("");
}

uae_u8 *target_load_keyfile (struct uae_prefs *p, const TCHAR *path, int *sizep, TCHAR *name) {
    STUB("");
    return NULL;
}

bool vsync_switchmode (int hz) {
    STUB("hz=%d", hz);
    return 0;
}

void ahi_hsync (void) {
    VERBOSE_STUB("");
}

int enforcer_disable(void) {
    STUB("");
    return 1;
}

void machdep_free (void) {
    STUB("");
}

void target_run (void) {
    STUB("");
}

void target_quit (void) {
    STUB("");
}

void target_fixup_options (struct uae_prefs *p) {
    STUB("");
}

int debuggable (void) {
    return 0;
}

void logging_init(void) {
    STUB("");
}

void flush_log(void) {
    STUB("");
}

int translate_message (int msg, TCHAR *out) {
    STUB("");
    return 0;
}

void machdep_save_options (FILE *f, const struct uae_prefs *p) {
    STUB("");
}

int machdep_parse_option (struct uae_prefs *p, const char *option, const char *value) {
    STUB("");
    return 0;
}

void machdep_default_options (struct uae_prefs *p) {
    STUB("");
}
