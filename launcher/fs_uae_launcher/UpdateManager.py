from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import urllib2
import traceback
import threading
from .Signal import Signal
from .Version import Version
import fs_uae_launcher.fs as fs

class UpdateManager:

    @classmethod
    def run_update_check(cls):
        threading.Thread(target=cls.update_thread_function).start()

    @classmethod
    def update_thread_function(cls):
        try:
            cls._update_thread_function()
        except:
            traceback.print_exc()

    @classmethod
    def _update_thread_function(cls):
        series = Version.SERIES
        if fs.windows:
            platform = "windows"
        elif fs.macosx:
            platform = "macosx"
        elif fs.linux:
            platform = "linux"
        else:
            platform = "other"
        url = "http://fengestad.no/fs-uae/{0}/latest-{1}".format(series,
                platform)
        f = urllib2.urlopen(url)
        version_str = f.read().strip().decode("UTF-8")
        result = fs.compare_versions(version_str, Version.VERSION)
        print("update check result: ", result)
        if result > 0:
            # a new version is available
            #if series == "stable":
            #    web_url = "http://fengestad.no/fs-uae/download"
            #else:
            web_url = "http://fengestad.no/fs-uae/{0}/download/".format(
                    series)
            Signal.broadcast("update_available", version_str, web_url)
