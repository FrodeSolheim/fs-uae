# uae-sana2 #

Enables uaenet.device

Type: BooleanUAE,
Default: `false`,
Example:

    uae-sana2 = true

Code:

    if c.uae_sana2.explicit:
        # FIXME: ok? keep already specified value
        value = c.uae_rtc.explicit
        # FIXME: match and normalize uae boolean
    else:
        value = "false"
