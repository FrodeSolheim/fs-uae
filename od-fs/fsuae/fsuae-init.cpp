#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fsuae-init.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <Python.h>
#include <SDL3/SDL.h>
#include <glib.h>

#include "fs/file.h"
#include "fs/log.h"
#include "fsapp-main.h"
#include "fsapp-time.h"

#define FSEMU_INTERNAL
#include "fs/path.h"
#include "fsemu-application.h"
#include "fsemu-audio.h"
#include "fsemu-frame.h"
#include "fsemu-input.h"
#include "fsemu-inputdevice.h"
#include "fsemu-inputport.h"
#include "fsemu-mainloop.h"
#include "fsemu-option.h"
#include "fsemu-python.h"
#include "fsuae-main.h"
#include "fsuae-path.h"
#include "fsuae-plugins.h"

// Defined in od-fs/3rdparty/lhafile/lzhlib.c (used by the lhafile Python package)
PyMODINIT_FUNC PyInit_lzhlib(void);

// -------------------------------------------------------------------------------------------------

static const char *g_cache_dir = NULL;
static const char *g_data_dir = NULL;

static bool check_portable_ini(const char *path)
{
    FILE *f = FS_OpenFile(path, "r");
    if (f != NULL) {
        char buffer[9];
        int read = fread(buffer, 1, 8, f);
        fclose(f);
        buffer[8] = '\0';
        if (read == 8 && strcmp(buffer, "# FS-UAE") == 0) {
            SDL_Log("Portable.ini starts with # FS-UAE");
            return true;
        } else {
            SDL_Log("Portable.ini does not start with # FS-UAE");
        }
    } else {
        SDL_Log("Could not open Portable.ini for reading");
    }
    return false;
}

static void use_base_directory(const char *path)
{
    g_data_dir = FS_JoinPath(path, "Data");
    g_cache_dir = FS_JoinPath(path, "Cache");
}

static void init_data_and_cache_directories(void)
{
    // The portable directory is the closest one, from the executable's directory upwards, with a
    // Portable.ini file starting with "# FS-UAE".
    const char *base_path = SDL_GetBasePath();
    char *dir = SDL_strdup(base_path ? base_path : "");
    while (dir[0] != '\0') {
        char *path = FS_JoinPath(dir, "Portable.ini");
        SDL_Log("Checking %s", path);
        bool portable = false;
        if (SDL_GetPathInfo(path, NULL)) {
            SDL_Log("Found potential portable directory!");
            portable = check_portable_ini(path);
        }
        SDL_free(path);
        if (portable) {
            use_base_directory(dir);
            break;
        }
        char *parent = FS_GetParentPath(dir);
        SDL_free(dir);
        dir = parent;
    }
    SDL_free(dir);

    if (g_data_dir == NULL) {
        char *pref_path = SDL_GetPrefPath("", "FS-UAE");
        SDL_Log("SDL_GetPrefPath(...) = %s\n", pref_path);
        // FIXME: SDL_GetPrefPath returns NULL when it cannot create the directory, which is not
        // handled.
        use_base_directory(pref_path);
        SDL_free(pref_path);
    }

    SDL_Log("   Data dir: %s", g_data_dir);
    SDL_setenv_unsafe("FSAPP_DATA_DIR", g_data_dir, 1);

    SDL_Log("  Cache dir: %s", g_cache_dir);
    SDL_setenv_unsafe("FSAPP_CACHE_DIR", g_cache_dir, 1);
}

// -------------------------------------------------------------------------------------------------

#ifdef _WIN32
// Python loads extension modules (.pyd) with LoadLibraryEx, which since Python 3.8 does not search
// PATH for their dependencies, only the executable's folder, the module's own folder, the system
// folders and folders added with AddDllDirectory. The executable in a native MSYS2 build directory
// (run from the MINGW64 shell) is not next to the DLLs (they are in the MinGW bin folder, found
// through PATH when the program starts), so a module whose DLL nothing else has loaded yet
// (_ctypes, which needs libffi-8.dll) fails to load. Adding the folder Python's own DLL was loaded
// from fixes that. In a dist and in a development build's Libs/<triplet>/, that is the
// executable's folder already.
// REVIEW: With the dist layout, the build directory's own fs-uae.exe no longer starts (its root
// would be two levels above it, see fsuae-path.cpp): Windows development builds run from
// Libs/<triplet>/ (build/windows/dev_layout.py). Kept as ticket 35 decided, for a Python DLL that
// is not next to the executable; remove it if nothing needs that any more.
static void add_python_dll_directory(void)
{
    // By name: the address of a Python function would be that of its import stub in the
    // executable. libpython3.X.dll is MinGW's name, python3X.dll MSVC's.
    wchar_t name[32];
    swprintf(name, 32, L"libpython%d.%d.dll", PY_MAJOR_VERSION, PY_MINOR_VERSION);
    HMODULE module = GetModuleHandleW(name);
    if (module == NULL) {
        swprintf(name, 32, L"python%d%d.dll", PY_MAJOR_VERSION, PY_MINOR_VERSION);
        module = GetModuleHandleW(name);
    }
    if (module == NULL) {
        SDL_Log("Could not find the Python DLL (error %lu)", GetLastError());
        return;
    }
    wchar_t path[MAX_PATH];
    DWORD length = GetModuleFileNameW(module, path, MAX_PATH);
    if (length == 0 || length == MAX_PATH) {
        SDL_Log("Could not get the path of the Python DLL (error %lu)", GetLastError());
        return;
    }
    wchar_t *separator = wcsrchr(path, L'\\');
    if (separator == NULL) {
        return;
    }
    *separator = L'\0';
    char *utf8 = g_utf16_to_utf8((const gunichar2 *)path, -1, NULL, NULL, NULL);
    if (AddDllDirectory(path) == NULL) {
        SDL_Log("Could not add %s to the DLL search (error %lu)", utf8 ? utf8 : "?",
                GetLastError());
    } else {
        SDL_Log("Added the Python DLL's folder to the DLL search: %s", utf8 ? utf8 : "?");
    }
    g_free(utf8);
}
#endif

static bool init_python(int argc, char *argv[])
{
    SDL_Log("Initialize Python");

    for (int i = 0; i < argc; i++) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "ARG: %d %s\n", i, argv[i]);
    }

    fsemu_python_init();
#ifdef _WIN32
    add_python_dll_directory();
#endif
    PyImport_AppendInittab("lzhlib", &PyInit_lzhlib);

    PyStatus status;

    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    config.isolated = 1;
    // PyConfig_SetString(&config, &config.home, value);

    // FIXME: Initialize elsewhere? (we're in init python now)
    // FIXME: Why does not SDL_setenv_unsafe work here...? g_setenv does work...
    g_setenv("FSAPP_RESOURCES_DIR", fsuae_path_get_resources_dir(), true);

    // The Python home, when the layout has the standard library (see fsuae_path_get_python_home).
    const char *python_home = fsuae_path_get_python_home();
    if (python_home != NULL) {
        SDL_Log("Setting Python Home to %s\n", python_home);
        PyConfig_SetBytesString(&config, &config.home, python_home);
    }

    // Optional but recommended, according to Python docs
    // status = PyConfig_SetBytesString(&config, &config.program_name,
    // argv[0]); if (PyStatus_Exception(status)) {
    //     PyConfig_Clear(&config);
    //     Py_ExitStatusException(status);
    //     return false;
    // }

    // char *argv2[] = {"example.py", "arg1", "arg2"};
    // status = PyConfig_SetBytesArgv(&config, 3, argv2);

    // printf("--- a ---\n");

    status = PyConfig_SetBytesArgv(&config, argc, argv);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        Py_ExitStatusException(status);
        // goto exception;
        return false;
    }

    // printf("--- b ---\n");

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        Py_ExitStatusException(status);
        return false;
    }
    PyConfig_Clear(&config);

    // printf("--- c ---\n");

    PyObject *sys_path = PySys_GetObject("path");
    PyObject *str_object;

    const char *python_zip = fsuae_path_get_python_zip();
    SDL_Log("Adding zip file to python sys.path: %s", python_zip);
    // FIXME: This only works when the (glib) locale is UTF-8!?!
    str_object = PyUnicode_FromString(python_zip);
    PyList_Insert(sys_path, 0, str_object);

    // const char *fs_uae_dat = fsuae_path_get_fs_uae_dat();
    // SDL_Log("Adding dat file to python sys.path: %s", fs_uae_dat);
    // // FIXME: This only works when the (glib) locale is UTF-8!?!
    // str_object = PyUnicode_FromString(python_zip);
    // PyList_Insert(sys_path, 0, str_object);

    // In a dist, the Python code folder only has FS-UAE.zip (added above), so adding the folder
    // itself does no harm; in a development build, it is the source tree's od-fs/Python.
    const char *python_dir = fsuae_path_get_python_dir();
    SDL_Log("Adding directory to python sys.path: %s", python_dir);
    // FIXME: This only works when the (glib) locale is UTF-8!?!
    // FIXME: Possibly just require UTF-8 as a locale on startup, and fail if not? Probably
    // not the only UTF-8 assumption around
    str_object = PyUnicode_FromString(python_dir);
    PyList_Insert(sys_path, 0, str_object);

    // Python's print, logging and tracebacks go to the log from here on (fsapp needs the path
    // above to import its Python side).
    fsapp_main_install_python_log();

    PyRun_SimpleString("print('Hello from Python')\n");
    PyRun_SimpleString("import sys; print('sys.path =', end=''); print(sys.path)\n");
    PyRun_SimpleString("import time; print('Today is', time.ctime(time.time()))\n");

    // PyEval_InitThreads();

    // // Must register custom events before starting Python thread
    // fsapp.SDL_EVENT_FSAPP_WINDOW = SDL_RegisterEvents(1);

    // // Release the GIL and allow the Python thread to use the Python API
    // fsapp.pythread_state = PyEval_SaveThread();
    // // FIXME: Maybe use thread names (such as this also for emulation thread,
    // // etc)
    // fsapp.python_thread = SDL_CreateThread(python_thread_function, "python", (void*)NULL);

    return true;
}

// -------------------------------------------------------------------------------------------------

// Opens the log file, by default FS-UAE.log.txt in the cache directory's Log folder (FS_LOG_FILE
// puts it elsewhere, fs/log.h). It gets every line logged since the start, the build header first.
static void open_log_file(void)
{
    char *dir = FS_JoinPath(g_cache_dir, "Log");
    if (!SDL_GetPathInfo(dir, NULL)) {
        SDL_Log("Creating the log directory %s", dir);
        if (!SDL_CreateDirectory(dir)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create directory '%s'", dir);
        }
    }
    char *path = FS_JoinPath(dir, "FS-UAE.log.txt");
    FS_OpenLogFile(path);
    SDL_free(path);
    SDL_free(dir);
}

// Checks that the Python code folder and the resources folder of the layout are there. If one is
// missing, logs an error naming the folder and the layout, shows it in a message box if it can, and
// returns false, so that FS-UAE stops at startup instead of failing later on a Python import.
static bool check_layout(void)
{
    const char *what = NULL;
    const char *missing = NULL;
    if (!FS_IsDirectory(fsuae_path_get_python_dir())) {
        what = "Python code";
        missing = fsuae_path_get_python_dir();
    } else if (!FS_IsDirectory(fsuae_path_get_resources_dir())) {
        what = "resources";
        missing = fsuae_path_get_resources_dir();
    } else {
        return true;
    }
    char *message = g_strdup_printf("FS-UAE cannot start: the %s folder is missing.\n\n"
                                    "Expected: %s\n\n"
                                    "FS-UAE was built for %s.",
                                    what, missing, fsuae_path_get_layout_name());
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", message);
    // Fails (and returns at once) when there is no display.
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "FS-UAE", message, NULL);
    g_free(message);
    return false;
}

// Set when fsemu has been initialized; the UAE glue checks it (declared in sysdeps.h).
int fsemu = 0;

static void init_fsemu(void)
{
    fsemu = 1;

    // The input modules, which also add their Python module definitions (before init_python).
    fsemu_input_init_module();
    fsemu_inputdevice_init_module();
    fsemu_inputport_init_module();

    fsapp_time_init();

    // Needed by fsemu_frame (maybe fsemu_frame should call it...)
    fsemu_option_init();

    fsemu_application_init();
    fsemu_application_set_name("FS-UAE");

    fsemu_audio_init();

    fsemu_frame_init();
    fsemu_input_init();

    // The emulation surface, and fsemu's hooks and pacer for fsapp's main loop.
    fsemu_mainloop_init();
}

bool fsuae_init(int argc, char *argv[], int *error)
{
    if (error != NULL) {
        *error = 0;
    }
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        if (error != NULL) {
            *error = 1;
        }
        return false;
    }

    init_data_and_cache_directories();

    // The log file, now that the cache directory is known.
    open_log_file();

    // Where FS-UAE's own files are (the layout it was built for), and stop now if they are not
    // there.

    fsuae_path_init();
    if (!check_layout()) {
        if (error != NULL) {
            *error = 1;
        }
        return false;
    }

    // Modules must be initialized before init_python creates the Python interpreter, because
    // Python module definitions must be registered first.

    fsuae_main_init();
    fsuae_plugins_init();

    // FIXME: Can also set data dir directly now?
    // fslib_data_init_directory() ?

    // FIXME: This is only partially used...
    // fslib_data_init_prefix("../../Data");
    // fslib_data_init_dev_prefix("System/FS-UAE/Data");
    // fslib_data_init_dev_test("od-fs/fsuae/fsuae-main.c");
    // fslib_data_init();

    // exit(1);

    // Initialize fsemu/fsapp - and implicitly, Python!

    if (!fsapp_main_init(argc, argv, error)) {
        return false;
    }

    init_fsemu();

    if (!init_python(argc, argv)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not init Python\n");
        if (error != NULL) {
            *error = 3;
        }
        return false;
    }

    return true;
}

const char *fsuae_init_get_data_dir(void)
{
    return g_data_dir;
}

const char *fsuae_init_get_cache_dir(void)
{
    return g_cache_dir;
}
