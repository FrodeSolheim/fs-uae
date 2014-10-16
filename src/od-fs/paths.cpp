#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "traps.h"
#include "uaenative.h"
#include "uae/fs.h"
#include "uae.h"

char *g_libamiga_save_image_path = NULL;

static const char **g_native_library_dirs;
static char *g_module_ripper_dir = NULL;

const TCHAR **uaenative_get_library_dirs(void)
{
	if (g_native_library_dirs) {
		return g_native_library_dirs;
	}

	static const char *paths[1];
	return paths;
}

void fixtrailing(TCHAR *p)
{
	int len = strlen(p);
	if (len == 0) {
		return;
	}
	if (len > MAX_DPATH - 2) {
		/* No room for additional character (and terminating 0) */
		return;
	}
	if (p[len - 1] == '/') {
		return;
	}
#ifdef _WIN32
	if (p[len - 1] == '\\') {
		return;
	}
#endif
	p[len] = FSDB_DIR_SEPARATOR;
	p[len + 1] = '\0';
}

void getpathpart(TCHAR *outpath, int size, const TCHAR *inpath)
{
	strcpy(outpath, inpath);
	TCHAR *p = strrchr(outpath, '/');
#ifdef _WIN32
	if (!p) {
		p = strrchr(outpath, '\\');
	}
#endif
	if (p) {
		p[0] = 0;
	}
	fixtrailing(outpath);
}

void getfilepart(TCHAR *out, int size, const TCHAR *path)
{
	out[0] = 0;
	const TCHAR *p = strrchr(path, '/');
#ifdef _WIN32
	if (!p) {
		p = strrchr(path, '\\');
	}
#endif
	if (p) {
		strcpy(out, p + 1);
	} else {
		strcpy(out, path);
	}
}

// convert path to absolute or relative
void fullpath (TCHAR *path, int size)
{
	// FIXME: forward/backslash fix needed
	if (path[0] == 0 || (path[0] == '\\' && path[1] == '\\') ||
	    path[0] == ':') {
		return;
	}
	/* <drive letter>: is supposed to mean same as <drive letter>:\ */
}

TCHAR start_path_data[MAX_DPATH];

static void fetch_path (const TCHAR *name, TCHAR *out, int size)
{
	//int size2 = size;
	//printf("fetch_path %s\n", name);
	//_tcscpy (start_path_data, "./");
	_tcscpy (start_path_data, "");
	_tcscpy (out, start_path_data);
	/*
	   if (!name) {
	    return;
	   }
	   if (!_tcscmp (name, "FloppyPath")) {
	    _tcscat (out, "./");
	   }
	   else if (!_tcscmp (name, "CDPath")) {
	    _tcscat (out, "./");
	   }
	   else if (!_tcscmp (name, "hdfPath")) {
	    _tcscat (out, "./");
	   }
	   else if (!_tcscmp (name, "KickstartPath")) {
	    _tcscat (out, "./");
	   }
	   else if (!_tcscmp (name, "ConfigurationPath")) {
	    _tcscat (out, "./");
	   }
	 */
}

void fetch_saveimagepath (TCHAR *out, int size, int dir)
{
	if (g_libamiga_save_image_path) {
		_tcscpy(out, g_libamiga_save_image_path);
	} else {
		_tcscpy(out, _T(""));
	}
}

void fetch_configurationpath (TCHAR *out, int size)
{
	fetch_path("ConfigurationPath", out, size);
}

void fetch_luapath (TCHAR *out, int size)
{
	fetch_path (_T("LuaPath"), out, size);
}

void fetch_screenshotpath (TCHAR *out, int size)
{
	fetch_path("ScreenshotPath", out, size);
}

void fetch_ripperpath (TCHAR *out, int size)
{
	if (g_module_ripper_dir) {
		_tcscpy(out, g_module_ripper_dir);
		fixtrailing(out);
	} else {
		_tcscpy(out, _T(""));
	}
}

void fetch_statefilepath (TCHAR *out, int size)
{
	fetch_path("StatefilePath", out, size);
}

void fetch_inputfilepath (TCHAR *out, int size)
{
	fetch_path("InputPath", out, size);
}

void fetch_datapath (TCHAR *out, int size)
{
	fetch_path (NULL, out, size);
}

void fetch_rompath(TCHAR *out, int size)
{
	int k = 0;
	while (k < (MAX_PATHS - 1) && currprefs.path_rom.path[k + 1][0]) {
		k += 1;
	}
	uae_tcslcpy(out, currprefs.path_rom.path[k], size);
}

extern "C" {

void amiga_set_native_library_dirs(const char **library_dirs)
{
	g_native_library_dirs = library_dirs;
}

void amiga_set_save_image_dir(const char *path)
{
	write_log("amiga_set_save_image_dir %s\n", path);
	g_libamiga_save_image_path = strdup(path);
}

void amiga_set_module_ripper_dir(const char *path)
{
	write_log("amiga_set_module_ripper_dir %s\n", path);
	g_module_ripper_dir = strdup(path);
}

} // extern "C"
