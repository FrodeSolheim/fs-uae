#ifndef LIBFSEMU_INCLUDE_FS_EMU_DIALOG_H_
#define LIBFSEMU_INCLUDE_FS_EMU_DIALOG_H_

#define DIALOG_RESULT_AFFIRMATIVE 1
#define DIALOG_RESULT_NEGATIVE 2

#define DIALOG_MAX_LINES 4

typedef struct fs_emu_dialog {
    int result;
    char *title;
    char *affirmative;
    char *negative;
    char *lines[DIALOG_MAX_LINES];
} fs_emu_dialog;

fs_emu_dialog* fs_emu_dialog_create(const char *title,
        const char *affirmative, const char *negative);
void fs_emu_dialog_add_option(fs_emu_dialog *dialog, const char *option,
        int value);

void fs_emu_dialog_set_line(fs_emu_dialog *dialog, int line, const char *text);

int fs_emu_dialog_result(fs_emu_dialog *dialog);
void fs_emu_dialog_destroy(fs_emu_dialog *dialog);

//void fs_emu_dialog_lock();
//void fs_emu_dialog_unlock();

// you must hold the dialog lock when calling this function


// important: must acquire GUI lock before using this
void fs_emu_dialog_show(fs_emu_dialog *dialog);

// important: must acquire GUI lock before using this
void fs_emu_dialog_dismiss(fs_emu_dialog *dialog);

// important: must acquire GUI lock before using this
fs_emu_dialog *fs_emu_dialog_get_current();

extern int g_fs_emu_dialog_mode;

static inline int fs_emu_in_dialog_mode() {
    return g_fs_emu_dialog_mode;
}

#endif // LIBFSEMU_INCLUDE_FS_EMU_DIALOG_H_
