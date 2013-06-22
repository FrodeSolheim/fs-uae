import os
import sys
import logging
from pkg_resources import resource_filename, resource_stream
from fengestad.fs import encode_path, memoize
from fsbc.Resources import Resources as BaseResources

logger = logging.getLogger('fengestad.gamecenter')

def _(msg):
    return unicode(msg)

def ngettext(n, msg1, msg2):
    return unicode(msg1) if n == 1 else unicode(msg2)

class Resources(BaseResources):
    def __init__(self, package):
        BaseResources.__init__(self, package, "res")
        self.req = package

    def resource_pil_image(self, resource):
        #resource_name = encode_path(u'res/' + resource_name)
        #return resource_pil_image(self.req, resource_name)
        stream = self.stream(resource)
        #from PIL import Image
        import Image
        return Image.open(stream)

    def resource_stream(self, resource):
        return self.stream(resource)

    def resource_filename(self, resource):
        return self.path(resource)


def resource_pil_image(package_or_requirement, resource_name):
    #print("resource_pil_image", package_or_requirement, resource_name)
    stream = resource_stream(package_or_requirement, resource_name)
    #from PIL import Image
    import Image
    return Image.open(stream)


# def resource_icon_stream(package_or_requirement, name, size):
#     #print(name, size)
#     resource_name = encode_path(u'res/icons/%dx%d/%s.png' % (size, size, name))
#     return resource_stream(package_or_requirement, resource_name)


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


resources = Resources("fengestad.gamecenter")
