#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"
#include "custom.h"

#include <fs/random.h>

static uae_u32 randseed;
static int oldhcounter;

// the following functions where moved here from main.cpp, in order to
// implemented deterministic behavior for netplay. TODO: be able to
// set seed from netplay server - currently using fixed seed

static int g_uaesrand_called;
static fs_rand_context *g_rand_context = NULL;

// FIXME: is this needed?
//#define HSYNC_BASED_SEED

uae_u32 uaesrand (uae_u32 seed) {
    if (g_uaesrand_called) {
        write_log("uaesrand call ignored (not first time)\n");
    }
    write_log("uaesrand(%ud)\n", seed);
    oldhcounter = -1;
    randseed = seed;
    randseed = 0x12345678;
    write_log (_T("seed=%08x\n"), randseed);

    g_rand_context = fs_rand_new();

#ifndef HSYNC_BASED_SEED
    fs_rand_set_seed(g_rand_context, randseed);
#endif
    g_uaesrand_called = 1;
    return randseed;
}

//uae_u32 uaerand_impl (void) {
uae_u32 uaerand (void) {
#ifdef HSYNC_BASED_SEED
    if (oldhcounter != hsync_counter) {
        srand(hsync_counter ^ randseed);
        oldhcounter = hsync_counter;
    }
#endif
    if (g_uaesrand_called == 0) {
        //write_log("ERROR: uaerand called before uaesrand\n");
        //exit(1);
        uaesrand(0);
    }
    // random number can be put in both signed and unsigned 32/64-bits int
    // without changes to interpretation (only 31 bits are used).
    uae_u32 r = fs_rand_int_range(g_rand_context, 0, 2147483647);
    //write_log (" ---- rand ---- %08x\n", r);
#ifdef DEBUG_SYNC
    write_sync_log("random number: %u\n", r);
#endif
    return r;
}
uae_u32 uaerandgetseed (void) {
    return randseed;
}
