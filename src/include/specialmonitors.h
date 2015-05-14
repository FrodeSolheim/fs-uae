#ifndef UAE_SPECIALMONITORS_H
#define UAE_SPECIALMONITORS_H

#include "xwin.h"

bool emulate_specialmonitors(struct vidbuffer *src, struct vidbuffer *dst);
void specialmonitor_store_fmode(int vpos, int hpos, uae_u16 fmode);

#endif /* UAE_SPECIALMONITORS_H */
