/***************************************************************************

    CDRDAO TOC parser for CHD compression frontend

    Copyright Nicola Salmoria and the MAME Team.
    Visit http://mamedev.org for licensing and usage restrictions.

***************************************************************************/

#pragma once

#ifndef __CHDCD_H__
#define __CHDCD_H__

#include "chdcdrom.h"

struct chdcd_track_input_entry
{
	chdcd_track_input_entry() { reset(); }
	void reset() { fname.reset(); offset = idx0offs = idx1offs = 0; swap = false; }

	astring fname;      // filename for each track
	UINT32 offset;      // offset in the data file for each track
	bool swap;          // data needs to be byte swapped
	UINT32 idx0offs;
	UINT32 idx1offs;
};

struct chdcd_track_input_info
{
	void reset() { for (int i = 0; i < CD_MAX_TRACKS; i++) track[i].reset(); }

	chdcd_track_input_entry track[CD_MAX_TRACKS];
};


chd_error chdcd_parse_toc(const TCHAR *tocfname, cdrom_toc &outtoc, chdcd_track_input_info &outinfo);

#endif  /* __CHDCD_H__ */
