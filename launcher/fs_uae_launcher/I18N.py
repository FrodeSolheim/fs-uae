from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
import locale
import gettext

#if (fs_config_get_boolean("localization") == 0) {
#    fs_log("localization was forced off\n");
#    return;
#}

translations = gettext.NullTranslations()
#language = ""

def initialize_locale(language=""):
    global translations

    loc = language    
    if not loc:
        try:
            loc, _charset = locale.getdefaultlocale()
            print("locale is", loc)
        except:
            print("exception while checking locale")
            loc = ""
        if not loc:
            loc = ""
    
    if not loc and sys.platform == 'darwin':
        import wx
        mac_locales = {
            wx.LANGUAGE_CZECH: "cs",
            wx.LANGUAGE_DANISH: "da",
            wx.LANGUAGE_ENGLISH: "en",
            wx.LANGUAGE_ENGLISH_UK: "en_GB",
            wx.LANGUAGE_ENGLISH_US: "en_US",
            wx.LANGUAGE_FINNISH: "fi",
            wx.LANGUAGE_FRENCH: "fr",
            wx.LANGUAGE_GERMAN: "de",
            wx.LANGUAGE_HUNGARIAN: "hu",
            wx.LANGUAGE_ITALIAN: "it",
            wx.LANGUAGE_NORWEGIAN_NYNORSK: "nn",
            wx.LANGUAGE_NORWEGIAN_BOKMAL: "nb",
            wx.LANGUAGE_POLISH: "pl",
            wx.LANGUAGE_PORTUGUESE: "pt",
            wx.LANGUAGE_RUSSIAN: "ru",
            wx.LANGUAGE_SERBIAN: "sr",
            wx.LANGUAGE_SPANISH: "es",
            wx.LANGUAGE_SWEDISH: "sv",
            wx.LANGUAGE_SWEDISH_FINLAND: "sv_FI"
        }
        try:
            loc = mac_locales[wx.Locale.GetSystemLanguage()]
        except:
            print("no mapping for wx locale value")
        print("mac locale", loc)
    
    dirs = [os.path.join(os.getcwdu(), "launcher", "share"),
            os.path.join(os.getcwdu(), "share"),
            os.getcwdu(), "/usr/local/share", "/usr/share"]
    
    locale_base = None
    for dir in dirs:
        if not os.path.exists(os.path.join(dir, "fs-uae-launcher",
                "share-dir")):
            continue
        locale_base = os.path.join(dir, "locale")
        print("bindtextdomain fs-uae-launcher:", locale_base)
        gettext.bindtextdomain("fs-uae-launcher", locale_base)
        break
    
    mo_path = None
    if locale_base:
        try:
            mo_path = gettext.find("fs-uae-launcher", locale_base, [loc])
        except Exception:
            # a bug in openSUSE 12.2's gettext.py can cause an exception
            # in gettext.find (checking len of None).
            pass
    print("path to mo file:", mo_path)
    
    translations = gettext.translation("fs-uae-launcher", locale_base, [loc],
            fallback=True)
    print(translations)
    #_ = translations.ugettext
    #ngettext = translations.ungettext

def _(msg):
    return translations.ugettext(msg)

def ngettext(n, msg1, msg2):
    return translations.ungettext(n, msg1, msg2)
