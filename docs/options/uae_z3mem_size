Description: Size in MB of Zorro-III Fast RAM expansion board
Type: integer

See: [uae_z3mem2_size], [uae_z3mem_start], [uae_z3realmapping]

Code:

    if c.uae_z3mem_size.explicit:
        value = c.uae_z3mem_size.explicit
    elif c.zorro_iii_memory:
        value = str(int(c.zorro_iii_memory) // 1024)
    else:
        value = "0"
