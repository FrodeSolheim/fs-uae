#ifndef UAE_CALC_H
#define UAE_CALC_H

#ifdef FSUAE // NL
#include "uae/types.h"
#endif

extern bool calc(const TCHAR *input, double *outval);
extern bool iscalcformula (const TCHAR *formula);

#endif // UAE_CALC_C
