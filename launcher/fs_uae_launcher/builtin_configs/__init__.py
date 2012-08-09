from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import xml.etree.ElementTree
from xml.etree.cElementTree import ElementTree, Element, SubElement, tostring

def builtin_configs():
    #result = []
    #for name in modules:
    #    m = __import__("fs_uae_launcher.builtin_configs." + name)
    #    for name, data in getattr(m).iteritems:
    #        result.append((name, data))
    
    for config_dict in configurations:
        yield create_xml_from_config_dict(config_dict)
    #return result

def create_xml_from_config_dict(config_dict):
    root = Element("config")
    root.set("uuid", config_dict["uuid"])
    game_node = SubElement(root, "game")
    game_node.set("uuid", config_dict["parent"])
    game_name_node = SubElement(game_node, "name")
    game_name_node.text = config_dict["game_name"]
    game_platform_node = SubElement(game_node, "platform")
    game_platform_node.text = config_dict["platform"]

    options_node = SubElement(root, "options")
    for key, value in config_dict.iteritems():
        if key in ["game_name", "platform", "config_name", "uuid", "parent"]:
            continue
        if key.startswith("file_"):
            continue
        option_node = SubElement(options_node, key)
        option_node.text = value

    for i in range(100):
        name = config_dict.get("file_{0}_name".format(i), "")
        if not name:
            break
        sha1 = config_dict.get("file_{0}_sha1".format(i), "")
        #url = config_dict.get("file_{0}_url".format(i), "")
        url = "http://fengestad.no/sha1/{0}/{1}".format(sha1[:2], sha1)
        file_node = SubElement(root, "file")
        file_name_node = SubElement(file_node, "name")
        file_name_node.text = name
        file_sha1_node = SubElement(file_node, "sha1")
        file_sha1_node.text = sha1
        file_url_node = SubElement(file_node, "url")
        file_url_node.text = url
    
    name = u"{0} ({1}, {2})".format(config_dict["game_name"],
            config_dict["platform"], config_dict["config_name"])
    return name, tostring(root)

configurations = [

# http://aminet.net/game/2play/BlitzBombers.readme
#
# Short:        It's BLITZBOMBERS. And it's FREE.
# Author:       redwhen@ldngedge.demon.co.uk (Red When Excited)
# Uploader:     Big Will Riker ldngedge demon co uk (Steve Matty)
# Type:         game/2play
# Architecture: m68k-amigaos
#
# It's FINALLY here. The game some, or even a LOT people have
# been craving for. It's FREE as well.
#
# It's a snapshot of the game, as it was, back in January 1996 -
# when it was 98% complete. There are some quirks, but hey.
#
# Download and enjoy. Major bugs will probably get fixed.
#
# It requires an AGA Amiga, needs at LEAST 1.5Mb ChipRAM to run.
# FastRAM and/or accelerator will help when using lots of CPU
# players, or the serial link.
#
# http://www.ldngedge.demon.co.uk   is the place for FAQ's,
# patches etc.
#
# Have fun.

{
"uuid": "bf1b7659-bb30-44b0-83dd-bd2c428a4214",
"parent": "c1bcb261-adad-5a51-8f25-c5575798e30b",
"game_name": "Cybernetix: The First Battle",
"platform": "Amiga",
"config_name": "Built-in, Shareware",
"kickstart": "AROS",
"file_0_name": "Cybernetix - The First Battle (1991)(Vision)(SW)[h BTTR].adf",
"file_0_sha1": "bb433ea476dca6db52c1a9851c312307dda63010",
},

# Does not work with builtin AROS kickstart (freezes at title screen)
# "Downfall (Amiga, Freeware)":
# """<config uuid="2e1998fb-41e0-4f67-9f6a-e7cddfbadffb">
#   <name>Shareware</name>
#   <game uuid="1b6a5c73-eaad-5601-9875-0ad3498a220a">
#     <name>Downfall</name>
#     <platform>Amiga</platform>
#   </game>
#   <options>
#   <kickstart>AROS</kickstart>
#   </options>
#   <file>
#     <name>Downfall.adf</name>
#     <sha1>cc9ea0e8d1ae139ea5313d9b22eccc2e965744e2</sha1>
#     <url>http://fengestad.no/sha1/cc/cc9ea0e8d1ae139ea5313d9b22eccc2e965744e2</url>
#   </file>
# </config>
# """

]