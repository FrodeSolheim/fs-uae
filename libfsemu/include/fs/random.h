#ifndef FS_RANDOM_H_
#define FS_RANDOM_H_

#include <fs/defines.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fs_rand_context;
typedef struct fs_rand_context fs_rand_context;

fs_rand_context* fs_rand_new_with_seed(uint32_t seed);
fs_rand_context* fs_rand_new_with_seed_array(const uint32_t *seed,
        unsigned int seed_length);
fs_rand_context* fs_rand_new(void);
void fs_rand_free(fs_rand_context *rand_);
fs_rand_context* fs_rand_copy(fs_rand_context *rand_);
void fs_rand_set_seed(fs_rand_context *rand_, uint32_t seed);
void fs_rand_set_seed_array(fs_rand_context *rand_, const uint32_t *seed,
        unsigned int seed_length);

#define fs_rand_boolean(rand_) ((g_rand_int (rand_) & (1 << 15)) != 0)

uint32_t fs_rand_int(fs_rand_context *rand_);
int32_t fs_rand_int_range(fs_rand_context *rand_, int32_t begin, int32_t end);
double fs_rand_double(fs_rand_context *rand_);
double fs_rand_double_range(fs_rand_context *rand_, double begin,
        double end);
void fs_random_set_seed(uint32_t seed);

#define g_random_boolean() ((g_random_int () & (1 << 15)) != 0)

uint32_t fs_random_int(void);
int32_t fs_random_int_range(int32_t begin, int32_t end);
double fs_random_double(void);
double fs_random_double_range(double begin, double end);

#ifdef __cplusplus
}
#endif

#endif // FS_RANDOM_H_
