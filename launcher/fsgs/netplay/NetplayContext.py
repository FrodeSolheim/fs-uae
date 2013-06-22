from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from fs_uae_launcher.netplay.Netplay import Netplay
from ..BaseContext import BaseContext

class NetplayContext(BaseContext):

    def __init__(self, main_context):
        BaseContext.__init__(self, main_context)

    @property
    def enabled(self):
        return Netplay.enabled
