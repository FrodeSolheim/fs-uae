/*
* UAE - The Un*x Amiga Emulator
*
* LUA Scripting Layer
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

#include "options.h"
#include "zfile.h"
#include "uae.h"
#include "luascript.h"
#include "lualibuae.h"

#ifdef WITH_LUA

typedef struct uae_lua_state {
    lua_State *state;
    int flags;
    struct uae_lua_state *next;
} uae_lua_state_t;

static uae_lua_state_t g_states[MAX_LUA_STATES];
static uae_lua_state_t *g_free;
static uae_lua_state_t *g_first;
static int g_num_states;
static uae_sem_t lua_sem;
static uae_lua_state_cb g_extra_state_setup;
static uae_lua_cb g_init_cb;
static uae_lua_cb g_exit_cb;

static void setup_state(lua_State *L)
{
    luaopen_uaelib(L); // get lib table
    lua_setglobal(L, "uae"); // assign "uae"
}

void uae_lua_log_error(lua_State *L, const char *msg)
{
    write_log("lua:ERROR: %s: %s\n", msg, lua_tostring(L, -1));
    //printf("%s: %s\n", msg, lua_tostring(L, -1));
}

int uae_lua_run_handler(const char *name)
{
    uae_lua_state *state = g_first;
    while(state != NULL) {
        if((state->flags & UAE_LUA_STATE_NO_HANDLER)==0) {
            lua_State *L = state->state;
            uae_lua_lock_state(L);
            lua_getglobal(L, name);
            if (lua_isnil(L, -1)) {
                //lua_pop(L, 1);
            }
            else if (lua_pcall(L, 0, 0, 0) != 0) {
                uae_lua_log_error(L, name);
                //lua_pop(L, 1);
            }
            lua_settop(L, 0);
            uae_lua_unlock_state(L);
        }
        state = state->next;
    }
    return 0;
}

int uae_lua_run_script(const TCHAR *filename)
{
    lua_State *L = uae_lua_create_state(UAE_LUA_STATE_AUTO_CLEAN);
    if(L == NULL) {
        write_log (_T("lua:%s: can't create state\n"), filename);
        return -1;
    }

    char *fn = ua (filename);
    int err = luaL_loadfilex(L, fn, NULL);
    if (!err) {
        err = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (!err) {
            write_log (_T("lua:%s: loaded\n"), filename);
        }
    }
    if (err) {
        write_log (_T("lua:%s: initialization failed: %d\n"), filename, err);
        uae_lua_destroy_state(L);
    }
    xfree (fn);
    return err;
}

void uae_lua_run_init_scripts(void)
{
    TCHAR tmp[MAX_DPATH];
    fetch_luapath (tmp, sizeof tmp / sizeof (TCHAR));
    _tcscat (tmp, _T("default.lua"));
    if (zfile_exists (tmp))
        uae_lua_run_script(tmp);
    for (int i = 0; i < MAX_LUA_STATES; i++) {
        if (currprefs.luafiles[i][0]) {
            uae_lua_run_script(currprefs.luafiles[i]);
        }
    }
}

void uae_lua_lock_state(lua_State *L)
{
    uae_sem_wait (&lua_sem);
}

void uae_lua_unlock_state(lua_State *L)
{
    uae_sem_post (&lua_sem);
}

lua_State *uae_lua_create_state(int flags)
{
    // create a new state
    lua_State *L = luaL_newstate();
    if(L == NULL) {
        write_log(_T("WARNING: can't create new lua state\n"));
        return NULL;
    }

    // setup state
    luaL_openlibs(L);
    setup_state(L);
    if(g_extra_state_setup != NULL) {
        g_extra_state_setup(L);
    }

    // find empty state slot
    uae_sem_wait (&lua_sem);
    uae_lua_state_t *state = g_free;
    if(state == NULL) {
        uae_sem_post (&lua_sem);
        write_log(_T("WARNING: too many lua states (ignored this one)\n"));
        return NULL;
    }

    // insert new state
    g_free = state->next;
    state->next = g_first;
    state->state = L;
    state->flags = flags;
    g_first = state;
    g_num_states ++;
    uae_sem_post (&lua_sem);

    write_log(_T("lua: created state %p (slot %p, num=%d)\n"),
        L, state, g_num_states);
    return L;
}

void uae_lua_destroy_state(lua_State *L)
{
    // find slot
    uae_sem_wait (&lua_sem);
    uae_lua_state_t *slot = g_first;
    uae_lua_state_t *last = NULL;
    while(slot != NULL) {
        if(slot->state == L) {
            break;
        }
        last = slot;
        slot = slot->next;
    }
    if(slot == NULL) {
        uae_sem_post (&lua_sem);
        write_log(_T("lua: can't destroy unknown state %p\n"), L);
        return;
    }

    // remove slot
    if(slot == g_first) {
        g_first = slot->next;
    } else {
        last->next = slot->next;
    }
    g_num_states --;
    slot->next = g_free;
    g_free = slot;
    uae_sem_post (&lua_sem);

    // free state
    lua_close(L);
    write_log(_T("lua: destroyed state %p (slot %p, num=%d)\n"),
        L, slot, g_num_states);
}

void uae_lua_set_extra_state_setup(uae_lua_state_cb func)
{
    g_extra_state_setup = func;
}

void uae_lua_set_callbacks(uae_lua_cb init_cb, uae_lua_cb exit_cb)
{
    g_init_cb = init_cb;
    g_exit_cb = exit_cb;
}

void uae_lua_init(void)
{
    write_log(_T("lua: init\n"));

    uae_sem_init (&lua_sem, 0, 1);

    // init state slots
    g_num_states = 0;
    g_first = NULL;
    g_free = g_states;
    uae_lua_state_t *last = NULL;
    uae_lua_state_t *current = g_states;
    for(int i=0;i<MAX_LUA_STATES;i++) {
        if(last != NULL) {
            last->next = current;
        }
        current->next = NULL;
        current->state = NULL;
        current->flags = 0;
        last = current;
        current++;
    }

    // notify callback
    if(g_init_cb != NULL) {
        g_init_cb();
    }
}

void uae_lua_free(void)
{
    write_log(_T("lua: free\n"));

    // notify callback
    if(g_exit_cb != NULL) {
        g_exit_cb();
    }

    // free states in slots
    uae_lua_state_t *current = g_first;
    while(current != NULL) {
        int flags = current->flags;
        if(flags & UAE_LUA_STATE_AUTO_CLEAN) {
            uae_lua_destroy_state(current->state);
        } else {
            write_log(_T("WARNING: state %p not auto clean but still there...\n"),
                current->state);
        }
    }

    uae_sem_destroy(&lua_sem);
}

#endif
