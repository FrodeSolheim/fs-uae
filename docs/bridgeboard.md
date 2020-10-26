# Bridgeboard emulation

Options (not implemented yet):

- bridgeboard=a2386sx
- bridgeboard_ram=8192
- bridgeboard_vga=cl-gd5426
- bridgeboard_vga_rom=5420.vbi

A2386SX
VGA: CL-GD5426 or CL-GD5429

## VGA adapter

Notes:

- Needs VGA bios (for example 5420.vbi or vgabios-0.7a.cirrus.bin)
- BIOS setting should not matter but you must set both mono and color options
  in pcprefs to OFF.
- VGA mode should be active very soon after binddrivers has run, END+F9 press
  is required to switch screens, it does not autoswitch.
- With VGA enabled, both PCMono and PCColor screens are always blank (but
  remember to still have either one open if you want working keyboard or
  mouse input..)

## Example configuration

No offical options yet, but bridgeboard + VGA can be enabled with config
similar to this:

    uae_a2386_rom_file = A2386SX 1.00 391168-01.bin
    uae_a2386_rom_options = 8M,config=
    uae_flash_file = flash.nvr
    uae_gfxcard_type = VGA
    uae_gfxcard_size = 2
    uae_x86vga_rom_file = 5420.vbi
    uae_floppy2type = 5

You might also want to disable joystick emulation:

    joystick_port_1 = nothing
