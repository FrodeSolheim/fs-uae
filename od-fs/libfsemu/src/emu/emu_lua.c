#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WITH_LUA

#include <fs/emu.h>
#include "emu_lua.h"
#include <fs/log.h>
#include <fs/thread.h>
#include <lualib.h>

static fs_mutex *g_mutex;
lua_State *fs_emu_lua_state = NULL;

void fs_emu_lua_run_handler(const char *name) {
    lua_State **L = &fs_emu_lua_state;
    while(*L) {
        fs_emu_acquire_lua();
        lua_getglobal(*L, name);
        if (lua_isnil(*L, -1)) {
            //lua_pop(*L, 1);
        }
        else if (lua_pcall(*L, 0, 0, 0) != 0) {
            fs_emu_lua_log_error(name);
            //lua_pop(*L, 1);
        }
        lua_settop(*L, 0);
        fs_emu_release_lua();

        //L++;
        break;
    }
}

lua_State *fs_emu_get_lua_state(void) {
    return fs_emu_lua_state;
}

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

void fs_emu_lua_log_error(const char *msg) {
    fs_log("%s: %s\n", msg, lua_tostring(fs_emu_lua_state, -1));
    printf("%s: %s\n", msg, lua_tostring(fs_emu_lua_state, -1));
}

void fs_emu_acquire_lua(void) {
    fs_mutex_lock(g_mutex);
}

void fs_emu_release_lua(void) {
    fs_mutex_unlock(g_mutex);
}

void fs_emu_lua_init(void) {
    fs_log("fs_emu_lua_init\n");
    fs_emu_lua_state = luaL_newstate();
    fs_log("new lua_State at %p\n", fs_emu_lua_state);
    luaL_openlibs(fs_emu_lua_state);
    lua_register(fs_emu_lua_state, "fs_emu_log", l_fs_emu_log);
    g_mutex = fs_mutex_create();
}

#else

void fs_emu_lua_run_handler(const char *name) {
    // do nothing
}

#endif
