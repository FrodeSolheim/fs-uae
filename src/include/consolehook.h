#ifndef UAE_CONSOLEHOOK_H
#define UAE_CONSOLEHOOK_H

#ifdef FSUAE // NL
#include "uae/types.h"
#endif

int consolehook_activate (void);
void consolehook_ret (uaecptr condev, uaecptr oldbeginio);
uaecptr consolehook_beginio (uaecptr request);
void consolehook_config (struct uae_prefs *p);

#endif // UAE_CONSOLEHOOK_H
