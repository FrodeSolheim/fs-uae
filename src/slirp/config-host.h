#ifndef SLIRP_CONFIG_HOST_H
#define SLIRP_CONFIG_HOST_H

#include "config.h"
#include "uae/types.h"

#ifdef restrict
#undef restrict
#endif

#define QEMU_PACKED __attribute__((packed))

#endif // SLIRP_CONFIG_HOST_H
