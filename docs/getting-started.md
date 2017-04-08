# Getting Started

This page will help you get started using FS-UAE. It is quite easy to get
FS-UAE up and running with your favorite games, as long as you have access
to [Kickstart ROM images](kickstarts.md) and floppy image files for your games.

First, you need to download and install FS-UAE! I recommend you download the
suite containing both FS-UAE and FS-UAE Launcher. FS-UAE Launcher is an
easy-to-use configuration frontend which makes it much easier to get started.
The alternative is to manually write config files yourself.

The next step is to install Kickstart ROMs for the Amiga models you wish
to emulate. FS-UAE Launcher has a kickstart import helper you can use to
copy kickstart ROMs to the correct directory. Information about how to
acquire Amiga Kickstart ROMs is [available here](kickstarts.md).

Finally, you should have some software you want to run, for instance games.
Amiga Forever comes with a selection of software and games, and there are
several web sites with legal Amiga downloads:

* Full versions of many games:
  [http://www.amigafuture.de/downloads.php?cat=6](http://www.amigafuture.de/downloads.php?cat=6)
* [Team 17 games](http://www.dream17.info/downloads.php?cat=1)
* [Gremlin games](http://gremlinworld.emuunlim.com/amiga.htm)
* [Some Factor 5 games](http://www.factor5.de/downloads.shtml)
* [List of downloadable games](https://openretro.org/reports/downloadable-games)

When you start FS-UAE Launcher, it should be quite straight-forward how to
select the floppy disk images and start the emulation :)

While you are running the emulation, press <kbd>F12</kbd> to open the menu.
From this menu, you can quit, change floppies, restart the emulation,
save states, and more. Depending on your keyboard and operating systems,
you may need to press more than just <kbd>F12</kbd> alone, for example
<kbd>Fn+F12</kbd> on some laptops, or even <kbd>Fn+Cmd+F12</kbd> on Mac
laptops.

## Starting FS-UAE for the First Time

The following information is useful if you want to run FS-UAE manually
without FS-UAE Launcher. If you are using FS-UAE Launcher, you do not need
the rest of these instructions.

When you start FS-UAE for the first time, an on-screen message
"No configuration file was found" will be shown. This is normal.

You can now enter the menu by pressing <kbd>F12</kbd> (or <kbd>Cmd+F12</kbd>,
or <kbd>Fn+12</kbd> or <kbd>Cmd+Fn+12</kbd> depending on your system setup),
navigate to the X button in the top right corner with the cursor keys,
and press Return to quit FS-UAE.

FS-UAE will now have created a directory FS-UAE under your `(My) Documents`
folders. The FS-UAE folder contains the following folders (among others):
`Kickstarts`, `Floppies`, and `Configurations`.

You should now copy your kickstart files into the Kickstarts folder. If you
have a Cloanto Amiga Forever CD-ROM, these files will be called
amiga-os-130.rom and similar. Copy all .rom files (and the rom.key file!!)
to the Kickstarts folder.

If you want to play a game from a floppy disk image (ADF), copy the file
(or files if a multi-disk game) to the Floppies folder.

> **Note:** It is not a requirement to copy the ADF files here, read the
> detailed documentation if you want to learn how to have the disk files
> stored wherever you like.

## Creating a Configuration File

The last thing you need to do is to create a configuration file for FS-UAE.
Download [Default.fs-uae](https://fs-uae.net/files/Default.fs-uae)
and save this file in the Configurations folder (right-click the link and 
choose "Save link as", or similar option in your browser).

If you now start FS-UAE, it should no longer complain that the configuration
file was not found. The Default configuration file you downloaded specified
Amiga 500 as the model, and if you copied the necessary kickstart file(s)
to the Kickstarts directory, you should now see the standard Kickstart 1.3
boot screen when you start FS-UAE.

Please quit FS-UAE again and I’ll show you how to boot your game.
Open the file Default.fs-uae which you saved to the Configurations folder
in a text editor, for instance Notepad on Windows, or gEdit on Linux.

The line

    floppy_drive_0 = Disk1.adf

should be changed to reflect the name of the floppy you want to boot.
If, for instance, your ADF file is named
`Lotus Turbo Challenge 2 (1991)(Gremlin)[cr CPY].adf`, then you should
change the line to read:

    floppy_drive_0 = Lotus Turbo Challenge 2 (1991)(Gremlin)[cr CPY].adf

## Booting the Game

When you now start FS-UAE, Lotus 2 (or the game you configured) should boot :)

If you had a recognized and supported gamepad or joystick controller
connected when you started FS-UAE, this will be used as the primary Amiga
joystick. The mouse will be connected to the second Amiga joystick port by
default. You can change the inserted controllers from the F12 menu.

If you do not have a joystick or gamepad connected, the cursor keys and
right ctrl/alt key on your keyboard will be used to emulate the primary
Amiga joystick.

## Configuring a Multi-Disk Game

When a game has multiple floppy disks, there are two ways to set this up in
FS-UAE. One way is to use multiple floppy drives, and put one disk in
each drive like you would on a real Amiga with multiple drives:

    floppy_drive_0 = Pinball Dreams (Disk 1 of 2)[cr FLT].adf
    floppy_drive_1 = Pinball Dreams (Disk 2 of 2)[cr FLT].adf

You can emulate up to four floppy drives this way
(floppy_drive_0...floppy_drive_3).

When a game has more than 4 floppies, or the game supports just one floppy
drive and requires floppy-swapping, you can do this instead:

    floppy_drive_0 = Pinball Dreams (Disk 1 of 2)[cr FLT].adf
    floppy_image_0 = Pinball Dreams (Disk 1 of 2)[cr FLT].ad
    floppy_image_1 = Pinball Dreams (Disk 2 of 2)[cr FLT].adf

You are then able to swap floppies using the FS-UAE (F12) menu. In the above
example, Disk 1 is inserted in drive 0, and both Disk 1 and Disk 2 is
added to the floppy image list. You can add up to 20 floppies to the floppy
image list using the floppy_image_* options.

## Troubleshooting

If you encounter any issue, please see if your problem is listed in the FAQ.
If not, you are welcome to post a question in the support forum. I monitor
this forum myself, and everyone else there is nice and will help you if they
can :)

## More Information and Configuration Options

Head over to the Documentation section on
[https://fs-uae.net/docs](https://fs-uae.net/docs) if you want to explore
the options and features FS-UAE can offer!
