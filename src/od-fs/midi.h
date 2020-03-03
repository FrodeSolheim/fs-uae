#ifndef UAE_OD_FS_MIDI_H
#define UAE_OD_FS_MIDI_H

extern int midi_open(const char *cfg);
extern void midi_close(void);
extern void midi_send_byte(uint8_t ch);
extern int midi_has_byte(void);
extern int midi_recv_byte(uint8_t *ch);

#endif
