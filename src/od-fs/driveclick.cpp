#include <fs/filesys.h>
#include "sysconfig.h"
#include "sysdeps.h"

#include "driveclick.h"
#include "uae/fs.h"
#include "uae/glib.h"

#include <fs/data.h>

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
    for (int type = 0; type < DS_END; type++) {
        const char *name = NULL;
        switch (type) {
        case 0:
            name = "share/fs-uae/floppy_sounds/drive_click.wav";
            break;
        case 1:
            name = "share/fs-uae/floppy_sounds/drive_spin.wav";
            break;
        case 2:
            name = "share/fs-uae/floppy_sounds/drive_spinnd.wav";
            break;
        case 3:
            name = "share/fs-uae/floppy_sounds/drive_startup.wav";
            break;
        case 4:
            name = "share/fs-uae/floppy_sounds/drive_snatch.wav";
            break;
        default:
            continue;
        }
        char *data = NULL;
        int size = 0;
        if (fs_data_file_content(name, &data, &size) == 0) {
            int len = (int) size;
            struct drvsample* s = sp + type;
            s->p = decodewav((uae_u8*) data, &len);
            s->len = len;
            free(data);
        }
    }
    return 1;
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
