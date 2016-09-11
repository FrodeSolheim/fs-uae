/*
* UAE - The Un*x Amiga Emulator
*
* LUA Scripting Layer: uae library
*
* Copyright 2013 Frode SOlheim
*/

#include "sysconfig.h"
#include "sysdeps.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <lualib.h>
#ifdef __cplusplus
}
#endif

#include "lualibuae.h"
#include "options.h"
#include "identify.h"
#include "debug.h"
#include "uaeexe.h"

#ifdef WITH_LUA

static int l_uae_read_u8(lua_State *L)
{
    int addr = luaL_checkint(L, 1);
    int value = debug_read_memory_8(addr);
    lua_pushinteger(L, value);
    return value >= 0 ? 1 : 0;
}

static int l_uae_write_u8(lua_State *L)
{
    int addr = luaL_checkint(L, 1);
    uint8_t value = luaL_checkint(L, 2);
    debug_write_memory_8(addr, value);
    return 0;
}
static int l_uae_write_u16(lua_State *L)
{
    int addr = luaL_checkint(L, 1);
    uint16_t value = luaL_checkint(L, 2);
    debug_write_memory_16(addr, value);
    return 0;
}

static int l_uae_read_u16(lua_State *L)
{
    int addr = luaL_checkint(L, 1);
    int value = debug_read_memory_16(addr);
    lua_pushinteger(L, value);
	return value >= 0 ? 1 : 0;
}

/* peek = read without any side-effects */
static int l_uae_peek_u16(lua_State *L)
{
    int result = 0;
	uint16_t value;
    int addr = luaL_checkint(L, 1);

	value = debug_peek_memory_16 (addr);
	if (value >= 0) {
        lua_pushinteger(L, value);
        result = 1;
    }
    return result;
}

static int l_uae_read_config(lua_State *L)
{
	int result = 0;
    const char *s = luaL_checkstring(L, 1);
	TCHAR *ts = au(s);
	TCHAR out[MAX_DPATH];
	if (cfgfile_searchconfig(ts, -1, out, sizeof out / sizeof(TCHAR)) == -1) {
		char *c = ua(out);
		lua_pushstring(L, c);
		xfree(c);
		result = 1;
	}
	xfree(ts);
	return result;
}

static int l_uae_write_config(lua_State *L)
{
    const char *s = luaL_checkstring(L, 1);
	TCHAR *ts = au(s);
	TCHAR out[MAX_DPATH];
	cfgfile_modify(-1, ts, _tcslen(ts), out, sizeof out / sizeof(TCHAR));
	char *c = ua(out);
	lua_pushstring(L, c);
	xfree(c);
	return 1;
}

static int l_uae_log(lua_State *L)
{
    const char *s = luaL_checkstring(L, 1);
    write_log("%s", s);
    //printf("%s", s);
    return 0;
}

static int l_uae_exe(lua_State *L)
{
    const char *s = luaL_checkstring(L, 1);
    int result = uaeexe(s);
    lua_pushinteger(L, result);
    return 1;
}

static const struct luaL_Reg uaelib[] = {
    {"log", l_uae_log},
    {"read_u8", l_uae_read_u8},
    {"read_u16", l_uae_read_u16},
    {"peek_u16", l_uae_peek_u16},
    {"write_u8", l_uae_write_u8},
    {"write_u16", l_uae_write_u16},
    {"read_config", l_uae_read_config},
    {"write_config", l_uae_write_config},
    {"exe", l_uae_exe},
    {NULL, NULL}
};

int luaopen_uaelib(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, uaelib, 0);
    // top is now uaelib table

    // create a new table "custom"
    lua_pushstring(L, "custom");
    lua_newtable(L);
    // add names of custom chip registers
    for (int i = 0; custd[i].name; i++) {
        char *s = ua(custd[i].name);
        lua_pushstring(L, s); // key
        lua_pushinteger(L, custd[i].adr); // value
        lua_settable(L, -3);
        xfree(s);
    }
    // set uae.custom = {}
    lua_settable(L, -3);

    return 1; // top is uaelib table
}

#endif
