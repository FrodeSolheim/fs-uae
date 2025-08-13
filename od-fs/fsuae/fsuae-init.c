#include "fsuae-init.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <Python.h>
#include <SDL3/SDL.h>
#include <glib.h>
#include <glib/gstdio.h>

#include "fsapp-main.h"
#include "fsemu-python.h"
#include "fslib-path.h"
#include "fsuae-main.h"
#include "fsuae-path.h"
#include "fsuae-plugins.h"

static FILE* g_fsuae_init_log_file = NULL;
SDL_Mutex* g_log_file_mutex;

// -------------------------------------------------------------------------------------------------

bool fsapp_development_mode;

static void init_development_mode(void) {
    const char* sdl_base_path = SDL_GetBasePath();
    SDL_assert_release(sdl_base_path != NULL);
    SDL_Log("SDL_GetBasePath() = %s\n", sdl_base_path);

    char check_path[FSLIB_PATH_MAX];
    int size = FSLIB_PATH_MAX;
    int pos = 0;
    pos += SDL_utf8strlcpy(check_path, SDL_GetBasePath(), size - pos);
    SDL_utf8strlcpy(check_path + pos, "fsuae/fsuae-main.c", size - pos);
    SDL_Log("Check for development mode (does %s exist?)", check_path);
    fsapp_development_mode = SDL_GetPathInfo(check_path, NULL);

    if (fsapp_development_mode) {
        SDL_Log("Yes, using development paths");
    } else {
        SDL_Log("No, using production mode paths");
    }
    SDL_setenv_unsafe("FSAPP_DEV_MODE", fsapp_development_mode ? "1" : "0", 1);
}

// -------------------------------------------------------------------------------------------------

char* fsapp_data_directory;

static void init_data_directory(void) {
    // char buffer[FSLIB_PATH_MAX];
    // int size = FSLIB_PATH_MAX;
    // int pos = 0;
    // pos += SDL_utf8strlcpy(buffer, SDL_GetBasePath(), size - pos);
    // SDL_strlcat

    // SDL_utf8strlcpy(check_path + pos, "fsuae/fsuae-main.c", size - pos);

    // SDL_strlcpy(buffer, SDL_GetBasePath(), FSLIB_PATH_MAX);

    // char *test_path = SDL_GetBasePath();

    // gchar *fsapp_data_directory;

    gchar* dir = g_canonicalize_filename(SDL_GetBasePath(), NULL);
    while (true) {
        gchar* portable_ini = g_build_filename(dir, "Portable.ini", NULL);
        SDL_Log("Checking path %s", portable_ini);
        bool found = g_file_test(portable_ini, G_FILE_TEST_EXISTS);
        if (found) {
            SDL_Log("Found potential portable directory!");
            FILE* f = g_fopen(portable_ini, "r");
            if (f != NULL) {
                char buffer[9];
                int read = fread(buffer, 1, 8, f);
                buffer[8] = '\0';
                if (read == 8 && strcmp(buffer, "# FS-UAE") == 0) {
                    SDL_Log("Portable.ini starts with # FS-UAE");
                    fsapp_data_directory = g_build_filename(dir, "Data", NULL);
                } else {
                    SDL_Log("Portable.ini does not start with # FS-UAE");
                }
            } else {
                SDL_Log("Could not open Portable.ini for reading");
            }
            g_free(portable_ini);
            break;
        }
        g_free(portable_ini);
        gchar* old_dir = dir;
        dir = g_path_get_dirname(old_dir);
        bool toplevel = strcmp(dir, old_dir) == 0;
        g_free(old_dir);
        if (toplevel) {
            break;
        }
        // printf("'%s' vs '%s'\n", dir, old_dir);
        // if (strcmp(dir, old_dir) == 0) {
        //     break;
        // }
    }
    g_free(dir);

    if (fsapp_data_directory == NULL) {
        const char* sdl_pref_path = SDL_GetPrefPath("", "FS-UAE");
        SDL_assert_release(sdl_pref_path != NULL);
        SDL_Log("SDL_GetPrefPath(...) = %s\n", sdl_pref_path);
        fsapp_data_directory = g_build_filename(sdl_pref_path, "Data", NULL);
    }

#if 0
    if (fsapp_development_mode) {
        fsapp_data_directory = g_build_filename(SDL_GetBasePath(), "Data", NULL);
    } else {
#ifdef MACOS
        fsapp_data_directory = g_build_filename(sdl_pref_path, "Data", NULL);
#else
#ifdef WINDOWS
        gchar* base_dir_temp = g_build_filename(SDL_GetBasePath(), "..", "..", "..", NULL);
#else
        gchar* base_dir_temp = g_build_filename(SDL_GetBasePath(), "..", NULL);
#endif
        gchar* base_dir = g_canonicalize_filename(base_dir_temp, NULL);
        g_free(base_dir_temp);

        SDL_Log("Base dir: %s", base_dir);

        fsapp_data_directory = g_build_filename(base_dir, "Data", NULL);
        g_free(base_dir);
#endif
    }
#endif

    SDL_Log("Data dir: %s", fsapp_data_directory);
    SDL_setenv_unsafe("FSAPP_DATA_DIR", fsapp_data_directory, 1);
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

    // fprintf(f, "%s\n", message);

    // fwrite
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
    if (fsapp_development_mode) {
        SDL_Log("Development mode\n");
#ifdef _WIN32
        const char* dll_directory = "C:\\msys64\\mingw64\\bin";
        SDL_Log("SetDllDirectory %s\n", dll_directory);
        SetDllDirectoryA(dll_directory);
        python_home = g_strdup("C:\\msys64\\mingw64");
#endif
        python_dir = g_build_filename(SDL_GetBasePath(), "python", NULL);
        python_home = NULL;
        resources_dir = g_build_filename(SDL_GetBasePath(), "resources", NULL);
    } else {
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
    }

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

    PyRun_SimpleString(
        "import sys\n"
        "print('sys.path =', end='')\n"
        "print(sys.path)\n"
        // "sys.path.insert(0, '.venv/lib/python3.13/site-packages')\n"
        // "sys.path.insert(0, '.')\n"
        // "sys.path.insert(0, 'python')\n"
        "from fsgui.init import init\n"
        "init()");

    PyRun_SimpleString(
        "from time import time,ctime\n"
        "print('Today is', ctime(time()))\n");

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
    init_data_directory();

    // Enable logging to file

    gchar* logs_dir = g_build_filename(fsapp_data_directory, "Logs", NULL);
    SDL_Log("Logs dir: %s", logs_dir);
    if (!fslib_path_exists(logs_dir)) {
        SDL_Log("Logs directory does not exist, creating!");
        if (!SDL_CreateDirectory(logs_dir)) {
            SDL_Log("Could not create logs directory '%s'", logs_dir);
        }
    }
    gchar* log_file = g_build_filename(logs_dir, "FS-UAE.log.txt", NULL);
    SDL_Log("Log file: %s", log_file);
    g_log_file_mutex = SDL_CreateMutex();
    g_fsuae_init_log_file = g_fopen(log_file, "w");
    if (g_fsuae_init_log_file != NULL) {
        SDL_SetLogOutputFunction(sdl_log_output_function, NULL);
    } else {
        SDL_Log("Could not open log file %s", log_file);
    }

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
