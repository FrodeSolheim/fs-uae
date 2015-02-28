Example: path/to/kickstart.rom

The path to a specific kickstart file. The path can be either absolute, or
relative to the current working directory. The same is true for all paths in
the configuration file. Generally, you do not need to specify the path to
the kickstart directly, if you have a compatible kickstart file for the
Amiga model stored in the (My) Documents/FS-UAE/Kickstarts.

Also, kickstarts located in the standard kickstarts dir can be referenced
by name only, no directory component is needed.

Code:

    if c.kickstart_file.explicit:
        value = c.kickstart_file.explicit
    else:
        value = ""
