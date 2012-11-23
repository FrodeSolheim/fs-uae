from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
import time
import json
import traceback
import xml.etree.ElementTree
from xml.etree.cElementTree import ElementTree
#from .Database import Database
from .Settings import Settings
from .I18N import _, ngettext
from .Util import get_real_case
from .fsgs.GameDatabaseClient import GameDatabaseClient

class ConfigurationScanner:

    def __init__(self, paths, on_status=None, stop_check=None):
        self.paths = paths
        self.on_status = on_status
        self._stop_check = stop_check
        self.scan_count = 0
        self.scan_version = int(time.time() * 100)

    def stop_check(self):
        if self._stop_check:
            return self._stop_check()

    def set_status(self, title, status):
        if self.on_status:
            self.on_status((title, status))

    def scan_fs_uae_files(self, database):
        configurations = database.get_files(ext=".fs-uae")
        for c in configurations:
            if self.stop_check():
                break
            #name = os.path.basename(c["path"])
            #name = c["name"]
            #name = name[:-7]
            #search = name.lower()
            path = c["path"]
            name, ext = os.path.splitext(c["name"])
            search = self.create_configuration_search(name)
            name = self.create_configuration_name(name)
            database.add_configuration(path=path, uuid="", name=name,
                    scan=self.scan_version, search=search)

    def scan_builtin_configs(self, database):
        from .builtin_configs import builtin_configs
        for name, data in builtin_configs():
            if self.stop_check():
                break
            search = self.create_configuration_search(name)
            name = self.create_configuration_name(name)
            database.add_configuration(data=data, name=name,
                    scan=self.scan_version, search=search)

    def scan(self, database, game_database):
        self.set_status(_("Scanning configurations"),
                _("Please wait..."))

        self.set_status(_("Scanning configurations"),
                _("Scanning .fs-uae files..."))
        self.scan_fs_uae_files(database)

        if Settings.get("database_feature") == "1":
            self.set_status(_("Scanning configurations"),
                    _("Scanning game database entries..."))
            self.scan_game_database(database, game_database)
        else:
            self.set_status(_("Scanning configurations"),
                    _("Scanning database entries..."))
            self.scan_configurations(database)

        if Settings.get("builtin_configs") == "0":
            print("builtin_configs was set to 0")
        else:
            self.set_status(_("Scanning configurations"),
                    _("Scanning built-in entries..."))
            self.scan_builtin_configs(database)

        if self.stop_check():
            # aborted
            #database.rollback()
            return

        #database.remove_unscanned_configurations(self.scan_version)
        print("remove unscanned games")
        self.set_status(_("Scanning configurations"), _("Purging old entries..."))
        database.remove_unscanned_games(self.scan_version)
        print("remove unscanned configurations")
        database.remove_unscanned_configurations(self.scan_version)
        self.set_status(_("Scanning configurations"), _("Committing data..."))
        database.commit()

    def scan_game_database(self, database, game_database):
        game_database_client = GameDatabaseClient(game_database)

        game_cursor = game_database.cursor()
        game_cursor.execute("SELECT uuid, game.game, variant, game.name, "
                "platform, value FROM game, value WHERE "
                "game.id = value.game AND status = 1 AND "
                "value.name = 'file_list'")
        for row in game_cursor:
            if self.stop_check():
                return

            uuid, game, variant, alt_name, platform, file_list_json = row
            #print (uuid, file_list)

            self.scan_count += 1
            self.set_status(
                    _("Scanning configurations ({count} scanned)").format(
                    count=self.scan_count), uuid)

            file_list = json.loads(file_list_json)
            all_found = True
            for file_item in file_list:
                if not self.check_if_file_exists(database, file_item):
                    all_found = False
                    break
            if not all_found:
                #print("not found", uuid)
                continue
            #print("found", uuid)
            if not game:
                game = alt_name.split("(", 1)[0]
            name = "{0} ({1}, {2})".format(game, platform, variant)
            search = self.create_configuration_search(name)
            name = self.create_configuration_name(name)
            database.add_configuration(path="", uuid=uuid,
                    name=name, scan=self.scan_version, search=search)

    def scan_configurations(self, database):
        for dir in self.paths:
            if self.stop_check():
                return
            self.scan_dir(database, dir)

    def scan_dir(self, database, dir):
        if not isinstance(dir, unicode):
            dir = dir.decode(sys.getfilesystemencoding())
        if not os.path.exists(dir):
            print("does not exist")
            return
        dir = get_real_case(dir)

        for name in os.listdir(dir):
            if self.stop_check():
                return
            if name in [".git"]:
                continue
            path = os.path.join(dir, name)
            if os.path.isdir(path):
                self.scan_dir(database, path)
                continue
            dummy, ext = os.path.splitext(path)
            ext = ext.lower()
            #if ext not in self.extensions:
            #    continue
            if ext != ".xml":
                continue

            self.scan_count += 1
            self.set_status(
                    _("Scanning configurations ({count} scanned)").format(
                    count=self.scan_count), name)

            print("scan", name)
            result = None
            try:
                tree = ElementTree()
                tree.parse(path)
                root = tree.getroot()
                if root.tag == "config":
                    result = self.scan_configuration(database, tree)
                elif root.tag == "game":
                    self.scan_game(database, tree, path)
            except Exception:
                traceback.print_exc()
            if result is not None:
                if "name" in result:
                    name = result["name"]
                else:
                    name, ext = os.path.splitext(name)
                print("found", name)
                search = self.create_configuration_search(name)
                #name = self.create_configuration_name_from_path(path)
                name = self.create_configuration_name(name)
                database.add_configuration(path=path, uuid=result["uuid"],
                        name=name, scan=self.scan_version, search=search)

    def check_if_file_exists(self, database, file_node):
        #print("check file", file_node)
        if isinstance(file_node, dict):
            sha1 = file_node["sha1"]
            #print(sha1)
            if database.find_file(sha1=sha1):
                return True
        if file_node.find("sha1") is not None:
            sha1 = file_node.find("sha1").text.strip()
            #print(sha1)
            if database.find_file(sha1=sha1):
                return True
        archive_node = file_node.find("archive")
        if archive_node is not None:
            sub_file_nodes = archive_node.findall("file")
            for sub_file_node in sub_file_nodes:
                sha1 = sub_file_node.find("sha1").text.strip()
                print("sub-sha1", sha1)
                if not database.find_file(sha1=sha1):
                    # file not found, so stop looking for other files
                    print("not found")
                    break
            else:
                # all files were found
                print("all found")
                return True
        name = file_node.find("name").text.strip()
        if database.find_file(name=name):
            return True
        return False

    def scan_configuration(self, database, tree):
        root = tree.getroot()
        file_nodes = root.findall("file")
        if len(file_nodes) == 0:
            print("no files in configuration")
            return
        for file_node in file_nodes:
            if not self.check_if_file_exists(database, file_node):
                return

        result = {}

        game_name = ""
        platform_name = ""
        #source_name = ""
        variant_name = ""

        name_node = root.find("name")
        if name_node is not None:
            variant_name = name_node.text.strip()
        #source_node = root.find("source")
        #if source_node is not None:
        #    source_name = source_node.text.strip()
        game_node = root.find("game")
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
        #if source_name:
        #    parts.append(source_name)
        if variant_name:
            parts.append(variant_name)
        if game_name and variant_name:
            result["name"] = u"{0} ({1})".format(game_name, u", ".join(parts))
        result["uuid"] = root.get("uuid", "")
        return result

    def scan_game(self, database, tree, path):
        #print("scan_game")
        root = tree.getroot()
        uuid = root.get("uuid")
        name = root.find("name").text.strip()
        search = self.create_configuration_search(name)
        database.add_game(uuid=uuid, path=path, name=name,
                scan=self.scan_version, search=search)

    @classmethod
    def create_configuration_search(cls, name):
        return name.lower()

    @classmethod
    def create_configuration_name(cls, name):
        if "(" in name:
            primary, secondary = name.split("(", 1)
            secondary = secondary.replace(", ", " \u00b7 ")
            #name = primary.rstrip() + u" \u2013 " + secondary.lstrip()
            name = primary.rstrip() + "\n" + secondary.lstrip()
            if name[-1] == ")":
                name = name[:-1]
            name = name.replace(") (", " \u00b7 ")
            name = name.replace(")(", " \u00b7 ")
        return name
