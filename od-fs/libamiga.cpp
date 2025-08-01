#include "sysconfig.h"
#include "sysdeps.h"

#include <string.h>

#include "autoconf.h"
#include "blkdev.h"
#include "clipboard.h"
#include "custom.h"
#include "disk.h"
#include "events.h"
#include "fs/filesys.h"
#include "fsemu-glib.h"
// #include "fsemu-mutex.h"
#include "gui.h"
#include "inputdevice.h"
#include "keyboard.h"
#include "luascript.h"
#include "options.h"
#include "uae.h"
#include "uae/fs.h"
#include "uae/glib.h"
#include "uae/log.h"
#include "uae/memory.h"
#include "uae/time.h"
#include "xwin.h"

#include "win32gfx.h"

static struct {
    // GMutex *mutex;
    GAsyncQueue *main_queue;
} module;

void keyboard_settrans(void);
libamiga_callbacks g_libamiga_callbacks = {};
log_function g_amiga_gui_message_function = NULL;
amiga_media_function g_amiga_media_function = NULL;

int g_uae_deterministic_mode = 0;
int g_amiga_paused = 0;
bool g_fs_uae_jit_compiler;
int g_amiga_savestate_docompress = 1;

#ifdef DEBUG_SYNC
FILE *g_fs_uae_sync_debug_file = NULL;
#endif

int g_amiga_video_format = AMIGA_VIDEO_FORMAT_RGBA;
int g_amiga_video_bpp = 4;

static char *g_floppy_sounds_dir;

int g_fs_uae_writable_disk_images = 0;

// This is called from the main UAE thread to inform the GUI that a floppy
// disk has been inserted or ejected.

void gui_filename(int num, const char *name)
{
    if (g_amiga_media_function) {
        g_amiga_media_function(num, name);
    }
}

uae_callback_function *uae_on_save_state_finished = NULL;
uae_callback_function *uae_on_restore_state_finished = NULL;
uae_callback_function *uae_on_update_leds = NULL;

extern "C" {

#if 0
int amiga_set_min_first_line(int line, int ntsc)
{
    if (line < 0 || line > 100) {
        return 1;
    }
    if (ntsc) {
        g_uae_min_first_line_ntsc = line;
    }
    else {
        g_uae_min_first_line_pal = line;
    }
    return 0;
}
#endif

int amiga_get_vsync_counter()
{
    return g_uae_vsync_counter;
}

void amiga_set_vsync_counter(int vsync_counter)
{
    g_uae_vsync_counter = vsync_counter;
}

void amiga_on_restore_state_finished(amiga_callback_function *function)
{
    uae_on_restore_state_finished = function;
}

void amiga_on_save_state_finished(amiga_callback_function *function)
{
    uae_on_save_state_finished = function;
}

void amiga_set_save_state_compression(int compress)
{
    g_amiga_savestate_docompress = compress ? 1 : 0;
}

#ifdef WITH_LUA

void amiga_init_lua(void (*lock)(void), void (*unlock)(void))
{
    // uae_lua_init(lock, unlock);
    write_log("WARNING: not sending lock function to uae_lua_init\n");
    uae_lua_init();
}

void amiga_init_lua_state(lua_State *L)
{
    uae_lua_init_state(L);
}

#endif

void amiga_set_floppy_sounds_dir(const char *path)
{
    int len = strlen(path);
    if (path[len - 1] == '/') {
        g_floppy_sounds_dir = g_strdup(path);
    } else {
        // must have directory separator at the end
        g_floppy_sounds_dir = g_strconcat(path, "/", NULL);
    }
}

void amiga_set_media_function(amiga_media_function function)
{
    g_amiga_media_function = function;
}

void amiga_floppy_set_writable_images(int writable)
{
    g_fs_uae_writable_disk_images = writable;
}

#include "fsemu-time.h"
#include "fsemu-types.h"

int amiga_init(void)
{
    uae_log("Initializing core derived from %s\n", UAE_BASE_VERSION);

#if 0
    int64_t now_us = fsemu_time_us();
	for (int i = 0; i < 1000000; i++) {
		uaerand();
	}
    printf("RNG benchmark: %lld\n", fsemu_lld(fsemu_time_us() - now_us));
#endif

    module.main_queue = g_async_queue_new();

    uae_register_main_thread();
    uae_time_init();

    /*
#ifdef DEBUG_SYNC
    g_sync_debug_file = fopen("sync.log", "wb");
#endif
    */

    // clock sync base is 1000000 (microseconds)
    syncbase = 1000000;

    // clock sync base is 10000 (tenths of milliseconds)
    // syncbase = 10000;

    filesys_host_init();

    romlist_init();
#ifdef WITH_CLIPBOARD
    clipboard_init();
#endif
    return 1;
}

bool amiga_init_jit_compiler(void)
{
    write_log("JIT: Enabling JIT compiler\n");
    g_fs_uae_jit_compiler = true;
    return true;
}

void amiga_set_video_format(int format)
{
    g_amiga_video_format = format;
    if (format == AMIGA_VIDEO_FORMAT_R5G6B5) {
        g_amiga_video_bpp = 2;
    } else if (format == AMIGA_VIDEO_FORMAT_R5G5B5A1) {
        g_amiga_video_bpp = 2;
    } else {
        g_amiga_video_bpp = 4;
    }
}

void amiga_add_rtg_resolution(int width, int height)
{
    write_log("adding rtg resolution %dx%d\n", width, height);
    int *m = g_amiga_rtg_modes;
    while (1) {
        if (*m == -1) {
            write_log("too many resolutions\n");
            return;
        }
        if (*m == width && *(m + 1) == height) {
            write_log("resolution already exists\n");
            return;
        }
        if (*m == 0 && *(m + 1) == 0) {
            *m = width;
            *(m + 1) = height;
            return;
        }
        m += 2;
    }
}

void amiga_map_cd_drives(int enable)
{
    write_log("setting automount_cddrives to %d\n", enable != 0);
    currprefs.win32_automount_cddrives = (enable != 0);
    changed_prefs.win32_automount_cddrives = (enable != 0);
}

void amiga_set_deterministic_mode()
{
    write_log("libamiga enabling net play mode\n");
    g_uae_deterministic_mode = 1;
}

void amiga_write_uae_config(const char *path)
{
    write_log("writing uae config to %s\n", path);
    cfgfile_save(&currprefs, path, 0);
}

static void set_path(TCHAR *d1, TCHAR *d2, const TCHAR *s)
{
    /* Use PATH_MAX - 1 so we have space for any trailing slash */
    uae_strlcpy(d1, s, PATH_MAX - 1);
    int d1_len = strlen(d1);
    if (d1[d1_len - 1] != '/') {
        strcat(d1, "/");
    }
    uae_strlcpy(d2, d1, PATH_MAX);
}

void amiga_set_paths(const char **rom_paths,
                     const char **floppy_paths,
                     const char **cd_paths,
                     const char **hd_paths)
{
    for (int i = 0; i < MAX_PATHS; i++) {
        if (floppy_paths[i] == NULL || floppy_paths[i][0] == '\0') {
            break;
        }
        set_path(currprefs.path_floppy.path[i],
                 changed_prefs.path_floppy.path[i],
                 floppy_paths[i]);
    }
    for (int i = 0; i < MAX_PATHS; i++) {
        if (cd_paths[i] == NULL || cd_paths[i][0] == '\0') {
            break;
        }
        set_path(currprefs.path_cd.path[i],
                 changed_prefs.path_cd.path[i],
                 cd_paths[i]);
    }
    for (int i = 0; i < MAX_PATHS; i++) {
        if (hd_paths[i] == NULL || hd_paths[i][0] == '\0') {
            break;
        }
        set_path(currprefs.path_hardfile.path[i],
                 changed_prefs.path_hardfile.path[i],
                 hd_paths[i]);
    }
    for (int i = 0; i < MAX_PATHS; i++) {
        if (rom_paths[i] == NULL || rom_paths[i][0] == '\0') {
            break;
        }
        set_path(currprefs.path_rom.path[i],
                 changed_prefs.path_rom.path[i],
                 rom_paths[i]);
    }
}

int amiga_set_synchronization_log_file(const char *path)
{
#ifdef DEBUG_SYNC
    FILE *f = g_fopen(path, "wb");
    if (f) {
        write_log("sync debug log to %s\n", path);
        g_fs_uae_sync_debug_file = f;
        return 1;
    } else {
        write_log("error opening synchronization log file\n");
        return 0;
    }
#endif
}

int amiga_quickstart(int quickstart_model, int quickstart_config, int accuracy)
{
    int quickstart_compa = 1 - accuracy;
    int quickstart_romcheck = 0;
    write_log("amiga_quickstart model=%d config=%d compa=%d (accuracy %d)\n",
              quickstart_model,
              quickstart_config,
              quickstart_compa,
              accuracy);
    return built_in_prefs(&currprefs,
                          quickstart_model,
                          quickstart_config,
                          quickstart_compa,
                          quickstart_romcheck);
}

int amiga_get_rand_checksum()
{
    return uaerand() & 0x00ffffff;
}

int amiga_get_state_checksum(void)
{
    int checksum = uae_get_memory_checksum(NULL, 0);
#ifdef DEBUG_SYNC
    write_sync_log("memcheck: %08x\n", checksum);
#endif
    return checksum & 0x00ffffff;
}

int amiga_get_state_checksum_and_dump(void *data, int size)
{
    int checksum = uae_get_memory_checksum(data, size);
    return checksum & 0x00ffffff;
}

void amiga_main(void)
{
    write_log("amiga_main\n");
    uae_register_emulation_thread();

    // FIXME: Is this needed now? Check how/when WinUAE calls this
    keyboard_settrans();

    int argc = 1;
    char *argv[] = {
        strdup("fs-uae"),
        NULL,
    };

    // See WinMain2

    max_uae_width = 8192;
    max_uae_height = 8192;


    write_log (_T("Enumerating display devices.. \n"));
    enumeratedisplays();

    real_main(argc, argv);
    free(argv[0]);
#ifdef FILESYS
    write_log("real_main returned\n");
    write_log("calling filesys_flush_cache\n");
    filesys_flush_cache();
#endif
    write_log("flushing all file streams\n");
    fflush(NULL);
}

void amiga_write_config(const char *path)
{
    cfgfile_save(&currprefs, path, 0);
}

int amiga_enable_serial_port(const char *serial_name)
{
    write_log("amiga_enable_serial_port\n");
    if (serial_name != NULL && serial_name[0]) {
        write_log("serial port device: %s\n", serial_name);
        strcpy(changed_prefs.sername, serial_name);
        strcpy(currprefs.sername, serial_name);
        changed_prefs.use_serial = 1;
        currprefs.use_serial = 1;
        return 1;
    }
    return 0;
}

int amiga_enable_parallel_port(const char *parallel_name)
{
    write_log("amiga_enable_parallel_port\n");
    if (parallel_name != NULL && parallel_name[0]) {
        write_log("Parallel port device: %s\n", parallel_name);
        strcpy(changed_prefs.prtname, parallel_name);
        strcpy(currprefs.prtname, parallel_name);
        return 1;
    }
    return 0;
}

void amiga_set_cpu_idle(int idle)
{
    idle = CLAMP(idle, 0, 10);
    if (idle != 0) {
        idle = (12 - idle) * 15;
    }
    uae_log("setting cpu_idle = %d\n", idle);
    changed_prefs.cpu_idle = idle;
    currprefs.cpu_idle = idle;
}

int amiga_pause(int pause)
{
    // pause_emulation = pause;
    if (pause) {
        write_log("calling pausemode (1)\n");
        // pausemode(-1);
        g_amiga_paused = 1;
        pausemode(9);
    } else {
        write_log("calling pausemode (0)\n");
        g_amiga_paused = 0;
        // pausemode(-1);
        pausemode(0);
    }
    return 0;
}

int amiga_reset(int hard)
{
    uae_reset(hard, 1);
    return hard;
}

int amiga_state_save(int slot)
{
    if (slot < 0) {
        return 0;
    }
    if (slot >= 9) {
        return 0;
    }
    write_log("amiga_state_save %d\n", slot);
    int code = AKS_STATESAVEQUICK1 + slot * 2;
    inputdevice_add_inputcode(code, 1, NULL);
    return 1;
}

int amiga_state_load(int slot)
{
    if (slot < 0) {
        return 0;
    }
    if (slot >= 9) {
        return 0;
    }
    write_log("amiga_state_load %d\n", slot);
    int code = AKS_STATERESTOREQUICK1 + slot * 2;
    inputdevice_add_inputcode(code, 1, NULL);
    return 1;
}

const char *amiga_floppy_get_file(int index)
{
    return currprefs.floppyslots[index].df;
}

const char *amiga_floppy_get_list_entry(int index)
{
    return currprefs.dfxlist[index];
}

int amiga_floppy_get_drive_type(int index)
{
    return currprefs.floppyslots[index].dfxtype + 1;
}

int amiga_get_num_cdrom_drives()
{
    // FIXME: a bit hackish, do some sanity check on this method
    for (int i = 0; i < MAX_TOTAL_SCSI_DEVICES; i++) {
        if (currprefs.cdslots[i].inuse != 0) {
            return i + 1;
        }
    }
    return 0;
}

int amiga_get_num_floppy_drives()
{
    for (int i = 0; i < 4; i++) {
        if (currprefs.floppyslots[i].dfxtype < 0) {
            return i;
        }
    }
    return 4;
}

int amiga_floppy_set_from_list(int drive, int index)
{
    write_log("insert floppy (%d) into drive (%d)\n", index, drive);
    if (drive < 0 || drive > 3) {
        // assert
        return 0;
    }
    if (index < 0 || index >= AMIGA_FLOPPY_LIST_SIZE) {
        // assert
        return 0;
    }
    write_log("perform disk_swap drive %d floppy entry %d\n", drive, index);
    strcpy(changed_prefs.floppyslots[drive].df, currprefs.dfxlist[index]);
    // strcpy(currprefs.floppyslots[drive].df, currprefs.dfxlist[entry]);
    disk_insert(drive, currprefs.floppyslots[drive].df);
    /*
    int i;
    // eject disk from other drive (if inserted)
    for (i = 0; i < 4; i++) {
        if (strcmp (currprefs.floppyslots[i].df, currprefs.dfxlist[index]) ==
    0) changed_prefs.floppyslots[i].df[0] = 0;
    }
    // insert disk
    // FIXME: IMPORTANT: CHECK length of file (prevent buffer overrun)
    strcpy(changed_prefs.floppyslots[drive].df, currprefs.dfxlist[index]);
    config_changed = 1;
    return 1;
    */
    return 1;
}

int amiga_floppy_set_file(int drive, const char *file)
{
    write_log("insert floppy (%s) into drive (%d)\n", file, drive);
    int i;
    // eject disk from other drive (if inserted)
    for (i = 0; i < 4; i++) {
        if (strcmp(currprefs.floppyslots[i].df, file) == 0) {
            changed_prefs.floppyslots[i].df[0] = 0;
        }
    }
    // insert disk
    // FIXME: IMPORTANT: CHECK length of file (prevent buffer overrun)
    strcpy(changed_prefs.floppyslots[drive].df, file);
    config_changed = 1;
    return 1;
}

const char *amiga_cdrom_get_file(int index)
{
    return currprefs.cdslots[index].name;
}

void amiga_cdrom_eject(int drive)
{
    write_log("CD-ROM: eject drive %d\n", drive);
#if 0
    write_log("  (currprefs.cdslots[%d].name = %s)\n",
              drive, currprefs.cdslots[drive].name);
#endif

    // changed_prefs.cdslots[drive].inuse = false;
    changed_prefs.cdslots[drive].name[0] = '\0';
    changed_prefs.cdslots[drive].type = SCSI_UNIT_DEFAULT;
    config_changed = 1;
}

int amiga_cdrom_set_file(int drive, const char *file)
{
    write_log("CD-ROM: insert \"%s\" into drive %d\n", file, drive);
#if 0
    write_log("  (currprefs.cdslots[%d].name = %s)\n",
              drive, currprefs.cdslots[drive].name);
    write_log("  (changed_prefs.cdslots[%d].name = %s)\n",
              drive, changed_prefs.cdslots[drive].name);
#endif

    /* Eject CD from current drive */
    amiga_cdrom_eject(drive);

    /* Insert new CD */
    if (file[0] != '\0') {
        uae_tcslcpy(changed_prefs.cdslots[drive].name, file, MAX_DPATH);
        // changed_prefs.cdslots[drive].inuse = 1;

        /* Ejecting CD from other drives if it is inserted there */
        // FIXME: REPLACE 4 with constant
        for (int i = 0; i < 4; i++) {
            if (i != drive && strcmp(currprefs.cdslots[i].name, file) == 0) {
                amiga_cdrom_eject(i);
            }
        }
    }
    config_changed = 1;

    return 1;
}

int amiga_floppy_get_speed()
{
    int speed = currprefs.floppy_speed;
    write_log("speed is %d\n", speed);
    return speed / 100;
}

int amiga_floppy_set_speed(int speed)
{
    write_log("set floppy speed to %d\n", speed);
    changed_prefs.floppy_speed = speed * 100;
    config_changed = 1;
    return 1;
}

int amiga_cpu_get_speed()
{
    int speed = currprefs.m68k_speed;
    write_log("cpu speed is %d\n", speed);
    write_log("cpu freq is %d\n", currprefs.cpu_frequency);
    write_log("cpu mult is %d\n", currprefs.cpu_clock_multiplier);
    return speed;
}

int amiga_cpu_set_speed(int speed)
{
    static int initialized = 0;
    static int org_cpu_clock_multiplier;
    static int org_cpu_cycle_exact;
    if (!initialized) {
        org_cpu_clock_multiplier = currprefs.cpu_clock_multiplier;
        org_cpu_cycle_exact = currprefs.cpu_cycle_exact;
        initialized = 1;
    }
    write_log("set cpu speed to %d\n", speed);
    changed_prefs.m68k_speed = speed;
    if (speed == 0) {
        changed_prefs.cpu_clock_multiplier = org_cpu_clock_multiplier;
        changed_prefs.cpu_cycle_exact = org_cpu_cycle_exact;
    } else {
        changed_prefs.cpu_clock_multiplier = 32;
        changed_prefs.cpu_cycle_exact = 0;
    }
    config_changed = 1;
    return 1;
}

static int amiga_parse_option(const char *option, const char *value, int type)
{
    // some strings are modified during parsing
    char *value2 = strdup(value);
    int result = cfgfile_parse_option(
        &currprefs, (char *) option, (char *) value2, type);
    free(value2);
    write_log(
        "Set option %s = \"%s\"%s\n", option, value, result == 1 ? "" : " FAILED");
    if (result != 1) {
        gui_message("Option failed: %s = %s", option, value);
        amiga_log_warning(
            "Failed to set option \"%s\" to \"%s\" "
            "(result: %d)\n",
            option,
            value,
            result);
    }
    return result;
}

// ----------------------------------------------------------------------------

#include "fsemu-glib.h"

static bool initialized;
static GList *pending_options;

void amiga_set_initialized_and_apply_options(void)
{
    uae_log("UAE initialized, apply pending options\n");
    uae_log("--------------------------------------\n");
    initialized = true;

    GList *list = pending_options;
    while (list) {
        const char *option = (const char *) list->data;
        list = list->next;
        const char *value = (const char *) list->data;
        list = list->next;

        int result = amiga_parse_option(option, value, 0);
    }

    uae_log("UAE initialized, apply pending options done\n");
    uae_log("-------------------------------------------\n");
    g_list_free_full(pending_options, free);
    pending_options = NULL;
}

// ----------------------------------------------------------------------------

int amiga_set_option(const char *option, const char *value)
{
    // We apply the option also before UAE is properly initialized, in case
    // some lib functions assume cfg has been updated. Would be a good idea
    // to verify that nothing depending on options being actually set before
    // UAE is initialized.

    // FIXME: Would be better to tweak the UAE code to allow UAE to start
    // partially and do initial configuration (in the main thread) earlier,
    // and avoid this.

    int result = amiga_parse_option(option, value, 0);

    if (!initialized) {
        pending_options = g_list_append(pending_options, strdup(option));
        pending_options = g_list_append(pending_options, strdup(value));
    }

    return result;
}

// ----------------------------------------------------------------------------

int amiga_set_option_and_free(const char *option,
                              char *value,
                              amiga_free_function free_function)
{
    int result = amiga_set_option(option, value);
    free_function(value);
    return result;
}

int amiga_set_hardware_option(const char *option, const char *value)
{
    return amiga_parse_option(option, value, CONFIG_TYPE_HARDWARE);
}

int amiga_set_int_option(const char *option, int value)
{
    char *str_value = g_strdup_printf("%d", value);
    int result = amiga_set_option(option, str_value);
    g_free(str_value);
    return result;
}

int amiga_quit()
{
    uae_log("Calling uae_quit\n");
    uae_quit();
    return 1;
}

void amiga_set_display_function(display_function event_handler)
{
    g_libamiga_callbacks.display = event_handler;
}

/*
typedef void (*gui_function_t)(int, char *);
void amiga_set_gui_function(gui_function_t function)
{
    gui_function = function;
}
*/

void amiga_set_event_function(event_function event_handler)
{
    g_libamiga_callbacks.event = event_handler;
}

void amiga_set_init_function(init_function function)
{
    g_libamiga_callbacks.init = function;
}

void amiga_set_render_function(render_function function)
{
    g_libamiga_callbacks.render = function;
}

void amiga_set_log_function(log_function function)
{
    g_libamiga_callbacks.log = function;
}

void amiga_set_gui_message_function(log_function function)
{
    g_amiga_gui_message_function = function;
}

typedef struct {
    int event;
    void *data;
    int intdata;
} uae_main_event_t;

void uae_main_free_event(int event, void *data)
{
    if (data) {
        free(data);
    }
}

bool uae_main_next_event(int *event, void **data, int* intdata)
{
    uae_main_event_t *main_event =
        (uae_main_event_t *) g_async_queue_try_pop(module.main_queue);
    if (main_event == NULL) {
        *event = 0;
        *data = NULL;
        *intdata = 0;
        return false;
    } else {
        *event = main_event->event;
        *data = main_event->data;
        *intdata = main_event->intdata;
        free(main_event);
        return true;
    }
}

void uae_main_post_event(int event, void *data, int intdata)
{
    uae_main_event_t *main_event =
        (uae_main_event_t *) malloc(sizeof(uae_main_event_t));
    main_event->event = event;
    main_event->data = data;
    main_event->intdata = intdata;
    g_async_queue_push(module.main_queue, main_event);
    // printf("Posted main event %d (data: %p)\n", event, data);
}

}  // extern "C"

int disk_setwriteprotect(int num, const TCHAR *name, bool writeprotected);

void gui_disk_image_change(int unitnum, const TCHAR *name, bool writeprotected)
{
    if (name && strlen(name) > 0) {
        write_log(
            "gui_disk_image_change drive %d name %s write protected %d\n",
            unitnum,
            name,
            writeprotected);
        /*
        if (writeprotected) {
            write_log("calling disk_setwriteprotect 0\n");
            disk_setwriteprotect(unitnum, name, 0);
        }
        */
        uae_main_post_event(UAE_EVENT_FLOPPY0PATH + unitnum, strdup(name), 0);
    } else {
        write_log("gui_disk_image_change drive %d <no disk>\n", unitnum);
        uae_main_post_event(UAE_EVENT_FLOPPY0PATH + unitnum, NULL, 0);
    }
}

bool get_plugin_path(TCHAR *out, int size, const TCHAR *path)
{
    // static char* plugin_path_none = NULL;

    if (strcmp(path, "floppysounds") == 0) {
        if (g_floppy_sounds_dir) {
            strncpy(out, g_floppy_sounds_dir, size);
        } else {
            strncpy(out, "floppy_sounds", size);
        }
        // make sure out is null-terminated in any case
        out[size - 1] = '\0';
    } else {
        write_log(
            "\n-----------------> STUB: get_plugin_path, "
            "size: %d, path: %s\n",
            size,
            path);
        out[0] = '\0';
    }
    return TRUE;
}
