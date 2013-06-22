from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import xml.etree.cElementTree as ET

class DatFile:
    
    def __init__(self, file=None):
        self.reset()
        self.extensions = []
        if file is not None:
            self.load(file)

    def load(self, file):
        if hasattr(file, "read"):
            self._load_file(file)
        else:
            self._load_path(file)

    def _load_path(self, path):
        with open(path, "rb") as f:
            self._load_file(f)

    def _load_file(self, f):
        self.load_data(f.read())

    def load_data(self, data):
        if data.startswith("<"):
            # assume XML
            self._load_xml(data)
        else:
            self._load_dat(data)

    def reset(self):
        self.games = []
        self.description = ""

    def _load_xml(self, data):
        self.reset()

        root = ET.fromstring(data)
        header_node = root.find("header")
        self.description = header_node.find("description").text.strip()
        for game_node in root.findall("game"):
            game = {
                "name": "",
                "files": [],
            }
            game["name"] = game_node.attrib["name"]
            for rom_node in game_node.findall("rom"):
                rom = {}
                rom["name"] = rom_node.attrib["name"] 
                rom["size"] = int(rom_node.attrib["size"])
                rom["crc32"] = rom_node.attrib["crc"]
                rom["md5"] = rom_node.attrib["md5"]
                rom["sha1"] = rom_node.attrib["sha1"]
                game["files"].append(rom)
            self.games.append(game)

    def _load_dat(self, data):
        self.reset()

        # this is a very quick-and-dirty/hackish parser...
        game = None
        for line in data.split("\n"):
            line = line.strip()
            if line.startswith("game ("):
                game = {
                    "name": "",
                    "files": [],
                }
                self.games.append(game)
            if line.startswith("name "):
                if game is not None:
                    if '"' in line:
                        parts = line.split('"')
                        assert len(parts) == 3
                        game["name"] = parts[1]
                    else:
                        game["name"] = line[5:]
            elif line.startswith("description "):
                if game is None:
                    parts = line.split('"')
                    assert len(parts) == 3
                    self.description = parts[1]
            elif line.startswith("rom ("):
                rom = {}
                parts = line.split('"')
                if len(parts) == 3:
                    rom["name"] = parts[1]
                else:
                    parts = line.split(' ')
                    rom["name"] = parts[3]
                n, ext = os.path.splitext(rom["name"])
                if self.extensions and ext.lower() not in self.extensions:
                    continue
                parts = line.split(" size ")
                rom["size"] = int(parts[1].strip().split(" ")[0].strip())
                parts = line.split(" crc ")
                rom["crc32"] = parts[1].strip().split(" ")[0].strip()
                parts = line.split(" md5 ")
                rom["md5"] = parts[1].strip().split(" ")[0].strip()
                parts = line.split(" sha1 ")
                rom["sha1"] = parts[1].strip().split(" ")[0].strip()
                game["files"].append(rom)
