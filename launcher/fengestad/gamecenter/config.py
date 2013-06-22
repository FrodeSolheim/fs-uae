
from fengestad import fs

class Config:
    
    @classmethod
    def get(cls, key, default=None):
        return fs.config_string(key, default)

    @classmethod
    def get_bool(cls, key, default):
        return fs.config_bool(key, default)

    @classmethod
    def get_int(cls, key, default):
        return fs.config_int(key, default)
        