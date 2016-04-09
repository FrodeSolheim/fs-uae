Summary: Motherboard RAM
Type: Choice
Default:
Example: 8192

Override the amount of Motherboard Fast RAM, specified in KB.

The default value depends on [amiga_model].

Value: 0 (0 MB)
Value: 1024 (1 MB)
Value: 2048 (2 MB)
Value: 4096 (4 MB)
Value: 8192 (8 MB)
Value: 16384 (16 MB)
Value: 32768 (32 MB)
Value: 65536 (64 MB)

Code:
    if c.motherboard_ram.explicit:
        value = c.motherboard_ram.explicit
    elif c.int_model == "A3000":
        value = "8192"
    elif c.int_model == "A4000":
        # FIXME: should PPC / OS4 models be excluded?
        value = "8192"
    else:
        value = "0"
