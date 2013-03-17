#ifndef FS_I18N_H_
#define FS_I18N_H_

#ifdef USE_GLIB
#include <glib/gi18n.h>
#else
//#include <libintl.h>
#define _(x) (x)
#define N_(x) (x)
#endif

#endif // FS_I18N_H_
