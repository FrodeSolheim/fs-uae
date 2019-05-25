#include "sysconfig.h"
#include "sysdeps.h"

#include "statusline.h"
#include "uae.h"

bool softstatusline(void)
{
	return false;
}

void statusline_updated(int monid)
{

}

void statusline_render(int monid, uae_u8 *buf, int bpp, int pitch, int width, int height, uae_u32 *rc, uae_u32 *gc, uae_u32 *bc, uae_u32 *alpha)
{

}

bool isguiactive(void)
{
	return true;
}
