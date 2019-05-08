 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Debugger
  *
  * (c) 1995 Bernd Schmidt
  *
  */

#ifndef UAE_DEBUG_H
#define UAE_DEBUG_H

#include "uae/types.h"
#ifdef FSUAE
#include "uae/inline.h"
#endif

#ifdef DEBUGGER

#define	MAX_HIST 500
#define MAX_LINEWIDTH 100

extern int debugging;
extern int memwatch_enabled;
extern int exception_debugging;
extern int debug_copper;
extern int debug_dma, debug_heatmap;
extern int debug_sprite_mask;
extern int debug_bpl_mask, debug_bpl_mask_one;
extern int debugger_active;
extern int debug_illegal;
extern uae_u64 debug_illegal_mask;

extern void debug (void);
extern void debugger_change (int mode);
extern void activate_debugger (void);
extern void deactivate_debugger (void);
extern int notinrom (void);
extern const TCHAR *debuginfo (int);
extern void record_copper (uaecptr addr, uae_u16 word1, uae_u16 word2, int hpos, int vpos);
extern void record_copper_blitwait (uaecptr addr, int hpos, int vpos);
extern void record_copper_reset (void);
extern int mmu_init (int, uaecptr,uaecptr);
extern void mmu_do_hit (void);
extern void dump_aga_custom (void);
extern void memory_map_dump (void);
extern void debug_help (void);
extern uaecptr dumpmem2 (uaecptr addr, TCHAR *out, int osize);
extern void update_debug_info (void);
extern int instruction_breakpoint (TCHAR **c);
extern int debug_bankchange (int);
extern void log_dma_record (void);
extern void debug_parser (const TCHAR *cmd, TCHAR *out, uae_u32 outsize);
extern void mmu_disasm (uaecptr pc, int lines);
extern int debug_read_memory_16 (uaecptr addr);
extern int debug_peek_memory_16 (uaecptr addr);
extern int debug_read_memory_8 (uaecptr addr);
extern int debug_peek_memory_8 (uaecptr addr);
extern int debug_write_memory_16 (uaecptr addr, uae_u16 v);
extern int debug_write_memory_8 (uaecptr addr, uae_u8 v);
extern bool debug_enforcer(void);

#define BREAKPOINT_TOTAL 20
#define BREAKPOINT_REG_Dx 0
#define BREAKPOINT_REG_Ax 8
#define BREAKPOINT_REG_PC 16
#define BREAKPOINT_REG_USP 17
#define BREAKPOINT_REG_MSP 18
#define BREAKPOINT_REG_ISP 19
#define BREAKPOINT_REG_VBR 20
#define BREAKPOINT_REG_SR 21
#define BREAKPOINT_REG_CCR 22
#define BREAKPOINT_REG_CACR 23
#define BREAKPOINT_REG_CAAR 24
#define BREAKPOINT_REG_SFC 25
#define BREAKPOINT_REG_DFC 26
#define BREAKPOINT_REG_TC 27
#define BREAKPOINT_REG_ITT0 28
#define BREAKPOINT_REG_ITT1 29
#define BREAKPOINT_REG_DTT0 30
#define BREAKPOINT_REG_DTT1 31
#define BREAKPOINT_REG_BUSC 32
#define BREAKPOINT_REG_PCR 33
#define BREAKPOINT_REG_END 34

#define BREAKPOINT_CMP_EQUAL 0
#define BREAKPOINT_CMP_NEQUAL 1
#define BREAKPOINT_CMP_SMALLER_EQUAL 2
#define BREAKPOINT_CMP_LARGER_EQUAL 3
#define BREAKPOINT_CMP_SMALLER 2
#define BREAKPOINT_CMP_LARGER 3
#define BREAKPOINT_CMP_RANGE 4
#define BREAKPOINT_CMP_NRANGE 5

struct breakpoint_node {
	uae_u32 value1;
	uae_u32 value2;
	uae_u32 mask;
	int type;
	int oper;
    int enabled;
};
extern struct breakpoint_node bpnodes[BREAKPOINT_TOTAL];

#define MW_MASK_CPU_I			0x00000001
#define MW_MASK_CPU_D_R			0x00000002
#define MW_MASK_CPU_D_W			0x00000004
#define MW_MASK_BLITTER_A		0x00000008
#define MW_MASK_BLITTER_B		0x00000010
#define MW_MASK_BLITTER_C		0x00000020
#define MW_MASK_BLITTER_D_N		0x00000040
#define MW_MASK_BLITTER_D_L		0x00000080
#define MW_MASK_BLITTER_D_F		0x00000100
#define MW_MASK_COPPER			0x00000200
#define MW_MASK_DISK			0x00000400
#define MW_MASK_AUDIO_0			0x00000800
#define MW_MASK_AUDIO_1			0x00001000
#define MW_MASK_AUDIO_2			0x00002000
#define MW_MASK_AUDIO_3			0x00004000
#define MW_MASK_BPL_0			0x00008000
#define MW_MASK_BPL_1			0x00010000
#define MW_MASK_BPL_2			0x00020000
#define MW_MASK_BPL_3			0x00040000
#define MW_MASK_BPL_4			0x00080000
#define MW_MASK_BPL_5			0x00100000
#define MW_MASK_BPL_6			0x00200000
#define MW_MASK_BPL_7			0x00400000
#define MW_MASK_SPR_0			0x00800000
#define MW_MASK_SPR_1			0x01000000
#define MW_MASK_SPR_2			0x02000000
#define MW_MASK_SPR_3			0x04000000
#define MW_MASK_SPR_4			0x08000000
#define MW_MASK_SPR_5			0x10000000
#define MW_MASK_SPR_6			0x20000000
#define MW_MASK_SPR_7			0x40000000
#define MW_MASK_NONE			0x80000000
#define MW_MASK_ALL				(MW_MASK_NONE - 1)

#define MEMWATCH_TOTAL 20
struct memwatch_node {
	uaecptr addr;
	int size;
	int rwi;
	uae_u32 val, val_mask, access_mask;
	int val_size, val_enabled;
	int mustchange;
	uae_u32 modval;
	int modval_written;
	int frozen;
	uae_u32 reg;
	uaecptr pc;
};
extern struct memwatch_node mwnodes[MEMWATCH_TOTAL];

extern void memwatch_dump2 (TCHAR *buf, int bufsize, int num);

uae_u16 debug_wgetpeekdma_chipram (uaecptr addr, uae_u32 v, uae_u32 mask, int reg);
uae_u16 debug_wputpeekdma_chipram (uaecptr addr, uae_u32 v, uae_u32 mask, int reg);
uae_u16 debug_wputpeekdma_chipset (uaecptr addr, uae_u32 v, uae_u32 mask, int reg);
void debug_lgetpeek (uaecptr addr, uae_u32 v);
void debug_wgetpeek (uaecptr addr, uae_u32 v);
void debug_bgetpeek (uaecptr addr, uae_u32 v);
void debug_bputpeek (uaecptr addr, uae_u32 v);
void debug_wputpeek (uaecptr addr, uae_u32 v);
void debug_lputpeek (uaecptr addr, uae_u32 v);

uae_u32 get_byte_debug (uaecptr addr);
uae_u32 get_word_debug (uaecptr addr);
uae_u32 get_long_debug (uaecptr addr);
uae_u32 get_ilong_debug (uaecptr addr);
uae_u32 get_iword_debug (uaecptr addr);


enum debugtest_item { DEBUGTEST_BLITTER, DEBUGTEST_KEYBOARD, DEBUGTEST_FLOPPY, DEBUGTEST_MAX };
void debugtest (enum debugtest_item, const TCHAR *, ...);

struct dma_rec
{
    uae_u16 reg;
    uae_u32 dat;
    uae_u32 addr;
    uae_u16 evt;
    int type;
	uae_s8 intlev;
};

#define DMA_EVENT_BLITIRQ 1
#define DMA_EVENT_BLITNASTY 2
#define DMA_EVENT_BLITSTARTFINISH 4
#define DMA_EVENT_BPLFETCHUPDATE 8
#define DMA_EVENT_COPPERWAKE 16
#define DMA_EVENT_CPUIRQ 32
#define DMA_EVENT_INTREQ 64
#define DMA_EVENT_COPPERWANTED 128
#define DMA_EVENT_SPECIAL 256

#define DMARECORD_REFRESH 1
#define DMARECORD_CPU_I 2
#define DMARECORD_CPU_D 3
#define DMARECORD_COPPER 4
#define DMARECORD_AUDIO 5
#define DMARECORD_BLITTER 6
#define DMARECORD_BLITTER_FILL 7
#define DMARECORD_BLITTER_LINE 8
#define DMARECORD_BITPLANE 9
#define DMARECORD_SPRITE 10
#define DMARECORD_DISK 11
#define DMARECORD_MAX 12

extern struct dma_rec *record_dma(uae_u16 reg, uae_u16 dat, uae_u32 addr, int hpos, int vpos, int type);
extern void record_dma_reset(void);
extern void record_dma_event(int evt, int hpos, int vpos);
extern void debug_draw(uae_u8 *buf, int bpp, int line, int width, int height, uae_u32 *xredcolors, uae_u32 *xgreencolors, uae_u32 *xbluescolors);

#else

STATIC_INLINE void activate_debugger (void) { };

#endif /* DEBUGGER */

#endif /* UAE_DEBUG_H */
