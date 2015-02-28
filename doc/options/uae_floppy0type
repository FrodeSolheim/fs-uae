Category: Hardware
Default:
Example: 1

Overrides the floppy drive type. Valid values are:

Value: -1 (No Drive)
Value: 0 (3.5" DD)
Value: 1 (3.5" HD)
Value: 2 (5.25" SD)
Value: 3 (3.5" DD Escom)


Code:

    if c.uae_floppy0type.explicit:
        value = c.uae_floppy0type.explicit
    elif int(c.floppy_drive_count) > 0:
        value = c.int_default_floppy_type
    else:
        value = "-1"
