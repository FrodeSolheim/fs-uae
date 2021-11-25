# Floppy disk images

You cannot generally use Amiga floppies in a PC floppy drive, and FS-UAE does
not support using real floppy drives. Instead, you need a floppy image – a file
with a copy of the data contained on a floppy.

FS-UAE supports floppy images in multiple file formats. ADF, IPF, DMS are the
most common. ADZ (compressed ADF) and ADFs in zip files are a also supported.

When you use FS-UAE Launcher, you have a convenient GUI for insert files into
floppy drives. Also, there is a "Select Multiple Files" function where you can
select multiple files for a game, and the files will be inserted into all
available drives and also added to the floppy swapping list.

## Floppy disk swapping

To be able to change floppy disks during play, the floppy image files must be
added to the list of swappable floppies. When you use FS-UAE Launcher, this is
easy. In the “Floppies” tab, there is a GUI to manage the list of floppies. If
you don’t add anything to the floppy list, the floppies assigned to floppy
drives will automatically be added to the swap list.

When running FS-UAE, you change disks in the floppy drives from the on-screen
menu by pressing [Mod](modifier-key.md) or F12.

> **Note**: Even though FS-UAE supports up to four floppy drives, a game may
> not support using all drives. Some games supports only reading from the first
> floppy drive. In this case, you must use the floppy swapping feature to swap
> the disks in the first floppy drive.

> **Note**: When using multiple Amiga drives, you may need to also add
> additional memory to your virtual Amiga.

## Manual Configuration

If you use FS-UAE manually, you specify floppy images for drives in
configuration files (see the documentation for these options for more
information):

    floppy_drive_0 = Pinball Dreams (1992)(21st Century)(Disk 1 of 2)[cr FLT].adf
    floppy_drive_1 = Pinball Dreams (1992)(21st Century)(Disk 2 of 2)[cr FLT].adf

When a game has multiple floppy disks, there are two ways to set this up in
FS-UAE. One way is to use multiple floppy drives, and put one disk in each
drive like you would on a real Amiga with multiple drives:

    floppy_drive_0 = Pinball Dreams (1992)(21st Century)(Disk 1 of 2)[cr FLT].adf
    floppy_drive_1 = Pinball Dreams (1992)(21st Century)(Disk 2 of 2)[cr FLT].adf

You can emulate up to four floppy drives this way
(floppy_drive_0…floppy_drive_3).

When a game has more than 4 floppies, or the game supports just one floppy
drive and requires floppy-swapping, you can do this instead:

    floppy_drive_0 = Pinball Dreams (1992)(21st Century)(Disk 1 of 2)[cr FLT].adf
    floppy_image_0 = Pinball Dreams (1992)(21st Century)(Disk 1 of 2)[cr FLT].adf
    floppy_image_1 = Pinball Dreams (1992)(21st Century)(Disk 2 of 2)[cr FLT].adf

You are then able to swap floppies using the FS-UAE menu. In the above example,
Disk 1 is inserted in drive 0, and both Disk 1 and Disk 2 is added to the
floppy image list. You can add up to 20 floppies to the floppy image list using
the `floppy_image_*` options.

## Floppy overlays and writable floppy images

By default, changes to floppy drives are not written back to the original
files. The changes are instead saved in overlay floppy files. FS-UAE stores
these files by default in Documents/FS-UAE/Floppy Overlays.

**FIXME: "Documents/FS-UAE/Floppy Overlays" is no longer the correct** **FIXME:
update the documentation.**

If you want to, you can use the option
[writable_floppy_images](options/writable-floppy-images.md) to enable writing
data directly to the original files instead.
