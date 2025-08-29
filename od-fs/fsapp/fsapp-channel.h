#ifndef FSAPP_CHANNEL_H
#define FSAPP_CHANNEL_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fsapp_channel;
typedef struct fsapp_channel fsapp_channel_t;

fsapp_channel_t* fsapp_channel_create(void);

void fsapp_channel_lock(fsapp_channel_t* channel);

void fsapp_channel_unlock(fsapp_channel_t* channel);

void fsapp_channel_add_message(fsapp_channel_t* channel, int type, const char* data);

bool fsapp_channel_next_message(fsapp_channel_t* channel, int* type, const char** data);

void fsapp_channel_init_module(void);

#ifdef __cplusplus
}
#endif

#endif  // FSAPP_CHANNEL_H
