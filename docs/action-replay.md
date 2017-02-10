# Action Replay

TODO: Write something about Action Replay. Bonus points for including some
specific examples of how it can be used, or link to tutorials.

Use the [freezer_cartridge](options/freezer-cartridge.md) option to enable
one of:

* Action Replay Mk II v2.14
* Action Replay Mk III v3.17

This option can be visually configured in FS-UAE Launcher. In order to use
Action Replay, you need a ROM file. See further below for more information.

To activate the freezer cartridge in FS-UAE, you press <kbd>Mod+A</kbd>.

## Action Replay Mk ROMs

The following ROM files are recognized for Action Replay Mk II:

    Action Replay Mk II v2.14.rom
    SHA-1: 255d6df63a4eab0a838eb1a16a267b0959dff634

    Action Replay Mk II v2.14 (1991)(Datel Electronics)[m].rom
    SHA-1: 14b1f5a69efb6f4e2331970e6ca0f33c0f04ac91

The following ROM files are recognized for Action Replay Mk III:

    Action Replay Mk III v3.17.rom
    SHA-1: 5d4987c2e3ffea8b1b02e31430ef190f2db76542

    Action Replay Mk III v3.17 (1991)(Datel Electronics)[m].rom
    SHA-1: 0439d6ccc2a0e5c2e83fcf2389dc4d4a440a4c62

FS-UAE will find the ROM (regardless of name) if the file extension is .rom
and it is stored in the Kickstarts directory. For FS-UAE Launcher, the name
of the file does not matter, as long as it is indexed by the file scanner.
But the SHA-1 checksum of the file must match one of the supported ROMs.

## Related Options

* [freezer_cartridge](options/freezer-cartridge.md)
* [modifier_key](options/modifier-key.md)
* [uae_cart](options/uae-cart.md)
* [uae_cart_file](options/uae-cart-file.md)
