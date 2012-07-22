#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"
#include "memory.h"
#include "custom.h"
#include "events.h"
#include "newcpu.h"
#include "cpu_prefetch.h"
#include "cputbl.h"
#include "cpummu.h"
#define CPUFUNC(x) x##_ff
#define SET_CFLG_ALWAYS(x) SET_CFLG(x)
#define SET_NFLG_ALWAYS(x) SET_NFLG(x)
#ifdef NOFLAGS
#include "noflags.h"
#endif

#if !defined(PART_1) && !defined(PART_2) && !defined(PART_3) && !defined(PART_4) && !defined(PART_5) && !defined(PART_6) && !defined(PART_7) && !defined(PART_8)
#define PART_1 1
#define PART_2 1
#define PART_3 1
#define PART_4 1
#define PART_5 1
#define PART_6 1
#define PART_7 1
#define PART_8 1
#endif

#ifdef PART_1
/* OR.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0000_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0010_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0018_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0020_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0028_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0030_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0038_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0039_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ORSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_003c_20)(uae_u32 opcode)
{
{	MakeSR ();
{	uae_s16 src = get_word_ce020_prefetch (2);
	src &= 0xFF;
	regs.sr |= src;
	MakeFromSR ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0040_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0050_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0058_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0060_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0068_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0070_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0078_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0079_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ORSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_007c_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5299; }
{	MakeSR ();
{	uae_s16 src = get_word_ce020_prefetch (2);
	regs.sr |= src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel5299: ;
return;
}

/* OR.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0080_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0090_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0098_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_00a0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_00a8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* OR.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_00b0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_00b8_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* OR.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_00b9_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CHK2.B #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00d0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce020 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce020 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5308; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel5308: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00e8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce020 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce020 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5309; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel5309: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00f0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce020 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce020 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5310; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5310: ;
return;
}

#endif
/* CHK2.B #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00f8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce020 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce020 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5311; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel5311: ;
return;
}

#endif
/* CHK2.B #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00f9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce020 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce020 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5312; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel5312: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce020 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce020 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5313; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel5313: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce020 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce020 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5314; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5314: ;
return;
}

#endif
/* BTST.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MVPMR.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_0108_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_u16 val = (get_byte_ce020 (memp) << 8) + get_byte_ce020 (memp + 2);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BTST.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BTST.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_0120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BTST.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_0128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_0138_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_0139_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_013a_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_013b_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B Dn,#<data>.B */
void REGPARAM2 CPUFUNC(op_013c_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = (uae_u8)get_word_ce020_prefetch (2);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= 31;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MVPMR.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_0148_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_u32 val = (get_byte_ce020 (memp) << 24) + (get_byte_ce020 (memp + 2) << 16)
              + (get_byte_ce020 (memp + 4) << 8) + get_byte_ce020 (memp + 6);
	m68k_dreg (regs, dstreg) = (val);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCHG.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCHG.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_0160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCHG.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_0168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCHG.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_0178_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_0179_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_017a_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_017b_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MVPRM.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_0188_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	uaecptr memp = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	put_byte_ce020 (memp, src >> 8); put_byte_ce020 (memp + 2, src);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCLR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCLR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_01a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCLR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_01a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_01b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_01b8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_01b9_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_01ba_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_01bb_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_01c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MVPRM.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_01c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	uaecptr memp = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	put_byte_ce020 (memp, src >> 24); put_byte_ce020 (memp + 2, src >> 16);
	put_byte_ce020 (memp + 4, src >> 8); put_byte_ce020 (memp + 6, src);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_01d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BSET.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_01d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BSET.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_01e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BSET.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_01e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_01f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_01f8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_01f9_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_01fa_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_01fb_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0200_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0210_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0218_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0220_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0228_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0230_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0238_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0239_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ANDSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_023c_20)(uae_u32 opcode)
{
{	MakeSR ();
{	uae_s16 src = get_word_ce020_prefetch (2);
	src |= 0xFF00;
	regs.sr &= src;
	MakeFromSR ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0240_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0250_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0258_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0260_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0268_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0270_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0278_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0279_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ANDSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_027c_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5377; }
{	MakeSR ();
{	uae_s16 src = get_word_ce020_prefetch (2);
	regs.sr &= src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel5377: ;
return;
}

/* AND.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0280_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0290_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0298_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_02a0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_02a8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* AND.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_02b0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_02b8_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* AND.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_02b9_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CHK2.W #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02d0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce020 (dsta); upper = (uae_s32)(uae_s16)get_word_ce020 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5386; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel5386: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02e8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce020 (dsta); upper = (uae_s32)(uae_s16)get_word_ce020 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5387; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel5387: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02f0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce020 (dsta); upper = (uae_s32)(uae_s16)get_word_ce020 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5388; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5388: ;
return;
}

#endif
/* CHK2.W #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02f8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce020 (dsta); upper = (uae_s32)(uae_s16)get_word_ce020 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5389; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel5389: ;
return;
}

#endif
/* CHK2.W #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02f9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce020 (dsta); upper = (uae_s32)(uae_s16)get_word_ce020 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5390; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel5390: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce020 (dsta); upper = (uae_s32)(uae_s16)get_word_ce020 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5391; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel5391: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce020 (dsta); upper = (uae_s32)(uae_s16)get_word_ce020 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5392; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5392: ;
return;
}

#endif
/* SUB.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0400_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0410_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0418_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0420_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0428_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0430_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0438_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0439_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0440_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0450_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0458_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0460_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0468_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0470_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0478_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0479_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0480_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0490_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0498_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_04a0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_04a8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_04b0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_04b8_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_04b9_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CHK2.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04d0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce020 (dsta); upper = get_long_ce020 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5417; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel5417: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04e8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce020 (dsta); upper = get_long_ce020 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5418; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel5418: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04f0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce020 (dsta); upper = get_long_ce020 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5419; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5419: ;
return;
}

#endif
/* CHK2.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04f8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce020 (dsta); upper = get_long_ce020 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5420; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel5420: ;
return;
}

#endif
/* CHK2.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04f9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce020 (dsta); upper = get_long_ce020 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5421; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel5421: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce020 (dsta); upper = get_long_ce020 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5422; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel5422: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce020 (dsta); upper = get_long_ce020 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel5423; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5423: ;
return;
}

#endif
/* ADD.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0600_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0610_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0618_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0620_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0628_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0630_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0638_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0639_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0640_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0650_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0658_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0660_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0668_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0670_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0678_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0679_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0680_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0690_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0698_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_06a0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_06a8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_06b0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_06b8_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_06b9_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* RTM.L Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* RTM.L An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06f8_20)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06f9_20)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06fa_20)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06fb_20)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BTST.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0800_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0810_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0818_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0820_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0828_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0830_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0838_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0839_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BTST.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_083a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_083b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B #<data>.W,#<data>.B */
void REGPARAM2 CPUFUNC(op_083c_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s8 dst = (uae_u8)get_word_ce020_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0840_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= 31;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0850_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0858_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0860_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0868_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0870_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCHG.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0878_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0879_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BCHG.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_087a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_087b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0880_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0890_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0898_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_08a0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_08a8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_08b0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_08b8_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_08b9_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BCLR.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_08ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_08bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_08c0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_08d0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_08d8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_08e0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_08e8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_08f0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_08f8_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_08f9_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BSET.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_08fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_08fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce020 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0a00_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0a10_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0a18_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0a20_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0a28_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0a30_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0a38_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0a39_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EORSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_0a3c_20)(uae_u32 opcode)
{
{	MakeSR ();
{	uae_s16 src = get_word_ce020_prefetch (2);
	src &= 0xFF;
	regs.sr ^= src;
	MakeFromSR ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0a40_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0a50_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0a58_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0a60_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0a68_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0a70_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0a78_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0a79_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EORSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_0a7c_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5515; }
{	MakeSR ();
{	uae_s16 src = get_word_ce020_prefetch (2);
	regs.sr ^= src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel5515: ;
return;
}

#endif

#ifdef PART_2
/* EOR.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0a80_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0a90_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0a98_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_0aa0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_0aa8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EOR.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0ab0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_0ab8_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EOR.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_0ab9_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CAS.B #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ad0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s8)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(m68k_dreg (regs, rc))) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_byte_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.B #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ad8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s8)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(m68k_dreg (regs, rc))) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_byte_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.B #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ae0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s8)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(m68k_dreg (regs, rc))) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_byte_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.B #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ae8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s8)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(m68k_dreg (regs, rc))) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_byte_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.B #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0af0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s8)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(m68k_dreg (regs, rc))) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_byte_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CAS.B #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0af8_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s8)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(m68k_dreg (regs, rc))) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_byte_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.B #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0af9_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s8)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(m68k_dreg (regs, rc))) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_byte_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* CMP.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0c00_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_0c10_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0c18_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0c20_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0c28_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0c30_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMP.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0c38_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0c39_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* CMP.B #<data>.B,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0c3a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CMP.B #<data>.B,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0c3b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CMP.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0c40_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0c50_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0c58_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0c60_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0c68_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0c70_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMP.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0c78_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0c79_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* CMP.W #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0c7a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CMP.W #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0c7b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CMP.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0c80_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0c90_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0c98_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_0ca0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_0ca8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* CMP.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0cb0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMP.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_0cb8_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* CMP.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_0cb9_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CMP.L #<data>.L,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 6;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* CMP.L #<data>.L,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cbb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (6);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CAS.W #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cd0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s16)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(m68k_dreg (regs, rc))) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_word_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.W #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cd8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s16)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(m68k_dreg (regs, rc))) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_word_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.W #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ce0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s16)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(m68k_dreg (regs, rc))) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_word_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.W #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ce8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s16)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(m68k_dreg (regs, rc))) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_word_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.W #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cf0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s16)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(m68k_dreg (regs, rc))) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_word_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CAS.W #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cf8_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s16)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(m68k_dreg (regs, rc))) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_word_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.W #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cf9_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = get_word_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s16)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(m68k_dreg (regs, rc))) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_word_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* CAS2.W #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cfc_20)(uae_u32 opcode)
{
{{	uae_s32 extra = get_long_ce020_prefetch (2);
	uae_u32 rn1 = regs.regs[(extra >> 28) & 15];
	uae_u32 rn2 = regs.regs[(extra >> 12) & 15];
	uae_u16 dst1 = get_word_ce020 (rn1), dst2 = get_word_ce020 (rn2);
{uae_u32 newv = ((uae_s16)(dst1)) - ((uae_s16)(m68k_dreg (regs, (extra >> 16) & 7)));
{	int flgs = ((uae_s16)(m68k_dreg (regs, (extra >> 16) & 7))) < 0;
	int flgo = ((uae_s16)(dst1)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(m68k_dreg (regs, (extra >> 16) & 7))) > ((uae_u16)(dst1)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()) {
{uae_u32 newv = ((uae_s16)(dst2)) - ((uae_s16)(m68k_dreg (regs, extra & 7)));
{	int flgs = ((uae_s16)(m68k_dreg (regs, extra & 7))) < 0;
	int flgo = ((uae_s16)(dst2)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(m68k_dreg (regs, extra & 7))) > ((uae_u16)(dst2)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()) {
	put_word_ce020 (rn1, m68k_dreg (regs, (extra >> 22) & 7));
	put_word_ce020 (rn1, m68k_dreg (regs, (extra >> 6) & 7));
	}}
}}}}	if (! GET_ZFLG ()) {
	m68k_dreg (regs, (extra >> 22) & 7) = (m68k_dreg (regs, (extra >> 22) & 7) & ~0xffff) | (dst1 & 0xffff);
	m68k_dreg (regs, (extra >> 6) & 7) = (m68k_dreg (regs, (extra >> 6) & 7) & ~0xffff) | (dst2 & 0xffff);
	}
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* MOVES.B #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e10_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5569; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	put_byte_ce020 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s8 src = get_byte_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5569: ;
return;
}

#endif
/* MOVES.B #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e18_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5570; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce020 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5570: ;
return;
}

#endif
/* MOVES.B #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e20_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5571; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce020 (dsta, src);
}}else{{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, dstreg) = srca;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5571: ;
return;
}

#endif
/* MOVES.B #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e28_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5572; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	put_byte_ce020 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 src = get_byte_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5572: ;
return;
}

#endif
/* MOVES.B #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e30_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5573; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	put_byte_ce020 (dsta, src);
}}}else{{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5573: ;
return;
}

#endif
/* MOVES.B #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e38_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5574; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	put_byte_ce020 (dsta, src);
}}else{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 src = get_byte_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5574: ;
return;
}

#endif
/* MOVES.B #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e39_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5575; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = get_long_ce020_prefetch (4);
	put_byte_ce020 (dsta, src);
}}else{{	uaecptr srca = get_long_ce020_prefetch (4);
{	uae_s8 src = get_byte_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (8);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5575: ;
return;
}

#endif
/* MOVES.W #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e50_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5576; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	put_word_ce020 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s16 src = get_word_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5576: ;
return;
}

#endif
/* MOVES.W #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e58_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5577; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	put_word_ce020 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, dstreg) += 2;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5577: ;
return;
}

#endif
/* MOVES.W #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e60_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5578; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	put_word_ce020 (dsta, src);
}}else{{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, dstreg) = srca;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5578: ;
return;
}

#endif
/* MOVES.W #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e68_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5579; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	put_word_ce020 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 src = get_word_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5579: ;
return;
}

#endif
/* MOVES.W #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e70_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5580; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	put_word_ce020 (dsta, src);
}}}else{{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5580: ;
return;
}

#endif
/* MOVES.W #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e78_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5581; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	put_word_ce020 (dsta, src);
}}else{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 src = get_word_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5581: ;
return;
}

#endif
/* MOVES.W #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e79_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5582; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = get_long_ce020_prefetch (4);
	put_word_ce020 (dsta, src);
}}else{{	uaecptr srca = get_long_ce020_prefetch (4);
{	uae_s16 src = get_word_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (8);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5582: ;
return;
}

#endif
/* MOVES.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e90_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5583; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	put_long_ce020 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s32 src = get_long_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5583: ;
return;
}

#endif
/* MOVES.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e98_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5584; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	put_long_ce020 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, dstreg) += 4;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5584: ;
return;
}

#endif
/* MOVES.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ea0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5585; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	put_long_ce020 (dsta, src);
}}else{{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, dstreg) = srca;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5585: ;
return;
}

#endif
/* MOVES.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ea8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5586; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	put_long_ce020 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 src = get_long_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5586: ;
return;
}

#endif
/* MOVES.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0eb0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel5587; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	put_long_ce020 (dsta, src);
}}}else{{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5587: ;
return;
}

#endif
/* MOVES.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0eb8_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5588; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	put_long_ce020 (dsta, src);
}}else{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 src = get_long_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5588: ;
return;
}

#endif
/* MOVES.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0eb9_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5589; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = get_long_ce020_prefetch (4);
	put_long_ce020 (dsta, src);
}}else{{	uaecptr srca = get_long_ce020_prefetch (4);
{	uae_s32 src = get_long_ce020 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (8);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5589: ;
return;
}

#endif
/* CAS.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ed0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s32)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(m68k_dreg (regs, rc))) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_long_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ed8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s32)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(m68k_dreg (regs, rc))) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_long_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ee0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s32)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(m68k_dreg (regs, rc))) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_long_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ee8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s32)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(m68k_dreg (regs, rc))) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_long_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ef0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s32)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(m68k_dreg (regs, rc))) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_long_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CAS.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ef8_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s32)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(m68k_dreg (regs, rc))) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_long_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ef9_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
{	int ru = (src >> 6) & 7;
	int rc = src & 7;
{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(m68k_dreg (regs, rc)));
{	int flgs = ((uae_s32)(m68k_dreg (regs, rc))) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(m68k_dreg (regs, rc))) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()){	put_long_ce020 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* CAS2.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0efc_20)(uae_u32 opcode)
{
{{	uae_s32 extra = get_long_ce020_prefetch (2);
	uae_u32 rn1 = regs.regs[(extra >> 28) & 15];
	uae_u32 rn2 = regs.regs[(extra >> 12) & 15];
	uae_u32 dst1 = get_long_ce020 (rn1), dst2 = get_long_ce020 (rn2);
{uae_u32 newv = ((uae_s32)(dst1)) - ((uae_s32)(m68k_dreg (regs, (extra >> 16) & 7)));
{	int flgs = ((uae_s32)(m68k_dreg (regs, (extra >> 16) & 7))) < 0;
	int flgo = ((uae_s32)(dst1)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(m68k_dreg (regs, (extra >> 16) & 7))) > ((uae_u32)(dst1)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()) {
{uae_u32 newv = ((uae_s32)(dst2)) - ((uae_s32)(m68k_dreg (regs, extra & 7)));
{	int flgs = ((uae_s32)(m68k_dreg (regs, extra & 7))) < 0;
	int flgo = ((uae_s32)(dst2)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(m68k_dreg (regs, extra & 7))) > ((uae_u32)(dst2)));
	SET_NFLG (flgn != 0);
	if (GET_ZFLG ()) {
	put_long_ce020 (rn1, m68k_dreg (regs, (extra >> 22) & 7));
	put_long_ce020 (rn1, m68k_dreg (regs, (extra >> 6) & 7));
	}}
}}}}	if (! GET_ZFLG ()) {
	m68k_dreg (regs, (extra >> 22) & 7) = dst1;
	m68k_dreg (regs, (extra >> 6) & 7) = dst2;
	}
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* MOVE.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_1000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),Dn */
void REGPARAM2 CPUFUNC(op_1010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_1018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_1020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_1028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_1030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_1038_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_1039_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_103a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_103b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_103c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_1080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(An) */
void REGPARAM2 CPUFUNC(op_1090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(An) */
void REGPARAM2 CPUFUNC(op_1098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(An) */
void REGPARAM2 CPUFUNC(op_10a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_10a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_10b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_10b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_10b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_10ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_10bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_10bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_10c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(An)+ */
void REGPARAM2 CPUFUNC(op_10d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_10d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_10e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_10e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_10f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_10f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_10f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_10fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_10fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_10fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_1100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),-(An) */
void REGPARAM2 CPUFUNC(op_1110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_1118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_1120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_1128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_1130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_1138_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_1139_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_113a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_113b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_113c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_1140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_1158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_1170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_1178_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_1179_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_117a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_117b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_117c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_11c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_11d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_11f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_11f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_11f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_11fa_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_11fb_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_11fc_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_13c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_13d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_13f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_13f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_13f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (10);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_13fa_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_13fb_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_13fc_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_2000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,Dn */
void REGPARAM2 CPUFUNC(op_2008_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),Dn */
void REGPARAM2 CPUFUNC(op_2010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_2018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_2020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_2028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_2030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_2038_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_2039_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_203a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_203b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_203c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L Dn,An */
void REGPARAM2 CPUFUNC(op_2040_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L An,An */
void REGPARAM2 CPUFUNC(op_2048_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (An),An */
void REGPARAM2 CPUFUNC(op_2050_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_2058_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L -(An),An */
void REGPARAM2 CPUFUNC(op_2060_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_2068_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_2070_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	m68k_areg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_2078_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_2079_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_207a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_207b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	m68k_areg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_207c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_2080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(An) */
void REGPARAM2 CPUFUNC(op_2088_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(An) */
void REGPARAM2 CPUFUNC(op_2090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(An) */
void REGPARAM2 CPUFUNC(op_2098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(An) */
void REGPARAM2 CPUFUNC(op_20a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_20a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_20b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_20b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_20b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_20ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_20bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_20bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_20c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(An)+ */
void REGPARAM2 CPUFUNC(op_20c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(An)+ */
void REGPARAM2 CPUFUNC(op_20d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_20d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_20e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_20e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_20f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_20f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_20f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_20fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_20fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_20fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_2100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,-(An) */
void REGPARAM2 CPUFUNC(op_2108_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),-(An) */
void REGPARAM2 CPUFUNC(op_2110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_2118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),-(An) */
void REGPARAM2 CPUFUNC(op_2120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_2128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_2130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_2138_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_2139_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_213a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_213b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_213c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_2140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(d16,An) */
void REGPARAM2 CPUFUNC(op_2148_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_2158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif

#ifdef PART_3
/* MOVE.L (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_2170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_2178_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_2179_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_217a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_217b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_217c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2188_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_21c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(xxx).W */
void REGPARAM2 CPUFUNC(op_21c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_21d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_21f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_21f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_21f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_21fa_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_21fb_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_21fc_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_23c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(xxx).L */
void REGPARAM2 CPUFUNC(op_23c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_23d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_23f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_23f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_23f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (10);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_23fa_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_23fb_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_23fc_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
	m68k_incpc (10);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_3000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,Dn */
void REGPARAM2 CPUFUNC(op_3008_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),Dn */
void REGPARAM2 CPUFUNC(op_3010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_3018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_3020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_3028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_3030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_3038_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_3039_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_303a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_303b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_303c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W Dn,An */
void REGPARAM2 CPUFUNC(op_3040_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W An,An */
void REGPARAM2 CPUFUNC(op_3048_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (An),An */
void REGPARAM2 CPUFUNC(op_3050_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_3058_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W -(An),An */
void REGPARAM2 CPUFUNC(op_3060_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_3068_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_3070_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_3078_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_3079_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_307a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_307b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_307c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_3080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(An) */
void REGPARAM2 CPUFUNC(op_3088_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(An) */
void REGPARAM2 CPUFUNC(op_3090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(An) */
void REGPARAM2 CPUFUNC(op_3098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(An) */
void REGPARAM2 CPUFUNC(op_30a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_30a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_30b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_30b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_30b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_30ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_30bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_30bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_30c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(An)+ */
void REGPARAM2 CPUFUNC(op_30c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(An)+ */
void REGPARAM2 CPUFUNC(op_30d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_30d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_30e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_30e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_30f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_30f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_30f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_30fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_30fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_30fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_3100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,-(An) */
void REGPARAM2 CPUFUNC(op_3108_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),-(An) */
void REGPARAM2 CPUFUNC(op_3110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_3118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),-(An) */
void REGPARAM2 CPUFUNC(op_3120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_3128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_3130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_3138_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_3139_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_313a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_313b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_313c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_3140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(d16,An) */
void REGPARAM2 CPUFUNC(op_3148_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_3158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_3170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_3178_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_3179_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_317a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_317b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_317c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3188_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_31c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(xxx).W */
void REGPARAM2 CPUFUNC(op_31c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_31d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_31f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_31f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_31f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_31fa_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_31fb_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_31fc_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_33c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(xxx).L */
void REGPARAM2 CPUFUNC(op_33c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_33d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_33f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_33f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_33f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (10);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_33fa_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_33fb_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_33fc_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.B Dn */
void REGPARAM2 CPUFUNC(op_4000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((newv) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.B (An) */
void REGPARAM2 CPUFUNC(op_4010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.B (An)+ */
void REGPARAM2 CPUFUNC(op_4018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.B -(An) */
void REGPARAM2 CPUFUNC(op_4020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4038_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4039_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEGX.W Dn */
void REGPARAM2 CPUFUNC(op_4040_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((newv) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.W (An) */
void REGPARAM2 CPUFUNC(op_4050_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.W (An)+ */
void REGPARAM2 CPUFUNC(op_4058_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.W -(An) */
void REGPARAM2 CPUFUNC(op_4060_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4068_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4070_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce020 (srca, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4078_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4079_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEGX.L Dn */
void REGPARAM2 CPUFUNC(op_4080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, srcreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.L (An) */
void REGPARAM2 CPUFUNC(op_4090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.L (An)+ */
void REGPARAM2 CPUFUNC(op_4098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.L -(An) */
void REGPARAM2 CPUFUNC(op_40a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.L (d16,An) */
void REGPARAM2 CPUFUNC(op_40a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_40b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce020 (srca, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.L (xxx).W */
void REGPARAM2 CPUFUNC(op_40b8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.L (xxx).L */
void REGPARAM2 CPUFUNC(op_40b9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVSR2.W Dn */
void REGPARAM2 CPUFUNC(op_40c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel5926; }
{{	MakeSR ();
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((regs.sr) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel5926: ;
return;
}

/* MVSR2.W (An) */
void REGPARAM2 CPUFUNC(op_40d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel5927; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
	MakeSR ();
	put_word_ce020 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel5927: ;
return;
}

/* MVSR2.W (An)+ */
void REGPARAM2 CPUFUNC(op_40d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel5928; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 2;
	MakeSR ();
	put_word_ce020 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel5928: ;
return;
}

/* MVSR2.W -(An) */
void REGPARAM2 CPUFUNC(op_40e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel5929; }
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	m68k_areg (regs, srcreg) = srca;
	MakeSR ();
	put_word_ce020 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel5929: ;
return;
}

/* MVSR2.W (d16,An) */
void REGPARAM2 CPUFUNC(op_40e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel5930; }
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	MakeSR ();
	put_word_ce020 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel5930: ;
return;
}

/* MVSR2.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_40f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel5931; }
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	MakeSR ();
	put_word_ce020 (srca, regs.sr);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5931: ;
return;
}

/* MVSR2.W (xxx).W */
void REGPARAM2 CPUFUNC(op_40f8_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5932; }
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	MakeSR ();
	put_word_ce020 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel5932: ;
return;
}

/* MVSR2.W (xxx).L */
void REGPARAM2 CPUFUNC(op_40f9_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel5933; }
{{	uaecptr srca = get_long_ce020_prefetch (2);
	MakeSR ();
	put_word_ce020 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel5933: ;
return;
}

/* CHK.L Dn,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5934;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5934;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5934: ;
return;
}

#endif
/* CHK.L (An),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5935;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5935;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5935: ;
return;
}

#endif
/* CHK.L (An)+,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5936;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5936;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5936: ;
return;
}

#endif
/* CHK.L -(An),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5937;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5937;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5937: ;
return;
}

#endif
/* CHK.L (d16,An),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5938;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5938;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5938: ;
return;
}

#endif
/* CHK.L (d8,An,Xn),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5939;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5939;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5939: ;
return;
}

#endif
/* CHK.L (xxx).W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4138_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5940;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5940;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5940: ;
return;
}

#endif
/* CHK.L (xxx).L,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4139_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (6);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5941;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5941;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5941: ;
return;
}

#endif
/* CHK.L (d16,PC),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_413a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5942;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5942;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5942: ;
return;
}

#endif
/* CHK.L (d8,PC,Xn),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_413b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5943;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5943;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5943: ;
return;
}

#endif
/* CHK.L #<data>.L,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_413c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (6);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5944;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5944;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5944: ;
return;
}

#endif
/* CHK.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_4180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5945;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5945;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5945: ;
return;
}

/* CHK.W (An),Dn */
void REGPARAM2 CPUFUNC(op_4190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5946;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5946;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5946: ;
return;
}

/* CHK.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_4198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5947;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5947;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5947: ;
return;
}

/* CHK.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_41a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5948;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5948;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5948: ;
return;
}

/* CHK.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_41a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5949;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5949;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5949: ;
return;
}

/* CHK.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_41b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5950;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5950;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5950: ;
return;
}

/* CHK.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_41b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5951;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5951;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5951: ;
return;
}

/* CHK.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_41b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (6);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5952;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5952;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5952: ;
return;
}

/* CHK.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_41ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5953;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5953;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5953: ;
return;
}

/* CHK.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_41bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5954;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5954;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5954: ;
return;
}

/* CHK.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_41bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel5955;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel5955;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel5955: ;
return;
}

/* LEA.L (An),An */
void REGPARAM2 CPUFUNC(op_41d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LEA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_41e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LEA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_41f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	m68k_areg (regs, dstreg) = (srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LEA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_41f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LEA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_41f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* LEA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_41fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LEA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_41fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	m68k_areg (regs, dstreg) = (srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.B Dn */
void REGPARAM2 CPUFUNC(op_4200_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((0) & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.B (An) */
void REGPARAM2 CPUFUNC(op_4210_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.B (An)+ */
void REGPARAM2 CPUFUNC(op_4218_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.B -(An) */
void REGPARAM2 CPUFUNC(op_4220_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4228_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4230_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce020 (srca, 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4238_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4239_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CLR.W Dn */
void REGPARAM2 CPUFUNC(op_4240_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((0) & 0xffff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.W (An) */
void REGPARAM2 CPUFUNC(op_4250_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.W (An)+ */
void REGPARAM2 CPUFUNC(op_4258_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.W -(An) */
void REGPARAM2 CPUFUNC(op_4260_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4268_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4270_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce020 (srca, 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4278_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4279_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CLR.L Dn */
void REGPARAM2 CPUFUNC(op_4280_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	m68k_dreg (regs, srcreg) = (0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.L (An) */
void REGPARAM2 CPUFUNC(op_4290_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.L (An)+ */
void REGPARAM2 CPUFUNC(op_4298_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.L -(An) */
void REGPARAM2 CPUFUNC(op_42a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.L (d16,An) */
void REGPARAM2 CPUFUNC(op_42a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_42b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce020 (srca, 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.L (xxx).W */
void REGPARAM2 CPUFUNC(op_42b8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif

#ifdef PART_4
/* CLR.L (xxx).L */
void REGPARAM2 CPUFUNC(op_42b9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce020 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVSR2.B Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	MakeSR ();
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((regs.sr & 0xff) & 0xffff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* MVSR2.B (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	MakeSR ();
	put_word_ce020 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* MVSR2.B (An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 2;
	MakeSR ();
	put_word_ce020 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* MVSR2.B -(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	m68k_areg (regs, srcreg) = srca;
	MakeSR ();
	put_word_ce020 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* MVSR2.B (d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	MakeSR ();
	put_word_ce020 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* MVSR2.B (d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	MakeSR ();
	put_word_ce020 (srca, regs.sr & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* MVSR2.B (xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	MakeSR ();
	put_word_ce020 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* MVSR2.B (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
	MakeSR ();
	put_word_ce020 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* NEG.B Dn */
void REGPARAM2 CPUFUNC(op_4400_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((dst) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.B (An) */
void REGPARAM2 CPUFUNC(op_4410_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.B (An)+ */
void REGPARAM2 CPUFUNC(op_4418_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.B -(An) */
void REGPARAM2 CPUFUNC(op_4420_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4428_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4430_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (srca, dst);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEG.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4438_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4439_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEG.W Dn */
void REGPARAM2 CPUFUNC(op_4440_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.W (An) */
void REGPARAM2 CPUFUNC(op_4450_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.W (An)+ */
void REGPARAM2 CPUFUNC(op_4458_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.W -(An) */
void REGPARAM2 CPUFUNC(op_4460_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4468_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4470_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (srca, dst);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEG.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4478_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4479_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEG.L Dn */
void REGPARAM2 CPUFUNC(op_4480_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, srcreg) = (dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.L (An) */
void REGPARAM2 CPUFUNC(op_4490_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.L (An)+ */
void REGPARAM2 CPUFUNC(op_4498_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.L -(An) */
void REGPARAM2 CPUFUNC(op_44a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.L (d16,An) */
void REGPARAM2 CPUFUNC(op_44a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_44b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (srca, dst);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEG.L (xxx).W */
void REGPARAM2 CPUFUNC(op_44b8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.L (xxx).L */
void REGPARAM2 CPUFUNC(op_44b9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MV2SR.B Dn */
void REGPARAM2 CPUFUNC(op_44c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MV2SR.B (An) */
void REGPARAM2 CPUFUNC(op_44d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MV2SR.B (An)+ */
void REGPARAM2 CPUFUNC(op_44d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MV2SR.B -(An) */
void REGPARAM2 CPUFUNC(op_44e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MV2SR.B (d16,An) */
void REGPARAM2 CPUFUNC(op_44e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MV2SR.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_44f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MV2SR.B (xxx).W */
void REGPARAM2 CPUFUNC(op_44f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MV2SR.B (xxx).L */
void REGPARAM2 CPUFUNC(op_44f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MV2SR.B (d16,PC) */
void REGPARAM2 CPUFUNC(op_44fa_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MV2SR.B (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_44fb_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MV2SR.B #<data>.B */
void REGPARAM2 CPUFUNC(op_44fc_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.B Dn */
void REGPARAM2 CPUFUNC(op_4600_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((dst) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.B (An) */
void REGPARAM2 CPUFUNC(op_4610_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.B (An)+ */
void REGPARAM2 CPUFUNC(op_4618_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.B -(An) */
void REGPARAM2 CPUFUNC(op_4620_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4628_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4630_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce020 (srca, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NOT.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4638_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4639_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NOT.W Dn */
void REGPARAM2 CPUFUNC(op_4640_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.W (An) */
void REGPARAM2 CPUFUNC(op_4650_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.W (An)+ */
void REGPARAM2 CPUFUNC(op_4658_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.W -(An) */
void REGPARAM2 CPUFUNC(op_4660_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4668_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4670_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce020 (srca, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NOT.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4678_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4679_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NOT.L Dn */
void REGPARAM2 CPUFUNC(op_4680_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (dst);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.L (An) */
void REGPARAM2 CPUFUNC(op_4690_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.L (An)+ */
void REGPARAM2 CPUFUNC(op_4698_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.L -(An) */
void REGPARAM2 CPUFUNC(op_46a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.L (d16,An) */
void REGPARAM2 CPUFUNC(op_46a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_46b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce020 (srca, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NOT.L (xxx).W */
void REGPARAM2 CPUFUNC(op_46b8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.L (xxx).L */
void REGPARAM2 CPUFUNC(op_46b9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce020 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MV2SR.W Dn */
void REGPARAM2 CPUFUNC(op_46c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel6054; }
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	regs.sr = src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6054: ;
return;
}

/* MV2SR.W (An) */
void REGPARAM2 CPUFUNC(op_46d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel6055; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6055: ;
return;
}

/* MV2SR.W (An)+ */
void REGPARAM2 CPUFUNC(op_46d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel6056; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6056: ;
return;
}

/* MV2SR.W -(An) */
void REGPARAM2 CPUFUNC(op_46e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel6057; }
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6057: ;
return;
}

/* MV2SR.W (d16,An) */
void REGPARAM2 CPUFUNC(op_46e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel6058; }
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6058: ;
return;
}

/* MV2SR.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_46f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel6059; }
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6059: ;
return;
}

/* MV2SR.W (xxx).W */
void REGPARAM2 CPUFUNC(op_46f8_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel6060; }
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6060: ;
return;
}

/* MV2SR.W (xxx).L */
void REGPARAM2 CPUFUNC(op_46f9_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel6061; }
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6061: ;
return;
}

/* MV2SR.W (d16,PC) */
void REGPARAM2 CPUFUNC(op_46fa_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel6062; }
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6062: ;
return;
}

/* MV2SR.W (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_46fb_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel6063; }
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6063: ;
return;
}

/* MV2SR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_46fc_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel6064; }
{{	uae_s16 src = get_word_ce020_prefetch (2);
	regs.sr = src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6064: ;
return;
}

/* NBCD.B Dn */
void REGPARAM2 CPUFUNC(op_4800_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((newv) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LINK.L An,#<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4808_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr olda;
	olda = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = olda;
{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 offs = get_long_ce020_prefetch (2);
	put_long_ce020 (olda, src);
	m68k_areg (regs, srcreg) = (m68k_areg (regs, 7));
	m68k_areg (regs, 7) += offs;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* NBCD.B (An) */
void REGPARAM2 CPUFUNC(op_4810_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B (An)+ */
void REGPARAM2 CPUFUNC(op_4818_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B -(An) */
void REGPARAM2 CPUFUNC(op_4820_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4828_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NBCD.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4830_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NBCD.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4838_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NBCD.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4839_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_u16 newv_lo = - (src & 0xF) - (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = - (src & 0xF0);
	uae_u16 newv;
	int cflg;
	if (newv_lo > 9) { newv_lo -= 6; }
	newv = newv_hi + newv_lo;	cflg = (newv & 0x1F0) > 0x90;
	if (cflg) newv -= 0x60;
	SET_CFLG (cflg);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SWAP.W Dn */
void REGPARAM2 CPUFUNC(op_4840_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_u32 dst = ((src >> 16)&0xFFFF) | ((src&0xFFFF)<<16);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (dst);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BKPTQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4848_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* PEA.L (An) */
void REGPARAM2 CPUFUNC(op_4850_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce020 (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* PEA.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4868_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce020 (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* PEA.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4870_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce020 (dsta, srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* PEA.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4878_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce020 (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* PEA.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4879_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce020 (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* PEA.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_487a_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce020 (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* PEA.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_487b_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce020 (dsta, srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EXT.W Dn */
void REGPARAM2 CPUFUNC(op_4880_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_u16 dst = (uae_s16)(uae_s8)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MVMLE.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_4890_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce020 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce020 (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_48a0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 0;
{	uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
	int type = get_cpu_model () >= 68020;
	while (amask) {
		srca -= 2;
		if (type) m68k_areg (regs, dstreg) = srca;
		put_word_ce020 (srca, m68k_areg (regs, movem_index2[amask]));
		amask = movem_next[amask];
	}
	while (dmask) { srca -= 2; put_word_ce020 (srca, m68k_dreg (regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_48a8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce020 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce020 (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_48b0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce020 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce020 (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMLE.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_48b8_20)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce020 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce020 (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_48b9_20)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = get_long_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce020 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce020 (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EXT.L Dn */
void REGPARAM2 CPUFUNC(op_48c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_u32 dst = (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (dst);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MVMLE.L #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_48d0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long_ce020 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long_ce020 (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.L #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_48e0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 0;
{	uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
	int type = get_cpu_model () >= 68020;
	while (amask) {
		srca -= 4;
		if (type) m68k_areg (regs, dstreg) = srca;
		put_long_ce020 (srca, m68k_areg (regs, movem_index2[amask]));
		amask = movem_next[amask];
	}
	while (dmask) { srca -= 4; put_long_ce020 (srca, m68k_dreg (regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.L #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_48e8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long_ce020 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long_ce020 (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.L #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_48f0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long_ce020 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long_ce020 (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMLE.L #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_48f8_20)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long_ce020 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long_ce020 (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.L #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_48f9_20)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = get_long_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long_ce020 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long_ce020 (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EXT.B Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_49c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_u32 dst = (uae_s32)(uae_s8)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (dst);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* TST.B Dn */
void REGPARAM2 CPUFUNC(op_4a00_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.B (An) */
void REGPARAM2 CPUFUNC(op_4a10_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.B (An)+ */
void REGPARAM2 CPUFUNC(op_4a18_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.B -(An) */
void REGPARAM2 CPUFUNC(op_4a20_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4a28_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4a30_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TST.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4a38_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4a39_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TST.B (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a3a_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* TST.B (d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a3b_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* TST.B #<data>.B */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a3c_20)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* TST.W Dn */
void REGPARAM2 CPUFUNC(op_4a40_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.W An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a48_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* TST.W (An) */
void REGPARAM2 CPUFUNC(op_4a50_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.W (An)+ */
void REGPARAM2 CPUFUNC(op_4a58_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.W -(An) */
void REGPARAM2 CPUFUNC(op_4a60_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4a68_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4a70_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TST.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4a78_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4a79_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TST.W (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a7a_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* TST.W (d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a7b_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* TST.W #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a7c_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* TST.L Dn */
void REGPARAM2 CPUFUNC(op_4a80_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.L An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a88_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* TST.L (An) */
void REGPARAM2 CPUFUNC(op_4a90_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.L (An)+ */
void REGPARAM2 CPUFUNC(op_4a98_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.L -(An) */
void REGPARAM2 CPUFUNC(op_4aa0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4aa8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4ab0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TST.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4ab8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4ab9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TST.L (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4aba_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* TST.L (d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4abb_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* TST.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4abc_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* TAS.B Dn */
void REGPARAM2 CPUFUNC(op_4ac0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((src) & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TAS.B (An) */
void REGPARAM2 CPUFUNC(op_4ad0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce020 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TAS.B (An)+ */
void REGPARAM2 CPUFUNC(op_4ad8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce020 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TAS.B -(An) */
void REGPARAM2 CPUFUNC(op_4ae0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce020 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TAS.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4ae8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce020 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TAS.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4af0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce020 (srca, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TAS.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4af8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce020 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TAS.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4af9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce020 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MULL.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c00_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}return;
}

#endif
/* MULL.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c10_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c18_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c20_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c28_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (6);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c30_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_mull(opcode, dst, extra);
}}}}}return;
}

#endif
/* MULL.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c38_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (6);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c39_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (8);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c3a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (6);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c3b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_mull(opcode, dst, extra);
}}}}}return;
}

#endif
/* MULL.L #<data>.W,#<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c3c_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020_prefetch (4);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (8);
	m68k_mull(opcode, dst, extra);
}}}return;
}

#endif
/* DIVL.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c40_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}return;
}

#endif
/* DIVL.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c50_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c58_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c60_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c68_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (6);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c70_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_divl(opcode, dst, extra);
}}}}}return;
}

#endif
/* DIVL.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c78_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (6);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c79_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (8);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c7a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (6);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c7b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_divl(opcode, dst, extra);
}}}}}return;
}

#endif
/* DIVL.L #<data>.W,#<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c7c_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020_prefetch (4);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (8);
	m68k_divl(opcode, dst, extra);
}}}return;
}

#endif
/* MVMEL.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_4c90_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_4c98_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; amask = movem_next[amask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_4ca8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4cb0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMEL.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_4cb8_20)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_4cb9_20)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = get_long_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* MVMEL.W #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_4cba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.W #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4cbb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce020 (srca); srca += 2; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMEL.L #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_4cd0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce020 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce020 (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.L #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_4cd8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce020 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce020 (srca); srca += 4; amask = movem_next[amask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.L #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_4ce8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce020 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce020 (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.L #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4cf0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce020 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce020 (srca); srca += 4; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMEL.L #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_4cf8_20)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce020 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce020 (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.L #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_4cf9_20)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = get_long_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce020 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce020 (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* MVMEL.L #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_4cfa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce020 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce020 (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.L #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4cfb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce020 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce020 (srca); srca += 4; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TRAPQ.L #<data> */
void REGPARAM2 CPUFUNC(op_4e40_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 15);
{{	uae_u32 src = srcreg;
	m68k_incpc (2);
	Exception (src + 32);
}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LINK.W An,#<data>.W */
void REGPARAM2 CPUFUNC(op_4e50_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr olda;
	olda = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = olda;
{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	put_long_ce020 (olda, src);
	m68k_areg (regs, srcreg) = (m68k_areg (regs, 7));
	m68k_areg (regs, 7) += offs;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* UNLK.L An */
void REGPARAM2 CPUFUNC(op_4e58_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
	m68k_areg (regs, 7) = src;
{	uaecptr olda = m68k_areg (regs, 7);
{	uae_s32 old = get_long_ce020 (olda);
	m68k_areg (regs, 7) += 4;
	m68k_areg (regs, srcreg) = (old);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MVR2USP.L An */
void REGPARAM2 CPUFUNC(op_4e60_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel6182; }
{{	uae_s32 src = m68k_areg (regs, srcreg);
	regs.usp = src;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6182: ;
return;
}

/* MVUSP2R.L An */
void REGPARAM2 CPUFUNC(op_4e68_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel6183; }
{{	m68k_areg (regs, srcreg) = (regs.usp);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6183: ;
return;
}

/* RESET.L  */
void REGPARAM2 CPUFUNC(op_4e70_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel6184; }
{	cpureset ();
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6184: ;
return;
}

/* NOP.L  */
void REGPARAM2 CPUFUNC(op_4e71_20)(uae_u32 opcode)
{
{	do_cycles_ce020 (6);
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* STOP.L #<data>.W */
void REGPARAM2 CPUFUNC(op_4e72_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel6186; }
{{	uae_s16 src = get_word_ce020_prefetch (2);
	regs.sr = src;
	MakeFromSR ();
	m68k_setstopped ();
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6186: ;
return;
}

/* RTE.L  */
void REGPARAM2 CPUFUNC(op_4e73_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel6187; }
{	uae_u16 newsr; uae_u32 newpc;
	for (;;) {
		uaecptr a = m68k_areg (regs, 7);
		uae_s16 sr = get_word_ce020 (a);
		uae_s32 pc = get_long_ce020 (a + 2);
		uae_s16 format = get_word_ce020 (a + 2 + 4);
		int frame = format >> 12;
		int offset = 8;
		newsr = sr; newpc = pc;
		if (frame == 0x0) { m68k_areg (regs, 7) += offset; break; }
		else if (frame == 0x1) { m68k_areg (regs, 7) += offset; }
		else if (frame == 0x2) { m68k_areg (regs, 7) += offset + 4; break; }
		else if (frame == 0x4) { m68k_areg (regs, 7) += offset + 8; break; }
		else if (frame == 0x8) { m68k_areg (regs, 7) += offset + 50; break; }
		else if (frame == 0x9) { m68k_areg (regs, 7) += offset + 12; break; }
		else if (frame == 0xa) { m68k_areg (regs, 7) += offset + 24; break; }
		else if (frame == 0xb) { m68k_areg (regs, 7) += offset + 84; break; }
		else { m68k_areg (regs, 7) += offset; Exception (14); goto endlabel6187; }
		regs.sr = newsr; MakeFromSR ();
}
	regs.sr = newsr; MakeFromSR ();
	if (newpc & 1) {
		exception3i (0x4E73, newpc);
		goto endlabel6187;
	}
		m68k_setpc (newpc);
	ipl_fetch ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6187: ;
return;
}

/* RTD.L #<data>.W */
void REGPARAM2 CPUFUNC(op_4e74_20)(uae_u32 opcode)
{
{{	uaecptr pca = m68k_areg (regs, 7);
{	uae_s32 pc = get_long_ce020 (pca);
	m68k_areg (regs, 7) += 4;
{	uae_s16 offs = get_word_ce020_prefetch (2);
	m68k_areg (regs, 7) += offs;
	if (pc & 1) {
		exception3i (0x4E74, pc);
		goto endlabel6188;
	}
	if (pc & 1) {
		exception3i (0x4E74, pc);
		goto endlabel6188;
	}
	m68k_setpc (pc);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6188: ;
return;
}

/* RTS.L  */
void REGPARAM2 CPUFUNC(op_4e75_20)(uae_u32 opcode)
{
{	uaecptr pc = m68k_getpc ();
	m68k_do_rts_ce020 ();
	if (m68k_getpc () & 1) {
		uaecptr faultpc = m68k_getpc ();
		m68k_setpc (pc);
		exception3i (0x4E75, faultpc);
	}
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TRAPV.L  */
void REGPARAM2 CPUFUNC(op_4e76_20)(uae_u32 opcode)
{
{	m68k_incpc (2);
	if (GET_VFLG ()) {
		Exception (7);
		goto endlabel6190;
	}
}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6190: ;
return;
}

/* RTR.L  */
void REGPARAM2 CPUFUNC(op_4e77_20)(uae_u32 opcode)
{
{	uaecptr oldpc = m68k_getpc ();
	MakeSR ();
{	uaecptr sra = m68k_areg (regs, 7);
{	uae_s16 sr = get_word_ce020 (sra);
	m68k_areg (regs, 7) += 2;
{	uaecptr pca = m68k_areg (regs, 7);
{	uae_s32 pc = get_long_ce020 (pca);
	m68k_areg (regs, 7) += 4;
	regs.sr &= 0xFF00; sr &= 0xFF;
	regs.sr |= sr;
	m68k_setpc (pc);
	MakeFromSR ();
	if (m68k_getpc () & 1) {
		uaecptr faultpc = m68k_getpc ();
		m68k_setpc (oldpc);
		exception3i (0x4E77, faultpc);
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEC2.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4e7a_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel6192; }
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	int regno = (src >> 12) & 15;
	uae_u32 *regp = regs.regs + regno;
	if (! m68k_movec2(src & 0xFFF, regp)) goto endlabel6192;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6192: ;
return;
}

#endif
/* MOVE2C.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4e7b_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel6193; }
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	int regno = (src >> 12) & 15;
	uae_u32 *regp = regs.regs + regno;
	if (! m68k_move2c(src & 0xFFF, regp)) goto endlabel6193;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6193: ;
return;
}

#endif
/* JSR.L (An) */
void REGPARAM2 CPUFUNC(op_4e90_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uaecptr oldpc = m68k_getpc () + 2;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6194;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce020 (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6194: ;
return;
}

/* JSR.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4ea8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6195;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce020 (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6195: ;
return;
}

/* JSR.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4eb0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uaecptr oldpc = m68k_getpc () + 0;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6196;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce020 (m68k_areg (regs, 7), oldpc);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6196: ;
return;
}

/* JSR.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4eb8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6197;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce020 (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6197: ;
return;
}

/* JSR.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4eb9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 6;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6198;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce020 (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6198: ;
return;
}

/* JSR.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_4eba_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6199;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce020 (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6199: ;
return;
}

/* JSR.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4ebb_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uaecptr oldpc = m68k_getpc () + 0;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6200;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce020 (m68k_areg (regs, 7), oldpc);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6200: ;
return;
}

/* JMP.L (An) */
void REGPARAM2 CPUFUNC(op_4ed0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6201;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6201: ;
return;
}

/* JMP.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4ee8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6202;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6202: ;
return;
}

/* JMP.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4ef0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6203;
	}
	m68k_setpc (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6203: ;
return;
}

/* JMP.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4ef8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6204;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6204: ;
return;
}

/* JMP.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4ef9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6205;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6205: ;
return;
}

/* JMP.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_4efa_20)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6206;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6206: ;
return;
}

/* JMP.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4efb_20)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel6207;
	}
	m68k_setpc (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6207: ;
return;
}

/* ADDQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.B #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.B #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.B #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.B #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.B #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDQ.B #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5038_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.B #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5039_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5040_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDAQ.W #<data>,An */
void REGPARAM2 CPUFUNC(op_5048_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.W #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5050_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.W #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5058_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.W #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5060_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif

#ifdef PART_5
/* ADDQ.W #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5068_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.W #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5070_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDQ.W #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5078_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.W #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5079_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDAQ.L #<data>,An */
void REGPARAM2 CPUFUNC(op_5088_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.L #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.L #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.L #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_50a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.L #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_50a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.L #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_50b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDQ.L #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_50b8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.L #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_50b9_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_50c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (0) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_50c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (0)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6235;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6235: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_50d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_50d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_50e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_50e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_50f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_50f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_50f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_50fa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (0)) { Exception (7); goto endlabel6243; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6243: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_50fb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (0)) { Exception (7); goto endlabel6244; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6244: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_50fc_20)(uae_u32 opcode)
{
{	if (cctrue (0)) { Exception (7); goto endlabel6245; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6245: ;
return;
}

#endif
/* SUBQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.B #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.B #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.B #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.B #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.B #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBQ.B #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5138_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.B #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5139_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBAQ.W #<data>,An */
void REGPARAM2 CPUFUNC(op_5148_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.W #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.W #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.W #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.W #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.W #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBQ.W #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5178_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.W #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5179_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBAQ.L #<data>,An */
void REGPARAM2 CPUFUNC(op_5188_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.L #<data>,(An) */
void REGPARAM2 CPUFUNC(op_5190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.L #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.L #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_51a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.L #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_51a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.L #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_51b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBQ.L #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_51b8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.L #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_51b9_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_51c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (1) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_51c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (1)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6273;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6273: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_51d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_51d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_51e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_51e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_51f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_51f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_51f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_51fa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (1)) { Exception (7); goto endlabel6281; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6281: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_51fb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (1)) { Exception (7); goto endlabel6282; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6282: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_51fc_20)(uae_u32 opcode)
{
{	if (cctrue (1)) { Exception (7); goto endlabel6283; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6283: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_52c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (2) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_52c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (2)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6285;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6285: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_52d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_52d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_52e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_52e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_52f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_52f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_52f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_52fa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (2)) { Exception (7); goto endlabel6293; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6293: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_52fb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (2)) { Exception (7); goto endlabel6294; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6294: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_52fc_20)(uae_u32 opcode)
{
{	if (cctrue (2)) { Exception (7); goto endlabel6295; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6295: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_53c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (3) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_53c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (3)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6297;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6297: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_53d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_53d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_53e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_53e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_53f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_53f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_53f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_53fa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (3)) { Exception (7); goto endlabel6305; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6305: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_53fb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (3)) { Exception (7); goto endlabel6306; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6306: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_53fc_20)(uae_u32 opcode)
{
{	if (cctrue (3)) { Exception (7); goto endlabel6307; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6307: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_54c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (4) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_54c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (4)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6309;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6309: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_54d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_54d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_54e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_54e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_54f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_54f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_54f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_54fa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (4)) { Exception (7); goto endlabel6317; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6317: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_54fb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (4)) { Exception (7); goto endlabel6318; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6318: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_54fc_20)(uae_u32 opcode)
{
{	if (cctrue (4)) { Exception (7); goto endlabel6319; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6319: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_55c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (5) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_55c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (5)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6321;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6321: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_55d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_55d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_55e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_55e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_55f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_55f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_55f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_55fa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (5)) { Exception (7); goto endlabel6329; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6329: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_55fb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (5)) { Exception (7); goto endlabel6330; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6330: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_55fc_20)(uae_u32 opcode)
{
{	if (cctrue (5)) { Exception (7); goto endlabel6331; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6331: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_56c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (6) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_56c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (6)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6333;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6333: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_56d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_56d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_56e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_56e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_56f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_56f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_56f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_56fa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (6)) { Exception (7); goto endlabel6341; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6341: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_56fb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (6)) { Exception (7); goto endlabel6342; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6342: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_56fc_20)(uae_u32 opcode)
{
{	if (cctrue (6)) { Exception (7); goto endlabel6343; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6343: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_57c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (7) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_57c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (7)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6345;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6345: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_57d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_57d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_57e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_57e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_57f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_57f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_57f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_57fa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (7)) { Exception (7); goto endlabel6353; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6353: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_57fb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (7)) { Exception (7); goto endlabel6354; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6354: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_57fc_20)(uae_u32 opcode)
{
{	if (cctrue (7)) { Exception (7); goto endlabel6355; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6355: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_58c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (8) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_58c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (8)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6357;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6357: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_58d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_58d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_58e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_58e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_58f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_58f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_58f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_58fa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (8)) { Exception (7); goto endlabel6365; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6365: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_58fb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (8)) { Exception (7); goto endlabel6366; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6366: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_58fc_20)(uae_u32 opcode)
{
{	if (cctrue (8)) { Exception (7); goto endlabel6367; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6367: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_59c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (9) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_59c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (9)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6369;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6369: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_59d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_59d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_59e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_59e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_59f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_59f8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_59f9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_59fa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (9)) { Exception (7); goto endlabel6377; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6377: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_59fb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (9)) { Exception (7); goto endlabel6378; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6378: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_59fc_20)(uae_u32 opcode)
{
{	if (cctrue (9)) { Exception (7); goto endlabel6379; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6379: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5ac0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (10) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5ac8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (10)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6381;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6381: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5ad0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5ad8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ae0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ae8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5af0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5af8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5af9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5afa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (10)) { Exception (7); goto endlabel6389; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6389: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5afb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (10)) { Exception (7); goto endlabel6390; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6390: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5afc_20)(uae_u32 opcode)
{
{	if (cctrue (10)) { Exception (7); goto endlabel6391; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6391: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5bc0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (11) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5bc8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (11)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6393;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6393: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5bd0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5bd8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5be0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5be8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5bf0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5bf8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5bf9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5bfa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (11)) { Exception (7); goto endlabel6401; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6401: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5bfb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (11)) { Exception (7); goto endlabel6402; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6402: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5bfc_20)(uae_u32 opcode)
{
{	if (cctrue (11)) { Exception (7); goto endlabel6403; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6403: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5cc0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (12) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5cc8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (12)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6405;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6405: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5cd0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5cd8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ce0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ce8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5cf0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5cf8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5cf9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5cfa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (12)) { Exception (7); goto endlabel6413; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6413: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5cfb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (12)) { Exception (7); goto endlabel6414; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6414: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5cfc_20)(uae_u32 opcode)
{
{	if (cctrue (12)) { Exception (7); goto endlabel6415; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6415: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5dc0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (13) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5dc8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (13)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6417;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6417: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5dd0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5dd8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5de0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5de8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5df0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5df8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5df9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5dfa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (13)) { Exception (7); goto endlabel6425; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6425: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5dfb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (13)) { Exception (7); goto endlabel6426; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6426: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5dfc_20)(uae_u32 opcode)
{
{	if (cctrue (13)) { Exception (7); goto endlabel6427; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6427: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5ec0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (14) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5ec8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (14)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6429;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6429: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5ed0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5ed8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ee0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ee8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5ef0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5ef8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5ef9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5efa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (14)) { Exception (7); goto endlabel6437; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6437: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5efb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (14)) { Exception (7); goto endlabel6438; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6438: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5efc_20)(uae_u32 opcode)
{
{	if (cctrue (14)) { Exception (7); goto endlabel6439; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6439: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5fc0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (15) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5fc8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (15)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel6441;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel6441: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5fd0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5fd8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5fe0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5fe8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5ff0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5ff8_20)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5ff9_20)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce020 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5ffa_20)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (15)) { Exception (7); goto endlabel6449; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel6449: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5ffb_20)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (15)) { Exception (7); goto endlabel6450; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel6450: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5ffc_20)(uae_u32 opcode)
{
{	if (cctrue (15)) { Exception (7); goto endlabel6451; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel6451: ;
return;
}

#endif
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6000_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6452;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6452: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6001_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6453;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6453: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_60ff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6454;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6454: ;
return;
}

/* BSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6100_20)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s16 src = get_word_ce020_prefetch (2);
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + s);
		goto endlabel6455;
	}
	m68k_do_bsr_ce020 (m68k_getpc () + 4, s);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6455: ;
return;
}

#endif

#ifdef PART_6
/* BSRQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6101_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{	uae_s32 s;
{	uae_u32 src = srcreg;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + s);
		goto endlabel6456;
	}
	m68k_do_bsr_ce020 (m68k_getpc () + 2, s);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6456: ;
return;
}

/* BSR.L #<data>.L */
void REGPARAM2 CPUFUNC(op_61ff_20)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s32 src = get_long_ce020_prefetch (2);
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + s);
		goto endlabel6457;
	}
	m68k_do_bsr_ce020 (m68k_getpc () + 6, s);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6457: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6200_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6458;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6458: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6201_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6459;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6459: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_62ff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6460;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6460: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6300_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6461;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6461: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6301_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6462;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6462: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_63ff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6463;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6463: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6400_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6464;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6464: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6401_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6465;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6465: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_64ff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6466;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6466: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6500_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6467;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6467: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6501_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6468;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6468: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_65ff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6469;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6469: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6600_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6470;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6470: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6601_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6471;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6471: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_66ff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6472;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6472: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6700_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6473;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6473: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6701_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6474;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6474: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_67ff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6475;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6475: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6800_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6476;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6476: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6801_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6477;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6477: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_68ff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6478;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6478: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6900_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6479;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6479: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6901_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6480;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6480: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_69ff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6481;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6481: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6a00_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6482;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6482: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6a01_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6483;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6483: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6aff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6484;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6484: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6b00_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6485;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6485: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6b01_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6486;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6486: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6bff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6487;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6487: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6c00_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6488;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6488: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6c01_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6489;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6489: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6cff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6490;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6490: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6d00_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6491;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6491: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6d01_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6492;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6492: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6dff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6493;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6493: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6e00_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6494;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6494: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6e01_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6495;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6495: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6eff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6496;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6496: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6f00_20)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6497;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6497: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6f01_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6498;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6498: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6fff_20)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel6499;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel6499: ;
return;
}

/* MOVEQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_7000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_u32 src = srcreg;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_8000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B (An),Dn */
void REGPARAM2 CPUFUNC(op_8010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_8018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_8020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_8028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_8030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_8038_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_8039_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_803a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_803b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_803c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_8040_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W (An),Dn */
void REGPARAM2 CPUFUNC(op_8050_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_8058_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_8060_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_8068_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_8070_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_8078_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_8079_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_807a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_807b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_807c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_8080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L (An),Dn */
void REGPARAM2 CPUFUNC(op_8090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_8098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_80a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_80a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_80b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_80b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_80ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_80bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* DIVU.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_80c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel6534;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (2);
	}
}}}endlabel6534: ;
return;
}

/* DIVU.W (An),Dn */
void REGPARAM2 CPUFUNC(op_80d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel6535;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (2);
	}
}}}}endlabel6535: ;
return;
}

/* DIVU.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_80d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel6536;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (2);
	}
}}}}endlabel6536: ;
return;
}

/* DIVU.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_80e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel6537;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (2);
	}
}}}}endlabel6537: ;
return;
}

/* DIVU.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_80e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel6538;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (4);
	}
}}}}endlabel6538: ;
return;
}

/* DIVU.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel6539;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	}
}}}}}endlabel6539: ;
return;
}

/* DIVU.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_80f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel6540;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (4);
	}
}}}}endlabel6540: ;
return;
}

/* DIVU.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_80f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (6);
		Exception (5);
		goto endlabel6541;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (6);
	}
}}}}endlabel6541: ;
return;
}

/* DIVU.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_80fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel6542;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (4);
	}
}}}}endlabel6542: ;
return;
}

/* DIVU.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel6543;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	}
}}}}}endlabel6543: ;
return;
}

/* DIVU.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_80fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel6544;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ce020memcycles += 36 * cpucycleunit;
		if (newv > 0xffff) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (4);
	}
}}}endlabel6544: ;
return;
}

/* SBCD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_8100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{	uae_u16 newv_lo = (dst & 0xF) - (src & 0xF) - (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = (dst & 0xF0) - (src & 0xF0);
	uae_u16 newv, tmp_newv;
	int bcd = 0;
	newv = tmp_newv = newv_hi + newv_lo;
	if (newv_lo & 0xF0) { newv -= 6; bcd = 6; };
	if ((((dst & 0xFF) - (src & 0xFF) - (GET_XFLG () ? 1 : 0)) & 0x100) > 0xFF) { newv -= 0x60; }
	SET_CFLG ((((dst & 0xFF) - (src & 0xFF) - bcd - (GET_XFLG () ? 1 : 0)) & 0x300) > 0xFF);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	SET_VFLG ((tmp_newv & 0x80) != 0 && (newv & 0x80) == 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SBCD.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_8108_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	uae_u16 newv_lo = (dst & 0xF) - (src & 0xF) - (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = (dst & 0xF0) - (src & 0xF0);
	uae_u16 newv, tmp_newv;
	int bcd = 0;
	newv = tmp_newv = newv_hi + newv_lo;
	if (newv_lo & 0xF0) { newv -= 6; bcd = 6; };
	if ((((dst & 0xFF) - (src & 0xFF) - (GET_XFLG () ? 1 : 0)) & 0x100) > 0xFF) { newv -= 0x60; }
	SET_CFLG ((((dst & 0xFF) - (src & 0xFF) - bcd - (GET_XFLG () ? 1 : 0)) & 0x300) > 0xFF);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	SET_VFLG ((tmp_newv & 0x80) != 0 && (newv & 0x80) == 0);
	put_byte_ce020 (dsta, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_8110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_8120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_8128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_8130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_8138_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_8139_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* PACK.L Dn,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_8140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uae_u16 val = m68k_dreg (regs, srcreg) + get_word_ce020_prefetch (2);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & 0xffffff00) | ((val >> 4) & 0xf0) | (val & 0xf);
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* PACK.L -(An),-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_8148_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uae_u16 val;
	m68k_areg (regs, srcreg) -= areg_byteinc[srcreg];
	val = (uae_u16)get_byte_ce020 (m68k_areg (regs, srcreg));
	m68k_areg (regs, srcreg) -= areg_byteinc[srcreg];
	val = (val | ((uae_u16)get_byte_ce020 (m68k_areg (regs, srcreg)) << 8)) + get_word_ce020_prefetch (2);
	m68k_areg (regs, dstreg) -= areg_byteinc[dstreg];
	put_byte_ce020 (m68k_areg (regs, dstreg),((val >> 4) & 0xf0) | (val & 0xf));
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* OR.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_8150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_8160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_8168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_8170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_8178_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_8179_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* UNPK.L Dn,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_8180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uae_u16 val = m68k_dreg (regs, srcreg);
	val = (((val << 4) & 0xf00) | (val & 0xf)) + get_word_ce020_prefetch (2);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & 0xffff0000) | (val & 0xffff);
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* UNPK.L -(An),-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_8188_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uae_u16 val;
	m68k_areg (regs, srcreg) -= areg_byteinc[srcreg];
	val = (uae_u16)get_byte_ce020 (m68k_areg (regs, srcreg));
	val = (((val << 4) & 0xf00) | (val & 0xf)) + get_word_ce020_prefetch (2);
	m68k_areg (regs, dstreg) -= 2 * areg_byteinc[dstreg];
	put_byte_ce020 (m68k_areg (regs, dstreg) + areg_byteinc[dstreg], val);
	put_byte_ce020 (m68k_areg (regs, dstreg), val >> 8);
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* OR.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_8190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_81a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_81a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_81b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_81b8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_81b9_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* DIVS.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_81c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel6572;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (2);
	}
}}}endlabel6572: ;
return;
}

/* DIVS.W (An),Dn */
void REGPARAM2 CPUFUNC(op_81d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel6573;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (2);
	}
}}}}endlabel6573: ;
return;
}

/* DIVS.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_81d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel6574;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (2);
	}
}}}}endlabel6574: ;
return;
}

/* DIVS.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_81e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel6575;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (2);
	}
}}}}endlabel6575: ;
return;
}

/* DIVS.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_81e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel6576;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (4);
	}
}}}}endlabel6576: ;
return;
}

/* DIVS.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_81f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel6577;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	}
}}}}}endlabel6577: ;
return;
}

/* DIVS.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_81f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel6578;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (4);
	}
}}}}endlabel6578: ;
return;
}

/* DIVS.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_81f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (6);
		Exception (5);
		goto endlabel6579;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (6);
	}
}}}}endlabel6579: ;
return;
}

/* DIVS.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_81fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel6580;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (4);
	}
}}}}endlabel6580: ;
return;
}

/* DIVS.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_81fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel6581;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	}
}}}}}endlabel6581: ;
return;
}

/* DIVS.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_81fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel6582;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ce020memcycles += 46 * cpucycleunit;
		if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
			SET_VFLG (1);
			SET_NFLG (1);
		} else {
			if (((uae_s16)rem < 0) != ((uae_s32)dst < 0)) rem = -rem;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(newv)) == 0);
	SET_NFLG   (((uae_s16)(newv)) < 0);
			newv = (newv & 0xffff) | ((uae_u32)rem << 16);
			m68k_dreg (regs, dstreg) = (newv);
		}
	m68k_incpc (4);
	}
}}}endlabel6582: ;
return;
}

/* SUB.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_9000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B (An),Dn */
void REGPARAM2 CPUFUNC(op_9010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_9018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_9020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_9028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_9030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_9038_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_9039_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_903a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_903b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_903c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_9040_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W An,Dn */
void REGPARAM2 CPUFUNC(op_9048_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W (An),Dn */
void REGPARAM2 CPUFUNC(op_9050_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_9058_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_9060_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_9068_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_9070_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_9078_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_9079_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_907a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_907b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_907c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_9080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L An,Dn */
void REGPARAM2 CPUFUNC(op_9088_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L (An),Dn */
void REGPARAM2 CPUFUNC(op_9090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_9098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_90a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_90a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_90b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_90b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_90b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_90ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_90bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_90bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBA.W Dn,An */
void REGPARAM2 CPUFUNC(op_90c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W An,An */
void REGPARAM2 CPUFUNC(op_90c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W (An),An */
void REGPARAM2 CPUFUNC(op_90d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_90d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W -(An),An */
void REGPARAM2 CPUFUNC(op_90e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_90e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_90f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_90f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_90f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_90fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_90fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_90fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBX.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_9100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = dst - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBX.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_9108_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	uae_u32 newv = dst - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_9110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_9120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_9128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_9130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_9138_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_9139_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBX.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_9140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = dst - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBX.W -(An),-(An) */
void REGPARAM2 CPUFUNC(op_9148_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	uae_u32 newv = dst - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_9150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_9160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_9168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_9170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_9178_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_9179_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBX.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_9180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = dst - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBX.L -(An),-(An) */
void REGPARAM2 CPUFUNC(op_9188_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	uae_u32 newv = dst - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_9190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_91a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_91a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_91b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_91b8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_91b9_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBA.L Dn,An */
void REGPARAM2 CPUFUNC(op_91c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L An,An */
void REGPARAM2 CPUFUNC(op_91c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L (An),An */
void REGPARAM2 CPUFUNC(op_91d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_91d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L -(An),An */
void REGPARAM2 CPUFUNC(op_91e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_91e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_91f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_91f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_91f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_91fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_91fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_91fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_b000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.B (An),Dn */
void REGPARAM2 CPUFUNC(op_b010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_b018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_b020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_b028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMP.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_b038_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_b039_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_b03a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b03b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMP.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_b03c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_b040_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.W An,Dn */
void REGPARAM2 CPUFUNC(op_b048_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.W (An),Dn */
void REGPARAM2 CPUFUNC(op_b050_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_b058_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_b060_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_b068_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b070_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMP.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_b078_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_b079_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_b07a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b07b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif

#ifdef PART_7
/* CMP.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_b07c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_b080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.L An,Dn */
void REGPARAM2 CPUFUNC(op_b088_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.L (An),Dn */
void REGPARAM2 CPUFUNC(op_b090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_b098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_b0a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMP.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_b0a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b0b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMP.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_b0b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_b0b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_b0ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMP.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_b0bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMP.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_b0bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMPA.W Dn,An */
void REGPARAM2 CPUFUNC(op_b0c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPA.W An,An */
void REGPARAM2 CPUFUNC(op_b0c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPA.W (An),An */
void REGPARAM2 CPUFUNC(op_b0d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_b0d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPA.W -(An),An */
void REGPARAM2 CPUFUNC(op_b0e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_b0e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMPA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_b0f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMPA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_b0f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMPA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_b0f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMPA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_b0fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMPA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_b0fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMPA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_b0fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_b100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPM.B (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_b108_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_b110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b138_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b139_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_b140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPM.W (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_b148_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_b150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b178_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b179_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_b180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPM.L (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_b188_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_b190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b1a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b1a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b1b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b1b8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b1b9_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMPA.L Dn,An */
void REGPARAM2 CPUFUNC(op_b1c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPA.L An,An */
void REGPARAM2 CPUFUNC(op_b1c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPA.L (An),An */
void REGPARAM2 CPUFUNC(op_b1d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_b1d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPA.L -(An),An */
void REGPARAM2 CPUFUNC(op_b1e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CMPA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_b1e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMPA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_b1f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMPA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_b1f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMPA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_b1f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMPA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_b1fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CMPA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_b1fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CMPA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_b1fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_c000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B (An),Dn */
void REGPARAM2 CPUFUNC(op_c010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_c018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_c020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_c028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_c038_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_c039_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_c03a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c03b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_c03c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_c040_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W (An),Dn */
void REGPARAM2 CPUFUNC(op_c050_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_c058_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_c060_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_c068_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c070_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_c078_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_c079_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_c07a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c07b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_c07c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_c080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L (An),Dn */
void REGPARAM2 CPUFUNC(op_c090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_c098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_c0a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_c0a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c0b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_c0b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_c0b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_c0ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c0bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_c0bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MULU.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_c0c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}return;
}

/* MULU.W (An),Dn */
void REGPARAM2 CPUFUNC(op_c0d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}return;
}

/* MULU.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_c0d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}return;
}

/* MULU.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_c0e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}return;
}

/* MULU.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_c0e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}return;
}

/* MULU.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c0f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}return;
}

/* MULU.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_c0f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}return;
}

/* MULU.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_c0f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (6);
}}}}}return;
}

/* MULU.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_c0fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}return;
}

/* MULU.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c0fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}return;
}

/* MULU.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_c0fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}return;
}

/* ABCD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_c100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{	uae_u16 newv_lo = (src & 0xF) + (dst & 0xF) + (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = (src & 0xF0) + (dst & 0xF0);
	uae_u16 newv, tmp_newv;
	int cflg;
	newv = tmp_newv = newv_hi + newv_lo;	if (newv_lo > 9) { newv += 6; }
	cflg = (newv & 0x3F0) > 0x90;
	if (cflg) newv += 0x60;
	SET_CFLG (cflg);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	SET_VFLG ((tmp_newv & 0x80) == 0 && (newv & 0x80) != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ABCD.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_c108_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	uae_u16 newv_lo = (src & 0xF) + (dst & 0xF) + (GET_XFLG () ? 1 : 0);
	uae_u16 newv_hi = (src & 0xF0) + (dst & 0xF0);
	uae_u16 newv, tmp_newv;
	int cflg;
	newv = tmp_newv = newv_hi + newv_lo;	if (newv_lo > 9) { newv += 6; }
	cflg = (newv & 0x3F0) > 0x90;
	if (cflg) newv += 0x60;
	SET_CFLG (cflg);
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	SET_VFLG ((tmp_newv & 0x80) == 0 && (newv & 0x80) != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_c110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c138_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c139_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EXG.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_c140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_dreg (regs, srcreg) = (dst);
	m68k_dreg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EXG.L An,An */
void REGPARAM2 CPUFUNC(op_c148_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	m68k_areg (regs, srcreg) = (dst);
	m68k_areg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_c150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c178_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c179_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EXG.L Dn,An */
void REGPARAM2 CPUFUNC(op_c188_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	m68k_dreg (regs, srcreg) = (dst);
	m68k_areg (regs, dstreg) = (src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_c190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c1a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c1a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c1b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c1b8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c1b9_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce020 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MULS.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_c1c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return;
}

/* MULS.W (An),Dn */
void REGPARAM2 CPUFUNC(op_c1d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}	m68k_incpc (2);
return;
}

/* MULS.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_c1d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}	m68k_incpc (2);
return;
}

/* MULS.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_c1e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}	m68k_incpc (2);
return;
}

/* MULS.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_c1e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}	m68k_incpc (4);
return;
}

/* MULS.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c1f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}return;
}

/* MULS.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_c1f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}	m68k_incpc (4);
return;
}

/* MULS.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_c1f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}	m68k_incpc (6);
return;
}

/* MULS.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_c1fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}	m68k_incpc (4);
return;
}

/* MULS.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_c1fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}return;
}

/* MULS.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_c1fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	regs.ce020memcycles += 20 * cpucycleunit;
	m68k_dreg (regs, dstreg) = (newv);
}}}}	m68k_incpc (4);
return;
}

/* ADD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_d000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B (An),Dn */
void REGPARAM2 CPUFUNC(op_d010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_d018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B -(An),Dn */
void REGPARAM2 CPUFUNC(op_d020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_d028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_d038_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_d039_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_d03a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d03b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s8 src = get_byte_ce020 (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_d03c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_d040_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W An,Dn */
void REGPARAM2 CPUFUNC(op_d048_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W (An),Dn */
void REGPARAM2 CPUFUNC(op_d050_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_d058_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_d060_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_d068_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d070_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_d078_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_d079_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_d07a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d07b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_d07c_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_d080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L An,Dn */
void REGPARAM2 CPUFUNC(op_d088_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L (An),Dn */
void REGPARAM2 CPUFUNC(op_d090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_d098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L -(An),Dn */
void REGPARAM2 CPUFUNC(op_d0a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_d0a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d0b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_d0b8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_d0b9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_d0ba_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_d0bb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_d0bc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDA.W Dn,An */
void REGPARAM2 CPUFUNC(op_d0c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W An,An */
void REGPARAM2 CPUFUNC(op_d0c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W (An),An */
void REGPARAM2 CPUFUNC(op_d0d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_d0d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W -(An),An */
void REGPARAM2 CPUFUNC(op_d0e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_d0e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_d0f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_d0f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_d0f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_d0fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_d0fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s16 src = get_word_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_d0fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDX.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_d100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = dst + src + (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDX.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_d108_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	uae_u32 newv = dst + src + (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_d110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d138_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d139_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = get_byte_ce020 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDX.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_d140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = dst + src + (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDX.W -(An),-(An) */
void REGPARAM2 CPUFUNC(op_d148_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	uae_u32 newv = dst + src + (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_d150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) += 2;
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d178_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d179_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = get_word_ce020 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDX.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_d180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
{	uae_u32 newv = dst + src + (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDX.L -(An),-(An) */
void REGPARAM2 CPUFUNC(op_d188_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{	uae_u32 newv = dst + src + (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_d190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) += 4;
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d1a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce020 (dsta);
	m68k_areg (regs, dstreg) = dsta;
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d1a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d1b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d1b8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d1b9_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = get_long_ce020 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce020 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDA.L Dn,An */
void REGPARAM2 CPUFUNC(op_d1c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L An,An */
void REGPARAM2 CPUFUNC(op_d1c8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L (An),An */
void REGPARAM2 CPUFUNC(op_d1d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_d1d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L -(An),An */
void REGPARAM2 CPUFUNC(op_d1e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce020 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_d1e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_d1f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_d1f8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_d1f9_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_d1fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_d1fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_s32 src = get_long_ce020 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_d1fc_20)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ASRQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e000_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	uae_u32 sign = (0x80 & val) >> 7;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 8) {
		val = 0xff & (uae_u32)-sign;
		SET_CFLG (sign);
		COPY_CARRY ();
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
		val |= (0xff << (8 - cnt)) & (uae_u32)-sign;
		val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e008_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 8) {
		SET_CFLG ((cnt == 8) & (val >> 7));
		COPY_CARRY ();
		val = 0;
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e010_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG ();
	hival <<= (7 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e018_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	uae_u32 hival;
	cnt &= 7;
	hival = val << (8 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xff;
	SET_CFLG ((val & 0x80) >> 7);
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif

#ifdef PART_8
/* ASR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e020_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	uae_u32 sign = (0x80 & val) >> 7;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 8) {
		val = 0xff & (uae_u32)-sign;
		SET_CFLG (sign);
		COPY_CARRY ();
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
		val |= (0xff << (8 - cnt)) & (uae_u32)-sign;
		val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e028_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 8) {
		SET_CFLG ((cnt == 8) & (val >> 7));
		COPY_CARRY ();
		val = 0;
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e030_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 36) cnt -= 36;
	if (cnt >= 18) cnt -= 18;
	if (cnt >= 9) cnt -= 9;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG ();
	hival <<= (7 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e038_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt > 0) {	uae_u32 hival;
	cnt &= 7;
	hival = val << (8 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xff;
	SET_CFLG ((val & 0x80) >> 7);
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e040_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = (0x8000 & val) >> 15;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 16) {
		val = 0xffff & (uae_u32)-sign;
		SET_CFLG (sign);
		COPY_CARRY ();
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
		val |= (0xffff << (16 - cnt)) & (uae_u32)-sign;
		val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e048_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 16) {
		SET_CFLG ((cnt == 16) & (val >> 15));
		COPY_CARRY ();
		val = 0;
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e050_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG ();
	hival <<= (15 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xffff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e058_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	uae_u32 hival;
	cnt &= 15;
	hival = val << (16 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xffff;
	SET_CFLG ((val & 0x8000) >> 15);
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e060_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = (0x8000 & val) >> 15;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 16) {
		val = 0xffff & (uae_u32)-sign;
		SET_CFLG (sign);
		COPY_CARRY ();
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
		val |= (0xffff << (16 - cnt)) & (uae_u32)-sign;
		val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e068_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 16) {
		SET_CFLG ((cnt == 16) & (val >> 15));
		COPY_CARRY ();
		val = 0;
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e070_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 34) cnt -= 34;
	if (cnt >= 17) cnt -= 17;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG ();
	hival <<= (15 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xffff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e078_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt > 0) {	uae_u32 hival;
	cnt &= 15;
	hival = val << (16 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xffff;
	SET_CFLG ((val & 0x8000) >> 15);
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e080_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	uae_u32 sign = (0x80000000 & val) >> 31;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 32) {
		val = 0xffffffff & (uae_u32)-sign;
		SET_CFLG (sign);
		COPY_CARRY ();
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
		val |= (0xffffffff << (32 - cnt)) & (uae_u32)-sign;
		val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e088_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 32) {
		SET_CFLG ((cnt == 32) & (val >> 31));
		COPY_CARRY ();
		val = 0;
	} else {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e090_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG ();
	hival <<= (31 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xffffffff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e098_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	uae_u32 hival;
	cnt &= 31;
	hival = val << (32 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xffffffff;
	SET_CFLG ((val & 0x80000000) >> 31);
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e0a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	uae_u32 sign = (0x80000000 & val) >> 31;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 32) {
		val = 0xffffffff & (uae_u32)-sign;
		SET_CFLG (sign);
		COPY_CARRY ();
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
		val |= (0xffffffff << (32 - cnt)) & (uae_u32)-sign;
		val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e0a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 32) {
		SET_CFLG ((cnt == 32) & (val >> 31));
		COPY_CARRY ();
		val = 0;
	} else if (cnt > 0) {
		val >>= cnt - 1;
		SET_CFLG (val & 1);
		COPY_CARRY ();
		val >>= 1;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e0b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 33) cnt -= 33;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 hival = (val << 1) | GET_XFLG ();
	hival <<= (31 - cnt);
	val >>= cnt;
	carry = val & 1;
	val >>= 1;
	val |= hival;
	SET_XFLG (carry);
	val &= 0xffffffff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e0b8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt > 0) {	uae_u32 hival;
	cnt &= 31;
	hival = val << (32 - cnt);
	val >>= cnt;
	val |= hival;
	val &= 0xffffffff;
	SET_CFLG ((val & 0x80000000) >> 31);
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRW.W (An) */
void REGPARAM2 CPUFUNC(op_e0d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e0d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e0e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e0e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e0f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ASRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e0f8_20)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e0f9_20)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ASLQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e100_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 8) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 8 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else {
		uae_u32 mask = (0xff << (7 - cnt)) & 0xff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x80) >> 7);
		COPY_CARRY ();
		val <<= 1;
		val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e108_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 8) {
		SET_CFLG (cnt == 8 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x80) >> 7);
		COPY_CARRY ();
		val <<= 1;
	val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e110_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (7 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG ()) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e118_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	uae_u32 loval;
	cnt &= 7;
	loval = val >> (8 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASL.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e120_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 8) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 8 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else if (cnt > 0) {
		uae_u32 mask = (0xff << (7 - cnt)) & 0xff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x80) >> 7);
		COPY_CARRY ();
		val <<= 1;
		val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSL.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e128_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 8) {
		SET_CFLG (cnt == 8 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else if (cnt > 0) {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x80) >> 7);
		COPY_CARRY ();
		val <<= 1;
	val &= 0xff;
	}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXL.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e130_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 36) cnt -= 36;
	if (cnt >= 18) cnt -= 18;
	if (cnt >= 9) cnt -= 9;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (7 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG ()) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROL.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e138_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u8)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt > 0) {
	uae_u32 loval;
	cnt &= 7;
	loval = val >> (8 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s8)(val)) == 0);
	SET_NFLG (((uae_s8)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e140_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 16) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 16 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else {
		uae_u32 mask = (0xffff << (15 - cnt)) & 0xffff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x8000) >> 15);
		COPY_CARRY ();
		val <<= 1;
		val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e148_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 16) {
		SET_CFLG (cnt == 16 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x8000) >> 15);
		COPY_CARRY ();
		val <<= 1;
	val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e150_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (15 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG ()) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xffff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e158_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	uae_u32 loval;
	cnt &= 15;
	loval = val >> (16 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xffff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASL.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e160_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 16) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 16 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else if (cnt > 0) {
		uae_u32 mask = (0xffff << (15 - cnt)) & 0xffff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x8000) >> 15);
		COPY_CARRY ();
		val <<= 1;
		val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSL.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e168_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 16) {
		SET_CFLG (cnt == 16 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else if (cnt > 0) {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x8000) >> 15);
		COPY_CARRY ();
		val <<= 1;
	val &= 0xffff;
	}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXL.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e170_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 34) cnt -= 34;
	if (cnt >= 17) cnt -= 17;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (15 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG ()) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xffff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROL.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_e178_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = (uae_u16)data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt > 0) {
	uae_u32 loval;
	cnt &= 15;
	loval = val >> (16 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xffff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s16)(val)) == 0);
	SET_NFLG (((uae_s16)(val)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e180_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 32) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 32 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else {
		uae_u32 mask = (0xffffffff << (31 - cnt)) & 0xffffffff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x80000000) >> 31);
		COPY_CARRY ();
		val <<= 1;
		val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e188_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 32) {
		SET_CFLG (cnt == 32 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x80000000) >> 31);
		COPY_CARRY ();
		val <<= 1;
	val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e190_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (31 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG ()) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xffffffff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e198_20)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
{	uae_u32 loval;
	cnt &= 31;
	loval = val >> (32 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xffffffff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASL.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e1a0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 32) {
		SET_VFLG (val != 0);
		SET_CFLG (cnt == 32 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else if (cnt > 0) {
		uae_u32 mask = (0xffffffff << (31 - cnt)) & 0xffffffff;
		SET_VFLG ((val & mask) != mask && (val & mask) != 0);
		val <<= cnt - 1;
		SET_CFLG ((val & 0x80000000) >> 31);
		COPY_CARRY ();
		val <<= 1;
		val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSL.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e1a8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 32) {
		SET_CFLG (cnt == 32 ? val & 1 : 0);
		COPY_CARRY ();
		val = 0;
	} else if (cnt > 0) {
		val <<= (cnt - 1);
		SET_CFLG ((val & 0x80000000) >> 31);
		COPY_CARRY ();
		val <<= 1;
	val &= 0xffffffff;
	}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXL.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e1b0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt >= 33) cnt -= 33;
	if (cnt > 0) {
	cnt--;
	{
	uae_u32 carry;
	uae_u32 loval = val >> (31 - cnt);
	carry = loval & 1;
	val = (((val << 1) | GET_XFLG ()) << cnt) | (loval >> 1);
	SET_XFLG (carry);
	val &= 0xffffffff;
	} }
	SET_CFLG (GET_XFLG ());
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROL.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_e1b8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
{	uae_u32 val = data;
	int ccnt = cnt & 63;
	cnt &= 63;
	CLEAR_CZNV ();
	if (cnt > 0) {
	uae_u32 loval;
	cnt &= 31;
	loval = val >> (32 - cnt);
	val <<= cnt;
	val |= loval;
	val &= 0xffffffff;
	SET_CFLG (val & 1);
}
	SET_ZFLG (((uae_s32)(val)) == 0);
	SET_NFLG (((uae_s32)(val)) < 0);
	m68k_dreg (regs, dstreg) = (val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLW.W (An) */
void REGPARAM2 CPUFUNC(op_e1d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY ();
	SET_VFLG (GET_VFLG () | (sign2 != sign));
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e1d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY ();
	SET_VFLG (GET_VFLG () | (sign2 != sign));
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e1e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY ();
	SET_VFLG (GET_VFLG () | (sign2 != sign));
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e1e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY ();
	SET_VFLG (GET_VFLG () | (sign2 != sign));
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e1f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY ();
	SET_VFLG (GET_VFLG () | (sign2 != sign));
	put_word_ce020 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ASLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e1f8_20)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY ();
	SET_VFLG (GET_VFLG () | (sign2 != sign));
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e1f9_20)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 sign2;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	sign2 = 0x8000 & val;
	SET_CFLG (sign != 0);
	COPY_CARRY ();
	SET_VFLG (GET_VFLG () | (sign2 != sign));
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* LSRW.W (An) */
void REGPARAM2 CPUFUNC(op_e2d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e2d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e2e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e2e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e2f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LSRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e2f8_20)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e2f9_20)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* LSLW.W (An) */
void REGPARAM2 CPUFUNC(op_e3d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e3d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e3e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e3e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e3f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LSLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e3f8_20)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e3f9_20)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ROXRW.W (An) */
void REGPARAM2 CPUFUNC(op_e4d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e4d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e4e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e4e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e4f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ROXRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e4f8_20)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e4f9_20)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ROXLW.W (An) */
void REGPARAM2 CPUFUNC(op_e5d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e5d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e5e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e5e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e5f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ROXLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e5f8_20)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e5f9_20)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* RORW.W (An) */
void REGPARAM2 CPUFUNC(op_e6d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e6d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORW.W -(An) */
void REGPARAM2 CPUFUNC(op_e6e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e6e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* RORW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e6f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce020 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* RORW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e6f8_20)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* RORW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e6f9_20)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ROLW.W (An) */
void REGPARAM2 CPUFUNC(op_e7d0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e7d8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e7e0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce020 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e7e8_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e7f0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce020 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ROLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e7f8_20)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e7f9_20)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = get_word_ce020 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce020 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BFTST.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e8c0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp = m68k_dreg(regs, dstreg);
	offset &= 0x1f;
	tmp = (tmp << offset) | (tmp >> (32 - offset));
	bdata[0] = tmp & ((1 << (32 - width)) - 1);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFTST.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e8d0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFTST.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e8e8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFTST.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e8f0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFTST.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e8f8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFTST.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e8f9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* BFTST.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e8fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFTST.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e8fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFEXTU.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e9c0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp = m68k_dreg(regs, dstreg);
	offset &= 0x1f;
	tmp = (tmp << offset) | (tmp >> (32 - offset));
	bdata[0] = tmp & ((1 << (32 - width)) - 1);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFEXTU.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e9d0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFEXTU.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e9e8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFEXTU.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e9f0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFEXTU.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e9f8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFEXTU.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e9f9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* BFEXTU.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e9fa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFEXTU.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e9fb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFCHG.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eac0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp = m68k_dreg(regs, dstreg);
	offset &= 0x1f;
	tmp = (tmp << offset) | (tmp >> (32 - offset));
	bdata[0] = tmp & ((1 << (32 - width)) - 1);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = tmp ^ (0xffffffffu >> (32 - width));
	tmp = bdata[0] | (tmp << (32 - width));
	m68k_dreg(regs, dstreg) = (tmp >> offset) | (tmp << (32 - offset));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFCHG.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ead0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = tmp ^ (0xffffffffu >> (32 - width));
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFCHG.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eae8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = tmp ^ (0xffffffffu >> (32 - width));
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFCHG.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eaf0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = tmp ^ (0xffffffffu >> (32 - width));
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFCHG.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eaf8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = tmp ^ (0xffffffffu >> (32 - width));
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFCHG.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eaf9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = tmp ^ (0xffffffffu >> (32 - width));
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* BFEXTS.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ebc0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp = m68k_dreg(regs, dstreg);
	offset &= 0x1f;
	tmp = (tmp << offset) | (tmp >> (32 - offset));
	bdata[0] = tmp & ((1 << (32 - width)) - 1);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp = (uae_s32)tmp >> (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFEXTS.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ebd0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp = (uae_s32)tmp >> (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFEXTS.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ebe8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp = (uae_s32)tmp >> (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFEXTS.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ebf0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp = (uae_s32)tmp >> (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFEXTS.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ebf8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp = (uae_s32)tmp >> (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFEXTS.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ebf9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp = (uae_s32)tmp >> (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* BFEXTS.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ebfa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp = (uae_s32)tmp >> (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFEXTS.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ebfb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp = (uae_s32)tmp >> (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	m68k_dreg (regs, (extra >> 12) & 7) = tmp;
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFCLR.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ecc0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp = m68k_dreg(regs, dstreg);
	offset &= 0x1f;
	tmp = (tmp << offset) | (tmp >> (32 - offset));
	bdata[0] = tmp & ((1 << (32 - width)) - 1);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0;
	tmp = bdata[0] | (tmp << (32 - width));
	m68k_dreg(regs, dstreg) = (tmp >> offset) | (tmp << (32 - offset));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFCLR.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ecd0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0;
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFCLR.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ece8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0;
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFCLR.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ecf0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0;
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFCLR.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ecf8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0;
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFCLR.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ecf9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0;
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* BFFFO.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_edc0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp = m68k_dreg(regs, dstreg);
	offset &= 0x1f;
	tmp = (tmp << offset) | (tmp >> (32 - offset));
	bdata[0] = tmp & ((1 << (32 - width)) - 1);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	{ uae_u32 mask = 1 << (width - 1);
	while (mask) { if (tmp & mask) break; mask >>= 1; offset++; }}
	m68k_dreg (regs, (extra >> 12) & 7) = offset;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFFFO.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_edd0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	{ uae_u32 mask = 1 << (width - 1);
	while (mask) { if (tmp & mask) break; mask >>= 1; offset++; }}
	m68k_dreg (regs, (extra >> 12) & 7) = offset;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFFFO.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_ede8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	{ uae_u32 mask = 1 << (width - 1);
	while (mask) { if (tmp & mask) break; mask >>= 1; offset++; }}
	m68k_dreg (regs, (extra >> 12) & 7) = offset;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFFFO.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_edf0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	{ uae_u32 mask = 1 << (width - 1);
	while (mask) { if (tmp & mask) break; mask >>= 1; offset++; }}
	m68k_dreg (regs, (extra >> 12) & 7) = offset;
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFFFO.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_edf8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	{ uae_u32 mask = 1 << (width - 1);
	while (mask) { if (tmp & mask) break; mask >>= 1; offset++; }}
	m68k_dreg (regs, (extra >> 12) & 7) = offset;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFFFO.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_edf9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	{ uae_u32 mask = 1 << (width - 1);
	while (mask) { if (tmp & mask) break; mask >>= 1; offset++; }}
	m68k_dreg (regs, (extra >> 12) & 7) = offset;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* BFFFO.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_edfa_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	{ uae_u32 mask = 1 << (width - 1);
	while (mask) { if (tmp & mask) break; mask >>= 1; offset++; }}
	m68k_dreg (regs, (extra >> 12) & 7) = offset;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFFFO.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_edfb_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	{ uae_u32 mask = 1 << (width - 1);
	while (mask) { if (tmp & mask) break; mask >>= 1; offset++; }}
	m68k_dreg (regs, (extra >> 12) & 7) = offset;
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFSET.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eec0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp = m68k_dreg(regs, dstreg);
	offset &= 0x1f;
	tmp = (tmp << offset) | (tmp >> (32 - offset));
	bdata[0] = tmp & ((1 << (32 - width)) - 1);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0xffffffffu >> (32 - width);
	tmp = bdata[0] | (tmp << (32 - width));
	m68k_dreg(regs, dstreg) = (tmp >> offset) | (tmp << (32 - offset));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFSET.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eed0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0xffffffffu >> (32 - width);
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFSET.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eee8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0xffffffffu >> (32 - width);
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFSET.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eef0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0xffffffffu >> (32 - width);
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFSET.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eef8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0xffffffffu >> (32 - width);
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFSET.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eef9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = 0xffffffffu >> (32 - width);
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* BFINS.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_efc0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp = m68k_dreg(regs, dstreg);
	offset &= 0x1f;
	tmp = (tmp << offset) | (tmp >> (32 - offset));
	bdata[0] = tmp & ((1 << (32 - width)) - 1);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = m68k_dreg (regs, (extra >> 12) & 7);
	tmp = tmp & (0xffffffffu >> (32 - width));
	SET_NFLG (tmp & (1 << (width - 1)) ? 1 : 0);
	SET_ZFLG (tmp == 0);
	tmp = bdata[0] | (tmp << (32 - width));
	m68k_dreg(regs, dstreg) = (tmp >> offset) | (tmp << (32 - offset));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFINS.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_efd0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = m68k_dreg (regs, (extra >> 12) & 7);
	tmp = tmp & (0xffffffffu >> (32 - width));
	SET_NFLG (tmp & (1 << (width - 1)) ? 1 : 0);
	SET_ZFLG (tmp == 0);
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* BFINS.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_efe8_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = m68k_dreg (regs, (extra >> 12) & 7);
	tmp = tmp & (0xffffffffu >> (32 - width));
	SET_NFLG (tmp & (1 << (width - 1)) ? 1 : 0);
	SET_ZFLG (tmp == 0);
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFINS.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eff0_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = m68k_dreg (regs, (extra >> 12) & 7);
	tmp = tmp & (0xffffffffu >> (32 - width));
	SET_NFLG (tmp & (1 << (width - 1)) ? 1 : 0);
	SET_ZFLG (tmp == 0);
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BFINS.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eff8_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = m68k_dreg (regs, (extra >> 12) & 7);
	tmp = tmp & (0xffffffffu >> (32 - width));
	SET_NFLG (tmp & (1 << (width - 1)) ? 1 : 0);
	SET_ZFLG (tmp == 0);
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* BFINS.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_eff9_20)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_u32 bdata[2];
	uae_s32 offset = extra & 0x800 ? m68k_dreg(regs, (extra >> 6) & 7) : (extra >> 6) & 0x1f;
	int width = (((extra & 0x20 ? m68k_dreg(regs, extra & 7) : extra) -1) & 0x1f) +1;
	uae_u32 tmp;
	dsta += offset >> 3;
	tmp = x_get_bitfield (dsta, bdata, offset, width);
	SET_NFLG_ALWAYS (((uae_s32)tmp) < 0 ? 1 : 0);
	tmp >>= (32 - width);
	SET_ZFLG (tmp == 0); SET_VFLG (0); SET_CFLG (0);
	tmp = m68k_dreg (regs, (extra >> 12) & 7);
	tmp = tmp & (0xffffffffu >> (32 - width));
	SET_NFLG (tmp & (1 << (width - 1)) ? 1 : 0);
	SET_ZFLG (tmp == 0);
	x_put_bitfield(dsta, bdata, tmp, offset, width);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* FPP.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f200_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f208_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f210_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f218_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f220_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f228_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f230_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f238_20)(uae_u32 opcode)
{
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f239_20)(uae_u32 opcode)
{
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f23a_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f23b_20)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FPP.L #<data>.W,#<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f23c_20)(uae_u32 opcode)
{
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_arithmetic(opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FScc.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f240_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_scc (opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FDBcc.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f248_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_dbcc (opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FScc.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f250_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_scc (opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FScc.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f258_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_scc (opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FScc.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f260_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_scc (opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FScc.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f268_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_scc (opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FScc.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f270_20)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_scc (opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FScc.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f278_20)(uae_u32 opcode)
{
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_scc (opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FScc.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f279_20)(uae_u32 opcode)
{
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_scc (opcode, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FTRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f27a_20)(uae_u32 opcode)
{
{
#ifdef FPUEMU
	uaecptr oldpc = m68k_getpc ();
	uae_u16 extra = get_word_ce020_prefetch (2);
{	uae_s16 dummy = get_word_ce020_prefetch (4);
	m68k_incpc (6);
	fpuop_trapcc (opcode, oldpc, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FTRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f27b_20)(uae_u32 opcode)
{
{
#ifdef FPUEMU
	uaecptr oldpc = m68k_getpc ();
	uae_u16 extra = get_word_ce020_prefetch (2);
{	uae_s32 dummy = get_long_ce020_prefetch (4);
	m68k_incpc (8);
	fpuop_trapcc (opcode, oldpc, extra);
}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FTRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f27c_20)(uae_u32 opcode)
{
{
#ifdef FPUEMU
	uaecptr oldpc = m68k_getpc ();
	uae_u16 extra = get_word_ce020_prefetch (2);
	m68k_incpc (4);
	fpuop_trapcc (opcode, oldpc, extra);

#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FBccQ.L #<data>,#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f280_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 63);
{
#ifdef FPUEMU
	m68k_incpc (2);
{	uaecptr pc = m68k_getpc ();
{	uae_s16 extra = get_word_ce020_prefetch (0);
	m68k_incpc (2);
	fpuop_bcc (opcode, pc,extra);
}}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FBccQ.L #<data>,#<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f2c0_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 63);
{
#ifdef FPUEMU
	m68k_incpc (2);
{	uaecptr pc = m68k_getpc ();
{	uae_s32 extra = get_long_ce020_prefetch (0);
	m68k_incpc (4);
	fpuop_bcc (opcode, pc,extra);
}}
#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FSAVE.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f310_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7108; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7108: ;
return;
}

#endif
/* FSAVE.L -(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f320_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7109; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7109: ;
return;
}

#endif
/* FSAVE.L (d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f328_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7110; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7110: ;
return;
}

#endif
/* FSAVE.L (d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f330_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7111; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7111: ;
return;
}

#endif
/* FSAVE.L (xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f338_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7112; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7112: ;
return;
}

#endif
/* FSAVE.L (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f339_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7113; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7113: ;
return;
}

#endif
/* FRESTORE.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f350_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7114; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7114: ;
return;
}

#endif
/* FRESTORE.L (An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f358_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7115; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7115: ;
return;
}

#endif
/* FRESTORE.L (d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f368_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7116; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7116: ;
return;
}

#endif
/* FRESTORE.L (d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f370_20)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7117; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7117: ;
return;
}

#endif
/* FRESTORE.L (xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f378_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7118; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7118: ;
return;
}

#endif
/* FRESTORE.L (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f379_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7119; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7119: ;
return;
}

#endif
/* FRESTORE.L (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f37a_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7120; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7120: ;
return;
}

#endif
/* FRESTORE.L (d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f37b_20)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7121; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7121: ;
return;
}

#endif
#endif

