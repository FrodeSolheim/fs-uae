from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import xml.etree.ElementTree
from xml.etree.cElementTree import ElementTree, Element, SubElement
from xml.etree.cElementTree import fromstring, tostring
import fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..I18N import _, ngettext

class XMLControl(fsui.TextArea):

    def __init__(self, parent):
        fsui.TextArea.__init__(self, parent, horizontal_scroll=True)
        self.path = ""

    def connect_game(self, info):
        tree = self.get_tree()
        root = tree.getroot()
        if not root.tag == "config":
            return
        game_node = self.find_or_create_node(root, "game")
        game_node.set("uuid", info["uuid"])
        game_name_node = self.find_or_create_node(game_node, "name")
        game_name_node.text = info["name"]
        self.set_tree(tree)

    def find_or_create_node(self, element, name):
        node = element.find(name)
        if node is None:
            node = SubElement(element, name)
        return node

    def set_path(self, path):
        if not os.path.exists(path):
            path = ""
        self.path = path
        if path:
            self.load_xml(path)
        else:
            self.set_text("")

    def get_tree(self):
        text = self.get_text().strip()
        try:
            root = fromstring(text.encode("UTF-8"))
        except Exception:
            # FIXME: show message
            import traceback
            traceback.print_exc()
            return
        tree = ElementTree(root)
        indent_tree(root)
        return tree

    def set_tree(self, tree):
        data = tostring(tree.getroot(), encoding="UTF-8").decode("UTF-8")
        std_decl = "<?xml version='1.0' encoding='UTF-8'?>"
        if data.startswith(std_decl):
            data = data[len(std_decl):].strip()
        self.set_text(data)

    def load_xml(self, path):
        with open(path, "rb") as f:
            data = f.read()
            self.set_text(data)

    def save(self):
        if not self.path:
            print("no path to save XML to")
            return
        self.save_xml(self.path)

    def save_xml(self, path):
        self.get_tree().write(self.path)

def indent_tree(elem, level=0):
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent_tree(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i

