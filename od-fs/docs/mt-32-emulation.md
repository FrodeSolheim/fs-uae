# MT-32 emulation

FS-UAE includes internal emulation of MT-32 / CM-32L.

## ROM images

You will need to supply a PCM ROM and a control ROM for the synthesizer you want to emulate. The ROMs must normally be stored in:

- `Data/ROM/MT-32`

For compatibility with WinUAE, you can alternatively store them in:

- `Data/ROM/mt32-roms`
- `C:\mt32-rom-data` (on Windows only)

but please note that these directories will only be searched if `Data/ROM/MT-32` does not exist.

See the following sections for supported ROM images and what names are recognized. In addition to the listed names, they are also recognized will all uppercase letters, and also as in split ROM configuration with `_a.rom` and `_b.rom` extensions.

### MT-32 ROMs

Recognized PCM ROM names:

- `pcm_mt32.rom`
- `mt32_pcm.rom`
- `pcm_mt32_l.rom`
- `pcm_mt32_h.rom`

Recognized control ROM names:

- `mt32_control.rom`
- `ctrl_mt32.rom`
- `ctrl_mt32_1_07.rom`
- `ctrl_mt32_1_06.rom`
- `ctrl_mt32_1_05.rom`
- `ctrl_mt32_1_04.rom`
- `ctrl_mt32_bluer.rom`
- `ctrl_mt32_2_04.rom`
- `ctrl_mt32_2_07.rom`
- `ctrl_mt32_2_06.rom`
- `ctrl_mt32_2_03.rom`

### CM-32L ROMs

Recognized PCM ROM names:

- `pcm_cm32l.rom`
- `cm32l_pcm.rom`
- `pcm_cm32l_l.rom`
- `pcm_cm32l_h.rom`

Recognized control ROM names:

- `cm32l_control.rom`
- `ctrl_cm32l.rom`
- `ctrl_cm32ln_1_00.rom`
- `ctrl_cm32l_1_02.rom`
- `ctrl_cm32l_1_00.rom`
