/*
* UAE - The Un*x Amiga Emulator
*
* LUA Scripting Layer: uae library
*
* Copyright 2013 Frode SOlheim
*/

#ifndef UAE_LUALIBUAE_H
#define UAE_LUALIBUAE_H

#ifdef WITH_LUA

#ifdef __cplusplus
extern "C" {
#endif
#include <lauxlib.h>
#ifdef __cplusplus
}
#endif

int luaopen_uaelib(lua_State *L);

#endif // WITH_LUA

#endif // UAE_LUALIBUAE_H
