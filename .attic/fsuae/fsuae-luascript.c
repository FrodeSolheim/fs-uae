#define FSUAE_INTERNAL
#include "fsuae-luascript.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/emu.h>
#include <fs/log.h>
#include <uae/uae.h>

#include "fs-uae.h"

#ifdef WITH_LUA

static int l_fs_uae_get_save_state_number(lua_State *L)
{
    lua_pushinteger(L, g_fs_uae_state_number);
    return 1;
}

static int l_fs_uae_get_input_event(lua_State *L)
{
    lua_pushinteger(L, g_fs_uae_last_input_event);
    lua_pushinteger(L, g_fs_uae_last_input_event_state);
    return 2;
}

static int l_fs_uae_set_input_event(lua_State *L)
{
    int input_event = luaL_checkint(L, -2);
    int state = luaL_checkint(L, -1);
    g_fs_uae_last_input_event = input_event;
    g_fs_uae_last_input_event_state = state;
    return 0;
}

static int l_fs_uae_send_input_event(lua_State *L)
{
    int input_event = luaL_checkint(L, -2);
    int state = luaL_checkint(L, -1);
    /*
        fs_uae_process_input_event(input_event, state);
    */
    return 0;
}

static int l_fs_uae_get_state_checksum(lua_State *L)
{
    lua_pushinteger(L, amiga_get_state_checksum());
    return 1;
}

static int l_fs_uae_get_rand_checksum(lua_State *L)
{
    lua_pushinteger(L, amiga_get_state_checksum());
    return 1;
}

void fs_uae_init_lua_state(lua_State *L)
{
    fsuae_log("fs_uae_lua_init_state %p\n", L);
    lua_register(L, "fs_uae_get_input_event", l_fs_uae_get_input_event);
    lua_register(L, "fs_uae_set_input_event", l_fs_uae_set_input_event);
    lua_register(L, "fs_uae_send_input_event", l_fs_uae_send_input_event);
    lua_register(
        L, "fs_uae_get_save_state_number", l_fs_uae_get_save_state_number);
    lua_register(L, "fs_uae_get_state_checksum", l_fs_uae_get_state_checksum);
    lua_register(L, "fs_uae_get_rand_checksum", l_fs_uae_get_rand_checksum);
}

#endif
