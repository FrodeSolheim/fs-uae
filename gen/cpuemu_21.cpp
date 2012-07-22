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
void REGPARAM2 CPUFUNC(op_0000_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0010_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0018_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0020_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0028_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0030_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0038_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0039_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ORSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_003c_21)(uae_u32 opcode)
{
{	MakeSR ();
{	uae_s16 src = get_word_ce030_prefetch (2);
	src &= 0xFF;
	regs.sr |= src;
	MakeFromSR ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0040_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0050_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0058_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0060_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0068_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0070_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0078_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0079_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ORSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_007c_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7139; }
{	MakeSR ();
{	uae_s16 src = get_word_ce030_prefetch (2);
	regs.sr |= src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7139: ;
return;
}

/* OR.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0080_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0090_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0098_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_00a0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* OR.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_00a8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* OR.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_00b0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_00b8_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* OR.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_00b9_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CHK2.B #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00d0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce030 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce030 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7148; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7148: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00e8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce030 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce030 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7149; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7149: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00f0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce030 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce030 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7150; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7150: ;
return;
}

#endif
/* CHK2.B #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00f8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce030 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce030 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7151; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7151: ;
return;
}

#endif
/* CHK2.B #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00f9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce030 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce030 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7152; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel7152: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce030 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce030 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7153; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7153: ;
return;
}

#endif
/* CHK2.B #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_00fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s8)get_byte_ce030 (dsta); upper = (uae_s32)(uae_s8)get_byte_ce030 (dsta + 1);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s8)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7154; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7154: ;
return;
}

#endif
/* BTST.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0100_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0108_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_u16 val = (get_byte_ce030 (memp) << 8) + get_byte_ce030 (memp + 2);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BTST.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BTST.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_0120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BTST.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_0128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_0138_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_0139_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_013a_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_013b_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B Dn,#<data>.B */
void REGPARAM2 CPUFUNC(op_013c_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = (uae_u8)get_word_ce030_prefetch (2);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0140_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0148_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_u32 val = (get_byte_ce030 (memp) << 24) + (get_byte_ce030 (memp + 2) << 16)
              + (get_byte_ce030 (memp + 4) << 8) + get_byte_ce030 (memp + 6);
	m68k_dreg (regs, dstreg) = (val);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0150_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCHG.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0158_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCHG.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_0160_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCHG.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_0168_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0170_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCHG.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_0178_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_0179_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_017a_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_017b_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_0180_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_0188_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	uaecptr memp = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	put_byte_ce030 (memp, src >> 8); put_byte_ce030 (memp + 2, src);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_0190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCLR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_0198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCLR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_01a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BCLR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_01a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_01b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_01b8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_01b9_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_01ba_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_01bb_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_01c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_01c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	uaecptr memp = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	put_byte_ce030 (memp, src >> 24); put_byte_ce030 (memp + 2, src >> 16);
	put_byte_ce030 (memp + 4, src >> 8); put_byte_ce030 (memp + 6, src);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_01d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BSET.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_01d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BSET.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_01e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* BSET.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_01e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_01f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_01f8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_01f9_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B Dn,(d16,PC) */
void REGPARAM2 CPUFUNC(op_01fa_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B Dn,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_01fb_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0200_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0210_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0218_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0220_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0228_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0230_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0238_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0239_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ANDSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_023c_21)(uae_u32 opcode)
{
{	MakeSR ();
{	uae_s16 src = get_word_ce030_prefetch (2);
	src |= 0xFF00;
	regs.sr &= src;
	MakeFromSR ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0240_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0250_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0258_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0260_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0268_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0270_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0278_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0279_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ANDSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_027c_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7217; }
{	MakeSR ();
{	uae_s16 src = get_word_ce030_prefetch (2);
	regs.sr &= src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7217: ;
return;
}

/* AND.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0280_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0290_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0298_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_02a0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* AND.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_02a8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* AND.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_02b0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_02b8_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* AND.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_02b9_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CHK2.W #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02d0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce030 (dsta); upper = (uae_s32)(uae_s16)get_word_ce030 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7226; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7226: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02e8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce030 (dsta); upper = (uae_s32)(uae_s16)get_word_ce030 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7227; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7227: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02f0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce030 (dsta); upper = (uae_s32)(uae_s16)get_word_ce030 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7228; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7228: ;
return;
}

#endif
/* CHK2.W #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02f8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce030 (dsta); upper = (uae_s32)(uae_s16)get_word_ce030 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7229; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7229: ;
return;
}

#endif
/* CHK2.W #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02f9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce030 (dsta); upper = (uae_s32)(uae_s16)get_word_ce030 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7230; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel7230: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce030 (dsta); upper = (uae_s32)(uae_s16)get_word_ce030 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7231; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7231: ;
return;
}

#endif
/* CHK2.W #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_02fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = (uae_s32)(uae_s16)get_word_ce030 (dsta); upper = (uae_s32)(uae_s16)get_word_ce030 (dsta + 2);
	if ((extra & 0x8000) == 0) reg = (uae_s32)(uae_s16)reg;
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7232; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7232: ;
return;
}

#endif
/* SUB.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0400_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0410_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0418_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0420_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0428_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0430_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0438_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0439_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0440_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0450_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0458_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0460_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0468_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0470_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0478_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0479_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0480_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0490_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0498_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_04a0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUB.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_04a8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_04b0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_04b8_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* SUB.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_04b9_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CHK2.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04d0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce030 (dsta); upper = get_long_ce030 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7257; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7257: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04e8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce030 (dsta); upper = get_long_ce030 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7258; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7258: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04f0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce030 (dsta); upper = get_long_ce030 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7259; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7259: ;
return;
}

#endif
/* CHK2.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04f8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce030 (dsta); upper = get_long_ce030 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7260; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7260: ;
return;
}

#endif
/* CHK2.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04f9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce030 (dsta); upper = get_long_ce030 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7261; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
endlabel7261: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce030 (dsta); upper = get_long_ce030 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7262; }
}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7262: ;
return;
}

#endif
/* CHK2.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_04fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
	{uae_s32 upper,lower,reg = regs.regs[(extra >> 12) & 15];
	lower = get_long_ce030 (dsta); upper = get_long_ce030 (dsta + 4);
	SET_ZFLG (upper == reg || lower == reg);
	SET_CFLG_ALWAYS (lower <= upper ? reg < lower || reg > upper : reg > upper || reg < lower);
	if ((extra & 0x800) && GET_CFLG ()) { Exception (6); goto endlabel7263; }
}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7263: ;
return;
}

#endif
/* ADD.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0600_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0610_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0618_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0620_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0628_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0630_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0638_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0639_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0640_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0650_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0658_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0660_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0668_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0670_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0678_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0679_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0680_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0690_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0698_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_06a0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADD.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_06a8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_06b0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_06b8_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* ADD.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_06b9_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* RTM.L Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_06c8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_06d0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_06e8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_06f0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_06f8_21)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06f9_21)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06fa_21)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CALLM.L (d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_06fb_21)(uae_u32 opcode)
{
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* BTST.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0800_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_0810_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0818_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0820_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BTST.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0828_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0830_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0838_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0839_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BTST.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_083a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BTST.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_083b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BTST.B #<data>.W,#<data>.B */
void REGPARAM2 CPUFUNC(op_083c_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uae_s8 dst = (uae_u8)get_word_ce030_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0840_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0850_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0858_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0860_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCHG.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0868_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0870_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCHG.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0878_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0879_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BCHG.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_087a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCHG.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_087b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0880_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0890_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0898_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_08a0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BCLR.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_08a8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_08b0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BCLR.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_08b8_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_08b9_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BCLR.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_08ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BCLR.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_08bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.L #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_08c0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_08d0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_08d8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_08e0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* BSET.B #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_08e8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_08f0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* BSET.B #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_08f8_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_08f9_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* BSET.B #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_08fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BSET.B #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_08fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte_ce030 (dsta, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0a00_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0a10_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_0a18_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_0a20_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_0a28_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0a30_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_0a38_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_0a39_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EORSR.B #<data>.W */
void REGPARAM2 CPUFUNC(op_0a3c_21)(uae_u32 opcode)
{
{	MakeSR ();
{	uae_s16 src = get_word_ce030_prefetch (2);
	src &= 0xFF;
	regs.sr ^= src;
	MakeFromSR ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_0a40_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0a50_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_0a58_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_0a60_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_0a68_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0a70_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_0a78_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_0a79_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EORSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_0a7c_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7355; }
{	MakeSR ();
{	uae_s16 src = get_word_ce030_prefetch (2);
	regs.sr ^= src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7355: ;
return;
}

#endif

#ifdef PART_2
/* EOR.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_0a80_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0a90_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_0a98_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_0aa0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_0aa8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EOR.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_0ab0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_0ab8_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EOR.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_0ab9_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (10);
return;
}

/* CAS.B #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ad0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_byte_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.B #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ad8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_byte_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.B #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ae0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_byte_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.B #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ae8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_byte_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.B #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0af0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_byte_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CAS.B #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0af8_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_byte_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.B #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0af9_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_byte_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* CMP.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_0c00_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0c10_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c18_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c20_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c28_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c30_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c38_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c39_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c3a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c3b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c40_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0c50_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c58_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c60_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c68_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c70_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c78_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c79_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c7a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c7b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c80_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_0c90_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0c98_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0ca0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0ca8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0cb0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0cb8_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0cb9_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0cba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 6;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
{	uae_s32 dst = get_long_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0cbb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (6);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_0cd0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_word_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.W #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cd8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_word_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.W #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ce0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_word_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.W #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ce8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_word_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.W #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cf0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_word_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CAS.W #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cf8_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_word_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.W #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cf9_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_word_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* CAS2.W #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0cfc_21)(uae_u32 opcode)
{
{{	uae_s32 extra = get_long_ce030_prefetch (2);
	uae_u32 rn1 = regs.regs[(extra >> 28) & 15];
	uae_u32 rn2 = regs.regs[(extra >> 12) & 15];
	uae_u16 dst1 = get_word_ce030 (rn1), dst2 = get_word_ce030 (rn2);
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
	put_word_ce030 (rn1, m68k_dreg (regs, (extra >> 22) & 7));
	put_word_ce030 (rn1, m68k_dreg (regs, (extra >> 6) & 7));
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
void REGPARAM2 CPUFUNC(op_0e10_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7409; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	put_byte_ce030 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s8 src = get_byte_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7409: ;
return;
}

#endif
/* MOVES.B #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e18_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7410; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	put_byte_ce030 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7410: ;
return;
}

#endif
/* MOVES.B #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e20_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7411; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	put_byte_ce030 (dsta, src);
}}else{{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, dstreg) = srca;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7411: ;
return;
}

#endif
/* MOVES.B #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e28_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7412; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	put_byte_ce030 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 src = get_byte_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7412: ;
return;
}

#endif
/* MOVES.B #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e30_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7413; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	put_byte_ce030 (dsta, src);
}}}else{{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7413: ;
return;
}

#endif
/* MOVES.B #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e38_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7414; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	put_byte_ce030 (dsta, src);
}}else{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s8 src = get_byte_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7414: ;
return;
}

#endif
/* MOVES.B #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e39_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7415; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = get_long_ce030_prefetch (4);
	put_byte_ce030 (dsta, src);
}}else{{	uaecptr srca = get_long_ce030_prefetch (4);
{	uae_s8 src = get_byte_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s8)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xff) | ((src) & 0xff);
	}
	m68k_incpc (8);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7415: ;
return;
}

#endif
/* MOVES.W #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e50_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7416; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	put_word_ce030 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s16 src = get_word_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7416: ;
return;
}

#endif
/* MOVES.W #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e58_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7417; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	put_word_ce030 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, dstreg) += 2;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7417: ;
return;
}

#endif
/* MOVES.W #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e60_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7418; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	put_word_ce030 (dsta, src);
}}else{{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, dstreg) = srca;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7418: ;
return;
}

#endif
/* MOVES.W #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e68_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7419; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	put_word_ce030 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 src = get_word_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7419: ;
return;
}

#endif
/* MOVES.W #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e70_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7420; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	put_word_ce030 (dsta, src);
}}}else{{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7420: ;
return;
}

#endif
/* MOVES.W #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e78_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7421; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	put_word_ce030 (dsta, src);
}}else{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s16 src = get_word_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7421: ;
return;
}

#endif
/* MOVES.W #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e79_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7422; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = get_long_ce030_prefetch (4);
	put_word_ce030 (dsta, src);
}}else{{	uaecptr srca = get_long_ce030_prefetch (4);
{	uae_s16 src = get_word_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = (uae_s32)(uae_s16)src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (m68k_dreg (regs, (extra >> 12) & 7) & ~0xffff) | ((src) & 0xffff);
	}
	m68k_incpc (8);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7422: ;
return;
}

#endif
/* MOVES.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e90_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7423; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	put_long_ce030 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s32 src = get_long_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7423: ;
return;
}

#endif
/* MOVES.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0e98_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7424; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	put_long_ce030 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, dstreg) += 4;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7424: ;
return;
}

#endif
/* MOVES.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ea0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7425; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	put_long_ce030 (dsta, src);
}}else{{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, dstreg) = srca;
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (4);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7425: ;
return;
}

#endif
/* MOVES.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ea8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7426; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	put_long_ce030 (dsta, src);
}}else{{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s32 src = get_long_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7426: ;
return;
}

#endif
/* MOVES.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0eb0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel7427; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	put_long_ce030 (dsta, src);
}}}else{{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7427: ;
return;
}

#endif
/* MOVES.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0eb8_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7428; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	put_long_ce030 (dsta, src);
}}else{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s32 src = get_long_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (6);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7428: ;
return;
}

#endif
/* MOVES.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0eb9_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7429; }
{{	uae_s16 extra = get_word_ce030_prefetch (2);
	if (extra & 0x800)
{	uae_u32 src = regs.regs[(extra >> 12) & 15];
{	uaecptr dsta = get_long_ce030_prefetch (4);
	put_long_ce030 (dsta, src);
}}else{{	uaecptr srca = get_long_ce030_prefetch (4);
{	uae_s32 src = get_long_ce030 (srca);
	if (extra & 0x8000) {
	m68k_areg (regs, (extra >> 12) & 7) = src;
	} else {
	m68k_dreg (regs, (extra >> 12) & 7) = (src);
	}
	m68k_incpc (8);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7429: ;
return;
}

#endif
/* CAS.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ed0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_long_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ed8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_long_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ee0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_long_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* CAS.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ee8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_long_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ef0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_long_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* CAS.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ef8_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_long_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* CAS.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0ef9_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	if (GET_ZFLG ()){	put_long_ce030 (dsta, (m68k_dreg (regs, ru)));
}else{m68k_dreg (regs, rc) = dst;
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

#endif
/* CAS2.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_0efc_21)(uae_u32 opcode)
{
{{	uae_s32 extra = get_long_ce030_prefetch (2);
	uae_u32 rn1 = regs.regs[(extra >> 28) & 15];
	uae_u32 rn2 = regs.regs[(extra >> 12) & 15];
	uae_u32 dst1 = get_long_ce030 (rn1), dst2 = get_long_ce030 (rn2);
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
	put_long_ce030 (rn1, m68k_dreg (regs, (extra >> 22) & 7));
	put_long_ce030 (rn1, m68k_dreg (regs, (extra >> 6) & 7));
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
void REGPARAM2 CPUFUNC(op_1000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_1010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,Dn */
void REGPARAM2 CPUFUNC(op_1018_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_1020_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_1028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_1030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_1038_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_1039_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_103a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_103b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,Dn */
void REGPARAM2 CPUFUNC(op_103c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_1080_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(An) */
void REGPARAM2 CPUFUNC(op_1090_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(An) */
void REGPARAM2 CPUFUNC(op_1098_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(An) */
void REGPARAM2 CPUFUNC(op_10a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_10a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_10b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_10b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_10b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_10ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_10bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(An) */
void REGPARAM2 CPUFUNC(op_10bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_10c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(An)+ */
void REGPARAM2 CPUFUNC(op_10d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_10d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_10e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_10e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_10f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_10f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_10f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_10fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_10fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(An)+ */
void REGPARAM2 CPUFUNC(op_10fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_1100_21)(uae_u32 opcode)
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
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),-(An) */
void REGPARAM2 CPUFUNC(op_1110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_1118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),-(An) */
void REGPARAM2 CPUFUNC(op_1120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_1128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_1130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_1138_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_1139_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_113a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_113b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,-(An) */
void REGPARAM2 CPUFUNC(op_113c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_1140_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1150_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_1158_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1160_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_1168_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_1170_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_1178_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_1179_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_117a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_117b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(d16,An) */
void REGPARAM2 CPUFUNC(op_117c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1180_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_1198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_11bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_11c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_11d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_11e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_11f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_11f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_11f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_11fa_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_11fb_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(xxx).W */
void REGPARAM2 CPUFUNC(op_11fc_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_13c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_13d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_13e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_13f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_13f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_13f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (10);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_13fa_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_13fb_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.B #<data>.B,(xxx).L */
void REGPARAM2 CPUFUNC(op_13fc_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_2000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_2008_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_2010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,Dn */
void REGPARAM2 CPUFUNC(op_2018_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_2020_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_2028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_2030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_2038_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_2039_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_203a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_203b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,Dn */
void REGPARAM2 CPUFUNC(op_203c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L Dn,An */
void REGPARAM2 CPUFUNC(op_2040_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_2048_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_2050_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_2058_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L -(An),An */
void REGPARAM2 CPUFUNC(op_2060_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_2068_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_2070_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	m68k_areg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_2078_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_2079_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_207a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_207b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	m68k_areg (regs, dstreg) = (src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_207c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_2080_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(An) */
void REGPARAM2 CPUFUNC(op_2088_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(An) */
void REGPARAM2 CPUFUNC(op_2090_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(An) */
void REGPARAM2 CPUFUNC(op_2098_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(An) */
void REGPARAM2 CPUFUNC(op_20a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_20a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_20b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_20b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_20b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_20ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_20bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(An) */
void REGPARAM2 CPUFUNC(op_20bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_20c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(An)+ */
void REGPARAM2 CPUFUNC(op_20c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(An)+ */
void REGPARAM2 CPUFUNC(op_20d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_20d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_20e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_20e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_20f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_20f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_20f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_20fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_20fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(An)+ */
void REGPARAM2 CPUFUNC(op_20fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_2100_21)(uae_u32 opcode)
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
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,-(An) */
void REGPARAM2 CPUFUNC(op_2108_21)(uae_u32 opcode)
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
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),-(An) */
void REGPARAM2 CPUFUNC(op_2110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_2118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),-(An) */
void REGPARAM2 CPUFUNC(op_2120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_2128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_2130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_2138_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_2139_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_213a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_213b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,-(An) */
void REGPARAM2 CPUFUNC(op_213c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_2140_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(d16,An) */
void REGPARAM2 CPUFUNC(op_2148_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2150_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_2158_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2160_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif

#ifdef PART_3
/* MOVE.L (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_2168_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_2170_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_2178_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_2179_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_217a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_217b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(d16,An) */
void REGPARAM2 CPUFUNC(op_217c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2180_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2188_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_2198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_21bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_21c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(xxx).W */
void REGPARAM2 CPUFUNC(op_21c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_21d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_21e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_21f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_21f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_21f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_21fa_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_21fb_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(xxx).W */
void REGPARAM2 CPUFUNC(op_21fc_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_23c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L An,(xxx).L */
void REGPARAM2 CPUFUNC(op_23c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_23d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_23e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_23f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_23f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_23f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (10);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_23fa_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_23fb_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.L #<data>.L,(xxx).L */
void REGPARAM2 CPUFUNC(op_23fc_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
	m68k_incpc (10);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_3000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_3008_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_3010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_3018_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_3020_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_3028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_3030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_3038_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_3039_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_303a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_303b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_303c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W Dn,An */
void REGPARAM2 CPUFUNC(op_3040_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_3048_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_3050_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_3058_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W -(An),An */
void REGPARAM2 CPUFUNC(op_3060_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_3068_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_3070_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_3078_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_3079_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_307a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_307b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVEA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_307c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_3080_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(An) */
void REGPARAM2 CPUFUNC(op_3088_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(An) */
void REGPARAM2 CPUFUNC(op_3090_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(An) */
void REGPARAM2 CPUFUNC(op_3098_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(An) */
void REGPARAM2 CPUFUNC(op_30a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(An) */
void REGPARAM2 CPUFUNC(op_30a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(An) */
void REGPARAM2 CPUFUNC(op_30b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(An) */
void REGPARAM2 CPUFUNC(op_30b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(An) */
void REGPARAM2 CPUFUNC(op_30b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(An) */
void REGPARAM2 CPUFUNC(op_30ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(An) */
void REGPARAM2 CPUFUNC(op_30bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_30bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_30c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(An)+ */
void REGPARAM2 CPUFUNC(op_30c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(An)+ */
void REGPARAM2 CPUFUNC(op_30d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(An)+ */
void REGPARAM2 CPUFUNC(op_30d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(An)+ */
void REGPARAM2 CPUFUNC(op_30e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(An)+ */
void REGPARAM2 CPUFUNC(op_30e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_30f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(An)+ */
void REGPARAM2 CPUFUNC(op_30f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(An)+ */
void REGPARAM2 CPUFUNC(op_30f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(An)+ */
void REGPARAM2 CPUFUNC(op_30fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(An)+ */
void REGPARAM2 CPUFUNC(op_30fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_30fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_3100_21)(uae_u32 opcode)
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
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,-(An) */
void REGPARAM2 CPUFUNC(op_3108_21)(uae_u32 opcode)
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
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),-(An) */
void REGPARAM2 CPUFUNC(op_3110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,-(An) */
void REGPARAM2 CPUFUNC(op_3118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),-(An) */
void REGPARAM2 CPUFUNC(op_3120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),-(An) */
void REGPARAM2 CPUFUNC(op_3128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_3130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,-(An) */
void REGPARAM2 CPUFUNC(op_3138_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,-(An) */
void REGPARAM2 CPUFUNC(op_3139_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),-(An) */
void REGPARAM2 CPUFUNC(op_313a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),-(An) */
void REGPARAM2 CPUFUNC(op_313b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_313c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	m68k_areg (regs, dstreg) = dsta;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_3140_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(d16,An) */
void REGPARAM2 CPUFUNC(op_3148_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3150_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(d16,An) */
void REGPARAM2 CPUFUNC(op_3158_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3160_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(d16,An) */
void REGPARAM2 CPUFUNC(op_3168_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_3170_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(d16,An) */
void REGPARAM2 CPUFUNC(op_3178_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(d16,An) */
void REGPARAM2 CPUFUNC(op_3179_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(d16,An) */
void REGPARAM2 CPUFUNC(op_317a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(d16,An) */
void REGPARAM2 CPUFUNC(op_317b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_317c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3180_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3188_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_3198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (6);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta;
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_31bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_31c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(xxx).W */
void REGPARAM2 CPUFUNC(op_31c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(xxx).W */
void REGPARAM2 CPUFUNC(op_31d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(xxx).W */
void REGPARAM2 CPUFUNC(op_31e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_31f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(xxx).W */
void REGPARAM2 CPUFUNC(op_31f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(xxx).W */
void REGPARAM2 CPUFUNC(op_31f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(xxx).W */
void REGPARAM2 CPUFUNC(op_31fa_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(xxx).W */
void REGPARAM2 CPUFUNC(op_31fb_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (2);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_31fc_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_33c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W An,(xxx).L */
void REGPARAM2 CPUFUNC(op_33c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (An)+,(xxx).L */
void REGPARAM2 CPUFUNC(op_33d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W -(An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (6);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,An),(xxx).L */
void REGPARAM2 CPUFUNC(op_33e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,An,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_33f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).W,(xxx).L */
void REGPARAM2 CPUFUNC(op_33f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (xxx).L,(xxx).L */
void REGPARAM2 CPUFUNC(op_33f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (10);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d16,PC),(xxx).L */
void REGPARAM2 CPUFUNC(op_33fa_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (8);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W (d8,PC,Xn),(xxx).L */
void REGPARAM2 CPUFUNC(op_33fb_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uaecptr dsta = get_long_ce030_prefetch (0);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (4);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MOVE.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_33fc_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
	m68k_incpc (8);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.B Dn */
void REGPARAM2 CPUFUNC(op_4000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.B (An)+ */
void REGPARAM2 CPUFUNC(op_4018_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.B -(An) */
void REGPARAM2 CPUFUNC(op_4020_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce030 (srca, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4038_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4039_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEGX.W Dn */
void REGPARAM2 CPUFUNC(op_4040_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4050_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.W (An)+ */
void REGPARAM2 CPUFUNC(op_4058_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
	put_word_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.W -(An) */
void REGPARAM2 CPUFUNC(op_4060_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
	put_word_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4068_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4070_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce030 (srca, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4078_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4079_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEGX.L Dn */
void REGPARAM2 CPUFUNC(op_4080_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4090_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.L (An)+ */
void REGPARAM2 CPUFUNC(op_4098_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
	put_long_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.L -(An) */
void REGPARAM2 CPUFUNC(op_40a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
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
	put_long_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEGX.L (d16,An) */
void REGPARAM2 CPUFUNC(op_40a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_40b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce030 (srca, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEGX.L (xxx).W */
void REGPARAM2 CPUFUNC(op_40b8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEGX.L (xxx).L */
void REGPARAM2 CPUFUNC(op_40b9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVSR2.W Dn */
void REGPARAM2 CPUFUNC(op_40c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7766; }
{{	MakeSR ();
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((regs.sr) & 0xffff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel7766: ;
return;
}

/* MVSR2.W (An) */
void REGPARAM2 CPUFUNC(op_40d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7767; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
	MakeSR ();
	put_word_ce030 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel7767: ;
return;
}

/* MVSR2.W (An)+ */
void REGPARAM2 CPUFUNC(op_40d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7768; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 2;
	MakeSR ();
	put_word_ce030 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel7768: ;
return;
}

/* MVSR2.W -(An) */
void REGPARAM2 CPUFUNC(op_40e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7769; }
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	m68k_areg (regs, srcreg) = srca;
	MakeSR ();
	put_word_ce030 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel7769: ;
return;
}

/* MVSR2.W (d16,An) */
void REGPARAM2 CPUFUNC(op_40e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7770; }
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	MakeSR ();
	put_word_ce030 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7770: ;
return;
}

/* MVSR2.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_40f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7771; }
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
	MakeSR ();
	put_word_ce030 (srca, regs.sr);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7771: ;
return;
}

/* MVSR2.W (xxx).W */
void REGPARAM2 CPUFUNC(op_40f8_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7772; }
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	MakeSR ();
	put_word_ce030 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7772: ;
return;
}

/* MVSR2.W (xxx).L */
void REGPARAM2 CPUFUNC(op_40f9_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7773; }
{{	uaecptr srca = get_long_ce030_prefetch (2);
	MakeSR ();
	put_word_ce030 (srca, regs.sr);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7773: ;
return;
}

/* CHK.L Dn,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4100_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7774;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7774;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7774: ;
return;
}

#endif
/* CHK.L (An),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7775;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7775;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7775: ;
return;
}

#endif
/* CHK.L (An)+,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7776;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7776;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7776: ;
return;
}

#endif
/* CHK.L -(An),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7777;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7777;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7777: ;
return;
}

#endif
/* CHK.L (d16,An),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7778;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7778;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7778: ;
return;
}

#endif
/* CHK.L (d8,An,Xn),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7779;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7779;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7779: ;
return;
}

#endif
/* CHK.L (xxx).W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4138_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7780;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7780;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7780: ;
return;
}

#endif
/* CHK.L (xxx).L,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4139_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (6);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7781;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7781;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7781: ;
return;
}

#endif
/* CHK.L (d16,PC),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_413a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7782;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7782;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7782: ;
return;
}

#endif
/* CHK.L (d8,PC,Xn),Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_413b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7783;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7783;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7783: ;
return;
}

#endif
/* CHK.L #<data>.L,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_413c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (6);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7784;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7784;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7784: ;
return;
}

#endif
/* CHK.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_4180_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7785;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7785;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7785: ;
return;
}

/* CHK.W (An),Dn */
void REGPARAM2 CPUFUNC(op_4190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7786;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7786;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7786: ;
return;
}

/* CHK.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_4198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7787;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7787;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7787: ;
return;
}

/* CHK.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_41a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7788;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7788;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7788: ;
return;
}

/* CHK.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_41a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7789;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7789;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7789: ;
return;
}

/* CHK.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_41b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7790;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7790;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7790: ;
return;
}

/* CHK.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_41b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7791;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7791;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7791: ;
return;
}

/* CHK.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_41b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (6);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7792;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7792;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7792: ;
return;
}

/* CHK.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_41ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7793;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7793;
	}
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7793: ;
return;
}

/* CHK.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_41bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7794;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7794;
	}
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7794: ;
return;
}

/* CHK.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_41bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel7795;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel7795;
	}
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7795: ;
return;
}

/* LEA.L (An),An */
void REGPARAM2 CPUFUNC(op_41d0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_41e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LEA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_41f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	m68k_areg (regs, dstreg) = (srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LEA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_41f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LEA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_41f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* LEA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_41fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	m68k_areg (regs, dstreg) = (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LEA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_41fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	m68k_areg (regs, dstreg) = (srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.B Dn */
void REGPARAM2 CPUFUNC(op_4200_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4210_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.B (An)+ */
void REGPARAM2 CPUFUNC(op_4218_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.B -(An) */
void REGPARAM2 CPUFUNC(op_4220_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4228_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4230_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce030 (srca, 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4238_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4239_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CLR.W Dn */
void REGPARAM2 CPUFUNC(op_4240_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4250_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.W (An)+ */
void REGPARAM2 CPUFUNC(op_4258_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.W -(An) */
void REGPARAM2 CPUFUNC(op_4260_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4268_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4270_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce030 (srca, 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4278_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4279_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CLR.L Dn */
void REGPARAM2 CPUFUNC(op_4280_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4290_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.L (An)+ */
void REGPARAM2 CPUFUNC(op_4298_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.L -(An) */
void REGPARAM2 CPUFUNC(op_42a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* CLR.L (d16,An) */
void REGPARAM2 CPUFUNC(op_42a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* CLR.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_42b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce030 (srca, 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* CLR.L (xxx).W */
void REGPARAM2 CPUFUNC(op_42b8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif

#ifdef PART_4
/* CLR.L (xxx).L */
void REGPARAM2 CPUFUNC(op_42b9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long_ce030 (srca, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVSR2.B Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_42d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	MakeSR ();
	put_word_ce030 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* MVSR2.B (An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += 2;
	MakeSR ();
	put_word_ce030 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* MVSR2.B -(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	m68k_areg (regs, srcreg) = srca;
	MakeSR ();
	put_word_ce030 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif
/* MVSR2.B (d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	MakeSR ();
	put_word_ce030 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* MVSR2.B (d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
	MakeSR ();
	put_word_ce030 (srca, regs.sr & 0xff);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* MVSR2.B (xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	MakeSR ();
	put_word_ce030 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* MVSR2.B (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_42f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
	MakeSR ();
	put_word_ce030 (srca, regs.sr & 0xff);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* NEG.B Dn */
void REGPARAM2 CPUFUNC(op_4400_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4410_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.B (An)+ */
void REGPARAM2 CPUFUNC(op_4418_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.B -(An) */
void REGPARAM2 CPUFUNC(op_4420_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4428_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4430_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (srca, dst);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEG.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4438_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4439_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEG.W Dn */
void REGPARAM2 CPUFUNC(op_4440_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4450_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.W (An)+ */
void REGPARAM2 CPUFUNC(op_4458_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
	put_word_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.W -(An) */
void REGPARAM2 CPUFUNC(op_4460_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
	put_word_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4468_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4470_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (srca, dst);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEG.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4478_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4479_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NEG.L Dn */
void REGPARAM2 CPUFUNC(op_4480_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4490_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.L (An)+ */
void REGPARAM2 CPUFUNC(op_4498_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
	put_long_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.L -(An) */
void REGPARAM2 CPUFUNC(op_44a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
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
	put_long_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NEG.L (d16,An) */
void REGPARAM2 CPUFUNC(op_44a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_44b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (srca, dst);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NEG.L (xxx).W */
void REGPARAM2 CPUFUNC(op_44b8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NEG.L (xxx).L */
void REGPARAM2 CPUFUNC(op_44b9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (srca, dst);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MV2SR.B Dn */
void REGPARAM2 CPUFUNC(op_44c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_44d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MV2SR.B (An)+ */
void REGPARAM2 CPUFUNC(op_44d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_44e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_44e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MV2SR.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_44f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MV2SR.B (xxx).W */
void REGPARAM2 CPUFUNC(op_44f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MV2SR.B (xxx).L */
void REGPARAM2 CPUFUNC(op_44f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MV2SR.B (d16,PC) */
void REGPARAM2 CPUFUNC(op_44fa_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MV2SR.B (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_44fb_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MV2SR.B #<data>.B */
void REGPARAM2 CPUFUNC(op_44fc_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.B Dn */
void REGPARAM2 CPUFUNC(op_4600_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4610_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.B (An)+ */
void REGPARAM2 CPUFUNC(op_4618_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.B -(An) */
void REGPARAM2 CPUFUNC(op_4620_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4628_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4630_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce030 (srca, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NOT.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4638_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4639_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NOT.W Dn */
void REGPARAM2 CPUFUNC(op_4640_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4650_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.W (An)+ */
void REGPARAM2 CPUFUNC(op_4658_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.W -(An) */
void REGPARAM2 CPUFUNC(op_4660_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4668_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4670_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce030 (srca, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NOT.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4678_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4679_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* NOT.L Dn */
void REGPARAM2 CPUFUNC(op_4680_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4690_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.L (An)+ */
void REGPARAM2 CPUFUNC(op_4698_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.L -(An) */
void REGPARAM2 CPUFUNC(op_46a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NOT.L (d16,An) */
void REGPARAM2 CPUFUNC(op_46a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_46b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce030 (srca, dst);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NOT.L (xxx).W */
void REGPARAM2 CPUFUNC(op_46b8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NOT.L (xxx).L */
void REGPARAM2 CPUFUNC(op_46b9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long_ce030 (srca, dst);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MV2SR.W Dn */
void REGPARAM2 CPUFUNC(op_46c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7894; }
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	regs.sr = src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel7894: ;
return;
}

/* MV2SR.W (An) */
void REGPARAM2 CPUFUNC(op_46d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7895; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel7895: ;
return;
}

/* MV2SR.W (An)+ */
void REGPARAM2 CPUFUNC(op_46d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7896; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel7896: ;
return;
}

/* MV2SR.W -(An) */
void REGPARAM2 CPUFUNC(op_46e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7897; }
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel7897: ;
return;
}

/* MV2SR.W (d16,An) */
void REGPARAM2 CPUFUNC(op_46e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7898; }
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7898: ;
return;
}

/* MV2SR.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_46f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel7899; }
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7899: ;
return;
}

/* MV2SR.W (xxx).W */
void REGPARAM2 CPUFUNC(op_46f8_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7900; }
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7900: ;
return;
}

/* MV2SR.W (xxx).L */
void REGPARAM2 CPUFUNC(op_46f9_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7901; }
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel7901: ;
return;
}

/* MV2SR.W (d16,PC) */
void REGPARAM2 CPUFUNC(op_46fa_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7902; }
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7902: ;
return;
}

/* MV2SR.W (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_46fb_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7903; }
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
	regs.sr = src;
	MakeFromSR ();
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel7903: ;
return;
}

/* MV2SR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_46fc_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel7904; }
{{	uae_s16 src = get_word_ce030_prefetch (2);
	regs.sr = src;
	MakeFromSR ();
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel7904: ;
return;
}

/* NBCD.B Dn */
void REGPARAM2 CPUFUNC(op_4800_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4808_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr olda;
	olda = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = olda;
{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 offs = get_long_ce030_prefetch (2);
	put_long_ce030 (olda, src);
	m68k_areg (regs, srcreg) = (m68k_areg (regs, 7));
	m68k_areg (regs, 7) += offs;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* NBCD.B (An) */
void REGPARAM2 CPUFUNC(op_4810_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B (An)+ */
void REGPARAM2 CPUFUNC(op_4818_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B -(An) */
void REGPARAM2 CPUFUNC(op_4820_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4828_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NBCD.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4830_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NBCD.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4838_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NBCD.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4839_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SWAP.W Dn */
void REGPARAM2 CPUFUNC(op_4840_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4848_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	m68k_incpc (2);
	op_illg (opcode);
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* PEA.L (An) */
void REGPARAM2 CPUFUNC(op_4850_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce030 (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* PEA.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4868_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce030 (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* PEA.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4870_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce030 (dsta, srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* PEA.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4878_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce030 (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* PEA.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4879_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce030 (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* PEA.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_487a_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce030 (dsta, srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* PEA.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_487b_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = dsta;
	put_long_ce030 (dsta, srca);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EXT.W Dn */
void REGPARAM2 CPUFUNC(op_4880_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4890_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce030 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce030 (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.W #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_48a0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 0;
{	uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
	int type = get_cpu_model () >= 68020;
	while (amask) {
		srca -= 2;
		if (type) m68k_areg (regs, dstreg) = srca;
		put_word_ce030 (srca, m68k_areg (regs, movem_index2[amask]));
		amask = movem_next[amask];
	}
	while (dmask) { srca -= 2; put_word_ce030 (srca, m68k_dreg (regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_48a8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce030 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce030 (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_48b0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce030 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce030 (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMLE.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_48b8_21)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce030 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce030 (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_48b9_21)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca = get_long_ce030_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word_ce030 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word_ce030 (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EXT.L Dn */
void REGPARAM2 CPUFUNC(op_48c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_48d0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long_ce030 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long_ce030 (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.L #<data>.W,-(An) */
void REGPARAM2 CPUFUNC(op_48e0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 0;
{	uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
	int type = get_cpu_model () >= 68020;
	while (amask) {
		srca -= 4;
		if (type) m68k_areg (regs, dstreg) = srca;
		put_long_ce030 (srca, m68k_areg (regs, movem_index2[amask]));
		amask = movem_next[amask];
	}
	while (dmask) { srca -= 4; put_long_ce030 (srca, m68k_dreg (regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMLE.L #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_48e8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long_ce030 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long_ce030 (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.L #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_48f0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long_ce030 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long_ce030 (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMLE.L #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_48f8_21)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long_ce030 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long_ce030 (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMLE.L #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_48f9_21)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce030_prefetch (2);
{	uaecptr srca = get_long_ce030_prefetch (4);
{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long_ce030 (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long_ce030 (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* EXT.B Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_49c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a00_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a10_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.B (An)+ */
void REGPARAM2 CPUFUNC(op_4a18_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.B -(An) */
void REGPARAM2 CPUFUNC(op_4a20_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4a28_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4a30_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TST.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4a38_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4a39_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TST.B (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a3a_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_4a3b_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* TST.B #<data>.B */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a3c_21)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* TST.W Dn */
void REGPARAM2 CPUFUNC(op_4a40_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a48_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a50_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.W (An)+ */
void REGPARAM2 CPUFUNC(op_4a58_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.W -(An) */
void REGPARAM2 CPUFUNC(op_4a60_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.W (d16,An) */
void REGPARAM2 CPUFUNC(op_4a68_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4a70_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TST.W (xxx).W */
void REGPARAM2 CPUFUNC(op_4a78_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.W (xxx).L */
void REGPARAM2 CPUFUNC(op_4a79_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TST.W (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a7a_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_4a7b_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* TST.W #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4a7c_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* TST.L Dn */
void REGPARAM2 CPUFUNC(op_4a80_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a88_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4a90_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.L (An)+ */
void REGPARAM2 CPUFUNC(op_4a98_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.L -(An) */
void REGPARAM2 CPUFUNC(op_4aa0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TST.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4aa8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4ab0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TST.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4ab8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TST.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4ab9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TST.L (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4aba_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_4abb_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* TST.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4abc_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* TAS.B Dn */
void REGPARAM2 CPUFUNC(op_4ac0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4ad0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce030 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TAS.B (An)+ */
void REGPARAM2 CPUFUNC(op_4ad8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce030 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TAS.B -(An) */
void REGPARAM2 CPUFUNC(op_4ae0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce030 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* TAS.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4ae8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce030 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TAS.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4af0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce030 (srca, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TAS.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4af8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce030 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* TAS.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4af9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	src |= 0x80;
	put_byte_ce030 (srca, src);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MULL.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c00_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}return;
}

#endif
/* MULL.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c10_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c18_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c20_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (4);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c28_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (6);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c30_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_mull(opcode, dst, extra);
}}}}}return;
}

#endif
/* MULL.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c38_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (6);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c39_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (8);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c3a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (6);
	m68k_mull(opcode, dst, extra);
}}}}return;
}

#endif
/* MULL.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c3b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_mull(opcode, dst, extra);
}}}}}return;
}

#endif
/* MULL.L #<data>.W,#<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c3c_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030_prefetch (4);
	regs.ce020memcycles += 40 * cpucycleunit;
	m68k_incpc (8);
	m68k_mull(opcode, dst, extra);
}}}return;
}

#endif
/* DIVL.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c40_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}return;
}

#endif
/* DIVL.L #<data>.W,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c50_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c58_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c60_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (4);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c68_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (6);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c70_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_divl(opcode, dst, extra);
}}}}}return;
}

#endif
/* DIVL.L #<data>.W,(xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c78_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (6);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c79_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (8);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c7a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (6);
	m68k_divl(opcode, dst, extra);
}}}}return;
}

#endif
/* DIVL.L #<data>.W,(d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c7b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_divl(opcode, dst, extra);
}}}}}return;
}

#endif
/* DIVL.L #<data>.W,#<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4c7c_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030_prefetch (4);
	regs.ce020memcycles += 70 * cpucycleunit;
	m68k_incpc (8);
	m68k_divl(opcode, dst, extra);
}}}return;
}

#endif
/* MVMEL.W #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_4c90_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.W #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_4c98_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; amask = movem_next[amask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.W #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_4ca8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.W #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4cb0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMEL.W #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_4cb8_21)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.W #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_4cb9_21)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = get_long_ce030_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* MVMEL.W #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_4cba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.W #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4cbb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word_ce030 (srca); srca += 2; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMEL.L #<data>.W,(An) */
void REGPARAM2 CPUFUNC(op_4cd0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce030 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce030 (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.L #<data>.W,(An)+ */
void REGPARAM2 CPUFUNC(op_4cd8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce030 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce030 (srca); srca += 4; amask = movem_next[amask]; }
	m68k_areg (regs, dstreg) = srca;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* MVMEL.L #<data>.W,(d16,An) */
void REGPARAM2 CPUFUNC(op_4ce8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce030 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce030 (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.L #<data>.W,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4cf0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca;
	m68k_incpc (4);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce030 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce030 (srca); srca += 4; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* MVMEL.L #<data>.W,(xxx).W */
void REGPARAM2 CPUFUNC(op_4cf8_21)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce030 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce030 (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.L #<data>.W,(xxx).L */
void REGPARAM2 CPUFUNC(op_4cf9_21)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = get_long_ce030_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce030 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce030 (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (8);
return;
}

/* MVMEL.L #<data>.W,(d16,PC) */
void REGPARAM2 CPUFUNC(op_4cfa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce030 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce030 (srca); srca += 4; amask = movem_next[amask]; }
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MVMEL.L #<data>.W,(d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4cfb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{	uae_u16 mask = get_word_ce030_prefetch (2);
	unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long_ce030 (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long_ce030 (srca); srca += 4; amask = movem_next[amask]; }
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* TRAPQ.L #<data> */
void REGPARAM2 CPUFUNC(op_4e40_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 15);
{{	uae_u32 src = srcreg;
	m68k_incpc (2);
	Exception (src + 32);
}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LINK.W An,#<data>.W */
void REGPARAM2 CPUFUNC(op_4e50_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr olda;
	olda = m68k_areg (regs, 7) - 4;
	m68k_areg (regs, 7) = olda;
{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	put_long_ce030 (olda, src);
	m68k_areg (regs, srcreg) = (m68k_areg (regs, 7));
	m68k_areg (regs, 7) += offs;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* UNLK.L An */
void REGPARAM2 CPUFUNC(op_4e58_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
	m68k_areg (regs, 7) = src;
{	uaecptr olda = m68k_areg (regs, 7);
{	uae_s32 old = get_long_ce030 (olda);
	m68k_areg (regs, 7) += 4;
	m68k_areg (regs, srcreg) = (old);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* MVR2USP.L An */
void REGPARAM2 CPUFUNC(op_4e60_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8022; }
{{	uae_s32 src = m68k_areg (regs, srcreg);
	regs.usp = src;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8022: ;
return;
}

/* MVUSP2R.L An */
void REGPARAM2 CPUFUNC(op_4e68_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8023; }
{{	m68k_areg (regs, srcreg) = (regs.usp);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8023: ;
return;
}

/* RESET.L  */
void REGPARAM2 CPUFUNC(op_4e70_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8024; }
{	cpureset ();
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8024: ;
return;
}

/* NOP.L  */
void REGPARAM2 CPUFUNC(op_4e71_21)(uae_u32 opcode)
{
{	do_cycles_ce020 (6);
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* STOP.L #<data>.W */
void REGPARAM2 CPUFUNC(op_4e72_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8026; }
{{	uae_s16 src = get_word_ce030_prefetch (2);
	regs.sr = src;
	MakeFromSR ();
	m68k_setstopped ();
	m68k_incpc (4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8026: ;
return;
}

/* RTE.L  */
void REGPARAM2 CPUFUNC(op_4e73_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8027; }
{	uae_u16 newsr; uae_u32 newpc;
	for (;;) {
		uaecptr a = m68k_areg (regs, 7);
		uae_s16 sr = get_word_ce030 (a);
		uae_s32 pc = get_long_ce030 (a + 2);
		uae_s16 format = get_word_ce030 (a + 2 + 4);
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
		else { m68k_areg (regs, 7) += offset; Exception (14); goto endlabel8027; }
		regs.sr = newsr; MakeFromSR ();
}
	regs.sr = newsr; MakeFromSR ();
	if (newpc & 1) {
		exception3i (0x4E73, newpc);
		goto endlabel8027;
	}
		m68k_setpc (newpc);
	ipl_fetch ();
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8027: ;
return;
}

/* RTD.L #<data>.W */
void REGPARAM2 CPUFUNC(op_4e74_21)(uae_u32 opcode)
{
{{	uaecptr pca = m68k_areg (regs, 7);
{	uae_s32 pc = get_long_ce030 (pca);
	m68k_areg (regs, 7) += 4;
{	uae_s16 offs = get_word_ce030_prefetch (2);
	m68k_areg (regs, 7) += offs;
	if (pc & 1) {
		exception3i (0x4E74, pc);
		goto endlabel8028;
	}
	if (pc & 1) {
		exception3i (0x4E74, pc);
		goto endlabel8028;
	}
	m68k_setpc (pc);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8028: ;
return;
}

/* RTS.L  */
void REGPARAM2 CPUFUNC(op_4e75_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4e76_21)(uae_u32 opcode)
{
{	m68k_incpc (2);
	if (GET_VFLG ()) {
		Exception (7);
		goto endlabel8030;
	}
}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8030: ;
return;
}

/* RTR.L  */
void REGPARAM2 CPUFUNC(op_4e77_21)(uae_u32 opcode)
{
{	uaecptr oldpc = m68k_getpc ();
	MakeSR ();
{	uaecptr sra = m68k_areg (regs, 7);
{	uae_s16 sr = get_word_ce030 (sra);
	m68k_areg (regs, 7) += 2;
{	uaecptr pca = m68k_areg (regs, 7);
{	uae_s32 pc = get_long_ce030 (pca);
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
void REGPARAM2 CPUFUNC(op_4e7a_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8032; }
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	int regno = (src >> 12) & 15;
	uae_u32 *regp = regs.regs + regno;
	if (! m68k_movec2(src & 0xFFF, regp)) goto endlabel8032;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8032: ;
return;
}

#endif
/* MOVE2C.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_4e7b_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8033; }
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	int regno = (src >> 12) & 15;
	uae_u32 *regp = regs.regs + regno;
	if (! m68k_move2c(src & 0xFFF, regp)) goto endlabel8033;
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8033: ;
return;
}

#endif
/* JSR.L (An) */
void REGPARAM2 CPUFUNC(op_4e90_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uaecptr oldpc = m68k_getpc () + 2;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8034;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce030 (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8034: ;
return;
}

/* JSR.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4ea8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8035;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce030 (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8035: ;
return;
}

/* JSR.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4eb0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uaecptr oldpc = m68k_getpc () + 0;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8036;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce030 (m68k_areg (regs, 7), oldpc);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8036: ;
return;
}

/* JSR.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4eb8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8037;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce030 (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8037: ;
return;
}

/* JSR.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4eb9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 6;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8038;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce030 (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8038: ;
return;
}

/* JSR.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_4eba_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8039;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce030 (m68k_areg (regs, 7), oldpc);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8039: ;
return;
}

/* JSR.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4ebb_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uaecptr oldpc = m68k_getpc () + 0;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8040;
	}
	m68k_setpc (srca);
	m68k_areg (regs, 7) -= 4;
	put_long_ce030 (m68k_areg (regs, 7), oldpc);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8040: ;
return;
}

/* JMP.L (An) */
void REGPARAM2 CPUFUNC(op_4ed0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8041;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8041: ;
return;
}

/* JMP.L (d16,An) */
void REGPARAM2 CPUFUNC(op_4ee8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8042;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8042: ;
return;
}

/* JMP.L (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4ef0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8043;
	}
	m68k_setpc (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8043: ;
return;
}

/* JMP.L (xxx).W */
void REGPARAM2 CPUFUNC(op_4ef8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8044;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8044: ;
return;
}

/* JMP.L (xxx).L */
void REGPARAM2 CPUFUNC(op_4ef9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8045;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8045: ;
return;
}

/* JMP.L (d16,PC) */
void REGPARAM2 CPUFUNC(op_4efa_21)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8046;
	}
	m68k_setpc (srca);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8046: ;
return;
}

/* JMP.L (d8,PC,Xn) */
void REGPARAM2 CPUFUNC(op_4efb_21)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel8047;
	}
	m68k_setpc (srca);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8047: ;
return;
}

/* ADDQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.B #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5018_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.B #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5020_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.B #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.B #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDQ.B #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5038_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.B #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5039_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5040_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5048_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5050_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.W #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5058_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.W #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5060_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif

#ifdef PART_5
/* ADDQ.W #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5068_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.W #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5070_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDQ.W #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5078_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.W #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5079_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5080_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5088_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5090_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.L #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5098_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.L #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_50a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDQ.L #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_50a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.L #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_50b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDQ.L #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_50b8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDQ.L #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_50b9_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_50c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (0) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_50c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (0)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8075;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8075: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_50d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_50d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_50e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_50e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_50f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_50f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_50f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (0) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_50fa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (0)) { Exception (7); goto endlabel8083; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8083: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_50fb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (0)) { Exception (7); goto endlabel8084; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8084: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_50fc_21)(uae_u32 opcode)
{
{	if (cctrue (0)) { Exception (7); goto endlabel8085; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8085: ;
return;
}

#endif
/* SUBQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5100_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.B #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.B #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.B #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.B #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBQ.B #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5138_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.B #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5139_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBQ.W #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5140_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5148_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5150_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.W #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5158_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.W #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_5160_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.W #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_5168_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.W #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5170_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBQ.W #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_5178_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.W #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_5179_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_5180_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5188_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_5190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.L #<data>,(An)+ */
void REGPARAM2 CPUFUNC(op_5198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.L #<data>,-(An) */
void REGPARAM2 CPUFUNC(op_51a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBQ.L #<data>,(d16,An) */
void REGPARAM2 CPUFUNC(op_51a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.L #<data>,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_51b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBQ.L #<data>,(xxx).W */
void REGPARAM2 CPUFUNC(op_51b8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBQ.L #<data>,(xxx).L */
void REGPARAM2 CPUFUNC(op_51b9_21)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_51c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (1) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_51c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (1)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8113;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8113: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_51d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_51d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_51e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_51e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_51f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_51f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_51f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (1) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_51fa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (1)) { Exception (7); goto endlabel8121; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8121: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_51fb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (1)) { Exception (7); goto endlabel8122; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8122: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_51fc_21)(uae_u32 opcode)
{
{	if (cctrue (1)) { Exception (7); goto endlabel8123; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8123: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_52c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (2) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_52c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (2)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8125;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8125: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_52d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_52d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_52e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_52e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_52f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_52f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_52f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (2) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_52fa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (2)) { Exception (7); goto endlabel8133; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8133: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_52fb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (2)) { Exception (7); goto endlabel8134; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8134: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_52fc_21)(uae_u32 opcode)
{
{	if (cctrue (2)) { Exception (7); goto endlabel8135; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8135: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_53c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (3) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_53c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (3)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8137;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8137: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_53d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_53d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_53e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_53e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_53f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_53f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_53f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (3) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_53fa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (3)) { Exception (7); goto endlabel8145; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8145: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_53fb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (3)) { Exception (7); goto endlabel8146; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8146: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_53fc_21)(uae_u32 opcode)
{
{	if (cctrue (3)) { Exception (7); goto endlabel8147; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8147: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_54c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (4) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_54c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (4)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8149;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8149: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_54d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_54d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_54e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_54e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_54f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_54f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_54f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (4) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_54fa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (4)) { Exception (7); goto endlabel8157; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8157: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_54fb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (4)) { Exception (7); goto endlabel8158; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8158: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_54fc_21)(uae_u32 opcode)
{
{	if (cctrue (4)) { Exception (7); goto endlabel8159; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8159: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_55c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (5) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_55c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (5)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8161;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8161: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_55d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_55d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_55e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_55e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_55f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_55f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_55f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (5) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_55fa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (5)) { Exception (7); goto endlabel8169; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8169: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_55fb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (5)) { Exception (7); goto endlabel8170; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8170: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_55fc_21)(uae_u32 opcode)
{
{	if (cctrue (5)) { Exception (7); goto endlabel8171; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8171: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_56c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (6) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_56c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (6)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8173;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8173: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_56d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_56d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_56e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_56e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_56f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_56f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_56f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (6) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_56fa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (6)) { Exception (7); goto endlabel8181; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8181: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_56fb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (6)) { Exception (7); goto endlabel8182; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8182: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_56fc_21)(uae_u32 opcode)
{
{	if (cctrue (6)) { Exception (7); goto endlabel8183; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8183: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_57c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (7) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_57c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (7)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8185;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8185: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_57d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_57d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_57e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_57e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_57f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_57f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_57f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (7) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_57fa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (7)) { Exception (7); goto endlabel8193; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8193: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_57fb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (7)) { Exception (7); goto endlabel8194; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8194: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_57fc_21)(uae_u32 opcode)
{
{	if (cctrue (7)) { Exception (7); goto endlabel8195; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8195: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_58c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (8) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_58c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (8)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8197;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8197: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_58d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_58d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_58e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_58e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_58f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_58f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_58f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (8) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_58fa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (8)) { Exception (7); goto endlabel8205; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8205: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_58fb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (8)) { Exception (7); goto endlabel8206; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8206: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_58fc_21)(uae_u32 opcode)
{
{	if (cctrue (8)) { Exception (7); goto endlabel8207; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8207: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_59c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (9) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_59c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (9)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8209;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8209: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_59d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_59d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_59e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_59e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_59f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_59f8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_59f9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (9) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_59fa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (9)) { Exception (7); goto endlabel8217; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8217: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_59fb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (9)) { Exception (7); goto endlabel8218; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8218: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_59fc_21)(uae_u32 opcode)
{
{	if (cctrue (9)) { Exception (7); goto endlabel8219; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8219: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5ac0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (10) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5ac8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (10)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8221;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8221: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5ad0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5ad8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ae0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ae8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5af0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5af8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5af9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (10) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5afa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (10)) { Exception (7); goto endlabel8229; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8229: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5afb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (10)) { Exception (7); goto endlabel8230; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8230: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5afc_21)(uae_u32 opcode)
{
{	if (cctrue (10)) { Exception (7); goto endlabel8231; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8231: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5bc0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (11) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5bc8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (11)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8233;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8233: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5bd0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5bd8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5be0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5be8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5bf0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5bf8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5bf9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (11) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5bfa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (11)) { Exception (7); goto endlabel8241; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8241: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5bfb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (11)) { Exception (7); goto endlabel8242; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8242: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5bfc_21)(uae_u32 opcode)
{
{	if (cctrue (11)) { Exception (7); goto endlabel8243; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8243: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5cc0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (12) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5cc8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (12)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8245;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8245: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5cd0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5cd8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ce0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ce8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5cf0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5cf8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5cf9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (12) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5cfa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (12)) { Exception (7); goto endlabel8253; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8253: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5cfb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (12)) { Exception (7); goto endlabel8254; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8254: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5cfc_21)(uae_u32 opcode)
{
{	if (cctrue (12)) { Exception (7); goto endlabel8255; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8255: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5dc0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (13) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5dc8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (13)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8257;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8257: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5dd0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5dd8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5de0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5de8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5df0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5df8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5df9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (13) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5dfa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (13)) { Exception (7); goto endlabel8265; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8265: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5dfb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (13)) { Exception (7); goto endlabel8266; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8266: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5dfc_21)(uae_u32 opcode)
{
{	if (cctrue (13)) { Exception (7); goto endlabel8267; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8267: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5ec0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (14) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5ec8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (14)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8269;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8269: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5ed0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5ed8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5ee0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5ee8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5ef0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5ef8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5ef9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (14) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5efa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (14)) { Exception (7); goto endlabel8277; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8277: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5efb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (14)) { Exception (7); goto endlabel8278; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8278: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5efc_21)(uae_u32 opcode)
{
{	if (cctrue (14)) { Exception (7); goto endlabel8279; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8279: ;
return;
}

#endif
/* Scc.B Dn */
void REGPARAM2 CPUFUNC(op_5fc0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{{{	int val = cctrue (15) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* DBcc.W Dn,#<data>.W */
void REGPARAM2 CPUFUNC(op_5fc8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = get_word_ce030_prefetch (2);
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (15)) {
	m68k_incpc ((uae_s32)offs + 2);
			m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
	regs.ce020memcycles += 4 * cpucycleunit;
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel8281;
			}
			return;
		}
	regs.ce020memcycles += 8 * cpucycleunit;
	} else {
	regs.ce020memcycles += 4 * cpucycleunit;
	}
	m68k_setpc (oldpc + 4);
}}}endlabel8281: ;
return;
}

/* Scc.B (An) */
void REGPARAM2 CPUFUNC(op_5fd0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (An)+ */
void REGPARAM2 CPUFUNC(op_5fd8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B -(An) */
void REGPARAM2 CPUFUNC(op_5fe0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
	m68k_areg (regs, srcreg) = srca;
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* Scc.B (d16,An) */
void REGPARAM2 CPUFUNC(op_5fe8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_5ff0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* Scc.B (xxx).W */
void REGPARAM2 CPUFUNC(op_5ff8_21)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* Scc.B (xxx).L */
void REGPARAM2 CPUFUNC(op_5ff9_21)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{{	int val = cctrue (15) ? 0xff : 0;
	put_byte_ce030 (srca, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* TRAPcc.L #<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5ffa_21)(uae_u32 opcode)
{
{{	uae_s16 dummy = get_word_ce030_prefetch (2);
	if (cctrue (15)) { Exception (7); goto endlabel8289; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
endlabel8289: ;
return;
}

#endif
/* TRAPcc.L #<data>.L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5ffb_21)(uae_u32 opcode)
{
{{	uae_s32 dummy = get_long_ce030_prefetch (2);
	if (cctrue (15)) { Exception (7); goto endlabel8290; }
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
endlabel8290: ;
return;
}

#endif
/* TRAPcc.L  */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_5ffc_21)(uae_u32 opcode)
{
{	if (cctrue (15)) { Exception (7); goto endlabel8291; }
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8291: ;
return;
}

#endif
/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6000_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8292;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8292: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6001_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8293;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8293: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_60ff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8294;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8294: ;
return;
}

/* BSR.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6100_21)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s16 src = get_word_ce030_prefetch (2);
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + s);
		goto endlabel8295;
	}
	m68k_do_bsr_ce030 (m68k_getpc () + 4, s);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8295: ;
return;
}

#endif

#ifdef PART_6
/* BSRQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6101_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{	uae_s32 s;
{	uae_u32 src = srcreg;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + s);
		goto endlabel8296;
	}
	m68k_do_bsr_ce030 (m68k_getpc () + 2, s);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8296: ;
return;
}

/* BSR.L #<data>.L */
void REGPARAM2 CPUFUNC(op_61ff_21)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s32 src = get_long_ce030_prefetch (2);
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + s);
		goto endlabel8297;
	}
	m68k_do_bsr_ce030 (m68k_getpc () + 6, s);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8297: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6200_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8298;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8298: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6201_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8299;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8299: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_62ff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8300;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8300: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6300_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8301;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8301: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6301_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8302;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8302: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_63ff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8303;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8303: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6400_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8304;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8304: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6401_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8305;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8305: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_64ff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8306;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8306: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6500_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8307;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8307: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6501_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8308;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8308: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_65ff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8309;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8309: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6600_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8310;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8310: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6601_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8311;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8311: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_66ff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8312;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8312: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6700_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8313;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8313: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6701_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8314;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8314: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_67ff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8315;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8315: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6800_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8316;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8316: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6801_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8317;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8317: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_68ff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8318;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8318: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6900_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8319;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8319: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6901_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8320;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8320: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_69ff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8321;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8321: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6a00_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8322;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8322: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6a01_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8323;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8323: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6aff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8324;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8324: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6b00_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8325;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8325: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6b01_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8326;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8326: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6bff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8327;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8327: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6c00_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8328;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8328: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6c01_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8329;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8329: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6cff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8330;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8330: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6d00_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8331;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8331: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6d01_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8332;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8332: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6dff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8333;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8333: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6e00_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8334;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8334: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6e01_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8335;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8335: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6eff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8336;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8336: ;
return;
}

/* Bcc.W #<data>.W */
void REGPARAM2 CPUFUNC(op_6f00_21)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_ce030_prefetch (2);
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8337;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (4);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8337: ;
return;
}

/* BccQ.B #<data> */
void REGPARAM2 CPUFUNC(op_6f01_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8338;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (2);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8338: ;
return;
}

/* Bcc.L #<data>.L */
void REGPARAM2 CPUFUNC(op_6fff_21)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_ce030_prefetch (2);
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel8339;
	}
	m68k_incpc ((uae_s32)src + 2);
	return;
didnt_jump:;
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8339: ;
return;
}

/* MOVEQ.L #<data>,Dn */
void REGPARAM2 CPUFUNC(op_7000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8018_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8020_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8038_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8039_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_803a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_803b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_803c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_8040_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8050_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8058_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8060_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8068_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8070_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8078_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8079_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_807a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_807b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_807c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_8080_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8090_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_8098_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_80a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_80a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_80b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_80b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_80b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_80ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_80bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_80bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_80c0_21)(uae_u32 opcode)
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
		goto endlabel8374;
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
}}}endlabel8374: ;
return;
}

/* DIVU.W (An),Dn */
void REGPARAM2 CPUFUNC(op_80d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel8375;
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
}}}}endlabel8375: ;
return;
}

/* DIVU.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_80d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel8376;
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
}}}}endlabel8376: ;
return;
}

/* DIVU.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_80e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel8377;
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
}}}}endlabel8377: ;
return;
}

/* DIVU.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_80e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel8378;
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
}}}}endlabel8378: ;
return;
}

/* DIVU.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel8379;
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
}}}}}endlabel8379: ;
return;
}

/* DIVU.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_80f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel8380;
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
}}}}endlabel8380: ;
return;
}

/* DIVU.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_80f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (6);
		Exception (5);
		goto endlabel8381;
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
}}}}endlabel8381: ;
return;
}

/* DIVU.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_80fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel8382;
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
}}}}endlabel8382: ;
return;
}

/* DIVU.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_80fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel8383;
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
}}}}}endlabel8383: ;
return;
}

/* DIVU.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_80fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		if (dst < 0) SET_NFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel8384;
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
}}}endlabel8384: ;
return;
}

/* SBCD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_8100_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8108_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_8110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_8120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_8128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_8130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_8138_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_8139_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* PACK.L Dn,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_8140_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uae_u16 val = m68k_dreg (regs, srcreg) + get_word_ce030_prefetch (2);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & 0xffffff00) | ((val >> 4) & 0xf0) | (val & 0xf);
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* PACK.L -(An),-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_8148_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uae_u16 val;
	m68k_areg (regs, srcreg) -= areg_byteinc[srcreg];
	val = (uae_u16)get_byte_ce030 (m68k_areg (regs, srcreg));
	m68k_areg (regs, srcreg) -= areg_byteinc[srcreg];
	val = (val | ((uae_u16)get_byte_ce030 (m68k_areg (regs, srcreg)) << 8)) + get_word_ce030_prefetch (2);
	m68k_areg (regs, dstreg) -= areg_byteinc[dstreg];
	put_byte_ce030 (m68k_areg (regs, dstreg),((val >> 4) & 0xf0) | (val & 0xf));
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* OR.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_8150_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8158_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_8160_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_8168_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_8170_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_8178_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_8179_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* UNPK.L Dn,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_8180_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uae_u16 val = m68k_dreg (regs, srcreg);
	val = (((val << 4) & 0xf00) | (val & 0xf)) + get_word_ce030_prefetch (2);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & 0xffff0000) | (val & 0xffff);
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* UNPK.L -(An),-(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_8188_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uae_u16 val;
	m68k_areg (regs, srcreg) -= areg_byteinc[srcreg];
	val = (uae_u16)get_byte_ce030 (m68k_areg (regs, srcreg));
	val = (((val << 4) & 0xf00) | (val & 0xf)) + get_word_ce030_prefetch (2);
	m68k_areg (regs, dstreg) -= 2 * areg_byteinc[dstreg];
	put_byte_ce030 (m68k_areg (regs, dstreg) + areg_byteinc[dstreg], val);
	put_byte_ce030 (m68k_areg (regs, dstreg), val >> 8);
}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

#endif
/* OR.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_8190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_8198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_81a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* OR.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_81a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_81b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* OR.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_81b8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* OR.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_81b9_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* DIVS.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_81c0_21)(uae_u32 opcode)
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
		goto endlabel8412;
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
}}}endlabel8412: ;
return;
}

/* DIVS.W (An),Dn */
void REGPARAM2 CPUFUNC(op_81d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel8413;
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
}}}}endlabel8413: ;
return;
}

/* DIVS.W (An)+,Dn */
void REGPARAM2 CPUFUNC(op_81d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel8414;
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
}}}}endlabel8414: ;
return;
}

/* DIVS.W -(An),Dn */
void REGPARAM2 CPUFUNC(op_81e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (2);
		Exception (5);
		goto endlabel8415;
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
}}}}endlabel8415: ;
return;
}

/* DIVS.W (d16,An),Dn */
void REGPARAM2 CPUFUNC(op_81e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel8416;
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
}}}}endlabel8416: ;
return;
}

/* DIVS.W (d8,An,Xn),Dn */
void REGPARAM2 CPUFUNC(op_81f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel8417;
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
}}}}}endlabel8417: ;
return;
}

/* DIVS.W (xxx).W,Dn */
void REGPARAM2 CPUFUNC(op_81f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel8418;
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
}}}}endlabel8418: ;
return;
}

/* DIVS.W (xxx).L,Dn */
void REGPARAM2 CPUFUNC(op_81f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (6);
		Exception (5);
		goto endlabel8419;
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
}}}}endlabel8419: ;
return;
}

/* DIVS.W (d16,PC),Dn */
void REGPARAM2 CPUFUNC(op_81fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel8420;
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
}}}}endlabel8420: ;
return;
}

/* DIVS.W (d8,PC,Xn),Dn */
void REGPARAM2 CPUFUNC(op_81fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (0);
		Exception (5);
		goto endlabel8421;
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
}}}}}endlabel8421: ;
return;
}

/* DIVS.W #<data>.W,Dn */
void REGPARAM2 CPUFUNC(op_81fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
		SET_VFLG (1);
		SET_ZFLG (1);
	m68k_incpc (4);
		Exception (5);
		goto endlabel8422;
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
}}}endlabel8422: ;
return;
}

/* SUB.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_9000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9018_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9020_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9038_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9039_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_903a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_903b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_903c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_9040_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9048_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9050_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9058_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9060_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9068_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9070_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9078_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9079_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_907a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_907b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_907c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_9080_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9088_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9090_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_9098_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_90a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_90a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_90b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_90b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_90b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_90ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_90bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_90bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_90c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_90c8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_90d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_90d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W -(An),An */
void REGPARAM2 CPUFUNC(op_90e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_90e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_90f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_90f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_90f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_90fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_90fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_90fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBX.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_9100_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9108_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_9110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_9120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_9128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_9130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_9138_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_9139_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBX.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_9140_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9148_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_9150_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9158_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_9160_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_9168_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_9170_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_9178_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_9179_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBX.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_9180_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_9188_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_9190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_9198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_91a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUB.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_91a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_91b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUB.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_91b8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUB.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_91b9_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBA.L Dn,An */
void REGPARAM2 CPUFUNC(op_91c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_91c8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_91d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_91d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L -(An),An */
void REGPARAM2 CPUFUNC(op_91e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* SUBA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_91e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_91f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_91f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_91f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* SUBA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_91fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* SUBA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_91fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* SUBA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_91fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMP.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_b000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b018_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b020_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b038_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b039_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b03a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b03b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b03c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_b040_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b048_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b050_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b058_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b060_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b068_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b070_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b078_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b079_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b07a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b07b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b07c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_b080_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b088_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b090_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b098_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_b0c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b0c8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b0d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b0fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_b100_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b108_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_b110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b138_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b139_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_b140_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b148_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_b150_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b158_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b160_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b168_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b170_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b178_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b179_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EOR.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_b180_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b188_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
void REGPARAM2 CPUFUNC(op_b190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_b198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_b1a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* EOR.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_b1a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_b1b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* EOR.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_b1b8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* EOR.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_b1b9_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* CMPA.L Dn,An */
void REGPARAM2 CPUFUNC(op_b1c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b1c8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_b1d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b1d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b1e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b1e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b1f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b1f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b1f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b1fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b1fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_b1fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_c000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c018_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c020_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c038_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c039_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c03a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c03b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c03c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_c040_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c050_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c058_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c060_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c068_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c070_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c078_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c079_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c07a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c07b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c07c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_c080_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c090_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c098_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_c0c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c0d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c0fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_c100_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c108_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_c110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c138_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c139_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EXG.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_c140_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c148_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c150_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c158_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) += 2;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c160_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c168_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c170_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c178_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c179_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* EXG.L Dn,An */
void REGPARAM2 CPUFUNC(op_c188_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_c198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) += 4;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_c1a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* AND.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_c1a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_c1b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* AND.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_c1b8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* AND.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_c1b9_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long_ce030 (dsta, src);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* MULS.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_c1c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c1d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c1d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c1e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c1e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c1f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c1f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c1f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c1fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c1fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_c1fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_d000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d018_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d020_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d038_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d039_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d03a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d03b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d03c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_d040_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d048_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d050_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d058_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d060_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d068_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d070_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d078_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d079_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d07a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d07b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d07c_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_d080_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d088_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d090_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d098_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d0a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d0a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d0b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d0b8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d0b9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d0ba_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d0bb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
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
void REGPARAM2 CPUFUNC(op_d0bc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_d0c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d0c8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d0d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W (An)+,An */
void REGPARAM2 CPUFUNC(op_d0d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W -(An),An */
void REGPARAM2 CPUFUNC(op_d0e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.W (d16,An),An */
void REGPARAM2 CPUFUNC(op_d0e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.W (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_d0f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.W (xxx).W,An */
void REGPARAM2 CPUFUNC(op_d0f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.W (xxx).L,An */
void REGPARAM2 CPUFUNC(op_d0f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDA.W (d16,PC),An */
void REGPARAM2 CPUFUNC(op_d0fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.W (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_d0fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s16 src = get_word_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.W #<data>.W,An */
void REGPARAM2 CPUFUNC(op_d0fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_ce030_prefetch (2);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDX.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_d100_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d108_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,(An) */
void REGPARAM2 CPUFUNC(op_d110_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d118_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d120_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.B Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d128_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d130_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.B Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d138_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.B Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d139_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s8 dst = get_byte_ce030 (dsta);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDX.W Dn,Dn */
void REGPARAM2 CPUFUNC(op_d140_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d148_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
{	uae_s16 src = get_word_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,(An) */
void REGPARAM2 CPUFUNC(op_d150_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d158_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d160_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
{	uae_s16 dst = get_word_ce030 (dsta);
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
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.W Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d168_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d170_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.W Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d178_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.W Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d179_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s16 dst = get_word_ce030 (dsta);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDX.L Dn,Dn */
void REGPARAM2 CPUFUNC(op_d180_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d188_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,(An) */
void REGPARAM2 CPUFUNC(op_d190_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,(An)+ */
void REGPARAM2 CPUFUNC(op_d198_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,-(An) */
void REGPARAM2 CPUFUNC(op_d1a0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
{	uae_s32 dst = get_long_ce030 (dsta);
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
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADD.L Dn,(d16,An) */
void REGPARAM2 CPUFUNC(op_d1a8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.L Dn,(d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_d1b0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	m68k_incpc (2);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADD.L Dn,(xxx).W */
void REGPARAM2 CPUFUNC(op_d1b8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADD.L Dn,(xxx).L */
void REGPARAM2 CPUFUNC(op_d1b9_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_ce030_prefetch (2);
{	uae_s32 dst = get_long_ce030 (dsta);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long_ce030 (dsta, newv);
}}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDA.L Dn,An */
void REGPARAM2 CPUFUNC(op_d1c0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d1c8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_d1d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L (An)+,An */
void REGPARAM2 CPUFUNC(op_d1d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L -(An),An */
void REGPARAM2 CPUFUNC(op_d1e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
{	uae_s32 src = get_long_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ADDA.L (d16,An),An */
void REGPARAM2 CPUFUNC(op_d1e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.L (d8,An,Xn),An */
void REGPARAM2 CPUFUNC(op_d1f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.L (xxx).W,An */
void REGPARAM2 CPUFUNC(op_d1f8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.L (xxx).L,An */
void REGPARAM2 CPUFUNC(op_d1f9_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ADDA.L (d16,PC),An */
void REGPARAM2 CPUFUNC(op_d1fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ADDA.L (d8,PC,Xn),An */
void REGPARAM2 CPUFUNC(op_d1fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	m68k_incpc (2);
{	tmppc = m68k_getpc ();
	srca = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
{	uae_s32 src = get_long_ce030 (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ADDA.L #<data>.L,An */
void REGPARAM2 CPUFUNC(op_d1fc_21)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_ce030_prefetch (2);
{	uae_s32 dst = m68k_areg (regs, dstreg);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ASRQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e000_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e008_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e010_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e018_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e020_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e028_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e030_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e038_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e040_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e048_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e050_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e058_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e060_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e068_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e070_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e078_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e080_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e088_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e090_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e098_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e0a0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e0a8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e0b0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e0b8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e0d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e0d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e0e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e0e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e0f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ASRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e0f8_21)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e0f9_21)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ASLQ.B #<data>,Dn */
void REGPARAM2 CPUFUNC(op_e100_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e108_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e110_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e118_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e120_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e128_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e130_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e138_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e140_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e148_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e150_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e158_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e160_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e168_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e170_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e178_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e180_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e188_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e190_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e198_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e1a0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e1a8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e1b0_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e1b8_21)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_e1d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e1d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e1e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ASLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e1e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e1f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ASLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e1f8_21)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ASLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e1f9_21)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* LSRW.W (An) */
void REGPARAM2 CPUFUNC(op_e2d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e2d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e2e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce030 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e2e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e2f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LSRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e2f8_21)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e2f9_21)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* LSLW.W (An) */
void REGPARAM2 CPUFUNC(op_e3d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e3d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e3e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce030 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* LSLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e3e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e3f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* LSLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e3f8_21)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* LSLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e3f9_21)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ROXRW.W (An) */
void REGPARAM2 CPUFUNC(op_e4d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e4d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRW.W -(An) */
void REGPARAM2 CPUFUNC(op_e4e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXRW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e4e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXRW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e4f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ROXRW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e4f8_21)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXRW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e4f9_21)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ROXLW.W (An) */
void REGPARAM2 CPUFUNC(op_e5d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e5d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e5e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce030 (dataa);
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
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROXLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e5e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e5f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ROXLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e5f8_21)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROXLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e5f9_21)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* RORW.W (An) */
void REGPARAM2 CPUFUNC(op_e6d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e6d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORW.W -(An) */
void REGPARAM2 CPUFUNC(op_e6e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce030 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* RORW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e6e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* RORW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e6f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce030 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* RORW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e6f8_21)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* RORW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e6f9_21)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* ROLW.W (An) */
void REGPARAM2 CPUFUNC(op_e7d0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLW.W (An)+ */
void REGPARAM2 CPUFUNC(op_e7d8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
{	uae_s16 data = get_word_ce030 (dataa);
	m68k_areg (regs, srcreg) += 2;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLW.W -(An) */
void REGPARAM2 CPUFUNC(op_e7e0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
{	uae_s16 data = get_word_ce030 (dataa);
	m68k_areg (regs, srcreg) = dataa;
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* ROLW.W (d16,An) */
void REGPARAM2 CPUFUNC(op_e7e8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROLW.W (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_e7f0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	m68k_incpc (2);
{	dataa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce030 (dataa, val);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* ROLW.W (xxx).W */
void REGPARAM2 CPUFUNC(op_e7f8_21)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* ROLW.W (xxx).L */
void REGPARAM2 CPUFUNC(op_e7f9_21)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_ce030_prefetch (2);
{	uae_s16 data = get_word_ce030 (dataa);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word_ce030 (dataa, val);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

/* BFTST.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_e8c0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_e8d0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_e8e8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_e8f0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_e8f8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_e8f9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_e8fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_e8fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_e9c0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_e9d0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_e9e8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_e9f0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_e9f8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_e9f9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_e9fa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_e9fb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_eac0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_ead0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_eae8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_eaf0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_eaf8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_eaf9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_ebc0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_ebd0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_ebe8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_ebf0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_ebf8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_ebf9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_ebfa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_ebfb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_ecc0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_ecd0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_ece8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_ecf0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_ecf8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_ecf9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_edc0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_edd0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_ede8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_edf0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_edf8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_edf9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_edfa_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_edfb_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr tmppc;
	uaecptr dsta;
	m68k_incpc (4);
{	tmppc = m68k_getpc ();
	dsta = x_get_disp_ea_020 (tmppc, next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_eec0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_eed0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_eee8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_eef0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_eef8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_eef9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_efc0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_efd0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_efe8_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_eff0_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta;
	m68k_incpc (4);
{	dsta = x_get_disp_ea_020 (m68k_areg (regs, dstreg), next_iword_030ce ());
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
void REGPARAM2 CPUFUNC(op_eff8_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_eff9_21)(uae_u32 opcode)
{
{{	uae_s16 extra = get_word_ce030_prefetch (2);
{	uaecptr dsta = get_long_ce030_prefetch (4);
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
/* MMUOP030.L (An),#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f010_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8922; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa = m68k_areg (regs, srcreg);
	mmu_op30 (pc, opcode, extra, extraa);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8922: ;
return;
}

#endif
/* MMUOP030.L (d16,An),#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f028_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8923; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	m68k_incpc (2);
	mmu_op30 (pc, opcode, extra, extraa);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8923: ;
return;
}

#endif
/* MMUOP030.L (d8,An,Xn),#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f030_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8924; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa;
{	extraa = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
	mmu_op30 (pc, opcode, extra, extraa);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8924: ;
return;
}

#endif
/* MMUOP030.L (xxx).W,#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f038_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8925; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa = (uae_s32)(uae_s16)get_word_ce030_prefetch (0);
	m68k_incpc (2);
	mmu_op30 (pc, opcode, extra, extraa);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8925: ;
return;
}

#endif
/* MMUOP030.L (xxx).L,#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f039_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8926; }
{	uaecptr pc = m68k_getpc ();
	uae_u16 extra = x_get_word (pc + 2);
	m68k_incpc (4);
{	uaecptr extraa = get_long_ce030_prefetch (0);
	m68k_incpc (4);
	mmu_op30 (pc, opcode, extra, extraa);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8926: ;
return;
}

#endif
/* FPP.L #<data>.W,Dn */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f200_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f208_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f210_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f218_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f220_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f228_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f230_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f238_21)(uae_u32 opcode)
{
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f239_21)(uae_u32 opcode)
{
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f23a_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f23b_21)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f23c_21)(uae_u32 opcode)
{
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f240_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f248_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f250_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f258_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f260_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f268_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f270_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f278_21)(uae_u32 opcode)
{
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f279_21)(uae_u32 opcode)
{
{
#ifdef FPUEMU
{	uae_s16 extra = get_word_ce030_prefetch (2);
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
void REGPARAM2 CPUFUNC(op_f27a_21)(uae_u32 opcode)
{
{
#ifdef FPUEMU
	uaecptr oldpc = m68k_getpc ();
	uae_u16 extra = get_word_ce030_prefetch (2);
{	uae_s16 dummy = get_word_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_f27b_21)(uae_u32 opcode)
{
{
#ifdef FPUEMU
	uaecptr oldpc = m68k_getpc ();
	uae_u16 extra = get_word_ce030_prefetch (2);
{	uae_s32 dummy = get_long_ce030_prefetch (4);
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
void REGPARAM2 CPUFUNC(op_f27c_21)(uae_u32 opcode)
{
{
#ifdef FPUEMU
	uaecptr oldpc = m68k_getpc ();
	uae_u16 extra = get_word_ce030_prefetch (2);
	m68k_incpc (4);
	fpuop_trapcc (opcode, oldpc, extra);

#endif
}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

#endif
/* FBccQ.L #<data>,#<data>.W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f280_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 63);
{
#ifdef FPUEMU
	m68k_incpc (2);
{	uaecptr pc = m68k_getpc ();
{	uae_s16 extra = get_word_ce030_prefetch (0);
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
void REGPARAM2 CPUFUNC(op_f2c0_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 63);
{
#ifdef FPUEMU
	m68k_incpc (2);
{	uaecptr pc = m68k_getpc ();
{	uae_s32 extra = get_long_ce030_prefetch (0);
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
void REGPARAM2 CPUFUNC(op_f310_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8953; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8953: ;
return;
}

#endif
/* FSAVE.L -(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f320_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8954; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8954: ;
return;
}

#endif
/* FSAVE.L (d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f328_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8955; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8955: ;
return;
}

#endif
/* FSAVE.L (d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f330_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8956; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8956: ;
return;
}

#endif
/* FSAVE.L (xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f338_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8957; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8957: ;
return;
}

#endif
/* FSAVE.L (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f339_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8958; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_save (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8958: ;
return;
}

#endif
/* FRESTORE.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f350_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8959; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8959: ;
return;
}

#endif
/* FRESTORE.L (An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f358_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8960; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8960: ;
return;
}

#endif
/* FRESTORE.L (d16,An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f368_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8961; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8961: ;
return;
}

#endif
/* FRESTORE.L (d8,An,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f370_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8962; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8962: ;
return;
}

#endif
/* FRESTORE.L (xxx).W */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f378_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8963; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8963: ;
return;
}

#endif
/* FRESTORE.L (xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f379_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8964; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8964: ;
return;
}

#endif
/* FRESTORE.L (d16,PC) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f37a_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8965; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8965: ;
return;
}

#endif
/* FRESTORE.L (d8,PC,Xn) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f37b_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel8966; }
{
#ifdef FPUEMU
	m68k_incpc (2);
	fpuop_restore (opcode);

#endif
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8966: ;
return;
}

#endif
/* CINVLQ.L #<data>,An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f408_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel8967; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8967: ;
return;
}

#endif
/* CINVPQ.L #<data>,An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f410_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel8968; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8968: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f418_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8969; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8969: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f419_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8970; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8970: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41a_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8971; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8971: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41b_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8972; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8972: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41c_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8973; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8973: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41d_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8974; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8974: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41e_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8975; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8975: ;
return;
}

#endif
/* CINVAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f41f_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8976; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8976: ;
return;
}

#endif
/* CPUSHLQ.L #<data>,An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f428_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel8977; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8977: ;
return;
}

#endif
/* CPUSHPQ.L #<data>,An */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f430_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
	uae_u32 dstreg = opcode & 7;
{if (!regs.s) { Exception (8); goto endlabel8978; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8978: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f438_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8979; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8979: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f439_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8980; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8980: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43a_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8981; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8981: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43b_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8982; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8982: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43c_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8983; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8983: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43d_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8984; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8984: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43e_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8985; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8985: ;
return;
}

#endif
/* CPUSHAQ.L #<data> */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f43f_21)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 6) & 3);
{if (!regs.s) { Exception (8); goto endlabel8986; }
{	if (opcode & 0x80)
		flush_icache(m68k_areg (regs, opcode & 3), (opcode >> 6) & 3);
}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
endlabel8986: ;
return;
}

#endif
/* PFLUSHN.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f500_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8987; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8987: ;
return;
}

#endif
/* PFLUSH.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f508_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8988; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8988: ;
return;
}

#endif
/* PFLUSHAN.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f510_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8989; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8989: ;
return;
}

#endif
/* PFLUSHA.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f518_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8990; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8990: ;
return;
}

#endif
/* PTESTR.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f548_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8991; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8991: ;
return;
}

#endif
/* PTESTW.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f568_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8992; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8992: ;
return;
}

#endif
/* PLPAR.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f588_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8993; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8993: ;
return;
}

#endif
/* PLPAW.L (An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f5c8_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel8994; }
{	m68k_incpc (2);
	mmu_op (opcode, 0);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel8994: ;
return;
}

#endif
/* MOVE16.L (An)+,(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f600_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	uae_u32 v1, v2, v3, v4;
{	uaecptr memsa = m68k_areg (regs, srcreg);
{	uaecptr memda = get_long_ce030_prefetch (2);
	memsa &= ~15;
	memda &= ~15;
	v1 = get_long_ce030 (memsa);
	v2 = get_long_ce030 (memsa + 4);
	v3 = get_long_ce030 (memsa + 8);
	v4 = get_long_ce030 (memsa + 12);
	put_long_ce030 (memda , v1);
	put_long_ce030 (memda + 4, v2);
	put_long_ce030 (memda + 8, v3);
	put_long_ce030 (memda + 12, v4);
	m68k_areg (regs, srcreg) += 16;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* MOVE16.L (xxx).L,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f608_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u32 v1, v2, v3, v4;
{	uaecptr memsa = get_long_ce030_prefetch (2);
{	uaecptr memda = m68k_areg (regs, dstreg);
	memsa &= ~15;
	memda &= ~15;
	v1 = get_long_ce030 (memsa);
	v2 = get_long_ce030 (memsa + 4);
	v3 = get_long_ce030 (memsa + 8);
	v4 = get_long_ce030 (memsa + 12);
	put_long_ce030 (memda , v1);
	put_long_ce030 (memda + 4, v2);
	put_long_ce030 (memda + 8, v3);
	put_long_ce030 (memda + 12, v4);
	m68k_areg (regs, dstreg) += 16;
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* MOVE16.L (An),(xxx).L */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f610_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{	uae_u32 v1, v2, v3, v4;
{	uaecptr memsa = m68k_areg (regs, srcreg);
{	uaecptr memda = get_long_ce030_prefetch (2);
	memsa &= ~15;
	memda &= ~15;
	v1 = get_long_ce030 (memsa);
	v2 = get_long_ce030 (memsa + 4);
	v3 = get_long_ce030 (memsa + 8);
	v4 = get_long_ce030 (memsa + 12);
	put_long_ce030 (memda , v1);
	put_long_ce030 (memda + 4, v2);
	put_long_ce030 (memda + 8, v3);
	put_long_ce030 (memda + 12, v4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* MOVE16.L (xxx).L,(An) */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f618_21)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u32 v1, v2, v3, v4;
{	uaecptr memsa = get_long_ce030_prefetch (2);
{	uaecptr memda = m68k_areg (regs, dstreg);
	memsa &= ~15;
	memda &= ~15;
	v1 = get_long_ce030 (memsa);
	v2 = get_long_ce030 (memsa + 4);
	v3 = get_long_ce030 (memsa + 8);
	v4 = get_long_ce030 (memsa + 12);
	put_long_ce030 (memda , v1);
	put_long_ce030 (memda + 4, v2);
	put_long_ce030 (memda + 8, v3);
	put_long_ce030 (memda + 12, v4);
}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif
/* MOVE16.L (An)+,(An)+ */
#ifndef CPUEMU_68000_ONLY
void REGPARAM2 CPUFUNC(op_f620_21)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = 0;
{	uae_u32 v1, v2, v3, v4;
	uaecptr mems = m68k_areg (regs, srcreg) & ~15, memd;
	dstreg = (get_word_ce030_prefetch (2) >> 12) & 7;
	memd = m68k_areg (regs, dstreg) & ~15;
	v1 = get_long_ce030 (mems);
	v2 = get_long_ce030 (mems + 4);
	v3 = get_long_ce030 (mems + 8);
	v4 = get_long_ce030 (mems + 12);
	put_long_ce030 (memd , v1);
	put_long_ce030 (memd + 4, v2);
	put_long_ce030 (memd + 8, v3);
	put_long_ce030 (memd + 12, v4);
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
void REGPARAM2 CPUFUNC(op_f800_21)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel9000; }
{	uae_u16 sw = x_get_iword (2);
	uae_u16 sr;
	if (sw != (0x100|0x80|0x40)) { Exception (4); goto endlabel9000; }
	sr = x_get_iword (4);
	if (!(sr & 0x8000)) { Exception (8); goto endlabel9000; }
	regs.sr = sr;
	MakeFromSR ();
	m68k_setstopped();
	m68k_incpc (6);
}}	regs.ce020memcycles += 2 * cpucycleunit;
endlabel9000: ;
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
void REGPARAM2 CPUFUNC(op_4800_23)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_4810_23)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B (An)+ */
void REGPARAM2 CPUFUNC(op_4818_23)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B -(An) */
void REGPARAM2 CPUFUNC(op_4820_23)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

/* NBCD.B (d16,An) */
void REGPARAM2 CPUFUNC(op_4828_23)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NBCD.B (d8,An,Xn) */
void REGPARAM2 CPUFUNC(op_4830_23)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	m68k_incpc (2);
{	srca = x_get_disp_ea_020 (m68k_areg (regs, srcreg), next_iword_030ce ());
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
return;
}

/* NBCD.B (xxx).W */
void REGPARAM2 CPUFUNC(op_4838_23)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (4);
return;
}

/* NBCD.B (xxx).L */
void REGPARAM2 CPUFUNC(op_4839_23)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_ce030_prefetch (2);
{	uae_s8 src = get_byte_ce030 (srca);
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
	put_byte_ce030 (srca, newv);
}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (6);
return;
}

#endif

#ifdef PART_5
#endif

#ifdef PART_6
/* SBCD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_8100_23)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_8108_23)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif

#ifdef PART_7
/* ABCD.B Dn,Dn */
void REGPARAM2 CPUFUNC(op_c100_23)(uae_u32 opcode)
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
void REGPARAM2 CPUFUNC(op_c108_23)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte_ce030 (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte_ce030 (dsta);
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
	put_byte_ce030 (dsta, newv);
}}}}}}	regs.ce020memcycles += 2 * cpucycleunit;
	m68k_incpc (2);
return;
}

#endif

#ifdef PART_8
#endif

