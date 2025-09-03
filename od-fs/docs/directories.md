# Directories used by FS-UAE


## Data

The `Data` directory - or one of its sub-directories - is where FS-UAE will store settings and other files, and where FS-UAE also expects you, the user, to add files such as Kickstart ROMs.

The data directory is (`*`):

- `C:\Users\<*username*>\AppData\Roaming\FS-UAE\Data` on Windows.
- `/home/<username>/.local/share/FS-UAE/Data` on Linux (`*`).
- `/Users/<username>/Library/Application Support/FS-UAE/` on macOS.
- `Data`  inside the portable directory if you are using FS-UAE in [portable mode](docs/portable.md).

Under the data directory you will typically find the following sub-directories.

- `Config` - FS-UAE stored configuration files here.
- `Plugins` - This is where FS-UAE expects to find Plugins.
- `ROM/Kickstart` - Put Amiga Kickstart ROMs in this directory.
- `ROM/MT-32` - If you want to use MT-32 emulation, you need to put the MT-32 ROMs in this directory.
- `ROM/accelerator`
- `ROM/harddrive`
- `ROM/x86`
- `Screenshots` - FS-UAE will store screenshots where when you use the builtin screenshot function.

### Alternative data directory locations

On Window, your roaming directory could be stored elsewhere - especially on older Windows versions.

On Linux, it could also be something else if you have overridden `XDG_DATA_HOME`. Also, while FS-UAE does not have official Snap or Flatpak packages - if you are running an unofficial FS-UAE version via snap or flatpak, the data directory might instead be somewhere under `$HOME/snap` or `$HOME/.var`. 

## Cache

The cache directory is:

- `C:\Users\<*username*>\AppData\Roaming\FS-UAE\Cache` on Windows.
- `/home/<username>/.local/share/FS-UAE/Cache on Linux (`*`).
- `/Users/<username>/Library/Application Support/FS-UAE/` on macOS.
- `Cache`  inside the portable directory if you are using FS-UAE in [portable mode](docs/portable.md).

The cache directory contains the follow sub-directories:

- `Log` - FS-UAE writes log files to this directory.
