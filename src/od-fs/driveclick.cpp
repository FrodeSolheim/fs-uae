#include <fs/filesys.h>
#include "sysconfig.h"
#include "sysdeps.h"

#include "driveclick.h"
#include "uae/fs.h"
#include "uae/glib.h"

static const char *g_driveclick_path = "";
//static char *g_driveclick_name = NULL;

extern "C" {

void amiga_set_builtin_driveclick_path(const char *path)
{
    g_driveclick_path = g_strdup(path);
}

#if 0
void amiga_set_drive_sound_name(const char *name)
{
    g_driveclick_name = g_strdup(name);
}
#endif

} // extern C

#ifdef DRIVESOUND

int driveclick_loadresource (struct drvsample *sp, int drivetype)
{
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

        char *path = g_build_filename(g_driveclick_path, name, NULL);
        int64_t size = fs_path_get_size(path);
        if (size >= 0) {
            int len = (int) size;
            void *buffer = g_malloc(len);
            FILE* f = g_fopen(path, "rb");
            int read = fread(buffer, 1, len, f);
            if (read == size) {
                struct drvsample* s = sp + type;
                //write_log("decode drive click sample %d from %p len %d\n",
                //        type, data, len);
                s->p = decodewav((uae_u8*) buffer, &len);
                s->len = len;
            }
            g_free(buffer);
        }
        g_free(path);
    }
    return ok;
}

void driveclick_fdrawcmd_close(int drive)
{

}

int driveclick_fdrawcmd_open(int drive)
{
    return 0;
}

void driveclick_fdrawcmd_detect(void)
{

}

void driveclick_fdrawcmd_seek(int drive, int cyl)
{

}

void driveclick_fdrawcmd_motor (int drive, int running)
{

}

void driveclick_fdrawcmd_vsync(void)
{

}

#endif /* DRIVESOUND */
