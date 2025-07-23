#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "videograb.h"

void uninitvideograb(void)
{
    write_log(_T("uninitvideograb\n"));
}

bool initvideograb(const TCHAR *filename)
{
    uninitvideograb();
    write_log(_T("initvideograb '%s'\n"), filename ? filename : _T("<null>"));
    return false;
}

void setvolumevideograb(int volume)
{
    STUB("");
}

bool getpausevideograb(void)
{
    return false;
}

void pausevideograb(int pause)
{
    STUB("");
}

bool getvideograb(long **buffer, int *width, int *height)
{
    return false;
}

uae_s64 getsetpositionvideograb(uae_s64 framepos)
{
    return 0;
}

bool isvideograb(void)
{
    return false;
}

void isvideograb_status(void)
{
    return;
}
