from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import json
import xml.etree.ElementTree
from xml.etree.cElementTree import ElementTree, fromstring
from .Database import Database

class ValueConfigLoader:

    def __init__(self):
        self.config = {}
        self.options = {}

    def get_config(self):
        return self.config.copy()

    def load_values(self, values):
        self.values = values

        cd_based = False
        amiga_model = "A500"
        platform = self.values.get("platform", "Amiga")
        if platform == "Amiga":
            pass
        elif platform == "CD32":
            amiga_model = "CD32"
            cd_based = True
            self.options["joystick_port_1_mode"] = "cd32 gamepad"
        elif platform == "CDTV":
            amiga_model = "CDTV"
            cd_based = True
        else:
            raise Exception("unknown platform")
        self.config["amiga_model"] = amiga_model

        #self.config["x_game_uuid"] = self.values["uuid"]

        self.viewport = []
        #game_node = self.root.find("game")
        #if game_node is not None:
        #    game_uuid = game_node.get("uuid", "")
        #    self.config["x_game_uuid"] = game_uuid
        #    self.load_game_info(game_uuid)

        #self.load_options())
        for key in sorted(values):
            self.load_option(key, values[key])

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
        #self.config["x_config_uuid"] = self.root.get("uuid", "")

        game_name = self.values.get("game_name", "")
        platform_name = self.values.get("platform", "")
        variant_name = self.values.get("variant_name", "")
        parts = []
        if platform_name:
            parts.append(platform_name)
        if variant_name:
            parts.append(variant_name)
        if game_name and variant_name:
            config_name = u"{0} ({1})".format(game_name, u", ".join(parts))
            self.config["x_config_name"] = config_name
        else:
            self.config["x_config_name"] = self.values.get("config_name", "")

    def load_cdroms(self):
        print("\n\n\nload_cdroms\n\n\n")
        media_list = self.build_media_list(cds=True)
        for i, values in enumerate(media_list):
            path, sha1 = values
            if i < 1:
                self.config["cdrom_drive_{0}".format(i)] = path
                self.config["x_cdrom_drive_{0}_sha1".format(i)] = sha1
            self.config["cdrom_image_{0}".format(i)] = path
            self.config["x_cdrom_image_{0}_sha1".format(i)] = sha1

    def load_option(self, key, value):
        if key == "viewport":
            if "=" in value:
                value = value.replace("=", "=>")
                value = value.replace("==>", "=>")
            else:
                value = "* * * * => " + value
            self.viewport.append(value)
        #if key.startswith("viewport_"):
        #    parts = key.split("_")
        #    if len(parts) == 5:
        #        parts = parts[1:]
        #        value = " ".join(parts) + " => " + value
        #        value = value.replace("x", "*")
        #        self.viewport.append(value)
        elif key == "whdload_args":
            self.options["x_whdload_args"] = value
        elif key == "kickstart":
            if value in ["2.0", "2.0+"]:
                model = self.options.get("amiga_model", "")
                if value == "2.0+" and model == "A1200":
                    pass
                elif model == "A500+":
                    pass
                else:
                    self.options["amiga_model"] = "A600"
            elif value in ["3.0", "3.0+", "3.1", "3.1+"]:
                self.options["amiga_model"] = "A1200"
            elif value == "AROS":
                self.options["kickstart_file"] = "internal"
            else:
                # FIXME: print warning
                pass
        elif key == "chipset":
            if value == "ECS":
                self.options["amiga_model"] = "A600"
            elif value == "AGA":
                self.options["amiga_model"] = "A1200"
        elif key == "cracktro":
            # FIXME: handle
            pass
        elif key in ["amiga_model", "joystick_port_0_mode",
                "floppy_drive_count", "slow_memory", "fast_memory"]:
            self.options[key] = value

    #def load_game_info(self, uuid):
    #    print("load_game_info", uuid)
    #    path = Database.get_instance().find_game(uuid=uuid)
    #    if not path:
    #        print("game xml file not found")
    #        return
    #    tree = ElementTree()
    #    tree.parse(path)
    #    self.config["x_game_xml_path"] = path
    #    self.load_options_from_tree(tree)

    def load_floppies(self):
        media_list = self.build_media_list(floppies=True)
        floppy_drive_count = 4
        if "floppy_drive_count" in self.options:
            try:
                floppy_drive_count = int(self.options["floppy_drive_count"])
            except ValueError:
                floppy_drive_count = 1
            floppy_drive_count = max(0, min(4, floppy_drive_count))
        for i, values in enumerate(media_list):
            path, sha1 = values
            if i < floppy_drive_count:
                self.config[u"floppy_drive_{0}".format(i)] = path
                self.config[u"x_floppy_drive_{0}_sha1".format(i)] = sha1
            self.config[u"floppy_image_{0}".format(i)] = path
            self.config[u"x_floppy_image_{0}_sha1".format(i)] = sha1
        if floppy_drive_count < 4:
            self.config["floppy_drive_count"] = floppy_drive_count

    def build_media_list(self, floppies=False, cds=False, hds=False):
        media_list = []
        added = set()
        #file_nodes = self.root.findall("file")
        file_list_json = self.values.get("file_list", "[]")
        file_list = json.loads(file_list_json)
        for file_item in file_list:
            sha1 = file_item["sha1"]
            name = file_item["name"]
            url = file_item.get("url", "")

            #type = file_node.get("type", "")
            #name = file_node.find("name").text.strip()

            if name.startswith("HardDrive/"):
                if hds:
                    p = os.path.join(self.path, "HardDrive")
                    if p in added:
                        # already added
                        continue
                    added.add(p)
                    # FIXME: hack for now
                    sha1 = "01234567-89ab-cdef-0123-456789abcdef"
                    media_list.append((p, sha1))
                else:
                    continue

            base, ext = os.path.splitext(name)
            #if type == "hd" and not hds:
            #    continue
            if hds:
                #if type and not type == "HD":
                #    continue
                if ext not in [".zip"]:
                    continue
            elif cds:
                #if type and not type == "cd":
                #    continue
                if ext not in [".cue", ".iso"]:
                    continue
                if "(Track" in base:
                    # probably part of a split multi-track cue
                    continue
            elif floppies:
                #if type and not type == "floppy":
                #    continue
                if ext not in [".adf", ".adz", ".dms", ".ipf"]:
                    continue

            path = ""
            found_sha1 = ""
            if sha1:
                print(sha1)
                path = Database.get_instance().find_file(sha1=sha1)
                if path:
                    found_sha1 = sha1
            if url and not path:
                path = url
                found_sha1 = sha1
            if not path:
                path = Database.get_instance().find_file(name=name)
            if not path:
                if self.path:
                    # loaded from an external XML file:
                    path = os.path.join(self.path, name)
            if path:
                media_list.append((path, found_sha1))
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
        for i, values in enumerate(media_list):
            path, sha1 = values
            self.config["hard_drive_{0}".format(i)] = path
            self.config["x_hard_drive_{0}_sha1".format(i)] = sha1
