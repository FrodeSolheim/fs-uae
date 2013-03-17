#include "sysconfig.h"
#include "sysdeps.h"
#include <fs/string.h>
#include <fs/filesys.h>
#include "include/driveclick.h"

static void *g_driveclick_data[DS_END];
static int g_driveclick_size[DS_END];
static char *g_driveclick_path = "";
//static char *g_driveclick_name = NULL;

extern "C" {

void amiga_set_builtin_driveclick_path(const char *path) {
    g_driveclick_path = fs_strdup(path);
}

#if 0
void amiga_set_drive_sound_name(const char *name) {
    g_driveclick_name = fs_strdup(name);
}
#endif

} // extern C

int driveclick_loadresource (struct drvsample *sp, int drivetype) {
    int type, ok;
    ok = 1;
    for (type = 0; type < DS_END; type++) {
        //int type = -1;
        //int len = -1;
        //unsigned char* data = NULL;
        const char *name = NULL;
        switch(type) {
            case 0:
                //type = DS_CLICK;
                //data = drive_click_data;
                //len = drive_click_data_size;
                name = "drive_click.wav";
                break;
            case 1:
                //type = DS_SPIN;
                //data = drive_spin_data;
                //len = drive_spin_data_size;
                name = "drive_spin.wav";
                break;
            case 2:
                //type = DS_SPINND;
                //data = drive_spinnd_data;
                //len = drive_spinnd_data_size;
                name = "drive_spinnd.wav";
                break;
            case 3:
                //type = DS_START;
                //data = drive_startup_data;
                //len = drive_startup_data_size;
                name = "drive_startup.wav";
                break;
            case 4:
                //type = DS_SNATCH;
                //data = drive_snatch_data;
                //len = drive_snatch_data_size;
                name = "drive_snatch.wav";
                break;
        }

        if (!name) {
            continue;
        }

        char *path = fs_path_join(g_driveclick_path, name, NULL);
        int64_t size = fs_path_get_size(path);
        if (size >= 0) {
            int len = (int) size;
            void *buffer = malloc(len);
            FILE* f = fs_fopen(path, "rb");
            int read = fread(buffer, 1, len, f);
            if (read == size) {
                struct drvsample* s = sp + type;
                //write_log("decode drive click sample %d from %p len %d\n",
                //        type, data, len);
                s->p = decodewav((uae_u8*) buffer, &len);
                s->len = len;
            }
            free(buffer);
        }
    }
    return ok;
}

void driveclick_fdrawcmd_close(int drive) {

}

int driveclick_fdrawcmd_open(int drive) {
    return 0;
}

void driveclick_fdrawcmd_detect(void) {

}

void driveclick_fdrawcmd_seek(int drive, int cyl) {

}
void driveclick_fdrawcmd_motor (int drive, int running) {

}

void driveclick_fdrawcmd_vsync(void) {

}
