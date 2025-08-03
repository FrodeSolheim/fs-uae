#ifndef FSUAE_MESSAGES_H
#define FSUAE_MESSAGES_H

#include "fsuae-config.h"

#include "fsapp-channel.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsuae_messages_set_channel(fsapp_channel_t* channel);

void fsuae_messages_process(void);

#ifdef __cplusplus
}
#endif

#endif // FSUAE_MESSAGES_H