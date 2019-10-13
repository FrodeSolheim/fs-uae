Summary: CD-ROM Drive Count
Type: Choice
Default:
Example: 1

Value: 0
Value: 1

Code:
    if c.cdrom_drive_count.explicit:
        value = c.cdrom_drive_count.explicit
    elif c.cdrom_drive_0:
        value = "1"
    elif c.cdrom_image_0:
        value = "1"
    elif c.int_model == "CD32":
        value = "1"
    elif c.int_model == "CDTV":
        value = "1"
    else:
        value = "0"
