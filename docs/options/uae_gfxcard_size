Code:

    t = c.uae_gfxcard_type
    if c.uae_gfxcard_size.explicit:
        value = c.uae_gfxcard_size.explicit
    elif t == "":
        value = "0"
    elif t == "PicassoII":
        value = "2"
    elif t == "PicassoII+":
        value = "2"
    elif t == "PicassoIV_Z2":
        value = "4"
    elif t == "PicassoIV_Z3":
        value = "4"
    elif t == "ZorroII":
        value = "4"
    elif t == "ZorroIII":
        value = "16"
    else:
        f.warning("Unrecognized graphics card: " + t)
        value = "0"