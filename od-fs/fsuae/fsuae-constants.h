#ifndef FSUAE_CONSTANTS_H
#define FSUAE_CONSTANTS_H

#define FSUAE_MESSAGE_RESET 10001
#define FSUAE_MESSAGE_RESTART_WITH_CONFIG 10002
#define FSUAE_MESSAGE_ADD_ROM 10003

// FIXME: Do we need something like this? That blocks reading
// further messages until the next reset??
// #define FSUAE_MESSAGE_WAIT_RESET

#endif // FSUAE_CONSTANTS_H
