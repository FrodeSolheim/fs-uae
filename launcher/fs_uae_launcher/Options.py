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

"automatic_input_grab": {
    "default": "1",
    "description": N_("Grab mouse and keyboard input when clicking on FS-UAE window"),
    "type": "boolean",
},

"builtin_configs": {
    "default": "1",
    "description": N_("Include built-in configurations"),
    "type": "boolean",
},

"floppy_drive_volume": {
    "default": "20",
    "description": N_("Floppy drive volume"),
    "type": "integer",
    "min": 0,
    "max": 100,
},

"fsaa": {
    "default": "0",
    "description": N_("Full-scene anti-aliasing (FSAA)"),
    "type": "choice",
    "values": [
        ("0", N_("Off")),
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

"initial_input_grab": {
    "default": "",
    "description": N_("Grab mouse and keyboard input when FS-UAE starts"),
    "type": "boolean",
},

"irc_nick": {
    "default": "",
    "description": N_("IRC nickname"),
    "type": "string",
},

"irc_server": {
    "default": "irc.fengestad.no",
    "description": N_("Custom IRC server address"),
    "type": "string",
},

"keep_aspect": {
    "default": "0",
    "description": N_("Keep aspect ratio when scaling (do not stretch)"),
    "type": "boolean",
},

"kickstart_setup": {
    "default": "1",
    "description": N_("Show kickstart setup page on startup when all ROMs are missing"),
    "type": "boolean",
},

"low_latency_vsync": {
    "default": "0",
    "description": N_("Low latency vsync (when using full vsync)"),
    "type": "boolean",
},

"middle_click_ungrab": {
    "default": "1",
    "description": N_("Ungrab mouse and keyboard on middle mouse click"),
    "type": "boolean",
},

"mouse_speed": {
    "default": "100",
    "description": N_("Mouse speed (%)"),
    "type": "integer",
    "min": 1,
    "max": 500,
},

"netplay_feature": {
    "default": "0",
    "description": N_("Enable experimental net play GUI (requires restart)"),
    "type": "boolean",
},

"netplay_tag": {
    "default": "UNK",
    "description": N_("Net play tag (max 3 characters)"),
    "type": "string",
},

"rtg_scanlines": {
    "default": "0",
    "description": N_("Render scan lines in RTG graphics mode"),
    "type": "boolean",
},

"scanlines": {
    "default": "0",
    "description": N_("Render scan lines"),
    "type": "boolean",
},

"texture_filter": {
    "default": "linear",
    "description": N_("Texture filter"),
    "type": "choice",
    "values": [
        ("nearest", "GL_NEAREST"),
        ("linear", "GL_LINEAR"),
    ]
},

"texture_format": {
    "default": "",
    "description": N_("Video texture format (on the GPU)"),
    "type": "choice",
    "values": [
        ("rgb", "GL_RGB"),
        ("rgb8", "GL_RGB8"),
        ("rgba", "GL_RGBA"),
        ("rgba8", "GL_RGBA8"),
        ("rgb5", "GL_RGB5"),
        ("rgb5_1", "GL_RGB5_1"),
    ]
},

"video_format": {
    "default": "bgra",
    "description": N_("Video buffer format and color depth"),
    "type": "choice",
    "values": [
        ("bgra", N_("32-bit BGRA")),
        ("rgba", N_("32-bit RGBA")),
        ("rgb565", N_("16-bit")),
    ]
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
    "description": N_("Zoom Amiga display (crop)"),
    "type": "choice",
    "values": [
        ("auto", N_("Auto")),
        ("full", N_("Full Frame")),
        ("640x400", "640x400"),
        ("640x480", "640x480"),
        ("640x512", "640x512"),
        ("auto+border", N_("Auto + Border")),
        ("640x400+border", N_("640x400 + Border")),
        ("640x480+border", N_("640x480 + Border")),
        ("640x512+border", N_("640x512 + Border")),
    ]
},

}
