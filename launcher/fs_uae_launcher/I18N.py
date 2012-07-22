from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import sys
import locale
import gettext

#if (fs_config_get_boolean("localization") == 0) {
#    fs_log("localization was forced off\n");
#    return;
#}
#print(dir(locale))
#loc = locale.setlocale(locale.LC_ALL, "")
#if loc:
#    print("locale is set to", loc)
#else:
#    print("failed to set current locale")
#locale.setlocale(locale.LC_NUMERIC, "C")

try:
    loc, _charset = locale.getdefaultlocale()
    print("locale is", loc)
except:
    print("exception while checking locale")
    loc = ""
if not loc:
    loc = ""

"""
if sys.platform == 'darwin':
    import wx
    mac_locales = {
        wx.LANGUAGE_DANISH: ('da_DK', 'da_DK'),
        wx.LANGUAGE_ENGLISH: ('en_US', 'en_US'),
        wx.LANGUAGE_ENGLISH_UK: ('en_GB', 'en_GB'),
        wx.LANGUAGE_ENGLISH_US: ('en_US', 'en_US'),
        wx.LANGUAGE_FINNISH: ('fi_FI', 'fi_FI'),
        wx.LANGUAGE_NORWEGIAN_NYNORSK: ('nn_NO', 'no_NO'),
        wx.LANGUAGE_NORWEGIAN_BOKMAL: ('nb_NO', 'no_NO'),
        wx.LANGUAGE_SWEDISH: ('sv_SE', 'sv_SE'),
        wx.LANGUAGE_SWEDISH_FINLAND: ('sv_FI', 'fi_FI'),
    }
    try:
        loc, set_locale_to = mac_locales[wx.Locale.GetSystemLanguage()]
    except:
        print("exception while setting locale")
    print("mac locale", loc)
"""

#gettext.textdomain("fs-uae-launcher")

dirs = [os.path.join(os.getcwd(), "launcher", "share"),
        os.path.join(os.getcwd(), "share"),
        os.getcwd(), "/usr/local/share", "/usr/share"]

locale_base = None
for dir in dirs:
    if not os.path.exists(os.path.join(dir, "fs-uae-launcher", "share-dir")):
        continue
    locale_base = os.path.join(dir, "locale")
    print("bindtextdomain fs-uae-launcher:", locale_base)
    gettext.bindtextdomain("fs-uae-launcher", locale_base)
    break

#gettext.bind_textdomain_codeset("fs-uae-launcher", "UTF-8");

"""
def _(message):
    #return unicode(text)
    return gettext.dgettext("fs-uae-launcher", message).decode("UTF-8")

def ngettext(msgid, msgid2, n):
    return gettext.dngettext("fs-uae-launcher", msgid1, msgid2,
            n).decode("UTF-8")

print(_("Close"))
print(gettext.textdomain())

"""

if locale_base:
    mo_path = gettext.find("fs-uae-launcher", locale_base, [loc])
else:
    mo_path = None
print("path to mo file:", mo_path)

translations = gettext.translation("fs-uae-launcher", locale_base, [loc],
        fallback=True)
print(translations)

_ = translations.ugettext
ngettext = translations.ungettext

#print(_("Close"))

#import sys
#sys.exit(1)

#    if num == 1:
#        return text
#    return plural

#_ = gettext.ugettext
#ngettext = gettext.ungettext
