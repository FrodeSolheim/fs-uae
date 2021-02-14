#define FSEMU_INTERNAL
#include "fsemu-application.h"

#include "fsemu-glib.h"
#include "fsemu-log.h"
#include "fsemu-module.h"
#include "fsemu-util.h"

// ----------------------------------------------------------------------------

int fsemu_application_log_level = FSEMU_LOG_LEVEL_INFO;

static struct {
    bool initialized;
    char *name;
    char *base_dir_name;
    char *base_dir;
} fsemu_application;

void fsemu_application_set_name(const char *name)
{
    fsemu_assert_release(name != NULL);
    if (fsemu_application.name) {
        fsemu_application_log("Changing name might not work in practice\n");
        free(fsemu_application.name);
    }
    fsemu_application.name = strdup(name);
}

const char *fsemu_application_name(void)
{
    fsemu_assert_release(fsemu_application.name != NULL);
    return fsemu_application.name;
}

void fsemu_application_set_base_dir_name(const char *dir_name)
{
    fsemu_assert_release(dir_name != NULL);
    if (fsemu_application.base_dir_name) {
        fsemu_application_log(
            "Changing base dir name might not work in practice\n");
        free(fsemu_application.base_dir_name);
    }
    fsemu_application.base_dir_name = strdup(dir_name);
}

// ----------------------------------------------------------------------------

const char *fsemu_application_base_dir(void)
{
    fsemu_assert_release(fsemu_application.initialized);
    // fsemu_assert_release(fsemu_application.base_dir_name != NULL);

    if (fsemu_application.base_dir == NULL) {
        const char *home_dir = g_get_home_dir();
        const char *name = fsemu_application.base_dir_name;
        if (name == NULL) {
            name = fsemu_application.name;
        }
        fsemu_assert_release(name != NULL);

        fsemu_application.base_dir = g_build_filename(home_dir, name, NULL);
        if (g_file_test(fsemu_application.base_dir, G_FILE_TEST_IS_DIR)) {
            // OK
        } else {
            int error = g_mkdir_with_parents(fsemu_application.base_dir, 0755);
            if (error) {
                fsemu_application_log_error(
                    "Could not create base directory %s\n",
                    fsemu_application.base_dir);
            }
        }
    }

    return fsemu_application.base_dir;
}

// ----------------------------------------------------------------------------

static void fsemu_application_quit(void)
{
}

// ----------------------------------------------------------------------------

void fsemu_application_init(void)
{
    if (FSEMU_MODULE_INIT(application)) {
        return;
    }
}
