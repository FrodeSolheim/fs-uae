Summary: UAE bsdsocket.library
Type: boolean
Default: 0
Example: 1
Tags: affectsemu, uaeboot
Desc:
    Enable emulation of bsdsocket.library. This will allow Amiga OS to open
    TCP connections (use web browsers, etc). The default is 0 (off).

    You cannot use this option if you want to run AmigaOS 4.x / PPC. For
    networking in this environment, please use the network_card option
    instead.
Code:
    if c.bsdsocket_library.explicit:
        value = c.bsdsocket_library.explicit
    else:
        value = "0"
