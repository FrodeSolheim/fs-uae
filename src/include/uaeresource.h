#ifndef UAE_UAERESOURCE_H
#define UAE_UAERESOURCE_H

#ifdef FSUAE // NL
#include "uae/types.h"
#endif

uaecptr uaeres_startup (uaecptr resaddr);
void uaeres_install (void);

#endif // UAE_UAERESOURCE_H
