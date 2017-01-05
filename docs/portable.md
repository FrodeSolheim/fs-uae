# Portable Version

FS-UAE includes a portable mode where it can be completely self-contained
within a single directory structure.

## Upgrading

You can generally upgrade to a new version by simple extracting the new
version on top of the old one.

FIXME: Write about old files being left around, how to clean up, and/or
how to upgrade "cleanly".

## Mixed Host OS Support

You can combine the portable versions for all supported operating systems
and CPU architectures. Just extract every portable version (the ones you
want) on top of each other.

It is important that when you upgrade to a new version, that you upgrade
every portable version you are using at the same time, since some files are
shared.

## Temp Files

The portable version will still store temporary files in the system-provided
temp directory. This is the only exception where FS-UAE (Launcher) writes
files outside the portable directory (except also if you specifically use
floppy / HD media from outside the portable directory of course).
