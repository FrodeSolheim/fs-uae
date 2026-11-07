# # FIXME: Maybe don't include PIL and instead use SDL3/SDL3_Image via fsapp?
# from PIL import Image


# class DataImage:
#     """Image class which supports lazy loading from data file (on demand).

#     Size must be specified if you want to use the size in code without loading the image.
#     """

#     def __init__(self, name: str, size: tuple[int, int] | None):
#         self._name = name
#         self._size = size
#         self._loaded = False

#     def get_size(self) -> tuple[int, int]:
#         if self._size is None and not self._loaded:
#             self.load()
#         return self._size

#     def load(self):
#         if self._loaded:
#             return
#         # FIXME: Need to support more flexible data locations
#         path = "data/" + self.name
#         im = Image.open(path)
#         # im.load()
#         self._size = im.size
