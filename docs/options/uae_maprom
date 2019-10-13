

The "default" is to specify the maprom location as 0x0f000000. However,
it will move around a bit depending on other variables.

For 24-bit address space CPUs, or when 128 MB processor slot RAM is
configured, it will automatically be moved to 0x00e00000.

It may again be moved to 0x00a80000 when extended kickstarts ROM is in used (or
when a Cloanto ROM is mirrored at 0x00e00000).

If an accelerator board comes with it's own maprom support, it will take
precedence. However, the uae_maprom option is needed to activate the maprom
support for some accelerators.

Cyberstorm MKIII / PPC and accelerator board memory >= 1 MB:
* Automatic maprom at 0xfff00000

BlizzardPPC and accelerator board memory >= 1 MB:
* Automatic maprom at 0xfff00000

Other Blizzard accelerators:
* CPUBoard MAPROM (up to several mirrors) if uae_maprom option is set.

