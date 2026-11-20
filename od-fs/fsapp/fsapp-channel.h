/*
 * Channels: typed text messages posted from any thread and read on one thread.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_CHANNEL_H
#define FSAPP_CHANNEL_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fsapp_channel;
typedef struct fsapp_channel fsapp_channel_t;

fsapp_channel_t *fsapp_channel_create(void);

void fsapp_channel_lock(fsapp_channel_t *channel);

void fsapp_channel_unlock(fsapp_channel_t *channel);

// Posts a message (copied), from any thread. Posting on the main channel (g_fsapp_main_channel)
// wakes the main loop (fsapp_frame_wake).
void fsapp_channel_post_message(fsapp_channel_t *channel, int type, const char *data);

bool fsapp_channel_get_next_message(fsapp_channel_t *channel, int *type, const char **data);

void fsapp_channel_init(void);

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_CHANNEL_H */
