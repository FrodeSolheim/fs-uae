#include "sysconfig.h"
#include "sysdeps.h"

#include "statusline.h"
#include "uae.h"

bool softstatusline(void)
{
	return false;
}

bool createstatusline(HWND parentHwnd, int monid)
{
	return true;
}

void statusline_updated(int monid)
{

}

void statusline_render(int monid, uae_u8 *buf, int pitch, int maxwidth, int maxheight, uae_u32 *rc, uae_u32 *gc, uae_u32 *bc, uae_u32 *alpha)
{

}

void ldp_render(const char *txt, int len, uae_u8 *buf, struct vidbuffer *vd, int dx, int dy, int mx, int my)
{

}
