from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.six as six
import os
import json
from gzip import GzipFile
from fs_uae_launcher.six.moves import cStringIO as StringIO
try:
    from urllib.request import HTTPBasicAuthHandler, build_opener, Request
except ImportError:
    from urllib2 import HTTPBasicAuthHandler, build_opener, Request
try:
    from urllib.parse import quote_plus
except ImportError:
    from urllib import quote_plus
from urllib import urlencode


class OAGDClient(object):

    def __init__(self):
        self._server = None
        self._opener = None

        # FIXME: we don't want dependency on Settings here
        from fs_uae_launcher.Settings import Settings
        self.username = Settings.get("database_username")
        self.password = Settings.get("database_password")

    def server(self):
        if self._server:
            return self._server
        try:
            self._server = os.environ["FS_GAME_DATABASE_SERVER"]
        except KeyError:
            self._server = "oagd.net"
        return self._server

    def opener(self):
        if self._opener:
            return self._opener
        server = self.server()
        auth_handler = HTTPBasicAuthHandler()
        auth_handler.add_password(
            realm="Open Amiga Game Database",
            uri="http://{0}".format(server), user=self.username,
            passwd=self.password)
        self._opener = build_opener(auth_handler)
        return self._opener

    def build_url(self, path, **kwargs):
        url = "http://{0}{1}".format(self.server(), path)
        if kwargs:
            url += "?" + urlencode(kwargs)
        return url

    def handle_response(self, response):
        self._json = None
        self.data = response.read()

        #print(dir(response.headers))
        content_encoding = response.headers.getheader(
            "content-encoding", "").lower()
        if content_encoding == "gzip":
            #data = zlib.decompress(data)
            fake_stream = StringIO(self.data)
            self.data = GzipFile(fileobj=fake_stream).read()

    def json_response(self):
        if self._json is None:
            self._json = json.loads(self.data)
        return self._json

    def get(self, key, default):
        doc = self.json_response()
        return doc.get(key, default)

    def get_request(self, url):
        opener = self.opener()
        request = Request(url)
        print("get_request:", url)
        request.add_header("Accept-Encoding", "gzip")
        response = opener.open(request)
        return self.handle_response(response)

    def rate_variant(self, variant_uuid, like=None, work=None):
        params = {
            "game": variant_uuid,
        }
        if like is not None:
            params["like"] = like
        if work is not None:
            params["work"] = work
        url = self.build_url("/api/1/rate_game", **params)
        self.get_request(url)
