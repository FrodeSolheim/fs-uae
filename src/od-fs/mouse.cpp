#include "sysconfig.h"
#include "sysdeps.h"

#include "inputdevice.h"
#include "xwin.h"

static int init_mouse (void)
{
	LOG_STUB ("return 1");
	return 0;
}

static void close_mouse (void)
{
	LOG_STUB ("");
}

static int acquire_mouse (int num, int flags)
{
	UAE_LOG_STUB ("num=%d flags=%d return 1", num, flags);
	return 1;
}

static void unacquire_mouse (int num)
{
	UAE_LOG_STUB ("num=%d", num);
}

static void read_mouse (void)
{
	UAE_LOG_STUB ("");
}

static int get_mouse_num (void)
{
	UAE_LOG_STUB_MAX (1, "return 0");
	return 0;
}

static TCHAR *get_mouse_friendlyname (int mouse)
{
	UAE_LOG_STUB ("mouse=%d", mouse);
	return strdup ("");
}

static TCHAR *get_mouse_uniquename (int mouse)
{
	UAE_LOG_STUB ("mouse=%d", mouse);
	return strdup ("");
}

static int get_mouse_widget_num (int mouse)
{
	UAE_LOG_STUB ("mouse=%d return 255", mouse);
	return 255;
}

static int get_mouse_widget_type (int mouse, int num, TCHAR *name, uae_u32 *code)
{
	UAE_LOG_STUB ("return IDEV_WIDGET_NONE");
	return IDEV_WIDGET_KEY;
}

static int get_mouse_widget_first (int mouse, int type)
{
	UAE_LOG_STUB ("mouse=%d type=%d return 0", mouse, type);
	return 0;
}

static int get_mouse_flags (int num)
{
	UAE_LOG_STUB ("num=%d return 0", num);
	return 0;
}

struct inputdevice_functions inputdevicefunc_mouse = {
	init_mouse,
	close_mouse,
	acquire_mouse,
	unacquire_mouse,
	read_mouse,
	get_mouse_num,
	get_mouse_friendlyname,
	get_mouse_uniquename,
	get_mouse_widget_num,
	get_mouse_widget_type,
	get_mouse_widget_first,
	get_mouse_flags
};

int input_get_default_mouse (struct uae_input_device *uid, int num, int port, int af, bool gp, bool wheel, bool joymouseswap)
{
	UAE_LOG_STUB ("num=%d return 0", num);
	return 0;
}

void toggle_mousegrab (void)
{
	UAE_LOG_STUB ("");
}

void setmouseactivexy(int monid, int x, int y, int dir)
{
	UAE_LOG_STUB ("");
}

void setmouseactive(int monid, int active)
{
	UAE_LOG_STUB ("");
}

extern bool ismouseactive (void)
{
	UAE_LOG_STUB ("");
	return 1;
}
