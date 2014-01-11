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

#include <fs/fs.h>
#include <fs/filesys.h>
#include <fs/time.h>

#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"
#include "uae/memory.h"

#include "fsdb.h"
#include "zfile.h"

#include <sys/types.h>
#include <sys/stat.h>
#ifdef WINDOWS
#include <sys/utime.h>
#else
#include <sys/time.h>
#endif
#include <string.h>

#include "fsdb_host.h"

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
    return fs_path_exists(nname);
}

#define PERM_OFFSET 0
#define PERM_LEN 8
//#define DAYS_LEN 5
//#define MINS_LEN 4
//#define TICKS_LEN 4

#define COMMENT_OFFSET 25

//#define TIME_OFFSET PERM_LEN + 1
//#define TIME_LEN (1 + DAYS_LEN + 1 + MINS_LEN + 1 + TICKS_LEN + 1)
#define TIME_LEN 22

void fsdb_init_file_info(fsdb_file_info *info) {
    info->days = 0;
    info->mins = 0;
    info->ticks = 0;
    info->comment = NULL;
    info->mode = A_FIBF_READ | A_FIBF_WRITE | A_FIBF_EXECUTE | A_FIBF_DELETE;

    if (uae_deterministic_mode()) {
        // leave time at 0, 0, 0
    }
    else {
        fs_time_val tv;
        fs_get_current_time(&tv);
        struct mytimeval mtv;
        mtv.tv_sec = tv.tv_sec + fs_get_local_time_offset(tv.tv_sec);
        mtv.tv_usec = tv.tv_usec;
        timeval_to_amiga (&mtv, &info->days, &info->mins, &info->ticks);
        if (g_fsdb_debug) {
            write_log("- initialized date/time from current time\n");
            write_log("- days %d mins %d ticks %d - %lld %d\n",
                    info->days, info->mins, info->ticks, mtv.tv_sec,
                    mtv.tv_usec);
        }
    }
}

FILE *fsdb_open_meta_file_for_path(const char *path, const char *mode,
        int always_open) {
    char *meta_file = fs_strconcat(path, ".uaem", NULL);
    if (g_fsdb_debug) {
        write_log("opening meta file %s mode %s\n", meta_file, mode);
    }
    if (!always_open) {
        if (!fs_path_exists(meta_file)) {
            if (g_fsdb_debug) {
                write_log("- didn't exist, don't force open\n");
            }
            return NULL;
        }
    }
    FILE *f = fs_fopen(meta_file, mode);
    if (g_fsdb_debug) {
        write_log("FILE is %p\n", f);
    }
    free(meta_file);
    return f;
}

/* return supported combination */
int fsdb_mode_supported(const a_inode *aino) {
    int mask = aino->amigaos_mode;
    return mask;
}

#define NUM_EVILCHARS 9
static TCHAR evilchars[NUM_EVILCHARS] = { '%', '\\', '*', '?', '\"', '/', '|',
        '<', '>'};
static char hex_chars[] = "0123456789abcdef";

char *aname_to_nname(const char *aname, int ascii) {
    size_t len = strlen(aname);
    int repl_1 = -1;
    int repl_2 = -1;

    TCHAR a = aname[0];
    TCHAR b = (a == '\0' ? a : aname[1]);
    TCHAR c = (b == '\0' ? b : aname[2]);
    TCHAR d = (c == '\0' ? c : aname[3]);

    if (a >= 'a' && a <= 'z') a -= 32;
    if (b >= 'a' && b <= 'z') b -= 32;
    if (c >= 'a' && c <= 'z') c -= 32;

    // reserved dos devices in Windows
    size_t ll = 0;
    if (a == 'A' && b == 'U' && c == 'X') ll = 3; // AUX
    if (a == 'C' && b == 'O' && c == 'N') ll = 3; // CON
    if (a == 'P' && b == 'R' && c == 'N') ll = 3; // PRN
    if (a == 'N' && b == 'U' && c == 'L') ll = 3; // NUL
    if (a == 'L' && b == 'P' && c == 'T' && (d >= '0' && d <= '9')) ll = 4; // LPT#
    if (a == 'C' && b == 'O' && c == 'M' && (d >= '0' && d <= '9')) ll = 4; // COM#
    // AUX.anything, CON.anything etc.. are also illegal names in Windows
    if (ll && (len == ll || (len > ll && aname[ll] == '.'))) {
        repl_1 = 2;
    }

    // spaces and periods at the end are a no-no in Windows
    int i = len - 1;
    if (aname[i] == '.' || aname[i] == ' ') {
        repl_2 = i;
    }

    // allocating for worst-case scenario here (max replacements)
    char *buf = (char*) malloc(len * 3 + 1);
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
        else if (ascii && x > 127) {
            repl = 1;
        }
        for (j = 0; j < NUM_EVILCHARS; j++) {
            if (x == evilchars[j]) {
                repl = 1;
                break;
            }
        }
        if (i == len - 1) {
            // last character, we can now check the file ending
            if (len >= 5 && strncasecmp(aname + len - 5, ".uaem", 5) == 0) {
                // we don't allow Amiga files ending with .uaem, so we replace
                // the last character
                repl = 1;
            }
        }
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

    if (ascii) {
        return buf;
    }

    char* result = fs_utf8_from_latin1(buf, -1);
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

char *nname_to_aname(const char *nname, int noconvert) {
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

    if (noconvert) {
        return result;
    }

    char* cresult = fs_utf8_to_latin1(result, -1);
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
    char *nname = aname_to_nname(suggestion, 0);
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

int fsdb_get_file_info(const char *nname, fsdb_file_info *info) {
    int error = 0;
    if (g_fsdb_debug) {
        write_log("fsdb_get_file_info %s\n", nname);
    }
    info->comment = NULL;
    if (!fs_path_exists(nname)) {
        if (g_fsdb_debug) {
            write_log("- file does not exist: %s\n", nname);
        }
        info->type = 0;
        return ERROR_OBJECT_NOT_AROUND;
    }

    info->type = fs_path_is_dir(nname) ? 2 : 1;
    info->mode = 0;

    int read_perm = 0;
    int read_time = 0;
    int read_comment = 0;

    char *meta_file = fs_strconcat(nname, ".uaem", NULL);

    FILE *f = fsdb_open_meta_file_for_path(nname, "rb", 1);
    int file_size = 0;
    if (f == NULL) {
        if (g_fsdb_debug) {
            write_log("fsdb_get_file_info - could not open file\n");
        }
        error = host_errno_to_dos_errno(errno);
    }
    else {
        fseek(f, 0, SEEK_END);
        file_size = ftell(f);
        fseek(f, 0, SEEK_SET);
    }

    char *data = (char *) malloc(file_size + 1);
    data[file_size] = '\0';
    char *p = data;
    char *end = data + file_size;
    if (end - data > 0) {
        int count = fread(data, 1, file_size, f);
        if (count != file_size) {
            write_log("WARNING: could not read permissions "
                    "from %s (%d)\n", meta_file, errno);
            error = host_errno_to_dos_errno(errno);
            end = data;
        }
    }
    if (f != NULL) {
        fclose(f);
    }

    if ((end - p) >= 3) {
        if (p[0] == 0xef && p[1] == 0xbb && p[2] == 0xbf) {
            p += 3;
        }
    }

    //while (*p == ' ') {
    //    p++;
    //}

    if ((end - p) >= 8) {
        info->mode |= *(p++) == 'h' ? A_FIBF_HIDDEN : 0;
        info->mode |= *(p++) == 's' ? A_FIBF_SCRIPT : 0;
        info->mode |= *(p++) == 'p' ? A_FIBF_PURE : 0;
        info->mode |= *(p++) == 'a' ? A_FIBF_ARCHIVE : 0;
        info->mode |= *(p++) == 'r' ? A_FIBF_READ : 0;
        info->mode |= *(p++) == 'w' ? A_FIBF_WRITE : 0;
        info->mode |= *(p++) == 'e' ? A_FIBF_EXECUTE : 0;
        info->mode |= *(p++) == 'd' ? A_FIBF_DELETE : 0;
        read_perm = 1;
    }

    //while (*p == ' ') {
    //    p++;
    //}

    p++;

    if ((end - p) >= TIME_LEN) {
        //p[TIME_LEN - 1] = '\0';
        struct tm tm;
        tm.tm_isdst = -1;

        // haven't got strptime on Windows...

        p[4] = '\0';
        tm.tm_year = atoi(p) - 1900;
        p += 5;
        p[2] = '\0';
        tm.tm_mon = atoi(p) - 1;
        p += 3;
        p[2] = '\0';
        tm.tm_mday = atoi(p);
        p += 3;

        p[2] = '\0';
        tm.tm_hour = atoi(p);
        p += 3;
        p[2] = '\0';
        tm.tm_min = atoi(p);
        p += 3;
        p[2] = '\0';
        tm.tm_sec = atoi(p);
        p += 3;

        p[2] = '\0';
        int sub = atoi(p);
        p += 3;

        //printf("hrs: %d\n", tm.tm_hour);
        //printf("min: %d\n", tm.tm_min);
        //printf("sec: %d\n", tm.tm_sec);
        //printf("sub: %d\n", sub);

        struct mytimeval mtv;
        mtv.tv_sec = fs_timegm(&tm);
        if (mtv.tv_sec == -1) {
            // an error occured while parsing time - invalid time
            write_log("- warning, error reading time from metadata\n");
        }
        else {
            mtv.tv_usec = sub * 10000;
            timeval_to_amiga(&mtv, &info->days, &info->mins, &info->ticks);
            read_time = 1;
        }
    }

    if ((end - p) > 0) {
        int len = end - p;
        info->comment = (char*) malloc(len + 1);
        if (g_fsdb_debug) {
            write_log("- malloced comment len %d (+1)\n", len);
        }
        char *o = info->comment;
        while (p < end && *p != '\r' && *p != '\n') {
            *o++ = *p++;
        }
        *o = '\0';
    }

    free(data);
    free(meta_file);

    if (!read_perm) {
        if (g_fsdb_debug) {
            write_log("- setting default perms\n");
        }
        info->mode |= A_FIBF_READ;
        info->mode |= A_FIBF_WRITE;
        info->mode |= A_FIBF_EXECUTE;
        info->mode |= A_FIBF_DELETE;
        if (! uae_deterministic_mode()) {
            // FIXME: remove WRITE and DELETE if file is not writable
        }
    }

    if (!read_time) {
        if (uae_deterministic_mode()) {
            // this is not a very good solution. For instance, WB 1.3 does
            // not update .info files correctly when the file date/time is
            // constant.
            info->days = 0;
            info->mins = 0;
            info->ticks = 0;
        }
        else {
            struct fs_stat buf;
            if (fs_stat(nname, &buf) != 0) {
                if (g_fsdb_debug) {
                    write_log("- error stating %s (%d)\n", nname, errno);
                }
            }

            mytimeval mtv;
            mtv.tv_sec = buf.mtime + fs_get_local_time_offset(buf.mtime);
            mtv.tv_usec = buf.mtime_nsec / 1000;
            timeval_to_amiga(&mtv, &info->days, &info->mins, &info->ticks);

            if (g_fsdb_debug) {
                write_log("- initialized date/time from file mtime\n");
                write_log("- days %d mins %d ticks %d - %lld %d\n",
                        info->days, info->mins, info->ticks, mtv.tv_sec,
                        mtv.tv_usec);
            }

        }
    }
    return error;
}

int fsdb_set_file_info(const char *nname, fsdb_file_info *info) {
    FILE *f = NULL;
    if (g_fsdb_debug) {
        write_log("fsdb_set_file_info %s\n", nname);
    }
    int error = 0;
    if (!fs_path_exists(nname)) {
        error = ERROR_OBJECT_NOT_AROUND;
    }

    int need_metadata_file = 1;
    if (info->comment != NULL) {
        if (g_fsdb_debug) {
            write_log("- comment is not NULL: \"%s\"\n", info->comment);
        }
    }
    else if (info->mode != (A_FIBF_READ | A_FIBF_WRITE | A_FIBF_EXECUTE | \
            A_FIBF_DELETE)) {
        if (g_fsdb_debug) {
            write_log("- mode was %d\n", info->mode);
        }
    }
    else {
        struct mytimeval mtv;
        amiga_to_timeval(&mtv, info->days, info->mins, info->ticks);
        mtv.tv_sec -= fs_get_local_time_offset(mtv.tv_sec);
        if (g_fsdb_debug) {
            write_log("- days %d mins %d ticks %d - %lld %d\n",
                    info->days, info->mins, info->ticks, mtv.tv_sec,
                    mtv.tv_usec);
            write_log("- fs_get_local_time_offset %d\n",
                    fs_get_local_time_offset(mtv.tv_sec));
        }

        struct timeval tv;
        tv.tv_sec = mtv.tv_sec;
        tv.tv_usec = mtv.tv_usec;
        if (fs_set_file_time(nname, &tv) != 0) {
            if (g_fsdb_debug) {
                write_log("- errno %d setting file mtime\n", errno);
            }
            error = errno;
        }
        else {
            if (g_fsdb_debug) {
                write_log("- checking if precision is good enough!\n");
            }
            struct fs_stat buf;
            if (fs_stat(nname, &buf) == 0) {
                write_log("- %d vs %d\n", (int) buf.mtime, (int) mtv.tv_sec);
                write_log("- %d vs %d\n", (int) buf.mtime_nsec, (int) (mtv.tv_usec * 1000));
                if (buf.mtime == mtv.tv_sec &&
                        buf.mtime_nsec == mtv.tv_usec * 1000) {
                    if (g_fsdb_debug) {
                        write_log("- good enough!\n");
                        need_metadata_file = 0;
                    }
                }
                else {
                    if (g_fsdb_debug) {
                        write_log("- *not* good enough!\n");
                    }
                }
            }
        }
    }

    // always write metadata file, for now...
    need_metadata_file = 1;

    if (!error) {
        f = fsdb_open_meta_file_for_path(nname, "wb", need_metadata_file);
        if (f == NULL) {
            if (g_fsdb_debug) {
                write_log("fsdb_set_file_info - could not open file\n");
            }
            error = host_errno_to_dos_errno(errno);
        }
    }

    if (need_metadata_file == 0 && f == NULL) {
        return 0;
    }

    if (!error) {
        char astr[] = "--------";
        if (info->mode & A_FIBF_HIDDEN) astr[0] = 'h';
        if (info->mode & A_FIBF_SCRIPT) astr[1] = 's';
        if (info->mode & A_FIBF_PURE) astr[2] = 'p';
        if (info->mode & A_FIBF_ARCHIVE) astr[3] = 'a';
        if (info->mode & A_FIBF_READ) astr[4] = 'r';
        if (info->mode & A_FIBF_WRITE) astr[5] = 'w';
        if (info->mode & A_FIBF_EXECUTE) astr[6] = 'e';
        if (info->mode & A_FIBF_DELETE) astr[7] = 'd';
        write_log("- writing mode %s\n", astr);

        if (fwrite(astr, 8, 1, f) != 1) {
            error = host_errno_to_dos_errno(errno);
        }
    }

    if (!error) {
        struct mytimeval mtv;
        amiga_to_timeval(&mtv, info->days, info->mins, info->ticks);

        // FIXME: reentrant?
        time_t secs = mtv.tv_sec;
        struct tm *gt = gmtime(&secs);

        if (fprintf(f, " %04d-%02d-%02d %02d:%02d:%02d.%02d ",
                1900 + gt->tm_year, 1 + gt->tm_mon, gt->tm_mday,
                gt->tm_hour, gt->tm_min, gt->tm_sec,
                mtv.tv_usec / 10000) < 0) {

        //if (fprintf(f, " %05d %04d %04d ", info->days, info->mins,
        //        info->ticks) < 0) {
            if (g_fsdb_debug) {
                write_log("fsdb_set_file_time - fprintf failed\n");
            }
            error = host_errno_to_dos_errno(errno);
        }
    }

    if (!error) {
        if (info->comment) {
            write_log("- writing comment %s\n", info->comment);
            int len = strlen(info->comment);
            if (len && fwrite(info->comment, len, 1, f) != 1) {
                error = host_errno_to_dos_errno(errno);
            }
        }
    }

    if (!error) {
        fprintf(f, "\n");
    }

    if (f != NULL) {
        fclose(f);
    }
    if (info->comment) {
        free(info->comment);
    }
    return error;
}

void fsdb_get_file_time(a_inode *aino, int *days, int *mins, int *ticks) {
    fsdb_file_info info;
    fsdb_get_file_info(aino->nname, &info);
    if (info.type) {
        *days = info.days;
        *mins = info.mins;
        *ticks = info.ticks;
        if (info.comment) {
            free(info.comment);
        }
    }
    else {
        // FIXME: file does not exist
        *days = 0;
        *mins = 0;
        *ticks = 0;
    }
}

bool my_utime(const char *name, struct mytimeval *tv) {
    int days = 0;
    int mins = 0;
    int ticks = 0;

    if (tv == NULL) {
        fs_time_val tv;
        fs_get_current_time(&tv);
        struct mytimeval mtv;
        mtv.tv_sec = tv.tv_sec + fs_get_local_time_offset(tv.tv_sec);
        mtv.tv_usec = tv.tv_usec;
        timeval_to_amiga (&mtv, &days, &mins, &ticks);
    }
    else {
        struct mytimeval mtv2;
        mtv2.tv_sec = tv->tv_sec + fs_get_local_time_offset(tv->tv_sec);
        mtv2.tv_usec = tv->tv_usec;
        timeval_to_amiga(&mtv2, &days, &mins, &ticks);
    }

    if (g_fsdb_debug) {
        write_log("fsdb_set_file_time nname is %s\n", name);
    }
    if (!fs_path_exists(name)) {
        write_log("WARNING: fsdb_set_file_time file \"%s\" does not exist\n",
                name);
        my_errno = ERROR_OBJECT_NOT_AROUND;
        return 0;
    }

    fsdb_file_info info;
    fsdb_get_file_info(name, &info);
    info.days = days;
    info.mins = mins;
    info.ticks = ticks;
    my_errno = fsdb_set_file_info(name, &info);
    return my_errno == 0;
}

extern unsigned char g_latin1_lower_table[256];

static void lower_latin1(char *s) {
    unsigned char *u = (unsigned char*) s;
    while (*u) {
        *u = g_latin1_lower_table[*u];
        u++;
    }
}

int fsdb_fill_file_attrs(a_inode *base, a_inode *aino) {
    if (g_fsdb_debug) {
        write_log("fsdb_fill_file_attrs nname is %s\n", aino->nname);
    }
    fsdb_file_info info;
    fsdb_get_file_info(aino->nname, &info);
    if (!info.type) {
        // file does not exist
        return 0;
    }
    aino->dir = info.type == 2;
    aino->amigaos_mode = filesys_parse_mask(info.mode);
    if (info.comment) {
        aino->comment = nname_to_aname(info.comment, 1);
        free(info.comment);
    }
    else {
        aino->comment = NULL;
    }
    return 1;
}

int fsdb_set_file_attrs(a_inode *aino) {
    if (g_fsdb_debug) {
        write_log("fsdb_set_file_attrs nname is %s\n", aino->nname);
    }
    if (!fs_path_exists(aino->nname)) {
        write_log("WARNING: fsdb_set_file_attrs file \"%s\" does not exist\n",
                aino->nname);
        return ERROR_OBJECT_NOT_AROUND;
    }
    fsdb_file_info info;
    fsdb_get_file_info(aino->nname, &info);
    info.mode = filesys_parse_mask(aino->amigaos_mode);
    if (info.comment) {
        free(info.comment);
        info.comment = NULL;
    }
    if (aino->comment && aino->comment[0]) {
        if (g_fsdb_debug) {
            write_log("- setting comment to %s\n", aino->comment);
        }
        info.comment = aname_to_nname(aino->comment, 1);
    }
    return fsdb_set_file_info(aino->nname, &info);
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

    //gsize read, written;
    //gchar *cmp_name = g_convert(*name, -1, "ISO-8859-1", "UTF-8", &read,
    //        &written, NULL);
    char *cmp_name = fs_utf8_to_latin1(*name, -1);
    if (cmp_name == NULL) {
        write_log("WARNING: could not convert to latin1: %s", *name);
        return;
    }
    lower_latin1(cmp_name);

    const char *result;
    while (1) {
        result = fs_dir_read_name(dir);
        if (!result) {
            break;
        }

        //char *cmp_result = g_convert(result, -1, "ISO-8859-1", "UTF-8", &read,
        //        &written, NULL);
        char *cmp_result = fs_utf8_to_latin1(result, -1);
        //printf("%s %s\n", result, cmp_result);
        if (cmp_result == NULL) {
            // file name could not be represented as ISO-8859-1, so it
            // will be ignored
            write_log("cannot convert name \"%s\" to ISO-8859-1 - ignoring\n",
                    result);
            continue;
        }
        lower_latin1(cmp_result);

        if (strcmp(cmp_name, cmp_result) == 0) {
            // FIXME: memory leak, free name first?
            *name = fs_strdup(result);
            if (g_fsdb_debug) {
                write_log("              %s\n", *name);
            }
            break;
        }
        free(cmp_result);
    }
    fs_dir_close(dir);
    free(cmp_name);
}

char *fsdb_native_path(const char *root_dir, const char *amiga_path) {
    if (g_fsdb_debug) {
        write_log("fsdb_native_path (%s) %s\n", root_dir, amiga_path);
    }
    // splitting / allocating strings is not the most efficient way to do it,
    // but the code gets very readable...
    char *current_path = fs_strdup(root_dir);
    char **parts = fs_strsplit(amiga_path, "/", 0);
    char **part = parts;
    while (*part) {
        if (g_fsdb_debug) {
            write_log("-  %s\n", *part);
        }
        char *nname = aname_to_nname(*part, 0);
        find_nname_case(current_path, &nname);
        char *free_me = current_path;
        current_path = fs_path_join(current_path, nname, NULL);
        free(free_me);
        free(nname);
        part++;
    }
    fs_strfreev(parts);
    return current_path;
}

a_inode *custom_fsdb_lookup_aino_aname(a_inode *base, const TCHAR *aname) {
    //STUB("base=? aname=\"%s\"", aname);

    char *nname = aname_to_nname(aname, 0);
    find_nname_case(base->nname, &nname);
    char *full_nname = build_nname(base->nname, (TCHAR*) nname);
    fsdb_file_info info;
    fsdb_get_file_info(full_nname, &info);
    if (!info.type) {
        if (g_fsdb_debug) {
            write_log("custom_fsdb_lookup_aino_aname aname = %s "
                    "(does not exist)\n", aname);
        }
        if (info.comment) {
            free(info.comment);
            info.comment = NULL;
        }
        free(full_nname);
        free(nname);
        return NULL;
    }
    a_inode *aino = xcalloc (a_inode, 1);

    aino->aname = nname_to_aname(nname, 0);
    free(nname);
    aino->nname = full_nname;
    if (info.comment) {
        aino->comment = nname_to_aname(info.comment, 1);
        free(info.comment);
    }
    else {
        aino->comment = NULL;
    }
    aino->amigaos_mode = filesys_parse_mask(info.mode);
    aino->dir = info.type == 2;
    aino->has_dbentry = 0;
    aino->dirty = 0;
    aino->db_offset = 0;
    if (g_fsdb_debug) {
        write_log("custom_fsdb_lookup_aino_aname aname = %s "
                "(full_nname = %s)\n", aname, full_nname);
    }
    return aino;
}

a_inode *custom_fsdb_lookup_aino_nname(a_inode *base, const TCHAR *nname) {
    //STUB("base=? nname=\"%s\"", nname);

    char *full_nname = build_nname(base->nname, (TCHAR*) nname);
    fsdb_file_info info;
    fsdb_get_file_info(full_nname, &info);
    if (!info.type) {
        if (info.comment) {
            free(info.comment);
            info.comment = NULL;
        }
        free(full_nname);
        return NULL;
    }
    a_inode *aino = xcalloc (a_inode, 1);
    aino->aname = nname_to_aname(nname, 0);
    aino->nname = full_nname;
    if (info.comment) {
        aino->comment = nname_to_aname(info.comment, 1);
        free(info.comment);
    }
    else {
        aino->comment = NULL;
    }
    aino->amigaos_mode = filesys_parse_mask(info.mode);
    aino->dir = info.type == 2;
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
