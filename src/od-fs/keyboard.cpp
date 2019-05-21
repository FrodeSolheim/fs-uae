#include "sysconfig.h"
#include "sysdeps.h"

#include "uae/fs.h"
#include "inputdevice.h"
#include "keyboard.h"
#include "keybuf.h"

static int init_kb (void)
{
	LOG_STUB ("return 1");
	return 0;
}

static void close_kb (void)
{
	LOG_STUB ("");
}

static int acquire_kb (int num, int flags)
{
	// UAE_LOG_STUB ("num=%d flags=%d return 1", num, flags);
	return 1;
}

static void unacquire_kb (int num)
{
	// UAE_LOG_STUB ("num=%d", num);
}

static void read_kb (void)
{
	UAE_LOG_STUB ("");
}

static int get_kb_num (void)
{
	UAE_LOG_STUB_MAX (1, "return 0");
	return 0;
}

static TCHAR *get_kb_friendlyname (int kb)
{
	UAE_LOG_STUB ("kb=%d", kb);
	return strdup ("");
}

static TCHAR *get_kb_uniquename (int kb)
{
	UAE_LOG_STUB ("kb=%d", kb);
	return strdup ("");
}

static int get_kb_widget_num (int kb)
{
	UAE_LOG_STUB ("kb=%d return 255", kb);
	return 255;
}

static int get_kb_widget_type (int kb, int num, TCHAR *name, uae_u32 *code)
{
	UAE_LOG_STUB ("return IDEV_WIDGET_NONE");
	return IDEV_WIDGET_KEY;
}

static int get_kb_widget_first (int kb, int type)
{
	UAE_LOG_STUB ("kb=%d type=%d return 0", kb, type);
	return 0;
}

static int get_kb_flags (int num)
{
	UAE_LOG_STUB ("num=%d return 0", num);
	return 0;
}

struct inputdevice_functions inputdevicefunc_keyboard = {
	init_kb,
	close_kb,
	acquire_kb,
	unacquire_kb,
	read_kb,
	get_kb_num,
	get_kb_friendlyname,
	get_kb_uniquename,
	get_kb_widget_num,
	get_kb_widget_type,
	get_kb_widget_first,
	get_kb_flags
};

int input_get_default_keyboard (int num)
{
	UAE_LOG_STUB ("num=%d return 0", num);
	return 0;
}

int target_checkcapslock (int scancode, int *state)
{
	UAE_LOG_STUB_MAX (1, "");
	return 0;
}

int getcapslockstate (void)
{
	UAE_LOG_STUB_MAX (1, "");
	return 0;
}

void setcapslockstate (int state)
{
	UAE_LOG_STUB_MAX (1, "");
}

static struct uae_input_device_kbr_default keytrans_amiga[] = {
	{ INPUTEVENT_KEY_CAPS_LOCK, { { INPUTEVENT_KEY_CAPS_LOCK, ID_FLAG_TOGGLE } } },
	{ -1, { { 0 } } }
};

static struct uae_input_device_kbr_default *keytrans[] = {
	keytrans_amiga,
	keytrans_amiga,
	keytrans_amiga,
};

static int kb_none[] = { -1 };

static int *kbmaps[] = {
	kb_none,
	kb_none,
	kb_none,
	kb_none,
	kb_none,
	kb_none,
	kb_none,
	kb_none,
	kb_none,
	kb_none
};

void keyboard_settrans (void)
{
	write_log ("KB: keyboard_settrans\n");
	// FIXME: can this be dropped?
	inputdevice_setkeytranslation (keytrans, kbmaps);
}
