#ifndef UAE_AMAX_H
#define UAE_AMAX_H

#ifdef FSUAE // NL
#include "uae/types.h"
#endif

void amax_diskwrite (uae_u16 w);
void amax_bfe001_write (uae_u8 pra, uae_u8 dra);
uae_u8 amax_disk_status (void);
void amax_disk_select (uae_u8 v, uae_u8 ov);
void amax_reset (void);
void amax_init (void);

#endif // UAE_AMAX_H
