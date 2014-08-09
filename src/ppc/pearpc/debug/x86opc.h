/* 
 *	HT Editor
 *	x86opc.h
 *
 *	Copyright (C) 1999-2002 Stefan Weyergraf
 *	Copyright (C) 2005-2006 Sebastian Biallas (sb@biallas.net)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __X86OPC_H__
#define __X86OPC_H__

#include "system/types.h"

#define X86_PREFIX_NO		-1

#define X86_PREFIX_LOCK		0	/* f0 */

#define X86_PREFIX_ES		0	/* 26 */
#define X86_PREFIX_CS		1	/* 2e */
#define X86_PREFIX_SS		2	/* 36 */
#define X86_PREFIX_DS		3	/* 3e */
#define X86_PREFIX_FS		4	/* 64 */
#define X86_PREFIX_GS		5	/* 65 */

#define X86_PREFIX_REPNZ	0	/* f2 */
#define X86_PREFIX_REPZ		1	/* f3 */

#define X86_PREFIX_OPSIZE	0	/* 66 */

enum X86OpSize {
	X86_OPSIZEUNKNOWN = -1,
	X86_OPSIZE16 = 0,
	X86_OPSIZE32 = 1,
	X86_OPSIZE64 = 2,
};

enum X86AddrSize {
	X86_ADDRSIZEUNKNOWN = -1,
	X86_ADDRSIZE16 = 0,
	X86_ADDRSIZE32 = 1,
	X86_ADDRSIZE64 = 2,
};

#define X86_OPTYPE_EMPTY	0
#define X86_OPTYPE_IMM		1
#define X86_OPTYPE_REG		2
#define X86_OPTYPE_SEG		3
#define X86_OPTYPE_MEM		4
#define X86_OPTYPE_CRX		5
#define X86_OPTYPE_DRX		6
#define X86_OPTYPE_TRX		7
#define X86_OPTYPE_STX		8
#define X86_OPTYPE_MMX		9
#define X86_OPTYPE_XMM		10
#define X86_OPTYPE_FARPTR	11

// user defined types start here
#define X86_OPTYPE_USER		32

struct x86_insn_op {
	int type;
	int size;
	bool need_rex;
	bool forbid_rex;
	union {
		struct {
			uint32 seg;
			uint32 offset;
		} farptr;			
		uint64 imm;
		int reg;
		int seg;
		struct {
			uint64 disp;
			int base;
			int index;
			int scale;
			int addrsize;
			bool floatptr;
			bool addrptr;
			bool hasdisp;
		} mem;
		int crx;
		int drx;
		int trx;
		int stx;
		int mmx;
		int xmm;
		union {
			int i;
			void *p;
		} user[4];
	};
};

#define TYPE_0		0
#define TYPE_A		1		/* direct address without ModR/M (generally
					   like imm, but can be 16:32 = 48 bit) */
#define TYPE_C		2		/* reg of ModR/M picks control register */
#define TYPE_D		3		/* reg of ModR/M picks debug register */
#define TYPE_E		4		/* ModR/M (general reg or memory) */
#define TYPE_G		5		/* reg of ModR/M picks general register */
#define TYPE_Is		6		/* signed immediate */
#define TYPE_I		7		/* unsigned immediate */
#define TYPE_Ix		8		/* fixed immediate */
#define TYPE_J		9		/* relative branch offset */
#define TYPE_M		10		/* ModR/M (memory only) */
#define TYPE_O		11		/* direct memory without ModR/M */
#define TYPE_P		12		/* reg of ModR/M picks MMX register */
#define TYPE_PR		13		/* rm of ModR/M picks MMX register */
#define TYPE_Q		14		/* ModR/M (MMX reg or memory) */
#define TYPE_R		15		/* rm of ModR/M picks general register */
#define TYPE_Rx		16		/* extra picks register */
#define TYPE_RXx	17		/* extra picks register, no REX extension */
#define TYPE_S		18		/* reg of ModR/M picks segment register */
#define TYPE_Sx		19		/* extra picks segment register */
#define TYPE_T		20		/* reg of ModR/M picks test register */
#define TYPE_V		21		/* reg of ModR/M picks XMM register */
#define TYPE_VR		22		/* reg of ModR/M picks XMM register */
#define TYPE_W		23		/* ModR/M (XMM reg or memory) */
#define TYPE_F		24		/* r/m of ModR/M picks a fpu register */
#define TYPE_Fx		25		/* extra picks a fpu register */

/* when name is == 0, the first op has a special meaning (layout see x86_insn_op_special) */
#define SPECIAL_TYPE_INVALID		0
#define SPECIAL_TYPE_PREFIX 		1
#define SPECIAL_TYPE_OPC_GROUP 		2
#define SPECIAL_TYPE_GROUP 		3
#define SPECIAL_TYPE_SGROUP 		4
#define SPECIAL_TYPE_FGROUP 		5

#define SIZE_0			'0'		/* size unimportant */
#define SIZE_B			'b'		/* byte */
#define SIZE_BV			'B'		/* byte, extended to SIZE_V */
#define SIZE_W			'w'		/* word */
#define SIZE_D			'd'		/* dword */
#define SIZE_Q			'q'		/* qword */
#define SIZE_U			'u'		/* qword OR oword (depending on 0x66 prefix) */
#define SIZE_Z			'z'		/* dword OR qword (depending on 0x66 prefix) */
#define SIZE_O			'o'		/* oword */
#define SIZE_V			'v'		/* word OR dword OR qword */
#define SIZE_VV			'V'		/* word OR dword OR sign extended dword */
#define SIZE_R			'r'		/* dword OR qword (depending on rex size) */
#define SIZE_P			'p'		/* word:word OR word:dword, memory only! */
#define SIZE_S			's'		/* short/single real (32-bit) */
#define SIZE_L			'l'		/* long/double real (64-bit) */
#define SIZE_T			't'		/* temp/extended real (80-bit) */
#define SIZE_A			'a'		/* packed decimal (80-bit BCD) */

#define INFO_PREFIX_66		0x66
#define INFO_DEFAULT_64		0x80

struct x86opc_insn_op {
	byte type;
	byte extra;
	byte info;
	byte size;
};

struct x86opc_insn_op_special {
	byte type;
	byte data;
	byte res1;
	byte res2;
};

struct x86opc_insn {
	const char *name;
	x86opc_insn_op op[3];
};

struct x86_64_insn_patch {
	int opc;
	x86opc_insn insn;
};

/* this can be a group (group!=0), an insn (group==0) && (insn.name!=0) or
   (otherwise) a reserved instruction. */
struct x86opc_finsn {
	x86opc_insn *group;	
	x86opc_insn insn;
};

#define X86_REG_INVALID		-2
#define X86_REG_NO		-1
#define X86_REG_AX		0
#define X86_REG_CX		1
#define X86_REG_DX		2
#define X86_REG_BX		3
#define X86_REG_SP		4
#define X86_REG_BP		5
#define X86_REG_SI		6
#define X86_REG_DI		7
#define X86_REG_R8		8
#define X86_REG_R9		9
#define X86_REG_R10		10
#define X86_REG_R11		11
#define X86_REG_R12		12
#define X86_REG_R13		13
#define X86_REG_R14		14
#define X86_REG_R15		15
#define X86_REG_IP		66

#define X86_OPC_GROUPS		2
#define X86_GROUPS		26
#define X86_SPECIAL_GROUPS	7

extern const char *x86_regs[4][8];
extern const char *x86_64regs[4][16];
extern const char *x86_ipregs[4];
extern const char *x86_segs[8];
extern x86opc_insn x86_32_insns[256];
extern x86_64_insn_patch x86_64_insn_patches[];
extern x86opc_insn x86_32_insns_ext[256];
extern x86opc_insn x86_insns_ext_f2[256];
extern x86opc_insn x86_insns_ext_f3[256];
extern x86opc_insn x86_opc_group_insns[X86_OPC_GROUPS][256];
extern x86opc_insn x86_32_group_insns[X86_GROUPS][8];
extern x86opc_insn x86_special_group_insns[X86_SPECIAL_GROUPS][9];

extern x86opc_insn x86_modfloat_group_insns[8][8];
extern x86opc_finsn x86_float_group_insns[8][8];

#endif /* __X86OPC_H__ */
