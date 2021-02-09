#ifndef FSUAE_MEDIA_H_
#define FSUAE_MEDIA_H_

#include "fsuae-config.h"

void fsuae_media_configure_hard_drives(void);
void fsuae_media_configure_cdrom(void);

void fsuae_media_handle_uae_event(int event, void *data);

void fsuae_media_init(void);

#endif  // FSUAE_MEDIA_H_
