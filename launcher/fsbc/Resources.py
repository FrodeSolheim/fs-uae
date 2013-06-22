from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
from pkg_resources import resource_filename, resource_stream
from .Application import Application
from .Paths import Paths


class Resources(object):

    def __init__(self, package, subdir=""):
        self.package = package
        self.subdir = subdir

    def resource_name(self, resource):
        if self.subdir:
            return os.path.join(self.subdir, resource)
        return resource

    def stream(self, resource):
        resource_name = self.resource_name(resource)
        try:
            return resource_stream(self.package, Paths.encode(resource_name))
        except Exception:
            return open(self.path(resource), "rb")

    def path(self, resource):
        print("looking up resource", resource)
        print("- package:", self.package)
        application = Application.get_instance()
        #if application is not None:
        #    share_dir = application.get_share_dir()
        #else:
        #    share_dir = ""
        resource_name = self.resource_name(resource)
        print("resource_name", resource_name)

        # for Windows
        #path = os.path.join(share_dir, self.package, resource_name)
        relative_path = os.path.join(self.package, resource_name)
        try:
            path = application.data_file(relative_path)
        except LookupError:
            path = None
        if path:
            return path
        #print >> sys.stderr, os.getcwd()
        #print >> sys.stderr, path
        #if os.path.exists(path):
        #    return path
        #raise KeyError(
        #    "Cannot find resource {0}".format(repr(resource)))

        try:
            path = resource_filename(self.package, Paths.encode(resource_name))
        except Exception:
            path = None
        #print >> sys.stderr, path
        if not path or not os.path.exists(path):
            raise LookupError(
                "Cannot find resource {0}".format(repr(resource)))
        return path


# import os
# import sys
# import logging
# from fengestad.fs import encode_path, memoize
#
# logger = logging.getLogger('fengestad.gamecenter')
#
# def _(msg):
#     return unicode(msg)
#
# def ngettext(n, msg1, msg2):
#     return unicode(msg1) if n == 1 else unicode(msg2)
#
# class Resources(object):
#     def __init__(self, package_or_requirement):
#         self.req = package_or_requirement
#
#     def resource_pil_image(self, resource_name):
#         resource_name = encode_path(u'res/' + resource_name)
#         return resource_pil_image(self.req, resource_name)
#
#     def resource_stream(self, resource_name):
#         resource_name = encode_path(u'res/' + resource_name)
#         return resource_stream(self.req, resource_name)
#
#     def resource_filename(self, resource_name):
#         print >> sys.stderr, resource_name
#         try:
#             resource_name_2 = encode_path(u'res/' + resource_name)
#             path = resource_filename(self.req, resource_name_2)
#             print >> sys.stderr, path
#             if not os.path.exists(path):
#                 raise Exception("Cannot find resource {0}".format(
#                         repr(resource_name)))
#             return path
#         except Exception:
#             # for Windows
#             path = os.path.join("share", "fs-game-center", resource_name)
#             print >> sys.stderr, os.getcwd()
#             print >> sys.stderr, path
#             if os.path.exists(path):
#                 return path
#             raise Exception(
#                 "Cannot find resource {0}".format(repr(resource_name)))

# def resource_pil_image(package_or_requirement, resource_name):
#     #print("resource_pil_image", package_or_requirement, resource_name)
#     stream = resource_stream(package_or_requirement, resource_name)
#     #from PIL import Image
#     import Image
#     return Image.open(stream)
#
#
# def resource_icon_stream(package_or_requirement, name, size):
#     #print(name, size)
#     resource_name = encode_path(u'res/icons/%dx%d/%s.png' % (size, size, name))
#     return resource_stream(package_or_requirement, resource_name)
#
#
# def resource_icon_pil(package_or_requirement, name, size):
#     #print("resource_icon_pil", name, size)
#     #path = os.path.join(self.path, "icons",
#     #        "%dx%d" % (size, size), name + ".png")
#     #if os.path.isfile(path):
#     #    return Image.open(path)
#     stream = resource_icon_stream(package_or_requirement, name, size)
#     #from PIL import Image
#     import Image
#     return Image.open(stream)
#
#
# resources = Resources("fengestad.gamecenter")
