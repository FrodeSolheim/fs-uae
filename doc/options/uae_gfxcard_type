
Code:

    t = c.graphics_card
    if c.uae_gfxcard_type.explicit:
        value = c.uae_gfxcard_type.explicit
    elif t == "":
        value = ""
    elif t == "uaegfx":
        if c.uae_cpu_24bit_addressing == "true":
            value = "ZorroII"
        else:
            value = "ZorroIII"
    elif t == "uaegfx-z2":
        value = "ZorroII"
    elif t == "uaegfx-z3":
        value = "ZorroIII"
    elif t == "picasso-ii":
        value = "PicassoII"
    elif t == "picasso-ii+":
        value = "PicassoII+"
    elif t == "picasso-iv":
        if c.uae_cpu_24bit_addressing == "true":
            value = "PicassoIV_Z2"
        else:
            value = "PicassoIV_Z3"
    elif t == "picasso-iv-z2":
        value = "PicassoIV_Z2"
    elif t == "picasso-iv-z3":
        value = "PicassoIV_Z3"
    # FIXME: cards are missing here
    else:
        f.warning("Unrecognized graphics card: " + t)
        value = ""
