from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import shutil
try:
    from urllib.request import urlopen
except ImportError:
    from urllib2 import urlopen
import hashlib

# FIXME: temporary package dependency cycle, must be fixed
from fs_uae_launcher.FSUAEDirectories import FSUAEDirectories

class Downloader:

    @classmethod
    def cache_file_from_url(cls, url, download=True):
        print("DownloadService.cache_file_from_url", url)

        cache_path = cls.get_url_cache_path(url)
        if os.path.exists(cache_path):
            print("(in cache)")
            # so we later can delete least accessed files in cache...
            os.utime(cache_path, None)
            return cache_path
        if not download:
            return None
        ifs = urlopen(url)
        temp_cache_path = cache_path + ".partial"
        with open(temp_cache_path, "wb") as ofs:
            while True:
                data = ifs.read(65536)
                if not data:
                    break
                ofs.write(data)
        os.rename(temp_cache_path, cache_path)
        return cache_path

    @classmethod
    def install_file_from_url(cls, url, path):
        print("DownloadService.install_file_from_url", url)
        print(repr(path))
        if not os.path.exists(os.path.dirname(path)):
            os.makedirs(os.path.dirname(path))
        cache_path = self.cache_file_from_url(url)
        temp_path = path + ".partial"
        shutil.copy(cache_path, temp_path)
        os.rename(temp_path, path)

    @classmethod
    def install_file_by_sha1(cls, sha1, name, path):
        print("DownloadService.install_file_by_sha1", sha1)
        print(repr(path))
        if not os.path.exists(os.path.dirname(path)):
            os.makedirs(os.path.dirname(path))
        cache_path = cls.get_cache_path(sha1)
        if os.path.exists(cache_path):
            print("(in cache)")
            shutil.copy(cache_path, path)
            # so we later can delete least accessed files in cache...
            os.utime(cache_path, None)
            return
        url = cls.sha1_to_url(sha1, name)
        ifile = urlopen(url)
        temp_path = path + ".part"
        h = hashlib.sha1()
        with open(temp_path, "wb") as ofile:
            while True:
                data = ifile.read(65536)
                if not data:
                    break
                h.update(data)
                ofile.write(data)
        if h.hexdigest() != sha1:
            print("error: downloaded sha1 is", sha1)
            raise Exception("sha1 of downloaded file does not match")
        temp_cache_path = cache_path + ".part"
        shutil.copy(temp_path, temp_cache_path)
        os.rename(temp_cache_path, cache_path)
        os.rename(temp_path, path)

    @classmethod
    def sha1_to_url(self, sha1, name):
        url = "http://fengestad.no/s/sha1/{0}/{1}".format(sha1, name)
        print(url)
        return url

    @classmethod
    def get_cache_path(cls, sha1):
        path = os.path.join(FSUAEDirectories.get_cache_dir(),
                "Downloads", sha1[:3])
        if not os.path.exists(path):
            os.makedirs(path)
        return os.path.join(path, sha1)

    @classmethod
    def get_url_cache_path(cls, url):
        return cls.get_cache_path(hashlib.sha1(url).hexdigest())
