#ifndef UAE_GLIB_H
#define UAE_GLIB_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_GLIB

#ifdef FSUAE

#include <fs/glib.h>

#else

#include <glib.h>
#include <glib/gstdio.h>

#endif

#endif

#endif /* UAE_GLIB_H */
