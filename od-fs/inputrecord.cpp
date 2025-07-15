#include "sysconfig.h"
#include "sysdeps.h"

#include "inputrecord.h"

// int input_record = 0;
// int input_play = 0;
int inputrecord_debug = 0;

void inprec_close (bool clear)
{
}

int inprec_getposition (void)
{
	return -1;
}

int inprec_open (const TCHAR *fname, const TCHAR *statefilename)
{
	return 0;
}

void inprec_recorddiskchange (int nr, const TCHAR *fname, bool writeprotected)
{
}

void inprec_setposition (int offset, int replaycounter)
{
}

void inprec_playdebug_cpu (int mode)
{
}

void inprec_recorddebug_cpu (int mode)
{
}

bool inprec_prepare_record (const TCHAR *statefilename)
{
	return true;
}

void inprec_startup (void)
{
}

static bool inprec_realtime (bool stopstart)
{
	return true;
}

bool inprec_realtime (void)
{
	return inprec_realtime (false);
}

bool inprec_playevent (int *nr, int *state, int *max, int *autofire)
{
	return false;
}

void inprec_playdiskchange (void)
{
}

void inprec_recordevent (int nr, int state, int max, int autofire)
{
}

void inprec_recorddebug_cia (uae_u32 v1, uae_u32 v2, uae_u32 v3)
{
}

void inprec_playdebug_cia (uae_u32 v1, uae_u32 v2, uae_u32 v3)
{
}
