#include "sysconfig.h"
#include "sysdeps.h"

#include "custom.h"
#include "options.h"
#include "uae.h"
#include <stdint.h>

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df   /* constant vector a */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

struct rand_context {
	uint32_t mt[N]; /* the array for the state vector  */
	unsigned int mti;
};

static void rand_set_seed(rand_context *rand, uint32_t seed)
{
	if (rand == NULL) {
		return;
	}
	rand->mt[0] = seed;
	for (rand->mti = 1; rand->mti < N; rand->mti++)
		rand->mt[rand->mti] =
			1812433253UL
			* (rand->mt[rand->mti - 1]
				^ (rand->mt[rand->mti - 1] >> 30))
			+ rand->mti;
}

static uint32_t rand_int(rand_context *rand)
{
	uint32_t y;
	static const uint32_t mag01[2] = { 0x0, MATRIX_A };
	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (rand == NULL) {
		return 0;
	}

	if (rand->mti >= N) { /* generate N words at one time */
		int kk;

		for (kk = 0; kk < N - M; kk++) {
			y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk + 1] & LOWER_MASK);
			rand->mt[kk] = rand->mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (; kk < N - 1; kk++) {
			y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk + 1] & LOWER_MASK);
			rand->mt[kk] = rand->mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (rand->mt[N - 1] & UPPER_MASK) | (rand->mt[0] & LOWER_MASK);
		rand->mt[N - 1] = rand->mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

		rand->mti = 0;
	}

	y = rand->mt[rand->mti++];
	y ^= TEMPERING_SHIFT_U(y);
	y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
	y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
	y ^= TEMPERING_SHIFT_L(y);

	return y;
}

// the following functions where moved here from main.cpp, in order to
// implemented deterministic behavior for netplay. TODO: be able to
// set seed from netplay server - currently using fixed seed

static uae_u32 randseed;
static int old_seed;
int g_random_debug_logging = 0;
static rand_context g_rand_context;

uae_u32 uaesrand (uae_u32 seed)
{
	old_seed = 0;
	randseed = seed;
	//randseed = 0x12345678;
	if (g_random_debug_logging) {
		write_log (_T("seed=%08x\n"), randseed);
	}
	return randseed;
}

uae_u32 uaerand (void)
{
#if 1
	static uae_u32 fakerand;
	if (fakerand == 0) {
		// Due to performance issues. Replace with lookup table or something.
		printf("WARNING: Not using random numbers at the moment\n");
	}
	fakerand += 1826165701;
	return fakerand;
#else
	int new_seed = g_uae_vsync_counter + vpos;
	if (old_seed != new_seed) {
		rand_set_seed (&g_rand_context, new_seed ^ randseed);
		old_seed = new_seed;
	}
	uae_u32 r = rand_int (&g_rand_context);
	if (g_random_debug_logging) {
		write_log (_T("rand=%08x\n"), r);
	}
	return r;
#endif
}

uae_u32 uaerandgetseed (void)
{
	return randseed;
}
