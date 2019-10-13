Category: Hardware
Default:
Description: Sound emulation
Example: interrupts

Allows you to choose whether sound is emulated or not. Valid values are:

Value: none (Disabled)
Value: interrupts (Emulated, No Output)
Value: exact (Enabled)

FIXME: check with WinUAE/Toni. In the WinUAE GUI there is only GUI selectors
for "none", "interrupts" and "exact" (or so it seems), but code in cfgfile.cpp
still automatically enables "normal" for many quickstart configurations...

EDIT: As far as I can see, the UAE code does not really distinguish between
2 (normal) and 3 (exact), except for one place (in uaelib.cpp):
if (!sound_available || currprefs.produce_sound == 2)

# Allows you to select the accuracy of the emulated sound. The default value
# depends on the accuracy option. Valid values are:
#
# Value: none (Disabled)
# Value: interrupts (No sound output)
# Value: normal (Normal)
# Value: exact (Exact)
