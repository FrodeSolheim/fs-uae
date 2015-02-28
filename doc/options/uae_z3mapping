Description: JIT Direct compatible Z3 memory mapping
Default: auto
Type: choice

* auto
* uae
* real

Notes:

FIXME: notes are old (related to uae_z3realmapping)

if p->jit_direct_compatible_memory || cpuboard_blizzardram(p):
    expamem_z3hack = true

		if (expamem_z3_sum < 0x10000000) {
			expamem_z3_sum = currprefs.z3autoconfig_start;
			if (currprefs.mbresmem_high_size == 128 * 1024 * 1024)
				expamem_z3_sum += 16 * 1024 * 1024;
			if (!expamem_z3hack(&currprefs))
				expamem_z3_sum = 0x40000000;
			if (expamem_z3_sum == 0x10000000) {
				expamem_z3_sum += currprefs.z3chipmem_size;
			}
		}

		expamem_z3_pointer = expamem_z3_sum;

- Implemented no-hack Z3 memory mapping, lets OS handle Z3 autoconfig without patching base addressess from >=0x40000000 to >=0x10000000. This mode is not JIT Direct compatible. Option in Memory panel. Do not enable unless you know what are you doing and you really need 100% matching real hardware Z3 address mapping.
- Use no-hack Z3 allocation automatically if all configured boards fit in official Z3 space, fully JIT Direct compatible. Requires 64-bit Windows and available space will be about 300M to 500M. RAM panel text string shows free space. 32-bit Chip RAM does not count, it is not Z3 board and it is always located at 0x10000000.

FIXME: remove old references to uae_z3realmapping

See: [uae_z3mem_size], [uae_z3mem2_size], [uae_z3mem_start]
