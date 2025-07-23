#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sysconfig.h"
#include "sysdeps.h"

#include <fs/fs.h>

#include "fsdb.h"
#include "uae/fs.h"
#include "uae/glib.h"
#include "options.h"
#include "filesys.h"
#include "zfile.h"
#include <fs/util.h>
#include <unistd.h>
#ifdef WINDOWS
#include <Windows.h>
#endif
#include "fsdb_host.h"
#include "uae.h"

int g_fsdb_debug = 0;

struct my_opendir_s {
    //GDir *dir;
    char *path;
    GList *items;
    GList *current;
};

struct my_openfile_s {
    int fd;
    char *path;
};

int my_errno = 0;

bool my_chmod (const TCHAR *name, uae_u32 mode) {
    STUB("");
    // return result of mystat so invalid file will return false
    struct mystat ms;
    return my_stat(name, &ms);
}

bool my_stat (const TCHAR *name, struct mystat *ms) {
    struct fs_stat sonuc;
    if (fs_stat(name, &sonuc) == -1) {
        write_log("my_stat: stat on file %s failed\n", name);
        return false;
    }
    if (S_ISDIR(sonuc.mode)) {
        if (g_fsdb_debug) {
            write_log("my_stat return 512 as stat size for directory\n");
        }
        ms->size = 0;
    } else {
        if (g_fsdb_debug) {
            write_log("fs_stat returned size %jd\n", sonuc.size);
        }
        ms->size = sonuc.size;
    }
    // FIXME: read mode more accurately
    // FIXME: Mode and mtime are not used by filesys in FS-UAE
    // (but disk.cpp might use it)
    ms->mode = 0;
    if (sonuc.mode & S_IRUSR) {
        ms->mode |= FILEFLAG_READ;
    }
    if (sonuc.mode & S_IWUSR) {
        ms->mode |= FILEFLAG_WRITE;
    }
    ms->mtime.tv_sec = sonuc.mtime;
    ms->mtime.tv_usec = 0;
    return true;
}

static int compare_strings(const void *a, const void *b) {
    return strcmp((const char *) a, (const char *) b);
}

struct my_opendir_s *my_opendir(const TCHAR *name, const TCHAR *mask) {
    if (g_fsdb_debug) {
        write_log("my_opendir %s\n", name);
    }
    // mask is ignored
    if (mask && strcmp(mask, "*.*") != 0) {
        write_log("WARNING: directory mask was not *.*");
    }
    GDir *dir = g_dir_open(name, 0, NULL);
    if (!dir) {
        my_errno = errno;
        write_log("my_opendir %s failed\n", name);
        return NULL;
    }

    struct my_opendir_s *mod = g_new(struct my_opendir_s, 1);
    //mod->dir = dir;
    mod->path = g_strdup(name);
    mod->items = NULL;

    const char *result;
    while (1) {
        result = g_dir_read_name(dir);
        if (!result) {
            break;
        }
        int len = strlen(result);

        if (strcasecmp(result, "_UAEFSDB.___") == 0) {
            continue;
        }
        if (strcasecmp(result, "Thumbs.db") == 0) {
            continue;
        }
        if (strcasecmp(result, ".DS_Store") == 0) {
            continue;
        }
        if (strcasecmp(result, "UAEFS.ini") == 0) {
            continue;
        }
        if (len > 5 && strncmp(result + len - 5, ".uaem", 5) == 0) {
            // ignore metadata / attribute files, obviously
            continue;
        }

        char *cresult = fs_utf8_to_latin1(result, -1);
        if (cresult == NULL) {
            // file name could not be represented as ISO-8859-1, so it
            // will be ignored
            write_log("ignoring file %s (cannot be represented in "
                    "ISO-8859-1)\n", result);
            continue;
        }
        g_free(cresult);

        mod->items = g_list_append(mod->items, g_strdup(result));
    }
    mod->items = g_list_sort(mod->items, compare_strings);
    mod->current = mod->items;
    g_dir_close(dir);
    return mod;
}

struct my_opendir_s *my_opendir(const TCHAR *name) {
    return my_opendir(name, _T("*.*"));
}

void my_closedir(struct my_opendir_s* mod) {
    if (g_fsdb_debug) {
        write_log("my_closedir (%s)\n", mod->path);
    }
    my_errno = 0;
    if (mod) {
        g_free(mod->path);
        GList *item = mod->items;
        while (item) {
            g_free(item->data);
            item = item->next;
        }
        g_list_free(mod->items);
        g_free(mod);
    }
}

int my_readdir(struct my_opendir_s* mod, TCHAR* name) {
    if (mod->current) {
        strcpy(name, (const char*) mod->current->data);
        if (g_fsdb_debug) {
            write_log("my_readdir => %s\n", name);
        }
        mod->current = mod->current->next;
        return 1;
    }
    // no more items
    return 0;
}

int my_existsfile(const char *name) {
    if (g_fsdb_debug) {
        write_log("my_existsfile %s\n", name);
    }
    return fs_path_is_file(name);
}

int my_existsdir(const char *name) {
    if (g_fsdb_debug) {
        write_log("my_existsdir %s\n", name);
    }
    return fs_path_is_dir(name);
}

uae_s64 my_fsize(struct my_openfile_s* mos) {
    struct fs_stat sonuc;
    if (fs_fstat(mos->fd, &sonuc) == -1) {
        write_log("my_fsize: fstat on file %s failed\n", mos->path);
        return -1;
    }
    return sonuc.size;
}

int my_getvolumeinfo(const char *root) {
    if (g_fsdb_debug) {
        write_log("my_getvolumeinfo %s\n", root);
    }
    // FIXME: check what this is supposed to do
    write_log("my_getvolumeinfo %s\n", root);
    struct fs_stat sonuc;
    int ret = 0;


    if (fs_stat(root, &sonuc) == -1) {
        return -1;
    }
    if (!S_ISDIR(sonuc.mode)) {
        return -1;
    }

    ret |= MYVOLUMEINFO_STREAMS;

    return ret;
}

struct my_openfile_s *my_open(const TCHAR *name, int flags) {
    if (g_fsdb_debug) {
        write_log("my_open %s flags=%d\n", name, flags);
    }

    int open_flags = O_BINARY;
    if (flags & O_TRUNC) {
        open_flags = open_flags | O_TRUNC; //write_log("  O_TRUNC\n");
    }
    if (flags & O_CREAT) {
        open_flags = open_flags | O_CREAT; //write_log("  O_CREAT\n");
    }
    if (flags & O_RDWR) {
        open_flags = open_flags | O_RDWR; //write_log("  O_RDRW\n");
    }
    else if (flags & O_RDONLY) {
        open_flags = open_flags | O_RDONLY; //write_log("  O_RDONLY\n");
    }
    else if (flags & O_WRONLY) {
        open_flags = open_flags | O_WRONLY; //write_log("  O_WRONLY\n");
    }
    char *path = uae_expand_path(name);

    int file_existed = fs_path_exists(path);
    int file = g_open(path, open_flags, 0644);
    if (file == -1) {
        my_errno = errno;
        write_log("WARNING: my_open could not open (%s, %d)\n", name,
                open_flags);
        if (open_flags & O_TRUNC) {
            write_log("  O_TRUNC\n");
        }
        if (open_flags & O_CREAT) {
            write_log("  O_CREAT\n");
        }
        if (open_flags & O_RDWR) {
            write_log("  O_RDWR\n");
        }
        else if (open_flags & O_RDONLY) {
            write_log("  O_RDONLY\n");
        }
        else if (open_flags & O_WRONLY) {
            write_log("  O_WRONLY\n");
        }
        free(path);
        return NULL;
    }
    if (!file_existed) {
        fsdb_file_info info;
        fsdb_init_file_info(&info);
        int error = fsdb_set_file_info(path, &info);
        if (error != 0) {
            if (g_fsdb_debug) {
                write_log("WARNING: fsdb_set_file_info error %d\n", error);
            }
        }
    }
    free(path);

    struct my_openfile_s *mos = g_new(struct my_openfile_s, 1);
    mos->fd = file;
    mos->path = g_strdup(name);
    my_errno = 0;
    return mos;
}

void my_close(struct my_openfile_s* mos) {
    if (g_fsdb_debug) {
        //write_log("my_close %d (%s)\n", mos->fd, mos->path);
        write_log("my_close (%s)\n", mos->path);
    }
    errno = 0;
    free(mos->path);
#ifdef WINDOWS
    int result = _close(mos->fd);
#else
    int result = close(mos->fd);
#endif
    my_errno = errno;
    if (result != 0) {
        write_log("error closing file\n");
#ifdef WINDOWS
        write_log("Windows error code %lu\n", GetLastError());
#endif
    }
    g_free(mos);
}

unsigned int my_read(struct my_openfile_s *mos, void *b, unsigned int size) {
    ssize_t bytes_read = read(mos->fd, b, size);
    if (bytes_read == -1) {
        my_errno = errno;
        write_log("WARNING: my_read failed (-1)\n");
        // can only return 0 since return type is unsigned...
        return 0;
    }
    my_errno = 0;
    if (g_fsdb_debug) {
        //write_log("my_read fd=%d buffer=%p size=%d => %zd\n", mos->fd, b,
        //        size, bytes_read);
        write_log("my_read size=%d => %zd\n", size, bytes_read);
    }
    return (unsigned int) bytes_read;
}

unsigned int my_write(struct my_openfile_s *mos, void *b, unsigned int size) {
    ssize_t bytes_written = write(mos->fd, b, size);
    if (bytes_written == -1) {
        my_errno = errno;
        write_log("WARNING: my_write failed (-1) fd=%d buffer=%p size=%d\n",
                mos->fd, b, size);
        // can only return 0 since return type is unsigned...
        write_log("errno %d\n", my_errno);
        write_log("  mos %p -> fd=%d\n", mos, mos->fd);
        //write_log("  GetLastError = %d\n", GetLastError());
        return 0;
    }
    my_errno = 0;
    if (g_fsdb_debug) {
        write_log("my_write fd=%d buffer=%p size=%d => %zd\n", mos->fd, b,
                size, bytes_written);
    }
    return (unsigned int) bytes_written;
}

int my_mkdir(const TCHAR *path) {
    if (g_fsdb_debug) {
        write_log("my_mkdir %s\n", path);
    }
    int error = g_mkdir(path, 0755);
    if (error) {
        my_errno = errno;
        return -1;
    }
    int file_existed = 0;
    if (!file_existed) {
        fsdb_file_info info;
        fsdb_init_file_info(&info);
        int error = fsdb_set_file_info(path, &info);
        if (error != 0) {
            if (g_fsdb_debug) {
                write_log("WARNING: fsdb_set_file_info error %d\n", error);
            }
        }
    }
    my_errno = 0;
    return 0;
}

int my_truncate(const TCHAR *name, uae_u64 len) {
    int int_len = (int) len;
    if (g_fsdb_debug) {
        write_log("my_truncate %s len = %d\n", name, int_len);
    }
    struct my_openfile_s *mos = my_open(name, O_WRONLY);
    if (mos == NULL) {
        my_errno = errno;
        write_log("WARNING: opening file for truncation failed\n");
        return -1;
    }
#ifdef WINDOWS
    int result = _chsize(mos->fd, int_len);
#else
    int result = ftruncate(mos->fd, int_len);
#endif
    my_close(mos);
    my_errno = 0;
    return result;
}

static void remove_extra_file(const char *path, const char *name) {
    char *p = g_build_filename(path, name, NULL);
    g_unlink(p);
    g_free(p);
}

int my_rmdir(const TCHAR *path) {
    if (g_fsdb_debug) {
        write_log("my_rmdir %s\n", path);
    }
    remove_extra_file(path, "Thumbs.db");
    remove_extra_file(path, ".DS_Store");

    errno = 0;
    int result = g_rmdir(path);
    my_errno = errno;

    char *meta_name = g_strconcat(path, ".uaem", NULL);
    g_unlink(meta_name);
    g_free(meta_name);

    return result;
}

int my_unlink(const TCHAR *path) {
    if (g_fsdb_debug) {
        write_log("my_unlink %s\n", path);
    }
    errno = 0;
    int result = g_unlink(path);
    my_errno = errno;

    char *meta_name = g_strconcat(path, ".uaem", NULL);
    g_unlink(meta_name);
    g_free(meta_name);

    return result;
}

static int rename_file(const char *oldname, const char *newname) {
    int result = 0;
    for (int i = 0; i < 10; i++) {
        result = g_rename(oldname, newname);
        my_errno = errno;
        if (result == 0) {
            break;
        }
#ifdef WINDOWS
        write_log("Could not rename \"%s\" to \"%s\": Windows error"
                "code %lu\n", oldname, newname, GetLastError());
#endif
        sleep_millis(10);
    }
    return result;
}

int my_rename(const TCHAR *oldname, const TCHAR *newname) {
    if (g_fsdb_debug) {
        write_log("my_rename %s => %s\n", oldname, newname);
    }
    errno = 0;
    int result = rename_file(oldname, newname);
    if (result != 0) {
        // could not rename file
        return result;
    }

    char *oldname2 = g_strconcat(oldname, ".uaem", NULL);
    if (fs_path_exists(oldname2)) {
        char *newname2 = g_strconcat(newname, ".uaem", NULL);
        if (rename_file(oldname2, newname2) != 0) {
            // could not rename meta file, revert changes
            int saved_errno = my_errno;
            rename_file(newname, oldname);
            my_errno = saved_errno;
            result = -1;
        }
        g_free(newname2);
    }
    g_free(oldname2);

    return result;
}

uae_s64 my_lseek(struct my_openfile_s *mos, uae_s64 offset, int whence) {
    if (g_fsdb_debug) {
        write_log("my_lseek %s %lld %d\n", mos->path, offset, whence);
    }

    errno = 0;
    off_t result = lseek(mos->fd, offset, whence);
    my_errno = errno;
    if (g_fsdb_debug) {
        write_log("lseek result %jd\n", result);
    }
    return result;
}

FILE *my_opentext(const TCHAR* name) {
    if (g_fsdb_debug) {
        write_log("my_opentext %s\n", name);
    }
    // FIXME: WinUAE's version does some content checking related to unicode.
    // see fsdb_mywin32.cpp
    return g_fopen(name, "rb");
}

bool my_createshortcut(const char *source, const char *target, const char *description) 
{
	STUB("");
    return false;
}

bool my_resolvesoftlink(TCHAR *linkfile, int size, bool linkonly)
{
	STUB("");
	return false;
}

const TCHAR *my_getfilepart(const TCHAR *filename)
{
	const TCHAR *p;

	p = _tcsrchr(filename, '\\');
	if (p)
		return p + 1;
	p = _tcsrchr(filename, '/');
	if (p)
		return p + 1;
	return p;
}

int host_errno_to_dos_errno(int err) {
    static int warned = 0;

    switch (err) {
    case ENOMEM:
        return ERROR_NO_FREE_STORE;
    case EEXIST:
        return ERROR_OBJECT_EXISTS;
    case EACCES:
    case EROFS:
        return ERROR_WRITE_PROTECTED;
        //case ENOMEDIUM:
    case EINVAL:
    case ENOENT:
    case EBADF:
        return ERROR_OBJECT_NOT_AROUND;
    case ENOSPC:
        return ERROR_DISK_IS_FULL;
    case EBUSY:
        return ERROR_OBJECT_IN_USE;
    case ENOTEMPTY:
        return ERROR_DIRECTORY_NOT_EMPTY;
    case ESPIPE:
        return ERROR_SEEK_ERROR;
    default:
        if (!warned) {
            gui_message(_T("Unimplemented error %d\nContact author!"), err);
            warned = 1;
        }
        return ERROR_NOT_IMPLEMENTED;
    }
}

void my_canonicalize_path(const TCHAR *path, TCHAR *out, int size)
{
#if 0
	TCHAR tmp[MAX_DPATH];
	int v;
	v = GetLongPathName (path, tmp, sizeof tmp / sizeof (TCHAR));
	if (!v || v > sizeof tmp / sizeof (TCHAR)) {
		_tcsncpy (out, path, size);
		out[size - 1] = 0;
		return;
	}
	PathCanonicalize(out, tmp);
#endif
	STUB("");
	_tcsncpy (out, path, size);
	out[size - 1] = 0;
	return;
}

int my_issamevolume(const TCHAR *path1, const TCHAR *path2, TCHAR *path)
{
	TCHAR p1[MAX_DPATH];
	TCHAR p2[MAX_DPATH];
	unsigned int len, cnt;

	my_canonicalize_path(path1, p1, sizeof p1 / sizeof (TCHAR));
	my_canonicalize_path(path2, p2, sizeof p2 / sizeof (TCHAR));
	len = _tcslen (p1);
	if (len > _tcslen (p2))
		len = _tcslen (p2);
	if (_tcsnicmp (p1, p2, len))
		return 0;
	_tcscpy (path, p2 + len);
	cnt = 0;
	for (unsigned int i = 0; i < _tcslen (path); i++) {
		if (path[i] == '\\' || path[i] == '/') {
			path[i] = '/';
			cnt++;
		}
	}
	write_log (_T("'%s' (%s) matched with '%s' (%s), extra = '%s'\n"), path1, p1, path2, p2, path);
	return cnt;
}

int dos_errno(void) {
    if (g_fsdb_debug) {
        write_log(_T("dos_errno: my_errno=%d\n"), my_errno);
    }
    return host_errno_to_dos_errno(my_errno);
}

void filesys_host_init() {
    if (getenv("FS_DEBUG_FILESYS")) {
        g_fsdb_debug = 1;
    }
}
