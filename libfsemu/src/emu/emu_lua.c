#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WITH_LUA

#include <string.h>

#include <fs/emu.h>
#include <fs/emu_lua.h>
#include <fs/log.h>
#include <fs/thread.h>
#include "lua_shell.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <lualib.h>
#ifdef __cplusplus
}
#endif

#define MAX_LUA_FUNCS   32
typedef struct lua_func {
    const char *name;
    fs_emu_lua_func func;
} lua_func;

#define MAX_POSTPONED_HANDLERS     16
static const char *g_postponed_handlers[MAX_POSTPONED_HANDLERS];
static int g_num_postponed_handlers;

static int g_num_func = 0;
static lua_func g_func_table[MAX_LUA_FUNCS];
static fs_emu_lua_binding *g_binding = NULL;
static int g_is_bound = 0;

static int l_fs_emu_log(lua_State *L) {
    int n = lua_gettop(L);
    if (n != 1) {
        lua_pushstring(L, "incorrect argument");
        lua_error(L);
    }

    const char *s = luaL_checkstring(L, 1);
    fs_emu_log_string(s);
    return 0;
}

void fs_emu_lua_init(void) {
    fs_log("lua-fs: init\n");
    fs_emu_lua_register_func("log", l_fs_emu_log);
}

void fs_emu_lua_set_binding(fs_emu_lua_binding *b)
{
    g_binding = b;
}

void fs_emu_lua_bind(void)
{
    if((g_binding != NULL) && !g_is_bound) {
        g_is_bound = 1;
        fs_log("lua-fs: bound to emu\n");

        // recall postponed handlers?
        for(int i=0;i<g_num_postponed_handlers;i++) {
            const char *name = g_postponed_handlers[i];
            fs_log("lua-fs: calling postponed handler '%s'\n", name);
            g_binding->run_handler(name);
        }
        g_num_postponed_handlers = 0;

        // setup lua shell if configured
        fs_emu_lua_shell_init();
    } else {
        fs_log("lua-fs: ERROR binding to emu!\n");
    }
}

void fs_emu_lua_unbind(void)
{
    if(g_is_bound) {
        // shutdown lua shell if configured
        fs_emu_lua_shell_free();

        g_is_bound = 0;
        fs_log("lua-fs: unbound from emu\n");
    } else {
        fs_log("lua-fs: ERROR unbinding from emu!\n");
    }
}

void fs_emu_lua_register_func(const char *name, fs_emu_lua_func func)
{
    if(g_num_func < MAX_LUA_FUNCS) {
        lua_func *f = &g_func_table[g_num_func++];
        f->name = name;
        f->func = func;
        fs_log("lua-fs: registered function '%s'\n", name);
    } else {
        fs_log("lua-fs: ERROR registering function '%s'\n", name);
    }
}

int luaopen_fsemulib(lua_State *L)
{
    fs_log("lua-fs: setup state %p with %d 'fsemu' functions\n", L, g_num_func);

    // create "fsemu" table
    lua_newtable(L);
    for(int i=0;i<g_num_func;i++) {
        lua_func *f = &g_func_table[i];
        lua_pushstring(L, f->name);
        lua_pushcfunction(L, f->func);
        lua_settable(L, -3);
    }
    return 1;
}

lua_State *fs_emu_lua_create_state(void)
{
    if(g_is_bound) {
        return g_binding->create_state();
    } else {
        fs_log("lua-fs: not bound: can't create state\n");
        return NULL;
    }
}

void fs_emu_lua_destroy_state(lua_State *state)
{
    if(g_is_bound) {
        g_binding->destroy_state(state);
    } else {
        fs_log("lua-fs: not bound: can't destroy state\n");
    }
}

void fs_emu_lua_lock_state(lua_State *state)
{
    if(g_is_bound) {
        g_binding->lock_state(state);
    } else {
        fs_log("lua-fs: not bound: can't lock state\n");
    }
}

void fs_emu_lua_unlock_state(lua_State *state)
{
    if(g_is_bound) {
        g_binding->unlock_state(state);
    } else {
        fs_log("lua-fs: not bound: can't unlock state\n");
    }
}

int fs_emu_lua_run_handler(const char *name) {
    if(g_is_bound) {
        return g_binding->run_handler(name);
    } else {
        // try to postpone handler
        for(int i=0;i<g_num_postponed_handlers;i++) {
            // already postponed?
            if(strcmp(g_postponed_handlers[i], name)==0) {
                return 0;
            }
        }
        // no more slots free?
        if(g_num_postponed_handlers == MAX_POSTPONED_HANDLERS) {
            fs_log("lua-fs: ignoring handler '%s'. no slots free!\n", name);
            return -1;
        }
        // store in slot
        g_postponed_handlers[g_num_postponed_handlers++] = name;
        fs_log("lua-fs: not bound: postpone handler '%s'\n", name);
        return 0;
    }
}

int fs_emu_lua_run_script(const char *path)
{
    if(g_is_bound) {
        return g_binding->run_script(path);
    } else {
        fs_log("lua-fs: not bound: ignoring script '%s'\n", path);
        return 0;
    }
}

#else

int fs_emu_lua_run_handler(const char *name) {
    // do nothing
    return 0;
}

int fs_emu_lua_run_script(const char *path) {
    // do nothing
    return 0;
}

#endif
