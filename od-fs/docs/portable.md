# Portable mode

> FIXME: This text duplicates od-fs/docs/portable.md. Maybe copy/append text from this document automatically when creating distributions?

FS-UAE includes support for a portable mode where it can be completely self-contained within a single directory structure - including data files. You need to use the FS-UAE versions downloaded from [fs-uae.net](https://fs-uae.net), and not any Linux distribution-specific version.

The simplest way to create a portable directory is to just copy the `Portable.ini` files from the `Extras` directory into the parent directory. Then everything should be set up correctly. 

But the system is somewhat flexible, and you can also follow these steps:

1. Create a new directory anywhere to function as the portable directory.
2. Move or copy the `Extras/Portable.ini`  into the portable directory.
3. Copy or move the  FS-UAE executable as well as the `Libs`, `Python` and `Resources` directories into the portable directory - or a sub-directory. You can for example store them inside a `System`, `Program` or `FS-UAE` sub-directory.

When starting FS-UAE in portable mode, you will get two additional directories inside the
portable directory: `Data` and `Cache`.

### Creating a portable directory for multiple operating systems

You can also, if you like, combine FS-UAE for different operating systems / architectures to create a portable version that works on many different types of computers (but don't mix version numbers). You may have to rename the main FS-UAE executable in some cases to make this work to avoid name classes (e.g. `FS-UAE (x86-64)` and `FS-UAE (ARM64)`).

The libraries are already stored in directories with unique names per architecture, so you only have to do this for the main executable.
