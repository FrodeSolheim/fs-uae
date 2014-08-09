/* 
 *	HT Editor
 *	x86dis.h
 *
 *	Copyright (C) 1999-2002 Stefan Weyergraf
 *	Copyright (C) 2005 Sebastian Biallas (sb@biallas.net)
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

#ifndef __X86DIS_H__
#define __X86DIS_H__

#include "asm.h"
#include "x86opc.h"

#define X86DIS_OPCODE_CLASS_STD		0		/* no prefix */
#define X86DIS_OPCODE_CLASS_EXT		1		/* 0F */
#define X86DIS_OPCODE_CLASS_EXT_F2	2		/* F2 0F */
#define X86DIS_OPCODE_CLASS_EXT_F3	3		/* F3 0F */
#define X86DIS_OPCODE_CLASS_EXTEXT	4		/* 0F 0F */

/* x86-specific styles */
#define X86DIS_STYLE_EXPLICIT_MEMSIZE	0x00000001	/* IF SET: mov word ptr [0000], ax 	ELSE: mov [0000], ax */
#define X86DIS_STYLE_OPTIMIZE_ADDR	0x00000002	/* IF SET: mov [eax*3], ax 		ELSE: mov [eax+eax*2+00000000], ax */

struct x86dis_insn {
	bool invalid;
	sint8 opsizeprefix;
	sint8 lockprefix;
	sint8 repprefix;
	sint8 segprefix;
	uint8 rexprefix;	
	int size;
	int opcode;
	int opcodeclass;
	X86OpSize eopsize;
	X86AddrSize eaddrsize;
	const char *name;
	x86_insn_op op[3];
};

/*
 *	CLASS x86dis
 */

class x86dis: public Disassembler {
public:
	X86OpSize opsize;
	X86AddrSize addrsize;

	x86opc_insn (*x86_insns)[256];
	x86opc_insn (*x86_insns_ext)[256];
	x86opc_insn (*x86_group_insns)[X86_GROUPS][8];

protected:
	x86dis_insn insn;
	char insnstr[256];
	const byte *codep, *ocodep;
	CPU_ADDR addr;
	byte c;
	int modrm;
	int sib;
	int maxlen;

/* new */
	virtual		void	checkInfo(x86opc_insn *xinsn);
			void	decode_insn(x86opc_insn *insn);
	virtual		void	decode_modrm(x86_insn_op *op, char size, bool allow_reg, bool allow_mem, bool mmx, bool xmm);
			void	decode_op(x86_insn_op *op, x86opc_insn_op *xop);
			void	decode_sib(x86_insn_op *op, int mod);
			int	esizeop(uint c);
			int	esizeop_ex(uint c);
			byte	getbyte();
			uint16	getword();
			uint32	getdword();
			uint64	getqword();
			int	getmodrm();
			int	getsib();
			void	invalidate();
			bool	isfloat(char c);
			bool	isaddr(char c);
	virtual		void	prefixes();
			int	special_param_ambiguity(x86dis_insn *disasm_insn);
			void	str_format(char **str, const char **format, char *p, char *n, char *op[3], int oplen[3], char stopchar, int print);
	virtual		void	str_op(char *opstr, int *opstrlen, x86dis_insn *insn, x86_insn_op *op, bool explicit_params);
			uint	mkmod(uint modrm);
			uint	mkreg(uint modrm);
			uint	mkindex(uint modrm);
			uint	mkrm(uint modrm);
	virtual		uint64	getoffset();
	virtual		void	filloffset(CPU_ADDR &addr, uint64 offset);
public:
				x86dis(X86OpSize opsize, X86AddrSize addrsize);

/* overwritten */
	virtual	dis_insn *	decode(const byte *code, int maxlen, CPU_ADDR addr);
	virtual	dis_insn *	duplicateInsn(dis_insn *disasm_insn);
	virtual	void		getOpcodeMetrics(int &min_length, int &max_length, int &min_look_ahead, int &avg_look_ahead, int &addr_align);
	virtual	const char *	getName();
	virtual	byte		getSize(dis_insn *disasm_insn);
	virtual const char *	str(dis_insn *disasm_insn, int options);
	virtual const char *	strf(dis_insn *disasm_insn, int options, const char *format);
	virtual bool		validInsn(dis_insn *disasm_insn);
};

class x86_64dis: public x86dis {
	static x86opc_insn (*x86_64_insns)[256];
	static x86opc_insn (*x86_64_insns_ext)[256];
	static x86opc_insn (*x86_64_group_insns)[X86_GROUPS][8];
public:	
				x86_64dis();
	virtual	void		checkInfo(x86opc_insn *xinsn);
	virtual	void		decode_modrm(x86_insn_op *op, char size, bool allow_reg, bool allow_mem, bool mmx, bool xmm);
	virtual	void		prefixes();
	virtual	uint64		getoffset();
	virtual	void		filloffset(CPU_ADDR &addr, uint64 offset);
	
			void	prepInsns();
};

#endif /* __X86DIS_H__ */
