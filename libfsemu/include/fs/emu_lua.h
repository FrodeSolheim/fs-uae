#ifndef LIBFSEMU_EMU_LUA_H_
#define LIBFSEMU_EMU_LUA_H_

#ifdef WITH_LUA

#include <lauxlib.h>

typedef struct fs_emu_lua_binding {
    int (*run_handler)(const char *name);
    int (*run_script)(const char *path);
    lua_State *(*create_state)(void);
    void (*destroy_state)(lua_State *);
    void (*lock_state)(lua_State *);
    void (*unlock_state)(lua_State *);
} fs_emu_lua_binding;

typedef int (*fs_emu_lua_func)(lua_State *);

// bind lua interpreter
extern void fs_emu_lua_set_binding(fs_emu_lua_binding *);
extern void fs_emu_lua_bind(void);
extern void fs_emu_lua_unbind(void);

// register functions
extern void fs_emu_lua_init(void);
extern void fs_emu_lua_register_func(const char *name, fs_emu_lua_func f);
extern int luaopen_fsemulib(lua_State *state);

extern lua_State *fs_emu_lua_create_state(void);
extern void fs_emu_lua_destroy_state(lua_State *);
extern void fs_emu_lua_lock_state(lua_State *);
extern void fs_emu_lua_unlock_state(lua_State *);

#endif

extern int fs_emu_lua_run_handler(const char *name);
extern int fs_emu_lua_run_script(const char *path);

#endif // LIBFSEMU_EMU_LUA_H_
