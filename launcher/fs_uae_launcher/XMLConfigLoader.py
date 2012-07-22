from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import xml.etree.ElementTree
from xml.etree.cElementTree import ElementTree
from .Database import Database

class XMLConfigLoader:

    def __init__(self):
        self.config = {}
        self.options = {}

    def get_config(self):
        return self.config.copy()

    def load(self, path):
        self.tree = ElementTree()
        self.tree.parse(path)
        self.root = self.tree.getroot()

        cd_based = False
        amiga_model = "A500"
        platform = self.root.find("game").find("platform").text.strip()

        if platform == "CD32":
            amiga_model = "CD32"
            cd_based = True

            self.options["joystick_port_1_mode"] = "cd32 gamepad"

        elif platform == "CDTV":
            amiga_model = "CDTV"
            cd_based = True
        self.config["amiga_model"] = amiga_model

        self.viewport = []
        game_node = self.root.find("game")
        if game_node is not None:
            game_uuid = game_node.get("uuid", "")
            self.config["x_game_uuid"] = game_uuid
            self.load_game_info(game_uuid)

        self.load_options_from_tree(self.tree)
        """
        if self.root.find("options"):
           for node in self.root.find("options"):
               print(node.tag)
               print(node.text)
               key = node.tag.replace("-", "_")
               value = node.text
               if key == "viewport":
                   if "=" in value:
                       value = value.replace("=", "=>")
                       value = value.replace("==>", "=>")
                   else:
                       value = "* * * * => " + value
                   viewport.append(value)
                   continue
               self.options[key] = value
        """
        if self.viewport:
            self.options["viewport"] = ", ".join(self.viewport)

        #self.options["sub_title"] = amiga_model + " - FS-UAE"
        #self.options["sub_title"] = "FS-UAE ({0})".format(amiga_model)

        if cd_based:
            self.load_cdroms()
        else:
            self.load_floppies()

        self.load_hard_drives()
        # for now, just copy all options to config without checking
        for key, value in self.options.iteritems():
           self.config[key] = value

        self.set_name_and_uuid()

    def set_name_and_uuid(self):
        self.config["x_config_uuid"] = self.root.get("uuid", "")
        game_name = ""
        platform_name = ""
        variant_name = ""

        name_node = self.root.find("name")
        if name_node is not None:
            variant_name = name_node.text.strip()
        game_node = self.root.find("game")
        if game_node is not None:
            game_name_node = game_node.find("name")
            if game_name_node is not None:
                game_name = game_name_node.text.strip()
            game_platform_node = game_node.find("platform")
            if game_platform_node is not None:
                platform_name = game_platform_node.text.strip()

        parts = []
        if platform_name:
            parts.append(platform_name)
        if variant_name:
            parts.append(variant_name)
        if game_name and variant_name:
            config_name = u"{0} ({1})".format(game_name, u", ".join(parts))
            self.config["x_config_name"] = config_name

    def load_cdroms(self):
        print("\n\n\nload_cdroms\n\n\n")
        media_list = self.build_media_list(cds=True)
        for i, path in enumerate(media_list):
            if i < 1:
                self.config["cdrom_drive_{0}".format(i)] = path
            self.config["cdrom_image_{0}".format(i)] = path

    def load_options_from_tree(self, tree):
        root = tree.getroot()
        if root.find("options"):
           for node in root.find("options"):
               print(node.tag)
               print(node.text)
               key = node.tag.replace("-", "_")
               value = node.text or ""
               if key == "viewport":
                   if "=" in value:
                       value = value.replace("=", "=>")
                       value = value.replace("==>", "=>")
                   else:
                       value = "* * * * => " + value
                   self.viewport.append(value)
                   continue
               elif key == "whdload_args":
                   key = "x_whdload_args"
               elif key == "kickstart":
                   if value == "2.0+":
                       key = "amiga_model"
                       value = "A600"
                   else:
                       # FIXME: print warning
                       continue
               elif key == "cracktro":
                   # FIXME: handle
                   continue
               self.options[key] = value

    def load_game_info(self, uuid):
        print("load_game_info", uuid)
        path = Database.get_instance().find_game(uuid=uuid)
        if not path:
            print("game xml file not found")
            return
        tree = ElementTree()
        tree.parse(path)
        self.config["x_game_xml_path"] = path
        self.load_options_from_tree(tree)

    def load_floppies(self):
        media_list = self.build_media_list(floppies=True)
        num_floppy_drives = 4
        if "num_floppy_drives" in self.options:
            try:
                num_floppy_drives = int(self.options["num_floppy_drives"])
            except ValueError:
                num_floppy_drives = 1
            num_floppy_drives = max(0, min(4, num_floppy_drives))
        for i, path in enumerate(media_list):
            if i < num_floppy_drives:
                self.config[u"floppy_drive_{0}".format(i)] = path
            self.config[u"floppy_image_{0}".format(i)] = path
        if num_floppy_drives < 4:
            self.config["num_floppy_drives"] = num_floppy_drives

    def build_media_list(self, floppies=False, cds=False, hds=False):
        media_list = []
        file_nodes = self.root.findall("file")
        for file_node in file_nodes:
            print(file_node)
            sha1 = ""
            #for cs_node in file_node.findall("checksum"):
            #    if cs_node.get("type") == "sha1":
            #        sha1 = cs_node.text.strip()

            sha1_node = file_node.find("sha1")
            if sha1_node is not None:
                sha1 = sha1_node.text.strip()
                print("sha1", sha1)

            type = file_node.get("type", "")
            name = file_node.find("name").text.strip()
            base, ext = os.path.splitext(name)
            #if type == "hd" and not hds:
            #    continue
            if hds:
                if type and not type == "HD":
                    continue
                if ext not in [".zip"]:
                    continue
            elif cds:
                if type and not type == "cd":
                    continue
                if ext not in [".cue", ".iso"]:
                    continue
                if "(Track" in base:
                    # probably part of a split multi-track cue
                    continue
            elif floppies:
                if type and not type == "floppy":
                    continue

            path = ""
            if sha1:
                print(sha1)
                path = Database.get_instance().find_file(sha1=sha1)
            if not path:
                path = Database.get_instance().find_file(name=name)
            if path:
                media_list.append(path)
            else:
                pass
                #return False
                # FIXME: handle it with a visible error message
                #raise Exception("could not find file " + repr(name))
        return media_list

    def load_hard_drives(self):
        print("load_hard_drives")
        media_list = self.build_media_list(hds=True)
        print(media_list)
        for i, path in enumerate(media_list):
            self.config[u"hard_drive_{0}".format(i)] = path

