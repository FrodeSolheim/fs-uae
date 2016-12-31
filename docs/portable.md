# Portable Version

FS-UAE includes a portable mode where it can be completely self-contained
within a single directory structure.

## Mixed Host OS Support

You can combine the portable versions for all supported operating systems
and CPU architectures. Just extract every portable version (the ones you
want) on top of each other.

## Temp Files

The portable version will still store temporary files in the system-provided
temp directory. This is the only exception where FS-UAE (Launcher) writes
files outside the portable directory (except also if you specifically use
floppy / HD media from outside the portable directory of course).
