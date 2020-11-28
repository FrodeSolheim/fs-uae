#include "fsemu-internal.h"
#include "fsemu-media.h"

#include "fsemu-glib.h"
#include "fsemu-mutex.h"
#include "fsemu-thread.h"
#include "fsemu-util.h"

#define module fsemu_media

struct fsemu_media_drive {
    char *file;
    // Short drive name such as "A", "DF0" or "CD".
    // Index of drive in list of media drives.
    int index;
    char *name;
    // Longer description such as "Floppy drive DF0".
    char *title;
    int type;
    // Number of the drive within the group of same drive type. Useful together
    // with type, giving you info such as for example "second floppy drive".
    int type_index;
};

static struct {
    // GList *quit_callbacks;
    int drivecount;
    fsemu_media_drive_t *drives[FSEMU_MEDIA_DRIVE_MAX_COUNT];
    GArray *files[FSEMU_MEDIA_TYPE_COUNT];
    bool initialized;
    GHashTable *label_cache;
    GRegex *label_regex;
    fsemu_mutex_t *mutex;
    int type_count[FSEMU_MEDIA_TYPE_COUNT];
} module;

static void fsemu_media_assert_valid_type(int type)
{
    fsemu_assert(type >= 0 && type < FSEMU_MEDIA_DRIVE_TYPE_AFTER_LAST);
}

// ----------------------------------------------------------------------------
// Media drive "class" functions
// ----------------------------------------------------------------------------

fsemu_media_drive_t *fsemu_media_drive_new(void)
{
    fsemu_thread_assert_main();

    fsemu_media_drive_t *drive = FSEMU_UTIL_MALLOC0(fsemu_media_drive_t);
    // fsemu_refable_init_with_finalizer(mediadrive,
    // fsemu_media_drive_finalize);
    return drive;
}

fsemu_media_drive_t *fsemu_media_drive_new_with_type(int type)
{
    fsemu_thread_assert_main();

    fsemu_media_drive_t *drive = fsemu_media_drive_new();
    fsemu_media_drive_set_type(drive, type);
    return drive;
}

int fsemu_media_drive_index(fsemu_media_drive_t *drive)
{
    return drive->index;
}

const char *fsemu_media_drive_name(fsemu_media_drive_t *drive)
{
    fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);

    if (drive->name) {
        return drive->name;
    } else {
        return "???";
    }
}

void fsemu_media_drive_set_name(fsemu_media_drive_t *drive, const char *name)
{
    fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);
    fsemu_assert(name != NULL);

    fsemu_media_drive_assign_name(drive, strdup(name));
}

void fsemu_media_drive_assign_name(fsemu_media_drive_t *drive, char *name)
{
    fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);

    if (drive->name) {
        free(drive->name);
    }
    drive->name = name;
}

const char *fsemu_media_drive_title(fsemu_media_drive_t *drive)
{
    fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);

    if (drive->title) {
        return drive->title;
    } else {
        return "Unnamed drive";
    }
}

void fsemu_media_drive_set_title(fsemu_media_drive_t *drive, const char *title)
{
    fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);
    fsemu_assert(title != NULL);

    fsemu_media_drive_assign_title(drive, strdup(title));
}

void fsemu_media_drive_assign_title(fsemu_media_drive_t *drive, char *title)
{
    fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);

    if (drive->title) {
        free(drive->title);
    }
    drive->title = title;
}

int fsemu_media_drive_type(fsemu_media_drive_t *drive)
{
    // fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);

    return drive->type;
}

void fsemu_media_drive_set_type(fsemu_media_drive_t *drive, int type)
{
    fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);
    fsemu_media_assert_valid_type(type);

    drive->type = type;
}

int fsemu_media_drive_type_index(fsemu_media_drive_t *drive)
{
    // fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);

    return drive->type_index;
}

const char *fsemu_media_drive_file(fsemu_media_drive_t *drive)
{
    fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);

    return drive->file;
}

void fsemu_media_drive_set_file(fsemu_media_drive_t *drive, const char *file)
{
    fsemu_assert(drive != NULL);
    // fsemu_assert(file != NULL);

    fsemu_media_drive_assign_file(drive, file ? strdup(file) : NULL);
}

void fsemu_media_drive_assign_file(fsemu_media_drive_t *drive, char *file)
{
    fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);

    // fsemu_mutex_lock(module.mutex);

    if (drive->file) {
        free(drive->file);
    }
    drive->file = file;

    // fsemu_mutex_unlock(module.mutex);
}

#if 0
void fsemu_media_drive_add_file(fsemu_media_drive_t *drive, const char *file)
{
    
}
#endif

// ----------------------------------------------------------------------------
// Module level functions
// ----------------------------------------------------------------------------

void fsemu_media_add_drive(fsemu_media_drive_t *drive)
{
    fsemu_thread_assert_main();
    fsemu_assert(drive != NULL);
    fsemu_assert(module.drivecount < FSEMU_MEDIA_DRIVE_MAX_COUNT);
    fsemu_media_assert_valid_type(drive->type);

    fsemu_media_init();

    module.drives[module.drivecount] = drive;
    drive->index = module.drivecount;

    drive->type_index = module.type_count[drive->type];
    module.type_count[drive->type] += 1;

    module.drivecount += 1;
}

int fsemu_media_drive_count(void)
{
    // Allow access from other threads, assuming drives themselves are not
    // modified once the emulation is running.
    // fsemu_thread_assert_main();

    return module.drivecount;
}

fsemu_media_drive_t *fsemu_media_drive_at_index(int index)
{
    // fsemu_thread_assert_main();
    fsemu_assert(index >= 0 && index < module.drivecount);

    return module.drives[index];
}

void fsemu_media_add_file(int type, const char *file)
{
    fsemu_thread_assert_main();
    fsemu_media_assert_valid_type(type);
    fsemu_assert(file != NULL);

    GArray *array = module.files[type];
    fsemu_assert_release(array->len < FSEMU_MEDIA_MAX_FILE_COUNT);

    char *file2 = strdup(file);
    // Using file2 because g_array_append_val requires a variable.
    g_array_append_val(array, file2);
}

int fsemu_media_count(int type)
{
    fsemu_thread_assert_main();
    fsemu_media_assert_valid_type(type);

    GArray *array = module.files[type];
    return array->len;
}

const char *fsemu_media_file(int type, int index)
{
    fsemu_thread_assert_main();
    fsemu_media_assert_valid_type(type);

    GArray *array = module.files[type];
    fsemu_assert(index < array->len);
    return g_array_index(array, const char *, index);
}

const char *fsemu_media_label(const char *file)
{
    fsemu_thread_assert_main();

    if (file == NULL) {
        return NULL;
    }

    const char *existing = g_hash_table_lookup(module.label_cache, file);
    if (existing) {
        return existing;
    }

    char *label = NULL;

    // FIXME: TODO: Also support explicit labels given by for example FS-UAE.

    GMatchInfo *match_info;
    g_regex_match(module.label_regex, file, 0, &match_info);
    while (g_match_info_matches(match_info)) {
        if (label) {
            free(label);
        }
        label = g_match_info_fetch(match_info, 0);
        g_print("Found: %s\n", label);

        // g_free(word);
        g_match_info_next(match_info, NULL);
        // break;
    }
    g_match_info_free(match_info);

    if (label == NULL) {
        int len = strlen(file);
        // FIXME:
        if (len > 24) {
            label = strdup(file + len - 24);
        } else {
            label = strdup(file);
        }
    }

    g_hash_table_insert(module.label_cache, strdup(file), label);

    return label;
}

void fsemu_media_init(void)
{
    if (module.initialized) {
        return;
    }
    module.initialized = true;

    fsemu_thread_init();
    fsemu_thread_assert_main();

    for (int i = 0; i < FSEMU_MEDIA_DRIVE_TYPE_AFTER_LAST; i++) {
        fsemu_media_log("Initializing media list for media type %d\n", i);
        module.files[i] = g_array_new(FALSE, FALSE, sizeof(char *));
    }

    // module.label_regex =
    //     g_regex_new("[(]([^)]*Disk[^)]*)[)]", G_REGEX_CASELESS, 0, NULL);
    // module.label_regex =
    //     g_regex_new("[^(]*Disk[^)]*", G_REGEX_CASELESS, 0, NULL);
    module.label_regex =
        g_regex_new("Disk[ ]?[0-9A-Z]+", G_REGEX_CASELESS, 0, NULL);

    fsemu_assert(module.label_regex != NULL);

    module.label_cache = g_hash_table_new(g_str_hash, g_str_equal);
}

#undef module
