# Known Issues

## CyberStorm PPC ROM Handling

The Launcher will create a copy of the CyberStorm PPC ROM and make sure
this copy is stored (if modifier) in the saves folder the current
configuration.

However, if you use FS-UAE standalone, if will just access the ROM it finds,
and this ROM file could be modified by the emulation. This difference in
behavior is unfortunate.

## Flickering When Opening Menu on Windows

Opening the F12 menu / Using the modifier key causes FS-UAE to blink on
Windows due to ungrabbing input (might have to disable this feature
on Windows).

## Missing Warning When Using Internal Kickstart

When internal kickstart is being used and kickstart_file = internal
was not used, show GUI message "Using AROS kickstart replacement".

## Possibly Sticky Amiga Keys When Toggling the FS-UAE Menu

Amiga key sticks when opening/closing FS-UAE menu with modifier+F12:
https://github.com/FrodeSolheim/fs-uae/issues/77.

## Possible Sticka Amiga Keys When Alt-tabbing

Similar to the above issue, alt-tabbing while holding keys pressed might
cause "sticky" key presses in emulation.
