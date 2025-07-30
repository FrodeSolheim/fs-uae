#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "zfile.h"

/** Copied from win32gui.cpp and removed/disabled WinUAE-specific code. */
int target_cfgfile_load (struct uae_prefs *p, const TCHAR *filename, int type, int isdefault)
{
	// FIXME: Re-sync with win32
    write_log(_T("target_cfgfile_load"));
	int v, i, type2;
	int ct, ct2, size;
	TCHAR tmp1[MAX_DPATH], tmp2[MAX_DPATH];
	TCHAR fname[MAX_DPATH], cname[MAX_DPATH];

	_tcscpy (fname, filename);
	cname[0] = 0;
#if 0
    if (!zfile_exists (fname)) {
		fetch_configurationpath (fname, sizeof (fname) / sizeof (TCHAR));
		if (_tcsncmp (fname, filename, _tcslen (fname)))
			_tcscat (fname, filename);
		else
			_tcscpy (fname, filename);
	}

	if (!isdefault)
		qs_override = 1;
        #endif
    if (type < 0) {
		type = 0;
		cfgfile_get_description(NULL, fname, NULL, NULL, NULL, NULL, NULL, &type);
		if (!isdefault) {
			const TCHAR *p = _tcsrchr(fname, '\\');
			if (!p)
				p = _tcsrchr(fname, '/');
			if (p)
				_tcscpy(cname, p + 1);
		}
	}
	if (type == 0 || type == 1) {
		discard_prefs (p, 0);
	}
	type2 = type;
	if (type == 0 || type == 3) {
		default_prefs (p, true, type);
		write_log(_T("config reset\n"));
#if 0
		if (isdefault == 0) {
			fetch_configurationpath (tmp1, sizeof (tmp1) / sizeof (TCHAR));
			_tcscat (tmp1, OPTIONSFILENAME);
			cfgfile_load (p, tmp1, NULL, 0, 0);
		}
#endif
	}
#if 0
    ct2 = 0;
	regqueryint (NULL, _T("ConfigFile_NoAuto"), &ct2);
#endif
	v = cfgfile_load (p, fname, &type2, ct2, isdefault ? 0 : 1);
 	if (!v)
		return v;
	if (type > 0)
		return v;
#if 0
	if (cname[0])
		_tcscpy(config_filename, cname);
	box_art_check(p);
	for (i = 1; i <= 2; i++) {
		if (type != i) {
			size = sizeof (ct);
			ct = 0;
			regqueryint (NULL, configreg2[i], &ct);
			if (ct && ((i == 1 && p->config_hardware_path[0] == 0) || (i == 2 && p->config_host_path[0] == 0) || ct2)) {
				size = sizeof (tmp1) / sizeof (TCHAR);
				regquerystr (NULL, configreg[i], tmp1, &size);
				fetch_path (_T("ConfigurationPath"), tmp2, sizeof (tmp2) / sizeof (TCHAR));
				_tcscat (tmp2, tmp1);
				v = i;
				cfgfile_load (p, tmp2, &v, 1, 0);
			}
		}
	}
	cfgfile_get_shader_config(p, 0);
#endif
  	v = 1;
	return v;
}
