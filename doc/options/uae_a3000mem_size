Description: Size in megabytes of motherboard fast memory
Default:
Range: 0 - 65536
Type: integer

A3000 and A4000 models use 8MB of motherboard fast memory by default.

Implementation: UAE code refers to this as mbresmem_low_size.

Code:

    if c.uae_a3000mem_size.explicit:
        value = c.uae_a3000mem_size.explicit
    elif c.motherboard_ram != "":
        value = str(int(c.motherboard_ram) // 1024)
    # elif c.int_model == "A3000":
    #     value = "8"
    # elif c.int_model == "A4000":
    #     # FIXME: should PPC / OS4 models be excluded?
    #     value = "8"
    else:
        value = "0"
