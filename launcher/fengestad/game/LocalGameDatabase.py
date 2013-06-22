from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

class LocalGameDatabase(object):

    @classmethod
    def get_instance(cls):
        from fs_uae_launcher.Database import Database
        return Database.get_instance()
