Summary: "Graphics Card"
Type: Choice
Default: none
Example: uaegfx
Since: 2.5.15

Use this option to enable a graphics card. This option is none by default,
in which case only chipset graphics (OCS/ECS/AGA) support is available.

The choices labelled with (Auto) auto-selects the Zorro III version if the
current Amiga model supports it, otherwise the Zorro II version is chosen.

Value: none ("None")

The following values enable a virtual graphics card for UAE:

Value: uaegfx (UAEGFX)
Value: uaegfx-z2 (UAEGFX Zorro II)
Value: uaegfx-z3 (UAEGFX Zorro III)

The following values enables emulation of real graphics cards:

Value: picasso-ii (Picasso II Zorro II)
Value: picasso-ii+ (Picasso II+ Zorro II)
Value: picasso-iv (Picasso IV)
Value: picasso-iv-z2 (Picasso IV Zorro II)
Value: picasso-iv-z3 (Picasso IV Zorro III)

The UAEGFX card is a virtual card optimized for UAE, with a driver
("Picasso 96") available for AmigaOS 3.x. The other "real" graphics cards are
especially useful when you want to run other operating system such as Linux
or AmigaOS 4.x.

When you enable a graphics card, it is given up to 4 MB memory by default
(Zorro II) or up to 16 MB (Zorro III) depending on the max amount supported
by the card. If you want to configure less memory, use the
[graphics_card_memory] option. Also use [graphics_card_memory] if you want
to configure more memory for the UAEGFX card.

This option replaces the older [uaegfx_card] option.

FIXME: The following cards have not been added yet:

// Value: Piccolo ("Piccolo (Auto)")
// Value: Piccolo_Z2 ("Piccolo (Zorro II)")
// Value: Piccolo_Z3 ("Piccolo (Zorro III)")
// Value: PiccoloSD64 ("Piccolo SD64 (Auto)")
// Value: PiccoloSD64_Z2 ("Piccolo SD64 (Zorro II)")
// Value: PiccoloSD64_Z3 ("Piccolo SD64 (Zorro III)")
// Value: Spectrum28/24 ("EGS-28/34 Spectrum (Auto)")
// Value: Spectrum28/24_Z2 ("EGS-28/34 Spectrum (Zorro II)")
// Value: Spectrum28/24_Z3 ("EGS-28/34 Spectrum (Zorro III)")

Code:

    if c.graphics_card.explicit:
        # FIXME: check supported
        value = c.graphics_card.explicit
    else:
        value = ""
