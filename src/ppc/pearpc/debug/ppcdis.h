/*
 *	HT Editor
 *	ppcdis.h
 *
 *	Copyright (C) 1999-2002 Sebastian Biallas (sb@biallas.net)
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

#ifndef __PPC_DIS_H__
#define __PPC_DIS_H__

#include "system/types.h"
#include "asm.h"
#include "ppcopc.h"

struct ppcdis_operand {
	int flags;
	const powerpc_operand *op;
	union {
		int reg;    // general register
		int freg;   // float register
		int creg;   // condition register
		int vreg;   // vector register
		uint64 imm;
		struct {
			uint64 disp;
			int gr;
		} mem;
		struct {
			uint64 mem;
		} abs;
		struct {
			uint64 mem;
		} rel;
	};
};

struct ppcdis_insn {
	bool			valid;
	int			size;
	const char *		name;
	uint32			data;
	int			ops;
	ppcdis_operand		op[8];
};

#define PPC_MODE_32 0
#define PPC_MODE_64 1

class PPCDisassembler: public Disassembler {
protected:
	char insnstr[256];
	ppcdis_insn insn;
	int mode;
public:
			PPCDisassembler(int mode);

	virtual	dis_insn	*decode(const byte *code, int maxlen, CPU_ADDR addr);
	virtual	dis_insn	*duplicateInsn(dis_insn *disasm_insn);
	virtual	void		getOpcodeMetrics(int &min_length, int &max_length, int &min_look_ahead, int &avg_look_ahead, int &addr_align);
	virtual	byte		getSize(dis_insn *disasm_insn);
	virtual	const char	*getName();
	virtual	const char	*str(dis_insn *disasm_insn, int style);
	virtual	const char	*strf(dis_insn *disasm_insn, int style, const char *format);
	virtual	bool		validInsn(dis_insn *disasm_insn);
};

#endif
