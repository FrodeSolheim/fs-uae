#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"

// FIXME: No longer a stub!!
// Cfgfile callback to save target-specific options to config file
void target_save_options (struct zfile *f, struct uae_prefs *p) {
    // LOG_STUB("zfile=%p p=%p", f, p);

    // Compatible with Amiberry
    cfgfile_target_dwrite_str_escape(f, _T("midiout_device_name"), p->midioutdev[0] ? p->midioutdev : _T("none"));
    cfgfile_target_dwrite_str_escape(f, _T("midiin_device_name"), p->midiindev[0] ? p->midiindev : _T("none"));
}

static const TCHAR *obsolete[] = {
	_T("killwinkeys"), _T("sound_force_primary"), _T("iconified_highpriority"),
	_T("sound_sync"), _T("sound_tweak"), _T("directx6"), _T("sound_style"),
	_T("file_path"), _T("iconified_nospeed"), _T("activepriority"), _T("magic_mouse"),
	_T("filesystem_codepage"), _T("aspi"), _T("no_overlay"), _T("soundcard_exclusive"),
	_T("specialkey"), _T("sound_speed_tweak"), _T("sound_lag"),
	nullptr
};

static int target_parse_option_hardware(uae_prefs *p, const TCHAR *option, const TCHAR *value)
{
	return 0;
}

static int target_parse_option_host(uae_prefs *p, const TCHAR *option, const TCHAR *value)
{
	// printf("target_parse_option_host option %s value %s\n", option, value);
	if (false
		|| cfgfile_string(option, value, _T("midi_device"), p->midioutdev, sizeof p->midioutdev)
		|| cfgfile_string(option, value, _T("midiout_device_name"), p->midioutdev, sizeof p->midioutdev)
		|| cfgfile_string(option, value, _T("midiin_device_name"), p->midiindev, sizeof p->midiindev)
		)
		return 1;

	return 0;
}

// Common with WinUAE and Amiberry
int target_parse_option(struct uae_prefs *p, const TCHAR *option, const TCHAR *value, int type)
{
	int v = 0;
	if (type & CONFIG_TYPE_HARDWARE) {
		v = target_parse_option_hardware(p, option, value);
	} else if (type & CONFIG_TYPE_HOST) {
		v = target_parse_option_host(p, option, value);
	}
	if (v) {
		return v;
	}
	
	int i = 0;
	while (obsolete[i]) {
		if (!strcasecmp(obsolete[i], option)) {
			write_log(_T("obsolete config entry '%s'\n"), option);
			return 1;
		}
		i++;
	}
	return 0;
}
