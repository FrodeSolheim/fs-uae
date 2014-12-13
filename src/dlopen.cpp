#include "sysconfig.h"
#include "sysdeps.h"

#include "uae/dlopen.h"
#include "uae/log.h"

#ifdef _WIN32
#include "windows.h"
#else
#include <dlfcn.h>
#endif
#ifdef WINUAE
#include "od-win32/win32.h"
#endif

UAE_DLHANDLE uae_dlopen(const TCHAR *path)
{
	UAE_DLHANDLE result;
	if (path == NULL or path[0] == _T('\0')) {
		write_log(_T("DLOPEN: No path given\n"));
		return NULL;
	}
#ifdef _WIN32
	result = LoadLibrary(path);
#else
	result = dlopen(path, RTLD_NOW);
	const char *error = dlerror();
	if (error != NULL)  {
		write_log("DLOPEN: %s\n", error);
	}
#endif
	return result;
}

void *uae_dlsym(UAE_DLHANDLE handle, const char *name)
{
#if 0
	if (handle == NULL) {
		return NULL;
	}
#endif
#ifdef _WIN32
	return (void *) GetProcAddress(handle, name);
#else
	return dlsym(handle, name);
#endif
}

void uae_dlclose(UAE_DLHANDLE handle)
{
#ifdef _WIN32
	FreeLibrary (handle);
#else
	dlclose(handle);
#endif
}

#ifdef FSUAE
#include "uae/uae.h"
static amiga_plugin_lookup_function plugin_lookup;
#endif

UAE_DLHANDLE uae_dlopen_plugin(const TCHAR *name)
{
#ifdef FSUAE
	const TCHAR *path = NULL;
#if 1
	if (plugin_lookup) {
		path = plugin_lookup(name);
	}
#else
	TCHAR lib_name[MAX_DPATH] = {};
	_tcscat(lib_name, _T("lib"));
	_tcscat(lib_name, name);
	if (plugin_lookup) {
		path = plugin_lookup(lib_name);
	}
#endif
	if (path == NULL or path[0] == _T('\0')) {
		write_log(_T("DLOPEN: Could not find plugin \"%s\"\n"), name);
		return NULL;
	}
	UAE_DLHANDLE handle = uae_dlopen(path);
#else
	TCHAR path[MAX_DPATH] = {};
	_tcscat(path, name);
	_tcscat(path, _T(".dll"));
	UAE_DLHANDLE handle = WIN32_LoadLibrary(path);
#endif
	if (handle) {
		write_log(_T("DLOPEN: Loaded plugin %s\n"), path);
		uae_dlopen_patch_common(handle);
	}
	return handle;
}

void uae_dlopen_patch_common(UAE_DLHANDLE handle)
{
	write_log(_T("DLOPEN: Patching common functions\n"));
	void *ptr;
	ptr = uae_dlsym(handle, "uae_log");
	if (ptr) *((uae_log_function *) ptr) = &uae_log;
}

#ifdef FSUAE // NL
#ifdef __cplusplus
extern "C" {
#endif

void amiga_set_plugin_lookup_function(amiga_plugin_lookup_function function)
{
	plugin_lookup = function;
}

#ifdef __cplusplus
}
#endif
#endif /* FSUAE */
