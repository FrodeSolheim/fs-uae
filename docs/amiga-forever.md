# Amiga Forever

You can purchase Amiga Forever Plus or Premium edition in order to get
licensed versions of Kickstart ROMs for all supported Amiga models.

There are two versions which contains all Kickstart ROMs:
* Plus: Gives you access to a downloadable .msi installer.
* Premium: Physical media, plus access to the plus edition .msi installer.

> **Note:** There is also a value edition, but this only provides kickstart
> ROM for Amiga 500 (and possibly a few other models depending on the
> Amiga Forever version). This is sufficient for most classic games though!

Please note that FS-UAE / Frode Solheim is not affiliated with
Amiga Forever, and I get no provisions from the sale of Amiga Forever.
I provide this information as a convenience to the users of FS-UAE.

## Using the Amiga Forever .msi installer (Plus edition)

If you are using Windows, you can simply install Amiga Forever. FS-UAE
should automatically find the Kickstart ROM files.

> **Note:** You may have to run the Amiga Forever Player at least once
> in order to create the (required) rom.key file.

On other systems (Linux, OS X), you need to get a copy of the ROM files
installed on your system in a location FS-UAE expects to find them.
Read on for alternative installation methods.

> **Note:** FS-UAE may be able to find the ROM files automatically if you
> have installed Amiga Forever via Wine.

## A note about Documents/FS-UAE

Whenever you see a reference to `Documents/FS-UAE`, this means the location
of the FS-UAE directory where user data is stored. If you are using the
portable version, you need to use the portable directory instead of
Documents/FS-UAE. Also, on some Linux distributions, the directory may
be ~/FS-UAE instead of ~/Documents/FS-UAE, depending on your setup.

## Installing using Amiga Forever .iso

If you have an .iso image of the Amiga Forever Plus edition, you can either:
* Mount this as a virtual DVD (depends on your system), or
* Burn the .iso image to a physical DVD.

And then proceed with the instructions in the next section.

## Installing using Amiga Forever on a DVD

Once you can access the files on the physical or virtual DVD, you need to:
1. Create `Documents/FS-UAE/AmigaForever`
2. Copy the entire contents of the DVD into `Documents/FS-UAE/AmigaForever`.

FS-UAE should automatically find the Kickstart ROM files from this directory.

<!--
### Extracting files with msiextract

> **Note:** This does not actually work, because it turns out that rom.key is
> missing from the extracted files.

These instructions assume that the FS-UAE base directory is Documents/FS-UAE,
and that you have `AmigaForever2016Plus.msi` in your downloads directory.
Run the following commands (with appropriate changes, if needed):

    cd ~/Documents/FS-UAE
    mkdir AmigaForever
    cd AmigaForever
    msiextract ~/Downloads/AmigaForever2016Plus.msi
    mv "Program Files"/Cloanto/"Amiga Forever" "Amiga Files"
    rm -Rf ".:Common"
    rm -Rf CommonAppData
    rm -Rf "Program Files"

The above commands are tested on Linux (Ubuntu 15.10) using Amiga Forever
2016 Plus edition.
-->

### Installing Amiga Forever using Wine

Amiga Forever 2016 Plus edition has been tested to install just fine
using Wine in Linux (Ubuntu 15.10). Install using the following command
(or something similar):

    wine msiexec /i ~/Downloads/AmigaForever2016Plus.msi

After installation, you need to copy the Amiga Forever into place.
These instructions assume that the FS-UAE base dir is Documents/FS-UAE,
and that you have performed the Amiga Forever installation using the default
Wine prefix:

    cd ~/Documents/FS-UAE
    mkdir AmigaForever
    cd AmigaForever
    cp -a ~/.wine/drive_c/users/Public/Documents/"Amiga Files" .

### Installing Amiga Forever on another computer

If you are using a non-Windows computer, and the above solutions does
not work for you, you can also install Amiga Forever in a virtual machine
with Windows installed, or borrow a Windows computer and perform the
installation there.

When you have installed Amiga Forever on a Windows computer, you have
several options:
* Start Amiga Forever and create a DVD .iso image (Tools -> Build Image...),
  and then follow the instructions for installing from a DVD.
* Start Amiga Forever and export to a directory (Tools -> Build Image...),
  exporting to an USB hard drive, for example.
* Copy the installed files to your computer using any appropriate method.

If you want to manually copy files, you will find the Amiga Forever files
in the "Shared Documents" directory. The exact location of this directory
may vary depending on Windows version, often one of:
* C:\Documents and Settings\All Users\Documents
* C:\Users\Public\Documents

Just transfer the entire `Amiga Files` directory to
`Documents/FS-UAE/Amiga Forever`, so you end up with
`Documents/FS-UAE/Amiga Forever/Amiga Files` after you are done copying.

## Value edition

**Warning:** The value edition does not contains Kickstart ROMs except for
version 1.3.

It is possible to extract an un-encrypted Kickstart ROM file from
Amiga Forever Value Edition. At the time of writing, a video tutorial for
doing this is available at https://www.youtube.com/watch?v=W2_edUd2KyI.

## Final notes

The most important use of Amiga Forever with FS-UAE is to provide the
Kickstart ROMs needed for accurate emulation.

If the above instructions do not work for you, it's useful to know that
you don't need to copy all the Amiga Forever files. You simply need
to find all the .rom files (amiga-os-130.rom, etc) - as well as rom.key -
from Amiga Forever Plus/Premium installation or install media, and copy these
to Documents/FS-UAE/Kickstarts.
