#ifndef fsemu_media_drive_H_
#define fsemu_media_drive_H_

#include "fsemu-config.h"
#include "fsemu-log.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_MEDIA_TYPE_COUNT 6

// This cannot be increased without also adding support for additional actions.
#define FSEMU_MEDIA_DRIVE_MAX_COUNT 10

// This cannot be increased without also adding support for additional actions.
#define FSEMU_MEDIA_MAX_FILE_COUNT 256
// #define FSEMU_MEDIA_ACTUAL_MAX_FILE_COUNT 20

enum {
    FSEMU_MEDIA_DRIVE_TYPE_UNKNOWN,
    FSEMU_MEDIA_DRIVE_TYPE_FLOPPY,
    FSEMU_MEDIA_DRIVE_TYPE_CDROM,
    FSEMU_MEDIA_DRIVE_TYPE_HD,
    FSEMU_MEDIA_DRIVE_TYPE_CARTRIDGE,
    FSEMU_MEDIA_DRIVE_TYPE_TAPE,
    FSEMU_MEDIA_DRIVE_TYPE_AFTER_LAST,
};

struct fsemu_media_drive;
typedef struct fsemu_media_drive fsemu_media_drive_t;

void fsemu_media_init(void);

// Don't assume that you can successfully add more drives after FSEMU has been
// fully initialized. All drives should be added before GUI is created.
void fsemu_media_add_drive(fsemu_media_drive_t *drive);

int fsemu_media_drive_count(void);

fsemu_media_drive_t *fsemu_media_drive_at_index(int index);

// ----------------------------------------------------------------------------

fsemu_media_drive_t *fsemu_media_drive_new(void);
fsemu_media_drive_t *fsemu_media_drive_new_with_type(int type);

// Index of drive in list of drives, only valid after the drive has been added
// with fsemu_media_add_drive.
int fsemu_media_drive_index(fsemu_media_drive_t *drive);

const char *fsemu_media_drive_name(fsemu_media_drive_t *drive);
void fsemu_media_drive_set_name(fsemu_media_drive_t *drive, const char *name);
void fsemu_media_drive_assign_name(fsemu_media_drive_t *drive, char *name);

const char *fsemu_media_drive_title(fsemu_media_drive_t *drive);
void fsemu_media_drive_set_title(fsemu_media_drive_t *drive,
                                 const char *title);
void fsemu_media_drive_assign_title(fsemu_media_drive_t *drive, char *name);

int fsemu_media_drive_type(fsemu_media_drive_t *drive);
void fsemu_media_drive_set_type(fsemu_media_drive_t *drive, int type);

int fsemu_media_drive_type_index(fsemu_media_drive_t *drive);

const char *fsemu_media_drive_file(fsemu_media_drive_t *drive);

// const char *fsemu_media_drive_file_label(fsemu_media_drive_t *drive);

// Note: This does not actually insert anything into the drive, it only tells
// FSEMU about the file currently inserted in the drive. This function is
// multithread-safe. FIXME: This is only true if the getter is changed to do
// a copy!
void fsemu_media_drive_set_file(fsemu_media_drive_t *drive, const char *file);

void fsemu_media_drive_assign_file(fsemu_media_drive_t *drive, char *file);

// Or add files to "type" swap list instead?
// void fsemu_media_drive_add_file(fsemu_media_drive_t *drive, const char
// *file);

void fsemu_media_add_file(int type, const char *file);

int fsemu_media_count(int type);

const char *fsemu_media_file(int type, int index);

const char *fsemu_media_label(const char *file);

// ----------------------------------------------------------------------------

#define fsemu_media_log(format, ...) \
    fsemu_log("[FSE] [MED] " format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif  // fsemu_media_drive_H_
