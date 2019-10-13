Type: Choice

Value: 68000
Value: 68010
Value: 68020
Value: 68030
Value: 68040
Value: 68060

See: [uae_cpu_24bit_addressing], [uae_fpu_model], [uae_cpu_speed]

Code:
    if c.uae_cpu_model.explicit:
        value = c.uae_cpu_model.explicit
    elif c.cpu == "68000":
        value = "68000"
    elif c.cpu == "68010":
        value = "68010"
    elif c.cpu == "68EC020":
        value = "68020"
    elif c.cpu == "68020":
        value = "68020"
    elif c.cpu == "68EC030":
        value = "68030"
    elif c.cpu == "68030":
        value = "68030"
    elif c.cpu == "68EC040":
        value = "68040"
    elif c.cpu == "68LC040":
        value = "68040"
    elif c.cpu == "68040-NOMMU":
        value = "68040"
    elif c.cpu == "68040":
        value = "68040"
    elif c.cpu == "68EC060":
        value = "68060"
    elif c.cpu == "68LC060":
        value = "68060"
    elif c.cpu == "68060-NOMMU":
        value = "68060"
    elif c.cpu == "68060":
        value = "68060"
    else:
        f.fail("Unknown CPU")
