/*
 * Where FS-UAE finds its Python code, the Python standard library, resources and plugins.
 *
 * The layout is chosen when FS-UAE is built (DIST_LAYOUT, from --enable-dist-layout), not guessed
 * at run time: the dist layout, which dists and development builds have, or the system install
 * layout. See GLOSSARY.md (Dist layout, System install layout).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// Python.h first, as Python asks (it sets feature macros), for PY_MINOR_VERSION.
#include <Python.h>

#define FSUAE_INTERNAL
#include "fsuae-path.h"

#include <SDL3/SDL.h>
#include <glib.h>
#include <string.h>

#include "fs/path.h"

static struct {
    bool initialized;
    const char *layout_name;
    char *python_dir;
    char *python_home;
    char *resources_dir;
    char *plugins_dir;
    char *python_zip;
    char *application_dir;
} module;

// -------------------------------------------------------------------------------------------------

#ifdef MACOS
// In an app bundle (the executable in Contents/MacOS), SDL_GetBasePath() is Contents/Resources/.
// Outside a bundle (od-fs/fs-uae, or a build directory's fs-uae), it is the executable's folder.
static bool in_app_bundle(const char *base_path)
{
    return g_str_has_suffix(base_path, "/Contents/Resources/");
}
#endif

#ifdef DIST_LAYOUT

// The Python home candidate is used only when it has the standard library (a dist). Otherwise the
// home is left unset: on Linux and macOS, Python then finds the standard library from where
// libpython was loaded (the env prefix's, or the system's); on Windows, the FS-UAE._pth file next
// to the executable decides (build/windows/dev_layout.py), which Python ignores when the home is
// set.
// Returns a copy of home (freed with SDL_free), or NULL.
static char *python_home_if_stdlib(const char *home)
{
    char *stdlib_dir = g_strdup_printf("%s/lib/python3.%d", home, PY_MINOR_VERSION);
    bool found = FS_IsDirectory(stdlib_dir);
    g_free(stdlib_dir);
    return found ? SDL_strdup(home) : NULL;
}

static void init_dist_layout(const char *base_path)
{
#ifdef MACOS
    if (in_app_bundle(base_path)) {
        // The bundle rule, as in the macOS dist (build/dist/assemble.sh) and the thin development
        // app (build/macos-thin-app.sh): the root is Contents/Resources, with the resources
        // directly in it, and the Python home is its Python folder.
        module.layout_name = "the dist layout (app bundle)";
        char *root_dir = g_canonicalize_filename(base_path, NULL);
        module.python_dir = FS_JoinPath(root_dir, "Python");
        module.python_home = python_home_if_stdlib(module.python_dir);
        module.resources_dir = g_strdup(root_dir);
        module.plugins_dir = FS_JoinPath(root_dir, "Plugins");
        g_free(root_dir);
        return;
    }
#endif
    module.layout_name = "the dist layout";
    // The root: the executable's folder, and on Windows two levels above it (the executable is in
    // Libs/<triplet>, with the proxy FS-UAE.exe at the root). In a development build, the root is
    // od-fs (od-fs/fs-uae, od-fs/Libs/<triplet>/FS-UAE.exe) or the build directory, where
    // od-fs/configure links Python and Resources to the source tree.
#ifdef _WIN32
    char *temp = FS_JoinPath(base_path, "..", "..");
    char *root_dir = g_canonicalize_filename(temp, NULL);
    SDL_free(temp);
#else
    char *root_dir = g_canonicalize_filename(base_path, NULL);
#endif
    module.python_dir = FS_JoinPath(root_dir, "Python");
    // Each platform's Python home is Libs/<triplet> (the standard library is in its lib), so that
    // the platforms in a combined dist (several dists unpacked into one directory) do not find each
    // other's standard library. On Windows, that is the executable's own folder.
    char *home = FS_JoinPath(root_dir, "Libs", HOST_TRIPLET);
    module.python_home = python_home_if_stdlib(home);
    SDL_free(home);
    module.resources_dir = FS_JoinPath(root_dir, "Resources");
    module.plugins_dir = FS_JoinPath(root_dir, "Plugins");
    g_free(root_dir);
}

#else // !DIST_LAYOUT

static void init_system_install_layout(const char *base_path)
{
    // A system install (./configure && make install): the Python code and resources are in the
    // prefix's share/<share dir name> (Makefile.am's install-data-local), and the system's Python
    // finds its own standard library.
    module.layout_name = "the system install layout";
    char *temp = FS_JoinPath(base_path, "..", "share", SHARE_DIR_NAME);
    char *share_dir = g_canonicalize_filename(temp, NULL);
    SDL_free(temp);
    module.python_dir = FS_JoinPath(share_dir, "python");
    module.python_home = NULL;
    module.resources_dir = FS_JoinPath(share_dir, "resources");
    module.plugins_dir = NULL;
    g_free(share_dir);
}

#endif // !DIST_LAYOUT

static void init_application_dir(const char *base_path)
{
#ifdef MACOS
    if (in_app_bundle(base_path)) {
        // Contents/Resources/ -> Contents -> the bundle -> the folder that contains it.
        char *contents = FS_GetParentPath(base_path);
        char *bundle = FS_GetParentPath(contents);
        module.application_dir = FS_GetParentPath(bundle);
        SDL_free(bundle);
        SDL_free(contents);
        return;
    }
#endif
    module.application_dir = SDL_strdup(base_path);
}

void fsuae_path_init(void)
{
    if (module.initialized) {
        return;
    }
    module.initialized = true;

    const char *base_path = SDL_GetBasePath();
    SDL_Log("SDL_GetBasePath() = %s", base_path ? base_path : "NULL");
    if (base_path == NULL) {
        // Relative to the working directory, then (and the layout check will say what is missing).
        base_path = "./";
    }
#ifdef DIST_LAYOUT
    init_dist_layout(base_path);
#else
    init_system_install_layout(base_path);
#endif
    module.python_zip = FS_JoinPath(module.python_dir, "FS-UAE.zip");
    init_application_dir(base_path);

    SDL_Log("Layout: %s", module.layout_name);
    SDL_Log("python_dir: %s", module.python_dir);
    SDL_Log("python_home: %s", module.python_home ? module.python_home : "NULL");
    SDL_Log("resources_dir: %s", module.resources_dir);
    SDL_Log("plugins_dir: %s", module.plugins_dir ? module.plugins_dir : "NULL");
    SDL_Log("application_dir: %s", module.application_dir);
}

const char *fsuae_path_get_layout_name(void)
{
    return module.layout_name;
}

const char *fsuae_path_get_python_dir(void)
{
    return module.python_dir;
}

const char *fsuae_path_get_python_home(void)
{
    return module.python_home;
}

const char *fsuae_path_get_resources_dir(void)
{
    return module.resources_dir;
}

const char *fsuae_path_get_plugins_dir(void)
{
    return module.plugins_dir;
}

const char *fsuae_path_get_python_zip(void)
{
    return module.python_zip;
}

const char *fsuae_path_get_application_dir(void)
{
    return module.application_dir;
}

// -------------------------------------------------------------------------------------------------

const char *fsuae_path_get_fs_uae_dat(void)
{
    static const char *path;
    if (!path) {
        const char *base_path = SDL_GetBasePath();
        path = FS_JoinPath(base_path ? base_path : "", "fs-uae.dat");
    }
    return path;
}
