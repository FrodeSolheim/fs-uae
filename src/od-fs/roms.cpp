#include "sysconfig.h"
#include "sysdeps.h"

#include "arcadia.h"
#include "crc32.h"
#include "uae/fs.h"
#include "rommgr.h"
#include "zfile.h"

#include <fs/filesys.h>
#include <fs/glib.h>

void romlist_init (void)
{
	TCHAR tmp2[MAX_DPATH];

	for (int id = 1;; id++) {
		struct romdata *rd = getromdatabyid (id);
		if (!rd) {
			break;
		}
		if (rd->crc32 == 0xffffffff) {
			if (rd->configname) {
				_stprintf (tmp2, _T (":%s"), rd->configname);
			} else {
				_stprintf (tmp2, _T (":ROM_%03d"), rd->id);
			}
			romlist_add (tmp2, rd);
		}
	}
}

#define AMIGA_OS_130_SHA1 \
	"\xc3\x9b\xd9\x09\x4d\x4e\x5f\x4e\x28\xc1" \
	"\x41\x1f\x30\x86\x95\x04\x06\x06\x2e\x87"

#define AMIGA_OS_310_SHA1 \
	"\xc3\xc4\x81\x16\x08\x66\xe6\x0d\x08\x5e" \
	"\x43\x6a\x24\xdb\x36\x17\xff\x60\xb5\xf9"

void romlist_patch_rom (uae_u8 *buf, size_t size)
{
	write_log ("romlist_patch_rom\n");
	uae_u8 sha1[SHA1_SIZE];
	get_sha1 (buf, size, sha1);
	write_log ("ROM: SHA1=");
	for (int i = 0; i < SHA1_SIZE; i++) {
		write_log ("%02x", sha1[i]);
	}
	write_log ("\n");
	int converted = 0;
	if (memcmp (sha1, AMIGA_OS_130_SHA1, SHA1_SIZE) == 0) {
		write_log ("convering amiga-os-130 ROM (in-memory) "
			   "to preferred A500 ROM\n");
		buf[413] = '\x08';
		buf[176029] = '\xb9';
		buf[262121] = '\x26';
		converted = 1;
	} else if (memcmp (sha1, AMIGA_OS_310_SHA1, SHA1_SIZE) == 0) {
		write_log ("converting amiga-os-310 ROM (in-memory) "
			   "to preferred A4000 ROM\n");
		buf[220] = '\x74';
		buf[222] = '\x7a';
		buf[326] = '\x70';
		buf[434] = '\x7c';
		buf[524264] = '\x45';
		buf[524266] = '\x14';
		converted = 1;
	}
	if (converted) {
		get_sha1 (buf, size, sha1);
		write_log ("ROM: SHA1=");
		for (int i = 0; i < SHA1_SIZE; i++) {
			write_log ("%02x", sha1[i]);
		}
		write_log ("\n");
	}
#if 0
	struct romdata *rd = getromdatabydata (buf, size);
	if (rd) {
		char tmp[MAX_DPATH];
		getromname (rd, tmp);
		printf ("KICKSTART: %s\n", tmp);
	}
#endif
}

static struct romdata *scan_single_rom_2 (struct zfile *f, uae_u32 *crc32)
{
	uae_u8 buffer[20] = { 0 };
	uae_u8 *rombuf;
	int cl = 0, size;
	struct romdata *rd = 0;

	zfile_fseek (f, 0, SEEK_END);
	size = zfile_ftell (f);
	zfile_fseek (f, 0, SEEK_SET);
	if (size > 524288 * 2) { /* don't skip KICK disks or 1M ROMs */
		write_log (_T ("'%s': too big %d, ignored\n"), zfile_getname (f), size);
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
	romlist_patch_rom (rombuf, size);
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
		write_log (_T ("!: Name='%s':%d\nCRC32=%08X SHA1=%s\n"),
			   zfile_getname (f), size, *crc32, get_sha1_txt (rombuf, size));
	} else {
		TCHAR tmp[MAX_DPATH];
		getromname (rd, tmp);
		write_log (_T ("*: %s:%d = %s\nCRC32=%08X SHA1=%s\n"),
			   zfile_getname (f), size, tmp, *crc32, get_sha1_txt (rombuf, size));
	}
	xfree (rombuf);
	return rd;
}

static struct romdata *scan_single_rom (const TCHAR *path, uae_u32 *crc32)
{
	struct zfile *z;
	struct romdata *rd;

#ifdef ARCADIA
	TCHAR tmp[MAX_DPATH];
	_tcscpy (tmp, path);
	rd = scan_arcadia_rom (tmp, 0);
	if (rd)
		return rd;
#endif
	rd = getromdatabypath (path);
	if (rd && rd->crc32 == 0xffffffff)
		return rd;
	z = zfile_fopen (path, _T ("rb"), ZFD_NORMAL);
	if (!z)
		return 0;
	return scan_single_rom_2 (z, crc32);
}

extern "C" {

void amiga_add_key_dir (const char *path)
{
	char *p = g_build_filename (path, "rom.key", NULL);
	addkeyfile (p);
	g_free (p);
}

int amiga_add_rom_file (const char *path, const char *cache_path)
{
	write_log ("amiga_add_rom_file %s\n", path);
	struct romdata *rd = NULL;
	struct fs_stat rom_stat;

	if (cache_path != NULL) {
		if (fs_stat (path, &rom_stat) != 0) {
			write_log ("- could not stat rom file\n");
			return 2;
		}
		struct fs_stat cache_stat;
		if (fs_stat (cache_path, &cache_stat) == 0) {
			if (rom_stat.mtime == cache_stat.mtime) {
				// we should have cached ROM information...
				unsigned char buf[4];
				FILE *f = g_fopen (cache_path, "rb");
				int read_count = 0;
				if (f != NULL) {
					read_count = fread (buf, 4, 1, f);
					fclose (f);
				}
				if (read_count == 1) {
					write_log ("- found cached crc32\n");
					uae_u32 crc32 = buf[0] << 24 | buf[1] << 16 |
							buf[2] << 8 | buf[3];
					rd = getromdatabycrc (crc32);
					if (rd) {
						write_log ("- rom added via cached entry\n");
						romlist_add (path, rd);
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
	rd = scan_single_rom (path, &crc32);
	if (rd) {
		crc32 = rd->crc32;
		//write_log("rom data at %p\n", rd);
		//write_log("adding to rom list (id: %d)\n", rd->id);
		//if (rd->name) {
		//	  write_log("rom name: %s\n", rd->name);
		//}
		romlist_add (path, rd);
		// FIXME: Should rd be freed here?
	} else {
		write_log ("- not a known rom file\n");
		// FIXME: should set crc32 here also, so this ROM won't be
		// rescanned again and again
	}
	if (crc32 && cache_path != NULL) {
		write_log ("- crc32 cache file: %s\n", cache_path);
		FILE *f = g_fopen (cache_path, "wb");
		if (f != NULL) {
			write_log ("- writing crc32 to ROM cache file\n");
			unsigned char buf[4];
			buf[0] = (crc32 >> 24) & 0xff;
			buf[1] = (crc32 >> 16) & 0xff;
			buf[2] = (crc32 >> 8) & 0xff;
			buf[3] = (crc32) & 0xff;
			if (fwrite (buf, 4, 1, f) == 1) {
				fclose (f);
				struct timeval tv;
				tv.tv_sec = rom_stat.mtime;
				tv.tv_usec = 0;
				//write_log("- setting file time\n");
				fs_set_file_time (cache_path, &tv);
			} else {
				fclose (f);
			}
		}
	}

	write_log ("- done\n");
	return 0;
}

} // extern C
