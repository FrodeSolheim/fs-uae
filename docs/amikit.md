# AmiKit


## Convert Windows installer to FS-UAE installer

Why create the installation from the Windows version of AmiKit? Because
that is the version most likely to be well-tested and supported.

Steps:

1. Install AmiKit for Windows (either on a real machine, a VM).
2. Do not run any post-installation options for AmiKit, we want the unpacked
   C:\AmiKit\AmiKit directory to be clean.
3. Create a new 1 GB AmiKitTransfer.hdf file (RDB mode) and create/format
   an 1 GB AmiKitTransfer: partition on in.
4. Set up a *new* WinUAE configuration with a Workbench 3.x boot floppy
   or boot HD, the C:\AmiKit\AmiKit directory as a directory hard drive, and
   AmiKitTransfer.hdf.
5. Start the new WinUAE configuration, booting to Workbench
6. Open a shell and execute: `copy AmiKit: AmiKitTransfer: all quiet clone`.
7. Shut down WinUAE after the copy operation is done.
8. Set up an FS-UAE config with a Workbench 3.x boot floppy, add a *new*
   *empty* directory hard drive called `AmiKit` (it is important that this
   is not C:\AmiKit\AmiKit, but a separate empty directory), and also add the
   AmiKitTransfer.hdf you created in step 3. Add the following option to
   custom configuration: `uaem_write_flags = nhsparwed`.
9. Boot the FS-UAE configuration, booting to Workbench
10. Open a shell and execute: `copy AmiKitTransfer: AmiKit: all quiet clone`.
11. Shut down FS-UAE after the copy operation is done.

The main idea of the above steps is to create a AmiKit directory hard drive
with correct metadata for a few files with special metadata and/or filenames.

You can now create a AmiKit.zip file with the new AmiKit directory you
created in step 8 if you want to keep the clean installer, or proceed to
use the AmiKit directory to install/run AmiKit.
