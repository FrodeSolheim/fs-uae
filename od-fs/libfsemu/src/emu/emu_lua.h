#ifndef LIBFSEMU_EMU_LUA_H_
#define LIBFSEMU_EMU_LUA_H_

#ifdef WITH_LUA

#include <lauxlib.h>

void fs_emu_lua_init(void);
void fs_emu_lua_log_error(const char *msg);

extern lua_State *fs_emu_lua_state;

#endif

#endif // LIBFSEMU_EMU_LUA_H_
