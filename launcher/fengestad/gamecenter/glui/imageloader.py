from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division

import os
import time
import urllib2
import threading
from fsgs import fsgs
from fengestad.gamecenter.gamecenter import GameCenter
from fengestad.gamecenter.resources import resources, logger, ngettext, _
from fengestad.base.Application import Application

from .texturemanager import TextureManager

USE_WX = False
USE_PIL = False
try:
    import _no_fengestad_image
except ImportError:
    #try:
    #    import wx
    #    USE_WX = True
    #except ImportError:
        import Image
        USE_PIL = True

error_set = set()

class ImageData(dict):

    def __init__(self):
        dict.__init__(self)

    def __del__(self):
        #print("ImageData.__del__")
        if "data" in self and self["data"] is not None:
            _no_fengestad_image.free_image(self)

def load_jpeg(path):
    image_data = ImageData()
    #image_data["hallo"] = "1"
    #print image_data
    #if draft_size is None:
    #    draft_size = (0, 0)
    #print(path, os.path.exists(path))
    _no_fengestad_image.load_jpeg(image_data, path, 0, 0)
    return image_data

def load_png(file):
    image_data = ImageData()
    _no_fengestad_image.load_png(image_data, file)
    return image_data

def get_file_for_sha1(sha1):
    #print("get_cache_path_for_sha1", sha1)
    """
    if request.args.get("is_cover", False):
        size_arg = "?size={0}".format(256)
        cache_ext = "_{0}".format(256)
    elif request.size:
        size_arg = "?w={0}&h={1}".format(request.size[0],
                request.size[1])
        cache_ext = "_{0}x{1}".format(request.size[0],
                request.size[1])
    else:
        size_arg = ""
        cache_ext = ""
    """

    #size_arg = "?size={0}".format(512)
    sha1, size_arg = sha1.split("?")
    if size_arg == "s=1x":
        cache_ext = "_1x.png"
    elif size_arg == "s=512&f=jpg":
        cache_ext = "_512.jpg"
    elif size_arg == "w=480&h=640&t=lbcover&f=jpg":
        cache_ext = "_480x640_lbcover.jpg"
    else:
        raise Exception("unrecognized size")
    
    #cache_ext = "_{0}".format(512)

    cache_dir = os.path.join(fsgs.cache_dir, "Images", sha1[:3])

    #cache_dir = os.path.join(Settings.get_cache_dir(),
    #        "Images", sha1[:3])
    if not os.path.exists(cache_dir):
        os.makedirs(cache_dir)

    cache_file = os.path.join(cache_dir, sha1 + cache_ext)
    #print(cache_file)
    if os.path.exists(cache_file):
        return cache_file

    try:
        server = os.environ["FS_GAME_DATABASE_SERVER"]
    except KeyError:
        server = "oagd.net"

    cache_file_part = cache_file + ".part"
    with open(cache_file_part, "wb") as f:
        url = "http://{0}/image/{1}?{2}".format(server, sha1, size_arg)
        print(url)
        r = urllib2.urlopen(url)
        #print(r)
        data = r.read()
        #h = hashlib.sha1()
        #h.update(data)
        #if h.hexdigest() == sha1:
        f.write(data)
    os.rename(cache_file_part, cache_file)
    return cache_file

def load_image(relative_path):
    #print("load_image", relative_path)
    if relative_path.startswith("sha1:"):
        sha1 = relative_path[5:]
        path = get_file_for_sha1(sha1)
        #print(path)
    else:

        base_path = os.path.join(GameCenter.get_local_data_dir())
        path = os.path.join(base_path, relative_path)
        if not os.path.exists(path):
            base_path = os.path.join(GameCenter.data_dir, "info")
            path = os.path.join(base_path, relative_path)

    if path in error_set:
        return None, (0, 0)
    if not os.path.exists(path):
        return None, (0, 0)

    try:
        if USE_PIL:
            # a few screenshot sizes (586 x 512, ...) has problems
            # when uploading to texture with GL_RGB, but works when the source is
            # GL_RGBA... ?
            im = Image.open(path)
            im.load()
            if im.mode != "RGBA":
                im = im.convert("RGBA")
            pixels = im.tostring("raw", "RGBA")
            #print(id(pixels))
            return pixels, im.size
        if USE_WX:
            print("load", path)
            im = wx.Image(path)
            print(im.Ok())
            pixels = im.GetData()
            print(id(pixels))
            return pixels, (im.GetWidth(), im.GetHeight())

        if path.endswith(".png"):
            data = load_png(path)
        elif path.endswith(".jpg"):
            data = load_jpeg(path)
        else:
            raise Exception("no image file extension for " + repr(path))
        pixels = data["data"]
        pixels = str(pixels)
        return pixels, (data["width"], data["height"])
        # HACK: Add data to size tuple to ensure the object lives long
        # enough so that __del__ will not be called too soon..
        #return pixels, (data["width"], data["height"], data)
    except Exception, e:
        print("error loading", repr(path), repr(e))
        error_set.add(path)
        return None, (0, 0)


class ImageLoader():

    __instance = None

    @classmethod
    def get(cls):
        if not cls.__instance:
            cls.__instance = cls()
        return cls.__instance

    def __init__(self):
        self._stop_flag = False

    def start(self):
        threading.Thread(target=self.image_loader_thread).start()
        #print("FIXME: not starting ImageLoader thread")
        pass

    def image_loader_thread(self):
        logger.debug("Image loader started")
        tm = TextureManager().get()
        while not self._stop_flag:
            time.sleep(0.01)

            load_ip = None
            with tm.lock:
                for ip in tm.image_list:
                    if tm.image_dict[ip][0] is False:
                        load_ip = ip
                        break
            if load_ip:
                pixels, size = load_image(load_ip)
                tm.set_image(ip, pixels, size)

    def stop(self):
        self._stop_flag = True
