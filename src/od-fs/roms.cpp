#include <fs/filesys.h>

#include "sysconfig.h"
#include "sysdeps.h"

#include "gui.h"
#include "options.h"
#include "uae/memory.h"
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

void amiga_patch_rom(uae_u8 *buf, size_t size);

static struct romdata *scan_single_rom_2 (struct zfile *f, uae_u32 *crc32)
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
    amiga_patch_rom(rombuf, size);
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
    *crc32 = get_crc32 (rombuf, size);
    if (!rd) {
        write_log (_T("!: Name='%s':%d\nCRC32=%08X SHA1=%s\n"),
            zfile_getname (f), size, *crc32, get_sha1_txt (rombuf, size));
    } else {
        TCHAR tmp[MAX_DPATH];
        getromname (rd, tmp);
        write_log (_T("*: %s:%d = %s\nCRC32=%08X SHA1=%s\n"),
            zfile_getname (f), size, tmp, *crc32, get_sha1_txt (rombuf, size));
    }
    xfree (rombuf);
    return rd;
}

static struct romdata *scan_single_rom (const TCHAR *path, uae_u32 *crc32)
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
    return scan_single_rom_2 (z, crc32);
}

#include <fs/filesys.h>

extern "C" {

void amiga_add_key_dir(const char *path) {
    char *p = fs_path_join(path, "rom.key", NULL);
    addkeyfile(p);
    free(p);
}

int amiga_add_rom_file(const char *path, const char *cache_path) {
    write_log("amiga_add_rom_file %s\n", path);
    struct romdata *rd = NULL;
    struct fs_stat rom_stat;

    if (cache_path != NULL) {
        if (fs_stat(path, &rom_stat) != 0) {
            write_log("- could not stat rom file\n");
            return 2;
        }
        struct fs_stat cache_stat;
        if (fs_stat(cache_path, &cache_stat) == 0) {
            if (rom_stat.mtime == cache_stat.mtime) {
                // we should have cached ROM information...
                unsigned char buf[4];
                FILE *f = fs_fopen(cache_path, "rb");
                int read_count = 0;
                if (f != NULL) {
                    read_count = fread(buf, 4, 1, f);
                    fclose(f);
                }
                if (read_count == 1) {
                    write_log("- found cached crc32\n");
                    uae_u32 crc32 = buf[0] << 24 | buf[1] << 16 |
                                    buf[2] << 8 | buf[3];
                    rd = getromdatabycrc(crc32);
                    if (rd) {
                        write_log("- rom added via cached entry\n");
                        romlist_add(path, rd);
                        return 0;
                    }
                    // we do not need / want to write cache information
                    // to this file since we have updated information
                    // cache_path = NULL;
                    return 1;
                }
            }
        }
    }

    uae_u32 crc32 = 0;
    rd = scan_single_rom(path, &crc32);
    if (rd) {
        crc32 = rd->crc32;
        //write_log("rom data at %p\n", rd);
        //write_log("adding to rom list (id: %d)\n", rd->id);
        //if (rd->name) {
        //    write_log("rom name: %s\n", rd->name);
        //}
        romlist_add(path, rd);
        // FIXME: Should rd be freed here?
    }
    else {
        write_log("- not a known rom file\n");
        // FIXME: should set crc32 here also, so this ROM won't be
        // rescanned again and again
    }
    if (crc32 && cache_path != NULL) {
        write_log("- crc32 cache file: %s\n", cache_path);
        FILE *f = fs_fopen(cache_path, "wb");
        if (f != NULL) {
            write_log("- writing crc32 to ROM cache file\n");
            unsigned char buf[4];
            buf[0] = (crc32 >> 24) & 0xff;
            buf[1] = (crc32 >> 16) & 0xff;
            buf[2] = (crc32 >> 8) & 0xff;
            buf[3] = (crc32) & 0xff;
            if (fwrite(buf, 4, 1, f) == 1) {
                fclose(f);
                struct timeval tv;
                tv.tv_sec = rom_stat.mtime;
                tv.tv_usec = 0;
                //write_log("- setting file time\n");
                fs_set_file_time(cache_path, &tv);
            }
            else {
                fclose(f);
            }
        }
    }

    write_log("- done\n");
    return 0;
}

} // extern C
