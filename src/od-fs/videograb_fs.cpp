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

void pausevideograb(int pause)
{

}

bool getvideograb(long **buffer, int *width, int *height)
{
    return false;
}

uae_s64 getsetpositionvideograb(uae_s64 framepos)
{
    return 0;
}
