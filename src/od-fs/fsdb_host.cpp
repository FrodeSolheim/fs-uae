/*
 * UAE - The Un*x Amiga Emulator
 *
 * Library of functions to make emulated filesystem as independent as
 * possible of the host filesystem's capabilities.
 *
 * Copyright 1997 Mathias Ortmann
 * Copyright 1999 Bernd Schmidt
 * Copyright 2012 Frode Solheim
 */

#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"
#include "memory.h"

#include "fsdb.h"

//#include "win32.h"
//#include <windows.h>
#include <fs/fs.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifdef WINDOWS
#include <sys/utime.h>
#else
#include <sys/time.h>
#endif
#include <string.h>
#include <glib.h>
#include <glib/gstdio.h>

extern int g_fsdb_debug;

void fsdb_lock() {
    // FIXME
}

void fsdb_unlock() {
    // FIXME
}

static char g_fsdb_file_path_buffer[PATH_MAX + 1] = { };

char *fsdb_file_path(const char* nname) {
    strncpy(g_fsdb_file_path_buffer, nname, PATH_MAX);
    return g_fsdb_file_path_buffer;
}

//static char g_fsdb_host_meta_name_buffer[PATH_MAX + 1] = {};

/*
 char *fsdb_host_meta_name(const char* nname) {
 strncpy(g_fsdb_host_meta_name_buffer, nname, PATH_MAX);
 return g_fsdb_host_meta_name_buffer;
 }
 */

static char g_fsdb_attr_path_buffer[PATH_MAX + 1] = { };

char *fsdb_attr_path(const char* fsn, int create_dir) {
    // FIXME: could do with some bounds checking here
    //g_fsdb_host_name_meta_buffer[0] = 0;
    int len = strlen(fsn);
    int pos = strlen(fsn) - 1;
    while (fsn[pos] != '/' && pos > 0) {
        pos--;
    }
    int name_pos = pos + 1;
    strncpy(g_fsdb_attr_path_buffer, fsn, pos);
    strcpy(g_fsdb_attr_path_buffer + pos, "/.meta/");
    pos += 7;
    if (create_dir) {
        fs_mkdir(g_fsdb_attr_path_buffer, 0755);
    }
    strcpy(g_fsdb_attr_path_buffer + pos, fsn + name_pos);
    pos += len - name_pos;
    strcpy(g_fsdb_attr_path_buffer + pos, ".attr");
    return g_fsdb_attr_path_buffer;
}

#define NUM_EVILCHARS 10
static TCHAR evilchars[NUM_EVILCHARS] = { '%', '\\', '*', '?', '\"', '/', '|',
        '<', '>', '|' };

#define UAEFSDB_BEGINS _T("__uae___")

int fsdb_name_invalid(const TCHAR *n) {
    if (g_fsdb_debug) {
        write_log("fsdb_name_invalid n=%s\n", n);
    }
    return 0;
}

int fsdb_name_invalid_dir(const TCHAR *n) {
    if (g_fsdb_debug) {
        write_log("fsdb_name_invalid_dir n=%s\n", n);
    }
    return 0;
}

uae_u32 filesys_parse_mask(uae_u32 mask) {
    return mask ^ 0xf;
}

int fsdb_exists(const TCHAR *nname) {
    return g_file_test(nname, G_FILE_TEST_EXISTS);
}

int fsdb_set_file_attrs(a_inode *aino) {
    return 0;
#if 0
    STUB("");
    int result = 0;

    fsdb_lock();
    char *fsn = fsdb_file_path(aino->nname);
    if (!g_file_test(fsn, G_FILE_TEST_EXISTS)) {
        fsdb_unlock();
        return ERROR_OBJECT_NOT_AROUND;
    }

    uint32_t mask = filesys_parse_mask(aino->amigaos_mode);
    char astr[] = "--------\n";
    if (mask & A_FIBF_HIDDEN) astr[0] = 'H';
    if (mask & A_FIBF_SCRIPT) astr[1] = 'S';
    if (mask & A_FIBF_PURE) astr[2] = 'P';
    if (mask & A_FIBF_ARCHIVE) astr[3] = 'A';
    if (mask & A_FIBF_READ) astr[4] = 'R';
    if (mask & A_FIBF_WRITE) astr[5] = 'W';
    if (mask & A_FIBF_EXECUTE) astr[6] = 'E';
    if (mask & A_FIBF_DELETE) astr[7] = 'D';
    printf("%s\n", astr);

    int meta_file = 1;
    /*
     int meta_file = 0;
     if (aino->comment != NULL) {
     // must write comment to file
     meta_file = 1;
     }
     else if (mask != (A_FIBF_READ | A_FIBF_WRITE | A_FIBF_DELETE)) {
     // non-standard attributes
     meta_file = 1;
     }
     */
    char *meta_file_path = fsdb_attr_path(fsn, 1);
    FILE *f = NULL;
    if (meta_file) {
        f = g_fopen(meta_file_path, "wb");
        if (f == NULL) {
            write_log("could not open meta file %s\n", meta_file_path);
            result = ERROR_DISK_WRITE_PROTECTED;
        }
        else {
            if (fwrite(astr, 1, 9, f) != 9) {
                // ...
                result = ERROR_DISK_IS_FULL;
            }
            if (result == 0) {
                if (aino->comment != NULL) {
                    size_t len = strlen(aino->comment);
                    if (fwrite(aino->comment, 1, len, f) != len) {
                        result = ERROR_DISK_IS_FULL;
                    }
                }
            }
        }
    }
    else {
        if (g_file_test(meta_file_path, G_FILE_TEST_EXISTS)) {
            if (g_unlink(meta_file_path) != 0) {
                write_log("could not unlink meta file %s\n", meta_file_path);
                result = ERROR_DISK_WRITE_PROTECTED;
            }
        }
    }
    if (f) {
        fclose(f);
    }
    /*
     if (result) {
     fsdb_unlock();
     return result;
     }
     */
    fsdb_unlock();
    //g_free(meta_file_path);
    return result;
#endif
}

/* return supported combination */
int fsdb_mode_supported(const a_inode *aino) {
    int mask = aino->amigaos_mode;
    return mask;
}

static char hex_chars[] = "0123456789abcdef";

char *aname_to_nname(const char *aname) {
    size_t len = strlen(aname);
    //int repl_from = 1000000;
    //char *aname_u = g_ascii_strup(aname, len);
    int repl_1 = -1;
    int repl_2 = -1;

    TCHAR a = aname[0];
    TCHAR b = (a == '\0' ? a : aname[1]);
    TCHAR c = (b == '\0' ? b : aname[2]);
    TCHAR d = (c == '\0' ? c : aname[3]);

    if (a >= 'a' && a <= 'z') a -= 32;
    if (b >= 'a' && b <= 'z') b -= 32;
    if (c >= 'a' && c <= 'z') c -= 32;

    // reserved dos devices
    size_t ll = 0;
    if (a == 'A' && b == 'U' && c == 'X') ll = 3; /* AUX  */
    if (a == 'C' && b == 'O' && c == 'N') ll = 3; /* CON  */
    if (a == 'P' && b == 'R' && c == 'N') ll = 3; /* PRN  */
    if (a == 'N' && b == 'U' && c == 'L') ll = 3; /* NUL  */
    if (a == 'L' && b == 'P' && c == 'T' && (d >= '0' && d <= '9')) ll = 4; /* LPT# */
    if (a == 'C' && b == 'O' && c == 'M' && (d >= '0' && d <= '9')) ll = 4; /* COM# */
    /* AUX.anything, CON.anything etc.. are also illegal names */
    if (ll && (len == ll || (len > ll && aname[ll] == '.'))) {
        repl_1 = 2;
    }

    /* spaces and periods at the end are a no-no */
    int i = len - 1;
    if (aname[i] == '.' || aname[i] == ' ') {
        repl_2 = i;
    }

    char *buf = (char*) malloc(PATH_MAX);
    char *p = buf;

    int repl, j;
    unsigned char x;
    for (i = 0; i < (int) len; i++) {
        x = (unsigned char) aname[i];
        repl = 0;
        if (i == repl_1) {
            repl = 1;
        }
        else if (i == repl_2) {
            repl = 2;
        }
        else if (x < 32) {
            // these are not allowed on Windows
            repl = 1;
        }
        /*
         else if (x > 127) {
         repl = 1;
         }
         */
        for (j = 0; j < NUM_EVILCHARS; j++) {
            if (x == evilchars[j]) {
                repl = 1;
                break;
            }
        }
        // FIXME: bounds check
        if (repl) {
            *p++ = '%';
            *p++ = hex_chars[(x & 0xf0) >> 4];
            *p++ = hex_chars[x & 0xf];
        }
        else {
            *p++ = x;
        }
    }
    *p++ = '\0';

    //char *result = strdup(buf);
    gsize read, written;
    gchar *result = g_convert(buf, -1, "UTF-8", "ISO-8859-1", &read, &written,
            NULL);
    free(buf);

    if (g_fsdb_debug) {
        write_log("aname_to_nname %s => %s\n", aname, result);
    }
    return result;
}

static unsigned char char_to_hex(unsigned char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return 10 + c - 'a';
    }
    if (c >= 'A' && c <= 'F') {
        return 10 + c - 'A';
    }
    return 0;
}

char *nname_to_aname(const char *nname) {
    int len = strlen(nname);
    char *result = strdup(nname);
    unsigned char *p = (unsigned char *) result;
    for (int i = 0; i < len; i++) {
        unsigned char c = nname[i];
        if (c == '%' && i < len - 2) {
            *p++ = (char_to_hex(nname[i + 1]) << 4) | char_to_hex(nname[i + 2]);
            i += 2;
        }
        else {
            *p++ = c;
        }
    }
    *p++ = '\0';

    gsize read, written;
    gchar *cresult = g_convert(result, -1, "ISO-8859-1", "UTF-8", &read,
            &written, NULL);
    free(result);

    if (g_fsdb_debug) {
        if (cresult) {
            write_log("nname_to_aname %s => %s\n", nname, cresult);
        }
        else {
            write_log("nname_to_aname %s => Failed\n", nname);
        }
    }
    return cresult;
}

/* Return nonzero if we can represent the amigaos_mode of AINO within the
 * native FS.  Return zero if that is not possible.
 */
int fsdb_mode_representable_p(const a_inode *aino, int amigaos_mode) {
    return 1;
}

TCHAR *fsdb_create_unique_nname(a_inode *base, const TCHAR *suggestion) {
    // FIXME: SHOULD NOT BE CALLED?
    //STUB("");
    //TCHAR *c;
    //TCHAR tmp[256] = UAEFSDB_BEGINS;
    //int i;
    //_tcsncat (tmp, suggestion, 240);
    char *nname = aname_to_nname(suggestion);
    TCHAR *p = build_nname(base->nname, nname);
    free(nname);
    return p;
}

/* Return 1 if the nname is a special host-only name which must be translated
 * to aname using fsdb.
 */
int custom_fsdb_used_as_nname(a_inode *base, const TCHAR *nname) {
    //STUB("base=? nname=\"%s\"", nname);
    return 1;
}

typedef struct file_info {
    int exists;
    int mode;
    int dir;
    char *comment;
    int days;
    int mins;
    int ticks;

} file_info;

// ----------
// FIXME: get_time and put_time from filesys.cpp -should merge these
// functions..

/* convert time_t to/from AmigaDOS time */
static const int secs_per_day = 24 * 60 * 60;
static const int diff = (8 * 365 + 2) * (24 * 60 * 60);

static void get_time (time_t t, int* days, int* mins, int* ticks) {
    /* time_t is secs since 1-1-1970 */
    /* days since 1-1-1978 */
    /* mins since midnight */
    /* ticks past minute @ 50Hz */

    t -= diff;
    if (t < 0)
        t = 0;
    *days = t / secs_per_day;
    t -= *days * secs_per_day;
    *mins = t / 60;
    t -= *mins * 60;
    *ticks = t * 50;
}

static time_t put_time (int days, int mins, int ticks) {
    time_t t;

    if (days < 0)
        days = 0;
    if (days > 9900 * 365)
        days = 9900 * 365; // in future far enough?
    if (mins < 0 || mins >= 24 * 60)
        mins = 0;
    if (ticks < 0 || ticks >= 60 * 50)
        ticks = 0;

    t = ticks / 50;
    t += mins * 60;
    t += ((uae_u64)days) * secs_per_day;
    t += diff;

    return t;
}

// ----------

static void get_file_info(const char *nname, file_info *info) {
    if (!fs_path_exists(nname)) {
        info->exists = 0;
        return;
    }
    info->exists = 1;
    info->dir = fs_path_is_dir(nname);

    info->mode = 0;
    //aino->amigaos_mode |= A_FIBF_HIDDEN;
    //aino->amigaos_mode |= A_FIBF_SCRIPT;
    //aino->amigaos_mode |= A_FIBF_PURE;
    //aino->amigaos_mode |= A_FIBF_ARCHIVE;
    info->mode |= A_FIBF_READ;
    info->mode |= A_FIBF_WRITE;
    info->mode |= A_FIBF_EXECUTE;
    info->mode |= A_FIBF_DELETE;

    // FIXME: remove WRITE and DELETE if file is not writable
    // FIXME: correct?
    info->mode = filesys_parse_mask(info->mode);

    info->comment = NULL;

    if (uae_synchronous_mode()) {
        // this is not a very good solution. For instance, WB 1.3 does not
        // update .info files correctly when the file date/time is
        // constant.
        info->days = 1;
        info->mins = 1;
        info->ticks = 1;
    }
    else {
        struct fs_stat buf;
        fs_stat(nname, &buf);
        get_time(buf.mtime, &info->days, &info->mins, &info->ticks);
    }
}

void fsdb_get_time(const char *nname, int *days, int *mins, int *ticks) {
    file_info info;
    get_file_info(nname, &info);
    if (info.exists) {
        *days = info.days;
        *mins = info.mins;
        *ticks = info.ticks;
        if (info.comment) {
            free(info.comment);
        }
    }
    else {
        *days = 1;
        *mins = 1;
        *ticks = 1;
    }
}

extern unsigned char g_latin1_lower_table[256];

static void lower_latin1(char *s) {
    unsigned char *u = (unsigned char*) s;
    while (*u) {
        *u = g_latin1_lower_table[*u];
        u++;
    }
}

/* For an a_inode we have newly created based on a filename we found on the
 * native fs, fill in information about this file/directory.  */
int fsdb_fill_file_attrs(a_inode *base, a_inode *aino) {
    if (g_fsdb_debug) {
        write_log("fsdb_fill_file_attrs nname is %s\n", aino->nname);
    }

    file_info info;
    get_file_info(aino->nname, &info);
    if (!info.exists) {
        return 0;
    }
    aino->dir = info.dir;
    aino->amigaos_mode = info.mode;
    aino->comment = info.comment;
    return 1;
}

static void find_nname_case(const char *dir_path, char **name) {
    // FIXME: should cache these results...!!
    if (g_fsdb_debug) {
        write_log("find case for %s in dir %s\n", *name, dir_path);
    }
    fs_dir *dir = fs_dir_open(dir_path, 0);
    if (dir == NULL) {
        write_log("open dir %s failed\n", *name);
        return;
    }

    gsize read, written;
    gchar *cmp_name = g_convert(*name, -1, "ISO-8859-1", "UTF-8", &read,
            &written, NULL);
    if (cmp_name == NULL) {
        write_log("WARNING: could not convert to latin1: %s", *name);
        return;
    }
    lower_latin1(cmp_name);

    const gchar *result;
    while (1) {
        result = fs_dir_read_name(dir);
        if (!result) {
            break;
        }

        gchar *cmp_result = g_convert(result, -1, "ISO-8859-1", "UTF-8", &read,
                &written, NULL);
        //printf("%s %s\n", result, cmp_result);
        if (cmp_result == NULL) {
            // file name could not be represented as ISO-8859-1, so it
            // will be ignored
            printf("ignoring\n");
            continue;
        }
        lower_latin1(cmp_result);

        if (strcmp(cmp_name, cmp_result) == 0) {
            *name = fs_strdup(result);
            if (g_fsdb_debug) {
                write_log("              %s\n", *name);
            }
            break;
        }
        g_free(cmp_result);
    }
    fs_dir_close(dir);
    free(cmp_name);
}

a_inode *custom_fsdb_lookup_aino_aname(a_inode *base, const TCHAR *aname) {
    //STUB("base=? aname=\"%s\"", aname);

    char *nname = aname_to_nname(aname);
    find_nname_case(base->nname, &nname);
    char *full_nname = build_nname(base->nname, (TCHAR*) nname);
    free(nname);
    file_info info;
    get_file_info(full_nname, &info);
    if (!info.exists) {
        free(full_nname);
        return NULL;
    }
    a_inode *aino = xcalloc (a_inode, 1);
    aino->aname = my_strdup(aname);
    aino->nname = full_nname;
    aino->comment = info.comment;
    aino->amigaos_mode = info.mode;
    aino->dir = info.dir;
    aino->has_dbentry = 0;
    aino->dirty = 0;
    aino->db_offset = 0;
    return aino;
}

a_inode *custom_fsdb_lookup_aino_nname(a_inode *base, const TCHAR *nname) {
    //STUB("base=? nname=\"%s\"", nname);

    char *full_nname = build_nname(base->nname, (TCHAR*) nname);
    file_info info;
    get_file_info(full_nname, &info);
    if (!info.exists) {
        free(full_nname);
        return NULL;
    }
    a_inode *aino = xcalloc (a_inode, 1);
    aino->aname = nname_to_aname(nname); //my_strdup(nname + 8);
    aino->nname = full_nname;
    aino->comment = info.comment;
    aino->amigaos_mode = info.mode;
    aino->dir = info.dir;
    aino->has_dbentry = 0;
    aino->dirty = 0;
    aino->db_offset = 0;
    return aino;
}

unsigned char g_latin1_lower_table[256] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
        47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
        97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
        112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 91, 92, 93, 94,
        95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
        110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
        124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137,
        138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151,
        152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
        166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
        180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 224, 225,
        226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
        240, 241, 242, 243, 244, 245, 246, 215, 248, 249, 250, 251, 252, 253,
        254, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235,
        236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
        250, 251, 252, 253, 254, 255, };
