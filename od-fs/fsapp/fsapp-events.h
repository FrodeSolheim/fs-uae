/*
 * Events that the main thread queues for the UI thread.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_EVENTS_H
#define FSAPP_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    FSAPP_EVENT_BEFORE_FIRST,

    FSAPP_EVENT_KEYBOARD_NAME,
    FSAPP_EVENT_MOUSE_NAME,
    FSAPP_EVENT_JOYSTICK_NAME,
    FSAPP_EVENT_GAMEPAD_NAME,

    FSAPP_EVENT_AFTER_LAST,
};

// TODO: ALMOST NOT USED ANY MORE...?

// Queues an app event for the UI thread (fsapp_events.get() in Python) and wakes it
// (fsapp_pythonthread_wake). Any thread.
void fsapp_events_push(const char *type, const char *strdata, const char *strdata2, int intdata,
                       int intdata2, float floatdata, float floatdata2);

#define fsapp_events_push_int(type, intdata)                                                       \
    fsapp_events_push(type, NULL, NULL, intdata, 0, 0.0f, 0.0f);

void fsapp_events_push_string(const char *type, const char *strdata);

void fsapp_events_push_string_int(const char *type, const char *strdata, int intdata);

void fsapp_events_init(void);

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_EVENTS_H */
