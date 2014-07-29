#ifndef _FS_UAE_RECORDING_H_
#define _FS_UAE_RECORDING_H_

void fs_uae_init_recording();
int fs_uae_is_recording_enabled();
void fs_uae_enable_recording(const char *record_file);

void fs_uae_record_frame(int frame);
void fs_uae_record_input_event(int line, int event, int state);
int fs_uae_get_recorded_input_event(int frame, int line, int *event, int *state);
void fs_uae_write_recorded_session(void);

#endif // _FS_UAE_RECORDING_H_
