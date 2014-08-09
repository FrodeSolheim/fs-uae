/*
 *	HT Editor
 *	debugger.cc
 *
 *	Copyright (C) 2003 Stefan Weyergraf
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

#include <stdio.h>

#include "system/display.h"
#include "tools/except.h"
#include "tools/snprintf.h"
#include "tools/debug.h"
#include "debugger.h"
#include "parsehelper.h"
#include "stdfuncs.h"
#include "cpu/cpu.h"

#include "debug/ppcdis.h"

#ifdef HAVE_DEBUGGER
#include "cpu/cpu_generic/ppc_mmu.h"
#endif

/*
 *	A function
 */
SInt64 *Function::evalInteger() const
{
	EvalType type = getReturnType();
	switch (type) {
		case ET_INTEGER: throw NotImplementedException(HERE);
		case ET_FLOAT: {
			Float *f = evalFloat();
			SInt64 *i = new SInt64((sint64)f->value);
			delete f;
			return i;
			break;
		}
		case ET_STRING: {
			String *s = evalString();
			uint64 u;
			SInt64 *i;
                        if (s->toInt64(u, 10)) {
				i = new SInt64(u);
			} else {
				i = new SInt64(0);
			}
			delete s;
			return i;
			break;
		}
		case ET_VARARGS: /* may-not-happen */
			ASSERT(0);
			break;
	}
	return NULL;
}

Float *Function::evalFloat() const
{
	EvalType type = getReturnType();
	switch (type) {
		case ET_INTEGER: {
			SInt64 *u = evalInteger();
			Float *f = new Float(u->value);
			delete u;
			return f;
		}
		case ET_FLOAT: throw NotImplementedException(HERE);
		case ET_STRING: {
			throw NotImplementedException(HERE);
			// but it should be
			break;
		}
		case ET_VARARGS: /* may-not-happen */
			ASSERT(0);
			break;
	}
	return NULL;
}

String *Function::evalString() const
{
	EvalType type = getReturnType();
	switch (type) {
		case ET_INTEGER: {
			SInt64 *u = evalInteger();
			String *s = new String();
			s->assignFormat("%qd", u->value);
			delete u;
			return s;
		}
		case ET_FLOAT: {
			Float *f = evalFloat();
			String *s = new String();
			s->assignFormat("%f", f->value);
			delete f;
			return s;
		}
		case ET_STRING: throw NotImplementedException(HERE);
		case ET_VARARGS: /* may-not-happen */
			ASSERT(0);
			break;
	}
	return NULL;
}

uint Function::getArgCount() const
{
	return 0;
}

Function *Function::getArg(uint i) const
{
	throw IllegalArgumentException(HERE);
}

void Function::setArg(uint i, Function *f)
{
	throw IllegalArgumentException(HERE);
}

/*
 *	A constant integer function
 */
SInt64Function::SInt64Function(sint64 v)
{
	value = v;
}

EvalType SInt64Function::getReturnType() const
{
	return ET_INTEGER;
}

SInt64 *SInt64Function::evalInteger() const
{
	return new SInt64(value);
}

/*
 *	A GPR
 */
GPRFunction::GPRFunction(int number)
{
	mNumber = number;
}

EvalType GPRFunction::getReturnType() const
{
	return ET_INTEGER;
}

SInt64 *GPRFunction::evalInteger() const
{
#ifdef HAVE_DEBUGGER
	return new SInt64(gCPU.gpr[mNumber]);
#else
	return NULL;
#endif
}

FPRFunction::FPRFunction(int number)
{
	mNumber = number;
}

EvalType FPRFunction::getReturnType() const
{
	return ET_FLOAT;
}

Float *FPRFunction::evalFloat() const
{
#ifdef HAVE_DEBUGGER
	double_uint64 du = { gCPU.fpr[mNumber] };
	return new Float(du.d);
#else
	return NULL;
#endif
}

UInt32PFunction::UInt32PFunction(uint32 *aValue)
{
	mValue = aValue;
}

EvalType UInt32PFunction::getReturnType() const
{
	return ET_INTEGER;	
}

SInt64 *UInt32PFunction::evalInteger() const
{
	return new SInt64(*mValue);
}

/*
 *	A constant float function
 */
FloatFunction::FloatFunction(double v)
{
	value = v;
}

EvalType FloatFunction::getReturnType() const
{
	return ET_FLOAT;
}

Float *FloatFunction::evalFloat() const
{
	return new Float(value);
}

/*
 *	A constant string function
 */
StringFunction::StringFunction(const byte *str, int len)
: value(str, len)
{
}

EvalType StringFunction::getReturnType() const
{
	return ET_STRING;
}

String *StringFunction::evalString() const
{
	return value.clone();
}

/**
 *	A parametrized function (implementation helper)
 */

PFunction::PFunction()
: mArgs(true)
{
}

uint PFunction::getArgCount() const
{
	return mArgs.count();
}

Function *PFunction::getArg(uint i) const
{
	Function *r = (Function*)mArgs[i];
	if (!r) throw IllegalArgumentException(HERE);
	return r;
}

void PFunction::setArg(uint i, Function *f)
{
	mArgs.forceSetByIdx(i, f);
}

/*
 *	An expression parser
 */

enum protoMatchQuality {
	PMQ_EXACT,
	PMQ_OK,
	PMQ_NOT_OK
};

static bool convertsInto(EvalType from, EvalType to)
{
	switch (from) {
		case ET_INTEGER:
			return true;
		case ET_FLOAT:
			return (to != ET_INTEGER);
		case ET_STRING:
			return (to == ET_STRING);
		case ET_VARARGS:
			/* may-not-happen */
			break;
	}
	return false;
}

static protoMatchQuality matchFunctionPrototype(const Enumerator &params, uint count, EvalType *decl)
{
	if (params.count() < count) return PMQ_NOT_OK;
	uint pcount = params.count();
	if (count && (decl[count-1] == ET_VARARGS)) {
		count--;
		pcount = count;
	}
	if (count == pcount) {
		protoMatchQuality r = PMQ_EXACT;
		for (uint i=0; i<count; i++) {
			Function *f = (Function*)params[i];
			if (!convertsInto(f->getReturnType(), decl[i])) return PMQ_NOT_OK;
			if (f->getReturnType() != decl[i]) r = PMQ_OK;
		}
		return r;
	}
	return PMQ_NOT_OK;
}

static Function *matchFunctionByDesc(const String &name, const Enumerator &params, FunctionDesc *descs)
{
	protoMatchQuality bestPMQ = PMQ_NOT_OK;
	FunctionDesc *bestMatch = NULL;
	while (descs->name) {
		if (strcmp(descs->name, name.contentChar()) == 0) {
			protoMatchQuality pmq = matchFunctionPrototype(params, descs->declparam_count, descs->declparam);
			if (pmq == PMQ_EXACT) {
				// if its exact take it immediately
				bestPMQ = PMQ_EXACT;
				bestMatch = descs;
				break;
			} else if ((bestPMQ == PMQ_NOT_OK) && (pmq == PMQ_OK)) {
				// try to take the first thats ok
				bestPMQ = PMQ_OK;
				bestMatch = descs;
			}
		}
		descs++;
	}
	if (bestPMQ != PMQ_NOT_OK) {
		Function *r = bestMatch->creator();
		for (uint i=0; i<params.count(); i++) {
			r->setArg(i, (Function*)params[i]);
		}
		return r;
	}
	return NULL;
}

Watch::Watch(String *aName, Function *aFunc)
{
	mName = aName->clone();
	f = aFunc;
}

Watch::~Watch()
{
	delete mName;
	delete f;
}

int Watch::toString(char *buf, int buflen) const
{
	SInt64 *sint = f->evalInteger();
	String *s = f->evalString();
	int ret = ht_snprintf(buf, buflen, "watch: '%y' = %y (0x%08x)", mName, s, (uint32)sint->value);
	delete sint;
	delete s;
	return ret;
}

Debugger::Debugger()
{
	mWatches = new LinkedList(true);
#ifdef HAVE_DEBUGGER
	savedCPUState = gCPU;
#endif
	mUseColors = true;
}

Debugger::~Debugger()
{
	delete mWatches;
}

Function *Debugger::eval_scalarToFunction(eval_scalar &s)
{
	switch (s.type) {
	case SCALAR_INT:
		return new SInt64Function(s.scalar.integer.value);
	case SCALAR_FLOAT:
		return new FloatFunction(s.scalar.floatnum.value);
	case SCALAR_REG:
		switch (s.scalar.reg.type) {
		case REG_GPR:
			return new GPRFunction(s.scalar.reg.num);
		case REG_FPR:
			return new FPRFunction(s.scalar.reg.num);
#ifdef HAVE_DEBUGGER
		case REG_PC:
			return new UInt32PFunction(&gCPU.pc);
		case REG_LR:
			return new UInt32PFunction(&gCPU.lr);
		case REG_CR:
			return new UInt32PFunction(&gCPU.cr);
		case REG_CTR:
			return new UInt32PFunction(&gCPU.ctr);
		case REG_XER:
			return new UInt32PFunction(&gCPU.xer);
		case REG_MSR:
			return new UInt32PFunction(&gCPU.msr);
		case REG_SRR0:
			return new UInt32PFunction(&gCPU.srr[0]);
		case REG_SRR1:
			return new UInt32PFunction(&gCPU.srr[1]);
#endif
		}
		break;
	case SCALAR_STR:
		return new StringFunction((byte*)s.scalar.str.value, s.scalar.str.len);
	case SCALAR_FUNCTION: {
		Array p(false);
		for (int i=0; i<s.scalar.function.param_count; i++) {
			p += eval_scalarToFunction(s.scalar.function.params[i]);
		}
		return matchFunction(s.scalar.function.name, p);
	}
	case SCALAR_EMPTY:
		return NULL;
	case SCALAR_ANY:
	case SCALAR_VARARGS:
		/* may-not-happen */
		break;
	}
	/* may-not-happen */
	ASSERT(0);
	return NULL;
}

Function *Debugger::matchFunction(const String &name, const Enumerator &params)
{
	Function *r = matchFunctionByDesc(name, params, gStdEvalFunctions);
	if (!r) {
		String msg;
		msg.assignFormat("parse/eval: no function matching signature '%y(", &name);
		for (uint i=0; i<params.count(); i++) {
			Function *f = (Function*)params[i];
			switch (f->getReturnType()) {
				case ET_INTEGER:
					msg.append("integer");
					break;
				case ET_FLOAT:
					msg.append("float");
					break;
				case ET_STRING:
					msg.append("string");
					break;
				case ET_VARARGS:
					/* may-not-happen */
					ASSERT(0);
					break;
			}
			if (i+1<params.count()) msg.append(",");
		}
		msg.append(")'");
		throw MsgException(msg.contentChar());
	}
	return r;
}

inline static void disasm(uint32 code, uint32 ea, char *result)
{
	PPCDisassembler dis(PPC_MODE_32);
	CPU_ADDR addr;
	addr.addr32.offset = ea;
	strcpy(result, dis.str(dis.decode((byte*)&code, 4, addr), 0));
}

void Debugger::dump()
{
#ifdef HAVE_DEBUGGER
	int r = 0;
	const char *hiColor, *loColor;
	if (mUseColors) {
		hiColor = "\e[33;1m";
		loColor = "\e[37;1m";
	} else {
		hiColor = "";
		loColor = "";
	}
	for (int i=0; i<8; i++) {
		for (int j=0; j<4; j++) {
			ht_printf("r%02d: %s%08x%s ", r, (savedCPUState.gpr[r] != gCPU.gpr[r])?hiColor:loColor , gCPU.gpr[r], loColor);
			r++;
		}
		ht_printf("\n");
	}
/*	ht_printf("dbatu0: %08x dbatl0: %08x\n", gCPU.dbatu[0], gCPU.dbatl[0]);
	ht_printf("dbatu1: %08x dbatl1: %08x\n", gCPU.dbatu[1], gCPU.dbatl[1]);
	ht_printf("dbatu2: %08x dbatl2: %08x\n", gCPU.dbatu[2], gCPU.dbatl[2]);
	ht_printf("dbatu3: %08x dbatl3: %08x\n", gCPU.dbatu[3], gCPU.dbatl[3]);*/
	ht_printf("cr:  %s%08x%s xer: %s%08x%s lr:  %s%08x%s ctr: %s%08x%s\n", 
	(savedCPUState.cr != gCPU.cr)?hiColor:loColor, gCPU.cr, loColor,
	(savedCPUState.xer != gCPU.xer)?hiColor:loColor, gCPU.xer, loColor,
	(savedCPUState.lr != gCPU.lr)?hiColor:loColor, gCPU.lr, loColor,
	(savedCPUState.ctr != gCPU.ctr)?hiColor:loColor, gCPU.ctr, loColor);
//	ht_printf("msr: %08x sv:    %d srr1: %08x\n", gCPU.msr, (gCPU.msr & MSR_PR) ? 0 : 1, gCPU.srr[1]);
		
	foreach(Watch, w, *mWatches, {
		ht_printf("%y\n", w);
	});

	char disstr[256];
	disasm(gCPU.current_opc, gCPU.pc, disstr);
	ht_printf("@%08x: %08x %s\n", gCPU.pc, gCPU.current_opc, disstr);
	
	if (disstr[0] == 'b') {
		int i = 1;
		while (disstr[i] && disstr[i] != ' ') i++;
		if (disstr[i-1] != 'l') {
			mForceSinglestep = true;
		}
	}
#endif
}

static void displayFPRs()
{
#if 0
	for (int i=0; i<32; i++) {
		ppc_double dd;
		ppc_fpu_unpack_double(dd, gCPU.fpr[i]);
		double d = ppc_fpu_get_double(dd);
		ht_printf("fr%02d: ", i);
		switch (dd.type) {
		case ppc_fpr_norm:
			ht_printf("%c%qb * 2^%d = %f", dd.s?'-':'+', &dd.m, dd.e, d);
			break;
		case ppc_fpr_zero:
			ht_printf("%c0.0", dd.s?'-':'+');
			break;
		case ppc_fpr_Inf:
			ht_printf("%cInf", dd.s?'-':'+');
			break;
		case ppc_fpr_NaN:		
			ht_printf("%cNaN", dd.s?'-':'+');
			break;
		}
		ht_printf("\n");
		ht_printf("   = %qx\n", &gCPU.fpr[i]);
	}
#endif
}

static bool translateAddress(uint32 ea, uint32 &pa, bool error)
{
#ifdef HAVE_DEBUGGER
	if (ppc_effective_to_physical(ea, PPC_MMU_READ|PPC_MMU_NO_EXC|PPC_MMU_SV, pa)) {
		if (error) ht_printf("cant translate effective address %08x\n", ea);
		return false;
	} else {
		return true;
	}
#endif
}

bool Debugger::parse(const String &str)
{
	eval_command s;
	bool ret = false;
	memset(&s, 0, sizeof s);
	if (!eval_parse(&s, str.contentChar())) {
		char *str2;
		int pos;
		if (!get_eval_error(&str2, &pos)) {
			str2 = "internal error! please report!";
			pos = 0;
		}
		throw MsgfException("error: %s at %d", str2, pos);
	} else {
		Function *params[3];
		for (int i=0; i<s.paramcount; i++) {
			params[i] = eval_scalarToFunction(s.param[i]);
		}
		switch (s.type) {
		case COMMAND_NOP: break;
		case COMMAND_PRINT: {
			String *s = params[0]->evalString();
			SInt64 *sint = params[0]->evalInteger();
			ht_printf("= %y (0x%08x)\n", s, sint->value);
			delete s;
			delete sint;
			break;
		}
		case COMMAND_REGS:
			dump();
			break;
		case COMMAND_FLOATS: 
			displayFPRs();
			break;
		case COMMAND_SETREG: {
			SInt64 *sint = params[1]->evalInteger();
			switch (s.param[0].scalar.reg.type) {
#ifdef HAVE_DEBUGGER
			case REG_GPR:
				gCPU.gpr[s.param[0].scalar.reg.num] = sint->value;
				break;
			case REG_FPR: {
				Float *f = params[1]->evalFloat();
				double_uint64 du;
				du.d = f->value;
				gCPU.fpr[s.param[0].scalar.reg.num] = du.u;
				delete f;
				break;
			}
			case REG_PC:
				gCPU.pc = gCPU.npc = sint->value;
				break;
			case REG_CR:
				gCPU.cr = sint->value;
				break;
			case REG_LR:
				gCPU.lr = sint->value;
				break;
			case REG_CTR:
				gCPU.ctr = sint->value;
				break;
			case REG_XER:
				gCPU.xer = sint->value;
				break;
			case REG_MSR:
				gCPU.msr = sint->value;
				break;
			case REG_SRR0:
				gCPU.srr[0] = sint->value;
				break;
			case REG_SRR1:
				gCPU.srr[1] = sint->value;
				break;
#endif				
			}
			delete sint;
			break;
		}
#ifdef HAVE_DEBUGGER
		case COMMAND_LIST_BREAK: 
			ht_printf("Breakpoint %d at %08x\n", 1, gBreakpoint);
			ht_printf("Breakpoint %d at %08x\n", 2, gBreakpoint2);
			break;
		case COMMAND_BREAK: {
			SInt64 *sint = params[0]->evalInteger();
			gBreakpoint2 = sint->value;
			ht_printf("Breakpoint %d at %08x\n", 2, gBreakpoint2);
			break;
		}
		case COMMAND_STEP:
			ppc_set_singlestep_nonverbose(true);
			ret = true;
			break;
		case COMMAND_NEXT:
			gBreakpoint = gCPU.npc;
			if (mForceSinglestep) {
				ppc_set_singlestep_nonverbose(true);
			} else {
				gBreakpoint = gCPU.npc;
				ppc_set_singlestep_nonverbose(false);
			}
			ret = true;
			break;
		case COMMAND_CONTINUE:
			ppc_set_singlestep_nonverbose(false);
			ret = true;
			break;
		case COMMAND_QUIT:
			ppc_cpu_stop();
			ppc_set_singlestep_nonverbose(false);
			ret = true;
			break;
#endif
		case COMMAND_E2P: {
			SInt64 *sint = params[0]->evalInteger();
			uint32 pa, ea = sint->value;
			if (translateAddress(ea, pa, true)) {
				ht_printf("effective: %08x, physical: %08x\n", ea, pa);
			}
			delete sint;
			break;
		}
		case COMMAND_INSPECT_BYTE: {
			SInt64 *sint = params[0]->evalInteger();
			uint32 pa, ea = sint->value;
			uint8 v;
			if (translateAddress(ea, pa, true)) {
#ifdef HAVE_DEBUGGER
				if (ppc_read_physical_byte(pa, v)) {
					ht_printf("cant read at physical address %08x\n", pa);
				} else {
					ht_printf("@%08x: %02x   (physical: %08x)\n", ea, v, pa);
				}
#endif
			}
			delete sint;
			break;
		}
		case COMMAND_INSPECT_HALF: {
			SInt64 *sint = params[0]->evalInteger();
			uint32 pa, ea = sint->value;
			uint16 v;
			if (translateAddress(ea, pa, true)) {
#ifdef HAVE_DEBUGGER
				if (ppc_read_physical_half(pa, v)) { 
					ht_printf("cant read at physical address %08x\n", pa);
				} else {
					ht_printf("@%08x: %04x   (physical: %08x)\n", ea, v, pa);
				}
#endif				
			}
			delete sint;
			break;
		}
		case COMMAND_INSPECT_WORD: {
			SInt64 *sint = params[0]->evalInteger();
			uint32 pa, ea = sint->value;
			delete sint;
			uint32 v;
			if (translateAddress(ea, pa, true)) {
#ifdef HAVE_DEBUGGER
				if (ppc_read_physical_word(pa, v)) { 
					ht_printf("cant read at physical address %08x\n", pa);
				} else {
					ht_printf("@%08x: %08x   (physical: %08x)\n", ea, v, pa);
				}
#endif				
			}
			break;
		}
		case COMMAND_INSPECT_DWORD: {
			SInt64 *sint = params[0]->evalInteger();
			uint32 pa, ea = sint->value;
			delete sint;
			uint64 v;
			if (translateAddress(ea, pa, true)) {
#ifdef HAVE_DEBUGGER
				if (ppc_read_physical_dword(pa, v)) { 
					ht_printf("cant read at physical address %08x\n", pa);
				} else {
					ht_printf("@%08x: %016x   (physical: %08x)\n", ea, v, pa);
				}
#endif				
			}
			break;
		}
		case COMMAND_INSPECT_STRING: {
			SInt64 *sint = params[0]->evalInteger();
			uint32 pa, ea = sint->value;
			delete sint;
			byte *v;
			if (translateAddress(ea, pa, true)) {
#ifdef HAVE_DEBUGGER
				if (ppc_direct_physical_memory_handle(pa, v)) { 
					ht_printf("cant read at physical address %08x\n", pa);
				} else {
					ht_printf("@%08x: '%s'   (physical: %08x)\n", ea, v, pa);
				}
#endif				
			}
			break;
		}
		case COMMAND_INSPECT_MEM: {
			SInt64 *sint = params[0]->evalInteger();
			uint32 pa, ea = sint->value;
			uint32 count = 0x100;
			delete sint;
			byte v;
			int x = 0;
			char buf[80];
			memset(buf, ' ', sizeof buf);
			buf[79] = 0;
			buf[sprintf(buf, "@%08x", ea)] = ' ';
			while (count) {
				if (translateAddress(ea, pa, true)) {
#ifdef HAVE_DEBUGGER
					if (ppc_read_physical_byte(pa, v)) { 
						ht_printf("cant read at physical address %08x\n", pa);
						break;
					} else {
//						ht_printf("@%08x: '%s'   (physical: %08x)\n", ea, v, pa);
						sprintf(buf+10+(x%16)*3, "%02x", v);
						buf[10+(x%16)*3+2] = ' ';
						sprintf(buf+10+16*3+x%16, "%c", (v>=32 && v<=127)?v:' ');
						buf[10+16*3+x%16+1] = ' ';
					}
#endif					
				} else {
					break;
				}
				count--;
				ea++;
				x++;
				if (x % 16 == 0) {
					ht_printf("%s\n", buf);
					memset(buf, ' ', sizeof buf);
		    			buf[79] = 0;
					buf[sprintf(buf, "@%08x", ea)] = ' ';
				}
			}
			if (x % 16) ht_printf("%s\n", buf);
			break;
		}
		case COMMAND_WATCH: {
			String *name;
			if (params[1]) {
				name = params[1]->evalString();
			} else {
				name = new String("noname");
			}
			mWatches->insert(new Watch(name, eval_scalarToFunction(s.param[0])));
			break;
		}
		case COMMAND_WATCH_BYTE:
		case COMMAND_WATCH_HALF:
		case COMMAND_WATCH_WORD:
		case COMMAND_WATCH_DWORD:
		case COMMAND_DELETE_WATCH:
		case COMMAND_DUMP:
		case COMMAND_DISASM: {
#ifdef HAVE_DEBUGGER
			uint32 ea, pa;
			if (params[0]) {
				SInt64 *sint;
				sint = params[0]->evalInteger();
				ea = sint->value & ~3;
				delete sint;
			} else {
				ea = gCPU.pc;
			}
			uint32 count;
			if (params[1]) {
				SInt64 *sint;
				sint = params[1]->evalInteger();
				count = sint->value;
				delete sint;
			} else {
				count = 16;
			}
			while (count) {
				uint32 opc;
				if (translateAddress(ea, pa, true)) {
					if (ppc_read_physical_word(pa, opc)) {
						ht_printf("cant read at physical address %08x\n", pa);
						break;
					}
				} else {
					break;
				}
				char disstr[256];
			    	disasm(opc, gCPU.pc, disstr);
				ht_printf("@%08x: %08x %s\n", ea, opc, disstr);
				count--;
				ea+=4;
			}
#endif			
			break;
		}
		case COMMAND_HELP:
			ht_printf("bist du jeck?\n");
			break;
		}
		for (int i=0; i<s.paramcount; i++) {
			delete params[i];
		}
	}
	return ret;
}

void Debugger::enter()
{
	char buf[1024];
	bool quit = false;
	if (mAlwaysShowRegs) dump();

	/*
	 *	If current instruction is a branch instruction
	 *	which doesnt set LR we force single step
	 */
	mForceSinglestep = false;
	char disstr[256];
#ifdef HAVE_DEBUGGER
	disasm(gCPU.current_opc, gCPU.pc, disstr);
#endif			
	if (disstr[0] == 'b') {
		int i = 1;
		while (disstr[i] && disstr[i] != ' ') i++;
		if (disstr[i-1] != 'l') {
			mForceSinglestep = true;
		}
	}
#ifdef HAVE_DEBUGGER
	gBreakpoint = 0;
#endif			
	
	while (!quit) {
		printf("> ");
		fgets(buf, sizeof buf, stdin);
		try {
			quit = parse(buf);
		} catch (const Exception &e) {
			String s;
			ht_printf("%y\n", &e.reason(s));
			continue;
		}
	}
#ifdef HAVE_DEBUGGER
	savedCPUState = gCPU; 
#endif
}

#include "configparser.h"
void debugger_init_config()
{
	gConfig->acceptConfigEntryString("debugger_exec", false);
}
