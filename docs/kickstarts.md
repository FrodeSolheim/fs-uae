# Kickstarts

For the best emulation experience, you should have copies of the original
Kickstart ROMs for each of the Amiga model you want to use. Using the
built-in replacement Kickstart is possible, but it is less compatible with
Amiga software.

## Getting Kickstarts via Amiga Forever

You can purchase Amiga Forever Plus or Premium edition in order to get
licensed versions of Kickstart ROMs for all supported Amiga models.

There are two versions which contains all Kickstart ROMs:

* Plus: Gives you access to a downloadable .msi installer.
* Premium: Physical media, plus access to the plus edition .msi installer. 

There is also a value edition, but this only provides kickstart ROM for
Amiga 500 (and possibly a few other models depending on the Amiga Forever
version). This is sufficient for most classic games though!

Please note that FS-UAE / Frode Solheim is not affiliated with Amiga Forever,
and this project gets no provisions from the sale of Amiga Forever.
This information is only provided as a convenience to the users of FS-UAE.

## Extracting Kickstarts from Real Amiga Computers

By using a tool such as Transrom or Grabkick, you can extract the ROM from
your own Amiga and copy it over to your computer. Here are some guides:

* http://www.pcguru.plus.com/uae_transfer.html (web.archive.org)
* http://ale.emuunlim.com/guides/get-kick-rom.shtml
* https://help.ubuntu.com/community/EUAEAmigaEmulator

Please note that I don’t provide support for extracting / transferring
Amiga ROM files, but you can probably get help on the
[English Amiga Board](http://eab.abime.net/) if you are stuck.

## Using the Replacement ROM

FS-UAE will automatically use the replacement AROS ROM when:

* The preferred kickstart rom for the Amiga model in use was not found,
  and no kickstart file was explicitly specified, or
* When kickstart_file is specified as “internal”

FS-UAE Launcher behaves a bit differently and will refuse to start the
emulation if the preferred kickstart ROM is not found. You must change the
Kickstart option to "Internal" in order to use the replacement ROM.

## Preferred Kickstart ROMs

This sections lists the Kickstart ROM used by default by FS-UAE and
FS-UAE Launcher. When checksums are listed for Amiga Forever ROMs, it refers
to the checksum of the decrypted content, not the scrambled file. The name
of the ROMs do not matter, only the content. Amiga Forever and TOSEC names
are used for reference.

### Amiga 500

    Kickstart v1.3 r34.5 (1987)(Commodore)(A500-A1000-A2000-CDTV)[!].rom
    SHA-1: 891e9a547772fe0c6c19b610baf8bc4ea7fcb785

Amiga Forever does not come with the actual A500 rom, but the following
kickstart will be patched so it is identical to the one above (it’s only a
3 byte difference):

    amiga-os-130.rom
    SHA-1: c39bd9094d4e5f4e28c1411f3086950406062e87

The following ROM is an overdumped version of the preferred ROM, but FS-UAE
will recognize this and use it like it were the preferred one:

    Kickstart v1.3 r34.5 (1987)(Commodore)(A500-A1000-A2000-CDTV)[o].rom
    SHA-1: 90933936cce43ca9bc6bf375662c076b27e3c458

### Amiga 500+

    amiga-os-204.rom
    Kickstart v2.04 r37.175 (1991)(Commodore)(A500+)[!].rom
    SHA-1: c5839f5cb98a7a8947065c3ed2f14f5f42e334a1

### Amiga 600

    amiga-os-205.rom
    Kickstart v2.05 r37.350 (1992)(Commodore)(A600HD)[!].rom
    SHA-1: 02843c4253bbd29aba535b0aa3bd9a85034ecde4

### Amiga 1200

    amiga-os-310-a1200.rom
    Kickstart v3.1 r40.68 (1993)(Commodore)(A1200)[!].rom
    SHA-1: e21545723fe8374e91342617604f1b3d703094f1

### Amiga 3000

    amiga-os-310-a3000.rom
    Kickstart v3.1 r40.68 (1993)(Commodore)(A3000).rom
    SHA-1: f8e210d72b4c4853e0c9b85d223ba20e3d1b36ee

### Amiga 4000

    Kickstart v3.1 r40.68 (1993)(Commodore)(A4000).rom
    SHA-1: 5fe04842d04a489720f0f4bb0e46948199406f49

The following ROM will be patched to the one above on demand:

    amiga-os-310.rom
    Kickstart v3.1 r40.68 (1993)(Commodore)(A4000)[h Cloanto].rom
    SHA-1: c3c481160866e60d085e436a24db3617ff60b5f9

### Amiga 1000

    amiga-os-120.rom
    Kickstart v1.2 r33.180 (1986)(Commodore)(A500-A1000-A2000)[!].rom
    SHA-1: 11f9e62cf299f72184835b7b2a70a16333fc0d88

### Amiga CD32

    amiga-os-310-cd32.rom
    Kickstart v3.1 r40.60 (1993)(Commodore)(CD32).rom
    SHA-1: 3525be8887f79b5929e017b42380a79edfee542d

You also need the extended ROM for CD32:

    amiga-ext-310-cd32.rom
    CD32 Extended-ROM r40.60 (1993)(Commodore)(CD32).rom
    SHA-1: 5bef3d628ce59cc02a66e6e4ae0da48f60e78f7f

### Commodore CDTV

Commodore uses the same Kickstart ROM as Amiga 500. But in addition, you also
need the extended ROM for CDTV:

    amiga-ext-130-cdtv.rom
    CDTV Extended-ROM v1.0 (1991)(Commodore)(CDTV)[!].rom
    SHA-1: 7ba40ffa17e500ed9fed041f3424bd81d9c907be
