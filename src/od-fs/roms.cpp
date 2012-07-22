#include "sysconfig.h"
#include "sysdeps.h"

#include "gui.h"
#include "options.h"
#include "memory.h"
#include "rommgr.h"
#include "custom.h"
#include "events.h"
#include "newcpu.h"
#include "traps.h"
#include "disk.h"
#include "uae.h"
#include "threaddep/thread.h"
#include "filesys.h"
#include "autoconf.h"
#include "inputdevice.h"
#include "inputrecord.h"
#include "xwin.h"
#include "keyboard.h"
#include "zfile.h"
#include "parallel.h"
#include "audio.h"
#include "arcadia.h"
#include "drawing.h"
#include "fsdb.h"
#include "crc32.h"
#include <stdio.h>
#include <stdlib.h>

#include <fs/filesys.h>

static struct romdata *scan_single_rom_2 (struct zfile *f)
{
    uae_u8 buffer[20] = { 0 };
    uae_u8 *rombuf;
    int cl = 0, size;
    struct romdata *rd = 0;

    zfile_fseek (f, 0, SEEK_END);
    size = zfile_ftell (f);
    zfile_fseek (f, 0, SEEK_SET);
    if (size > 524288 * 2)  {/* don't skip KICK disks or 1M ROMs */
        write_log (_T("'%s': too big %d, ignored\n"), zfile_getname(f), size);
        return 0;
    }
    zfile_fread (buffer, 1, 11, f);
    if (!memcmp (buffer, "KICK", 4)) {
        zfile_fseek (f, 512, SEEK_SET);
        if (size > 262144)
            size = 262144;
    } else if (!memcmp (buffer, "AMIROMTYPE1", 11)) {
        cl = 1;
        size -= 11;
    } else {
        zfile_fseek (f, 0, SEEK_SET);
    }
    rombuf = xcalloc (uae_u8, size);
    if (!rombuf)
        return 0;
    zfile_fread (rombuf, 1, size, f);
    if (cl > 0) {
        decode_cloanto_rom_do (rombuf, size, size);
        cl = 0;
    }
    if (!cl) {
        rd = getromdatabydata (rombuf, size);
        if (!rd && (size & 65535) == 0) {
            /* check byteswap */
            int i;
            for (i = 0; i < size; i+=2) {
                uae_u8 b = rombuf[i];
                rombuf[i] = rombuf[i + 1];
                rombuf[i + 1] = b;
            }
            rd = getromdatabydata (rombuf, size);
        }
    }
    if (!rd) {
        write_log (_T("!: Name='%s':%d\nCRC32=%08X SHA1=%s\n"),
            zfile_getname (f), size, get_crc32 (rombuf, size), get_sha1_txt (rombuf, size));
    } else {
        TCHAR tmp[MAX_DPATH];
        getromname (rd, tmp);
        write_log (_T("*: %s:%d = %s\nCRC32=%08X SHA1=%s\n"),
            zfile_getname (f), size, tmp, get_crc32 (rombuf, size), get_sha1_txt (rombuf, size));
    }
    xfree (rombuf);
    return rd;
}

static struct romdata *scan_single_rom (const TCHAR *path)
{
    struct zfile *z;
    TCHAR tmp[MAX_DPATH];
    struct romdata *rd;

    _tcscpy (tmp, path);
    rd = scan_arcadia_rom (tmp, 0);
    if (rd)
        return rd;
    rd = getromdatabypath (path);
    if (rd && rd->crc32 == 0xffffffff)
        return rd;
    z = zfile_fopen (path, _T("rb"), ZFD_NORMAL);
    if (!z)
        return 0;
    return scan_single_rom_2 (z);
}

extern "C" {

void amiga_add_key_dir(const char *path) {
    char *p = fs_path_join(path, "rom.key", NULL);
    addkeyfile(p);
    free(p);
}

int amiga_add_rom_file(const char *path) {
    write_log("amiga_add_rom_file %s\n", path);
    struct romdata *rd = scan_single_rom(path);
    if (rd) {
        //write_log("rom data at %p\n", rd);
        //write_log("adding to rom list (id: %d)\n", rd->id);
        //if (rd->name) {
        //    write_log("rom name: %s\n", rd->name);
        //}
        romlist_add(path, rd);
        // FIXME: Should rd be freed here?
    }
    else {
        write_log("not a known rom file\n");
    }
    write_log("done\n");
    return 0;
}

}
