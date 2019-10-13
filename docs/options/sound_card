Summary: "Sound Card"
Type: Choice
Default: 0
Example: toccata
Since: 2.5.25

Use this option to enable a sound card. The option is disabled by default,
in which case only chipset audio (OCS/ECS/AGA) support is available.

Value: 0 ("None")

The following values enables emulation of real sound cards:

Value: toccata (Toccata)

Code:
    if c.sound_card.explicit:
        # FIXME: check supported
        value = c.sound_card.explicit
        if f.matches(value, "toccata"):
            value = "toccata"
        else:
            f.warning(value + ": invalid value")
            value = "0"
    else:
        value = "0"

Test correct value:
    > sound_card = toccata
    sound_card = toccata

Test wrong case:
    > sound_card = Toccata
    sound_card = toccata

Test mis-spelled name:
    > sound_card = tocata
    sound_card = 0

Test default value:
    > amiga_model = A500
    sound_card = 0
