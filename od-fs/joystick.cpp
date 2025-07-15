#include "sysconfig.h"
#include "sysdeps.h"

#include "inputdevice.h"

static int init_joystick (void)
{
	UAE_LOG_STUB ("return 1");
	return 1;
}

static void close_joystick (void)
{
	UAE_LOG_STUB ("");
}

static int acquire_joystick (int num, int flags)
{
	// UAE_LOG_STUB ("num=%d flags=%d return 1", num, flags);
	return 1;
}

static void unacquire_joystick (int num)
{
	// UAE_LOG_STUB ("num=%d", num);
}

static void read_joystick (void)
{
	UAE_LOG_STUB ("");
}

static int get_joystick_num (void)
{
	UAE_LOG_STUB_MAX (1, "return 0");
	return 0;
}

static TCHAR *get_joystick_friendlyname (int joy)
{
	UAE_LOG_STUB ("joy=%d", joy);
	return strdup ("");
}

static TCHAR *get_joystick_uniquename (int joy)
{
	UAE_LOG_STUB ("joy=%d", joy);
	return strdup ("");
}

static int get_joystick_widget_num (int joy)
{
	UAE_LOG_STUB ("joy=%d return 0", joy);
	return 0;
}

static int get_joystick_widget_type (int joy, int num, char *name, uae_u32 *code)
{
	UAE_LOG_STUB ("return IDEV_WIDGET_NONE");
	return IDEV_WIDGET_NONE;
}

static int get_joystick_widget_first (int joy, int type)
{
	UAE_LOG_STUB ("joy=%d type=%d return 0", joy, type);
	return 0;
}

static int get_joystick_flags (int num)
{
	UAE_LOG_STUB ("num=%d return 0", num);
	return 0;
}

struct inputdevice_functions inputdevicefunc_joystick = {
	init_joystick,
	close_joystick,
	acquire_joystick,
	unacquire_joystick,
	read_joystick,
	get_joystick_num,
	get_joystick_friendlyname,
	get_joystick_uniquename,
	get_joystick_widget_num,
	get_joystick_widget_type,
	get_joystick_widget_first,
	get_joystick_flags
};

int input_get_default_joystick (struct uae_input_device *uid, int num, int port, int af, int mode, bool gp, bool joymouseswap)
{
	UAE_LOG_STUB ("num=%d return 0", num);
	return 0;
}

int input_get_default_joystick_analog (struct uae_input_device *uid, int num, int port, int af, bool gp, bool joymouseswap)
{
	UAE_LOG_STUB ("num=%d return 0", num);
	return 0;
}
