#ifndef FSAPP_EVENTS_H
#define FSAPP_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

void fsapp_events_push(
    const char* type, const char* strdata, const char* strdata2, int intdata,
    int intdata2, float floatdata, float floatdata2
);

#define fsapp_events_push_int(type, intdata)                                  \
    fsapp_events_push(type, NULL, NULL, intdata, 0, 0.0f, 0.0f);

void fsapp_events_push_string(const char* type, const char* strdata);

void fsapp_events_push_string_int(
    const char* type, const char* strdata, int intdata
);

void fsapp_events_init_module(void);

#ifdef __cplusplus
}
#endif

#endif // FSAPP_EVENTS_H
