
#ifndef UAE_COMPAT_WINDOWS_H
#define UAE_COMPAT_WINDOWS_H

/*
 * Define Windows compatibility functions used by WinUAE throughout the source.
 * For example, _byteswap_ulong is used directly in the modified PCem source
 * code.
 *
 * Copyright (c) 2025 Frode Solheim
 */

#include "uae/byteswap.h"

#define _byteswap_ushort uae_bswap16
#define _byteswap_ulong uae_bswap32

#endif // UAE_COMPAT_WINDOWS_H
