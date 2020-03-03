#ifndef FSEMU_FONTCACHE_H_
#define FSEMU_FONTCACHE_H_

#include "fsemu-config.h"
#include "fsemu-font.h"

#ifdef __cplusplus
extern "C" {
#endif

// You do not need to call this, it will be called on demand.
void fsemu_fontcache_init(void);

fsemu_font_t *fsemu_fontcache_font(const char *name, int size);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_FONTCACHE_H_
