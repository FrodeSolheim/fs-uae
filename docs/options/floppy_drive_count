Summary: Floppy Drive Count
Type: Choice
Default:
Example: 2

Value: 0
Value: 1
Value: 2
Value: 3
Value: 4

Code:
    if c.floppy_drive_count.explicit:
        value = c.floppy_drive_count.explicit
    elif c.floppy_drive_3:
        value = "4"
    elif c.floppy_drive_2:
        value = "3"
    elif c.floppy_drive_1:
        value = "2"
    elif c.int_model == "A4000":
        value = "2"
    elif c.floppy_drive_0:
        value = "1"
    elif c.int_model == "CD32":
        value = "0"
    elif c.int_model == "CDTV":
        value = "0"
    elif c.platform == "amiga":
        value = "1"
    elif c.platform == "cd32":
        value = "1"
    elif c.platform == "cdtv":
        value = "1"
    elif c.platform == "":
        value = "1"
    else:
        value = "0"
