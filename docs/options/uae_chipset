Category: Hardware
Default:
Example: aga

Overrides the chipset type in the emulated Amiga. You normally don't need
to use this option since a suitable chipset is automatically chosen based
on Amiga model. Valid values are:

Values:
* ocs (OCS)
* ecs (ECS)
* ecs_agnus (ECS Agnus)
* ecs_denise (ECS Denise)
* aga (AGA)

Code:

Code:

    if c.uae_chipset.explicit:
        if int(c.int_chipmem_size) >= 1048576:
            value = "ecs_agnus"
        else:
            value = "ocs"
    elif c.amiga_model == "A500":
        if int(c.int_chipmem_size) >= 1048576:
            value = "ecs_agnus"
        else:
            value = "ocs"
    elif c.amiga_model == "A500/512K":
        value = "ocs"
    elif c.amiga_model == "A500+":
        value = "ecs"
    elif c.amiga_model == "A600":
        value = "ecs"
    elif c.amiga_model == "A1000":
        value = "ocs"
    elif c.amiga_model == "A1200":
        value = "aga"
    elif c.amiga_model == "A1200/3.0":
        value = "aga"
    elif c.amiga_model == "A1200/020":
        value = "aga"
    elif c.amiga_model == "A1200/1230":
        value = "aga"
    elif c.amiga_model == "A1200/1240":
        value = "aga"
    elif c.amiga_model == "A1200/1260":
        value = "aga"
    elif c.amiga_model == "A1200/PPC":
        value = "aga"
    elif c.amiga_model == "A3000":
        value = "ecs"
    elif c.amiga_model == "A4000":
        value = "aga"
    elif c.amiga_model == "A4000/040":
        value = "aga"
    elif c.amiga_model == "A4000/OS4":
        value = "aga"
    elif c.amiga_model == "A4000/PPC":
        value = "aga"
    elif c.amiga_model == "CD32":
        value = "aga"
    elif c.amiga_model == "CD32/FMV":
        value = "aga"
    elif c.amiga_model == "CDTV":
        value = "ecs_agnus"
    else:
        f.fail("Unknown amiga_model")
