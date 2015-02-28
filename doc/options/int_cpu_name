
Code:
    if f.matches(c.uae_cpu_model, "68000"):
        value = "68000"
    elif f.matches(c.uae_cpu_model, "68010"):
        value = "68010"
    elif f.matches(c.uae_cpu_model, "68020"):
        if f.matches(c.uae_cpu_24bit_addressing, "true"):
            value = "68EC020"
        else:
            value = "68020"
    elif f.matches(c.uae_cpu_model, "68030"):
        if f.matches(c.uae_mmu_model, "68030"):
            value = "68030"
        else:
            value = "68EC030"
    elif f.matches(c.uae_cpu_model, "68040"):
        if f.matches(c.uae_fpu_model, "68040"):
            value = "68040"
        elif f.matches(c.uae_mmu_model, "68040"):
            value = "68LC040"
        else:
            value = "68EC040"
    elif f.matches(c.uae_cpu_model, "68060"):
        if f.matches(c.uae_fpu_model, "68060"):
            value = "68060"
        elif f.matches(c.uae_mmu_model, "68060"):
            value = "68LC060"
        else:
            value = "68EC060"
    else:
        f.fail("Unknown CPU")
