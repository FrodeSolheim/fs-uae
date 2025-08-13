#ifndef FSUAE_MESSAGES_H
#define FSUAE_MESSAGES_H

#include "fsapp-channel.h"
#include "fsuae-config.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsuae_messages_set_channel(fsapp_channel_t* channel);

void fsuae_messages_process_early(void);
void fsuae_messages_process(void);

enum {
    FSUAE_MESSAGE_RESET = 10001,
    FSUAE_MESSAGE_RESTART_WITH_CONFIG = 10002,
    FSUAE_MESSAGE_ADD_ROM = 10003,
    FSUAE_MESSAGE_EARLY_STOP = 10004,
    FSUAE_MESSAGE_QUICKSTART_A500 = 10005,
    FSUAE_MESSAGE_QUICKSTART_A500P = 10006,
    FSUAE_MESSAGE_QUICKSTART_A600 = 10007,
    FSUAE_MESSAGE_QUICKSTART_A1000 = 10008,
    FSUAE_MESSAGE_QUICKSTART_A1200 = 10009,
    FSUAE_MESSAGE_QUICKSTART_A3000 = 10010,
    FSUAE_MESSAGE_QUICKSTART_A4000 = 10011,
    FSUAE_MESSAGE_QUICKSTART_A4000T = 10012,
    FSUAE_MESSAGE_QUICKSTART_CD32 = 10013,
    FSUAE_MESSAGE_QUICKSTART_CDTV = 10014,
};

#ifdef __cplusplus
}
#endif

#endif  // FSUAE_MESSAGES_H
