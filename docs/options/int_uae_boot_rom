Code:
    if c.uae_bsdsocket_emu == "true":
        value = "true"
    elif c.uae_sana2 == "true":
        value = "true"
    elif c.uae_gfxcard_type == "ZorroII" and c.uae_gfxcard_size != "0":
        value = "true"
    elif c.uae_gfxcard_type == "ZorroIII" and c.uae_gfxcard_size != "0":
        value = "true"
    elif c.uae_z3chipmem_size != "0":
        value = "true"
    else:
        value = "false"
    # FIXME: chipmem size > 2 MB

    # FIXME:
    # if (nr_directory_units (NULL))
    #     return b;
    # if (nr_directory_units (&currprefs))
    #     return b;
    # if (currprefs.uaeserial)
    #     return b;
    # if (currprefs.scsi == 1)
    #     return b;
    # if (currprefs.input_tablet > 0)
    #     return b;
    # if (currprefs.rtgmem_size && currprefs.rtgmem_type < GFXBOARD_HARDWARE)
    #     return b;
    # if (currprefs.win32_automount_removable)
    #     return b;
    # if (currprefs.chipmem_size > 2 * 1024 * 1024)
    #     return b;
