#ifndef FSUAE_RECORDING_H_
#define FSUAE_RECORDING_H_

#include "fsuae-config.h"

void fs_uae_init_recording(void);
int fs_uae_is_recording_enabled(void);
void fs_uae_enable_recording(const char *record_file);

void fs_uae_record_frame(int frame);
void fs_uae_record_input_event(int line, int event, int state);
int fs_uae_get_recorded_input_event(int frame,
                                    int line,
                                    int *event,
                                    int *state);
void fs_uae_write_recorded_session(void);

#endif  // FS_UAE_RECORDING_H_
