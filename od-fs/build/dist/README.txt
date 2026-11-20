# FS-UAE README

Note: This is an early preview release of FS-UAE 5 - so many features are still missing!

## Portable mode

In portable mode, FS-UAE keeps its files in a portable directory of your choice instead of in your
user directory. The portable directory is the directory with the Portable.ini file (from Extras):
FS-UAE looks for it in the directory with the FS-UAE program (on macOS, the one with FS-UAE.app),
and in the directories above it.

When starting FS-UAE in portable mode, you will get two additional directories inside the portable
directory: `Data` and `Cache`.

### Linux and Windows

The simplest way to create a portable directory is to just copy the Portable.ini file from Extras
into the parent directory (the directory with this README). Then everything should be set up
correctly.

But the system is somewhat flexible, and you can also follow these steps:

1. Create a new directory anywhere to function as the portable directory.
2. Move or copy the "Extras/Portable.ini" into the portable directory.
3. Copy or move the FS-UAE executable as well as the Libs, Python and Resources directories into the
   portable directory - or a sub-directory. You can for example store the FS-UAE files inside a
   `System`, `Program` or `FS-UAE` sub-directory.

### macOS

The disk image (.dmg) cannot be changed, so the portable directory must be somewhere else:

1. Create a new folder anywhere (for example on a USB drive) to function as the portable directory.
2. In Finder, drag FS-UAE.app from the disk image into the new folder (on its own).
3. Then copy "Extras/Portable.ini" from the disk image into the same folder, next to FS-UAE.app.

You can also put FS-UAE.app in a sub-directory of the portable directory.

### Creating a portable directory for multiple operating systems

You can also, if you like, combine FS-UAE for different operating systems / architectures to create
a portable version that works on many different types of computers (but don't mix version numbers).
You may have to rename the main FS-UAE executable in some cases to make this work to avoid name
clashes (e.g. `FS-UAE (x86-64)` and `FS-UAE (ARM64)`).

The libraries are already stored in directories with unique names per architecture, so you only have
to do this for the main executable.

On macOS, FS-UAE.app has the libraries inside it, but the arm64 and x86_64 versions have the same
name: put each in a sub-directory of its own (for example `macOS ARM64` and `macOS x86-64`) in the
portable directory.
