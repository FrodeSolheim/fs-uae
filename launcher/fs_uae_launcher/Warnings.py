from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

class Warnings:

    @classmethod
    def set(cls, category, item, text):
        print("WARNING:", category, item, text)
