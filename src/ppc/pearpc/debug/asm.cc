/*
 *	HT Editor
 *	asm.cc
 *
 *	Copyright (C) 1999-2002 Stefan Weyergraf
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
#include <cstdio>
#include <stdarg.h>

#include "asm.h"
#include "tools/data.h"
#include "tools/atom.h"
#include "tools/debug.h"
#include "tools/snprintf.h"

/*
 *	CLASS disassembler
 */

Disassembler::Disassembler()
{
	disable_highlighting();
}

char* (*addr_sym_func)(CPU_ADDR addr, int *symstrlen, void *context) = NULL;
void* addr_sym_func_context = NULL;

dis_insn *Disassembler::createInvalidInsn()
{
	return NULL;
}

void Disassembler::hexd(char **s, int size, int options, uint32 imm)
{
	char ff[16];
	char *f = (char*)&ff;
	char *t = *s;
	*f++ = '%';
	if (imm >= 0 && imm <= 9) {
		*s += sprintf(*s, "%d", imm);
	} else if (options & DIS_STYLE_SIGNED) {
		if (!(options & DIS_STYLE_HEX_NOZEROPAD)) f += sprintf(f, "0%d", size);
		*f++ = 'd';
		*f = 0;
		*s += sprintf(*s, ff, imm);
	} else {
		if (options & DIS_STYLE_HEX_CSTYLE) *f++ = '#';
		if (!(options & DIS_STYLE_HEX_NOZEROPAD)) f += sprintf(f, "0%d", size);
		if (options & DIS_STYLE_HEX_UPPERCASE) *f++ = 'X'; else
			*f++ = 'x';
		if (options & DIS_STYLE_HEX_ASMSTYLE) *f++ = 'h';
		*f = 0;
		*s += sprintf(*s, ff, imm);
		if ((options & DIS_STYLE_HEX_NOZEROPAD) && (*t-'0'>9)) {
			memmove(t+1, t, strlen(t)+1);
			*t = '0';
			(*s)++;
		}
	}
}

void Disassembler::hexq(char **s, int size, int options, uint64 imm)
{
	char ff[32];
	char *f = (char*)&ff;
	char *t = *s;
	*f++ = '%';
	if (imm >= 0 && imm <= 9) {
		*s += ht_snprintf(*s, 32, "%qd", imm);
	} else if (options & DIS_STYLE_SIGNED) {
		if (!(options & DIS_STYLE_HEX_NOZEROPAD)) f += sprintf(f, "0%d", size);
		*f++ = 'q';
		*f++ = 'd';
		*f = 0;
		*s += ht_snprintf(*s, 32, ff, imm);
	} else {
		if (options & DIS_STYLE_HEX_CSTYLE) *f++ = '#';
		if (!(options & DIS_STYLE_HEX_NOZEROPAD)) f += sprintf(f, "0%d", size);
		if (options & DIS_STYLE_HEX_UPPERCASE) *f++ = 'X'; else
		*f++ = 'q';
		*f++ = 'x';
		if (options & DIS_STYLE_HEX_ASMSTYLE) *f++ = 'h';
		*f = 0;
		*s += ht_snprintf(*s, 32, ff, imm);
		if ((options & DIS_STYLE_HEX_NOZEROPAD) && (*t-'0'>9)) {
			memmove(t+1, t, strlen(t)+1);
			*t = '0';
			(*s)++;
		}
	}
}

bool Disassembler::selectNext(dis_insn *disasm_insn)
{
	return false;
}

const char *Disassembler::str(dis_insn *disasm_insn, int style)
{
	return strf(disasm_insn, style, DISASM_STRF_DEFAULT_FORMAT);
}

const char *Disassembler::get_cs(AsmSyntaxHighlightEnum style)
{
	const char *highlights[] = {
		ASM_SYNTAX_DEFAULT,
		ASM_SYNTAX_COMMENT,
		ASM_SYNTAX_NUMBER,
		ASM_SYNTAX_SYMBOL,
		ASM_SYNTAX_STRING
	};
	return (highlight) ? highlights[(int)style] : "";
}

void Disassembler::enable_highlighting()
{
	highlight = true;
}

void Disassembler::disable_highlighting()
{
	highlight = false;
}
