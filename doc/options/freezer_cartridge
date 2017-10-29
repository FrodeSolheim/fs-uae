Summary: "Freezer Cartridge"
Type: Choice
Default: 0
Example: hrtmon
Since: 2.7.10

Select a type of freezer cartridge to attach to the emulated Amiga.

Value: 0 ("None")
Value: hrtmon ("HRTMon v2.36 (Built-in)")
Value: action-replay-2 (Action Replay Mk II v2.14)
Value: action-replay-3 (Action Replay Mk III v3.17)

Except for the built-in HRTMon freezer, you need to have the corresponding
ROM file for the freezer in the `Kickstarts` folder (yes, it's not a
kickstart...). If you use FS-UAE Launcher, the ROMs can be located anywhere
as long as the files are indexed by the Launcher's file database.

The ROM file is identifed by checksum, and the name does not matter, but
it should have the file extension .rom to make sure it is found.

255d6df63a4eab0a838eb1a16a267b0959dff634 Action Replay Mk II v2.14.rom
14b1f5a69efb6f4e2331970e6ca0f33c0f04ac91 Action Replay Mk II v2.14 (Mod).rom
5d4987c2e3ffea8b1b02e31430ef190f2db76542 Action Replay Mk III v3.17.rom
0439d6ccc2a0e5c2e83fcf2389dc4d4a440a4c62 Action Replay Mk III v3.17 (Mod).rom

See https://fs-uae.net/docs/action-replay for more information.

Code:

    if c.freezer_cartridge.explicit:
        c.freezer_cartridge = c.freezer_cartridge.explicit
        # FIXME: Check valid values
    else:
        c.freezer_cartridge = "0"
