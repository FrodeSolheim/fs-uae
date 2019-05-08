/*
* UAE - The Un*x Amiga Emulator
*
* Misc accelerator board special features
* Blizzard 1230 IV, 1240/1260, 2040/2060, PPC
* CyberStorm MK1, MK2, MK3, PPC.
* TekMagic
* Warp Engine
*
* Copyright 2014 Toni Wilen
*
*/

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "uae/memory.h"
#include "zfile.h"
#include "rommgr.h"
#include "autoconf.h"
#include "cpuboard.h"
#include "custom.h"
#include "newcpu.h"
#include "ncr_scsi.h"
#include "ncr9x_scsi.h"
#include "debug.h"
#include "flashrom.h"
#include "uae.h"
#include "uae/ppc.h"
#include "uae/vm.h"
#include "idecontrollers.h"
#include "scsi.h"
#include "cpummu030.h"

#ifdef FSUAE // NL
#include "uae/fs.h"
#endif

// ROM expansion board diagrom call
// 00F83B7C 3.1 A4000
// 00F83C96 3.1 A1200

#define MAPROM_DEBUG 0
#define PPC_IRQ_DEBUG 0
#define CPUBOARD_IO_LOG 0
#define CPUBOARD_IRQ_LOG 0

#define F0_WAITSTATES (2 * CYCLE_UNIT)

// CS MK3/PPC
#define CYBERSTORM_MAPROM_BASE 0xfff00000
#define CSIII_NCR			0xf40000
#define CSIII_BASE			0xf60000
#define CSIII_REG_RESET		0x00 // 0x00
#define CSIII_REG_IRQ		0x01 // 0x08
#define CSIII_REG_WAITSTATE	0x02 // 0x10
#define CSIII_REG_SHADOW	0x03 // 0x18
#define CSIII_REG_LOCK		0x04 // 0x20
#define CSIII_REG_INT		0x05 // 0x28
#define CSIII_REG_IPL_EMU	0x06 // 0x30
#define CSIII_REG_INT_LVL	0x07 // 0x38

// BPPC only
#define BPPC_MAPROM_ON		0x12
#define BPPC_MAPROM_OFF		0x13
#define BPPC_UNLOCK_FLASH	0x92
#define BPPC_LOCK_FLASH		0x93
#define BPPC_MAGIC_UNLOCK_VALUE 0x42

/* bit definitions */
#define	P5_SET_CLEAR		0x80

/* REQ_RESET 0x00 */
// M68K can only reset PPC and vice versa
// if P5_SELF_RESET is not active.
#define	P5_PPC_RESET		0x10
#define	P5_M68K_RESET		0x08
#define	P5_AMIGA_RESET		0x04
#define	P5_AUX_RESET		0x02
#define	P5_SCSI_RESET		0x01

/* REG_IRQ 0x08 */
#define P5_IRQ_SCSI			0x01
#define P5_IRQ_SCSI_EN		0x02
// 0x04
#define P5_IRQ_PPC_1		0x08
#define P5_IRQ_PPC_2		0x10
#define P5_IRQ_PPC_3		0x20 // MOS sets this bit
// 0x40 always cleared

/* REG_WAITSTATE 0x10 */
#define	P5_PPC_WRITE		0x08
#define	P5_PPC_READ			0x04
#define	P5_M68K_WRITE		0x02
#define	P5_M68K_READ		0x01

/* REG_SHADOW 0x18 */
#define	P5_SELF_RESET		0x40
// 0x20
// 0x10
// 0x08 always set
#define P5_FLASH			0x04 // Flash writable (CSMK3,CSPPC only)
// 0x02 (can be modified even when locked)
#define	P5_SHADOW			0x01 // KS MAP ROM (CSMK3,CSPPC only)

/* REG_LOCK 0x20. CSMK3,CSPPC only */
#define	P5_MAGIC1			0x60 // REG_SHADOW and flash write protection unlock sequence
#define	P5_MAGIC2			0x50
#define	P5_MAGIC3			0x30
#define	P5_MAGIC4			0x70
// when cleared, another CPU gets either stopped or can't access memory until set again.
#define P5_LOCK_CPU			0x01 

/* REG_INT 0x28 */
// 0x40 always set
// 0x20
// 0x10 always set
// 0x08
// 0x04 // MOS sets this bit
#define	P5_ENABLE_IPL		0x02
#define	P5_INT_MASTER		0x01 // 1=m68k gets interrupts, 0=ppc gets interrupts.

/* IPL_EMU 0x30 */
#define	P5_DISABLE_INT		0x40 // if set: all CPU interrupts disabled?
#define	P5_M68K_IPL2		0x20
#define	P5_M68K_IPL1		0x10
#define	P5_M68K_IPL0		0x08
#define P5_M68k_IPL_MASK	0x38
#define	P5_PPC_IPL2			0x04
#define	P5_PPC_IPL1			0x02
#define	P5_PPC_IPL0			0x01
#define P5_PPC_IPL_MASK		0x07

/* INT_LVL 0x38 */
#define	P5_LVL7				0x40
#define	P5_LVL6				0x20
#define	P5_LVL5				0x10
#define	P5_LVL4				0x08
#define	P5_LVL3				0x04
#define	P5_LVL2				0x02
#define	P5_LVL1				0x01

#define CS_RAM_BASE 0x08000000

#define BLIZZARDMK4_RAM_BASE_48 0x48000000
#define BLIZZARDMK4_MAPROM_BASE	0x4ff80000
#define BLIZZARDMK2_MAPROM_BASE	0x0ff80000
#define BLIZZARDMK3_MAPROM_BASE 0x1ef80000
#define BLIZZARD_MAPROM_ENABLE  0x80ffff00
#define BLIZZARD_BOARD_DISABLE  0x80fa0000
#define BLIZZARD_BOARD_DISABLE2 0x80f00000

#define CSMK2_2060_BOARD_DISABLE 0x83000000

static int cpuboard_size = -1, cpuboard2_size = -1;
static int configured;
static int blizzard_jit;
static int maprom_state;
static uae_u32 maprom_base;
static int delayed_rom_protect;
static int f0rom_size, earom_size;
static uae_u8 io_reg[64];
static void *flashrom, *flashrom2;
static struct zfile *flashrom_file;
static int flash_unlocked;
static int csmk2_flashaddressing;
static bool blizzardmaprom_bank_mapped, blizzardmaprom2_bank_mapped;
static bool cpuboard_non_byte_ea;
static uae_u16 a2630_io;

#ifdef WITH_PPC

static bool ppc_irq_pending;

static void set_ppc_interrupt(void)
{
	if (ppc_irq_pending)
		return;
#if PPC_IRQ_DEBUG
	write_log(_T("set_ppc_interrupt\n"));
#endif
	uae_ppc_interrupt(true);
	ppc_irq_pending = true;
}
static void clear_ppc_interrupt(void)
{
	if (!ppc_irq_pending)
		return;
#if PPC_IRQ_DEBUG
	write_log(_T("clear_ppc_interrupt\n"));
#endif
	uae_ppc_interrupt(false);
	ppc_irq_pending = false;
}

static void check_ppc_int_lvl(void)
{
	bool m68kint = (io_reg[CSIII_REG_INT] & P5_INT_MASTER) != 0;
	bool active = (io_reg[CSIII_REG_IPL_EMU] & P5_DISABLE_INT) == 0;
	bool iplemu = (io_reg[CSIII_REG_INT] & P5_ENABLE_IPL) == 0;

	if (m68kint && iplemu && active) {
		uae_u8 ppcipl = (~io_reg[CSIII_REG_IPL_EMU]) & P5_PPC_IPL_MASK;
		if (ppcipl < 7) {
			uae_u8 ilvl = (~io_reg[CSIII_REG_INT_LVL]) & 0x7f;
			if (ilvl) {
				for (int i = ppcipl; i < 7; i++) {
					if (ilvl & (1 << i)) {
						set_ppc_interrupt();
						return;
					}
				}
			}
		}
		clear_ppc_interrupt();
	}
}

bool ppc_interrupt(int new_m68k_ipl)
{
	bool m68kint = (io_reg[CSIII_REG_INT] & P5_INT_MASTER) != 0;
	bool active = (io_reg[CSIII_REG_IPL_EMU] & P5_DISABLE_INT) == 0;
	bool iplemu = (io_reg[CSIII_REG_INT] & P5_ENABLE_IPL) == 0;

	if (!active)
		return false;

	if (!m68kint && iplemu && active) {

		uae_u8 ppcipl = (~io_reg[CSIII_REG_IPL_EMU]) & P5_PPC_IPL_MASK;
		if (new_m68k_ipl < 0)
			new_m68k_ipl = 0;
		io_reg[CSIII_REG_IPL_EMU] &= ~P5_M68k_IPL_MASK;
		io_reg[CSIII_REG_IPL_EMU] |= (new_m68k_ipl << 3) ^ P5_M68k_IPL_MASK;
		if (new_m68k_ipl > ppcipl) {
			set_ppc_interrupt();
		} else {
			clear_ppc_interrupt();
		}
	}

	return m68kint;
}

#endif /* WITH_PPC */

static bool mapromconfigured(void)
{
	if (currprefs.maprom && !currprefs.cpuboard_type)
		return true;
	if (currprefs.cpuboard_settings & 1)
		return true;
	return false;
}

void cpuboard_set_flash_unlocked(bool unlocked)
{
	flash_unlocked = unlocked;
}

static bool is_blizzard1230mk2(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_BLIZZARD, BOARD_BLIZZARD_SUB_1230II);
}
static bool is_blizzard1230mk3(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_BLIZZARD, BOARD_BLIZZARD_SUB_1230III);
}
static bool is_blizzard(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_BLIZZARD, BOARD_BLIZZARD_SUB_1230IV) || ISCPUBOARDP(p, BOARD_BLIZZARD, BOARD_BLIZZARD_SUB_1260);
}
static bool is_blizzard2060(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_BLIZZARD, BOARD_BLIZZARD_SUB_2060);
}
static bool is_csmk1(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_CYBERSTORM, BOARD_CYBERSTORM_SUB_MK1);
}
static bool is_csmk2(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_CYBERSTORM, BOARD_CYBERSTORM_SUB_MK2);
}
static bool is_csmk3(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_CYBERSTORM, BOARD_CYBERSTORM_SUB_MK3) || ISCPUBOARDP(p, BOARD_CYBERSTORM, BOARD_CYBERSTORM_SUB_PPC);
}
static bool is_blizzardppc(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_BLIZZARD, BOARD_BLIZZARD_SUB_PPC);
}
static bool is_ppc(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_BLIZZARD, BOARD_BLIZZARD_SUB_PPC) || ISCPUBOARDP(p, BOARD_CYBERSTORM, BOARD_CYBERSTORM_SUB_PPC);
}
static bool is_tekmagic(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_GVP, BOARD_GVP_SUB_TEKMAGIC);
}
static bool is_a2630(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_COMMODORE, BOARD_COMMODORE_SUB_A26x0);
}
static bool is_dkb_12x0(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_DKB, BOARD_DKB_SUB_12x0);
}
static bool is_dkb_wildfire(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_DKB, BOARD_DKB_SUB_WILDFIRE);
}
static bool is_mtec_ematrix530(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_MTEC, BOARD_MTEC_SUB_EMATRIX530);
}
static bool is_fusionforty(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_RCS, BOARD_RCS_SUB_FUSIONFORTY);
}
static bool is_apollo(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_ACT, BOARD_ACT_SUB_APOLLO);
}
static bool is_kupke(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_KUPKE, 0);
}
static bool is_sx32pro(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_DCE, 0);
}
static bool is_ivsvector(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_IVS, BOARD_IVS_VECTOR);
}
static bool is_aca500(struct uae_prefs *p)
{
	return ISCPUBOARDP(p, BOARD_IC, BOARD_IC_ACA500);
}

extern addrbank cpuboardmem1_bank;
MEMORY_FUNCTIONS(cpuboardmem1);
addrbank cpuboardmem1_bank = {
	cpuboardmem1_lget, cpuboardmem1_wget, cpuboardmem1_bget,
	cpuboardmem1_lput, cpuboardmem1_wput, cpuboardmem1_bput,
	cpuboardmem1_xlate, cpuboardmem1_check, NULL, _T("*B"), _T("cpuboard ram"),
	cpuboardmem1_lget, cpuboardmem1_wget,
	ABFLAG_RAM | ABFLAG_THREADSAFE, 0, 0
};
extern addrbank cpuboardmem2_bank;
MEMORY_FUNCTIONS(cpuboardmem2);
addrbank cpuboardmem2_bank = {
	cpuboardmem2_lget, cpuboardmem2_wget, cpuboardmem2_bget,
	cpuboardmem2_lput, cpuboardmem2_wput, cpuboardmem2_bput,
	cpuboardmem2_xlate, cpuboardmem2_check, NULL, _T("*B"), _T("cpuboard ram #2"),
	cpuboardmem2_lget, cpuboardmem2_wget,
	ABFLAG_RAM | ABFLAG_THREADSAFE, 0, 0
};


DECLARE_MEMORY_FUNCTIONS(blizzardio);
static addrbank blizzardio_bank = {
	blizzardio_lget, blizzardio_wget, blizzardio_bget,
	blizzardio_lput, blizzardio_wput, blizzardio_bput,
	default_xlate, default_check, NULL, NULL, _T("CPUBoard IO"),
	blizzardio_wget, blizzardio_bget,
	ABFLAG_IO, S_READ, S_WRITE
};

DECLARE_MEMORY_FUNCTIONS(blizzardram);
static addrbank blizzardram_bank = {
	blizzardram_lget, blizzardram_wget, blizzardram_bget,
	blizzardram_lput, blizzardram_wput, blizzardram_bput,
	blizzardram_xlate, blizzardram_check, NULL, NULL, _T("CPUBoard RAM"),
	blizzardram_lget, blizzardram_wget,
	ABFLAG_RAM | ABFLAG_THREADSAFE, 0, 0
};

DECLARE_MEMORY_FUNCTIONS(blizzardea);
static addrbank blizzardea_bank = {
	blizzardea_lget, blizzardea_wget, blizzardea_bget,
	blizzardea_lput, blizzardea_wput, blizzardea_bput,
	blizzardea_xlate, blizzardea_check, NULL, _T("rom_ea"), _T("CPUBoard E9/EA Autoconfig"),
	blizzardea_lget, blizzardea_wget,
	ABFLAG_IO | ABFLAG_SAFE, S_READ, S_WRITE
};

DECLARE_MEMORY_FUNCTIONS(blizzarde8);
static addrbank blizzarde8_bank = {
	blizzarde8_lget, blizzarde8_wget, blizzarde8_bget,
	blizzarde8_lput, blizzarde8_wput, blizzarde8_bput,
	blizzarde8_xlate, blizzarde8_check, NULL, NULL, _T("CPUBoard E8 Autoconfig"),
	blizzarde8_lget, blizzarde8_wget,
	ABFLAG_IO | ABFLAG_SAFE, S_READ, S_WRITE
};

DECLARE_MEMORY_FUNCTIONS(blizzardf0);
static addrbank blizzardf0_bank = {
	blizzardf0_lget, blizzardf0_wget, blizzardf0_bget,
	blizzardf0_lput, blizzardf0_wput, blizzardf0_bput,
	blizzardf0_xlate, blizzardf0_check, NULL, _T("rom_f0_ppc"), _T("CPUBoard F00000"),
	blizzardf0_lget, blizzardf0_wget,
	ABFLAG_ROM, S_READ, S_WRITE
};

#if 0
DECLARE_MEMORY_FUNCTIONS(blizzardram_nojit);
static addrbank blizzardram_nojit_bank = {
	blizzardram_nojit_lget, blizzardram_nojit_wget, blizzardram_nojit_bget,
	blizzardram_nojit_lput, blizzardram_nojit_wput, blizzardram_nojit_bput,
	blizzardram_nojit_xlate, blizzardram_nojit_check, NULL, NULL, _T("CPUBoard RAM"),
	blizzardram_nojit_lget, blizzardram_nojit_wget,
	ABFLAG_RAM | ABFLAG_THREADSAFE, S_READ, S_WRITE
};
#endif

DECLARE_MEMORY_FUNCTIONS(blizzardmaprom);
static addrbank blizzardmaprom_bank = {
	blizzardmaprom_lget, blizzardmaprom_wget, blizzardmaprom_bget,
	blizzardmaprom_lput, blizzardmaprom_wput, blizzardmaprom_bput,
	blizzardmaprom_xlate, blizzardmaprom_check, NULL, _T("maprom"), _T("CPUBoard MAPROM"),
	blizzardmaprom_lget, blizzardmaprom_wget,
	ABFLAG_RAM, S_READ, S_WRITE
};
DECLARE_MEMORY_FUNCTIONS(blizzardmaprom2);
static addrbank blizzardmaprom2_bank = {
	blizzardmaprom2_lget, blizzardmaprom2_wget, blizzardmaprom2_bget,
	blizzardmaprom2_lput, blizzardmaprom2_wput, blizzardmaprom2_bput,
	blizzardmaprom2_xlate, blizzardmaprom2_check, NULL, _T("maprom2"), _T("CPUBoard MAPROM2"),
	blizzardmaprom2_lget, blizzardmaprom2_wget,
	ABFLAG_RAM, S_READ, S_WRITE
};

// hack to map F41000 SCSI SCRIPTS RAM to JIT friendly address
void cyberstorm_scsi_ram_put(uaecptr addr, uae_u32 v)
{
	addr &= 0xffff;
	addr += (CSIII_NCR & 0x7ffff);
	blizzardf0_bank.baseaddr[addr] = v;
}
uae_u32 cyberstorm_scsi_ram_get(uaecptr addr)
{
	uae_u32 v;
	addr &= 0xffff;
	addr += (CSIII_NCR & 0x7ffff);
	v = blizzardf0_bank.baseaddr[addr];
	return v;
}
uae_u8 *REGPARAM2 cyberstorm_scsi_ram_xlate(uaecptr addr)
{
	addr &= 0xffff;
	addr += (CSIII_NCR & 0x7ffff);
	return blizzardf0_bank.baseaddr + addr;
}
int REGPARAM2 cyberstorm_scsi_ram_check(uaecptr a, uae_u32 b)
{
	a &= 0xffff;
	return a >= 0x1000 && a + b < 0x3000;
}

MEMORY_FUNCTIONS(blizzardram);

#if 0
MEMORY_BGET(blizzardram_nojit);
MEMORY_WGET(blizzardram_nojit);
MEMORY_LGET(blizzardram_nojit);
MEMORY_CHECK(blizzardram_nojit);
MEMORY_XLATE(blizzardram_nojit);

static void REGPARAM2 blizzardram_nojit_lput(uaecptr addr, uae_u32 l)
{
	uae_u32 *m;

	addr &= blizzardram_nojit_bank.mask;
	if (maprom_state && addr >= maprom_base)
		return;
	m = (uae_u32 *)(blizzardram_nojit_bank.baseaddr + addr);
	do_put_mem_long(m, l);
}
static void REGPARAM2 blizzardram_nojit_wput(uaecptr addr, uae_u32 w)
{
	uae_u16 *m;

	addr &= blizzardram_nojit_bank.mask;
	if (maprom_state && addr >= maprom_base)
		return;
	m = (uae_u16 *)(blizzardram_nojit_bank.baseaddr + addr);
	do_put_mem_word(m, w);
}
static void REGPARAM2 blizzardram_nojit_bput(uaecptr addr, uae_u32 b)
{
	addr &= blizzardram_nojit_bank.mask;
	if (maprom_state && addr >= maprom_base)
		return;
	blizzardram_nojit_bank.baseaddr[addr] = b;
}
#endif

static void no_rom_protect(void)
{
	if (delayed_rom_protect)
		return;
	delayed_rom_protect = 10;
	protect_roms(false);
}

static uae_u8 *writeprotect_addr;
static int writeprotect_size;

static void maprom_unwriteprotect(void)
{
	if (!writeprotect_addr)
		return;
	if (currprefs.cachesize && !currprefs.comptrustlong)
		uae_vm_protect(writeprotect_addr, writeprotect_size, UAE_VM_READ_WRITE);
	writeprotect_addr = NULL;
}

static void maprom_writeprotect(uae_u8 *addr, int size)
{
	maprom_unwriteprotect();
	writeprotect_addr = addr;
	writeprotect_size = size;
	if (currprefs.cachesize && !currprefs.comptrustlong)
		uae_vm_protect(addr, size, UAE_VM_READ);
}

MEMORY_BGET(blizzardmaprom2);
MEMORY_WGET(blizzardmaprom2);
MEMORY_LGET(blizzardmaprom2);
MEMORY_CHECK(blizzardmaprom2);
MEMORY_XLATE(blizzardmaprom2);

static void REGPARAM2 blizzardmaprom2_lput(uaecptr addr, uae_u32 l)
{
}
static void REGPARAM2 blizzardmaprom2_wput(uaecptr addr, uae_u32 l)
{
}
static void REGPARAM2 blizzardmaprom2_bput(uaecptr addr, uae_u32 l)
{
}

MEMORY_BGET(blizzardmaprom);
MEMORY_WGET(blizzardmaprom);
MEMORY_LGET(blizzardmaprom);
MEMORY_CHECK(blizzardmaprom);
MEMORY_XLATE(blizzardmaprom);

static void REGPARAM2 blizzardmaprom_lput(uaecptr addr, uae_u32 l)
{
	uae_u32 *m;
	if (is_blizzard2060(&currprefs) && !maprom_state)
		return;
	addr &= blizzardmaprom_bank.mask;
	m = (uae_u32 *)(blizzardmaprom_bank.baseaddr + addr);
	do_put_mem_long(m, l);
#if MAPROM_DEBUG
	write_log(_T("LPUT %08x %08x %d %08x\n"), addr, l, maprom_state, M68K_GETPC);
#endif
	if (maprom_state > 0 && !(addr & 0x80000)) {
		no_rom_protect();
		m = (uae_u32 *)(kickmem_bank.baseaddr + addr);
		do_put_mem_long(m, l);
	}
}
static void REGPARAM2 blizzardmaprom_wput(uaecptr addr, uae_u32 w)
{
	uae_u16 *m;
	if (is_blizzard2060(&currprefs) && !maprom_state)
		return;
	addr &= blizzardmaprom_bank.mask;
	m = (uae_u16 *)(blizzardmaprom_bank.baseaddr + addr);
	do_put_mem_word(m, w);
#if MAPROM_DEBUG
	write_log(_T("WPUT %08x %08x %d\n"), addr, w, maprom_state);
#endif
	if (maprom_state > 0 && !(addr & 0x80000)) {
		no_rom_protect();
		m = (uae_u16 *)(kickmem_bank.baseaddr + addr);
		do_put_mem_word(m, w);
	}
}
static void REGPARAM2 blizzardmaprom_bput(uaecptr addr, uae_u32 b)
{
	if (is_blizzard2060(&currprefs) && !maprom_state)
		return;
	addr &= blizzardmaprom_bank.mask;
	blizzardmaprom_bank.baseaddr[addr] = b;
#if MAPROM_DEBUG
	write_log(_T("LPUT %08x %08x %d\n"), addr, b, maprom_state);
#endif
	if (maprom_state > 0 && !(addr & 0x80000)) {
		no_rom_protect();
		kickmem_bank.baseaddr[addr] = b;
	}
}

MEMORY_CHECK(blizzardea);
MEMORY_XLATE(blizzardea);

static void blizzardf0_slow(int size)
{
	if (is_blizzard(&currprefs) || is_blizzardppc(&currprefs) || is_blizzard2060(&currprefs)) {
		if (size == 4)
			regs.memory_waitstate_cycles += F0_WAITSTATES * 6;
		else if (size == 2)
			regs.memory_waitstate_cycles += F0_WAITSTATES * 3;
		else
			regs.memory_waitstate_cycles += F0_WAITSTATES * 1;
	}
}

static int REGPARAM2 blizzarde8_check(uaecptr addr, uae_u32 size)
{
	return 0;
}
static uae_u8 *REGPARAM2 blizzarde8_xlate(uaecptr addr)
{
	return NULL;
}

static uae_u32 REGPARAM2 blizzardf0_bget(uaecptr addr)
{
	uae_u8 v;

	blizzardf0_slow(1);

	if (is_csmk3(&currprefs) || is_blizzardppc(&currprefs)) {
		if (flash_unlocked) {
			return flash_read(flashrom, addr);
		}
	} else if (is_csmk2(&currprefs)) {
		addr &= 65535;
		addr += 65536;
		return flash_read(flashrom, addr);
	} else if (is_csmk1(&currprefs)) {
		addr &= 65535;
		addr += 65536;
		return flash_read(flashrom, addr);
	} else if (is_dkb_wildfire(&currprefs)) {
		if (flash_unlocked) {
			if (addr & 1)
				return flash_read(flashrom2, addr);
			else
				return flash_read(flashrom, addr);
		}
	}
	addr &= blizzardf0_bank.mask;
	v = blizzardf0_bank.baseaddr[addr];
	return v;
}
static uae_u32 REGPARAM2 blizzardf0_lget(uaecptr addr)
{
	uae_u32 *m;

	//write_log(_T("F0 LONGGET %08x\n"), addr);

	blizzardf0_slow(4);

	addr &= blizzardf0_bank.mask;
	m = (uae_u32 *)(blizzardf0_bank.baseaddr + addr);
	return do_get_mem_long(m);
}
static uae_u32 REGPARAM2 blizzardf0_wget(uaecptr addr)
{
	uae_u16 *m, v;

	blizzardf0_slow(2);
	if (is_dkb_wildfire(&currprefs) && flash_unlocked) {
		v = blizzardf0_bget(addr + 0) << 8;
		v |= blizzardf0_bget(addr + 1);
	} else {
		addr &= blizzardf0_bank.mask;
		m = (uae_u16 *)(blizzardf0_bank.baseaddr + addr);
		v = do_get_mem_word(m);
	}
	return v;
}

static void REGPARAM2 blizzardf0_bput(uaecptr addr, uae_u32 b)
{
	blizzardf0_slow(1);

	if (is_csmk3(&currprefs) || is_blizzardppc(&currprefs)) {
		if (flash_unlocked) {
			flash_write(flashrom, addr, b);
		}
	} else if (is_csmk2(&currprefs)) {
		addr &= 65535;
		addr += 65536;
		addr &= ~3;
		addr |= csmk2_flashaddressing;
		flash_write(flashrom, addr, b);
	} else if (is_csmk1(&currprefs)) {
		addr &= 65535;
		addr += 65536;
		flash_write(flashrom, addr, b);
	} else if (is_dkb_wildfire(&currprefs)) {
		if (flash_unlocked) {
			if (addr & 1)
				flash_write(flashrom2, addr, b);
			else
				flash_write(flashrom, addr, b);
		}
	}
}
static void REGPARAM2 blizzardf0_lput(uaecptr addr, uae_u32 b)
{
	blizzardf0_slow(4);
}
static void REGPARAM2 blizzardf0_wput(uaecptr addr, uae_u32 b)
{
	blizzardf0_slow(2);
	if (is_dkb_wildfire(&currprefs)) {
		blizzardf0_bput(addr + 0, b >> 8);
		blizzardf0_bput(addr + 1, b >> 0);
	}
}

MEMORY_CHECK(blizzardf0);
MEMORY_XLATE(blizzardf0);

static uae_u32 REGPARAM2 blizzardea_lget(uaecptr addr)
{
	uae_u32 v = 0;
	if (cpuboard_non_byte_ea) {
		v = blizzardea_bget(addr + 0) <<  24;
		v |= blizzardea_bget(addr + 1) << 16;
		v |= blizzardea_bget(addr + 2) <<  8;
		v |= blizzardea_bget(addr + 3) <<  0;
	}
	return v;
}
static uae_u32 REGPARAM2 blizzardea_wget(uaecptr addr)
{
	uae_u32 v = 0;
	if (cpuboard_non_byte_ea) {
		v  = blizzardea_bget(addr + 0) <<  8;
		v |= blizzardea_bget(addr + 1) <<  0;
	}
	return v;
}
static uae_u32 REGPARAM2 blizzardea_bget(uaecptr addr)
{
	uae_u8 v;

	addr &= blizzardea_bank.mask;
	if (is_tekmagic(&currprefs)) {
		cpuboard_non_byte_ea = true;
		v = cpuboard_ncr710_io_bget(addr);
	} else if (is_blizzard2060(&currprefs) && addr >= BLIZZARD_2060_SCSI_OFFSET) {
		v = cpuboard_ncr9x_scsi_get(addr);
	} else if (is_blizzard1230mk2(&currprefs) && addr >= 0x10000 && (currprefs.cpuboard_settings & 2)) {
		v = cpuboard_ncr9x_scsi_get(addr);
	} else if (is_blizzard(&currprefs)) {
		if (addr & BLIZZARD_SCSI_KIT4_SCSI_OFFSET)
			v = cpuboard_ncr9x_scsi_get(addr);
		else
			v = blizzardea_bank.baseaddr[addr];
	} else if (is_blizzard1230mk3(&currprefs)) {
		if (addr & BLIZZARD_SCSI_KIT3_SCSI_OFFSET)
			v = cpuboard_ncr9x_scsi_get(addr);
		else
			v = blizzardea_bank.baseaddr[addr];
	} else if (is_csmk1(&currprefs)) {
		if (addr >= CYBERSTORM_MK1_SCSI_OFFSET) {
			v = cpuboard_ncr9x_scsi_get(addr);
		} else if (flash_active(flashrom, addr)) {
			v = flash_read(flashrom, addr);
		} else {
			v = blizzardea_bank.baseaddr[addr];
		}
	} else if (is_csmk2(&currprefs)) {
		if (addr >= CYBERSTORM_MK2_SCSI_OFFSET) {
			v = cpuboard_ncr9x_scsi_get(addr);
		} else if (flash_active(flashrom, addr)) {
			v = flash_read(flashrom, addr);
		} else {
			v = blizzardea_bank.baseaddr[addr];
		}
	} else if (is_mtec_ematrix530(&currprefs)) {
		v = blizzardea_bank.baseaddr[addr];
		if ((addr & 0xf800) == 0xe800) {
			if ((addr & 3) < 2) {
				map_banks(&dummy_bank, 0x10000000 >> 16, 0x8000000 >> 16, 0);
				if (custmem1_bank.allocated_size) {
					map_banks(&custmem1_bank, (0x18000000 - custmem1_bank.allocated_size) >> 16, custmem1_bank.allocated_size >> 16, 0);
					if (custmem1_bank.allocated_size < 128 * 1024 * 1024) {
						map_banks(&custmem1_bank, (0x18000000 - 2 * custmem1_bank.allocated_size) >> 16, custmem1_bank.allocated_size >> 16, 0);
					}
				}
			}
			if ((addr & 3) >= 2) {
				map_banks(&dummy_bank, 0x18000000 >> 16, 0x8000000 >> 16, 0);
				if (custmem2_bank.allocated_size) {
					map_banks(&custmem2_bank, 0x18000000 >> 16, custmem2_bank.allocated_size >> 16, 0);
					if (custmem2_bank.allocated_size < 128 * 1024 * 1024) {
						map_banks(&custmem2_bank, (0x18000000 + custmem2_bank.allocated_size) >> 16, custmem2_bank.allocated_size >> 16, 0);
					}
				}
			}
		}
	} else {
		v = blizzardea_bank.baseaddr[addr];
	}
	return v;
}

static void REGPARAM2 blizzardea_lput(uaecptr addr, uae_u32 l)
{
	if (cpuboard_non_byte_ea) {
		blizzardea_bput(addr + 0, l >> 24);
		blizzardea_bput(addr + 1, l >> 16);
		blizzardea_bput(addr + 2, l >>  8);
		blizzardea_bput(addr + 3, l >>  0);
	}
}
static void REGPARAM2 blizzardea_wput(uaecptr addr, uae_u32 w)
{
	if (cpuboard_non_byte_ea) {
		blizzardea_bput(addr + 0, w >> 8);
		blizzardea_bput(addr + 1, w >> 0);
	}
}
static void REGPARAM2 blizzardea_bput(uaecptr addr, uae_u32 b)
{
	addr &= blizzardea_bank.mask;
	if (is_tekmagic(&currprefs)) {
		cpuboard_non_byte_ea = true;
		cpuboard_ncr710_io_bput(addr, b);
	} else if (is_blizzard1230mk2(&currprefs) && addr >= 0x10000 && (currprefs.cpuboard_settings & 2)) {
		cpuboard_ncr9x_scsi_put(addr, b);
	} else if (is_blizzard2060(&currprefs) && addr >= BLIZZARD_2060_SCSI_OFFSET) {
		cpuboard_ncr9x_scsi_put(addr, b);
	} else if ((is_blizzard(&currprefs)) && addr >= BLIZZARD_SCSI_KIT4_SCSI_OFFSET) {
		cpuboard_ncr9x_scsi_put(addr, b);
	} else if ((is_blizzard1230mk3(&currprefs)) && addr >= BLIZZARD_SCSI_KIT3_SCSI_OFFSET) {
		cpuboard_ncr9x_scsi_put(addr, b);
	} else if (is_csmk1(&currprefs)) {
		if (addr >= CYBERSTORM_MK1_SCSI_OFFSET) {
			cpuboard_ncr9x_scsi_put(addr, b);
		} else {
			flash_write(flashrom, addr, b);
		}
	} else if (is_csmk2(&currprefs)) {
		if (addr >= CYBERSTORM_MK2_SCSI_OFFSET) {
			cpuboard_ncr9x_scsi_put(addr, b);
		}  else {
			addr &= ~3;
			addr |= csmk2_flashaddressing;
			flash_write(flashrom, addr, b);
		}
	} else if (is_mtec_ematrix530(&currprefs)) {
		if ((addr & 0xf800) == 0xe800) {
			if ((addr & 3) < 2) {
				map_banks(&dummy_bank, 0x10000000 >> 16, 0x8000000 >> 16, 0);
				if (custmem1_bank.allocated_size)
					map_banks(&custmem1_bank, (0x18000000 - custmem1_bank.allocated_size) >> 16, custmem1_bank.allocated_size >> 16, 0);
			}
			if ((addr & 3) >= 2) {
				map_banks(&dummy_bank, 0x18000000 >> 16, 0x8000000 >> 16, 0);
				if (custmem2_bank.allocated_size)
					map_banks(&custmem2_bank, 0x18000000 >> 16, custmem2_bank.allocated_size >> 16, 0);
			}
		}
	}
}

static void REGPARAM2 blizzarde8_lput(uaecptr addr, uae_u32 b)
{
}
static void REGPARAM2 blizzarde8_wput(uaecptr addr, uae_u32 b)
{
}
static void REGPARAM2 blizzarde8_bput(uaecptr addr, uae_u32 b)
{
	b &= 0xff;
	addr &= 65535;
	if (addr == 0x48 && !configured) {
		uae_u32 size = map_banks_z2_autosize(&blizzardea_bank, b);
		write_log(_T("Accelerator Z2 board autoconfigured at %02X0000, size %08x\n"), b, size);
		configured = 1;
		expamem_next (&blizzardea_bank, NULL);
		return;
	}
	if (addr == 0x4c && !configured) {
		write_log(_T("Blizzard Z2 SHUT-UP!\n"));
		configured = 1;
		expamem_next (NULL, NULL);
		return;
	}
}
static uae_u32 REGPARAM2 blizzarde8_bget(uaecptr addr)
{
	uae_u32 v = 0xffff;
	v = blizzardea_bank.baseaddr[addr & blizzardea_bank.mask];
	return v;
}
static uae_u32 REGPARAM2 blizzarde8_wget(uaecptr addr)
{
	uae_u32 v = 0xffff;
	v = (blizzardea_bank.baseaddr[addr & blizzardea_bank.mask] << 8) | blizzardea_bank.baseaddr[(addr + 1) & blizzardea_bank.mask];
	return v;
}
static uae_u32 REGPARAM2 blizzarde8_lget(uaecptr addr)
{
	uae_u32 v = 0xffff;
	v = (blizzarde8_wget(addr) << 16) | blizzarde8_wget(addr + 2);
	return v;
}

static void blizzard_copymaprom(void)
{
	if (!maprom_state) {
		reload_roms();
	} else {
		uae_u8 *src = NULL;
		if (is_blizzardppc(&currprefs)) {
			src = blizzardram_bank.baseaddr + cpuboard_size - 524288;
		} else {
			src = blizzardmaprom_bank.baseaddr;
		}
		if (src) {
			uae_u8 *dst = kickmem_bank.baseaddr;
			protect_roms(false);
			memcpy(dst, src, 524288);
			protect_roms(true);
			set_roms_modified();
		}
		if (is_blizzard1230mk2(&currprefs) && cpuboard_size >= 64 * 1024 * 1024) {
			map_banks(&blizzardmaprom_bank, BLIZZARDMK2_MAPROM_BASE >> 16, 524288 >> 16, 0);
		}
		if (is_blizzard(&currprefs) && cpuboard_size >= 256 * 1024 * 1024) {
			map_banks(&blizzardmaprom_bank, BLIZZARDMK4_MAPROM_BASE >> 16, 524288 >> 16, 0);
		}
	}
}
static void cyberstorm_copymaprom(void)
{
	if (!maprom_state) {
		reload_roms();
	} else if (blizzardmaprom_bank.baseaddr) {
		uae_u8 *src = blizzardmaprom_bank.baseaddr;
		uae_u8 *dst = kickmem_bank.baseaddr;
#ifdef FSUAE
		write_log("cyberstorm_copymaprom src=%p dst=%p\n", src, dst);
#endif
		protect_roms(false);
		memcpy(dst, src, 524288);
		protect_roms(true);
		set_roms_modified();
	}
}
static void cyberstormmk2_copymaprom(void)
{
	if (blizzardmaprom_bank.baseaddr) {
		uae_u8 *src = a3000hmem_bank.baseaddr + a3000hmem_bank.allocated_size - 524288;
		uae_u8 *dst = kickmem_bank.baseaddr;
		protect_roms(false);
		memcpy(dst, src, 524288);
		protect_roms(true);
		set_roms_modified();
	}
}
static void cyberstormmk1_copymaprom(void)
{
	if (blizzardmaprom_bank.baseaddr) {
		uae_u8 *src = blizzardmaprom_bank.baseaddr;
		uae_u8 *dst = kickmem_bank.baseaddr;
		protect_roms(false);
		memcpy(dst, src, 524288);
		protect_roms(true);
		set_roms_modified();
	}
}

bool cpuboard_is_ppcboard_irq(void)
{
	if (is_csmk3(&currprefs) || is_blizzardppc(&currprefs)) {
		if (!(io_reg[CSIII_REG_IRQ] & (P5_IRQ_SCSI_EN | P5_IRQ_SCSI))) {
			return true;
		} else if (!(io_reg[CSIII_REG_IRQ] & (P5_IRQ_PPC_1 | P5_IRQ_PPC_2))) {
			return true;
		}
	}
	return false;
}

void cpuboard_rethink(void)
{
	if (is_csmk3(&currprefs) || is_blizzardppc(&currprefs)) {
		if (!(io_reg[CSIII_REG_IRQ] & (P5_IRQ_SCSI_EN | P5_IRQ_SCSI))) {
			INTREQ_0(0x8000 | 0x0008);
			if (currprefs.cachesize)
				atomic_or(&uae_int_requested, 0x010000);
			uae_ppc_wakeup_main();
		} else if (!(io_reg[CSIII_REG_IRQ] & (P5_IRQ_PPC_1 | P5_IRQ_PPC_2))) {
			INTREQ_0(0x8000 | 0x0008);
			if (currprefs.cachesize)
				atomic_or(&uae_int_requested, 0x010000);
			uae_ppc_wakeup_main();
		} else {
			atomic_and(&uae_int_requested, ~0x010000);
		}
#ifdef WITH_PPC
		check_ppc_int_lvl();
		ppc_interrupt(intlev());
#endif
	}
}

static void blizzardppc_maprom(void)
{
	if (cpuboard_size <= 2 * 524288)
		return;

	map_banks(&dummy_bank, CYBERSTORM_MAPROM_BASE >> 16, 524288 >> 16, 0);

	if (blizzardmaprom_bank_mapped)
		mapped_free(&blizzardmaprom_bank);
	if (blizzardmaprom2_bank_mapped)
		mapped_free(&blizzardmaprom2_bank);

	// BPPC write protects MAP ROM region. CSPPC does not.

	if (maprom_state) {
		blizzardmaprom_bank.baseaddr = blizzardram_bank.baseaddr + cpuboard_size - 2 * 524288;

		blizzardmaprom2_bank.baseaddr = blizzardram_bank.baseaddr + cpuboard_size - 524288;
		blizzardmaprom2_bank.start = blizzardram_bank.start + cpuboard_size - 524288;
		blizzardmaprom2_bank.reserved_size = 524288;
		blizzardmaprom2_bank.mask = 524288 - 1;
		blizzardmaprom2_bank.flags |= ABFLAG_INDIRECT | ABFLAG_NOALLOC;
		mapped_malloc(&blizzardmaprom2_bank);
		blizzardmaprom2_bank_mapped = true;
		map_banks(&blizzardmaprom2_bank, blizzardmaprom2_bank.start >> 16, 524288 >> 16, 0);

		maprom_writeprotect(blizzardmaprom2_bank.baseaddr, 524288);

	} else {

		blizzardmaprom_bank.baseaddr = blizzardram_bank.baseaddr + cpuboard_size - 524288;
		blizzardmaprom2_bank.baseaddr = NULL;
		maprom_unwriteprotect();

	}

	blizzardmaprom_bank.start = CYBERSTORM_MAPROM_BASE;
	blizzardmaprom_bank.reserved_size = 524288;
	blizzardmaprom_bank.mask = 524288 - 1;
	blizzardmaprom_bank.flags |= ABFLAG_INDIRECT | ABFLAG_NOALLOC;
	mapped_malloc(&blizzardmaprom_bank);
	blizzardmaprom_bank_mapped = true;
	map_banks(&blizzardmaprom_bank, CYBERSTORM_MAPROM_BASE >> 16, 524288 >> 16, 0);
}

static void cyberstorm_maprom(void)
{
	if (a3000hmem_bank.reserved_size <= 2 * 524288)
		return;

	map_banks(&dummy_bank, CYBERSTORM_MAPROM_BASE >> 16, 524288 >> 16, 0);
	if (blizzardmaprom_bank_mapped)
		mapped_free(&blizzardmaprom_bank);
	if (maprom_state) {
		blizzardmaprom_bank.baseaddr = a3000hmem_bank.baseaddr + a3000hmem_bank.reserved_size - 2 * 524288;
	} else {
		blizzardmaprom_bank.baseaddr = a3000hmem_bank.baseaddr + a3000hmem_bank.reserved_size - 524288;
	}
	blizzardmaprom_bank.start = CYBERSTORM_MAPROM_BASE;
	blizzardmaprom_bank.reserved_size = 524288;
	blizzardmaprom_bank.mask = 524288 - 1;
	blizzardmaprom_bank.flags |= ABFLAG_INDIRECT | ABFLAG_NOALLOC;
	mapped_malloc(&blizzardmaprom_bank);
	blizzardmaprom_bank_mapped = true;

	map_banks(&blizzardmaprom_bank, CYBERSTORM_MAPROM_BASE >> 16, 524288 >> 16, 0);
}

static void cyberstormmk2_maprom(void)
{
	if (maprom_state)
		map_banks_nojitdirect(&blizzardmaprom_bank, blizzardmaprom_bank.start >> 16, 524288 >> 16, 0);
}

void cyberstorm_mk3_ppc_irq(int level)
{
	if (level)
		io_reg[CSIII_REG_IRQ] &= ~P5_IRQ_SCSI;
	else
		io_reg[CSIII_REG_IRQ] |= P5_IRQ_SCSI;
	cpuboard_rethink();
}

void blizzardppc_irq(int level)
{
	if (level)
		io_reg[CSIII_REG_IRQ] &= ~P5_IRQ_SCSI;
	else
		io_reg[CSIII_REG_IRQ] |= P5_IRQ_SCSI;
	cpuboard_rethink();
}

static uae_u32 REGPARAM2 blizzardio_bget(uaecptr addr)
{
	uae_u8 v = 0;
	//write_log(_T("CS IO XBGET %08x=%02X PC=%08x\n"), addr, v & 0xff, M68K_GETPC);
	if (is_csmk3(&currprefs) || is_blizzardppc(&currprefs)) {
		uae_u32 bank = addr & 0x10000;
		if (bank == 0) {
			int reg = (addr & 0xff) / 8;
			v = io_reg[reg];
			if (reg == CSIII_REG_LOCK) {
				v &= 0x01;
				v |= 0x10;
				if (is_blizzardppc(&currprefs)) {
					v |= 0x08; // BPPC identification
					if (maprom_state)
						v |= 0x04;
				}
			} else if (reg == CSIII_REG_IRQ)  {
				v &= 0x3f;
			} else if (reg == CSIII_REG_INT) {
				v |= 0x40 | 0x10;
			} else if (reg == CSIII_REG_SHADOW) {
				v |= 0x08;
			} else if (reg == CSIII_REG_RESET) {
				v &= 0x1f;
			} else if (reg == CSIII_REG_IPL_EMU) {
				// PPC not master: m68k IPL = all ones
				if (io_reg[CSIII_REG_INT] & P5_INT_MASTER)
					v |= P5_M68k_IPL_MASK;
			}
#if CPUBOARD_IO_LOG > 0
			if (reg != CSIII_REG_IRQ || CPUBOARD_IO_LOG > 2)
				write_log(_T("CS IO BGET %08x=%02X PC=%08x\n"), addr, v & 0xff, M68K_GETPC);
#endif
		} else {
#if CPUBOARD_IO_LOG > 0
			write_log(_T("CS IO BGET %08x=%02X PC=%08x\n"), addr, v & 0xff, M68K_GETPC);
#endif
		}
	}
	return v;
}
static uae_u32 REGPARAM2 blizzardio_wget(uaecptr addr)
{
	if (is_csmk3(&currprefs) || is_blizzardppc(&currprefs)) {
		;//write_log(_T("CS IO WGET %08x\n"), addr);
		//activate_debugger();
	} else if (is_aca500(&currprefs)) {
		addr &= 0x3f000;
		switch (addr)
		{
			case 0x03000:
			return 0;
			case 0x07000:
			return 0;
			case 0x0b000:
			return 0;
			case 0x0f000:
			return 0;
			case 0x13000:
			return 0;
			case 0x17000:
			return 0;
			case 0x1b000:
			return 0x8000;
			case 0x1f000:
			return 0x8000;
			case 0x23000:
			return 0;
			case 0x27000:
			return 0;
			case 0x2b000:
			return 0;
			case 0x2f000:
			return 0;
			case 0x33000:
			return 0x8000;
			case 0x37000:
			return 0;
			case 0x3b000:
			return 0;
		}
	}
	return 0;
}
static uae_u32 REGPARAM2 blizzardio_lget(uaecptr addr)
{
	write_log(_T("CS IO LGET %08x PC=%08x\n"), addr, M68K_GETPC);
	if (is_blizzard2060(&currprefs) && mapromconfigured()) {
		if (addr & 0x10000000) {
			maprom_state = 0;
		} else {
			maprom_state = 1;
		}
	}
	return 0;
}
static void REGPARAM2 blizzardio_bput(uaecptr addr, uae_u32 v)
{
#if CPUBOARD_IO_LOG > 1
	write_log(_T("CS IO XBPUT %08x %02x PC=%08x\n"), addr, v & 0xff, M68K_GETPC);
#endif
	if (is_fusionforty(&currprefs)) {
		write_log(_T("FusionForty IO XBPUT %08x %02x PC=%08x\n"), addr, v & 0xff, M68K_GETPC);
	} else if (is_csmk2(&currprefs)) {
		csmk2_flashaddressing = addr & 3;
		if (addr == 0x880000e3 && v == 0x2a) {
			maprom_state = 1;
			write_log(_T("CSMKII: MAPROM enabled\n"));
			cyberstormmk2_copymaprom();
		}
	} else if (is_blizzard(&currprefs) || is_blizzard1230mk2(&currprefs) || is_blizzard1230mk3(&currprefs)) {
		if ((addr & 65535) == (BLIZZARD_MAPROM_ENABLE & 65535)) {
			if (v != 0x42 || maprom_state || !mapromconfigured())
				return;
			maprom_state = 1;
			write_log(_T("Blizzard: MAPROM enabled\n"));
			blizzard_copymaprom();
		}
	} else if (is_csmk3(&currprefs) || is_blizzardppc(&currprefs)) {
#if CPUBOARD_IO_LOG > 0
		write_log(_T("CS IO BPUT %08x %02x PC=%08x\n"), addr, v & 0xff, M68K_GETPC);
#endif
		uae_u32 bank = addr & 0x10000;
		if (bank == 0) {
			addr &= 0xff;
			if (is_blizzardppc(&currprefs)) {
				if (addr == BPPC_UNLOCK_FLASH && v == BPPC_MAGIC_UNLOCK_VALUE) {
					flash_unlocked = 1;
					write_log(_T("BPPC: flash unlocked\n"));
				} else 	if (addr == BPPC_LOCK_FLASH) {
					flash_unlocked = 0;
					write_log(_T("BPPC: flash locked\n"));
				} else if (addr == BPPC_MAPROM_ON) {
					write_log(_T("BPPC: maprom enabled\n"));
					maprom_state = -1;
					cyberstorm_copymaprom();
					blizzardppc_maprom();
				} else if (addr == BPPC_MAPROM_OFF) {
					write_log(_T("BPPC: maprom disabled\n"));
					maprom_state = 0;
					blizzardppc_maprom();
					cyberstorm_copymaprom();
				}
			}
			addr /= 8;
			uae_u8 oldval = io_reg[addr];
			if (addr == CSIII_REG_LOCK) {
				uae_u8 regval = io_reg[CSIII_REG_LOCK] & 0x0f;
				if (v == P5_MAGIC1)
					regval |= P5_MAGIC1;
				else if ((v & 0x70) == P5_MAGIC2 && (io_reg[CSIII_REG_LOCK] & 0x70) == P5_MAGIC1)
					regval |= P5_MAGIC2;
				else if ((v & 0x70) == P5_MAGIC3 && (io_reg[CSIII_REG_LOCK] & 0x70) == P5_MAGIC2)
					regval |= P5_MAGIC3;
				else if ((v & 0x70) == P5_MAGIC4 && (io_reg[CSIII_REG_LOCK] & 0x70) == P5_MAGIC3)
					regval |= P5_MAGIC4;
				if ((regval & 0x70) == P5_MAGIC3)
					flash_unlocked = 1;
				else
					flash_unlocked &= ~2;
				if (v & P5_LOCK_CPU) {
					if (v & 0x80) {
#ifdef WITH_PPC
						if (uae_ppc_cpu_unlock())
#endif
							regval |= P5_LOCK_CPU;
					} else {
#ifdef WITH_PPC
						if (!(regval & P5_LOCK_CPU))
							uae_ppc_cpu_lock();
#endif
						regval &= ~P5_LOCK_CPU;
					}
#if CPUBOARD_IO_LOG > 0
					write_log(_T("CSIII_REG_LOCK bit 0 = %d!\n"), (v & 0x80) ? 1 : 0);
#endif
				}
				io_reg[CSIII_REG_LOCK] = regval;
			} else {
				uae_u32 regval;
				// reg shadow is only writable if unlock sequence is active
				if (addr == CSIII_REG_SHADOW) {
					if (v & 2) {
						// for some reason this unknown bit can be modified even when locked
						io_reg[CSIII_REG_LOCK] &= ~2;
						if (v & 0x80)
							io_reg[CSIII_REG_LOCK] |= 2;
					}
					if ((io_reg[CSIII_REG_LOCK] & 0x70) != P5_MAGIC3)
						return;
				}
				if (v & 0x80)
					io_reg[addr] |= v & 0x7f;
				else
					io_reg[addr] &= ~v;
				regval = io_reg[addr];
				if (addr == CSIII_REG_RESET) {
					map_banks(&dummy_bank, 0xf00000 >> 16, 0x80000 >> 16, 0);
					map_banks(&blizzardio_bank, 0xf60000 >> 16, 0x10000 >> 16, 0);
					if (regval & P5_SCSI_RESET) {
						if ((regval & P5_SCSI_RESET) != (oldval & P5_SCSI_RESET))
							write_log(_T("CS: SCSI reset cleared\n"));
						map_banks(&blizzardf0_bank, 0xf00000 >> 16, 0x40000 >> 16, 0);
						if (is_blizzardppc(&currprefs) || flash_size(flashrom) >= 262144) {
							map_banks(&ncr_bank_generic, 0xf40000 >> 16, 0x10000 >> 16, 0);
						} else {
							map_banks(&ncr_bank_cyberstorm, 0xf40000 >> 16, 0x10000 >> 16, 0);
							map_banks(&blizzardio_bank, 0xf50000 >> 16, 0x10000 >> 16, 0);
						}
					} else {
						if ((regval & P5_SCSI_RESET) != (oldval & P5_SCSI_RESET))
							write_log(_T("CS: SCSI reset\n"));
						map_banks(&blizzardf0_bank, 0xf00000 >> 16, 0x60000 >> 16, 0);
					}
					if (!(io_reg[CSIII_REG_SHADOW] & P5_SELF_RESET) || uae_self_is_ppc() == false) {
						if ((oldval & P5_PPC_RESET) && !(regval & P5_PPC_RESET)) {
#ifdef WITH_PPC
							uae_ppc_cpu_stop();
#endif
						} else if (!(oldval & P5_PPC_RESET) && (regval & P5_PPC_RESET)) {
#ifdef WITH_PPC
							uae_ppc_cpu_reboot();
#endif
						}
					} else {
						io_reg[CSIII_REG_RESET] &= ~P5_PPC_RESET;
						io_reg[CSIII_REG_RESET] |= oldval & P5_PPC_RESET;
					}
					if (!(io_reg[CSIII_REG_SHADOW] & P5_SELF_RESET) || uae_self_is_ppc() == true) {
						if ((regval & P5_M68K_RESET) && !(oldval & P5_M68K_RESET)) {
							m68k_reset();
							write_log(_T("CS: M68K Reset\n"));
						} else if (!(regval & P5_M68K_RESET) && (oldval & P5_M68K_RESET)) {
							write_log(_T("CS: M68K Halted\n"));
							if (!(regval & P5_PPC_RESET)) {
								write_log(_T("CS: PPC is also halted. STOP.\n"));
								cpu_halt(CPU_HALT_ALL_CPUS_STOPPED);
							} else {
								// halt 68k, leave ppc message processing active.
								cpu_halt(CPU_HALT_PPC_ONLY);
							}
						}
					} else {
						io_reg[CSIII_REG_RESET] &= ~P5_M68K_RESET;
						io_reg[CSIII_REG_RESET] |= oldval & P5_M68K_RESET;
					}
					if (!(io_reg[CSIII_REG_SHADOW] & P5_SELF_RESET)) {
						if (!(regval & P5_AMIGA_RESET)) {
#ifdef WITH_PPC
							uae_ppc_cpu_stop();
#endif
							uae_reset(0, 0);
							write_log(_T("CS: Amiga Reset\n"));
							io_reg[addr] |= P5_AMIGA_RESET;
						}
					} else {
						io_reg[CSIII_REG_RESET] &= ~P5_AMIGA_RESET;
						io_reg[CSIII_REG_RESET] |= oldval & P5_AMIGA_RESET;
					}
				} else if (addr == CSIII_REG_IPL_EMU) {
					// M68K_IPL_MASK is read-only
					regval &= ~P5_M68k_IPL_MASK;
					regval |= oldval & P5_M68k_IPL_MASK;
					bool active = (regval & P5_DISABLE_INT) == 0;
					if (!active) {
#ifdef WITH_PPC
						clear_ppc_interrupt();
#endif
					}
					io_reg[addr] = regval;
#if CPUBOARD_IRQ_LOG > 0
					if ((regval & P5_DISABLE_INT) != (oldval & P5_DISABLE_INT))
						write_log(_T("CS: interrupt state: %s\n"), (regval & P5_DISABLE_INT) ? _T("disabled") : _T("enabled"));
					if ((regval & P5_PPC_IPL_MASK) != (oldval & P5_PPC_IPL_MASK))
						write_log(_T("CS: PPC IPL %02x\n"), (~regval) & P5_PPC_IPL_MASK);
#endif
				} else if (addr == CSIII_REG_INT) {
#if CPUBOARD_IRQ_LOG > 0
					if ((regval & P5_INT_MASTER) != (oldval & P5_INT_MASTER))
						write_log(_T("CS: interrupt master: %s\n"), (regval & P5_INT_MASTER) ? _T("m68k") : _T("ppc"));
					if ((regval & P5_ENABLE_IPL) != (oldval & P5_ENABLE_IPL))
						write_log(_T("CS: IPL state: %s\n"), (regval & P5_ENABLE_IPL) ? _T("disabled") : _T("enabled"));
#endif
				} else if (addr == CSIII_REG_INT_LVL) {
#if CPUBOARD_IRQ_LOG > 0
					if (regval != oldval)
						write_log(_T("CS: interrupt level: %02x\n"), regval);
#endif
				} else if (addr == CSIII_REG_IRQ) {
#if CPUBOARD_IRQ_LOG > 0
					if (regval != oldval)
						write_log(_T("CS: IRQ: %02x\n"), regval);
#endif
				} else if (addr == CSIII_REG_SHADOW) {
					if (is_csmk3(&currprefs) && ((oldval ^ regval) & 1)) {
						maprom_state = (regval & P5_SHADOW) ? 0 : -1;
						write_log(_T("CyberStorm MAPROM = %d\n"), maprom_state);
						cyberstorm_copymaprom();
						cyberstorm_maprom();
					}
					if ((regval & P5_FLASH) != (oldval & P5_FLASH)) {
						flash_unlocked = (regval & P5_FLASH) ? 0 : 1;
						write_log(_T("CS: Flash writable = %d\n"), flash_unlocked);
					}
//					if ((oldval ^ regval) & 7) {
//						activate_debugger();
//					}
				}
				cpuboard_rethink();
			}
		}
	}
}
static void REGPARAM2 blizzardio_wput(uaecptr addr, uae_u32 v)
{
	if (is_fusionforty(&currprefs)) {
		write_log(_T("FusionForty IO WPUT %08x %04x\n"), addr, v);
	} else if (is_blizzard(&currprefs)) {
		write_log(_T("CS IO WPUT %08x %04x\n"), addr, v);
		if((addr & 65535) == (BLIZZARD_BOARD_DISABLE & 65535)) {
			if (v != 0xcafe)
				return;
			write_log(_T("Blizzard: fallback CPU!\n"));
			cpu_fallback(0);
		}
	} else if (is_csmk3(&currprefs) || is_blizzardppc(&currprefs)) {
		write_log(_T("CS IO WPUT %08x %04x\n"), addr, v);
	} else if (is_csmk2(&currprefs) || is_blizzard2060(&currprefs)) {
		write_log(_T("CS IO WPUT %08x %04x\n"), addr, v);
		if (addr == CSMK2_2060_BOARD_DISABLE) {
			if (v != 0xcafe)
				return;
			write_log(_T("CSMK2/2060: fallback CPU!\n"));
			cpu_fallback(0);
		}
	}
}
static void REGPARAM2 blizzardio_lput(uaecptr addr, uae_u32 v)
{
	write_log(_T("CS IO LPUT %08x %08x\n"), addr, v);
	if (is_csmk1(&currprefs)) {
		if (addr == 0x80f80000) {
			maprom_state = 1;
			cyberstormmk1_copymaprom();
		}
	}
	if (is_blizzard2060(&currprefs) && mapromconfigured()) {
		if (addr & 0x10000000) {
			maprom_state = 0;
		} else {
			maprom_state = 1;
		}
	}
}

void cpuboard_hsync(void)
{
	// we should call check_ppc_int_lvl() immediately
	// after PPC CPU's interrupt flag is cleared but this
	// should be fast enough for now
	if (is_csmk3(&currprefs) || is_blizzardppc(&currprefs)) {
#ifdef WITH_PPC
		check_ppc_int_lvl();
#endif
	}
}

void cpuboard_vsync(void)
{
	if (delayed_rom_protect <= 0)
		return;
	delayed_rom_protect--;
	if (delayed_rom_protect == 0)
		protect_roms(true);
}

void cpuboard_map(void)
{
#ifdef FSUAE
    write_log("cpuboard_map currprefs.cpuboard_type = %d\n", currprefs.cpuboard_type);
#endif
	if (!currprefs.cpuboard_type)
		return;

	bool fallback_cpu = currprefs.cpu_model < 68020;

	if (is_blizzard1230mk2(&currprefs) || is_blizzard1230mk3(&currprefs)) {
		map_banks(&blizzardram_bank, blizzardram_bank.start >> 16, cpuboard_size >> 16, 0);
		map_banks(&blizzardio_bank, BLIZZARD_MAPROM_ENABLE >> 16, 65536 >> 16, 0);
		if (is_blizzard1230mk2 (&currprefs) && cpuboard_size < 64 * 1024 * 1024) {
			map_banks(&blizzardmaprom_bank, BLIZZARDMK2_MAPROM_BASE >> 16, 524288 >> 16, 0);
		}
		if (is_blizzard1230mk3(&currprefs)) {
			map_banks(&blizzardmaprom_bank, BLIZZARDMK3_MAPROM_BASE >> 16, 524288 >> 16, 0);
		}
	}
	if (is_blizzard(&currprefs) || is_blizzardppc(&currprefs)) {
		if (!fallback_cpu) {
			map_banks(&blizzardram_bank, blizzardram_bank.start >> 16, cpuboard_size >> 16, 0);
			if (!is_blizzardppc(&currprefs)) {
				if (cpuboard_size < 256 * 1024 * 1024)
					map_banks(&blizzardmaprom_bank, BLIZZARDMK4_MAPROM_BASE >> 16, 524288 >> 16, 0);
				map_banks(&blizzardf0_bank, 0xf00000 >> 16, 65536 >> 16, 0);
				map_banks(&blizzardio_bank, BLIZZARD_MAPROM_ENABLE >> 16, 65536 >> 16, 0);
				map_banks(&blizzardio_bank, BLIZZARD_BOARD_DISABLE >> 16, 65536 >> 16, 0);
				map_banks(&blizzardio_bank, BLIZZARD_BOARD_DISABLE2 >> 16, 65536 >> 16, 0);
			} else {
				map_banks(&blizzardf0_bank, 0xf00000 >> 16, 0x60000 >> 16, 0);
				map_banks(&blizzardio_bank, 0xf60000 >> 16, (2 * 65536) >> 16, 0);
				blizzardppc_maprom();
			}
		} else {
			map_banks(&dummy_bank, 0xf00000 >> 16, 0x80000 >> 16, 0);
		}
	}
	if (is_csmk3(&currprefs)) {
		map_banks(&blizzardf0_bank, 0xf00000 >> 16, 0x40000 >> 16, 0);
		map_banks(&blizzardio_bank, 0xf50000 >> 16, (3 * 65536) >> 16, 0);
		cyberstorm_maprom();
	}
	if (is_csmk2(&currprefs)) {
		if (!fallback_cpu) {
			map_banks(&blizzardio_bank, 0x88000000 >> 16, 65536 >> 16, 0);
			map_banks(&blizzardio_bank, 0x83000000 >> 16, 65536 >> 16, 0);
			map_banks(&blizzardf0_bank, 0xf00000 >> 16, 65536 >> 16, 0);
			cyberstormmk2_maprom();
		} else {
			map_banks(&dummy_bank, 0xf00000 >> 16, 0x80000 >> 16, 0);
		}
	}
	if (is_csmk1(&currprefs)) {
		map_banks(&blizzardio_bank, 0x80f80000 >> 16, 65536 >> 16, 0);
		map_banks(&blizzardf0_bank, 0xf00000 >> 16, 65536 >> 16, 0);
		map_banks(&blizzardmaprom_bank, 0x07f80000 >> 16, 524288 >> 16, 0);
	}
	if (is_blizzard2060(&currprefs)) {
		if (!fallback_cpu) {
			map_banks(&blizzardf0_bank, 0xf00000 >> 16, 65536 >> 16, 0);
			map_banks(&blizzardio_bank, 0x80000000 >> 16, 0x10000000 >> 16, 0);
			if (mapromconfigured())
				map_banks_nojitdirect(&blizzardmaprom_bank, (a3000hmem_bank.start + a3000hmem_bank.allocated_size - 524288) >> 16, 524288 >> 16, 0);
		} else {
			map_banks(&dummy_bank, 0xf00000 >> 16, 0x80000 >> 16, 0);
		}
	}
	if (is_tekmagic(&currprefs)) {
		map_banks(&blizzardf0_bank, 0xf00000 >> 16, 131072 >> 16, 0);
		map_banks(&blizzardea_bank, 0xf40000 >> 16, 65536 >> 16, 0);
	}
	if (is_fusionforty(&currprefs)) {
		map_banks(&blizzardf0_bank, 0x00f40000 >> 16, 131072 >> 16, 0);
		map_banks(&blizzardf0_bank, 0x05000000 >> 16, 131072 >> 16, 0);
		map_banks(&blizzardio_bank, 0x021d0000 >> 16, 65536 >> 16, 0);
		map_banks(&blizzardram_bank, blizzardram_bank.start >> 16, cpuboard_size >> 16, 0);
	}
	if (is_apollo(&currprefs)) {
		map_banks(&blizzardf0_bank, 0xf00000 >> 16, 131072 >> 16, 0);
	}
	if (is_dkb_wildfire(&currprefs)) {
		map_banks(&blizzardf0_bank, 0xf00000 >> 16, 0x80000 >> 16, 0);
	}
	if (is_dkb_12x0(&currprefs)) {
		if (cpuboard_size >= 4 * 1024 * 1024) {
			if (cpuboard_size <= 0x4000000) {
				map_banks(&blizzardram_bank, blizzardram_bank.start >> 16, 0x4000000 >> 16, cpuboard_size >> 16);
			} else {
				map_banks(&blizzardram_bank, blizzardram_bank.start >> 16, cpuboard_size >> 16, 0);
			}
		}
	}
	if (is_aca500(&currprefs)) {
		map_banks(&blizzardf0_bank, 0xf00000 >> 16, 524288 >> 16, 0);
		map_banks(&blizzardf0_bank, 0xa00000 >> 16, 524288 >> 16, 0);
		map_banks(&blizzardio_bank, 0xb00000 >> 16, 262144 >> 16, 0);
	}
	if (is_ivsvector(&currprefs)) {
		if (!currprefs.address_space_24) {
			map_banks(&cpuboardmem1_bank, cpuboardmem1_bank.start >> 16, (0x04000000 - cpuboardmem1_bank.start) >> 16, cpuboardmem1_bank.allocated_size);
			if (cpuboardmem2_bank.allocated_size)
				map_banks(&cpuboardmem2_bank, cpuboardmem2_bank.start >> 16, (0x10000000 - cpuboardmem2_bank.start) >> 16, cpuboardmem2_bank.allocated_size);
		}
	}
	if (is_mtec_ematrix530(&currprefs) || is_sx32pro(&currprefs)) {
		if (cpuboardmem1_bank.allocated_size) {
			map_banks(&cpuboardmem1_bank, cpuboardmem1_bank.start >> 16, cpuboardmem1_bank.allocated_size >> 16, 0);
		}
	}
}

void cpuboard_reset(void)
{
	bool hardreset = is_hardreset();
#if 0
	if (is_blizzard() || is_blizzardppc())
		canbang = 0;
#endif
	configured = false;
	delayed_rom_protect = 0;
	currprefs.cpuboardmem1_size = changed_prefs.cpuboardmem1_size;
	maprom_unwriteprotect();
	if (hardreset || (!mapromconfigured() && (is_blizzard(&currprefs) || is_blizzard2060(&currprefs))))
		maprom_state = 0;
	if (is_csmk3(&currprefs) || is_blizzardppc(&currprefs)) {
		if (hardreset)
			memset(io_reg, 0x7f, sizeof io_reg);
		io_reg[CSIII_REG_RESET] = 0x7f;
		io_reg[CSIII_REG_IRQ] = 0x7f;
		io_reg[CSIII_REG_IPL_EMU] = 0x40;
		io_reg[CSIII_REG_LOCK] = 0x01;
	}
	if (hardreset || is_keyboardreset())
		a2630_io = 0;
	flash_unlocked = 0;
	cpuboard_non_byte_ea = false;

	flash_free(flashrom);
	flashrom = NULL;
	flash_free(flashrom2);
	flashrom2 = NULL;
	zfile_fclose(flashrom_file);
	flashrom_file = NULL;
}

void cpuboard_cleanup(void)
{
	configured = false;
	maprom_state = 0;

	flash_free(flashrom);
	flashrom = NULL;
	flash_free(flashrom2);
	flashrom2 = NULL;
	zfile_fclose(flashrom_file);
	flashrom_file = NULL;

//	if (blizzard_jit) {
		mapped_free(&blizzardram_bank);
#if 0
} else {
		if (blizzardram_nojit_bank.baseaddr) {
#ifdef CPU_64_BIT
			uae_vm_free(blizzardram_nojit_bank.baseaddr, blizzardram_nojit_bank.allocated);
#else
			xfree(blizzardram_nojit_bank.baseaddr);
#endif
		}
	}
#endif
	if (blizzardmaprom_bank_mapped)
		mapped_free(&blizzardmaprom_bank);
	if (blizzardmaprom2_bank_mapped)
		mapped_free(&blizzardmaprom2_bank);
	maprom_unwriteprotect();

	blizzardram_bank.baseaddr = NULL;
//	blizzardram_nojit_bank.baseaddr = NULL;
	blizzardmaprom_bank.baseaddr = NULL;
	blizzardmaprom2_bank.baseaddr = NULL;
	blizzardmaprom_bank_mapped = false;
	blizzardmaprom2_bank_mapped = false;

	mapped_free(&blizzardf0_bank);
	blizzardf0_bank.baseaddr = NULL;

	mapped_free(&blizzardea_bank);
	blizzardea_bank.baseaddr = NULL;

	cpuboard_size = cpuboard2_size = -1;

	blizzardmaprom_bank.flags &= ~(ABFLAG_INDIRECT | ABFLAG_NOALLOC);
	blizzardmaprom2_bank.flags &= ~(ABFLAG_INDIRECT | ABFLAG_NOALLOC);

	mapped_free(&cpuboardmem1_bank);
	mapped_free(&cpuboardmem2_bank);
}

static void cpuboard_init_2(void)
{
	if (!currprefs.cpuboard_type)
		return;

	cpuboard_size = currprefs.cpuboardmem1_size;
	cpuboardmem1_bank.reserved_size = 0;
	cpuboardmem2_bank.reserved_size = 0;

	if (is_kupke(&currprefs) || is_mtec_ematrix530(&currprefs) || is_sx32pro(&currprefs)) {
		// plain 64k autoconfig, nothing else.
		blizzardea_bank.reserved_size = 65536;
		blizzardea_bank.mask = blizzardea_bank.reserved_size - 1;
		mapped_malloc(&blizzardea_bank);

		if (is_mtec_ematrix530(&currprefs) || is_sx32pro(&currprefs)) {
			cpuboardmem1_bank.start = 0x18000000 - cpuboard_size;
			cpuboardmem1_bank.reserved_size = cpuboard_size;
			cpuboardmem1_bank.mask = cpuboardmem1_bank.reserved_size - 1;
			mapped_malloc(&cpuboardmem1_bank);
		}

	} else if (is_aca500(&currprefs)) {

		blizzardf0_bank.start = 0x00f00000;
		blizzardf0_bank.reserved_size = 524288;
		blizzardf0_bank.mask = blizzardf0_bank.reserved_size - 1;
		mapped_malloc(&blizzardf0_bank);

	} else if (is_a2630(&currprefs)) {

		blizzardf0_bank.start = 0x00f00000;
		blizzardf0_bank.reserved_size = 131072;
		blizzardf0_bank.mask = blizzardf0_bank.reserved_size - 1;
		mapped_malloc(&blizzardf0_bank);

		blizzardea_bank.reserved_size = 65536;
		blizzardea_bank.mask = blizzardea_bank.reserved_size - 1;
		mapped_malloc(&blizzardea_bank);

	}
	else if (is_dkb_12x0(&currprefs)) {

		blizzardram_bank.start = 0x10000000;
		blizzardram_bank.reserved_size = cpuboard_size;
		blizzardram_bank.mask = blizzardram_bank.reserved_size - 1;
		blizzardram_bank.startmask = 0x10000000;
		blizzardram_bank.label = _T("dkb");
		mapped_malloc(&blizzardram_bank);

	} else if (is_dkb_wildfire(&currprefs)) {

		blizzardf0_bank.start = 0x00f00000;
		blizzardf0_bank.reserved_size = 65536;
		blizzardf0_bank.mask = blizzardf0_bank.reserved_size - 1;
		mapped_malloc(&blizzardf0_bank);

	} else if (is_apollo(&currprefs)) {

		blizzardf0_bank.start = 0x00f00000;
		blizzardf0_bank.reserved_size = 131072;
		blizzardf0_bank.mask = blizzardf0_bank.reserved_size - 1;
		mapped_malloc(&blizzardf0_bank);

	} else if (is_fusionforty(&currprefs)) {

		blizzardf0_bank.start = 0x00f40000;
		blizzardf0_bank.reserved_size = 262144;
		blizzardf0_bank.mask = blizzardf0_bank.reserved_size - 1;
		mapped_malloc(&blizzardf0_bank);

		blizzardram_bank.start = 0x11000000;
		blizzardram_bank.reserved_size = cpuboard_size;
		blizzardram_bank.mask = blizzardram_bank.reserved_size - 1;
		blizzardram_bank.startmask = 0x11000000;
		blizzardram_bank.label = _T("fusionforty");
		mapped_malloc(&blizzardram_bank);

	} else if (is_tekmagic(&currprefs)) {

		blizzardf0_bank.start = 0x00f00000;
		blizzardf0_bank.reserved_size = 131072;
		blizzardf0_bank.mask = blizzardf0_bank.reserved_size - 1;
		mapped_malloc(&blizzardf0_bank);

		blizzardea_bank.reserved_size = 65536;
		blizzardea_bank.mask = blizzardea_bank.reserved_size - 1;
		mapped_malloc(&blizzardea_bank);

	} else if (is_blizzard1230mk2(&currprefs)) {

		blizzardea_bank.reserved_size = 2 * 65536;
		blizzardea_bank.mask = blizzardea_bank.reserved_size - 1;
		mapped_malloc(&blizzardea_bank);

		blizzardram_bank.start = 0x0e000000 - cpuboard_size / 2;
		blizzardram_bank.reserved_size = cpuboard_size;
		blizzardram_bank.mask = blizzardram_bank.reserved_size - 1;
		if (cpuboard_size) {
			blizzardram_bank.label = _T("*");
			mapped_malloc(&blizzardram_bank);
		}

		blizzardmaprom_bank.baseaddr = blizzardram_bank.baseaddr + cpuboard_size - 524288;
		blizzardmaprom_bank.start = BLIZZARDMK2_MAPROM_BASE;
		blizzardmaprom_bank.reserved_size = 524288;
		blizzardmaprom_bank.mask = 524288 - 1;
		blizzardmaprom_bank.flags |= ABFLAG_INDIRECT | ABFLAG_NOALLOC;
		mapped_malloc(&blizzardmaprom_bank);
		blizzardmaprom_bank_mapped = true;

		maprom_base = blizzardram_bank.reserved_size - 524288;

	} else if (is_blizzard1230mk3(&currprefs)) {

		blizzardea_bank.reserved_size = 2 * 65536;
		blizzardea_bank.mask = blizzardea_bank.reserved_size - 1;
		mapped_malloc(&blizzardea_bank);

		blizzardram_bank.start = 0x1e000000 - cpuboard_size / 2;
		blizzardram_bank.reserved_size = cpuboard_size;
		blizzardram_bank.mask = blizzardram_bank.reserved_size - 1;
		if (cpuboard_size) {
			blizzardram_bank.label = _T("*");
			mapped_malloc(&blizzardram_bank);
		}

		blizzardmaprom_bank.baseaddr = blizzardram_bank.baseaddr + cpuboard_size - 524288;
		blizzardmaprom_bank.start = BLIZZARDMK3_MAPROM_BASE;
		blizzardmaprom_bank.reserved_size = 524288;
		blizzardmaprom_bank.mask = 524288 - 1;
		blizzardmaprom_bank.flags |= ABFLAG_INDIRECT | ABFLAG_NOALLOC;
		mapped_malloc(&blizzardmaprom_bank);
		blizzardmaprom_bank_mapped = true;

		maprom_base = blizzardram_bank.reserved_size - 524288;


	} else if (is_blizzard(&currprefs) || is_blizzardppc(&currprefs)) {

		blizzard_jit = 1;
		blizzardram_bank.start = BLIZZARDMK4_RAM_BASE_48 - cpuboard_size / 2;
		blizzardram_bank.reserved_size = cpuboard_size;
		blizzardram_bank.mask = blizzardram_bank.reserved_size - 1;
		if (cpuboard_size) {
			blizzardram_bank.label = _T("*");
			mapped_malloc(&blizzardram_bank);
		}

		blizzardf0_bank.start = 0x00f00000;
		blizzardf0_bank.reserved_size = 524288;
		blizzardf0_bank.mask = blizzardf0_bank.reserved_size - 1;
		mapped_malloc(&blizzardf0_bank);

		if (!is_blizzardppc(&currprefs)) {
			blizzardea_bank.reserved_size = 2 * 65536;
			blizzardea_bank.mask = blizzardea_bank.reserved_size - 1;
			// Blizzard 12xx autoconfig ROM must be mapped at $ea0000-$ebffff, board requires it.
			mapped_malloc(&blizzardea_bank);
		}

		if (cpuboard_size > 2 * 524288) {
			if (!is_blizzardppc(&currprefs)) {
				blizzardmaprom_bank.baseaddr = blizzardram_bank.baseaddr + cpuboard_size - 524288;
				blizzardmaprom_bank.start = BLIZZARDMK4_MAPROM_BASE;
				blizzardmaprom_bank.reserved_size = 524288;
				blizzardmaprom_bank.mask = 524288 - 1;
				blizzardmaprom_bank.flags |= ABFLAG_INDIRECT | ABFLAG_NOALLOC;
				mapped_malloc(&blizzardmaprom_bank);
				blizzardmaprom_bank_mapped = true;
			}
		}

		maprom_base = blizzardram_bank.reserved_size - 524288;

	} else if (is_csmk1(&currprefs)) {

		blizzardf0_bank.start = 0x00f00000;
		blizzardf0_bank.reserved_size = 65536;
		blizzardf0_bank.mask = blizzardf0_bank.reserved_size - 1;
		mapped_malloc(&blizzardf0_bank);

		blizzardea_bank.reserved_size = 2 * 65536;
		blizzardea_bank.mask = 65535 - 1;
		mapped_malloc(&blizzardea_bank);

		blizzardmaprom_bank.reserved_size = 524288;
		blizzardmaprom_bank.start = 0x07f80000;
		blizzardmaprom_bank.mask = 524288 - 1;
		blizzardmaprom_bank_mapped = true;
		mapped_malloc(&blizzardmaprom_bank);

	} else if (is_csmk2(&currprefs) || is_blizzard2060(&currprefs)) {

		blizzardea_bank.reserved_size = 2 * 65536;
		blizzardea_bank.mask = blizzardea_bank.reserved_size - 1;
		mapped_malloc(&blizzardea_bank);

		blizzardf0_bank.start = 0x00f00000;
		blizzardf0_bank.reserved_size = 65536;
		blizzardf0_bank.mask = blizzardf0_bank.reserved_size - 1;
		mapped_malloc(&blizzardf0_bank);

		blizzardmaprom_bank.baseaddr = a3000hmem_bank.baseaddr + a3000hmem_bank.reserved_size - 524288;
		blizzardmaprom_bank.start = a3000hmem_bank.start + a3000hmem_bank.reserved_size - 524288;
		blizzardmaprom_bank.reserved_size = 524288;
		blizzardmaprom_bank.mask = 524288 - 1;
		blizzardmaprom_bank.flags |= ABFLAG_INDIRECT | ABFLAG_NOALLOC;
		mapped_malloc(&blizzardmaprom_bank);

	} else if (is_csmk3(&currprefs)) {

		blizzardf0_bank.start = 0x00f00000;
		blizzardf0_bank.reserved_size = 524288;
		blizzardf0_bank.mask = blizzardf0_bank.reserved_size - 1;
		mapped_malloc(&blizzardf0_bank);

	} else if (is_ivsvector(&currprefs)) {
	
		cpuboardmem1_bank.start = 0x02000000;
		cpuboardmem1_bank.reserved_size = 32 * 1024 * 1024;
		cpuboardmem1_bank.mask = cpuboardmem1_bank.reserved_size - 1;
		mapped_malloc(&cpuboardmem1_bank);

		cpuboardmem2_bank.start = 0x0c000000;
		cpuboardmem2_bank.reserved_size = cpuboard_size;
		cpuboardmem2_bank.mask = cpuboard_size - 1;
		mapped_malloc(&cpuboardmem2_bank);

	}

	if (!cpuboardmem1_bank.baseaddr)
		cpuboardmem1_bank.reserved_size = 0;
	if (!cpuboardmem2_bank.baseaddr)
		cpuboardmem2_bank.reserved_size = 0;
}

void cpuboard_init(void)
{
	if (!currprefs.cpuboard_type)
		return;

	if (cpuboard_size == currprefs.cpuboardmem1_size)
		return;

	cpuboard_cleanup();
	cpuboard_init_2();
}


void cpuboard_overlay_override(void)
{
	if (is_a2630(&currprefs)) {
		if (!(a2630_io & 2))
			map_banks(&blizzardf0_bank, 0xf80000 >> 16, f0rom_size >> 16, 0);
		if (mem25bit_bank.allocated_size)
			map_banks(&chipmem_bank, (mem25bit_bank.start + mem25bit_bank.allocated_size) >> 16, (1024 * 1024) >> 16, 0);
		else
			map_banks(&chipmem_bank, 0x01000000 >> 16, (1024 * 1024) >> 16, 0);
	}
}

void cpuboard_clear(void)
{
	if (blizzardmaprom_bank.baseaddr)
		memset(blizzardmaprom_bank.baseaddr, 0, 524288);
	if (blizzardmaprom2_bank.baseaddr)
		memset(blizzardmaprom2_bank.baseaddr, 0, 524288);
	if (is_csmk3(&currprefs)) // SCSI RAM
		memset(blizzardf0_bank.baseaddr + 0x40000, 0, 0x10000);
}

// Adds resource resident that CSPPC/BPPC flash updater checks.

#define FAKEPPCROM_OFFSET 32
static const uae_u8 fakeppcrom[] = {
	// struct Resident
	0x4a, 0xfc,
	0x00, 0xf0, 0x00, FAKEPPCROM_OFFSET,
	0x00, 0xf0, 0x01, 0x00,
	0x02, 0x01, 0x00, 0x78,
	0x00, 0xf0, 0x00, FAKEPPCROM_OFFSET + 30,
	0x00, 0xf0, 0x00, FAKEPPCROM_OFFSET + 30,
	0x00, 0xf0, 0x00, FAKEPPCROM_OFFSET + 26,
	// moveq #0,d0; rts
	0x70, 0x00, 0x4e, 0x75
};
static const char fakeppcromtxt_cs[] = { "CyberstormPPC.IDTag" };
static const char fakeppcromtxt_bz[] = { "BlizzardPPC.IDTag" };

static void makefakeppcrom(uae_u8 *rom, int type)
{
	memcpy(rom + FAKEPPCROM_OFFSET, fakeppcrom, sizeof fakeppcrom);
	const char *txt = type ? fakeppcromtxt_bz : fakeppcromtxt_cs;
	memcpy(rom + FAKEPPCROM_OFFSET + sizeof fakeppcrom, txt, strlen(txt) + 1);
}

bool is_ppc_cpu(struct uae_prefs *p)
{
	return is_ppc(p);
}

bool cpuboard_maprom(void)
{
	if (!currprefs.cpuboard_type || !cpuboard_size)
		return false;
	if (is_blizzard(&currprefs) || is_blizzardppc(&currprefs)) {
		if (maprom_state)
			blizzard_copymaprom();
	} else if (is_csmk3(&currprefs)) {
		if (maprom_state)
			cyberstorm_copymaprom();
	}
	return true;
}

bool cpuboard_jitdirectompatible(struct uae_prefs *p)
{
	if (cpuboard_memorytype(p) == BOARD_MEMORY_BLIZZARD_12xx || cpuboard_memorytype(p) == BOARD_MEMORY_BLIZZARD_PPC) {
		return false;
	}
	return true;
}

bool cpuboard_32bit(struct uae_prefs *p)
{
	int b = cpuboard_memorytype(p);
	if (p->cpuboard_type) {
		if (cpuboards[p->cpuboard_type].subtypes[p->cpuboard_subtype].deviceflags & EXPANSIONTYPE_HAS_FALLBACK)
			return false;
		if (!(cpuboards[p->cpuboard_type].subtypes[p->cpuboard_subtype].deviceflags & EXPANSIONTYPE_24BIT))
			return true;
	}
	return b == BOARD_MEMORY_HIGHMEM ||
		b == BOARD_MEMORY_BLIZZARD_12xx ||
		b == BOARD_MEMORY_BLIZZARD_PPC ||
		b == BOARD_MEMORY_Z3 ||
		b == BOARD_MEMORY_25BITMEM ||
		b == BOARD_MEMORY_CUSTOM_32;
}

int cpuboard_memorytype(struct uae_prefs *p)
{
	if (cpuboards[p->cpuboard_type].subtypes)
		return cpuboards[p->cpuboard_type].subtypes[p->cpuboard_subtype].memorytype;
	return 0;
}

int cpuboard_maxmemory(struct uae_prefs *p)
{
	if (cpuboards[p->cpuboard_type].subtypes)
		return cpuboards[p->cpuboard_type].subtypes[p->cpuboard_subtype].maxmemory;
	return 0;
}

void cpuboard_setboard(struct uae_prefs *p, int type, int subtype)
{
	p->cpuboard_type = 0;
	p->cpuboard_subtype = 0;
	for (int i = 0; cpuboards[i].name; i++) {
		if (cpuboards[i].id == type) {
			p->cpuboard_type = type;
			if (subtype >= 0)
				p->cpuboard_subtype = subtype;
			return;
		}
	}
}

uaecptr cpuboard_get_reset_pc(uaecptr *stack)
{
	if (is_aca500(&currprefs)) {
		*stack = get_long(0xa00000);
		return get_long(0xa00004);
	} else {
		*stack = get_long(0);
		return get_long(4);
	}
}

bool cpuboard_io_special(int addr, uae_u32 *val, int size, bool write)
{
	addr &= 65535;
	if (write) {
		uae_u16 w = *val;
		if (is_a2630(&currprefs)) {
			if ((addr == 0x0040 && size == 2) || (addr == 0x0041 && size == 1)) {
				write_log(_T("A2630 write %04x PC=%08x\n"), w, M68K_GETPC);
				a2630_io = w;
				// bit 0: unmap 0x000000
				// bit 1: unmap 0xf80000
				if (w & 2) {
					if (currprefs.mmu_model == 68030) {
						// HACK!
						mmu030_fake_prefetch = 0x4ed0;
					}
					map_banks(&kickmem_bank, 0xF8, 8, 0);
					write_log(_T("A2630 boot rom unmapped\n"));
				}
				// bit 2: autoconfig region enable
				if (w & 4) {
					write_log(_T("A2630 Autoconfig enabled\n"));
					expamem_next(NULL, NULL);
				}
				// bit 3: unknown
				// bit 4: 68000 mode
				if (w & 0x10) {
					write_log(_T("A2630 fallback CPU mode!\n"));
					cpu_fallback(0);
				}
				return true;
			}
		}
		return false;
	} else {
		if (is_a2630(&currprefs)) {
			// osmode (j304)
			if (addr == 0x0c && (a2630_io & 4) == 0) {
				(*val) |= 0x80;
				if (currprefs.cpuboard_settings & 1)
					(*val) &= ~0x80;
				return true;
			}
		}
	}
	return false;
}

static void fixserial(struct uae_prefs *p, uae_u8 *rom, int size)
{
	uae_u8 value1 = rom[16];
	uae_u8 value2 = rom[17];
	uae_u8 value3 = rom[18];
	int seroffset = 17, longseroffset = 24;
	uae_u32 serialnum = 0x1234;
	char serial[10];
	int type = -1;

	if (ISCPUBOARDP(p, BOARD_BLIZZARD, BOARD_BLIZZARD_SUB_PPC)) {
		value1 = 'I';
		value2 = 'D';
		if (currprefs.cpu_model == 68060)
			value3 = 'H';
		else if (currprefs.fpu_model)
			value3 = 'B';
		else
			value3 = 'A';
		seroffset = 19;
		sprintf(serial, "%04X", serialnum);
		type = 1;
	} else if (ISCPUBOARDP(p, BOARD_CYBERSTORM, BOARD_CYBERSTORM_SUB_PPC)) {
		value1 = 'D';
		value2 = 'B';
		sprintf(serial, "%05X", serialnum);
		value3 = 0;
		seroffset = 18;
		type = 0;
	} else if (ISCPUBOARDP(p, BOARD_CYBERSTORM, BOARD_CYBERSTORM_SUB_MK3)) {
		value1 = 'F';
		sprintf(serial, "%05X", serialnum);
		value2 = value3 = 0;
	} else {
		return;
	}

	rom[16] = value1;
	if (value2)
		rom[17] = value2;
	if (value3)
		rom[18] = value3;
	if (rom[seroffset] == 0) {
		strcpy((char*)rom + seroffset, serial);
		if (longseroffset) {
			rom[longseroffset + 0] = serialnum >> 24;
			rom[longseroffset + 1] = serialnum >> 16;
			rom[longseroffset + 2] = serialnum >>  8;
			rom[longseroffset + 3] = serialnum >>  0;
		}
	}

	if (type >= 0 && rom[0] == 0 && rom[1] == 0)
		makefakeppcrom(rom, type);
}

static struct zfile *flashfile_open(const TCHAR *name)
{
	struct zfile *f;
	TCHAR path[MAX_DPATH];
	bool rw = true;

	if (name == NULL || !name[0])
		return NULL;
	f = zfile_fopen(name, _T("rb"), ZFD_NORMAL);
	if (f) {
		if (zfile_iscompressed(f)) {
			rw = false;
		} else {
			zfile_fclose(f);
			f = NULL;
		}
	}
	if (!f) {
		rw = true;
		f = zfile_fopen(name, _T("rb+"), ZFD_NONE);
		if (!f) {
			rw = false;
			f = zfile_fopen(name, _T("rb"), ZFD_NORMAL);
			if (!f) {
				fetch_rompath(path, sizeof path / sizeof(TCHAR));
				_tcscat(path, name);
				rw = true;
				f = zfile_fopen(path, _T("rb+"), ZFD_NONE);
				if (!f) {
					rw = false;
					f = zfile_fopen(path, _T("rb"), ZFD_NORMAL);
				}
			}
		}
	}
	if (f) {
		write_log(_T("CPUBoard '%s' flash file '%s' loaded, %s.\n"),
		cpuboards[currprefs.cpuboard_type].subtypes[currprefs.cpuboard_subtype].name,
		name, rw ? _T("RW") : _T("RO"));
	}
	return f;
}

static struct zfile *board_rom_open(int *roms, const TCHAR *name)
{
	struct zfile *zf = NULL;
#ifdef FSUAE
	write_log("board_rom_open roms=... name=%s\n", name ? name : "(null)");
#endif
	struct romlist *rl = getromlistbyids(roms, name);
	if (rl)
		zf = read_rom(rl->rd);
	if (!zf && name) {
#ifdef FSUAE
	write_log("zfile_fopen %s\n", name);
#endif
		zf = zfile_fopen(name, _T("rb"), ZFD_NORMAL);
		if (zf) {
			return zf;
		}
		TCHAR path[MAX_DPATH];
		fetch_rompath(path, sizeof path / sizeof(TCHAR));
		_tcscat(path, name);
#ifdef FSUAE
	write_log("zfile_fopen %s\n", path);
#endif
		zf = zfile_fopen(path, _T("rb"), ZFD_NORMAL);
	}
	return zf;
}

static void ew(uae_u8 *p, int addr, uae_u8 value)
{
	if (addr == 00 || addr == 02 || addr == 0x40 || addr == 0x42) {
		p[addr] = (value & 0xf0);
		p[addr + 2] = (value & 0x0f) << 4;
	} else {
		p[addr] = ~(value & 0xf0);
		p[addr + 2] = ~((value & 0x0f) << 4);
	}
}

bool cpuboard_autoconfig_init(struct autoconfig_info *aci)
{
	struct zfile *autoconfig_rom = NULL;
	struct boardromconfig *brc;
	int roms[4], roms2[4];
	bool autoconf = true;
	bool autoconf_stop = false;
	const TCHAR *defaultromname = NULL;
	const TCHAR *romname = NULL;
	bool isflashrom = false;
	struct romdata *rd = NULL;
	const TCHAR *boardname;
	struct uae_prefs *p = aci->prefs;

	boardname = cpuboards[p->cpuboard_type].subtypes[p->cpuboard_subtype].name;
	aci->label = boardname;

	int idx;
	brc = get_device_rom(p, ROMTYPE_CPUBOARD, 0, &idx);
	if (brc)
		romname = brc->roms[idx].romfile;

	for (int i = 0; i < 4; i++) {
		roms[i] = -1;
		roms2[i] = -1;
	}
	cpuboard_non_byte_ea = false;
	int boardid = cpuboards[p->cpuboard_type].id;
	switch (boardid)
	{
		case BOARD_IC:
		break;

		case BOARD_IVS:
		switch (p->cpuboard_subtype)
		{
			case BOARD_IVS_VECTOR:
			aci->addrbank = &expamem_null;
			return true;
		}
		break;

		case BOARD_COMMODORE:
		switch(p->cpuboard_subtype)
		{
			case BOARD_COMMODORE_SUB_A26x0:
			roms[0] = 105;
			roms[1] = 106;
			roms[2] = 164;
			break;
		}
		break;

		case BOARD_ACT:
		switch(p->cpuboard_subtype)
		{
			case BOARD_ACT_SUB_APOLLO:
			roms[0] = 119;
			break;
		}
		break;

		case BOARD_MTEC:
		switch (p->cpuboard_subtype)
		{
			case BOARD_MTEC_SUB_EMATRIX530:
			roms[0] = 144;
			break;
		}
		break;

		case BOARD_MACROSYSTEM:
		switch(p->cpuboard_subtype)
		{
			case BOARD_MACROSYSTEM_SUB_WARPENGINE_A4000:
			aci->addrbank = &expamem_null;
			return true;
		}
		break;

		case BOARD_DKB:
		switch(p->cpuboard_subtype)
		{
			case BOARD_DKB_SUB_12x0:
			aci->addrbank = &expamem_null;
			return true;
			case BOARD_DKB_SUB_WILDFIRE:
			roms[0] = 143;
			break;
		}
		break;

		case BOARD_RCS:
		switch(p->cpuboard_subtype)
		{
			case BOARD_RCS_SUB_FUSIONFORTY:
			roms[0] = 113;
			break;
		}
		break;
	
		case BOARD_GVP:
		switch(p->cpuboard_subtype)
		{
			case BOARD_GVP_SUB_A530:
			case BOARD_GVP_SUB_GFORCE030:
				aci->addrbank = &expamem_null;
				return true;
			case BOARD_GVP_SUB_A3001SI:
			case BOARD_GVP_SUB_A3001SII:
				aci->addrbank = &expamem_null;
				return true;
			case BOARD_GVP_SUB_TEKMAGIC:
				roms[0] = 104;
			break;
		}
		break;

		case BOARD_CYBERSTORM:
		switch(p->cpuboard_subtype)
		{
			case BOARD_CYBERSTORM_SUB_MK1:
				roms[0] = p->cpu_model == 68040 ? 95 : 101;
				isflashrom = true;
				break;
			case BOARD_CYBERSTORM_SUB_MK2:
				roms[0] = 96;
				isflashrom = true;
				break;
			case BOARD_CYBERSTORM_SUB_MK3:
				roms[0] = 97;
				isflashrom = true;
				break;
			case BOARD_CYBERSTORM_SUB_PPC:
				roms[0] = 98;
				isflashrom = true;
				break;
		}
		break;

		case BOARD_BLIZZARD:
		switch(p->cpuboard_subtype)
		{
			case BOARD_BLIZZARD_SUB_1230II:
				roms[0] = 163;
				break;
			case BOARD_BLIZZARD_SUB_1230III:
				roms[0] = 162;
				roms2[0] = 167;
				break;
			case BOARD_BLIZZARD_SUB_1230IV:
				roms[0] = 89;
				roms2[0] = 94;
				break;
			case BOARD_BLIZZARD_SUB_1260:
				roms[0] = 90;
				roms2[0] = 94;
				break;
			case BOARD_BLIZZARD_SUB_2060:
				roms[0] = 92;
				roms[1] = 168;
				break;
			case BOARD_BLIZZARD_SUB_PPC:
				roms[0] = p->cpu_model == 68040 ? 99 : 100;
				isflashrom = true;
				break;
		}
		break;

		case BOARD_KUPKE:
			roms[0] = 126;
		break;

		case BOARD_DCE:
			roms[0] = 160;
		break;

		default:
			return false;
	}

	struct romlist *rl = NULL;
	if (roms[0] >= 0) {
		rl = getromlistbyids(roms, romname);
		if (!rl) {
			rd = getromdatabyids(roms);
			if (!rd)
				return false;
		} else {
			rd = rl->rd;
		}
		defaultromname = rd->defaultfilename;
	}
	if (!isflashrom) {
		if (romname)
			autoconfig_rom = zfile_fopen(romname, _T("rb"));
		if (!autoconfig_rom && defaultromname)
			autoconfig_rom = zfile_fopen(defaultromname, _T("rb"));
		if (rl) {
			if (autoconfig_rom) {
				struct romdata *rd2 = getromdatabyids(roms);
				// Do not use image if it is not long enough (odd or even only?)
				if (!rd2 || zfile_size(autoconfig_rom) < rd2->size) {
					zfile_fclose(autoconfig_rom);
					autoconfig_rom = NULL;
				}
			}
			if (!autoconfig_rom)
				autoconfig_rom = read_rom(rl->rd);
		}
	} else {
		autoconfig_rom = flashfile_open(romname);
		if (!autoconfig_rom) {
			if (rl)
				autoconfig_rom = flashfile_open(rl->path);
			if (!autoconfig_rom)
				autoconfig_rom = flashfile_open(defaultromname);
		}
		if (!autoconfig_rom) {
			if (aci->doinit)
				romwarning(roms);
			write_log(_T("Couldn't open CPUBoard '%s' rom '%s'\n"), boardname, defaultromname);
			return false;
		}
	}
	
	if (!autoconfig_rom && roms[0] != -1) {
		if (aci->doinit)
			romwarning(roms);
		write_log (_T("ROM id %d not found for CPUBoard '%s' emulation\n"), roms[0], boardname);
		return false;
	}
	if (!autoconfig_rom) {
		write_log(_T("Couldn't open CPUBoard '%s' rom '%s'\n"), boardname, defaultromname);
		return false;
	}

	write_log(_T("CPUBoard '%s' ROM '%s' %lld loaded\n"), boardname, zfile_getname(autoconfig_rom), zfile_size(autoconfig_rom));

	uae_u8 *blizzardea_tmp = blizzardea_bank.baseaddr;
	uae_u8 *blizzardf0_tmp = blizzardf0_bank.baseaddr;
	if (!aci->doinit) {
		blizzardea_bank.baseaddr = xcalloc(uae_u8, 65536 * 2);
		blizzardf0_bank.baseaddr = xcalloc(uae_u8, 524288);
	} else {
		protect_roms(false);
	}

	cpuboard_non_byte_ea = true;

	if (is_sx32pro(p)) {
		earom_size = 65536;
		for (int i = 0; i < 32768; i++) {
			uae_u8 b = 0xff;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardea_bank.baseaddr[i * 2 + 0] = b;
			blizzardea_bank.baseaddr[i * 2 + 1] = 0xff;
		}
	} else if (is_mtec_ematrix530(p)) {
		earom_size = 65536;
		for (int i = 0; i < 32768; i++) {
			uae_u8 b = 0xff;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardea_bank.baseaddr[i * 2 + 0] = b;
			blizzardea_bank.baseaddr[i * 2 + 1] = 0xff;
		}
	} else if (is_dkb_wildfire(p)) {
		f0rom_size = 65536;
		zfile_fread(blizzardf0_bank.baseaddr, 1, f0rom_size, autoconfig_rom);
		flashrom = flash_new(blizzardf0_bank.baseaddr + 0, 32768, 65536, 0x20, flashrom_file, FLASHROM_EVERY_OTHER_BYTE | FLASHROM_PARALLEL_EEPROM);
		flashrom2 = flash_new(blizzardf0_bank.baseaddr + 1, 32768, 65536, 0x20, flashrom_file, FLASHROM_EVERY_OTHER_BYTE | FLASHROM_EVERY_OTHER_BYTE_ODD | FLASHROM_PARALLEL_EEPROM);
		autoconf = false;
		aci->start = 0xf00000;
		aci->size = 65536;
	} else if (is_aca500(p)) {
		f0rom_size = 524288;
		zfile_fread(blizzardf0_bank.baseaddr, f0rom_size, 1, autoconfig_rom);
		autoconf = false;
		if (zfile_needwrite(autoconfig_rom)) {
			flashrom_file = autoconfig_rom;
			autoconfig_rom = NULL;
		}
		flashrom = flash_new(blizzardf0_bank.baseaddr, f0rom_size, f0rom_size, 0xa4, flashrom_file, 0);
	} else if (is_a2630(p)) {
		f0rom_size = 65536;
		zfile_fread(blizzardf0_bank.baseaddr, 1, f0rom_size, autoconfig_rom);
		autoconf = false;
		autoconf_stop = true;
		aci->start = 0xf00000;
		aci->size = f0rom_size;
	} else if (is_kupke(p)) {
		earom_size = 65536;
		for (int i = 0; i < 8192; i++) {
			uae_u8 b = 0xff;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardea_bank.baseaddr[i * 2 + 0] = b;
			blizzardea_bank.baseaddr[i * 2 + 1] = 0xff;
		}
	} else if (is_apollo(p)) {
		f0rom_size = 131072;
		zfile_fread(blizzardf0_bank.baseaddr, 1, 131072, autoconfig_rom);
		autoconf = false;
		aci->start = 0xf00000;
		aci->size = f0rom_size;
	} else if (is_fusionforty(p)) {
		f0rom_size = 262144;
		zfile_fread(blizzardf0_bank.baseaddr, 1, 131072, autoconfig_rom);
		autoconf = false;
		aci->start = 0xf00000;
		aci->size = f0rom_size;
	} else if (is_tekmagic(p)) {
		earom_size = 65536;
		f0rom_size = 131072;
		zfile_fread(blizzardf0_bank.baseaddr, 1, f0rom_size, autoconfig_rom);
		autoconf = false;
		cpuboard_non_byte_ea = false;
		aci->start = 0xf00000;
		aci->size = f0rom_size;
	} else if (is_blizzard2060(p)) {
		f0rom_size = 65536;
		earom_size = 131072;
		// 2060 = 2x32k
		for (int i = 0; i < 32768; i++) {
			uae_u8 b = 0xff;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardf0_bank.baseaddr[i * 2 + 1] = b;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardf0_bank.baseaddr[i * 2 + 0] = b;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardea_bank.baseaddr[i * 2 + 1] = b;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardea_bank.baseaddr[i * 2 + 0] = b;
		}
	} else if (is_csmk1(p)) {
		earom_size = 131072; // really 64k but ea and f0 use same space
		f0rom_size = 65536;
		for (int i = 0; i < 32768; i++) {
			uae_u8 b = 0xff;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardea_bank.baseaddr[i * 2 + 0] = b;
			blizzardea_bank.baseaddr[i * 2 + 1] = 0xff;
		}
		zfile_fread(blizzardea_bank.baseaddr + 65536, 65536, 1, autoconfig_rom);
		if (zfile_needwrite(autoconfig_rom)) {
			flashrom_file = autoconfig_rom;
			autoconfig_rom = NULL;
		}
		flashrom = flash_new(blizzardea_bank.baseaddr, earom_size, earom_size, 0x20, flashrom_file, 0);
		memcpy(blizzardf0_bank.baseaddr, blizzardea_bank.baseaddr + 65536, 65536);
	} else if (is_csmk2(p)) {
		earom_size = 131072;
		f0rom_size = 65536;
		zfile_fread(blizzardea_bank.baseaddr, earom_size, 1, autoconfig_rom);
		if (zfile_needwrite(autoconfig_rom)) {
			flashrom_file = autoconfig_rom;
			autoconfig_rom = NULL;
		}
		flashrom = flash_new(blizzardea_bank.baseaddr, earom_size, earom_size, 0x20, flashrom_file, 0);
		memcpy(blizzardf0_bank.baseaddr, blizzardea_bank.baseaddr + 65536, 65536);
	} else if (is_csmk3(p) || is_blizzardppc(p)) {
		uae_u8 flashtype;
		earom_size = 0;
		if (is_blizzardppc(p)) {
			flashtype = 0xa4;
			f0rom_size = 524288;
			aci->last_high_ram = BLIZZARDMK4_RAM_BASE_48 + p->cpuboardmem1_size / 2;
		} else {
			flashtype = 0x20;
			f0rom_size = 131072;
			if (zfile_size(autoconfig_rom) >= 262144) {
				flashtype = 0xa4;
				f0rom_size = 524288;
			}
		}
		// empty rom space must be cleared, not set to ones.
		memset(blizzardf0_bank.baseaddr, 0x00, f0rom_size);
		if (f0rom_size == 524288) // but empty config data must be 0xFF
			memset(blizzardf0_bank.baseaddr + 0x50000, 0xff, 0x10000);
		zfile_fread(blizzardf0_bank.baseaddr, f0rom_size, 1, autoconfig_rom);
		autoconf = false;
		if (zfile_needwrite(autoconfig_rom)) {
			flashrom_file = autoconfig_rom;
			autoconfig_rom = NULL;
		}
		fixserial(p, blizzardf0_bank.baseaddr, f0rom_size);
		flashrom = flash_new(blizzardf0_bank.baseaddr, f0rom_size, f0rom_size, flashtype, flashrom_file, 0);
		aci->start = 0xf00000;
		aci->size = 0x80000;
	} else if (is_blizzard(p)) {
		// 1230 MK IV / 1240/60
		f0rom_size = 65536;
		earom_size = 131072;
		// 12xx = 1x32k
		for (int i = 0; i < 16384; i++) {
			uae_u8 b = 0xff;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardf0_bank.baseaddr[i] = b;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardea_bank.baseaddr[i] = b;
		}
		zfile_fclose(autoconfig_rom);
		autoconfig_rom = NULL;
		if (roms2[0] != -1) {
			int idx2;
			struct boardromconfig *brc2 = get_device_rom(p, ROMTYPE_BLIZKIT4, 0, &idx2);
			if (brc2 && brc2->roms[idx2].romfile[0])
				autoconfig_rom = board_rom_open(roms2, brc2->roms[idx2].romfile);
			if (autoconfig_rom) {
				memset(blizzardea_bank.baseaddr + 0x10000, 0xff, 65536);
				zfile_fread(blizzardea_bank.baseaddr + 0x10000, 32768, 1, autoconfig_rom);
			}
		}
		aci->last_high_ram = BLIZZARDMK4_RAM_BASE_48 + p->cpuboardmem1_size / 2;
	} else if (is_blizzard1230mk2(p)) {
		earom_size = 131072;
		for (int i = 0; i < 32768; i++) {
			uae_u8 b = 0xff;
			zfile_fread(&b, 1, 1, autoconfig_rom);
			blizzardea_bank.baseaddr[i * 2 + 0] = b;
			blizzardea_bank.baseaddr[i * 2 + 1] = 0xff;
		}
		zfile_fclose(autoconfig_rom);
		autoconfig_rom = NULL;
	} else if (is_blizzard1230mk3(p)) {
		earom_size = 131072;
		zfile_fread(blizzardea_bank.baseaddr, 65536, 1, autoconfig_rom);
		zfile_fclose(autoconfig_rom);
		autoconfig_rom = NULL;
		if (roms2[0] != -1) {
			int idx2;
			struct boardromconfig *brc2 = get_device_rom(p, ROMTYPE_BLIZKIT3, 0, &idx2);
			if (brc2 && brc2->roms[idx2].romfile[0])
				autoconfig_rom = board_rom_open(roms2, brc2->roms[idx2].romfile);
			if (autoconfig_rom) {
				zfile_fread(blizzardea_bank.baseaddr, 65536, 1, autoconfig_rom);
				zfile_fclose(autoconfig_rom);
				autoconfig_rom = NULL;
			}
		}
	}

	if (autoconf_stop) {
		aci->addrbank = &expamem_none;
	} else if (!autoconf) {
		aci->zorro = 0;
	} else {
		aci->addrbank = &blizzarde8_bank;
		memcpy(aci->autoconfig_raw, blizzardea_bank.baseaddr, sizeof aci->autoconfig_raw);
		aci->zorro = 2;
	}

	zfile_fclose(autoconfig_rom);

	if (!aci->doinit) {
		xfree(blizzardea_bank.baseaddr);
		xfree(blizzardf0_bank.baseaddr);
		blizzardea_bank.baseaddr = blizzardea_tmp;
		blizzardf0_bank.baseaddr = blizzardf0_tmp;
		flash_free(flashrom);
		flashrom = NULL;
		return true;
	} else {
		protect_roms(true);
	}

	if (f0rom_size) {
		if (is_a2630(p)) {
			if (!(a2630_io & 2))
				map_banks(&blizzardf0_bank, 0xf80000 >> 16, f0rom_size >> 16, 0);
			if (!(a2630_io & 1))
				map_banks(&blizzardf0_bank, 0x000000 >> 16, f0rom_size >> 16, 0);
		} else if (is_fusionforty(p)) {
			map_banks(&blizzardf0_bank, 0x00f40000 >> 16, f0rom_size >> 16, 0);
			map_banks(&blizzardf0_bank, 0x05000000 >> 16, f0rom_size >> 16, 0);
			map_banks(&blizzardf0_bank, 0x00000000 >> 16, f0rom_size >> 16, 0);
		} else {
			map_banks(&blizzardf0_bank, 0xf00000 >> 16, (f0rom_size > 262144 ? 262144 : f0rom_size) >> 16, 0);
		}
	}
	return true;
}
