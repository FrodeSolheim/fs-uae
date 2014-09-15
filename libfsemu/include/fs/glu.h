#ifndef FS_GLU_H
#define FS_GLU_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(USE_GLES) && !defined(FAKE_GLES)

// no GLU

#elif defined(MACOSX)

#include <OpenGL/glu.h>

#else

#include <GL/glu.h>

#endif

#endif /* FS_GLU_H */
