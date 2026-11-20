#ifndef FSUAE_PATH_H
#define FSUAE_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

// Finds where FS-UAE's files are, from the layout it was built for (DIST_LAYOUT, see configure.ac):
// the dist layout (folders relative to the executable, as a dist and a development build have
// them) or the system install layout (<exe>/../share/<share dir name>). Call once, after SDL_Init.
void fsuae_path_init(void);

// The layout FS-UAE was built for (and on macOS, whether it runs in an app bundle), for messages.
const char *fsuae_path_get_layout_name(void);

// The folder with FS-UAE's Python code, added to sys.path.
const char *fsuae_path_get_python_dir(void);

// The Python home (the folder whose lib/python3.X is the standard library), or NULL when it is not
// set: then Python finds its standard library itself (on Windows, through FS-UAE._pth).
const char *fsuae_path_get_python_home(void);

// The resources folder (fonts, images and so on).
const char *fsuae_path_get_resources_dir(void);

// The folder with plugins that come with FS-UAE, or NULL when the layout has none.
const char *fsuae_path_get_plugins_dir(void);

// The zip file with FS-UAE's byte-compiled Python code (in a dist), in the Python code folder.
const char *fsuae_path_get_python_zip(void);

// The application's folder, where side-by-side plugins are looked for: the executable's folder, and
// on macOS, in an app bundle, the folder that contains the bundle.
const char *fsuae_path_get_application_dir(void);

const char *fsuae_path_get_fs_uae_dat(void);

#ifdef __cplusplus
}
#endif

#endif /* FSUAE_PATH_H */
