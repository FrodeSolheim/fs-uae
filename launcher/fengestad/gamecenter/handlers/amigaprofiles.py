

class AmigaProfiles:

    amiga_500 = ["amiga-os-130", "34.005", "34.5"], """
sdl.use_gl=true
config_description=FS Game Center Configuration
config_hardware=false
config_host=false
config_version=
unix.cpu_idle=0
x86.use_tsc=yes
amd64.use_tsc=yes
sdl.map_raw_keys=false
alsa.device=default
alsa.verbose=false
use_debugger=false
flash_file=
cart_file=
kickshifter=false
parallel_on_demand=false
serial_on_demand=false
serial_hardware_ctsrts=true
serial_direct=false
scsi=false
scsi_device=

sound_output=exact
sound_bits=16
sound_channels=stereo
sound_stereo_separation=7
sound_stereo_mixing_delay=0
sound_frequency=44100
sound_interpol=sinc
sound_adjust=0
sound_volume=0
sound_latency=32

comp_trustbyte=direct
comp_trustword=direct
comp_trustlong=direct
comp_trustnaddr=direct
comp_nf=true
comp_constjump=true
comp_oldsegv=false
comp_flushmode=soft
compforcesettings=false
compfpu=true
cachesize=0

chipmem_size=1
bogomem_size=2
fastmem_size=0
a3000mem_size=0
z3mem_size=0
gfxcard_size=0

cpu_type=68000
cpu_model=68000
cpu_speed=real
cpu_24bit_addressing=false
cpu_multiplier=2
cpu_compatible=true
cpu_cycle_exact=true
cycle_exact=true
chipset=ocs
chipset_compatible=A500
blitter_cycle_exact=true
immediate_blits=false
collision_level=full

log_illegal_mem=false
catweasel_io=0x0
kbd_lang=us
bsdsocket_emu=false
synchronize_clock=no
maprom=0x0
ntsc=false
hide_cursor=true
keyboard_leds=numlock:none,capslock:none,scrolllock:none
"""

    amiga_600 = ["amiga-os-205", "37.350"], """
sdl.use_gl=true
config_description=FS Game Center Configuration
config_hardware=false
config_host=false
config_version=0.8.29
unix.cpu_idle=0
x86.use_tsc=yes
amd64.use_tsc=yes
sdl.map_raw_keys=false
alsa.device=default
alsa.verbose=false
use_debugger=false
flash_file=
cart_file=
kickshifter=false
parallel_on_demand=false
serial_on_demand=false
serial_hardware_ctsrts=true
serial_direct=false
scsi=false
scsi_device=

sound_output=exact
sound_bits=16
sound_channels=stereo
sound_stereo_separation=7
sound_stereo_mixing_delay=0
sound_frequency=44100
sound_interpol=sinc
sound_adjust=0
sound_volume=0
sound_latency=32

comp_trustbyte=direct
comp_trustword=direct
comp_trustlong=direct
comp_trustnaddr=direct
comp_nf=true
comp_constjump=true
comp_oldsegv=false
comp_flushmode=soft
compforcesettings=false
compfpu=true
cachesize=0

chipmem_size=2
bogomem_size=0
fastmem_size=0
a3000mem_size=0
z3mem_size=0
gfxcard_size=0

cpu_type=68000
cpu_model=68000
cpu_speed=real
cpu_24bit_addressing=false
cpu_multiplier=2
cpu_compatible=true
cpu_cycle_exact=true
cycle_exact=true
chipset=ecs
chipset_compatible=A600
blitter_cycle_exact=true
immediate_blits=false
collision_level=full

log_illegal_mem=false
catweasel_io=0x0
kbd_lang=us
bsdsocket_emu=false
synchronize_clock=no
maprom=0x0
ntsc=false
hide_cursor=true
keyboard_leds=numlock:none,capslock:none,scrolllock:none

"""

    amiga_1200 = ["amiga-os-310-a1200", "40.068", "40.68"], """
sdl.use_gl=true
config_description=FS Game Center Configuration
config_hardware=false
config_host=false
config_version=0.8.29
unix.cpu_idle=0
x86.use_tsc=yes
amd64.use_tsc=yes
sdl.map_raw_keys=false
alsa.device=default
alsa.verbose=false
use_debugger=false
flash_file=
cart_file=
kickshifter=false
parallel_on_demand=false
serial_on_demand=false
serial_hardware_ctsrts=true
serial_direct=false
scsi=false
scsi_device=

sound_output=exact
sound_bits=16
sound_channels=stereo
sound_stereo_separation=7
sound_stereo_mixing_delay=0
sound_frequency=44100
sound_interpol=sinc
sound_adjust=0
sound_volume=0
sound_latency=32

comp_trustbyte=direct
comp_trustword=direct
comp_trustlong=direct
comp_trustnaddr=direct
comp_nf=true
comp_constjump=true
comp_oldsegv=false
comp_flushmode=soft
compforcesettings=false
compfpu=true
cachesize=0

chipmem_size=4
bogomem_size=0
fastmem_size=8
a3000mem_size=0
z3mem_size=0
gfxcard_size=0

cpu_type=68ec020
cpu_model=68020
cpu_speed=real
cpu_24bit_addressing=true
cpu_multiplier=4
cpu_compatible=true
cpu_cycle_exact=true
cycle_exact=true
chipset=aga
chipset_compatible=A1200
blitter_cycle_exact=true
immediate_blits=false
collision_level=full

log_illegal_mem=false
catweasel_io=0x0
kbd_lang=us
bsdsocket_emu=false
synchronize_clock=no
maprom=0x0
ntsc=false
hide_cursor=true
keyboard_leds=numlock:none,capslock:none,scrolllock:none

"""
