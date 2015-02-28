Description: Write .uaem metadata files
Default: 1
Example: 0
Since: 2.5.8
Type: flags

Controls when .uaem metadata files are created.

0 - disable (never create .uaem files)
1 - default (always create .uaem files)

n - when file notes are added
t - when file time cannot be stored exactly on the host system

h - when the hold bit is set
s - when the script bit is set
p - when the pure bit is set
a - when the archive bit is set

r - when the read bit is NOT set
w - when the write bit is NOT set
e - when the execute bit is NOT set
d - when the delete bit is NOT set

To only create .uaem metadata files when files notes are added:

  uaem_write_flags = n

To only create .uaem files when files notes are added or non-default
protection bits are set:

  uaem_write_flags = nhsparwed

Since file notes are required for the operating of some software, it is
recommended to always include the n flag.

Regardless of the use of this option, when specific .uaem files already
exists, they will always be updated. 

FIXME: can affect net play, will probably want to use as a global setting
and not a per-configuration one.
