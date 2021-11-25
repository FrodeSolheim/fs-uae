# Picasso IV emulation

## ROM

In order to use the emulated Picasso IV board, you need a copy of the ROM with
SHA-1 checksum:

    cafafb916f16b9f3ec9b49aa4b40eb4eeceb5b5b

The ROM file can be downloaded from
[www.sophisticated-development.de](https://www.sophisticated-development.de/software/index.php?thisfile=.//P-IV&section=P-IV).

You want the file called `PIV_FlashImage74.zip` or something similar. If you
extract this file, you'll get PIV_FLAS.BIN which is the ROM file you need. Put
this file into `[FS-UAE](base-dir.md)/Kickstarts` and rename it to
`picasso_iv_flash.rom`.

Tip: When you are using FS-UAE Launcher, the name and location of this file is
less important. As long as it is indexed in Launcher's file database, the
Launcher will automatically tell FS-UAE to use this ROM file. It can even be
kept inside it's original zip file, and the Launcher will temporarily extract
in when necessary.
