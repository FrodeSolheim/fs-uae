/*
* UAE - The Un*x Amiga Emulator
*
* LUA Scripting Layer
*
* Copyright 2013 Frode SOlheim
*/

#ifndef UAE_LUASCRIPT_H
#define UAE_LUASCRIPT_H

#ifdef WITH_LUA

#ifdef __cplusplus
extern "C" {
#endif
#include <lauxlib.h>
#ifdef __cplusplus
}
#endif

#define UAE_LUA_STATE_AUTO_CLEAN        1
#define UAE_LUA_STATE_NO_HANDLER        2

typedef void (*uae_lua_state_cb)(lua_State *);
typedef void (*uae_lua_cb)(void);

void uae_lua_set_extra_state_setup(uae_lua_state_cb func);
void uae_lua_set_callbacks(uae_lua_cb init_cb, uae_lua_cb exit_cb);

void uae_lua_init(void);
void uae_lua_free(void);

int uae_lua_run_script(const TCHAR *filename);
int uae_lua_run_handler(const char *name);
void uae_lua_run_init_scripts(void);

lua_State *uae_lua_create_state(int flags);
void uae_lua_destroy_state(lua_State *state);
void uae_lua_lock_state(lua_State *state);
void uae_lua_unlock_state(lua_State *state);

#endif /* WITH_LUA */

#endif /* UAE_LUASCRIPT_H */
