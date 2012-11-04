from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

class Options:

    @staticmethod
    def get(name):
        return options[name]

N_ = lambda x: x

options = {

"builtin_configs": {
    "default": "1",
    "description": N_("Include built-in configurations"),
    "type": "boolean",
},

"fsaa": {
    "default": "0",
    "description": N_("Full-scene anti-aliasing (FSAA)"),
    "type": "choice",
    "values": [
        ("0", "Off"),
        ("2", "2x2"),
        ("4", "4x4"),
        ("8", "8x8"),
    ]
},

"fullscreen": {
    "default": "0",
    "description": N_("Start FS-UAE in fullscreen mode"),
    "type": "boolean",
},

"automatic_input_grab": {
    "default": "1",
    "description": N_("Grab mouse and keyboard input when clicking on FS-UAE window"),
    "type": "boolean",
},

"initial_input_grab": {
    "default": "1",
    "description": N_("Grab mouse and keyboard input when FS-UAE starts"),
    "type": "boolean",
},

"keep_aspect": {
    "default": "0",
    "description": N_("Scale with original aspect ratio (instead of stretching video)"),
    "type": "boolean",
},

"kickstart_setup": {
    "default": "1",
    "description": N_("Show kickstart setup page on startup when all ROMs are missing"),
    "type": "boolean",
},

"netplay_feature": {
    "default": "0",
    "description": N_("Enable experimental net play GUI (requires re-start)"),
    "type": "boolean",
},

"video_sync": {
    "default": "auto",
    "description": N_("Video synchronization"),
    "type": "choice",
    "values": [
        ("off", N_("Off")),
        ("auto", N_("Auto")),
        ("vblank", N_("Sync to Vblank")),
        ("full", N_("Force Full Vsync")),
    ]
},

"video_sync_method": {
    "default": "",
    "description": N_("Video synchronization method"),
    "type": "choice",
    "values": [
        ("swap", "Swap"),
        ("swap-finish", "Swap-Finish"),
        ("finish-swap-finish", "Finish-Swap-Finish"),
        ("finish-sleep-swap-finish", "Finish-Sleep-Swap-Finish"),
        ("sleep-swap-finish", "Sleep-Swap-Finish"),
        ("swap-fence", "Swap-Fence"),
        ("swap-sleep-fence", "Swap-Sleep-Fence"),
    ]
},

"zoom": {
    "default": "auto",
    "description": N_("Crop Amiga display (zoom)"),
    "type": "choice",
    "values": [
        ("auto", N_("Auto")),
        ("full", N_("Full Frame")),
        ("640x400", N_("640x400")),
        ("640x480", N_("640x480")),
        ("640x512", N_("640x512")),
        ("auto+border", N_("Auto + Border")),
        ("640x400+border", N_("640x400 + Border")),
        ("640x480+border", N_("640x480 + Border")),
        ("640x512+border", N_("640x512 + Border")),
    ]
},

#"update_check": {
#    "default": "1",
#    "description": N_("..."),
#    "type": "boolean",
#},

}
