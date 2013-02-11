/*
* UAE - The Un*x Amiga Emulator
*
* LUA Scripting Layer
*
* Copyright 2013 Frode SOlheim
*/

#include "sysconfig.h"
#include "sysdeps.h"

#include "savestate.h"
#include "uae/memory.h"
#include "luascript.h"

#ifdef WITH_LUA

#define MAX_STATES 16

static int g_num_states;
static lua_State *g_states[MAX_STATES];
static void (*g_lock_function)(void);
static void (*g_unlock_function)(void);

//#define longget(addr) (call_mem_get_func(get_mem_bank(addr).lget, addr))
//#define wordget(addr) (call_mem_get_func(get_mem_bank(addr).wget, addr))
//#define byteget(addr) (call_mem_get_func(get_mem_bank(addr).bget, addr))
//#define longgeti(addr) (call_mem_get_func(get_mem_bank(addr).lgeti, addr))
//#define wordgeti(addr) (call_mem_get_func(get_mem_bank(addr).wgeti, addr))
//#define longput(addr,l) (call_mem_put_func(get_mem_bank(addr).lput, addr, l))
//#define wordput(addr,w) (call_mem_put_func(get_mem_bank(addr).wput, addr, w))
//#define byteput(addr,b) (call_mem_put_func(get_mem_bank(addr).bput, addr, b))

static int l_uae_read_u8(lua_State *L) {
    int addr = luaL_checkint(L, 1);
    uint8_t value = byteget(addr);
    lua_pushinteger(L, value);
    return 1;
}

static int l_uae_write_u8(lua_State *L) {
    int addr = luaL_checkint(L, 1);
    uint8_t value = luaL_checkint(L, 2);
    byteput(addr, value);
    return 0;
}

static int l_uae_read_u16(lua_State *L) {
    int addr = luaL_checkint(L, 1);
    uint16_t value = wordget(addr);
    lua_pushinteger(L, value);
    return 1;
}

static int l_uae_peek_u16(lua_State *L) {
    int result = 0;
    int addr = luaL_checkint(L, 1);

    if (addr >= 0xdff000 && addr < 0xe00000) {
        // + 4 to skip chipset mask
        int offset = addr - 0xdff000 + 4;
        int len;
        uae_u8 *data = save_custom (&len, NULL, 1);
        //printf("%d - %d  %d %d %d %d\n", offset, len, data[offset],
        //data[offset+1], data[offset+2], data[offset+3]);
        if (len - offset >= 2) {
            uint16_t value = *((uint16_t*) (data + offset));
            value = ((value & 0xff) << 8) | ((value & 0xff00) >> 8);
            lua_pushinteger(L, value);
            result += 1;
        }
        free(data);
    }

    return result;
}

static int l_uae_log(lua_State *L) {
    const char *s = luaL_checkstring(L, 1);
    write_log("%s", s);
    printf("%s", s);
    return 0;
}

void uae_lua_log_error(lua_State *L, const char *msg) {
    write_log("%s: %s\n", msg, lua_tostring(L, -1));
    printf("%s: %s\n", msg, lua_tostring(L, -1));
}

void uae_lua_aquire_lock() {
    if (g_lock_function) {
        g_lock_function();
    }
}

void uae_lua_release_lock() {
    if (g_unlock_function) {
        g_unlock_function();
    }
}

void uae_lua_run_handler(const char *name) {
    lua_State **L = g_states;
    while(*L) {
        uae_lua_aquire_lock();
        lua_getglobal(*L, name);
        if (lua_isnil(*L, -1)) {
            //lua_pop(*L, 1);
        }
        else if (lua_pcall(*L, 0, 0, 0) != 0) {
            uae_lua_log_error(*L, name);
            //lua_pop(*L, 1);
        }
        lua_settop(*L, 0);
        uae_lua_release_lock();
        L++;
    }
}

void uae_lua_init(void (*lock)(void), void (*unlock)(void)) {
    g_lock_function = lock;
    g_unlock_function = unlock;
    write_log("uae_lua_init\n");
}

#define SET_GLOBAL(v, n) lua_pushinteger(L, v); lua_setglobal(L, n);

void uae_lua_init_state(lua_State *L) {
    write_log("uae_lua_init_state %p\n", L);
    if (g_num_states == MAX_STATES) {
        write_log("WARNING: too many lua states (ignored this one)\n");
        return;
    }
    g_states[g_num_states] = L;
    g_num_states++;

    lua_register(L, "uae_log", l_uae_log);
    lua_register(L, "uae_read_u8", l_uae_read_u8);
    lua_register(L, "uae_read_u16", l_uae_read_u16);
    lua_register(L, "uae_peek_u16", l_uae_peek_u16);

    lua_register(L, "uae_write_u8", l_uae_write_u8);

    SET_GLOBAL(0xDFF000, "BLTDDAT");
    SET_GLOBAL(0xDFF002, "DMACONR");
    SET_GLOBAL(0xDFF004, "VPOSR");
    SET_GLOBAL(0xDFF006, "VHPOSR");
    SET_GLOBAL(0xDFF008, "DSKDATR");
    SET_GLOBAL(0xDFF00A, "JOY0DAT");
    SET_GLOBAL(0xDFF00C, "JOY1DAT");
    SET_GLOBAL(0xDFF00E, "CLXDAT");
    SET_GLOBAL(0xDFF010, "ADKCONR");
    SET_GLOBAL(0xDFF012, "POT0DAT");
    SET_GLOBAL(0xDFF014, "POT1DAT");
    SET_GLOBAL(0xDFF016, "POTINP");
    SET_GLOBAL(0xDFF018, "SERDATR");
    SET_GLOBAL(0xDFF01A, "DSKBYTR");
    SET_GLOBAL(0xDFF01C, "INTENAR");
    SET_GLOBAL(0xDFF01E, "INTREQR");
    SET_GLOBAL(0xDFF020, "DSKPTH");
    SET_GLOBAL(0xDFF022, "DSKPTL");
    SET_GLOBAL(0xDFF024, "DSKLEN");
    SET_GLOBAL(0xDFF026, "DSKDAT");
    SET_GLOBAL(0xDFF028, "REFPTR");
    SET_GLOBAL(0xDFF02A, "VPOSW");
    SET_GLOBAL(0xDFF02C, "VHPOSW");
    SET_GLOBAL(0xDFF02E, "COPCON");
    SET_GLOBAL(0xDFF030, "SERDAT");
    SET_GLOBAL(0xDFF032, "SERPER");
    SET_GLOBAL(0xDFF034, "POTGO");
    SET_GLOBAL(0xDFF036, "JOYTEST");
    SET_GLOBAL(0xDFF038, "STREQU");
    SET_GLOBAL(0xDFF03A, "STRVBL");
    SET_GLOBAL(0xDFF03C, "STRHOR");
    SET_GLOBAL(0xDFF03E, "STRLONG");
    SET_GLOBAL(0xDFF040, "BLTCON0");
    SET_GLOBAL(0xDFF042, "BLTCON1");
    SET_GLOBAL(0xDFF044, "BLTAFWM");
    SET_GLOBAL(0xDFF046, "BLTALWM");
    SET_GLOBAL(0xDFF048, "BLTCPTH");
    SET_GLOBAL(0xDFF04A, "BLTCPTL");
    SET_GLOBAL(0xDFF04C, "BLTBPTH");
    SET_GLOBAL(0xDFF04E, "BLTBPTL");
    SET_GLOBAL(0xDFF050, "BLTAPTH");
    SET_GLOBAL(0xDFF052, "BLTAPTL");
    SET_GLOBAL(0xDFF054, "BLTDPTH");
    SET_GLOBAL(0xDFF056, "BLTDPTL");
    SET_GLOBAL(0xDFF058, "BLTSIZE");
    SET_GLOBAL(0xDFF05A, "BLTCON0L");
    SET_GLOBAL(0xDFF05C, "BLTSIZV");
    SET_GLOBAL(0xDFF05E, "BLTSIZH");
    SET_GLOBAL(0xDFF060, "BLTCMOD");
    SET_GLOBAL(0xDFF062, "BLTBMOD");
    SET_GLOBAL(0xDFF064, "BLTAMOD");
    SET_GLOBAL(0xDFF066, "BLTDMOD");
    SET_GLOBAL(0xDFF070, "BLTCDAT");
    SET_GLOBAL(0xDFF072, "BLTBDAT");
    SET_GLOBAL(0xDFF074, "BLTADAT");
    SET_GLOBAL(0xDFF078, "SPRHDAT");
    SET_GLOBAL(0xDFF07A, "BPLHDAT");
    SET_GLOBAL(0xDFF07C, "DENISEID");
    SET_GLOBAL(0xDFF07E, "DSKSYNC");
    SET_GLOBAL(0xDFF080, "COP1LCH");
    SET_GLOBAL(0xDFF082, "COP1LCL");
    SET_GLOBAL(0xDFF084, "COP2LCH");
    SET_GLOBAL(0xDFF086, "COP2LCL");
    SET_GLOBAL(0xDFF088, "COPJMP1");
    SET_GLOBAL(0xDFF08A, "COPJMP2");
    SET_GLOBAL(0xDFF08C, "COPINS");
    SET_GLOBAL(0xDFF08E, "DIWSTRT");
    SET_GLOBAL(0xDFF090, "DIWSTOP");
    SET_GLOBAL(0xDFF092, "DDFSTRT");
    SET_GLOBAL(0xDFF094, "DDFSTOP");
    SET_GLOBAL(0xDFF096, "DMACON");
    SET_GLOBAL(0xDFF098, "CLXCON");
    SET_GLOBAL(0xDFF09A, "INTENA");
    SET_GLOBAL(0xDFF09C, "INTREQ");
    SET_GLOBAL(0xDFF09E, "ADKCON");
    SET_GLOBAL(0xDFF0A0, "AUD0LCH");
    SET_GLOBAL(0xDFF0A2, "AUD0LCL");
    SET_GLOBAL(0xDFF0A4, "AUD0LEN");
    SET_GLOBAL(0xDFF0A6, "AUD0PER");
    SET_GLOBAL(0xDFF0A8, "AUD0VOL");
    SET_GLOBAL(0xDFF0AA, "AUD0DAT");
    SET_GLOBAL(0xDFF0B0, "AUD1LCH");
    SET_GLOBAL(0xDFF0B2, "AUD1LCL");
    SET_GLOBAL(0xDFF0B4, "AUD1LEN");
    SET_GLOBAL(0xDFF0B6, "AUD1PER");
    SET_GLOBAL(0xDFF0B8, "AUD1VOL");
    SET_GLOBAL(0xDFF0BA, "AUD1DAT");
    SET_GLOBAL(0xDFF0C0, "AUD2LCH");
    SET_GLOBAL(0xDFF0C2, "AUD2LCL");
    SET_GLOBAL(0xDFF0C4, "AUD2LEN");
    SET_GLOBAL(0xDFF0C6, "AUD2PER");
    SET_GLOBAL(0xDFF0C8, "AUD2VOL");
    SET_GLOBAL(0xDFF0CA, "AUD2DAT");
    SET_GLOBAL(0xDFF0D0, "AUD3LCH");
    SET_GLOBAL(0xDFF0D2, "AUD3LCL");
    SET_GLOBAL(0xDFF0D4, "AUD3LEN");
    SET_GLOBAL(0xDFF0D6, "AUD3PER");
    SET_GLOBAL(0xDFF0D8, "AUD3VOL");
    SET_GLOBAL(0xDFF0DA, "AUD3DAT");
    SET_GLOBAL(0xDFF0E0, "BPL1PTH");
    SET_GLOBAL(0xDFF0E2, "BPL1PTL");
    SET_GLOBAL(0xDFF0E4, "BPL2PTH");
    SET_GLOBAL(0xDFF0E6, "BPL2PTL");
    SET_GLOBAL(0xDFF0E8, "BPL3PTH");
    SET_GLOBAL(0xDFF0EA, "BPL3PTL");
    SET_GLOBAL(0xDFF0EC, "BPL4PTH");
    SET_GLOBAL(0xDFF0EE, "BPL4PTL");
    SET_GLOBAL(0xDFF0F0, "BPL5PTH");
    SET_GLOBAL(0xDFF0F2, "BPL5PTL");
    SET_GLOBAL(0xDFF0F4, "BPL6PTH");
    SET_GLOBAL(0xDFF0F6, "BPL6PTL");
    SET_GLOBAL(0xDFF0F8, "BPL7PTH");
    SET_GLOBAL(0xDFF0FA, "BPL7PTL");
    SET_GLOBAL(0xDFF0FC, "BPL8PTH");
    SET_GLOBAL(0xDFF0FE, "BPL8PTL");
    SET_GLOBAL(0xDFF100, "BPLCON0");
    SET_GLOBAL(0xDFF102, "BPLCON1");
    SET_GLOBAL(0xDFF104, "BPLCON2");
    SET_GLOBAL(0xDFF106, "BPLCON3");
    SET_GLOBAL(0xDFF108, "BPL1MOD");
    SET_GLOBAL(0xDFF10A, "BPL2MOD");
    SET_GLOBAL(0xDFF10C, "BPLCON4");
    SET_GLOBAL(0xDFF10E, "CLXCON2");
    SET_GLOBAL(0xDFF110, "BPL1DAT");
    SET_GLOBAL(0xDFF112, "BPL2DAT");
    SET_GLOBAL(0xDFF114, "BPL3DAT");
    SET_GLOBAL(0xDFF116, "BPL4DAT");
    SET_GLOBAL(0xDFF118, "BPL5DAT");
    SET_GLOBAL(0xDFF11A, "BPL6DAT");
    SET_GLOBAL(0xDFF11C, "BPL7DAT");
    SET_GLOBAL(0xDFF11E, "BPL8DAT");
    SET_GLOBAL(0xDFF120, "SPR0PTH");
    SET_GLOBAL(0xDFF122, "SPR0PTL");
    SET_GLOBAL(0xDFF124, "SPR1PTH");
    SET_GLOBAL(0xDFF126, "SPR1PTL");
    SET_GLOBAL(0xDFF128, "SPR2PTH");
    SET_GLOBAL(0xDFF12A, "SPR2PTL");
    SET_GLOBAL(0xDFF12C, "SPR3PTH");
    SET_GLOBAL(0xDFF12E, "SPR3PTL");
    SET_GLOBAL(0xDFF130, "SPR4PTH");
    SET_GLOBAL(0xDFF132, "SPR4PTL");
    SET_GLOBAL(0xDFF134, "SPR5PTH");
    SET_GLOBAL(0xDFF136, "SPR5PTL");
    SET_GLOBAL(0xDFF138, "SPR6PTH");
    SET_GLOBAL(0xDFF13A, "SPR6PTL");
    SET_GLOBAL(0xDFF13C, "SPR7PTH");
    SET_GLOBAL(0xDFF13E, "SPR7PTL");
    SET_GLOBAL(0xDFF140, "SPR0POS");
    SET_GLOBAL(0xDFF142, "SPR0CTL");
    SET_GLOBAL(0xDFF144, "SPR0DATA");
    SET_GLOBAL(0xDFF146, "SPR0DATB");
    SET_GLOBAL(0xDFF148, "SPR1POS");
    SET_GLOBAL(0xDFF14A, "SPR1CTL");
    SET_GLOBAL(0xDFF14C, "SPR1DATA");
    SET_GLOBAL(0xDFF14E, "SPR1DATB");
    SET_GLOBAL(0xDFF150, "SPR2POS");
    SET_GLOBAL(0xDFF152, "SPR2CTL");
    SET_GLOBAL(0xDFF154, "SPR2DATA");
    SET_GLOBAL(0xDFF156, "SPR2DATB");
    SET_GLOBAL(0xDFF158, "SPR3POS");
    SET_GLOBAL(0xDFF15A, "SPR3CTL");
    SET_GLOBAL(0xDFF15C, "SPR3DATA");
    SET_GLOBAL(0xDFF15E, "SPR3DATB");
    SET_GLOBAL(0xDFF160, "SPR4POS");
    SET_GLOBAL(0xDFF162, "SPR4CTL");
    SET_GLOBAL(0xDFF164, "SPR4DATA");
    SET_GLOBAL(0xDFF166, "SPR4DATB");
    SET_GLOBAL(0xDFF168, "SPR5POS");
    SET_GLOBAL(0xDFF16A, "SPR5CTL");
    SET_GLOBAL(0xDFF16C, "SPR5DATA");
    SET_GLOBAL(0xDFF16E, "SPR5DATB");
    SET_GLOBAL(0xDFF170, "SPR6POS");
    SET_GLOBAL(0xDFF172, "SPR6CTL");
    SET_GLOBAL(0xDFF174, "SPR6DATA");
    SET_GLOBAL(0xDFF176, "SPR6DATB");
    SET_GLOBAL(0xDFF178, "SPR7POS");
    SET_GLOBAL(0xDFF17A, "SPR7CTL");
    SET_GLOBAL(0xDFF17C, "SPR7DATA");
    SET_GLOBAL(0xDFF17E, "SPR7DATB");
    SET_GLOBAL(0xDFF180, "COLOR00");
    SET_GLOBAL(0xDFF182, "COLOR01");
    SET_GLOBAL(0xDFF184, "COLOR02");
    SET_GLOBAL(0xDFF186, "COLOR03");
    SET_GLOBAL(0xDFF188, "COLOR04");
    SET_GLOBAL(0xDFF18A, "COLOR05");
    SET_GLOBAL(0xDFF18C, "COLOR06");
    SET_GLOBAL(0xDFF18E, "COLOR07");
    SET_GLOBAL(0xDFF190, "COLOR08");
    SET_GLOBAL(0xDFF192, "COLOR09");
    SET_GLOBAL(0xDFF194, "COLOR10");
    SET_GLOBAL(0xDFF196, "COLOR11");
    SET_GLOBAL(0xDFF198, "COLOR12");
    SET_GLOBAL(0xDFF19A, "COLOR13");
    SET_GLOBAL(0xDFF19C, "COLOR14");
    SET_GLOBAL(0xDFF19E, "COLOR15");
    SET_GLOBAL(0xDFF1A0, "COLOR16");
    SET_GLOBAL(0xDFF1A2, "COLOR17");
    SET_GLOBAL(0xDFF1A4, "COLOR18");
    SET_GLOBAL(0xDFF1A6, "COLOR19");
    SET_GLOBAL(0xDFF1A8, "COLOR20");
    SET_GLOBAL(0xDFF1AA, "COLOR21");
    SET_GLOBAL(0xDFF1AC, "COLOR22");
    SET_GLOBAL(0xDFF1AE, "COLOR23");
    SET_GLOBAL(0xDFF1B0, "COLOR24");
    SET_GLOBAL(0xDFF1B2, "COLOR25");
    SET_GLOBAL(0xDFF1B4, "COLOR26");
    SET_GLOBAL(0xDFF1B6, "COLOR27");
    SET_GLOBAL(0xDFF1B8, "COLOR28");
    SET_GLOBAL(0xDFF1BA, "COLOR29");
    SET_GLOBAL(0xDFF1BC, "COLOR30");
    SET_GLOBAL(0xDFF1BE, "COLOR31");
    SET_GLOBAL(0xDFF1C0, "HTOTAL");
    SET_GLOBAL(0xDFF1C2, "HSSTOP");
    SET_GLOBAL(0xDFF1C4, "HBSTRT");
    SET_GLOBAL(0xDFF1C6, "HBSTOP");
    SET_GLOBAL(0xDFF1C8, "VTOTAL");
    SET_GLOBAL(0xDFF1CA, "VSSTOP");
    SET_GLOBAL(0xDFF1CC, "VBSTRT");
    SET_GLOBAL(0xDFF1CE, "VBSTOP");
    SET_GLOBAL(0xDFF1D0, "SPRHSTRT");
    SET_GLOBAL(0xDFF1D2, "SPRHSTOP");
    SET_GLOBAL(0xDFF1D4, "BPLHSTRT");
    SET_GLOBAL(0xDFF1D6, "BPLHSTOP");
    SET_GLOBAL(0xDFF1D8, "HHPOSW");
    SET_GLOBAL(0xDFF1DA, "HHPOSR");
    SET_GLOBAL(0xDFF1DC, "BEAMCON0");
    SET_GLOBAL(0xDFF1DE, "HSSTRT");
    SET_GLOBAL(0xDFF1E0, "VSSTRT");
    SET_GLOBAL(0xDFF1E2, "HCENTER");
    SET_GLOBAL(0xDFF1E4, "DIWHIGH");
    SET_GLOBAL(0xDFF1E6, "BPLHMOD");
    SET_GLOBAL(0xDFF1E8, "SPRHPTH");
    SET_GLOBAL(0xDFF1EA, "SPRHPTL");
    SET_GLOBAL(0xDFF1EC, "BPLHPTH");
    SET_GLOBAL(0xDFF1EE, "BPLHPTL");
    SET_GLOBAL(0xDFF1FC, "FMODE");
}

#endif
