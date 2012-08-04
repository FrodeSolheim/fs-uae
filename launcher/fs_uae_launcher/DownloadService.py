from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import shutil
import urllib2
import hashlib
from .Settings import Settings

class DownloadService:

    @classmethod
    def install_file_from_url(cls, url, path):
        print("DownloadService.install_file_from_url", url)
        print(repr(path))
        if not os.path.exists(os.path.dirname(path)):
            os.makedirs(os.path.dirname(path))
        cache_path = cls.get_url_cache_path(url)
        if os.path.exists(cache_path):
            print("(in cache)")
            shutil.copy(cache_path, path)
            # so we later can delete least accessed files in cache...
            os.utime(cache_path, None)
            return
        ifile = urllib2.urlopen(url)
        temp_path = path + u".part"
        #h = hashlib.sha1()
        with open(temp_path, "wb") as ofile:
            while True:
                data = ifile.read(65536)
                if not data:
                    break
                #h.update(data)
                ofile.write(data)
        #if h.hexdigest() != sha1:
        #    print("error: downloaded sha1 is", sha1)
        #    raise Exception("sha1 of downloaded file does not match")
        temp_cache_path = cache_path + u".part"
        shutil.copy(temp_path, temp_cache_path)
        os.rename(temp_cache_path, cache_path)
        os.rename(temp_path, path)

    @classmethod
    def install_file_by_sha1(cls, sha1, path):
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
        url = cls.sha1_to_url(sha1)
        ifile = urllib2.urlopen(url)
        temp_path = path + u".part"
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
        temp_cache_path = cache_path + u".part"
        shutil.copy(temp_path, temp_cache_path)
        os.rename(temp_cache_path, cache_path)
        os.rename(temp_path, path)

    @classmethod
    def sha1_to_url(self, sha1):
        return "http://fengestad.no/sha1/{0}/{1}".format(sha1[:2], sha1)

    @classmethod
    def get_cache_path(cls, sha1):
        path = os.path.join(Settings.get_launcher_dir(), "Cache", 
                "Download", sha1[:2])
        if not os.path.exists(path):
            os.makedirs(path)
        return os.path.join(path, sha1)

    @classmethod
    def get_url_cache_path(cls, url):
        h = hashlib.sha1()
        h.update(url)
        sha1 = h.hexdigest()
        path = os.path.join(Settings.get_launcher_dir(), "Cache", 
                "Download", sha1[:2])
        if not os.path.exists(path):
            os.makedirs(path)
        return os.path.join(path, sha1)
