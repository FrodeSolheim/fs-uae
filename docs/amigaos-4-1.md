# AmigaOS 4.1

## UAE boot ROM

UAE Boot ROM cannot be enabled when you want to use AmigaOS 4.1. The UAE boot
ROM is automatically enabled when you enable certain features, so make sure to
observe the following limitations:

- Chip RAM size must not be more than 2 MB.
- Directory hard drives must not be used. You can use .hdf files for hard drive
  support (and other formats simulating real hard drives).
- UAEGFX graphics card must not be enabled. You can use
  [Picasso IV](picasso-iv.md) instead. Picasso IV emulation is enabled
  automatically when using the A4000/OS4 option.
- UAE [bsdsocket.library](options/bsdsocket_library) must not be enabled.
  Instead, enable A2065 emulation for network support.
- Do not enable uaenet.device.
- Do not use Zorro III chip memory.

## CyberStorm PPC

IF you are using CyberStorm PPC - which is the default if you use the A4000/OS4
Amiga model in FS-UAE, then please make sure you have the requirements for
[CyberStorm PPC emulation](cyberstorm-ppc.md).

## See also

- [CyberStorm PPC](cyberstorm-ppc.md)

## Related options

- [amiga_model](options/amiga-model.md)
- [accelerator](options/accelerator.md)
- [cdrom_drive_0](options/cdrom-drive-0.md)
- [floppy_drive_0](options/floppy-drive-0.md)
- [hard_drive_0](options/hard-drive-0.md)
- [network_card](options/network-card.md)
