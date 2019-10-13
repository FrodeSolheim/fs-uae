Code:
    t = c.uae_gfxcard_type
    if t == "":
        value = ""
    elif t == "PicassoII":
        value = "zorro-ii"
    elif t == "PicassoII+":
        value = "zorro-ii"
    elif t == "PicassoIV_Z2":
        value = "zorro-ii"
    elif t == "PicassoIV_Z3":
        value = "zorro-iii"
    elif t == "ZorroII":
        value = "zorro-ii"
    elif t == "ZorroIII":
        value = "zorro-iii"
    else:
        f.warning("Unrecognized graphics card: " + t)
        value = ""

    if value == "zorro-iii" and c.uae_cpu_24bit_addressing == "true":
        f.warning("Zorro III graphics card cannot be used with 24-bit CPU")