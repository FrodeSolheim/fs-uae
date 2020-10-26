# Hard drives

When it comes to using emulated hard drives in FS-UAE, you have several
choices:

* You can mount directories on your system as Amiga volumes ("directory hard
  drives").
* You can use hard drive images (HDF files).
* You can mount zip files as Amiga volumes.
* You can in some cases use a real Amiga hard drive / compact flash card.

The best choice depends on how you want to use it. The most convenient is
directory hard drives, and I recommend to use this unless you have a good
reason not to, especially with FS-UAE v2.1.28+, when support for file
permission and metadata was added.

## Using directory hard drives

The advantages of using directory hard drives compared with other approaches
are:

* It will work with all Amiga models, with no Amiga-side setup needed. The
  volumes (directories) will automatically be accessible from the Amiga.
* You’ll have “unlimited” Amiga storage capacity (as long as you have enough
  free space on your system), and you don’t have to allocate space upfront as
  you must do with HDFs.
* You can also access the files directly from the host system, and you can
  easily add new files to your Amiga drive from the host system.

> **Note:** You should be somewhat careful about modifying the directory hard
> drive from the Amiga and from the host system at the same time. Ideally, you
> should only modify the hard drive from the host system when FS-UAE is not
> running, since some information about the file system may be cached in the
> Amiga.

## Amiga and host file names

Some file names which are valid on the Amiga are not allowed on the host file
system. This is particularly true on Windows, which has one of the most
restrictive set of allowed file names (Linux and macOS are more flexible).

FS-UAE handles this by "escaping" the invalid Amiga file names so the can be
stored on the local file system. This is only done when necessary, so most file
names will not be escaped. Also, the escaping is done so the host file name is
valid on all supported FS-UAE platforms.

For example, the following file name is valid on the amiga: `Foo\Bar`. This is
not valid on Windows however, so the file name is escaped and stored as
`Foo%5cBar`. The Amiga will see a file called Foo\Bar when it accesses a
directory with a host file called `Foo%5cBar`. Since `Foo\Bar` is not allowed
on Windows, the file is stored as `Foo%5cBar` on *all* platforms. This makes
the directory hard drives portable across multiple operating systems.

See also "Extracting Amiga archives" for more relevant information.

## Amiga file permissions

When FS-UAE looks up existing files in a directory hard drives, it initializes
metadata about the file either from the host file system, or if it exists, from
an accompanying metadata file.

When a file has an accompanying metadata file, date/time, file permissions and
comment are read from the metadata file. If a metadata file does not exist,
then:

* The file will get the default permissions: `----rwed`.
* The file date/time will be read from the last modified timestamp of the host
  file.
* The file will have no file comment.

When files are created/modified from within the Amiga, the host file mtime
(last modified time) is updated based on the Amiga file date/time. FS-UAE then
decides if it needs to store more information about the file in a metadata
file:

* If the file has a file comment, FS-UAE will create a metadata file.
* If the file has non-default file permissions, FS-UAE will create a metadata
  file.
* FS-UAE reads back the last modified time from the host file, and checks that
  the time was stored with high enough precision (Amiga files are stored with
  1/50s precision). If the time could not be stored accurately, FS-UAE will
  create a metadata file.
* If the file already has a metadata file, FS-UAE will always update the
  metadata file.

Additional metadata is stored in files with .uaem extension. If the original
file is called Image.jpg, the additional metadata is stored in Image.jpg.uaem
in the same directory. The metadata is stored in a text format, and you can
even edit this file manually from the host side to alter the information about
the file. Here’s an example of the content of a metadata file (where "hello" is
a file comment):

    ----rwed 2013-02-12 21:20:52.02 hello

**FIXME** Format might be outdated, file comments start on the next line now?

## Extracting Amiga archives

If you have an .lha archive with Amiga software, you have two choices when you
want to extract it:

* Extract the .lha archive to the directory hard drive from the host system.
* Copy the .lha archive to the a directory hard drive or otherwise make it
  available to to the virtual Amiga. Start FS-UAE and extract te archive with
  the `lha` program (or similar) inside the Amiga.

In many cases, both approaches will work well, but there are some cases where
you should extract the archive from inside the emulated Amiga:

* If the archive contains Amiga file names which are illegal on the host file
  system, you’ll not be able to extract the archive correctly on the host
  system.
* The archive can contain file metadata (date/time, file permissions, comment),
  and if you extract the archive on the host system, information will be lost
  (the files will get the default file permissions `----rwed`).

So whether you can extract an archive without (significant) loss of information
on the host system depends on the archive. To be sure, you can always extract
archives from inside the Amiga.

## Using hard drive images (HDF files)

HDF files are "raw files" containing the content of hard drives, used directly
by the Amiga as storage devices (like it would use a real hard drive). This
means that the Amiga manages the file system.

There are two common variants of HDF files:

* A full HDF image of a hard drive, often referred to as HDF files in RDB
  (Rigid Disk Block) format. This HDF file must be partitioned, like a real
  hard drive, before that Amiga can use it.
* A HDF file which only contains a single partition. Behind the scenes, FS-UAE
  will fake a full HDF image with the content of the HDF file as the single
  partition in the drive.

## Using HDF files in RDB format

When you mount a .hdf file as a hard drive in FS-UAE, FS-UAE will automatically
recognize it as an RDB file when the HD is already partitioned (the file begins
with the `RDSK` header). If the file is completely empty, FS-UAE will think
that it is a partition HDF instead, but you can use the `hard_drive_x_type`
options to force FS-UAE to handle it as an RDB file.

> **Note**: When you use the ADF/HDF creator in FS-UAE Launcher to create a HDF
> file in RDB format, the file is created in a way that allows FS-UAE to
> autodetect it as an RDB file.

Empty hard drive images must be partitioned and formatted before use. Workbench
may come with a tool called “HDToolBox” which you can use to partition the
image. By default, the hard drive is accessed through a HD controller called
`uaehf`.device. You need to tell `HDToolBox` to use this device when you start
it, for example by running the command:

    DF0:Tools/HDToolBox uaehf.device

Alternatively, if you emulate an Amiga 600 or Amiga 1200, you can attach the
drives to an emulated IDE controller instead (see hard_drive_x_controller).
When you use this controller, you can only use hard drive images which are
less than 4 GB. The uaehf.device does not have this limitation, so using the
default uaehf.device is recommended unless you have a very good reason not to
use it. Additionally, `uaehf.device` is likely more efficient (faster) to use.

After you have partitioned the file, you may need to restart the Amiga. Then
you can proceed to format the partition(s).

## Using Partition HDF files

When you use an empty partition HDF file, you’ll not partition it – but you
still need to format it inside the Amiga before you can use it.

FS-UAE needs to know the geometry of the HDF file. Currently, FS-UAE assumes –
and only supports – the default geometry (sectors=32, surfaces=1, reserved=2,
block_size=512). There is no option to override this yet. This is compatible
with most HDF partition files < 512 MB.

## Mounting ZIP archives as hard drives

FS-UAE supports mounting ZIP files as hard drive. This works more or less like
a read-only directory hard drive.

> **Note:** There is currently no support for .uaem metadata files in ZIP
> archives.

When you mount ZIP files and start FS-UAE via FS-UAE Launcher, the ZIP file is
handled differently:

* The ZIP file is extracted to a temporary directory
* This temporary directory is mounted as a directory hard drive in FS-UAE
* When FS-UAE is done running, the Launcher scans the temporary directory for
  changes and saves the changes to the state folder, and when you run the same
  configuration later, the changed files are merged with the content from the
  zip file.

Via FS-UAE Launcher, ZIP files work more like a hard drive *template*, and you
get support for metadata files and get a writable file system. The ZIP file
itself will remain untouched.

## Using real hard drives

> **Note**: This is not an officially supported feature, but it is described
> here because it works on some platforms. This does not work on MS Windows.

On Unix-like systems, devices can be accessed like files. This means that you
can mount a block device (for example a real hard drive or a compact flash
(CF) card) as a hard drive, and FS-UAE will use it like it was a HDF file.

You need to make sure FS-UAE have read and write access to the device (you may
need to chmod it and/or change device ownership), and also that you use the
correct device -you don’t want to overwrite another device by accident.

People have been using this feature on Linux and macOS, but no guarantee is
given that it works. The most common use is to mount a CF card which is also
used as a hard drive on a real Amiga.
