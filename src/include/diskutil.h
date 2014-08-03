#ifndef UAE_DISKUTIL_H
#define UAE_DISKUTIL_H

#ifdef FSUAE // NL
#include "uae/types.h"
#endif

int isamigatrack (uae_u16 *amigamfmbuffer, uae_u8 *mfmdata, int len, uae_u8 *writebuffer, uae_u8 *writebuffer_ok, int track, int *outsize);
int ispctrack (uae_u16 *amigamfmbuffer, uae_u8 *mfmdata, int len, uae_u8 *writebuffer, uae_u8 *writebuffer_ok, int track, int *outsize);

#endif // UAE_DISKUTIL_H
