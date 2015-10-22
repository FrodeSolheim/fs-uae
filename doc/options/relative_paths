Summary: Relative paths
Example: relative_paths = 0

This options controls whether relative paths are given priority or not.
Due to historical reasons (or a design flaw, if you will), a relative path
given to FS-UAE can resolve as both a traditional path relative to the
current directory, but also as a path relative to for example the hard
drives directory.

This causes a conflict if you've specified the name of a hard drive in the
"Hard Drives" directory, which also exists relative to the current directory,
and you want to use the former.

Setting this option to 0 allows you to give priority to the directory in
"Hard Drives".

This option got very relevant recently, since OS X now uses / as current
directory when you launch an .app. Since there is a "System" directory, this
will block the use of "System" as a reference to a directory in "Hard Drives",
unless you use this option.
