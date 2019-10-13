
Code:
    if c.fpu.explicit:
        value = c.fpu.explicit
    elif c.amiga_model == "A3000" and not c.cpu.explicit:
        value = "68882"
    elif c.amiga_model == "A4000" and not c.cpu.explicit:
        value = "68882"
    elif c.cpu == "68000":
        value = "0"
    elif c.cpu == "68010":
        value = "0"
    elif c.cpu == "68EC020":
        value = "0"
    elif c.cpu == "68020":
        value = "0"
    elif c.cpu == "68EC030":
        value = "0"
    elif c.cpu == "68030":
        value = "0"
    elif c.cpu == "68EC040":
        value = "0"
    elif c.cpu == "68LC040":
        value = "0"
    elif c.cpu == "68040-NOMMU":
        value = "68040"
    elif c.cpu == "68040":
        value = "68040"
    elif c.cpu == "68EC060":
        value = "0"
    elif c.cpu == "68LC060":
        value = "0"
    elif c.cpu == "68060-NOMMU":
        value = "68060"
    elif c.cpu == "68060":
        value = "68060"
    else:
        f.fail("Unknown CPU")
