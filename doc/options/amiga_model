Default: A500
Example: A1200/020

Specify the Amiga model you want to emulate. The default is A500. Valid
values are:

* A500 - A standard Amiga 500 with 512 KB chip RAM and 512 KB slow RAM,
  defaulting to Kickstart 1.3.
* A500+ - A standard Amiga 500+ with 1 MB chip RAM, defaulting to
  Kickstart 2.04.
* A600 - A standard Amiga 600 with 1 MB chip RAM, defaulting to
  Kickstart 2.05.
* A1000 - Since 1.3.27. An Amiga 1000 with 512 KB chip RAM, defaulting to
  Kickstart 1.2.
* A1200 - A standard Amiga 1200 with 2 MB chip RAM, defaulting to
  Kickstart 3.1.
* A1200/020 - Amiga 1200 but with 68020 processor instead of 68ec020 - allows
  the use of Zorro III RAM
* A3000 - (Since 2.1.21) Amiga 3000 with 2 MB chip RAM, 8 MB fast RAM and a
  68030 processor running as fast as possible, defaulting to Kickstart 3.1.
* A4000/040 - Amiga 4000 with 2 MB chip RAM, 8 MB fast RAM  and a 68040
  processor running as fast as possible, defaulting to Kickstart 3.1.
* CD32 - A standard CD32 unit.
* CDTV - A standard Commodore CDTV unit.

The amiga_model option configures FS-UAE with common hardware settings. You
can override the amount of RAM (and other hardware options) with other options.

See also [accuracy], [ntsc_mode], [chip_memory], [slow_memory], [fast_memory], [kickstart_file].

Code:

    if c.amiga_model.explicit:
        c.amiga_model = c.amiga_model.explicit
    else:
        c.amiga_model = "A500"

