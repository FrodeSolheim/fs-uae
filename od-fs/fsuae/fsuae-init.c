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
#include <glib/gstdio.h>

#include "FS/FS.h"
#include "fsapp-main.h"
#include "fsemu-python.h"
#include "fslib-path.h"
#include "fsuae-main.h"
#include "fsuae-path.h"
#include "fsuae-plugins.h"

static FILE* g_fsuae_init_log_file = NULL;
SDL_Mutex* g_log_file_mutex;

// -------------------------------------------------------------------------------------------------

const char* fsapp_development_dir = NULL;

static void init_development_mode(void) {
    char path[FS_MAX_PATH];
    FS_CopyPath(path, SDL_GetBasePath());
    SDL_Log("SDL_GetBasePath() = %s\n", path);
    int reset = strlen(path);
    FS_AppendDirName(path, "od-fs");
    if (!FS_PathExists(path)) {
        // Try one level up
        path[reset] = '\0';
        FS_ParentDir(path);
        reset = strlen(path);
        FS_AppendDirName(path, "od-fs");
    }

    if (FS_PathExists(path)) {
        path[reset] = '\0';
        fsapp_development_dir = strdup(path);
        // SDL_setenv_unsafe("FSAPP_DEVELOPMENT_DIR", path, 1);
    }
}

// -------------------------------------------------------------------------------------------------

const char* fsapp_cache_directory = NULL;
const char* fsapp_data_directory = NULL;

static bool check_portable_ini(const char* path) {
    FILE* f = g_fopen(path, "r");
    if (f != NULL) {
        char buffer[9];
        int read = fread(buffer, 1, 8, f);
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

static void use_base_directory(char* path) {
    int reset = strlen(path);
    FS_AppendDirName(path, "Data");
    fsapp_data_directory = strdup(path);
    path[reset] = '\0';
    FS_AppendDirName(path, "Cache");
    fsapp_cache_directory = strdup(path);
}

static void init_data_and_cache_directories(void) {
    char path[FS_MAX_PATH];
    FS_CopyPath(path, SDL_GetBasePath());  // Path ends with /
    while (true) {
        int reset = strlen(path);
        FS_AppendFileName(path, "Portable.ini");
        SDL_Log("Checking %s", path);
        if (FS_PathExists(path)) {
            SDL_Log("Found potential portable directory!");
            if (check_portable_ini(path)) {
                path[reset] = '\0';
                use_base_directory(path);
                break;
            }
        }
        path[reset] = '\0';
        if (!FS_ParentDir(path)) {
            // Got to top level
            break;
        }
    }

    if (fsapp_data_directory == NULL) {
        FS_CopyPath(path, SDL_GetPrefPath("", "FS-UAE"));
        SDL_Log("SDL_GetPrefPath(...) = %s\n", path);
        use_base_directory(path);
    }

    SDL_Log("   Data dir: %s", fsapp_data_directory);
    SDL_setenv_unsafe("FSAPP_DATA_DIR", fsapp_data_directory, 1);

    SDL_Log("  Cache dir: %s", fsapp_cache_directory);
    SDL_setenv_unsafe("FSAPP_CACHE_DIR", fsapp_cache_directory, 1);
}

// -------------------------------------------------------------------------------------------------

void SDLCALL sdl_log_output_function(void* userdata, int category, SDL_LogPriority priority,
                                     const char* message) {
    SDL_LogOutputFunction log_function = SDL_GetDefaultLogOutputFunction();
    log_function(userdata, category, priority, message);

    SDL_LockMutex(g_log_file_mutex);
    FILE* f = g_fsuae_init_log_file;
    if (f == NULL) {
        return;
    }

    fputs(message, f);
    fputc('\n', f);

    // Don't always flush, in order to avoid emulation thread having to wait for flush to finish
    // (not sure if that's a real problem or not). -But if we log a warning or error message,
    // definitively do flush!

    if (priority >= SDL_LOG_PRIORITY_WARN) {
        printf("Flushing!\n");
        fflush(f);
    }

    SDL_UnlockMutex(g_log_file_mutex);
}

// -------------------------------------------------------------------------------------------------

static bool init_python(int argc, char* argv[]) {
    SDL_Log("Initialize Python");

    for (int i = 0; i < argc; i++) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "ARG: %d %s\n", i, argv[i]);
    }

    fsemu_python_init();

    PyStatus status;

    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    config.isolated = 1;
    // PyConfig_SetString(&config, &config.home, value);

    char* python_home = NULL;
    char* python_dir = NULL;
    char* resources_dir = NULL;

    // FIXME: fslib_misc / fslib_path ?
    if (fsapp_development_dir) {
        SDL_Log("Development mode\n");
#ifdef _WIN32
        const char* dll_directory = "C:\\msys64\\mingw64\\bin";
        SDL_Log("SetDllDirectory %s\n", dll_directory);
        SetDllDirectoryA(dll_directory);
        python_home = g_strdup("C:\\msys64\\mingw64");
#endif
        python_dir = g_build_filename(fsapp_development_dir, "od-fs", "python", NULL);
        python_home = NULL;
        resources_dir = g_build_filename(fsapp_development_dir, "od-fs", "resources", NULL);
    } else {
#ifdef PORTABLE_MODE

        // char temp[FSLIB_PATH_MAX];
        const char* temp = g_build_filename(SDL_GetBasePath(),
#if defined(WINDOWS)
                                            "..", "..",
#endif
                                            NULL);
        // #if defined(WINDOWS)
        //         SDL_strlcpy(temp, SDL_GetBasePath(), FSLIB_PATH_MAX);
        //         SDL_strlcat(temp, "..\\..\\Python", FSLIB_PATH_MAX);
        // #else  // FIXME: Same as MACOS actually
        //         SDL_strlcpy(temp, SDL_GetBasePath(), FSLIB_PATH_MAX);
        //         SDL_strlcat(temp, "Python", FSLIB_PATH_MAX);
        // #endif
        char* base_dir = g_canonicalize_filename(temp, NULL);
        python_dir = g_build_filename(base_dir, "Python", NULL);
        python_home = g_strdup(python_dir);
#ifdef MACOS
        resources_dir = g_strdup(base_dir);
#else
        resources_dir = g_build_filename(base_dir, "Resources", NULL);
#endif

#else  // !PORTABLE_MODE

        python_home = NULL;

        gchar* temp = g_build_filename(SDL_GetBasePath(), "..", "share", SHARE_DIR_NAME, NULL);
        gchar* share_fs_uae_dir = g_canonicalize_filename(temp, NULL);
        g_free(temp);
        resources_dir = g_build_filename(share_fs_uae_dir, "resources", NULL);
        python_dir = g_build_filename(share_fs_uae_dir, "python", NULL);
        g_free(share_fs_uae_dir);

#endif  // !PORTABLE_MODE
    }

    SDL_Log("python_dir: %s", python_dir ? python_dir : "NULL");
    SDL_Log("python_home: %s", python_home ? python_home : "NULL");
    SDL_Log("resources_dir: %s", resources_dir ? resources_dir : "NULL");

    // FIXME: Initialize elsewhere? (we're in init python now)
    // FIXME: Why does not SDL_setenv_unsafe work here...? g_setenv does work...
    g_setenv("FSAPP_RESOURCES_DIR", resources_dir, true);
    g_free(resources_dir);
    resources_dir = NULL;

    if (python_home != NULL) {
        SDL_Log("Setting Python Home to %s\n", python_home);
        PyConfig_SetBytesString(&config, &config.home, python_home);
        g_free(python_home);
        python_home = NULL;
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

    PyRun_SimpleString("print('Hello from Python')\n");

    PyObject* sys_path = PySys_GetObject("path");
    PyObject* str_object;

    const char* python_zip = fsuae_path_python_zip();
    SDL_Log("Adding zip file to python sys.path: %s", python_zip);
    // FIXME: This only works when the (glib) locale is UTF-8!?!
    str_object = PyUnicode_FromString(python_zip);
    PyList_Insert(sys_path, 0, str_object);

    // const char *fs_uae_dat = fsuae_path_fs_uae_dat();
    // SDL_Log("Adding dat file to python sys.path: %s", fs_uae_dat);
    // // FIXME: This only works when the (glib) locale is UTF-8!?!
    // str_object = PyUnicode_FromString(python_zip);
    // PyList_Insert(sys_path, 0, str_object);

#ifdef MACOS
#warning in non-development mode, do not add python dir to path?
#endif

    if (python_dir != NULL) {
        SDL_Log("Adding directory to python sys.path: %s", python_dir);
        // FIXME: This only works when the (glib) locale is UTF-8!?!
        // FIXME: Possibly just require UTF-8 as a locale on startup, and fail if not? Probably
        // not the only UTF-8 assumption around
        str_object = PyUnicode_FromString(python_dir);
        PyList_Insert(sys_path, 0, str_object);
        g_free(python_dir);
        python_dir = NULL;
    }

    // const char* python_dir = fsuae_path_python_dir();
    // SDL_Log("Adding directory to python sys.path: %s", python_dir);
    // // FIXME: This only works when the (glib) locale is UTF-8!?!
    // str_object = PyUnicode_FromString(python_dir);
    // PyList_Insert(sys_path, 0, str_object);

    PyRun_SimpleString("import sys; print('sys.path =', end=''); print(sys.path)\n");
    PyRun_SimpleString("from fsgui import init; init()");
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

void init_logging(void) {
    char path[FS_MAX_PATH];
    FS_CopyPath(path, fsapp_cache_directory);
    FS_AppendDirName(path, "Log");
    SDL_Log("Log dir: %s", path);
    if (!FS_PathExists(path)) {
        SDL_Log("Log directory does not exist, creating!");
        if (!SDL_CreateDirectory(path)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create directory '%s'", path);
        }
    }

    FS_AppendFileName(path, "FS-UAE.log.txt");
    SDL_Log("Log file: %s", path);
    g_log_file_mutex = SDL_CreateMutex();
    g_fsuae_init_log_file = g_fopen(path, "w");
    if (g_fsuae_init_log_file != NULL) {
        SDL_SetLogOutputFunction(sdl_log_output_function, NULL);
    } else {
        SDL_Log("Could not open log file %s", path);
    }
}

bool fsuae_init(int argc, char* argv[], int* error) {
    if (error != NULL) {
        *error = 0;
    }
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Determine development mode and directory/file structure

    init_development_mode();
    init_data_and_cache_directories();

    // Enable logging to file

    init_logging();

    // Modules must be initialized before calling fsemu_init, because that will
    // create the Python interpreter and Python module definitions must be
    // registered first.

    fsuae_main_init();
    fsuae_plugins_init();

    fsuae_path_init();

    // FIXME: This is only partially used...
    // g_setenv("FSAPP_DATA_DIR", fsuae_path_appdata_dir(), true);
    g_setenv("FSUAE_BASE_DIR", fsuae_path_base_dir(), true);
    g_setenv("FSUAE_SYSTEM_DIR", fsuae_path_system_dir(), true);

    // FIXME: Can also set data dir directly now?
    // fslib_data_init_directory() ?

    // FIXME: This is only partially used...
    // fslib_data_init_prefix("../../Data");
    // fslib_data_init_dev_prefix("System/FS-UAE/Data");
    // fslib_data_init_dev_test("od-fs/fsuae/fsuae-main.c");
    // fslib_data_init();

    // exit(1);

    // Initialize fsemu/fsapp - and implicitly, Python!

    if (!fsapp_init(argc, argv, error)) {
        return false;
    }

    if (!fsemu_init(argc, argv, error)) {
        return false;
    }

    if (!init_python(argc, argv)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not init Python\n");
        if (error != NULL) {
            *error = 3;
        }
        return false;
    }

    return true;
}

void fsuae_init_close_log_file() {
    SDL_LockMutex(g_log_file_mutex);
    if (g_fsuae_init_log_file != NULL) {
        SDL_Log("Closing log file\n");
        fclose(g_fsuae_init_log_file);
    }
    SDL_UnlockMutex(g_log_file_mutex);
}
