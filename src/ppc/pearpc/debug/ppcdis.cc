/*
 *	HT Editor
 *	ppcdis.cc
 *
 *	Copyright (C) 1999-2002 Sebastian Biallas (sb@biallas.net)
 *	Copyright 1994 Free Software Foundation, Inc.
 *	Written by Ian Lance Taylor, Cygnus Support
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

#include <cstring>

#include "tools/endianess.h"
#include "tools/snprintf.h"
#include "ppcdis.h"
#include "ppcopc.h"

PPCDisassembler::PPCDisassembler(int aMode)
{
	mode = aMode;
}

dis_insn *PPCDisassembler::decode(const byte *code, int maxlen, CPU_ADDR addr)
{
	const struct powerpc_opcode *opcode;
	const struct powerpc_opcode *opcode_end;
	uint32 op;
	int dialect = -1;

	insn.data = createHostInt(code, 4, big_endian);
	
	if (maxlen<4) {
		insn.valid = false;
		insn.size = maxlen;
		return &insn;
	}

	insn.size = 4;

	/* Get the major opcode of the instruction.  */
	op = PPC_OP(insn.data);

	/* Find the first match in the opcode table.  We could speed this up
	   a bit by doing a binary search on the major opcode.  */
	opcode_end = powerpc_opcodes + powerpc_num_opcodes;
	
	for (opcode = powerpc_opcodes; opcode < opcode_end; opcode++) {
		uint32 table_op;
		const byte *opindex;
		const struct powerpc_operand *operand;
		bool invalid;

		table_op = PPC_OP (opcode->opcode);

		if ((insn.data & opcode->mask) != opcode->opcode || (opcode->flags & dialect) == 0) {
			continue;
		}

		/* Make two passes over the operands.  First see if any of them
		   have extraction functions, and, if they do, make sure the
		   instruction is valid.  */
		invalid = false;
		for (opindex = opcode->operands; *opindex != 0; opindex++) {
			operand = powerpc_operands + *opindex;
			if (operand->extract) (*operand->extract)(insn.data, &invalid);
		}
		if (invalid) continue;

		/* The instruction is valid.  */
		insn.name = opcode->name;

		/* Now extract and print the operands.  */
		int opidx = 0;
		for (opindex = opcode->operands; *opindex != 0; opindex++) {
			sint32 value;

			operand = powerpc_operands + *opindex;

			/* Operands that are marked FAKE are simply ignored.  We
			   already made sure that the extract function considered
			   the instruction to be valid.  */
			if ((operand->flags & PPC_OPERAND_FAKE) != 0) continue;

			insn.op[opidx].op = operand;
			insn.op[opidx].flags = operand->flags;
			/* Extract the value from the instruction.  */
			if (operand->extract) {
				value = (*operand->extract)(insn.data, NULL);
			} else {
				value = (insn.data >> operand->shift) & ((1 << operand->bits) - 1);
				if ((operand->flags & PPC_OPERAND_SIGNED) != 0 && (value & (1 << (operand->bits - 1))) != 0) {
					value -= 1 << operand->bits;
				}
			}

			/* If the operand is optional, and the value is zero, don't
			   print anything.  */
			if ((operand->flags & PPC_OPERAND_OPTIONAL) != 0 && (operand->flags & PPC_OPERAND_NEXT) == 0 && value == 0) {
				insn.op[opidx++].imm = 0;
				continue;
			}

			if (operand->flags & PPC_OPERAND_GPR_0) {
				if (value) {
					insn.op[opidx].flags |= PPC_OPERAND_GPR;
					insn.op[opidx++].reg = value;
				} else {
					insn.op[opidx].flags = 0;
					insn.op[opidx].imm = value;
				}
			} else if (operand->flags & PPC_OPERAND_GPR) {
				insn.op[opidx++].reg = value;
			} else if (operand->flags & PPC_OPERAND_FPR) {
				insn.op[opidx++].freg = value;
			} else if (operand->flags & PPC_OPERAND_VR) {
				insn.op[opidx++].vreg = value;
			} else if (operand->flags & PPC_OPERAND_RELATIVE) {
				if (mode == PPC_MODE_32) {
					insn.op[opidx++].rel.mem = addr.addr32.offset + value;
				} else {
					insn.op[opidx++].rel.mem = addr.flat64.addr + value;
				}
			} else if ((operand->flags & PPC_OPERAND_ABSOLUTE) != 0) {
				insn.op[opidx++].abs.mem = value;
			} else if ((operand->flags & PPC_OPERAND_CR) == 0 || (dialect & PPC_OPCODE_PPC) == 0) {
				insn.op[opidx++].imm = (sint64)value;
			} else {
				insn.op[opidx++].creg = value;			}

		}
		insn.ops = opidx;

		/* We have found and printed an instruction; return.  */
		insn.valid = true;
		return &insn;
	}

	insn.valid = false;
	return &insn;
}

dis_insn *PPCDisassembler::duplicateInsn(dis_insn *disasm_insn)
{
	ppcdis_insn *insn = ppc_malloc(sizeof (ppcdis_insn));
	*insn = *(ppcdis_insn *)disasm_insn;
	return insn;
}

void PPCDisassembler::getOpcodeMetrics(int &min_length, int &max_length, int &min_look_ahead, int &avg_look_ahead, int &addr_align)
{
	min_length = max_length = min_look_ahead = avg_look_ahead = addr_align = 4;
}

byte PPCDisassembler::getSize(dis_insn *disasm_insn)
{
	return ((ppcdis_insn*)disasm_insn)->size;
}

const char *PPCDisassembler::getName()
{
	return "PPC/Disassembler";
}

const char *PPCDisassembler::str(dis_insn *disasm_insn, int style)
{
	return strf(disasm_insn, style, "");
}

const char *PPCDisassembler::strf(dis_insn *disasm_insn, int style, const char *format)
{
	if (style & DIS_STYLE_HIGHLIGHT) enable_highlighting();
	
	const char *cs_default = get_cs(e_cs_default);
	const char *cs_number = get_cs(e_cs_number);
	const char *cs_symbol = get_cs(e_cs_symbol);

	ppcdis_insn *ppc_insn = (ppcdis_insn *) disasm_insn;
	if (!ppc_insn->valid) {
		switch (ppc_insn->size) {
			case 1:
				strcpy(insnstr, "db         ?");
				break;
			case 2:
				strcpy(insnstr, "dw         ?");
				break;
			case 3:
				strcpy(insnstr, "db         ? * 3");
				break;
			case 4:
				sprintf(insnstr, "dd         %s0x%08x", cs_number, ppc_insn->data);
				break;
			default: { /* braces for empty assert */
				strcpy(insnstr, "?");
//				assert(0);
			}
		}
	} else {
		char *is = insnstr+sprintf(insnstr, "%-10s", ppc_insn->name);
		int dialect=-1;

		bool need_comma = false;
		bool need_paren = false;
		for (int opidx = 0; opidx < ppc_insn->ops; opidx++) {
			int flags = ppc_insn->op[opidx].flags;
/*			if ((flags & PPC_OPERAND_OPTIONAL) != 0 && (flags & PPC_OPERAND_NEXT) == 0 && ppc_insn->op[opidx].imm == 0) {
				continue;
			}*/
			if (need_comma) {
				is += sprintf(is, "%s, ", cs_symbol);
				need_comma = false;
			}
			if (flags & PPC_OPERAND_GPR) {
				is += sprintf(is, "%sr%d", cs_default, ppc_insn->op[opidx].reg);
			} else if ((flags & PPC_OPERAND_FPR) != 0) {
				is += sprintf(is, "%sf%d", cs_default, ppc_insn->op[opidx].freg);
			} else if ((flags & PPC_OPERAND_VR) != 0) {
				is += sprintf(is, "%svr%d", cs_default, ppc_insn->op[opidx].vreg);
			} else if ((flags & PPC_OPERAND_RELATIVE) != 0) {
				CPU_ADDR caddr;
				if (mode == PPC_MODE_32) {
					caddr.addr32.offset = (uint32)ppc_insn->op[opidx].mem.disp;
				} else {
					caddr.flat64.addr = ppc_insn->op[opidx].mem.disp;
				}
				int slen;
				char *s = (addr_sym_func) ? addr_sym_func(caddr, &slen, addr_sym_func_context) : 0;
				if (s) {
					is += sprintf(is, "%s", cs_default);
					memcpy(is, s, slen);
					is[slen] = 0;
					is += slen;
				} else {
					is += ht_snprintf(is, 100, "%s0x%qx", cs_number, ppc_insn->op[opidx].rel.mem);
				}
			} else if ((flags & PPC_OPERAND_ABSOLUTE) != 0) {
				is += ht_snprintf(is, 100, "%s0x%qx", cs_number, ppc_insn->op[opidx].abs.mem);
			} else if ((flags & PPC_OPERAND_CR) == 0 || (dialect & PPC_OPCODE_PPC) == 0) {
				is += ht_snprintf(is, 100, "%s%qd", cs_number, ppc_insn->op[opidx].imm);
			} else if (ppc_insn->op[opidx].op->bits == 3) {
				is += sprintf(is, "%scr%d", cs_default, ppc_insn->op[opidx].creg);
			} else {
				static const char *cbnames[4] = { "lt", "gt", "eq", "so" };
				int cr;
				int cc;
				cr = ppc_insn->op[opidx].creg >> 2;
				if (cr != 0) is += sprintf(is, "%s4%s*%scr%d", cs_number, cs_symbol, cs_default, cr);
				cc = ppc_insn->op[opidx].creg & 3;
				if (cc != 0) {
					if (cr != 0) is += sprintf(is, "%s+", cs_symbol);
					is += sprintf(is, "%s%s", cs_default, cbnames[cc]);
				}
			}
		
			if (need_paren) {
				is += sprintf(is, "%s)", cs_symbol);
				need_paren = false;
			}

			if ((flags & PPC_OPERAND_PARENS) == 0) {
				need_comma = true;
			} else {
				is += sprintf(is, "%s(", cs_symbol);
				need_paren = true;
			}
		}
	}
	disable_highlighting();
	return insnstr;     
}

bool PPCDisassembler::validInsn(dis_insn *disasm_insn)
{
	return ((ppcdis_insn*)disasm_insn)->valid;
}


