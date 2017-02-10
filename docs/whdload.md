# WHDLoad

[WHDLoad](http://whdload.de/) is a sofware solution used to (primarily)
allow floppy-based games to be installed to an Amiga hard drive and run
from Workbench. Custom WHDLoad "slaves" are written for each game, which
patches the game to work on modern HD-based Amigas.

Since WHDLoad is a pure Amiga-side program, it will, like any other Amiga
software, run just fine in the emulated Amiga environment provided by FS-UAE.
See the official WHLoad documentation for information about how to use WHDLoad
in the classical sense.

This document describes how FS-UAE Launcher supports using WHDLoad to
make it easy and convient to launch individual WHDLoad-based games. These
convenience features are provided by **FS-UAE Launcher** (and Arcade), not
FS-UAE itself.

## WHDLoad Game Archives

A WHDLoad game archive is an archive (`.zip` or `.lha`) containing a single
WHDLoad-installed game.

Please note that the archives available from [whdload.de](http://whdload.de/)
are *installers*, not game archives. In order to create a game archive you
run to run the WHDLoad installer for a game, and then create a `.zip` or
`.lha` archive containing the destination directory where you installed the
game.

## Loading a WHDLoad Game Archive

In FS-UAE Launcher, if you insert a `.zip` or `.lha` file into the primary
hard drive slot, the Launcher will check if the archive is a WHDLoad game
archive. If it is, it will try to extract WHDLoad arguments from the archive
and automatically set some options suitable for running the game (A1200
model, 8 MB fast RAM).

## WHDLoad Variants in the Game Database

The online game database contains information about (most) available
WHDLoad games. In order for these to appear in FS-UAE Launcher, you need
to have the game files on your computer, and indexed by the file scanner
in the Launcher.

**Note:** The individual game files needed for a particular WHDLoad game
install do not have to be contained one and the same archive. Also, the name
of the archive(s) containing the files do not matter. The Launcher will find
each individual required file based on checksums, and create a temporary
HD suitable for running the game. Any custom configuration needed for the
game will be retrieved from the game database.

## Running a WHDLoad Game Archive Directly

You can run fs-uae-launcher with the path to a WHDLoad game archive.
The Launcher will then try to automatically create a config suited to run
the game, and start it.

2.9.4dev+: If you are using the online game database, FS-UAE Launcher will
try to match the archive with a variant from the game database, and use the
configuration from the database. If you want to prevent this, you can add the
`--no-auto-detect-game` parameter.

You can disable the progress dialog opened by FS-UAE Launcher by including
the `--no-gui` parameter.

FIXME: Write about launching games using game / variant UUIDs.

## Saving Games

* WHDLoad quit key
* State dir name
* Unsafe save states


## Related Options

* [whdload_quit_key](options/whdload-quit-key.md)
* [whdload_preload](options/whdload-preload.md)
* [whdload_splash_delay](options/whdload-splash-delay.md)
* [x_whdload_args](options/x_whdload-args.md)
* [x_whdload_version](options/x_whdload-version.md)
