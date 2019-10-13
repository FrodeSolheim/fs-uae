Summary: Toccata Z2 sound card emulation
Type: uaeyesno

Code:
    if c.uae_toccata.explicit:
        value = c.uae_toccata.explicit
        if f.matches(value, ["true", "yes", "1"]):
            value = "true"
        elif f.matches(value, ["false", "no", "0"]):
            value = "false"
        else:
            f.warning(value + ": invalid value")
            value = "false"
    elif c.sound_card == "toccata":
        value = "true"
    else:
        value = "false"
