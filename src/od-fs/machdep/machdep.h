/*
 * E-UAE - The portable Amiga Emulator
 *
 * Processor-specific definitions
 */

#ifndef MACHDEP_MACHDEP_H
#define MACHDEP_MACHDEP_H

#include <stdint.h>

#define MACHDEP_X86
#define MACHDEP_NAME    "x86"

#define HAVE_MACHDEP_TIMER

//typedef uae_s64 frame_time_t;
//typedef unsigned long frame_time_t;
//typedef int64_t frame_time_t;
//#define MAX_FRAME_TIME 9223372036854775807LL

// frame_time_t is often cast to int in the code...
typedef int frame_time_t;


#endif
