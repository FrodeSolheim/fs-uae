#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"
#include "uae/memory.h"
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
void REGPARAM2 CPUFUNC(op_0000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0010_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0018_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0020_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0028_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0030_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0038_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0039_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ORSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_003c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0050_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0058_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0060_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0068_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0070_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0078_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0079_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ORSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_007c_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8990; }
{	MakeSR ();
{	uae_s16 src = get_word_ce020_prefetch (2);
	regs.sr |= src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8990: ;
return;
}

/* OR.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0080_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0090_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0098_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_00a0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_00a8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* OR.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_00b0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_00b8_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* OR.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_00b9_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CHK2.B #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00d0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)x_get_byte (dsta); upper = (uae_s32)(uae_s8)x_get_byte (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel8999; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8999: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00e8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)x_get_byte (dsta); upper = (uae_s32)(uae_s8)x_get_byte (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9000; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9000: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00f0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)x_get_byte (dsta); upper = (uae_s32)(uae_s8)x_get_byte (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9001; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9001: ;
return;
}

#endif
/* CHK2.B #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00f8_22)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)x_get_byte (dsta); upper = (uae_s32)(uae_s8)x_get_byte (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9002; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9002: ;
return;
}

#endif
/* CHK2.B #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00f9_22)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)x_get_byte (dsta); upper = (uae_s32)(uae_s8)x_get_byte (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9003; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel9003: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)x_get_byte (dsta); upper = (uae_s32)(uae_s8)x_get_byte (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9004; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9004: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)x_get_byte (dsta); upper = (uae_s32)(uae_s8)x_get_byte (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9005; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9005: ;
return;
}

#endif
/* BTST.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0100_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0108_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_u16 val = (x_get_byte (memp) << 8) + x_get_byte (memp + 2);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BTST.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BTST.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_0120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BTST.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_0128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_0138_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_0139_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_013a_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_013b_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B Dn,#<data>.B */
void REGPARAM2 CPUFUNC(op_013c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0140_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0148_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_u32 val = (x_get_byte (memp) << 24) + (x_get_byte (memp + 2) << 16)
              + (x_get_byte (memp + 4) << 8) + x_get_byte (memp + 6);
	m68k_dreg (regs, dstreg) = (val);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0150_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCHG.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0158_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCHG.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_0160_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCHG.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_0168_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0170_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCHG.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_0178_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_0179_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_017a_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_017b_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0180_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0188_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	uaecptr memp = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	x_put_byte (memp, src >> 8); x_put_byte (memp + 2, src);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCLR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCLR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_01a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCLR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_01a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_01b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_01b8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_01b9_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_01ba_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_01bb_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_01c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_01c8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	uaecptr memp = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	x_put_byte (memp, src >> 24); x_put_byte (memp + 2, src >> 16);
	x_put_byte (memp + 4, src >> 8); x_put_byte (memp + 6, src);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_01d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BSET.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_01d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BSET.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_01e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BSET.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_01e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_01f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_01f8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_01f9_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_01fa_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_01fb_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0200_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0210_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0218_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0220_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0228_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0230_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0238_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0239_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ANDSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_023c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0240_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0250_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0258_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0260_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0268_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0270_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0278_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0279_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ANDSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_027c_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9068; }
{	MakeSR ();
{	uae_s16 src = get_word_ce020_prefetch (2);
	regs.sr &= src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9068: ;
return;
}

/* AND.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0280_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0290_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0298_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_02a0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_02a8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* AND.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_02b0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_02b8_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* AND.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_02b9_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CHK2.W #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02d0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)x_get_word (dsta); upper = (uae_s32)(uae_s16)x_get_word (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9077; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9077: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02e8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)x_get_word (dsta); upper = (uae_s32)(uae_s16)x_get_word (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9078; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9078: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02f0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)x_get_word (dsta); upper = (uae_s32)(uae_s16)x_get_word (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9079; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9079: ;
return;
}

#endif
/* CHK2.W #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02f8_22)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)x_get_word (dsta); upper = (uae_s32)(uae_s16)x_get_word (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9080; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9080: ;
return;
}

#endif
/* CHK2.W #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02f9_22)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)x_get_word (dsta); upper = (uae_s32)(uae_s16)x_get_word (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9081; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel9081: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)x_get_word (dsta); upper = (uae_s32)(uae_s16)x_get_word (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9082; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9082: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)x_get_word (dsta); upper = (uae_s32)(uae_s16)x_get_word (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9083; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9083: ;
return;
}

#endif
/* SUB.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0400_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0410_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0418_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0420_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0428_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0430_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0438_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0439_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0440_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0450_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0458_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0460_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0468_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0470_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0478_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0479_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0480_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0490_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0498_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_04a0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_04a8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_04b0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_04b8_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_04b9_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CHK2.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04d0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = x_get_long (dsta); upper = x_get_long (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9108; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9108: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04e8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = x_get_long (dsta); upper = x_get_long (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9109; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9109: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04f0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = x_get_long (dsta); upper = x_get_long (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9110; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9110: ;
return;
}

#endif
/* CHK2.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04f8_22)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = x_get_long (dsta); upper = x_get_long (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9111; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9111: ;
return;
}

#endif
/* CHK2.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04f9_22)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = x_get_long (dsta); upper = x_get_long (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9112; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel9112: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = x_get_long (dsta); upper = x_get_long (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9113; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9113: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = x_get_long (dsta); upper = x_get_long (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel9114; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9114: ;
return;
}

#endif
/* ADD.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0600_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0610_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0618_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0620_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0628_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0630_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0638_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0639_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0640_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0650_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0658_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0660_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0668_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0670_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0678_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0679_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0680_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0690_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0698_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_06a0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_06a8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_06b0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_06b8_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_06b9_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* RTM.L Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_06c8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_06d0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_06e8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_06f0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_06f8_22)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06f9_22)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06fa_22)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06fb_22)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BTST.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0800_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0810_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0818_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0820_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0828_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0830_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0838_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0839_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BTST.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_083a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_083b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B #<data>.W,#<data>.B */
void REGPARAM2 CPUFUNC(op_083c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0840_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0850_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0858_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0860_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0868_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0870_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCHG.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0878_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0879_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BCHG.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_087a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_087b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0880_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0890_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0898_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_08a0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_08a8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_08b0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_08b8_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_08b9_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BCLR.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_08ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_08bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_08c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_08d0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_08d8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_08e0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_08e8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_08f0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_08f8_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_08f9_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BSET.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_08fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_08fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	x_put_byte (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0a00_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0a10_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0a18_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0a20_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0a28_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0a30_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0a38_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0a39_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EORSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_0a3c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0a40_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0a50_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0a58_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0a60_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0a68_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0a70_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0a78_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0a79_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EORSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_0a7c_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9206; }
{	MakeSR ();
{	uae_s16 src = get_word_ce020_prefetch (2);
	regs.sr ^= src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9206: ;
return;
}

/* EOR.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0a80_22)(uae_u32 opcode)
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

#endif

#ifdef PART_2
/* EOR.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_0a90_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0a98_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_0aa0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_0aa8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EOR.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0ab0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_0ab8_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EOR.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_0ab9_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CAS.B #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ad0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
	if (GET_ZFLG ()){	x_put_byte (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.B #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ad8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
	if (GET_ZFLG ()){	x_put_byte (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.B #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ae0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	if (GET_ZFLG ()){	x_put_byte (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.B #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ae8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
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
	if (GET_ZFLG ()){	x_put_byte (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.B #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0af0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
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
	if (GET_ZFLG ()){	x_put_byte (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CAS.B #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0af8_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
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
	if (GET_ZFLG ()){	x_put_byte (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.B #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0af9_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
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
	if (GET_ZFLG ()){	x_put_byte (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* CMP.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0c00_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0c10_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
void REGPARAM2 CPUFUNC(op_0c18_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
void REGPARAM2 CPUFUNC(op_0c20_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
void REGPARAM2 CPUFUNC(op_0c28_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
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
void REGPARAM2 CPUFUNC(op_0c30_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
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
void REGPARAM2 CPUFUNC(op_0c38_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
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
void REGPARAM2 CPUFUNC(op_0c39_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
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
void REGPARAM2 CPUFUNC(op_0c3a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 dst = x_get_byte (dsta);
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
void REGPARAM2 CPUFUNC(op_0c3b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
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
void REGPARAM2 CPUFUNC(op_0c40_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0c50_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
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
void REGPARAM2 CPUFUNC(op_0c58_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
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
void REGPARAM2 CPUFUNC(op_0c60_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
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
void REGPARAM2 CPUFUNC(op_0c68_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
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
void REGPARAM2 CPUFUNC(op_0c70_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
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
void REGPARAM2 CPUFUNC(op_0c78_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
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
void REGPARAM2 CPUFUNC(op_0c79_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
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
void REGPARAM2 CPUFUNC(op_0c7a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
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
void REGPARAM2 CPUFUNC(op_0c7b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
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
void REGPARAM2 CPUFUNC(op_0c80_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0c90_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
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
void REGPARAM2 CPUFUNC(op_0c98_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
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
void REGPARAM2 CPUFUNC(op_0ca0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
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
void REGPARAM2 CPUFUNC(op_0ca8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
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
void REGPARAM2 CPUFUNC(op_0cb0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
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
void REGPARAM2 CPUFUNC(op_0cb8_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
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
void REGPARAM2 CPUFUNC(op_0cb9_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
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
void REGPARAM2 CPUFUNC(op_0cba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 6;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
{	uae_s32 dst = x_get_long (dsta);
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
void REGPARAM2 CPUFUNC(op_0cbb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (6);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
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
void REGPARAM2 CPUFUNC(op_0cd0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	if ((dsta & 1) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9252;
	}
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
	if (GET_ZFLG ()){	x_put_word (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9252: ;
return;
}

#endif
/* CAS.W #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cd8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	if ((dsta & 1) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9253;
	}
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
	if (GET_ZFLG ()){	x_put_word (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9253: ;
return;
}

#endif
/* CAS.W #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ce0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	if ((dsta & 1) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9254;
	}
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
	if (GET_ZFLG ()){	x_put_word (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9254: ;
return;
}

#endif
/* CAS.W #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ce8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	if ((dsta & 1) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9255;
	}
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
	if (GET_ZFLG ()){	x_put_word (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9255: ;
return;
}

#endif
/* CAS.W #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cf0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
	if ((dsta & 1) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9256;
	}
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
	if (GET_ZFLG ()){	x_put_word (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9256: ;
return;
}

#endif
/* CAS.W #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cf8_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	if ((dsta & 1) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9257;
	}
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
	if (GET_ZFLG ()){	x_put_word (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9257: ;
return;
}

#endif
/* CAS.W #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cf9_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s16 dst = x_get_word (dsta);
	if ((dsta & 1) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9258;
	}
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
	if (GET_ZFLG ()){	x_put_word (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel9258: ;
return;
}

#endif
/* CAS2.W #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cfc_22)(uae_u32 opcode)
{
{{	uae_s32 extra = get_long_ce020_prefetch (2);
	uae_u32 rn1 = regs.regs[(extra >> 28) & 15];
	uae_u32 rn2 = regs.regs[(extra >> 12) & 15];
	uae_u16 dst1 = x_get_word (rn1), dst2 = x_get_word (rn2);
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
	x_put_word (rn1, m68k_dreg (regs, (extra >> 22) & 7));
	x_put_word (rn1, m68k_dreg (regs, (extra >> 6) & 7));
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
void REGPARAM2 CPUFUNC(op_0e10_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9260; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	x_put_byte (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s8 src = x_get_byte (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
}}}	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9260: ;
return;
}

#endif
/* MOVES.B #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e18_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9261; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	x_put_byte (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
}}}	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9261: ;
return;
}

#endif
/* MOVES.B #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e20_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9262; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	x_put_byte (dsta, src);
}}else{{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, dstreg) = srca;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
}}}	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9262: ;
return;
}

#endif
/* MOVES.B #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e28_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9263; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	x_put_byte (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 src = x_get_byte (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
}}}	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9263: ;
return;
}

#endif
/* MOVES.B #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e30_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9264; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	x_put_byte (dsta, src);
}}}else{{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9264: ;
return;
}

#endif
/* MOVES.B #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e38_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9265; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	x_put_byte (dsta, src);
}}else{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s8 src = x_get_byte (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
}}}	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9265: ;
return;
}

#endif
/* MOVES.B #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e39_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9266; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = get_long_ce020_prefetch (4);
	x_put_byte (dsta, src);
}}else{{	uaecptr srca = get_long_ce020_prefetch (4);
{	uae_s8 src = x_get_byte (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
}}}	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9266: ;
return;
}

#endif
/* MOVES.W #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e50_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9267; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	x_put_word (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s16 src = x_get_word (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
}}}	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9267: ;
return;
}

#endif
/* MOVES.W #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e58_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9268; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	x_put_word (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, dstreg) += 2;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
}}}	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9268: ;
return;
}

#endif
/* MOVES.W #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e60_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9269; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	x_put_word (dsta, src);
}}else{{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, dstreg) = srca;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
}}}	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9269: ;
return;
}

#endif
/* MOVES.W #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e68_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9270; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	x_put_word (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 src = x_get_word (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
}}}	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9270: ;
return;
}

#endif
/* MOVES.W #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e70_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9271; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	x_put_word (dsta, src);
}}}else{{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9271: ;
return;
}

#endif
/* MOVES.W #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e78_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9272; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	x_put_word (dsta, src);
}}else{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s16 src = x_get_word (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
}}}	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9272: ;
return;
}

#endif
/* MOVES.W #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e79_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9273; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = get_long_ce020_prefetch (4);
	x_put_word (dsta, src);
}}else{{	uaecptr srca = get_long_ce020_prefetch (4);
{	uae_s16 src = x_get_word (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
}}}	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9273: ;
return;
}

#endif
/* MOVES.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e90_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9274; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	x_put_long (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s32 src = x_get_long (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
}}}	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9274: ;
return;
}

#endif
/* MOVES.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e98_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9275; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	x_put_long (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, dstreg) += 4;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
}}}	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9275: ;
return;
}

#endif
/* MOVES.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ea0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9276; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	x_put_long (dsta, src);
}}else{{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, dstreg) = srca;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
}}}	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9276: ;
return;
}

#endif
/* MOVES.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ea8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9277; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	x_put_long (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 src = x_get_long (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
}}}	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9277: ;
return;
}

#endif
/* MOVES.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0eb0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel9278; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	x_put_long (dsta, src);
}}}else{{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9278: ;
return;
}

#endif
/* MOVES.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0eb8_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9279; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	x_put_long (dsta, src);
}}else{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 src = x_get_long (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
}}}	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9279: ;
return;
}

#endif
/* MOVES.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0eb9_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9280; }
{{	uae_s16 extra = get_word_ce020_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = get_long_ce020_prefetch (4);
	x_put_long (dsta, src);
}}else{{	uaecptr srca = get_long_ce020_prefetch (4);
{	uae_s32 src = x_get_long (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
}}}	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9280: ;
return;
}

#endif
/* CAS.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ed0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	if ((dsta & 3) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9281;
	}
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
	if (GET_ZFLG ()){	x_put_long (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9281: ;
return;
}

#endif
/* CAS.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ed8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	if ((dsta & 3) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9282;
	}
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
	if (GET_ZFLG ()){	x_put_long (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9282: ;
return;
}

#endif
/* CAS.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ee0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	if ((dsta & 3) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9283;
	}
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
	if (GET_ZFLG ()){	x_put_long (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9283: ;
return;
}

#endif
/* CAS.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ee8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	if ((dsta & 3) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9284;
	}
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
	if (GET_ZFLG ()){	x_put_long (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9284: ;
return;
}

#endif
/* CAS.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ef0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	if ((dsta & 3) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9285;
	}
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
	if (GET_ZFLG ()){	x_put_long (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9285: ;
return;
}

#endif
/* CAS.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ef8_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	if ((dsta & 3) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9286;
	}
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
	if (GET_ZFLG ()){	x_put_long (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9286: ;
return;
}

#endif
/* CAS.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ef9_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	if ((dsta & 3) && currprefs.cpu_compatible && get_cpu_model () == 68060) {
		op_unimpl ();
		goto endlabel9287;
	}
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
	if (GET_ZFLG ()){	x_put_long (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel9287: ;
return;
}

#endif
/* CAS2.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0efc_22)(uae_u32 opcode)
{
{{	uae_s32 extra = get_long_ce020_prefetch (2);
	uae_u32 rn1 = regs.regs[(extra >> 28) & 15];
	uae_u32 rn2 = regs.regs[(extra >> 12) & 15];
	uae_u32 dst1 = x_get_long (rn1), dst2 = x_get_long (rn2);
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
	x_put_long (rn1, m68k_dreg (regs, (extra >> 22) & 7));
	x_put_long (rn1, m68k_dreg (regs, (extra >> 6) & 7));
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
void REGPARAM2 CPUFUNC(op_1000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_1010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_1018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_1020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_1028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_1030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_1038_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_1039_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_103a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_103b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_103c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_1080_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(An) */
void REGPARAM2 CPUFUNC(op_1090_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(An) */
void REGPARAM2 CPUFUNC(op_1098_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(An) */
void REGPARAM2 CPUFUNC(op_10a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_10a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_10b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_10b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_10b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_10ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_10bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_10bc_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_10c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(An)+ */
void REGPARAM2 CPUFUNC(op_10d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_10d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_10e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_10e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_10f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_10f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_10f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_10fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_10fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_10fc_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_1100_22)(uae_u32 opcode)
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
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),-(An) */
void REGPARAM2 CPUFUNC(op_1110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_1118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_1120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_1128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_1130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_1138_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_1139_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_113a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_113b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_113c_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_1140_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1150_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_1158_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1160_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1168_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_1170_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_1178_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_1179_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_117a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_117b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_117c_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1180_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11bc_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_11c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_11d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_11f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_11f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_11f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_11fa_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_11fb_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_11fc_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_13c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_13d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_13f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_13f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_13f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = get_long_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (10);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_13fa_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_13fb_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_13fc_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_2000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_2008_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_2010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_2018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_2020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_2028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_2030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_2038_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_2039_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_203a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_203b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_203c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_2040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_2048_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_2050_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_2058_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L -(An),An */
void REGPARAM2 CPUFUNC(op_2060_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_2068_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_2070_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_2078_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_2079_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_207a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_207b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_207c_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_2080_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(An) */
void REGPARAM2 CPUFUNC(op_2088_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(An) */
void REGPARAM2 CPUFUNC(op_2090_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(An) */
void REGPARAM2 CPUFUNC(op_2098_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(An) */
void REGPARAM2 CPUFUNC(op_20a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_20a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_20b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_20b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_20b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_20ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_20bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_20bc_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_20c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(An)+ */
void REGPARAM2 CPUFUNC(op_20c8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(An)+ */
void REGPARAM2 CPUFUNC(op_20d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_20d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_20e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_20e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_20f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_20f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_20f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_20fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_20fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_20fc_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_2100_22)(uae_u32 opcode)
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
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,-(An) */
void REGPARAM2 CPUFUNC(op_2108_22)(uae_u32 opcode)
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
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),-(An) */
void REGPARAM2 CPUFUNC(op_2110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_2118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),-(An) */
void REGPARAM2 CPUFUNC(op_2120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_2128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_2130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_2138_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_2139_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_213a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_213b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_213c_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_2140_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(d16,An) */
void REGPARAM2 CPUFUNC(op_2148_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2150_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_2158_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2160_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2168_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif

#ifdef PART_3
/* MOVE.L (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_2170_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_2178_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_2179_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_217a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_217b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_217c_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2180_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2188_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21bc_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_21c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(xxx).W */
void REGPARAM2 CPUFUNC(op_21c8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_21d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_21f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_21f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_21f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_21fa_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_21fb_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_21fc_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_23c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(xxx).L */
void REGPARAM2 CPUFUNC(op_23c8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_23d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_23f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_23f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_23f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = get_long_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (10);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_23fa_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_23fb_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_23fc_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
	m68k_incpc (10);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_3000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_3008_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_3010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_3018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_3020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_3028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_3030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_3038_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_3039_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_303a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_303b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_303c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_3040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_3048_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_3050_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_3058_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W -(An),An */
void REGPARAM2 CPUFUNC(op_3060_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_3068_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_3070_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_3078_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_3079_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_307a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_307b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_307c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_3080_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(An) */
void REGPARAM2 CPUFUNC(op_3088_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(An) */
void REGPARAM2 CPUFUNC(op_3090_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(An) */
void REGPARAM2 CPUFUNC(op_3098_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(An) */
void REGPARAM2 CPUFUNC(op_30a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_30a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_30b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_30b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_30b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_30ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_30bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_30bc_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_30c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(An)+ */
void REGPARAM2 CPUFUNC(op_30c8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(An)+ */
void REGPARAM2 CPUFUNC(op_30d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_30d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_30e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_30e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_30f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_30f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_30f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_30fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_30fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_30fc_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_3100_22)(uae_u32 opcode)
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
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,-(An) */
void REGPARAM2 CPUFUNC(op_3108_22)(uae_u32 opcode)
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
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),-(An) */
void REGPARAM2 CPUFUNC(op_3110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_3118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),-(An) */
void REGPARAM2 CPUFUNC(op_3120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_3128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_3130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_3138_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_3139_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_313a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_313b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_313c_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_3140_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(d16,An) */
void REGPARAM2 CPUFUNC(op_3148_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3150_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_3158_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3160_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3168_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_3170_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_3178_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_3179_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_317a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_317b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_317c_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3180_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3188_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31bc_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_31c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(xxx).W */
void REGPARAM2 CPUFUNC(op_31c8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_31d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_31f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_31f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_31f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_31fa_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_31fb_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_31fc_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_33c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(xxx).L */
void REGPARAM2 CPUFUNC(op_33c8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_33d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_33f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_33f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_33f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = get_long_ce020_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (10);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_33fa_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_33fb_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uaecptr dsta = get_long_ce020_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_33fc_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.B Dn */
void REGPARAM2 CPUFUNC(op_4000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	x_put_byte (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.B (An)+ */
void REGPARAM2 CPUFUNC(op_4018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.B -(An) */
void REGPARAM2 CPUFUNC(op_4020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	x_put_byte (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	x_put_byte (srca, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4038_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	x_put_byte (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4039_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	x_put_byte (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEGX.W Dn */
void REGPARAM2 CPUFUNC(op_4040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4050_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	x_put_word (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.W (An)+ */
void REGPARAM2 CPUFUNC(op_4058_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
	x_put_word (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.W -(An) */
void REGPARAM2 CPUFUNC(op_4060_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
	x_put_word (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4068_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	x_put_word (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4070_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	x_put_word (srca, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4078_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	x_put_word (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4079_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	x_put_word (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEGX.L Dn */
void REGPARAM2 CPUFUNC(op_4080_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4090_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	x_put_long (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.L (An)+ */
void REGPARAM2 CPUFUNC(op_4098_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
	x_put_long (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.L -(An) */
void REGPARAM2 CPUFUNC(op_40a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
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
	x_put_long (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.L (d16,An) */
void REGPARAM2 CPUFUNC(op_40a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	x_put_long (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_40b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	x_put_long (srca, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.L (xxx).W */
void REGPARAM2 CPUFUNC(op_40b8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	x_put_long (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.L (xxx).L */
void REGPARAM2 CPUFUNC(op_40b9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	x_put_long (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVSR2.W Dn */
void REGPARAM2 CPUFUNC(op_40c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9617; }
{{	MakeSR ();
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((regs.sr) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9617: ;
return;
}

/* MVSR2.W (An) */
void REGPARAM2 CPUFUNC(op_40d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9618; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
	MakeSR ();
	x_put_word (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9618: ;
return;
}

/* MVSR2.W (An)+ */
void REGPARAM2 CPUFUNC(op_40d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9619; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 2;
	MakeSR ();
	x_put_word (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9619: ;
return;
}

/* MVSR2.W -(An) */
void REGPARAM2 CPUFUNC(op_40e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9620; }
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	m68k_areg (regs, srcreg) = srca;
	MakeSR ();
	x_put_word (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9620: ;
return;
}

/* MVSR2.W (d16,An) */
void REGPARAM2 CPUFUNC(op_40e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9621; }
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	MakeSR ();
	x_put_word (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9621: ;
return;
}

/* MVSR2.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_40f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9622; }
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	MakeSR ();
	x_put_word (srca, regs.sr);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9622: ;
return;
}

/* MVSR2.W (xxx).W */
void REGPARAM2 CPUFUNC(op_40f8_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9623; }
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	MakeSR ();
	x_put_word (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9623: ;
return;
}

/* MVSR2.W (xxx).L */
void REGPARAM2 CPUFUNC(op_40f9_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9624; }
{{	uaecptr srca = get_long_ce020_prefetch (2);
	MakeSR ();
	x_put_word (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9624: ;
return;
}

/* CHK.L Dn,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4100_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9625;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9625;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9625: ;
return;
}

#endif
/* CHK.L (An),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9626;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9626;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9626: ;
return;
}

#endif
/* CHK.L (An)+,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9627;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9627;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9627: ;
return;
}

#endif
/* CHK.L -(An),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9628;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9628;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9628: ;
return;
}

#endif
/* CHK.L (d16,An),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9629;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9629;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9629: ;
return;
}

#endif
/* CHK.L (d8,An,Xn),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9630;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9630;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9630: ;
return;
}

#endif
/* CHK.L (xxx).W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4138_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9631;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9631;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9631: ;
return;
}

#endif
/* CHK.L (xxx).L,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4139_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (6);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9632;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9632;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9632: ;
return;
}

#endif
/* CHK.L (d16,PC),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_413a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9633;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9633;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9633: ;
return;
}

#endif
/* CHK.L (d8,PC,Xn),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_413b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9634;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9634;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9634: ;
return;
}

#endif
/* CHK.L #<data>.L,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_413c_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce020_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (6);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9635;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9635;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9635: ;
return;
}

#endif
/* CHK.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_4180_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9636;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9636;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9636: ;
return;
}

/* CHK.W (An),Dn */
void REGPARAM2 CPUFUNC(op_4190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9637;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9637;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9637: ;
return;
}

/* CHK.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_4198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9638;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9638;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9638: ;
return;
}

/* CHK.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_41a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9639;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9639;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9639: ;
return;
}

/* CHK.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_41a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9640;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9640;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9640: ;
return;
}

/* CHK.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_41b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9641;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9641;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9641: ;
return;
}

/* CHK.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_41b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9642;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9642;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9642: ;
return;
}

/* CHK.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_41b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (6);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9643;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9643;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9643: ;
return;
}

/* CHK.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_41ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9644;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9644;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9644: ;
return;
}

/* CHK.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_41bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9645;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9645;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9645: ;
return;
}

/* CHK.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_41bc_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel9646;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel9646;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9646: ;
return;
}

/* LEA.L (An),An */
void REGPARAM2 CPUFUNC(op_41d0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_41e8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_41f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	m68k_areg (regs, dstreg) = (srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LEA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_41f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LEA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_41f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* LEA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_41fa_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_41fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	m68k_areg (regs, dstreg) = (srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.B Dn */
void REGPARAM2 CPUFUNC(op_4200_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4210_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	x_put_byte (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.B (An)+ */
void REGPARAM2 CPUFUNC(op_4218_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	x_put_byte (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.B -(An) */
void REGPARAM2 CPUFUNC(op_4220_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	x_put_byte (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4228_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	x_put_byte (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4230_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	x_put_byte (srca, 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4238_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	x_put_byte (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4239_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	x_put_byte (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CLR.W Dn */
void REGPARAM2 CPUFUNC(op_4240_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4250_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	x_put_word (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.W (An)+ */
void REGPARAM2 CPUFUNC(op_4258_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	x_put_word (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.W -(An) */
void REGPARAM2 CPUFUNC(op_4260_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	x_put_word (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4268_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	x_put_word (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4270_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	x_put_word (srca, 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4278_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	x_put_word (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4279_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	x_put_word (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CLR.L Dn */
void REGPARAM2 CPUFUNC(op_4280_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4290_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	x_put_long (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.L (An)+ */
void REGPARAM2 CPUFUNC(op_4298_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	x_put_long (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.L -(An) */
void REGPARAM2 CPUFUNC(op_42a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	x_put_long (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.L (d16,An) */
void REGPARAM2 CPUFUNC(op_42a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	x_put_long (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_42b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	x_put_long (srca, 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.L (xxx).W */
void REGPARAM2 CPUFUNC(op_42b8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	x_put_long (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.L (xxx).L */
void REGPARAM2 CPUFUNC(op_42b9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	x_put_long (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVSR2.B Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	MakeSR ();
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((regs.sr & 0xff) & 0xffff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
#endif

#ifdef PART_4
/* MVSR2.B (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	MakeSR ();
	x_put_word (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* MVSR2.B (An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 2;
	MakeSR ();
	x_put_word (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* MVSR2.B -(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	m68k_areg (regs, srcreg) = srca;
	MakeSR ();
	x_put_word (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* MVSR2.B (d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	MakeSR ();
	x_put_word (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* MVSR2.B (d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	MakeSR ();
	x_put_word (srca, regs.sr & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* MVSR2.B (xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	MakeSR ();
	x_put_word (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* MVSR2.B (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
	MakeSR ();
	x_put_word (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* NEG.B Dn */
void REGPARAM2 CPUFUNC(op_4400_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4410_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.B (An)+ */
void REGPARAM2 CPUFUNC(op_4418_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.B -(An) */
void REGPARAM2 CPUFUNC(op_4420_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4428_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4430_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (srca, dst);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEG.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4438_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4439_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEG.W Dn */
void REGPARAM2 CPUFUNC(op_4440_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4450_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.W (An)+ */
void REGPARAM2 CPUFUNC(op_4458_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
	x_put_word (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.W -(An) */
void REGPARAM2 CPUFUNC(op_4460_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
	x_put_word (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4468_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4470_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (srca, dst);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEG.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4478_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4479_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEG.L Dn */
void REGPARAM2 CPUFUNC(op_4480_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4490_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.L (An)+ */
void REGPARAM2 CPUFUNC(op_4498_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
	x_put_long (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.L -(An) */
void REGPARAM2 CPUFUNC(op_44a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
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
	x_put_long (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.L (d16,An) */
void REGPARAM2 CPUFUNC(op_44a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_44b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (srca, dst);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEG.L (xxx).W */
void REGPARAM2 CPUFUNC(op_44b8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.L (xxx).L */
void REGPARAM2 CPUFUNC(op_44b9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MV2SR.B Dn */
void REGPARAM2 CPUFUNC(op_44c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_44d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MV2SR.B (An)+ */
void REGPARAM2 CPUFUNC(op_44d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_44e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_44e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MV2SR.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_44f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MV2SR.B (xxx).W */
void REGPARAM2 CPUFUNC(op_44f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MV2SR.B (xxx).L */
void REGPARAM2 CPUFUNC(op_44f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MV2SR.B (d16,PC) */
void REGPARAM2 CPUFUNC(op_44fa_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MV2SR.B (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_44fb_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MV2SR.B #<data>.B */
void REGPARAM2 CPUFUNC(op_44fc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4600_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4610_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	x_put_byte (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.B (An)+ */
void REGPARAM2 CPUFUNC(op_4618_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	x_put_byte (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.B -(An) */
void REGPARAM2 CPUFUNC(op_4620_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	x_put_byte (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4628_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	x_put_byte (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4630_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	x_put_byte (srca, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NOT.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4638_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	x_put_byte (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4639_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	x_put_byte (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NOT.W Dn */
void REGPARAM2 CPUFUNC(op_4640_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4650_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	x_put_word (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.W (An)+ */
void REGPARAM2 CPUFUNC(op_4658_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	x_put_word (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.W -(An) */
void REGPARAM2 CPUFUNC(op_4660_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	x_put_word (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4668_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	x_put_word (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4670_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	x_put_word (srca, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NOT.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4678_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	x_put_word (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4679_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	x_put_word (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NOT.L Dn */
void REGPARAM2 CPUFUNC(op_4680_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4690_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	x_put_long (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.L (An)+ */
void REGPARAM2 CPUFUNC(op_4698_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	x_put_long (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.L -(An) */
void REGPARAM2 CPUFUNC(op_46a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	x_put_long (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.L (d16,An) */
void REGPARAM2 CPUFUNC(op_46a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	x_put_long (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_46b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	x_put_long (srca, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NOT.L (xxx).W */
void REGPARAM2 CPUFUNC(op_46b8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	x_put_long (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.L (xxx).L */
void REGPARAM2 CPUFUNC(op_46b9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	x_put_long (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MV2SR.W Dn */
void REGPARAM2 CPUFUNC(op_46c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9745; }
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	regs.sr = src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9745: ;
return;
}

/* MV2SR.W (An) */
void REGPARAM2 CPUFUNC(op_46d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9746; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9746: ;
return;
}

/* MV2SR.W (An)+ */
void REGPARAM2 CPUFUNC(op_46d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9747; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9747: ;
return;
}

/* MV2SR.W -(An) */
void REGPARAM2 CPUFUNC(op_46e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9748; }
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9748: ;
return;
}

/* MV2SR.W (d16,An) */
void REGPARAM2 CPUFUNC(op_46e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9749; }
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9749: ;
return;
}

/* MV2SR.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_46f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9750; }
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9750: ;
return;
}

/* MV2SR.W (xxx).W */
void REGPARAM2 CPUFUNC(op_46f8_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9751; }
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9751: ;
return;
}

/* MV2SR.W (xxx).L */
void REGPARAM2 CPUFUNC(op_46f9_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9752; }
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9752: ;
return;
}

/* MV2SR.W (d16,PC) */
void REGPARAM2 CPUFUNC(op_46fa_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9753; }
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9753: ;
return;
}

/* MV2SR.W (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_46fb_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9754; }
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9754: ;
return;
}

/* MV2SR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_46fc_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9755; }
{{	uae_s16 src = get_word_ce020_prefetch (2);
	regs.sr = src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9755: ;
return;
}

/* NBCD.B Dn */
void REGPARAM2 CPUFUNC(op_4800_22)(uae_u32 opcode)
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
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((newv) & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LINK.L An,#<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4808_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr olda;
	olda = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = olda;
{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 offs = get_long_ce020_prefetch (2);
	x_put_long (olda, src);
	m68k_areg (regs, srcreg) = (m68k_areg (regs, 7));
	m68k_areg (regs, 7) += offs;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* NBCD.B (An) */
void REGPARAM2 CPUFUNC(op_4810_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B (An)+ */
void REGPARAM2 CPUFUNC(op_4818_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B -(An) */
void REGPARAM2 CPUFUNC(op_4820_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4828_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NBCD.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4830_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NBCD.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4838_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NBCD.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4839_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SWAP.W Dn */
void REGPARAM2 CPUFUNC(op_4840_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4848_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* PEA.L (An) */
void REGPARAM2 CPUFUNC(op_4850_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	x_put_long (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* PEA.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4868_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	x_put_long (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* PEA.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4870_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	x_put_long (dsta, srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* PEA.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4878_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	x_put_long (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* PEA.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4879_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	x_put_long (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* PEA.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_487a_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	x_put_long (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* PEA.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_487b_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	x_put_long (dsta, srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EXT.W Dn */
void REGPARAM2 CPUFUNC(op_4880_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4890_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { x_put_word (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { x_put_word (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_48a0_22)(uae_u32 opcode)
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
		x_put_word (srca, m68k_areg (regs, movem_index2[amask]));
		amask = movem_next[amask];
	}
	while (dmask) { srca -= 2; x_put_word (srca, m68k_dreg (regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_48a8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { x_put_word (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { x_put_word (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_48b0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { x_put_word (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { x_put_word (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMLE.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_48b8_22)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { x_put_word (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { x_put_word (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_48b9_22)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = get_long_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { x_put_word (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { x_put_word (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EXT.L Dn */
void REGPARAM2 CPUFUNC(op_48c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_48d0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { x_put_long (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { x_put_long (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.L #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_48e0_22)(uae_u32 opcode)
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
		x_put_long (srca, m68k_areg (regs, movem_index2[amask]));
		amask = movem_next[amask];
	}
	while (dmask) { srca -= 4; x_put_long (srca, m68k_dreg (regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.L #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_48e8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { x_put_long (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { x_put_long (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.L #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_48f0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { x_put_long (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { x_put_long (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMLE.L #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_48f8_22)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { x_put_long (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { x_put_long (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.L #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_48f9_22)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
{	uaecptr srca = get_long_ce020_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { x_put_long (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { x_put_long (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EXT.B Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_49c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a00_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a10_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.B (An)+ */
void REGPARAM2 CPUFUNC(op_4a18_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.B -(An) */
void REGPARAM2 CPUFUNC(op_4a20_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4a28_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4a30_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TST.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4a38_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4a39_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TST.B (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a3a_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_4a3b_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* TST.B #<data>.B */
void REGPARAM2 CPUFUNC(op_4a3c_22)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.W Dn */
void REGPARAM2 CPUFUNC(op_4a40_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a48_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a50_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.W (An)+ */
void REGPARAM2 CPUFUNC(op_4a58_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.W -(An) */
void REGPARAM2 CPUFUNC(op_4a60_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4a68_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4a70_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TST.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4a78_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4a79_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TST.W (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a7a_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_4a7b_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* TST.W #<data>.W */
void REGPARAM2 CPUFUNC(op_4a7c_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.L Dn */
void REGPARAM2 CPUFUNC(op_4a80_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a88_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a90_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.L (An)+ */
void REGPARAM2 CPUFUNC(op_4a98_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.L -(An) */
void REGPARAM2 CPUFUNC(op_4aa0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4aa8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4ab0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TST.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4ab8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4ab9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TST.L (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4aba_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_4abb_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* TST.L #<data>.L */
void REGPARAM2 CPUFUNC(op_4abc_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TAS.B Dn */
void REGPARAM2 CPUFUNC(op_4ac0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4ad0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	x_put_byte (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TAS.B (An)+ */
void REGPARAM2 CPUFUNC(op_4ad8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	x_put_byte (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TAS.B -(An) */
void REGPARAM2 CPUFUNC(op_4ae0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	x_put_byte (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TAS.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4ae8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	x_put_byte (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TAS.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4af0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	x_put_byte (srca, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TAS.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4af8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	x_put_byte (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TAS.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4af9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	x_put_byte (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MULL.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c00_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4c10_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c18_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c20_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c28_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (6);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c30_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_mull(opcode, dst, extra);
}}}}}return;
}

#endif
/* MULL.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c38_22)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (6);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c39_22)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (8);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c3a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (6);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c3b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_mull(opcode, dst, extra);
}}}}}return;
}

#endif
/* MULL.L #<data>.W,#<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c3c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4c40_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4c50_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c58_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c60_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c68_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (6);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c70_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_divl(opcode, dst, extra);
}}}}}return;
}

#endif
/* DIVL.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c78_22)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (6);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c79_22)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = get_long_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (8);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c7a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (6);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c7b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_divl(opcode, dst, extra);
}}}}}return;
}

#endif
/* DIVL.L #<data>.W,#<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c7c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4c90_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_4c98_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; amask = movem_next[amask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_4ca8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4cb0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMEL.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_4cb8_22)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_4cb9_22)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = get_long_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* MVMEL.W #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_4cba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.W #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4cbb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)x_get_word (srca); srca += 2; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMEL.L #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_4cd0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = x_get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = x_get_long (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.L #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_4cd8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = x_get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = x_get_long (srca); srca += 4; amask = movem_next[amask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.L #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_4ce8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = x_get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = x_get_long (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.L #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4cf0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = x_get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = x_get_long (srca); srca += 4; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMEL.L #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_4cf8_22)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = x_get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = x_get_long (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.L #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_4cf9_22)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = get_long_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = x_get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = x_get_long (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* MVMEL.L #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_4cfa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = x_get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = x_get_long (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.L #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4cfb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{	uae_u16 mask = get_word_ce020_prefetch (2);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = x_get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = x_get_long (srca); srca += 4; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TRAPQ.L #<data> */
void REGPARAM2 CPUFUNC(op_4e40_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 15);
{{	uae_u32 src = srcreg;
	m68k_incpc (2);
	Exception (src + 32);
}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LINK.W An,#<data>.W */
void REGPARAM2 CPUFUNC(op_4e50_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr olda;
	olda = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = olda;
{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s16 offs = get_word_ce020_prefetch (2);
	x_put_long (olda, src);
	m68k_areg (regs, srcreg) = (m68k_areg (regs, 7));
	m68k_areg (regs, 7) += offs;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* UNLK.L An */
void REGPARAM2 CPUFUNC(op_4e58_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
	m68k_areg (regs, 7) = src;
{	uaecptr olda = m68k_areg (regs, 7);
{	uae_s32 old = x_get_long (olda);
	m68k_areg (regs, 7) += 4;
	m68k_areg (regs, srcreg) = (old);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MVR2USP.L An */
void REGPARAM2 CPUFUNC(op_4e60_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9873; }
{{	uae_s32 src = m68k_areg (regs, srcreg);
	regs.usp = src;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9873: ;
return;
}

/* MVUSP2R.L An */
void REGPARAM2 CPUFUNC(op_4e68_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel9874; }
{{	m68k_areg (regs, srcreg) = (regs.usp);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9874: ;
return;
}

/* RESET.L  */
void REGPARAM2 CPUFUNC(op_4e70_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9875; }
{	cpureset ();
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9875: ;
return;
}

/* NOP.L  */
void REGPARAM2 CPUFUNC(op_4e71_22)(uae_u32 opcode)
{
{	do_cycles_ce020 (6);
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* STOP.L #<data>.W */
void REGPARAM2 CPUFUNC(op_4e72_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9877; }
{{	uae_s16 src = get_word_ce020_prefetch (2);
	regs.sr = src;
	MakeFromSR ();
	m68k_setstopped ();
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9877: ;
return;
}

/* RTE.L  */
void REGPARAM2 CPUFUNC(op_4e73_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9878; }
{	uae_u16 newsr; uae_u32 newpc;
	for (;;) {
		uaecptr a = m68k_areg (regs, 7);
		uae_s16 sr = x_get_word (a);
		uae_s32 pc = x_get_long (a + 2);
		uae_s16 format = x_get_word (a + 2 + 4);
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
		else { m68k_areg (regs, 7) += offset; Exception (14); goto endlabel9878; }
		regs.sr = newsr; MakeFromSR ();
}
	regs.sr = newsr; MakeFromSR ();
	if (newpc & 1) {
		exception3i (0x4E73, newpc);
		goto endlabel9878;
	}
		m68k_setpc (newpc);
	ipl_fetch ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9878: ;
return;
}

/* RTD.L #<data>.W */
void REGPARAM2 CPUFUNC(op_4e74_22)(uae_u32 opcode)
{
{{	uaecptr pca = m68k_areg (regs, 7);
{	uae_s32 pc = x_get_long (pca);
	m68k_areg (regs, 7) += 4;
{	uae_s16 offs = get_word_ce020_prefetch (2);
	m68k_areg (regs, 7) += offs;
	if (pc & 1) {
		exception3i (0x4E74, pc);
		goto endlabel9879;
	}
	if (pc & 1) {
		exception3i (0x4E74, pc);
		goto endlabel9879;
	}
	m68k_setpc (pc);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9879: ;
return;
}

/* RTS.L  */
void REGPARAM2 CPUFUNC(op_4e75_22)(uae_u32 opcode)
{
{	uaecptr pc = m68k_getpc ();
	m68k_do_rts_ce030 ();
	if (m68k_getpc () & 1) {
		uaecptr faultpc = m68k_getpc ();
		m68k_setpc (pc);
		exception3i (0x4E75, faultpc);
	}
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TRAPV.L  */
void REGPARAM2 CPUFUNC(op_4e76_22)(uae_u32 opcode)
{
{	m68k_incpc (2);
	if (GET_VFLG ()) {
		Exception (7);
		goto endlabel9881;
	}
}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9881: ;
return;
}

/* RTR.L  */
void REGPARAM2 CPUFUNC(op_4e77_22)(uae_u32 opcode)
{
{	uaecptr oldpc = m68k_getpc ();
	MakeSR ();
{	uaecptr sra = m68k_areg (regs, 7);
{	uae_s16 sr = x_get_word (sra);
	m68k_areg (regs, 7) += 2;
{	uaecptr pca = m68k_areg (regs, 7);
{	uae_s32 pc = x_get_long (pca);
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
void REGPARAM2 CPUFUNC(op_4e7a_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9883; }
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	int regno = (src >> 12) & 15;
	uae_u32 *regp = regs.regs + regno;
	if (! m68k_movec2(src & 0xFFF, regp)) goto endlabel9883;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9883: ;
return;
}

#endif
/* MOVE2C.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4e7b_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9884; }
{{	uae_s16 src = get_word_ce020_prefetch (2);
{	int regno = (src >> 12) & 15;
	uae_u32 *regp = regs.regs + regno;
	if (! m68k_move2c(src & 0xFFF, regp)) goto endlabel9884;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9884: ;
return;
}

#endif
/* JSR.L (An) */
void REGPARAM2 CPUFUNC(op_4e90_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uaecptr oldpc = m68k_getpc () + 2;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9885;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	x_put_long (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9885: ;
return;
}

/* JSR.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4ea8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9886;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	x_put_long (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9886: ;
return;
}

/* JSR.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4eb0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uaecptr oldpc = m68k_getpc () + 0;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9887;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	x_put_long (m68k_areg (regs, 7), oldpc);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9887: ;
return;
}

/* JSR.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4eb8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9888;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	x_put_long (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9888: ;
return;
}

/* JSR.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4eb9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 6;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9889;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	x_put_long (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9889: ;
return;
}

/* JSR.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_4eba_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9890;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	x_put_long (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9890: ;
return;
}

/* JSR.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4ebb_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uaecptr oldpc = m68k_getpc () + 0;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9891;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	x_put_long (m68k_areg (regs, 7), oldpc);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9891: ;
return;
}

/* JMP.L (An) */
void REGPARAM2 CPUFUNC(op_4ed0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9892;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9892: ;
return;
}

/* JMP.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4ee8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9893;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9893: ;
return;
}

/* JMP.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4ef0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9894;
	}
	m68k_setpc (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9894: ;
return;
}

/* JMP.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4ef8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9895;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9895: ;
return;
}

/* JMP.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4ef9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9896;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9896: ;
return;
}

/* JMP.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_4efa_22)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9897;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9897: ;
return;
}

/* JMP.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4efb_22)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel9898;
	}
	m68k_setpc (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9898: ;
return;
}

/* ADDQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.B #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.B #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.B #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.B #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDQ.B #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5038_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.B #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5039_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5048_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5050_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.W #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5058_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.W #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5060_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.W #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5068_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.W #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5070_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif

#ifdef PART_5
/* ADDQ.W #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5078_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.W #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5079_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5080_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5088_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5090_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.L #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5098_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.L #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_50a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.L #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_50a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.L #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_50b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDQ.L #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_50b8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.L #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_50b9_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_50c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (0) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_50c8_22)(uae_u32 opcode)
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
				goto endlabel9926;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel9926: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_50d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (0) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_50d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (0) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_50e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (0) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_50e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (0) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_50f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (0) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_50f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (0) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_50f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (0) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_50fa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (0)) { Exception (7); goto endlabel9934; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9934: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_50fb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (0)) { Exception (7); goto endlabel9935; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9935: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_50fc_22)(uae_u32 opcode)
{
{	if (cctrue (0)) { Exception (7); goto endlabel9936; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9936: ;
return;
}

#endif
/* SUBQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5100_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.B #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.B #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.B #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.B #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBQ.B #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5138_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.B #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5139_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5140_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5148_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5150_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.W #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5158_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.W #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5160_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.W #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5168_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.W #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5170_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBQ.W #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5178_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.W #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5179_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5180_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5188_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.L #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.L #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_51a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.L #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_51a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.L #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_51b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBQ.L #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_51b8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.L #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_51b9_22)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_51c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (1) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_51c8_22)(uae_u32 opcode)
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
				goto endlabel9964;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel9964: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_51d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (1) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_51d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (1) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_51e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (1) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_51e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (1) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_51f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (1) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_51f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (1) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_51f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (1) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_51fa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (1)) { Exception (7); goto endlabel9972; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9972: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_51fb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (1)) { Exception (7); goto endlabel9973; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9973: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_51fc_22)(uae_u32 opcode)
{
{	if (cctrue (1)) { Exception (7); goto endlabel9974; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9974: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_52c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (2) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_52c8_22)(uae_u32 opcode)
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
				goto endlabel9976;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel9976: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_52d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (2) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_52d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (2) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_52e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (2) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_52e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (2) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_52f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (2) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_52f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (2) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_52f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (2) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_52fa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (2)) { Exception (7); goto endlabel9984; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9984: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_52fb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (2)) { Exception (7); goto endlabel9985; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9985: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_52fc_22)(uae_u32 opcode)
{
{	if (cctrue (2)) { Exception (7); goto endlabel9986; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9986: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_53c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (3) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_53c8_22)(uae_u32 opcode)
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
				goto endlabel9988;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel9988: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_53d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (3) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_53d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (3) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_53e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (3) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_53e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (3) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_53f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (3) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_53f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (3) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_53f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (3) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_53fa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (3)) { Exception (7); goto endlabel9996; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel9996: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_53fb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (3)) { Exception (7); goto endlabel9997; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel9997: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_53fc_22)(uae_u32 opcode)
{
{	if (cctrue (3)) { Exception (7); goto endlabel9998; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel9998: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_54c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (4) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_54c8_22)(uae_u32 opcode)
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
				goto endlabel10000;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10000: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_54d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (4) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_54d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (4) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_54e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (4) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_54e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (4) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_54f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (4) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_54f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (4) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_54f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (4) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_54fa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (4)) { Exception (7); goto endlabel10008; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10008: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_54fb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (4)) { Exception (7); goto endlabel10009; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10009: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_54fc_22)(uae_u32 opcode)
{
{	if (cctrue (4)) { Exception (7); goto endlabel10010; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10010: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_55c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (5) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_55c8_22)(uae_u32 opcode)
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
				goto endlabel10012;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10012: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_55d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (5) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_55d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (5) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_55e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (5) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_55e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (5) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_55f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (5) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_55f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (5) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_55f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (5) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_55fa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (5)) { Exception (7); goto endlabel10020; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10020: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_55fb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (5)) { Exception (7); goto endlabel10021; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10021: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_55fc_22)(uae_u32 opcode)
{
{	if (cctrue (5)) { Exception (7); goto endlabel10022; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10022: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_56c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (6) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_56c8_22)(uae_u32 opcode)
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
				goto endlabel10024;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10024: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_56d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (6) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_56d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (6) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_56e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (6) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_56e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (6) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_56f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (6) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_56f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (6) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_56f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (6) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_56fa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (6)) { Exception (7); goto endlabel10032; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10032: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_56fb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (6)) { Exception (7); goto endlabel10033; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10033: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_56fc_22)(uae_u32 opcode)
{
{	if (cctrue (6)) { Exception (7); goto endlabel10034; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10034: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_57c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (7) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_57c8_22)(uae_u32 opcode)
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
				goto endlabel10036;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10036: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_57d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (7) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_57d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (7) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_57e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (7) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_57e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (7) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_57f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (7) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_57f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (7) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_57f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (7) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_57fa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (7)) { Exception (7); goto endlabel10044; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10044: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_57fb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (7)) { Exception (7); goto endlabel10045; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10045: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_57fc_22)(uae_u32 opcode)
{
{	if (cctrue (7)) { Exception (7); goto endlabel10046; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10046: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_58c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (8) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_58c8_22)(uae_u32 opcode)
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
				goto endlabel10048;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10048: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_58d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (8) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_58d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (8) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_58e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (8) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_58e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (8) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_58f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (8) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_58f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (8) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_58f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (8) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_58fa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (8)) { Exception (7); goto endlabel10056; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10056: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_58fb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (8)) { Exception (7); goto endlabel10057; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10057: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_58fc_22)(uae_u32 opcode)
{
{	if (cctrue (8)) { Exception (7); goto endlabel10058; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10058: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_59c0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (9) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_59c8_22)(uae_u32 opcode)
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
				goto endlabel10060;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10060: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_59d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (9) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_59d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (9) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_59e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (9) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_59e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (9) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_59f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (9) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_59f8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (9) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_59f9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (9) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_59fa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (9)) { Exception (7); goto endlabel10068; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10068: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_59fb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (9)) { Exception (7); goto endlabel10069; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10069: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_59fc_22)(uae_u32 opcode)
{
{	if (cctrue (9)) { Exception (7); goto endlabel10070; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10070: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5ac0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (10) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5ac8_22)(uae_u32 opcode)
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
				goto endlabel10072;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10072: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5ad0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (10) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5ad8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (10) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ae0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (10) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ae8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (10) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5af0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (10) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5af8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (10) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5af9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (10) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5afa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (10)) { Exception (7); goto endlabel10080; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10080: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5afb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (10)) { Exception (7); goto endlabel10081; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10081: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5afc_22)(uae_u32 opcode)
{
{	if (cctrue (10)) { Exception (7); goto endlabel10082; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10082: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5bc0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (11) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5bc8_22)(uae_u32 opcode)
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
				goto endlabel10084;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10084: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5bd0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (11) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5bd8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (11) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5be0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (11) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5be8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (11) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5bf0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (11) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5bf8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (11) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5bf9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (11) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5bfa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (11)) { Exception (7); goto endlabel10092; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10092: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5bfb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (11)) { Exception (7); goto endlabel10093; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10093: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5bfc_22)(uae_u32 opcode)
{
{	if (cctrue (11)) { Exception (7); goto endlabel10094; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10094: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5cc0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (12) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5cc8_22)(uae_u32 opcode)
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
				goto endlabel10096;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10096: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5cd0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (12) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5cd8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (12) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ce0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (12) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ce8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (12) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5cf0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (12) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5cf8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (12) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5cf9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (12) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5cfa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (12)) { Exception (7); goto endlabel10104; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10104: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5cfb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (12)) { Exception (7); goto endlabel10105; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10105: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5cfc_22)(uae_u32 opcode)
{
{	if (cctrue (12)) { Exception (7); goto endlabel10106; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10106: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5dc0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (13) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5dc8_22)(uae_u32 opcode)
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
				goto endlabel10108;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10108: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5dd0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (13) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5dd8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (13) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5de0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (13) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5de8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (13) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5df0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (13) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5df8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (13) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5df9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (13) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5dfa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (13)) { Exception (7); goto endlabel10116; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10116: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5dfb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (13)) { Exception (7); goto endlabel10117; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10117: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5dfc_22)(uae_u32 opcode)
{
{	if (cctrue (13)) { Exception (7); goto endlabel10118; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10118: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5ec0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (14) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5ec8_22)(uae_u32 opcode)
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
				goto endlabel10120;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10120: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5ed0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (14) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5ed8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (14) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ee0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (14) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ee8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (14) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5ef0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (14) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5ef8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (14) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5ef9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (14) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5efa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (14)) { Exception (7); goto endlabel10128; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10128: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5efb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (14)) { Exception (7); goto endlabel10129; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10129: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5efc_22)(uae_u32 opcode)
{
{	if (cctrue (14)) { Exception (7); goto endlabel10130; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10130: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5fc0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (15) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5fc8_22)(uae_u32 opcode)
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
				goto endlabel10132;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel10132: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5fd0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (15) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5fd8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (15) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5fe0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (15) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5fe8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (15) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5ff0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{{	int val = cctrue (15) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5ff8_22)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{{	int val = cctrue (15) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5ff9_22)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{{	int val = cctrue (15) ? 0xff : 0;
	x_put_byte (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5ffa_22)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce020_prefetch (2);
	if (cctrue (15)) { Exception (7); goto endlabel10140; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel10140: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5ffb_22)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce020_prefetch (2);
	if (cctrue (15)) { Exception (7); goto endlabel10141; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel10141: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5ffc_22)(uae_u32 opcode)
{
{	if (cctrue (15)) { Exception (7); goto endlabel10142; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10142: ;
return;
}

#endif
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6000_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10143;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10143: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6001_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10144;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10144: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_60ff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10145;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10145: ;
return;
}

/* BSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6100_22)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s16 src = get_word_ce020_prefetch (2);
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3 (opcode, m68k_getpc () + s, 0, 1, m68k_getpc () + s);
		goto endlabel10146;
	}
	m68k_do_bsr_ce030 (m68k_getpc () + 4, s);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10146: ;
return;
}

/* BSRQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6101_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{	uae_s32 s;
{	uae_u32 src = srcreg;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3 (opcode, m68k_getpc () + s, 0, 1, m68k_getpc () + s);
		goto endlabel10147;
	}
	m68k_do_bsr_ce030 (m68k_getpc () + 2, s);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10147: ;
return;
}

/* BSR.L #<data>.L */
void REGPARAM2 CPUFUNC(op_61ff_22)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s32 src = get_long_ce020_prefetch (2);
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3 (opcode, m68k_getpc () + s, 0, 1, m68k_getpc () + s);
		goto endlabel10148;
	}
	m68k_do_bsr_ce030 (m68k_getpc () + 6, s);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10148: ;
return;
}

#endif

#ifdef PART_6
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6200_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10149;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10149: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6201_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10150;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10150: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_62ff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10151;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10151: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6300_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10152;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10152: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6301_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10153;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10153: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_63ff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10154;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10154: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6400_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10155;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10155: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6401_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10156;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10156: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_64ff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10157;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10157: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6500_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10158;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10158: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6501_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10159;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10159: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_65ff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10160;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10160: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6600_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10161;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10161: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6601_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10162;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10162: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_66ff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10163;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10163: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6700_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10164;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10164: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6701_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10165;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10165: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_67ff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10166;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10166: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6800_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10167;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10167: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6801_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10168;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10168: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_68ff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10169;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10169: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6900_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10170;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10170: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6901_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10171;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10171: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_69ff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10172;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10172: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6a00_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10173;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10173: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6a01_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10174;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10174: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6aff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10175;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10175: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6b00_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10176;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10176: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6b01_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10177;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10177: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6bff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10178;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10178: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6c00_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10179;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10179: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6c01_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10180;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10180: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6cff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10181;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10181: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6d00_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10182;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10182: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6d01_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10183;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10183: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6dff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10184;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10184: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6e00_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10185;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10185: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6e01_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10186;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10186: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6eff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10187;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10187: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6f00_22)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce020_prefetch (2);
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10188;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10188: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6f01_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10189;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10189: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6fff_22)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce020_prefetch (2);
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel10190;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10190: ;
return;
}

/* MOVEQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_7000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_8018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_8020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_8028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_8030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_8038_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_8039_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_803a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_803b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_803c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8050_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_8058_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_8060_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_8068_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_8070_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_8078_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_8079_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_807a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_807b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_807c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8080_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8090_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_8098_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_80a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_80a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_80b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_80b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_80b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_80ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_80bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_80bc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_80c0_22)(uae_u32 opcode)
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
		goto endlabel10225;
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
}}}endlabel10225: ;
return;
}

/* DIVU.W (An),Dn */
void REGPARAM2 CPUFUNC(op_80d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel10226;
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
}}}}endlabel10226: ;
return;
}

/* DIVU.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_80d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel10227;
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
}}}}endlabel10227: ;
return;
}

/* DIVU.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_80e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel10228;
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
}}}}endlabel10228: ;
return;
}

/* DIVU.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_80e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel10229;
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
}}}}endlabel10229: ;
return;
}

/* DIVU.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel10230;
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
}}}}}endlabel10230: ;
return;
}

/* DIVU.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_80f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel10231;
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
}}}}endlabel10231: ;
return;
}

/* DIVU.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_80f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (6);
		Exception (5);
		goto endlabel10232;
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
}}}}endlabel10232: ;
return;
}

/* DIVU.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_80fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel10233;
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
}}}}endlabel10233: ;
return;
}

/* DIVU.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel10234;
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
}}}}}endlabel10234: ;
return;
}

/* DIVU.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_80fc_22)(uae_u32 opcode)
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
		goto endlabel10235;
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
}}}endlabel10235: ;
return;
}

/* SBCD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_8100_22)(uae_u32 opcode)
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
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SBCD.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_8108_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_8110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_8120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_8128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_8130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_8138_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_8139_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* PACK.L Dn,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_8140_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8148_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uae_u16 val;
	m68k_areg (regs, srcreg) -= areg_byteinc[srcreg];
	val = (uae_u16)x_get_byte (m68k_areg (regs, srcreg));
	m68k_areg (regs, srcreg) -= areg_byteinc[srcreg];
	val = (val | ((uae_u16)x_get_byte (m68k_areg (regs, srcreg)) << 8)) + get_word_ce020_prefetch (2);
	m68k_areg (regs, dstreg) -= areg_byteinc[dstreg];
	x_put_byte (m68k_areg (regs, dstreg),((val >> 4) & 0xf0) | (val & 0xf));
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* OR.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_8150_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8158_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_8160_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_8168_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_8170_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_8178_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_8179_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* UNPK.L Dn,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_8180_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8188_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uae_u16 val;
	m68k_areg (regs, srcreg) -= areg_byteinc[srcreg];
	val = (uae_u16)x_get_byte (m68k_areg (regs, srcreg));
	val = (((val << 4) & 0xf00) | (val & 0xf)) + get_word_ce020_prefetch (2);
	m68k_areg (regs, dstreg) -= 2 * areg_byteinc[dstreg];
	x_put_byte (m68k_areg (regs, dstreg) + areg_byteinc[dstreg], val);
	x_put_byte (m68k_areg (regs, dstreg), val >> 8);
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* OR.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_8190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_81a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_81a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_81b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_81b8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_81b9_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* DIVS.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_81c0_22)(uae_u32 opcode)
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
		goto endlabel10263;
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
}}}endlabel10263: ;
return;
}

/* DIVS.W (An),Dn */
void REGPARAM2 CPUFUNC(op_81d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel10264;
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
}}}}endlabel10264: ;
return;
}

/* DIVS.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_81d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel10265;
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
}}}}endlabel10265: ;
return;
}

/* DIVS.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_81e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel10266;
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
}}}}endlabel10266: ;
return;
}

/* DIVS.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_81e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel10267;
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
}}}}endlabel10267: ;
return;
}

/* DIVS.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_81f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel10268;
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
}}}}}endlabel10268: ;
return;
}

/* DIVS.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_81f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel10269;
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
}}}}endlabel10269: ;
return;
}

/* DIVS.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_81f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (6);
		Exception (5);
		goto endlabel10270;
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
}}}}endlabel10270: ;
return;
}

/* DIVS.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_81fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel10271;
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
}}}}endlabel10271: ;
return;
}

/* DIVS.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_81fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel10272;
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
}}}}}endlabel10272: ;
return;
}

/* DIVS.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_81fc_22)(uae_u32 opcode)
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
		goto endlabel10273;
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
}}}endlabel10273: ;
return;
}

/* SUB.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_9000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_9018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_9020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_9028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_9030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_9038_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_9039_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_903a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_903b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_903c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9048_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9050_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_9058_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_9060_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_9068_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_9070_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_9078_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_9079_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_907a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_907b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_907c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9080_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9088_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9090_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_9098_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_90a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_90a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_90b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_90b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_90b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_90ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_90bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_90bc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_90c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_90c8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_90d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_90d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W -(An),An */
void REGPARAM2 CPUFUNC(op_90e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_90e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_90f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_90f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_90f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_90fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_90fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_90fc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9100_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9108_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_9110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_9120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_9128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_9130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_9138_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_9139_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBX.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_9140_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9148_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_9150_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9158_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_9160_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_9168_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_9170_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_9178_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_9179_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBX.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_9180_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9188_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_9190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_91a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_91a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_91b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_91b8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_91b9_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBA.L Dn,An */
void REGPARAM2 CPUFUNC(op_91c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_91c8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_91d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_91d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L -(An),An */
void REGPARAM2 CPUFUNC(op_91e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_91e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_91f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_91f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_91f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_91fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_91fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_91fc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_b018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_b020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_b028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_b030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_b038_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_b039_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_b03a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_b03b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_b03c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b048_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b050_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b058_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b060_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b068_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b070_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b078_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b079_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b07a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b07b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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

/* CMP.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_b07c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b080_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b088_22)(uae_u32 opcode)
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

#endif

#ifdef PART_7
/* CMP.L (An),Dn */
void REGPARAM2 CPUFUNC(op_b090_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b098_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b0a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b0a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b0b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b0b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b0b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b0ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b0bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b0bc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b0c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b0c8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b0d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b0d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b0e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b0e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b0f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b0f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b0f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b0fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b0fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_b0fc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b100_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b108_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
void REGPARAM2 CPUFUNC(op_b110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b138_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b139_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_b140_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b148_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
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
void REGPARAM2 CPUFUNC(op_b150_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b158_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b160_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b168_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b170_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b178_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b179_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_b180_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b188_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
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
void REGPARAM2 CPUFUNC(op_b190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b1a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b1a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b1b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b1b8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b1b9_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMPA.L Dn,An */
void REGPARAM2 CPUFUNC(op_b1c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b1c8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b1d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b1d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b1e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b1e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b1f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b1f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b1f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b1fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b1fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_b1fc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_c018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_c020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_c028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_c030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_c038_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_c039_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_c03a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_c03b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_c03c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c050_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c058_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c060_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c068_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c070_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c078_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c079_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c07a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c07b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c07c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c080_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c090_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_c098_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_c0a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_c0a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_c0b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_c0b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_c0b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_c0ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_c0bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_c0bc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c0c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c0d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c0d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c0e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c0e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c0f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c0f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c0f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c0fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c0fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c0fc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c100_22)(uae_u32 opcode)
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
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((newv) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ABCD.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_c108_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_c110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c138_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c139_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	x_put_byte (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EXG.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_c140_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c148_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c150_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c158_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c160_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c168_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c170_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c178_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c179_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	x_put_word (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EXG.L Dn,An */
void REGPARAM2 CPUFUNC(op_c188_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c1a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c1a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c1b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c1b8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c1b9_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	x_put_long (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MULS.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_c1c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c1d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c1d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c1e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c1e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c1f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c1f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c1f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c1fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c1fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_c1fc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_d018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_d020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_d028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_d030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_d038_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_d039_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_d03a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_d03b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
void REGPARAM2 CPUFUNC(op_d03c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d048_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d050_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_d058_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_d060_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_d068_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_d070_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_d078_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_d079_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_d07a_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_d07b_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
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
void REGPARAM2 CPUFUNC(op_d07c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d080_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d088_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d090_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_d098_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_d0a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_d0a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_d0b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_d0b8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_d0b9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_d0ba_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_d0bb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
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
void REGPARAM2 CPUFUNC(op_d0bc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d0c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d0c8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d0d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_d0d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W -(An),An */
void REGPARAM2 CPUFUNC(op_d0e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_d0e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_d0f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_d0f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_d0f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_d0fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_d0fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s16 src = x_get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_d0fc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d100_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d108_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_d110_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d118_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d120_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d128_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d130_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d138_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d139_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s8 dst = x_get_byte (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_byte (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDX.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_d140_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d148_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = x_get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_d150_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d158_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d160_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = x_get_word (dsta);
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
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d168_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d170_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d178_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d179_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s16 dst = x_get_word (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_word (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDX.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_d180_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d188_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_d190_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d198_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d1a0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = x_get_long (dsta);
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
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d1a8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d1b0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d1b8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d1b9_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce020_prefetch (2);
{	uae_s32 dst = x_get_long (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	x_put_long (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDA.L Dn,An */
void REGPARAM2 CPUFUNC(op_d1c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d1c8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d1d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_d1d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L -(An),An */
void REGPARAM2 CPUFUNC(op_d1e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = x_get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_d1e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_d1f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_d1f8_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_d1f9_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_d1fa_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_d1fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
{	uae_s32 src = x_get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_d1fc_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e000_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e008_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e010_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e018_22)(uae_u32 opcode)
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

/* ASR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e020_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e028_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e030_22)(uae_u32 opcode)
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

#endif

#ifdef PART_8
/* ROR.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_e038_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e040_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e048_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e050_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e058_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e060_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e068_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e070_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e078_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e080_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e088_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e090_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e098_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e0a0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e0a8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e0b0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e0b8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e0d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e0d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e0e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e0e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e0f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = x_get_word (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ASRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e0f8_22)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e0f9_22)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ASLQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e100_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e108_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e110_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e118_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e120_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e128_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e130_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e138_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e140_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e148_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e150_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e158_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e160_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e168_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e170_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e178_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e180_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e188_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e190_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e198_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e1a0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e1a8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e1b0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e1b8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e1d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e1d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e1e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e1e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e1f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ASLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e1f8_22)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e1f9_22)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* LSRW.W (An) */
void REGPARAM2 CPUFUNC(op_e2d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e2d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e2e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = x_get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e2e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e2f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = x_get_word (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LSRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e2f8_22)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e2f9_22)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* LSLW.W (An) */
void REGPARAM2 CPUFUNC(op_e3d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e3d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e3e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = x_get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e3e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e3f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LSLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e3f8_22)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e3f9_22)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ROXRW.W (An) */
void REGPARAM2 CPUFUNC(op_e4d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e4d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e4e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e4e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e4f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ROXRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e4f8_22)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e4f9_22)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ROXLW.W (An) */
void REGPARAM2 CPUFUNC(op_e5d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e5d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e5e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = x_get_word (dataa);
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
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e5e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e5f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ROXLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e5f8_22)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e5f9_22)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* RORW.W (An) */
void REGPARAM2 CPUFUNC(op_e6d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e6d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORW.W -(An) */
void REGPARAM2 CPUFUNC(op_e6e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = x_get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e6e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* RORW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e6f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	x_put_word (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* RORW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e6f8_22)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* RORW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e6f9_22)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ROLW.W (An) */
void REGPARAM2 CPUFUNC(op_e7d0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e7d8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = x_get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e7e0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = x_get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e7e8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e7f0_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	x_put_word (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ROLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e7f8_22)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e7f9_22)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce020_prefetch (2);
{	uae_s16 data = x_get_word (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	x_put_word (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BFTST.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e8c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e8d0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e8e8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e8f0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_e8f8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e8f9_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e8fa_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e8fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_e9c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e9d0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e9e8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e9f0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_e9f8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e9f9_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e9fa_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e9fb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_eac0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ead0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_eae8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_eaf0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_eaf8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_eaf9_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ebc0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ebd0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ebe8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ebf0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_ebf8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ebf9_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ebfa_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ebfb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_ecc0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ecd0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ece8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ecf0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_ecf8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ecf9_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_edc0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_edd0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_ede8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_edf0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_edf8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_edf9_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_edfa_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_edfb_22)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_ce020 (tmppc, next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_eec0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_eed0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_eee8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_eef0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_eef8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_eef9_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_efc0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_efd0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_efe8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_eff0_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce020_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_ce020 (m68k_areg (regs, dstreg), next_iword_020ce ());
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
void REGPARAM2 CPUFUNC(op_eff8_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_eff9_22)(uae_u32 opcode)
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
/* MMUOP030.L Dn,#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f000_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10773; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
	uae_u16 extraa = 0;
	mmu_op30 (pc, opcode, extra, extraa);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10773: ;
return;
}

#endif
/* MMUOP030.L An,#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f008_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10774; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
	uae_u16 extraa = 0;
	mmu_op30 (pc, opcode, extra, extraa);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10774: ;
return;
}

#endif
/* MMUOP030.L (An),#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f010_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10775; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa = m68k_areg (regs, srcreg);
	mmu_op30 (pc, opcode, extra, extraa);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10775: ;
return;
}

#endif
/* MMUOP030.L (An)+,#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f018_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10776; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 4;
	mmu_op30 (pc, opcode, extra, extraa);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10776: ;
return;
}

#endif
/* MMUOP030.L -(An),#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f020_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10777; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa;
	extraa = m68k_areg (regs, srcreg) - 4;
	m68k_areg (regs, srcreg) = extraa;
	mmu_op30 (pc, opcode, extra, extraa);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10777: ;
return;
}

#endif
/* MMUOP030.L (d16,An),#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f028_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10778; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	m68k_incpc (2);
	mmu_op30 (pc, opcode, extra, extraa);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10778: ;
return;
}

#endif
/* MMUOP030.L (d8,An,Xn),#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f030_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10779; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa;
{	extraa = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
	mmu_op30 (pc, opcode, extra, extraa);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10779: ;
return;
}

#endif
/* MMUOP030.L (xxx).W,#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f038_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel10780; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa = (uae_s32)(uae_s16)get_word_ce020_prefetch (0);
	m68k_incpc (2);
	mmu_op30 (pc, opcode, extra, extraa);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10780: ;
return;
}

#endif
/* MMUOP030.L (xxx).L,#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f039_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel10781; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa = get_long_ce020_prefetch (0);
	m68k_incpc (4);
	mmu_op30 (pc, opcode, extra, extraa);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10781: ;
return;
}

#endif
/* FPP.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f200_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f208_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f210_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f218_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f220_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f228_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f230_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f238_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f239_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f23a_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f23b_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f23c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f240_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f248_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f250_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f258_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f260_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f268_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f270_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f278_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f279_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f27a_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f27b_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f27c_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f280_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f2c0_22)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_f310_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10808; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10808: ;
return;
}

#endif
/* FSAVE.L -(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f320_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10809; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10809: ;
return;
}

#endif
/* FSAVE.L (d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f328_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10810; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10810: ;
return;
}

#endif
/* FSAVE.L (d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f330_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10811; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10811: ;
return;
}

#endif
/* FSAVE.L (xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f338_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel10812; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10812: ;
return;
}

#endif
/* FSAVE.L (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f339_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel10813; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10813: ;
return;
}

#endif
/* FRESTORE.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f350_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10814; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10814: ;
return;
}

#endif
/* FRESTORE.L (An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f358_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10815; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10815: ;
return;
}

#endif
/* FRESTORE.L (d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f368_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10816; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10816: ;
return;
}

#endif
/* FRESTORE.L (d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f370_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10817; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10817: ;
return;
}

#endif
/* FRESTORE.L (xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f378_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel10818; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10818: ;
return;
}

#endif
/* FRESTORE.L (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f379_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel10819; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10819: ;
return;
}

#endif
/* FRESTORE.L (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f37a_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel10820; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10820: ;
return;
}

#endif
/* FRESTORE.L (d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f37b_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel10821; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10821: ;
return;
}

#endif
/* CINVLQ.L #<data>,An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f408_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel10822; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10822: ;
return;
}

#endif
/* CINVPQ.L #<data>,An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f410_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel10823; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10823: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f418_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10824; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10824: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f419_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10825; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10825: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41a_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10826; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10826: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41b_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10827; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10827: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41c_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10828; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10828: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41d_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10829; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10829: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41e_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10830; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10830: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41f_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10831; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10831: ;
return;
}

#endif
/* CPUSHLQ.L #<data>,An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f428_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel10832; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10832: ;
return;
}

#endif
/* CPUSHPQ.L #<data>,An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f430_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel10833; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10833: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f438_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10834; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10834: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f439_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10835; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10835: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43a_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10836; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10836: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43b_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10837; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10837: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43c_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10838; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10838: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43d_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10839; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10839: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43e_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10840; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10840: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43f_22)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel10841; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel10841: ;
return;
}

#endif
/* PFLUSHN.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f500_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10842; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10842: ;
return;
}

#endif
/* PFLUSH.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f508_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10843; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10843: ;
return;
}

#endif
/* PFLUSHAN.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f510_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10844; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10844: ;
return;
}

#endif
/* PFLUSHA.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f518_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10845; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10845: ;
return;
}

#endif
/* PTESTR.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f548_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10846; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10846: ;
return;
}

#endif
/* PTESTW.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f568_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10847; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10847: ;
return;
}

#endif
/* PLPAR.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f588_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10848; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10848: ;
return;
}

#endif
/* PLPAW.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f5c8_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel10849; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10849: ;
return;
}

#endif
/* MOVE16.L (An)+,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f600_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	uae_u32 v1, v2, v3, v4;
{	uaecptr memsa = m68k_areg (regs, srcreg);
{	uaecptr memda = get_long_ce020_prefetch (2);
	memsa &= ~15;
	memda &= ~15;
	v1 = x_get_long (memsa);
	v2 = x_get_long (memsa + 4);
	v3 = x_get_long (memsa + 8);
	v4 = x_get_long (memsa + 12);
	x_put_long (memda , v1);
	x_put_long (memda + 4, v2);
	x_put_long (memda + 8, v3);
	x_put_long (memda + 12, v4);
	m68k_areg (regs, srcreg) += 16;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* MOVE16.L (xxx).L,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f608_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u32 v1, v2, v3, v4;
{	uaecptr memsa = get_long_ce020_prefetch (2);
{	uaecptr memda = m68k_areg (regs, dstreg);
	memsa &= ~15;
	memda &= ~15;
	v1 = x_get_long (memsa);
	v2 = x_get_long (memsa + 4);
	v3 = x_get_long (memsa + 8);
	v4 = x_get_long (memsa + 12);
	x_put_long (memda , v1);
	x_put_long (memda + 4, v2);
	x_put_long (memda + 8, v3);
	x_put_long (memda + 12, v4);
	m68k_areg (regs, dstreg) += 16;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* MOVE16.L (An),(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f610_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	uae_u32 v1, v2, v3, v4;
{	uaecptr memsa = m68k_areg (regs, srcreg);
{	uaecptr memda = get_long_ce020_prefetch (2);
	memsa &= ~15;
	memda &= ~15;
	v1 = x_get_long (memsa);
	v2 = x_get_long (memsa + 4);
	v3 = x_get_long (memsa + 8);
	v4 = x_get_long (memsa + 12);
	x_put_long (memda , v1);
	x_put_long (memda + 4, v2);
	x_put_long (memda + 8, v3);
	x_put_long (memda + 12, v4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* MOVE16.L (xxx).L,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f618_22)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u32 v1, v2, v3, v4;
{	uaecptr memsa = get_long_ce020_prefetch (2);
{	uaecptr memda = m68k_areg (regs, dstreg);
	memsa &= ~15;
	memda &= ~15;
	v1 = x_get_long (memsa);
	v2 = x_get_long (memsa + 4);
	v3 = x_get_long (memsa + 8);
	v4 = x_get_long (memsa + 12);
	x_put_long (memda , v1);
	x_put_long (memda + 4, v2);
	x_put_long (memda + 8, v3);
	x_put_long (memda + 12, v4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* MOVE16.L (An)+,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f620_22)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = 0;
{	uae_u32 v1, v2, v3, v4;
	uaecptr mems = m68k_areg (regs, srcreg) & ~15, memd;
	dstreg = (get_word_ce020_prefetch (2) >> 12) & 7;
	memd = m68k_areg (regs, dstreg) & ~15;
	v1 = x_get_long (mems);
	v2 = x_get_long (mems + 4);
	v3 = x_get_long (mems + 8);
	v4 = x_get_long (mems + 12);
	x_put_long (memd , v1);
	x_put_long (memd + 4, v2);
	x_put_long (memd + 8, v3);
	x_put_long (memd + 12, v4);
	if (srcreg != dstreg)
		m68k_areg (regs, srcreg) += 16;
	m68k_areg (regs, dstreg) += 16;
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* LPSTOP.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f800_22)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel10855; }
{	uae_u16 sw = x_get_iword (2);
	uae_u16 sr;
	if (sw != (0x100|0x80|0x40)) { Exception (4); goto endlabel10855; }
	sr = x_get_iword (4);
	if (!(sr & 0x8000)) { Exception (8); goto endlabel10855; }
	regs.sr = sr;
	MakeFromSR ();
	m68k_setstopped();
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel10855: ;
return;
}

#endif
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
#endif

#ifdef PART_2
#endif

#ifdef PART_3
#endif

#ifdef PART_4
#endif

#ifdef PART_5
#endif

#ifdef PART_6
#endif

#ifdef PART_7
#endif

#ifdef PART_8
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
#endif

#ifdef PART_2
#endif

#ifdef PART_3
#endif

#ifdef PART_4
/* NBCD.B Dn */
void REGPARAM2 CPUFUNC(op_4800_24)(uae_u32 opcode)
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

/* NBCD.B (An) */
void REGPARAM2 CPUFUNC(op_4810_24)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B (An)+ */
void REGPARAM2 CPUFUNC(op_4818_24)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B -(An) */
void REGPARAM2 CPUFUNC(op_4820_24)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4828_24)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NBCD.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4830_24)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_ce020 (m68k_areg (regs, srcreg), next_iword_020ce ());
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NBCD.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4838_24)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NBCD.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4839_24)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce020_prefetch (2);
{	uae_s8 src = x_get_byte (srca);
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
	x_put_byte (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif

#ifdef PART_5
#endif

#ifdef PART_6
/* SBCD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_8100_24)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8108_24)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif

#ifdef PART_7
/* ABCD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_c100_24)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c108_24)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = x_get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = x_get_byte (dsta);
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
	x_put_byte (dsta, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif

#ifdef PART_8
#endif

