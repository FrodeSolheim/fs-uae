#include "sysconfig.h"
#include "sysdeps.h"

#include "custom.h"
#include "options.h"
#include "uae.h"
#include <stdint.h>

// ----------------------------------------------------------------------------
// Park-Miller RNG
// Code from https://en.wikipedia.org/wiki/Lehmer_random_number_generator

static uint32_t lcg_parkmiller(uint32_t *state)
{
#if 1
	return *state = (uint64_t) *state * 48271 % 0x7fffffff;
#else
#if 0
	// To avoid the 64-bit division, do the reduction by hand:
	uint64_t product = (uint64_t) *state * 48271;
	uint32_t x = (product & 0x7fffffff) + (product >> 31);
	x = (x & 0x7fffffff) + (x >> 31);
	return *state = x;
#else
	// To use only 32-bit arithmetic, use Schrage's method:
	// Precomputed parameters for Schrage's method
	const uint32_t M = 0x7fffffff;
	const uint32_t A = 48271;
	const uint32_t Q = M / A;
	const uint32_t R = M % A;
	uint32_t div = *state / Q;
	uint32_t rem = *state % Q;
	int32_t s = rem * A;
	int32_t t = div * R;
	int32_t result = s - t;
	if (result < 0)
		result += M;
	return *state = result;
#endif
#endif
}

// ----------------------------------------------------------------------------
// The following functions were moved here from main.cpp in order to implement
// deterministic behavior for netplay.
// TODO: Ability to set seed from netplay server - currently using fixed seed.

static int old_seed = -1;
static uae_u32 randseed;
int g_random_debug_logging = 0;
// static rand_context g_rand_context;

static uint32_t uae_random_seed;
static uint32_t uae_random_state;

// FIXME: Review use of this; should only be called once by emulator startup?
// FIXME: Problematic with re-recording and savestates. Must be included in
// save state if desired to use...

uae_u32 uaesrand (uae_u32 seed)
{
#if 0
	// Temporarily disabled, need to check that combined seed is valid
	// for lcg_parkmiller
#else
	// uae_assert(seed >= 0 && seed < 0x7fffffff - 1);
	old_seed = -1;
	// randseed = seed;
	uae_random_seed = seed;
	//randseed = 0x12345678;
	if (g_random_debug_logging) {
		write_log (_T("seed=%08x\n"), uae_random_seed);
	}
	return uae_random_seed;
#endif
}

// #include <signal.h>
uae_u32 uaerand (void)
{
	int new_seed = vsync_counter;
	if (old_seed != new_seed) {
		// Seed must be > 0 and < 0x7fffffff
		// FIXME: strip away some bits of seed if necessary?
		uae_random_state = 1 + new_seed + uae_random_seed;
		old_seed = new_seed;
	}

	return lcg_parkmiller(&uae_random_state);
}

uae_u32 uaerandgetseed (void)
{
	return uae_random_seed;
}
