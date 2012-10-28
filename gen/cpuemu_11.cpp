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
uae_u32 REGPARAM2 CPUFUNC(op_0000_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* OR.B #<data>.B,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0010_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.B #<data>.B,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0018_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.B #<data>.B,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0020_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 22 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.B #<data>.B,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0028_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* OR.B #<data>.B,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0030_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
{	uae_s8 dst = get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 26 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* OR.B #<data>.B,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0038_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* OR.B #<data>.B,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0039_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_byte (dsta, src);
}}}}	m68k_incpc (8);
return 28 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* ORSR.B #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_003c_11)(uae_u32 opcode)
{
{	MakeSR ();
{	uae_s16 src = get_word_prefetch (4);
	get_wordi (6);
	src &= 0xFF;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	regs.sr |= src;
	MakeFromSR ();
}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0040_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* OR.W #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0050_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2187;
	}
{{	uae_s16 dst = get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel2187: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.W #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0058_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2188;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel2188: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.W #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0060_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2189;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel2189: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.W #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0068_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2190;
	}
{{	uae_s16 dst = get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2190: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* OR.W #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0070_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2191;
	}
{{	uae_s16 dst = get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2191: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* OR.W #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0078_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2192;
	}
{{	uae_s16 dst = get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2192: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* OR.W #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0079_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2193;
	}
{{	uae_s16 dst = get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_word (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2193: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* ORSR.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_007c_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2194; }
{	MakeSR ();
{	uae_s16 src = get_word_prefetch (4);
	get_wordi (6);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	regs.sr |= src;
	MakeFromSR ();
}}}	m68k_incpc (4);
endlabel2194: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0080_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (src);
}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.L #<data>.L,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0090_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2196;
	}
{{	uae_s32 dst = get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2196: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* OR.L #<data>.L,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0098_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2197;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2197: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* OR.L #<data>.L,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_00a0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2198;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2198: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* OR.L #<data>.L,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_00a8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2199;
	}
{{	uae_s32 dst = get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_long (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2199: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* OR.L #<data>.L,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_00b0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (8));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2200;
	}
{{	uae_s32 dst = get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_long (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2200: ;
return 34 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* OR.L #<data>.L,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_00b8_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2201;
	}
{{	uae_s32 dst = get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_long (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2201: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* OR.L #<data>.L,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_00b9_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = get_long_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2202;
	}
{{	uae_s32 dst = get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (12);
	put_long (dsta, src);
}}}}}	m68k_incpc (10);
endlabel2202: ;
return 36 * CYCLE_UNIT / 2;
} /* 36 (7/2) */

/* BTST.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MVPMR.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0108_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_u16 val = (get_byte (memp) << 8) + get_byte (memp + 2);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((val) & 0xffff);
}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BTST.B Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* BTST.B Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	get_word_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* BTST.B Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	get_word_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (2);
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* BTST.B Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* BTST.B Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* BTST.B Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0138_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* BTST.B Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0139_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BTST.B Dn,(d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_013a_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* BTST.B Dn,(d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_013b_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc () + 2;
	dsta = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* BTST.B Dn,#<data>.B */
uae_u32 REGPARAM2 CPUFUNC(op_013c_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = (uae_u8)get_word_prefetch (4);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* BCHG.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0140_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	src &= 31;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MVPMR.L (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0148_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{	uaecptr memp = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_u32 val = (get_byte (memp) << 24) + (get_byte (memp + 2) << 16)
              + (get_byte (memp + 4) << 8) + get_byte (memp + 6);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (val);
}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* BCHG.B Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (4);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* BCHG.B Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	get_word_prefetch (4);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* BCHG.B Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	get_word_prefetch (4);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* BCHG.B Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCHG.B Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCHG.B Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0178_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCHG.B Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0179_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCHG.B Dn,(d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_017a_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCHG.B Dn,(d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_017b_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc () + 2;
	dsta = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCLR.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0180_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MVPRM.W Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0188_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	uaecptr memp = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	put_byte (memp, src >> 8); put_byte (memp + 2, src);
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* BCLR.B Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* BCLR.B Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	get_word_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* BCLR.B Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_01a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	get_word_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* BCLR.B Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_01a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCLR.B Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_01b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCLR.B Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_01b8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCLR.B Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_01b9_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCLR.B Dn,(d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_01ba_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCLR.B Dn,(d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_01bb_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc () + 2;
	dsta = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BSET.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_01c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MVPRM.L Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_01c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	uaecptr memp = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	put_byte (memp, src >> 24); put_byte (memp + 2, src >> 16);
	put_byte (memp + 4, src >> 8); put_byte (memp + 6, src);
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 24 (2/4) */

/* BSET.B Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_01d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* BSET.B Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_01d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	get_word_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* BSET.B Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_01e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	get_word_prefetch (4);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* BSET.B Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_01e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BSET.B Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_01f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BSET.B Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_01f8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BSET.B Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_01f9_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BSET.B Dn,(d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_01fa_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 2;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_getpc () + 2;
	dsta += (uae_s32)(uae_s16)get_word_prefetch (4);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BSET.B Dn,(d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_01fb_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = 3;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc () + 2;
	dsta = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0200_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* AND.B #<data>.B,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0210_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.B #<data>.B,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0218_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.B #<data>.B,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0220_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 22 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.B #<data>.B,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0228_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* AND.B #<data>.B,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0230_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
{	uae_s8 dst = get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 26 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* AND.B #<data>.B,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0238_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* AND.B #<data>.B,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0239_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_byte (dsta, src);
}}}}	m68k_incpc (8);
return 28 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* ANDSR.B #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_023c_11)(uae_u32 opcode)
{
{	MakeSR ();
{	uae_s16 src = get_word_prefetch (4);
	get_wordi (6);
	src |= 0xFF00;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	regs.sr &= src;
	MakeFromSR ();
}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0240_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* AND.W #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0250_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2258;
	}
{{	uae_s16 dst = get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel2258: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.W #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0258_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2259;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel2259: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.W #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0260_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2260;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel2260: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.W #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0268_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2261;
	}
{{	uae_s16 dst = get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2261: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* AND.W #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0270_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2262;
	}
{{	uae_s16 dst = get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2262: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* AND.W #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0278_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2263;
	}
{{	uae_s16 dst = get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2263: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* AND.W #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0279_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2264;
	}
{{	uae_s16 dst = get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_word (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2264: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* ANDSR.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_027c_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2265; }
{	MakeSR ();
{	uae_s16 src = get_word_prefetch (4);
	get_wordi (6);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	regs.sr &= src;
	MakeFromSR ();
}}}	m68k_incpc (4);
endlabel2265: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0280_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (src);
}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.L #<data>.L,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0290_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2267;
	}
{{	uae_s32 dst = get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2267: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* AND.L #<data>.L,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0298_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2268;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2268: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* AND.L #<data>.L,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_02a0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2269;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2269: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* AND.L #<data>.L,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_02a8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2270;
	}
{{	uae_s32 dst = get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_long (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2270: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* AND.L #<data>.L,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_02b0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (8));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2271;
	}
{{	uae_s32 dst = get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_long (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2271: ;
return 34 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* AND.L #<data>.L,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_02b8_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2272;
	}
{{	uae_s32 dst = get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_long (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2272: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* AND.L #<data>.L,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_02b9_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = get_long_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2273;
	}
{{	uae_s32 dst = get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (12);
	put_long (dsta, src);
}}}}}	m68k_incpc (10);
endlabel2273: ;
return 36 * CYCLE_UNIT / 2;
} /* 36 (7/2) */

/* SUB.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0400_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUB.B #<data>.B,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0410_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.B #<data>.B,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0418_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.B #<data>.B,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0420_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 22 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.B #<data>.B,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0428_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (6);
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUB.B #<data>.B,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0430_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (6);
return 26 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUB.B #<data>.B,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0438_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (6);
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUB.B #<data>.B,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0439_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (8);
return 28 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* SUB.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0440_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUB.W #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0450_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2283;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2283: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.W #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0458_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2284;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2284: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.W #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0460_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2285;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2285: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.W #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0468_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2286;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2286: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUB.W #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0470_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2287;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2287: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUB.W #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0478_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2288;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2288: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUB.W #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0479_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2289;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (8);
endlabel2289: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* SUB.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0480_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.L #<data>.L,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0490_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2291;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2291: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* SUB.L #<data>.L,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0498_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2292;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2292: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* SUB.L #<data>.L,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_04a0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2293;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2293: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* SUB.L #<data>.L,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_04a8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2294;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (8);
endlabel2294: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* SUB.L #<data>.L,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_04b0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (8));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2295;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (8);
endlabel2295: ;
return 34 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* SUB.L #<data>.L,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_04b8_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2296;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (8);
endlabel2296: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* SUB.L #<data>.L,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_04b9_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = get_long_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2297;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (12);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (10);
endlabel2297: ;
return 36 * CYCLE_UNIT / 2;
} /* 36 (7/2) */

/* ADD.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0600_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADD.B #<data>.B,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0610_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.B #<data>.B,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0618_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.B #<data>.B,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0620_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 22 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.B #<data>.B,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0628_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (6);
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ADD.B #<data>.B,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0630_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (6);
return 26 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ADD.B #<data>.B,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0638_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (6);
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ADD.B #<data>.B,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0639_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (8);
return 28 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* ADD.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0640_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADD.W #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0650_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2307;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2307: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.W #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0658_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2308;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2308: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.W #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0660_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2309;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2309: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.W #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0668_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2310;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2310: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ADD.W #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0670_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2311;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2311: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ADD.W #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0678_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2312;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2312: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ADD.W #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0679_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2313;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (8);
endlabel2313: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* ADD.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0680_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.L #<data>.L,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0690_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2315;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2315: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* ADD.L #<data>.L,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0698_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2316;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2316: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* ADD.L #<data>.L,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_06a0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2317;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2317: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* ADD.L #<data>.L,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_06a8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2318;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (8);
endlabel2318: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* ADD.L #<data>.L,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_06b0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (8));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2319;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (8);
endlabel2319: ;
return 34 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* ADD.L #<data>.L,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_06b8_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2320;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (8);
endlabel2320: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* ADD.L #<data>.L,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_06b9_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = get_long_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2321;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (12);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (10);
endlabel2321: ;
return 36 * CYCLE_UNIT / 2;
} /* 36 (7/2) */

/* BTST.L #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0800_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* BTST.B #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0810_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* BTST.B #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0818_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* BTST.B #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0820_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* BTST.B #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0828_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BTST.B #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0830_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BTST.B #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0838_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BTST.B #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0839_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (10);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (8);
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* BTST.B #<data>.W,(d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_083a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BTST.B #<data>.W,(d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_083b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc () + 4;
	dsta = get_disp_ea_000 (tmppc, get_word_prefetch (6));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}}	m68k_incpc (6);
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BTST.B #<data>.W,#<data>.B */
uae_u32 REGPARAM2 CPUFUNC(op_083c_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s8 dst = (uae_u8)get_word_prefetch (6);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* BCHG.L #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0840_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	src &= 31;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* BCHG.B #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0850_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCHG.B #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0858_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	get_word_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCHG.B #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0860_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	get_word_prefetch (6);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCHG.B #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0868_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCHG.B #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0870_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCHG.B #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0878_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCHG.B #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0879_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (10);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (8);
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* BCHG.B #<data>.W,(d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_087a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCHG.B #<data>.W,(d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_087b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc () + 4;
	dsta = get_disp_ea_000 (tmppc, get_word_prefetch (6));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	dst ^= (1 << src);
	SET_ZFLG (((uae_u32)dst & (1 << src)) >> src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCLR.L #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0880_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* BCLR.B #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0890_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCLR.B #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0898_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCLR.B #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_08a0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BCLR.B #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_08a8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCLR.B #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_08b0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCLR.B #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_08b8_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCLR.B #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_08b9_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (10);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (8);
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* BCLR.B #<data>.W,(d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_08ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BCLR.B #<data>.W,(d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_08bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc () + 4;
	dsta = get_disp_ea_000 (tmppc, get_word_prefetch (6));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst &= ~(1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BSET.L #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_08c0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	src &= 31;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	m68k_dreg (regs, dstreg) = (dst);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* BSET.B #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_08d0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BSET.B #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_08d8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BSET.B #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_08e0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	get_word_prefetch (6);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* BSET.B #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_08e8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BSET.B #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_08f0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BSET.B #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_08f8_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BSET.B #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_08f9_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (10);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (8);
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* BSET.B #<data>.W,(d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_08fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_getpc () + 4;
	dsta += (uae_s32)(uae_s16)get_word_prefetch (6);
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* BSET.B #<data>.W,(d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_08fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr tmppc;
	uaecptr dsta;
	tmppc = m68k_getpc () + 4;
	dsta = get_disp_ea_000 (tmppc, get_word_prefetch (6));
	regs.ir = regs.irc;
	regs.irc = 0;
{	uae_s8 dst = get_byte (dsta);
	get_word_prefetch (8);
	src &= 7;
	SET_ZFLG (1 ^ ((dst >> src) & 1));
	dst |= (1 << src);
	put_byte (dsta, dst);
}}}}	m68k_incpc (6);
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EOR.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0a00_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* EOR.B #<data>.B,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0a10_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.B #<data>.B,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0a18_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.B #<data>.B,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0a20_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 22 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.B #<data>.B,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0a28_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EOR.B #<data>.B,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0a30_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
{	uae_s8 dst = get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 26 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EOR.B #<data>.B,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0a38_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EOR.B #<data>.B,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0a39_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_byte (dsta, src);
}}}}	m68k_incpc (8);
return 28 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* EORSR.B #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_0a3c_11)(uae_u32 opcode)
{
{	MakeSR ();
{	uae_s16 src = get_word_prefetch (4);
	get_wordi (6);
	src &= 0xFF;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	regs.sr ^= src;
	MakeFromSR ();
}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* EOR.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0a40_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* EOR.W #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0a50_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2373;
	}
{{	uae_s16 dst = get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel2373: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.W #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0a58_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2374;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel2374: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.W #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0a60_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2375;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel2375: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.W #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0a68_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2376;
	}
{{	uae_s16 dst = get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2376: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EOR.W #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0a70_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2377;
	}
{{	uae_s16 dst = get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2377: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EOR.W #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0a78_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2378;
	}
{{	uae_s16 dst = get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2378: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EOR.W #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0a79_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2379;
	}
{{	uae_s16 dst = get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_word (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2379: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* EORSR.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_0a7c_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2380; }
{	MakeSR ();
{	uae_s16 src = get_word_prefetch (4);
	get_wordi (6);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	regs.sr ^= src;
	MakeFromSR ();
}}}	m68k_incpc (4);
endlabel2380: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* EOR.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0a80_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (src);
}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

#endif

#ifdef PART_2
/* EOR.L #<data>.L,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0a90_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2382;
	}
{{	uae_s32 dst = get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2382: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* EOR.L #<data>.L,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0a98_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2383;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2383: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* EOR.L #<data>.L,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0aa0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2384;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel2384: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* EOR.L #<data>.L,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0aa8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2385;
	}
{{	uae_s32 dst = get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_long (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2385: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* EOR.L #<data>.L,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0ab0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (8));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2386;
	}
{{	uae_s32 dst = get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_long (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2386: ;
return 34 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* EOR.L #<data>.L,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0ab8_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2387;
	}
{{	uae_s32 dst = get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (10);
	put_long (dsta, src);
}}}}}	m68k_incpc (8);
endlabel2387: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* EOR.L #<data>.L,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0ab9_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = get_long_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2388;
	}
{{	uae_s32 dst = get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (12);
	put_long (dsta, src);
}}}}}	m68k_incpc (10);
endlabel2388: ;
return 36 * CYCLE_UNIT / 2;
} /* 36 (7/2) */

/* CMP.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0c00_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMP.B #<data>.B,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0c10_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.B #<data>.B,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0c18_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.B #<data>.B,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0c20_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.B #<data>.B,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0c28_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.B #<data>.B,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0c30_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (6);
return 22 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.B #<data>.B,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0c38_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.B #<data>.B,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0c39_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (8);
return 24 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* CMP.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0c40_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMP.W #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0c50_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2398;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel2398: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.W #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0c58_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2399;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel2399: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.W #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0c60_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2400;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel2400: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.W #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0c68_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2401;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel2401: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.W #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0c70_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2402;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel2402: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.W #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0c78_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2403;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel2403: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.W #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0c79_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2404;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (8);
endlabel2404: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* CMP.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_0c80_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.L #<data>.L,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0c90_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2406;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel2406: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* CMP.L #<data>.L,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_0c98_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2407;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel2407: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* CMP.L #<data>.L,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_0ca0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2408;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel2408: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* CMP.L #<data>.L,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_0ca8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2409;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (8);
endlabel2409: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* CMP.L #<data>.L,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_0cb0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (8));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2410;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (8);
endlabel2410: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* CMP.L #<data>.L,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_0cb8_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2411;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (10);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (8);
endlabel2411: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* CMP.L #<data>.L,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_0cb9_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = get_long_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2412;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (12);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (10);
endlabel2412: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (7/0) */

/* MOVE.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_1000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MOVE.B (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_1010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVE.B (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_1018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVE.B -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_1020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVE.B (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_1028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.B (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_1030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.B (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_1038_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.B (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_1039_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVE.B (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_103a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.B (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_103b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_103c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVE.B Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MOVE.B (An),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B (An)+,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B -(An),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_10a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B (d16,An),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_10a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (d8,An,Xn),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_10b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (xxx).W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_10b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (xxx).L,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_10b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (d16,PC),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_10ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (d8,PC,Xn),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_10bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B #<data>.B,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_10bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 16 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MOVE.B (An),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B (An)+,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B -(An),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B (d16,An),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (d8,An,Xn),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (xxx).W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (xxx).L,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (d16,PC),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (d8,PC,Xn),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B #<data>.B,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_10fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 16 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
}}}return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MOVE.B (An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
}}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B (An)+,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
}}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (2);
}}}}return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B (d16,An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (d8,An,Xn),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
}}}}return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (xxx).W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1138_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (xxx).L,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_1139_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (d16,PC),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_113a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (d8,PC,Xn),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_113b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
}}}}return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B #<data>.B,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_113c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
}}}return 16 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_1140_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B (An),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_1150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (An)+,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_1158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B -(An),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_1160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (d16,An),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_1168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (d8,An,Xn),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_1170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (xxx).W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_1178_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (xxx).L,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_1179_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.B (d16,PC),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_117a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (d8,PC,Xn),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_117b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B #<data>.B,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_117c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_1180_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B (An),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_1190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (An)+,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_1198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B -(An),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_11a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (d16,An),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_11a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (d8,An,Xn),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_11b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (xxx).W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_11b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (xxx).L,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_11b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (8));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 26 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.B (d16,PC),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_11ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (d8,PC,Xn),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_11bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B #<data>.B,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_11bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 22 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.B (An),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (An)+,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B -(An),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (d16,An),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (d8,An,Xn),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (xxx).W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (xxx).L,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.B (d16,PC),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11fa_11)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (d8,PC,Xn),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11fb_11)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B #<data>.B,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_11fc_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.B (An),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (4) << 16;
	dsta |= regs.irc;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (An)+,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta;
	dsta = get_word_prefetch (4) << 16;
	dsta |= regs.irc;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B -(An),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_word_prefetch (4) << 16;
	dsta |= regs.irc;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (6);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.B (d16,An),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.B (d8,An,Xn),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 26 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.B (xxx).W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.B (xxx).L,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (8) << 16;
	dsta |= regs.irc;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (10);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 28 * CYCLE_UNIT / 2;
} /* 28 (6/1) */

/* MOVE.B (d16,PC),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13fa_11)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.B (d8,PC,Xn),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13fb_11)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}return 26 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.B #<data>.B,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_13fc_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	put_byte (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_2000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MOVE.L An,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_2008_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MOVE.L (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_2010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2503;
	}
{{	uae_s32 src = get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2503: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.L (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_2018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2504;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2504: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.L -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_2020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2505;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2505: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.L (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_2028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2506;
	}
{{	uae_s32 src = get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2506: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVE.L (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_2030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2507;
	}
{{	uae_s32 src = get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2507: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVE.L (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_2038_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2508;
	}
{{	uae_s32 src = get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2508: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVE.L (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_2039_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2509;
	}
{{	uae_s32 src = get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2509: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* MOVE.L (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_203a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2510;
	}
{{	uae_s32 src = get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2510: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVE.L (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_203b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2511;
	}
{{	uae_s32 src = get_long (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2511: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVE.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_203c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVEA.L Dn,An */
uae_u32 REGPARAM2 CPUFUNC(op_2040_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MOVEA.L An,An */
uae_u32 REGPARAM2 CPUFUNC(op_2048_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MOVEA.L (An),An */
uae_u32 REGPARAM2 CPUFUNC(op_2050_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2515;
	}
{{	uae_s32 src = get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2515: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVEA.L (An)+,An */
uae_u32 REGPARAM2 CPUFUNC(op_2058_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2516;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2516: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVEA.L -(An),An */
uae_u32 REGPARAM2 CPUFUNC(op_2060_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2517;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2517: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVEA.L (d16,An),An */
uae_u32 REGPARAM2 CPUFUNC(op_2068_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2518;
	}
{{	uae_s32 src = get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2518: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVEA.L (d8,An,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_2070_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2519;
	}
{{	uae_s32 src = get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2519: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVEA.L (xxx).W,An */
uae_u32 REGPARAM2 CPUFUNC(op_2078_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2520;
	}
{{	uae_s32 src = get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2520: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVEA.L (xxx).L,An */
uae_u32 REGPARAM2 CPUFUNC(op_2079_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2521;
	}
{{	uae_s32 src = get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2521: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* MOVEA.L (d16,PC),An */
uae_u32 REGPARAM2 CPUFUNC(op_207a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2522;
	}
{{	uae_s32 src = get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2522: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVEA.L (d8,PC,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_207b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2523;
	}
{{	uae_s32 src = get_long (srca);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2523: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVEA.L #<data>.L,An */
uae_u32 REGPARAM2 CPUFUNC(op_207c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	m68k_areg (regs, dstreg) = (src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.L Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2525;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2525: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (1/2) */

/* MOVE.L An,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2088_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2526;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2526: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (1/2) */

/* MOVE.L (An),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2527;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2527;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2527: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L (An)+,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2528;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2528;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2528: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L -(An),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_20a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2529;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2529;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2529: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L (d16,An),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_20a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2530;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2530;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2530: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (d8,An,Xn),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_20b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2531;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2531;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2531: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (xxx).W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_20b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2532;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2532;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2532: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (xxx).L,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_20b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2533;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2533;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2533: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (d16,PC),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_20ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2534;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2534;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2534: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (d8,PC,Xn),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_20bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2535;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2535;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2535: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L #<data>.L,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_20bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2536;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2536: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2537;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2537: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (1/2) */

/* MOVE.L An,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2538;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2538: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (1/2) */

/* MOVE.L (An),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2539;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2539;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2539: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L (An)+,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2540;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2540;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2540: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L -(An),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2541;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2541;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2541: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L (d16,An),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2542;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2542;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2542: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (d8,An,Xn),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2543;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2543;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2543: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (xxx).W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2544;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2544;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2544: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (xxx).L,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2545;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2545;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2545: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (d16,PC),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2546;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2546;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2546: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (d8,PC,Xn),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2547;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2547;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2547: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L #<data>.L,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_20fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2548;
	}
{	m68k_areg (regs, dstreg) += 4;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2548: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2549;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
}}}}endlabel2549: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (1/2) */

/* MOVE.L An,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2108_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2550;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
}}}}endlabel2550: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (1/2) */

/* MOVE.L (An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2551;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2551;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
}}}}}}endlabel2551: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L (An)+,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2552;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2552;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
}}}}}}endlabel2552: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2553;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2553;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (2);
}}}}}}endlabel2553: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L (d16,An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2554;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2554;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
}}}}}}endlabel2554: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (d8,An,Xn),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2555;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2555;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
}}}}}}endlabel2555: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (xxx).W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2138_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2556;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2556;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
}}}}}}endlabel2556: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (xxx).L,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_2139_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2557;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2557;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
}}}}}}endlabel2557: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (d16,PC),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_213a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2558;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2558;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
}}}}}}endlabel2558: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (d8,PC,Xn),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_213b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2559;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2559;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
}}}}}}endlabel2559: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L #<data>.L,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_213c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2560;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
}}}}endlabel2560: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_2140_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2561;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2561: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* MOVE.L An,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_2148_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2562;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2562: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* MOVE.L (An),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_2150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2563;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2563;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2563: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (An)+,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_2158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2564;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2564;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2564: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L -(An),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_2160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2565;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2565;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2565: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (d16,An),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_2168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2566;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2566;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2566: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

#endif

#ifdef PART_3
/* MOVE.L (d8,An,Xn),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_2170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2567;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2567;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2567: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (xxx).W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_2178_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2568;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2568;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2568: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (xxx).L,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_2179_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2569;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2569;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2569: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* MOVE.L (d16,PC),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_217a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2570;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2570;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2570: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (d8,PC,Xn),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_217b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2571;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2571;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2571: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L #<data>.L,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_217c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2572;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2572: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_2180_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2573;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2573: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* MOVE.L An,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_2188_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2574;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2574: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* MOVE.L (An),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_2190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2575;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2575;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2575: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (An)+,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_2198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2576;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2576;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2576: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L -(An),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_21a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2577;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2577;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2577: ;
return 28 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (d16,An),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_21a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2578;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2578;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2578: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (d8,An,Xn),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_21b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2579;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2579;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2579: ;
return 32 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (xxx).W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_21b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2580;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2580;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2580: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (xxx).L,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_21b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2581;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (8));
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2581;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2581: ;
return 34 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* MOVE.L (d16,PC),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_21ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2582;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2582;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2582: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (d8,PC,Xn),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_21bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2583;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2583;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2583: ;
return 32 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L #<data>.L,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_21bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (8));
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2584;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2584: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2585;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2585: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* MOVE.L An,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2586;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2586: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* MOVE.L (An),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2587;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2587;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2587: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (An)+,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2588;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2588;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2588: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L -(An),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2589;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2589;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2589: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L (d16,An),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2590;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2590;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2590: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (d8,An,Xn),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2591;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2591;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2591: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (xxx).W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2592;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2592;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2592: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (xxx).L,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2593;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2593;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2593: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* MOVE.L (d16,PC),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21fa_11)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2594;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2594;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2594: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (d8,PC,Xn),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21fb_11)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2595;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2595;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2595: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L #<data>.L,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_21fc_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2596;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2596: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* MOVE.L Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2597;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2597: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L An,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2598;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2598: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* MOVE.L (An),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2599;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2599;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2599: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (An)+,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2600;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta;
	dsta = get_word_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2600;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2600: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L -(An),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2601;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_word_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2601;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (6);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2601: ;
return 30 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.L (d16,An),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2602;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2602;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2602: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* MOVE.L (d8,An,Xn),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2603;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2603;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2603: ;
return 34 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* MOVE.L (xxx).W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2604;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2604;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2604: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* MOVE.L (xxx).L,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2605;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (8) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (10);
		exception3 (opcode, dsta);
		goto endlabel2605;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (10);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2605: ;
return 36 * CYCLE_UNIT / 2;
} /* 36 (7/2) */

/* MOVE.L (d16,PC),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23fa_11)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2606;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2606;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2606: ;
return 32 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* MOVE.L (d8,PC,Xn),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23fb_11)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2607;
	}
{{	uae_s32 src = get_long (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2607;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2607: ;
return 34 * CYCLE_UNIT / 2;
} /* 32 (6/2) */

/* MOVE.L #<data>.L,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_23fc_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
{	uaecptr dsta = get_long_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (10);
		exception3 (opcode, dsta);
		goto endlabel2608;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	put_long (dsta, src);
	m68k_incpc (10);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2608: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MOVE.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_3000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MOVE.W An,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_3008_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MOVE.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_3010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2611;
	}
{{	uae_s16 src = get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2611: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVE.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_3018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2612;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2612: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVE.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_3020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2613;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2613: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVE.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_3028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2614;
	}
{{	uae_s16 src = get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2614: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_3030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2615;
	}
{{	uae_s16 src = get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2615: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_3038_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2616;
	}
{{	uae_s16 src = get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2616: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_3039_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2617;
	}
{{	uae_s16 src = get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2617: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVE.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_303a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2618;
	}
{{	uae_s16 src = get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2618: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_303b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2619;
	}
{{	uae_s16 src = get_word (srca);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2619: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVE.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_303c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVEA.W Dn,An */
uae_u32 REGPARAM2 CPUFUNC(op_3040_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MOVEA.W An,An */
uae_u32 REGPARAM2 CPUFUNC(op_3048_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MOVEA.W (An),An */
uae_u32 REGPARAM2 CPUFUNC(op_3050_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2623;
	}
{{	uae_s16 src = get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2623: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVEA.W (An)+,An */
uae_u32 REGPARAM2 CPUFUNC(op_3058_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2624;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2624: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVEA.W -(An),An */
uae_u32 REGPARAM2 CPUFUNC(op_3060_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2625;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2625: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVEA.W (d16,An),An */
uae_u32 REGPARAM2 CPUFUNC(op_3068_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2626;
	}
{{	uae_s16 src = get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2626: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVEA.W (d8,An,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_3070_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2627;
	}
{{	uae_s16 src = get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2627: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVEA.W (xxx).W,An */
uae_u32 REGPARAM2 CPUFUNC(op_3078_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2628;
	}
{{	uae_s16 src = get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2628: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVEA.W (xxx).L,An */
uae_u32 REGPARAM2 CPUFUNC(op_3079_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2629;
	}
{{	uae_s16 src = get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2629: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MOVEA.W (d16,PC),An */
uae_u32 REGPARAM2 CPUFUNC(op_307a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2630;
	}
{{	uae_s16 src = get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2630: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVEA.W (d8,PC,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_307b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2631;
	}
{{	uae_s16 src = get_word (srca);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2631: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MOVEA.W #<data>.W,An */
uae_u32 REGPARAM2 CPUFUNC(op_307c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	src = (uae_s32)(uae_s16)src;
	m68k_areg (regs, dstreg) = (uae_s32)(uae_s16)(src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MOVE.W Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2633;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2633: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MOVE.W An,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3088_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2634;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2634: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MOVE.W (An),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2635;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2635;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2635: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W (An)+,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2636;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2636;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2636: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W -(An),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_30a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2637;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2637;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2637: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W (d16,An),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_30a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2638;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2638;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2638: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (d8,An,Xn),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_30b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2639;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2639;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2639: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (xxx).W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_30b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2640;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2640;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2640: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (xxx).L,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_30b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2641;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2641;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2641: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (d16,PC),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_30ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2642;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2642;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2642: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (d8,PC,Xn),(An) */
uae_u32 REGPARAM2 CPUFUNC(op_30bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2643;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2643;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2643: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_30bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2644;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2644: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2645;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2645: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MOVE.W An,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2646;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2646: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MOVE.W (An),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2647;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2647;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2647: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W (An)+,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2648;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2648;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2648: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W -(An),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2649;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2649;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2649: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W (d16,An),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2650;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2650;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2650: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (d8,An,Xn),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2651;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2651;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2651: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (xxx).W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2652;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2652;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2652: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (xxx).L,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2653;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2653;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2653: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (d16,PC),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2654;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2654;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2654: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (d8,PC,Xn),(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2655;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2655;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2655: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_30fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2656;
	}
{	m68k_areg (regs, dstreg) += 2;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2656: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2657;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
}}}}endlabel2657: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MOVE.W An,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3108_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2658;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
}}}}endlabel2658: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MOVE.W (An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2659;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2659;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
}}}}}}endlabel2659: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W (An)+,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2660;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2660;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
}}}}}}endlabel2660: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2661;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2661;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (2);
}}}}}}endlabel2661: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W (d16,An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2662;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2662;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
}}}}}}endlabel2662: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (d8,An,Xn),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2663;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2663;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
}}}}}}endlabel2663: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (xxx).W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3138_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2664;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2664;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
}}}}}}endlabel2664: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (xxx).L,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_3139_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2665;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2665;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
}}}}}}endlabel2665: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (d16,PC),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_313a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2666;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2666;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
}}}}}}endlabel2666: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (d8,PC,Xn),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_313b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2667;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2667;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
}}}}}}endlabel2667: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_313c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2668;
	}
{	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
}}}}endlabel2668: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_3140_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2669;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2669: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W An,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_3148_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2670;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2670: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W (An),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_3150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2671;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2671;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2671: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (An)+,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_3158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2672;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2672;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2672: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W -(An),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_3160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2673;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2673;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2673: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (d16,An),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_3168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2674;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2674;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2674: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (d8,An,Xn),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_3170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2675;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2675;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2675: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (xxx).W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_3178_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2676;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2676;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2676: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (xxx).L,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_3179_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2677;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2677;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2677: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.W (d16,PC),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_317a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2678;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2678;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2678: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (d8,PC,Xn),(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_317b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2679;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2679;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2679: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_317c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2680;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2680: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_3180_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2681;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2681: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W An,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_3188_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2682;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2682: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W (An),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_3190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2683;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2683;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2683: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (An)+,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_3198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2684;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2684;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2684: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W -(An),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_31a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2685;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2685;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2685: ;
return 20 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (d16,An),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_31a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2686;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2686;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2686: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (d8,An,Xn),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_31b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2687;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2687;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2687: ;
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (xxx).W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_31b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2688;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2688;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2688: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (xxx).L,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_31b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2689;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (8));
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2689;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2689: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.W (d16,PC),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_31ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2690;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2690;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2690: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (d8,PC,Xn),(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_31bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2691;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2691;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2691: ;
return 24 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_31bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2692;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2692: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2693;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2693: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W An,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2694;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2694: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MOVE.W (An),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2695;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2695;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2695: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (An)+,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2696;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2696;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2696: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W -(An),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2697;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2697;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (4);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2697: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (d16,An),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2698;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2698;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2698: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (d8,An,Xn),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2699;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2699;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2699: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (xxx).W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2700;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2700;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2700: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (xxx).L,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2701;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (8);
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2701;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2701: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.W (d16,PC),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31fa_11)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2702;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2702;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2702: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (d8,PC,Xn),(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31fb_11)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2703;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2703;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2703: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_31fc_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2704;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2704: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2705;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2705: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W An,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2706;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2706: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* MOVE.W (An),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2707;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2707;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2707: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (An)+,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2708;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta;
	dsta = get_word_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2708;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2708: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W -(An),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2709;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = get_word_prefetch (4) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (6);
		exception3 (opcode, dsta);
		goto endlabel2709;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (6);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2709: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MOVE.W (d16,An),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2710;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2710;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2710: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.W (d8,An,Xn),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2711;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2711;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2711: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.W (xxx).W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2712;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2712;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2712: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.W (xxx).L,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
	m68k_incpc (4);
		exception3 (opcode, srca);
		goto endlabel2713;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (8) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (10);
		exception3 (opcode, dsta);
		goto endlabel2713;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (10);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2713: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (6/1) */

/* MOVE.W (d16,PC),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33fa_11)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2714;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2714;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2714: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.W (d8,PC,Xn),(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33fb_11)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2715;
	}
{{	uae_s16 src = get_word (srca);
{	uaecptr dsta;
	dsta = get_word_prefetch (6) << 16;
	dsta |= regs.irc;
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2715;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (8);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}endlabel2715: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (5/1) */

/* MOVE.W #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_33fc_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
{	uaecptr dsta = get_long_prefetch (6);
	if (dsta & 1) {
	m68k_incpc (8);
		exception3 (opcode, dsta);
		goto endlabel2716;
	}
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	put_word (dsta, src);
	m68k_incpc (8);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}endlabel2716: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* NEGX.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NEGX.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte (srca, newv);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEGX.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte (srca, newv);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEGX.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte (srca, newv);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEGX.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte (srca, newv);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEGX.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte (srca, newv);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEGX.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4038_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte (srca, newv);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEGX.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4039_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte (srca, newv);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* NEGX.W Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4040_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NEGX.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4050_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2726;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word (srca, newv);
}}}}}}	m68k_incpc (2);
endlabel2726: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEGX.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4058_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2727;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word (srca, newv);
}}}}}}	m68k_incpc (2);
endlabel2727: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEGX.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4060_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2728;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word (srca, newv);
}}}}}}	m68k_incpc (2);
endlabel2728: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEGX.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4068_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2729;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word (srca, newv);
}}}}}}	m68k_incpc (4);
endlabel2729: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEGX.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4070_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2730;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word (srca, newv);
}}}}}}	m68k_incpc (4);
endlabel2730: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEGX.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4078_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2731;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word (srca, newv);
}}}}}}	m68k_incpc (4);
endlabel2731: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEGX.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4079_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2732;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word (srca, newv);
}}}}}}	m68k_incpc (6);
endlabel2732: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* NEGX.L Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NEGX.L (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2734;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long (srca, newv);
}}}}}}	m68k_incpc (2);
endlabel2734: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* NEGX.L (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2735;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long (srca, newv);
}}}}}}	m68k_incpc (2);
endlabel2735: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* NEGX.L -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_40a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2736;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long (srca, newv);
}}}}}}	m68k_incpc (2);
endlabel2736: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* NEGX.L (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_40a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2737;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long (srca, newv);
}}}}}}	m68k_incpc (4);
endlabel2737: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* NEGX.L (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_40b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2738;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long (srca, newv);
}}}}}}	m68k_incpc (4);
endlabel2738: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* NEGX.L (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_40b8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2739;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long (srca, newv);
}}}}}}	m68k_incpc (4);
endlabel2739: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* NEGX.L (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_40b9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2740;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = 0 - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long (srca, newv);
}}}}}}	m68k_incpc (6);
endlabel2740: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MVSR2.W Dn */
uae_u32 REGPARAM2 CPUFUNC(op_40c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	regs.ir = regs.irc;
	get_word_prefetch (4);
	MakeSR ();
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((regs.sr) & 0xffff);
}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MVSR2.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_40d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2742;
	}
{	regs.ir = regs.irc;
	get_word_prefetch (4);
	MakeSR ();
	put_word (srca, regs.sr);
}}}	m68k_incpc (2);
endlabel2742: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MVSR2.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_40d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2743;
	}
{	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	MakeSR ();
	put_word (srca, regs.sr);
}}}	m68k_incpc (2);
endlabel2743: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MVSR2.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_40e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2744;
	}
{	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	MakeSR ();
	put_word (srca, regs.sr);
}}}	m68k_incpc (2);
endlabel2744: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (1/1) */

/* MVSR2.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_40e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2745;
	}
{	regs.ir = regs.irc;
	get_word_prefetch (6);
	MakeSR ();
	put_word (srca, regs.sr);
}}}	m68k_incpc (4);
endlabel2745: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MVSR2.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_40f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2746;
	}
{	regs.ir = regs.irc;
	get_word_prefetch (6);
	MakeSR ();
	put_word (srca, regs.sr);
}}}	m68k_incpc (4);
endlabel2746: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MVSR2.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_40f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2747;
	}
{	regs.ir = regs.irc;
	get_word_prefetch (6);
	MakeSR ();
	put_word (srca, regs.sr);
}}}	m68k_incpc (4);
endlabel2747: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* MVSR2.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_40f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2748;
	}
{	regs.ir = regs.irc;
	get_word_prefetch (8);
	MakeSR ();
	put_word (srca, regs.sr);
}}}	m68k_incpc (6);
endlabel2748: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* CHK.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4180_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2749;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2749;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel2749: ;
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CHK.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2750;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2750;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2750;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2750: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CHK.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2751;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2751;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2751;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2751: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CHK.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_41a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2752;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (2);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2752;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2752;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2752: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CHK.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_41a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2753;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2753;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2753;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2753: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CHK.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_41b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2754;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2754;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2754;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2754: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CHK.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_41b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2755;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2755;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2755;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2755: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CHK.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_41b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2756;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (6);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2756;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2756;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2756: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CHK.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_41ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2757;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2757;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2757;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2757: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CHK.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_41bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2758;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2758;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2758;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2758: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CHK.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_41bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	m68k_incpc (4);
	if (dst > src) {
		SET_NFLG (0);
		Exception (6);
		goto endlabel2759;
	}
	if ((uae_s32)dst < 0) {
		SET_NFLG (1);
		Exception (6);
		goto endlabel2759;
	}
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel2759: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* LEA.L (An),An */
uae_u32 REGPARAM2 CPUFUNC(op_41d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_areg (regs, dstreg) = (srca);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LEA.L (d16,An),An */
uae_u32 REGPARAM2 CPUFUNC(op_41e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_areg (regs, dstreg) = (srca);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* LEA.L (d8,An,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_41f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_areg (regs, dstreg) = (srca);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* LEA.L (xxx).W,An */
uae_u32 REGPARAM2 CPUFUNC(op_41f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_areg (regs, dstreg) = (srca);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* LEA.L (xxx).L,An */
uae_u32 REGPARAM2 CPUFUNC(op_41f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_areg (regs, dstreg) = (srca);
}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* LEA.L (d16,PC),An */
uae_u32 REGPARAM2 CPUFUNC(op_41fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_areg (regs, dstreg) = (srca);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* LEA.L (d8,PC,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_41fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_areg (regs, dstreg) = (srca);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CLR.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4200_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((0) & 0xff);
}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CLR.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4210_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte (srca, 0);
}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* CLR.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4218_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte (srca, 0);
}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* CLR.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4220_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte (srca, 0);
}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* CLR.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4228_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte (srca, 0);
}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* CLR.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4230_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte (srca, 0);
}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* CLR.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4238_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte (srca, 0);
}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* CLR.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4239_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(0)) == 0);
	SET_NFLG   (((uae_s8)(0)) < 0);
	put_byte (srca, 0);
}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* CLR.W Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4240_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((0) & 0xffff);
}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CLR.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4250_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2776;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word (srca, 0);
}}}}	m68k_incpc (2);
endlabel2776: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* CLR.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4258_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2777;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word (srca, 0);
}}}}	m68k_incpc (2);
endlabel2777: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* CLR.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4260_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2778;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word (srca, 0);
}}}}	m68k_incpc (2);
endlabel2778: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* CLR.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4268_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2779;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word (srca, 0);
}}}}	m68k_incpc (4);
endlabel2779: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* CLR.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4270_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2780;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word (srca, 0);
}}}}	m68k_incpc (4);
endlabel2780: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* CLR.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4278_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2781;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word (srca, 0);
}}}}	m68k_incpc (4);
endlabel2781: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* CLR.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4279_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2782;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(0)) == 0);
	SET_NFLG   (((uae_s16)(0)) < 0);
	put_word (srca, 0);
}}}}	m68k_incpc (6);
endlabel2782: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* CLR.L Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4280_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	m68k_dreg (regs, srcreg) = (0);
}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CLR.L (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4290_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2784;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long (srca, 0);
}}}}	m68k_incpc (2);
endlabel2784: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* CLR.L (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4298_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2785;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long (srca, 0);
}}}}	m68k_incpc (2);
endlabel2785: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* CLR.L -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_42a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2786;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long (srca, 0);
}}}}	m68k_incpc (2);
endlabel2786: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* CLR.L (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_42a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2787;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long (srca, 0);
}}}}	m68k_incpc (4);
endlabel2787: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* CLR.L (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_42b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2788;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long (srca, 0);
}}}}	m68k_incpc (4);
endlabel2788: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* CLR.L (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_42b8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2789;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long (srca, 0);
}}}}	m68k_incpc (4);
endlabel2789: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* CLR.L (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_42b9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2790;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(0)) == 0);
	SET_NFLG   (((uae_s32)(0)) < 0);
	put_long (srca, 0);
}}}}	m68k_incpc (6);
endlabel2790: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

#endif

#ifdef PART_4
/* NEG.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4400_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NEG.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4410_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (srca, dst);
}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEG.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4418_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (srca, dst);
}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEG.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4420_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (srca, dst);
}}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEG.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4428_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (srca, dst);
}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEG.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4430_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (srca, dst);
}}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEG.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4438_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (srca, dst);
}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEG.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4439_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(0)) < 0;
	int flgn = ((uae_s8)(dst)) < 0;
	SET_ZFLG (((uae_s8)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (srca, dst);
}}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* NEG.W Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4440_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NEG.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4450_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2800;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (srca, dst);
}}}}}}}	m68k_incpc (2);
endlabel2800: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEG.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4458_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2801;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (srca, dst);
}}}}}}}	m68k_incpc (2);
endlabel2801: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEG.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4460_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2802;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (srca, dst);
}}}}}}}	m68k_incpc (2);
endlabel2802: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NEG.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4468_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2803;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (srca, dst);
}}}}}}}	m68k_incpc (4);
endlabel2803: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEG.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4470_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2804;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (srca, dst);
}}}}}}}	m68k_incpc (4);
endlabel2804: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEG.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4478_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2805;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (srca, dst);
}}}}}}}	m68k_incpc (4);
endlabel2805: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NEG.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4479_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2806;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(0)) < 0;
	int flgn = ((uae_s16)(dst)) < 0;
	SET_ZFLG (((uae_s16)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (srca, dst);
}}}}}}}	m68k_incpc (6);
endlabel2806: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* NEG.L Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4480_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NEG.L (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4490_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2808;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (srca, dst);
}}}}}}}	m68k_incpc (2);
endlabel2808: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* NEG.L (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4498_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2809;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (srca, dst);
}}}}}}}	m68k_incpc (2);
endlabel2809: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* NEG.L -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_44a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2810;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (srca, dst);
}}}}}}}	m68k_incpc (2);
endlabel2810: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* NEG.L (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_44a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2811;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (srca, dst);
}}}}}}}	m68k_incpc (4);
endlabel2811: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* NEG.L (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_44b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2812;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (srca, dst);
}}}}}}}	m68k_incpc (4);
endlabel2812: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* NEG.L (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_44b8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2813;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (srca, dst);
}}}}}}}	m68k_incpc (4);
endlabel2813: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* NEG.L (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_44b9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2814;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(0)) < 0;
	int flgn = ((uae_s32)(dst)) < 0;
	SET_ZFLG (((uae_s32)(dst)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(0)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (srca, dst);
}}}}}}}	m68k_incpc (6);
endlabel2814: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MV2SR.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_44c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	get_wordi (4);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (4);
}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MV2SR.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_44d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2816;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (4);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (4);
}}}}	m68k_incpc (2);
endlabel2816: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MV2SR.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_44d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2817;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
	get_wordi (4);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (4);
}}}}	m68k_incpc (2);
endlabel2817: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MV2SR.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_44e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2818;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
	get_wordi (4);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (4);
}}}}	m68k_incpc (2);
endlabel2818: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MV2SR.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_44e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2819;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (6);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2819: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MV2SR.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_44f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2820;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (6);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2820: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MV2SR.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_44f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2821;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (6);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2821: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MV2SR.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_44f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2822;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (8);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2822: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* MV2SR.B (d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_44fa_11)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2823;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (6);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2823: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MV2SR.B (d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_44fb_11)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2824;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (6);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2824: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MV2SR.B #<data>.B */
uae_u32 REGPARAM2 CPUFUNC(op_44fc_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
	get_wordi (6);
	MakeSR ();
	regs.sr &= 0xFF00;
	regs.sr |= src & 0xFF;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* NOT.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4600_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((dst) & 0xff);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NOT.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4610_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte (srca, dst);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NOT.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4618_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte (srca, dst);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NOT.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4620_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte (srca, dst);
}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NOT.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4628_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte (srca, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NOT.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4630_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte (srca, dst);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NOT.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4638_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte (srca, dst);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NOT.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4639_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(dst)) == 0);
	SET_NFLG   (((uae_s8)(dst)) < 0);
	put_byte (srca, dst);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* NOT.W Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4640_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NOT.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4650_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2835;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word (srca, dst);
}}}}}	m68k_incpc (2);
endlabel2835: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NOT.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4658_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2836;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word (srca, dst);
}}}}}	m68k_incpc (2);
endlabel2836: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NOT.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4660_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2837;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word (srca, dst);
}}}}}	m68k_incpc (2);
endlabel2837: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NOT.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4668_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2838;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word (srca, dst);
}}}}}	m68k_incpc (4);
endlabel2838: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NOT.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4670_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2839;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word (srca, dst);
}}}}}	m68k_incpc (4);
endlabel2839: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NOT.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4678_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2840;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word (srca, dst);
}}}}}	m68k_incpc (4);
endlabel2840: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NOT.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4679_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2841;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	put_word (srca, dst);
}}}}}	m68k_incpc (6);
endlabel2841: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* NOT.L Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4680_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (dst);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NOT.L (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4690_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2843;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long (srca, dst);
}}}}}	m68k_incpc (2);
endlabel2843: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* NOT.L (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4698_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2844;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long (srca, dst);
}}}}}	m68k_incpc (2);
endlabel2844: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* NOT.L -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_46a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2845;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long (srca, dst);
}}}}}	m68k_incpc (2);
endlabel2845: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* NOT.L (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_46a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2846;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long (srca, dst);
}}}}}	m68k_incpc (4);
endlabel2846: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* NOT.L (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_46b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2847;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long (srca, dst);
}}}}}	m68k_incpc (4);
endlabel2847: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* NOT.L (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_46b8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2848;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long (srca, dst);
}}}}}	m68k_incpc (4);
endlabel2848: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* NOT.L (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_46b9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2849;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 dst = ~src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	put_long (srca, dst);
}}}}}	m68k_incpc (6);
endlabel2849: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MV2SR.W Dn */
uae_u32 REGPARAM2 CPUFUNC(op_46c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel2850; }
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	get_wordi (4);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (4);
}}}	m68k_incpc (2);
endlabel2850: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* MV2SR.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_46d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel2851; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2851;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (4);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (4);
}}}}}	m68k_incpc (2);
endlabel2851: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MV2SR.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_46d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel2852; }
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2852;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
	get_wordi (4);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (4);
}}}}}	m68k_incpc (2);
endlabel2852: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MV2SR.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_46e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel2853; }
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2853;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
	get_wordi (4);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (4);
}}}}}	m68k_incpc (2);
endlabel2853: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MV2SR.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_46e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel2854; }
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2854;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (6);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}}	m68k_incpc (4);
endlabel2854: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MV2SR.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_46f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel2855; }
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2855;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (6);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}}	m68k_incpc (4);
endlabel2855: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MV2SR.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_46f8_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2856; }
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2856;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (6);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}}	m68k_incpc (4);
endlabel2856: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MV2SR.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_46f9_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2857; }
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2857;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (8);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}}	m68k_incpc (6);
endlabel2857: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* MV2SR.W (d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_46fa_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2858; }
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2858;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (6);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}}	m68k_incpc (4);
endlabel2858: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MV2SR.W (d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_46fb_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2859; }
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2859;
	}
{{	uae_s16 src = get_word (srca);
	get_wordi (6);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}}	m68k_incpc (4);
endlabel2859: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* MV2SR.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_46fc_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2860; }
{{	uae_s16 src = get_word_prefetch (4);
	get_wordi (6);
	regs.sr = src;
	MakeFromSR ();
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}	m68k_incpc (4);
endlabel2860: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* NBCD.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4800_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NBCD.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4810_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
	put_byte (srca, newv);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NBCD.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4818_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
	put_byte (srca, newv);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NBCD.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4820_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
	put_byte (srca, newv);
}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* NBCD.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4828_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
	put_byte (srca, newv);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NBCD.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4830_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
	put_byte (srca, newv);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NBCD.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4838_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
	put_byte (srca, newv);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* NBCD.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4839_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
	put_byte (srca, newv);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SWAP.W Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4840_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = ((src >> 16)&0xFFFF) | ((src&0xFFFF)<<16);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (dst);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* PEA.L (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4850_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2870;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_long (dsta, srca);
}}}}	m68k_incpc (2);
endlabel2870: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (1/2) */

/* PEA.L (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4868_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2871;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, srca);
}}}}	m68k_incpc (4);
endlabel2871: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* PEA.L (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4870_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2872;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, srca);
}}}}	m68k_incpc (4);
endlabel2872: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* PEA.L (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4878_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2873;
	}
{	m68k_areg (regs, 7) = dsta;
	put_long (dsta, srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2873: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* PEA.L (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4879_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	if (dsta & 1) {
	m68k_incpc (4);
		exception3 (opcode, dsta);
		goto endlabel2874;
	}
{	m68k_areg (regs, 7) = dsta;
	put_long (dsta, srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2874: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* PEA.L (d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_487a_11)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2875;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, srca);
}}}}	m68k_incpc (4);
endlabel2875: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* PEA.L (d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_487b_11)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uaecptr dsta;
	dsta = m68k_areg (regs, 7) - 4;
	if (dsta & 1) {
	m68k_incpc (2);
		exception3 (opcode, dsta);
		goto endlabel2876;
	}
{	m68k_areg (regs, 7) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, srca);
}}}}	m68k_incpc (4);
endlabel2876: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* EXT.W Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4880_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 dst = (uae_s16)(uae_s8)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(dst)) == 0);
	SET_NFLG   (((uae_s16)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MVMLE.W #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4890_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca = m68k_areg (regs, dstreg);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2878;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2878: ;
return 8 * CYCLE_UNIT / 2;
} /* 12+ (2/1) */

/* MVMLE.W #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_48a0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 0;
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2879;
	}
{{	uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
	int type = get_cpu_model () >= 68020;
	while (amask) {
		srca -= 2;
		if (type) m68k_areg (regs, dstreg) = srca;
		put_word (srca, m68k_areg (regs, movem_index2[amask]));
		amask = movem_next[amask];
	}
	while (dmask) { srca -= 2; put_word (srca, m68k_dreg (regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg (regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2879: ;
return 10 * CYCLE_UNIT / 2;
} /* 12+ (2/1) */

/* MVMLE.W #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_48a8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2880;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2880: ;
return 12 * CYCLE_UNIT / 2;
} /* 16+ (3/1) */

/* MVMLE.W #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_48b0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2881;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2881: ;
return 14 * CYCLE_UNIT / 2;
} /* 16+ (3/1) */

/* MVMLE.W #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_48b8_11)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2882;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2882: ;
return 12 * CYCLE_UNIT / 2;
} /* 16+ (3/1) */

/* MVMLE.W #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_48b9_11)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca = get_long_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2883;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_word (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { put_word (srca, m68k_areg (regs, movem_index1[amask])); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (10);
}}}}	m68k_incpc (8);
endlabel2883: ;
return 16 * CYCLE_UNIT / 2;
} /* 20+ (4/1) */

/* EXT.L Dn */
uae_u32 REGPARAM2 CPUFUNC(op_48c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 dst = (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(dst)) == 0);
	SET_NFLG   (((uae_s32)(dst)) < 0);
	m68k_dreg (regs, srcreg) = (dst);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MVMLE.L #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_48d0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca = m68k_areg (regs, dstreg);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2885;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2885: ;
return 8 * CYCLE_UNIT / 2;
} /* 16+ (2/2) */

/* MVMLE.L #<data>.W,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_48e0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca;
	srca = m68k_areg (regs, dstreg) - 0;
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2886;
	}
{{	uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
	int type = get_cpu_model () >= 68020;
	while (amask) {
		srca -= 4;
		if (type) m68k_areg (regs, dstreg) = srca;
		put_long (srca, m68k_areg (regs, movem_index2[amask]));
		amask = movem_next[amask];
	}
	while (dmask) { srca -= 4; put_long (srca, m68k_dreg (regs, movem_index2[dmask])); dmask = movem_next[dmask]; }
	m68k_areg (regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2886: ;
return 10 * CYCLE_UNIT / 2;
} /* 16+ (2/2) */

/* MVMLE.L #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_48e8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2887;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2887: ;
return 12 * CYCLE_UNIT / 2;
} /* 20+ (3/2) */

/* MVMLE.L #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_48f0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2888;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2888: ;
return 14 * CYCLE_UNIT / 2;
} /* 20+ (3/2) */

/* MVMLE.L #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_48f8_11)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2889;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2889: ;
return 12 * CYCLE_UNIT / 2;
} /* 20+ (3/2) */

/* MVMLE.L #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_48f9_11)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_prefetch (4);
{	uaecptr srca = get_long_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2890;
	}
{{	uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
	while (dmask) { put_long (srca, m68k_dreg (regs, movem_index1[dmask])); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { put_long (srca, m68k_areg (regs, movem_index1[amask])); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (10);
}}}}	m68k_incpc (8);
endlabel2890: ;
return 16 * CYCLE_UNIT / 2;
} /* 24+ (4/2) */

/* TST.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4a00_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* TST.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4a10_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* TST.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4a18_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* TST.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4a20_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	m68k_incpc (2);
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* TST.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4a28_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* TST.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4a30_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* TST.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4a38_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* TST.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4a39_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* TST.B #<data>.B */
uae_u32 REGPARAM2 CPUFUNC(op_4a3c_11)(uae_u32 opcode)
{
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* TST.W Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4a40_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* TST.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4a50_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2901;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel2901: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* TST.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4a58_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2902;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel2902: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* TST.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4a60_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2903;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel2903: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* TST.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4a68_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2904;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel2904: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* TST.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4a70_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2905;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel2905: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* TST.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4a78_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2906;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel2906: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* TST.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4a79_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2907;
	}
{{	uae_s16 src = get_word (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}}}	m68k_incpc (6);
endlabel2907: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* TST.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_4a7c_11)(uae_u32 opcode)
{
{{	uae_s16 src = get_word_prefetch (4);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* TST.L Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4a80_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* TST.L (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4a90_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2910;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel2910: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* TST.L (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4a98_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2911;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel2911: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* TST.L -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4aa0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2912;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (2);
endlabel2912: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* TST.L (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4aa8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2913;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel2913: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* TST.L (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4ab0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2914;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel2914: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* TST.L (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4ab8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2915;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (4);
endlabel2915: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* TST.L (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4ab9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel2916;
	}
{{	uae_s32 src = get_long (srca);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}}}	m68k_incpc (6);
endlabel2916: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* TST.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_4abc_11)(uae_u32 opcode)
{
{{	uae_s32 src = get_long_prefetch (4);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* TAS.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_4ac0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	src |= 0x80;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((src) & 0xff);
}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* TAS.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4ad0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	src |= 0x80;
	put_byte (srca, src);
}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* TAS.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4ad8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	src |= 0x80;
	put_byte (srca, src);
}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* TAS.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4ae0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	src |= 0x80;
	put_byte (srca, src);
}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* TAS.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4ae8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	src |= 0x80;
	put_byte (srca, src);
}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* TAS.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4af0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	src |= 0x80;
	put_byte (srca, src);
}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* TAS.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4af8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	src |= 0x80;
	put_byte (srca, src);
}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* TAS.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4af9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	src |= 0x80;
	put_byte (srca, src);
}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* MVMEL.W #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4c90_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2926;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2926: ;
return 8 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* MVMEL.W #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4c98_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2927;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; amask = movem_next[amask]; }
	m68k_areg (regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2927: ;
return 8 * CYCLE_UNIT / 2;
} /* 16+ (4/0) */

/* MVMEL.W #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4ca8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2928;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2928: ;
return 12 * CYCLE_UNIT / 2;
} /* 16+ (4/0) */

/* MVMEL.W #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4cb0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2929;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2929: ;
return 14 * CYCLE_UNIT / 2;
} /* 16+ (4/0) */

/* MVMEL.W #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4cb8_11)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2930;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2930: ;
return 12 * CYCLE_UNIT / 2;
} /* 16+ (4/0) */

/* MVMEL.W #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4cb9_11)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = get_long_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2931;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (10);
}}}}	m68k_incpc (8);
endlabel2931: ;
return 16 * CYCLE_UNIT / 2;
} /* 20+ (5/0) */

/* MVMEL.W #<data>.W,(d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_4cba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2932;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2932: ;
return 12 * CYCLE_UNIT / 2;
} /* 16+ (4/0) */

/* MVMEL.W #<data>.W,(d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4cbb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 4;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (6));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2933;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = (uae_s32)(uae_s16)get_word (srca); srca += 2; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2933: ;
return 14 * CYCLE_UNIT / 2;
} /* 16+ (4/0) */

/* MVMEL.L #<data>.W,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_4cd0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2934;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long (srca); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2934: ;
return 8 * CYCLE_UNIT / 2;
} /* 16+ (4/0) */

/* MVMEL.L #<data>.W,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_4cd8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2935;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long (srca); srca += 4; amask = movem_next[amask]; }
	m68k_areg (regs, dstreg) = srca;
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}	m68k_incpc (4);
endlabel2935: ;
return 8 * CYCLE_UNIT / 2;
} /* 20+ (5/0) */

/* MVMEL.L #<data>.W,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4ce8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2936;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long (srca); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2936: ;
return 12 * CYCLE_UNIT / 2;
} /* 20+ (5/0) */

/* MVMEL.L #<data>.W,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4cf0_11)(uae_u32 opcode)
{
	uae_u32 dstreg = opcode & 7;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (6));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2937;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long (srca); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2937: ;
return 14 * CYCLE_UNIT / 2;
} /* 20+ (5/0) */

/* MVMEL.L #<data>.W,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4cf8_11)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2938;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long (srca); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2938: ;
return 12 * CYCLE_UNIT / 2;
} /* 20+ (5/0) */

/* MVMEL.L #<data>.W,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4cf9_11)(uae_u32 opcode)
{
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = get_long_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2939;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long (srca); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (10);
}}}}	m68k_incpc (8);
endlabel2939: ;
return 16 * CYCLE_UNIT / 2;
} /* 24+ (6/0) */

/* MVMEL.L #<data>.W,(d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_4cfa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 2;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr srca = m68k_getpc () + 4;
	srca += (uae_s32)(uae_s16)get_word_prefetch (6);
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2940;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long (srca); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2940: ;
return 12 * CYCLE_UNIT / 2;
} /* 20+ (5/0) */

/* MVMEL.L #<data>.W,(d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4cfb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = 3;
{	uae_u16 mask = get_word_prefetch (4);
	uae_u32 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 4;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (6));
	if (srca & 1) {
	m68k_incpc (2);
		exception3 (opcode, srca);
		goto endlabel2941;
	}
{{	while (dmask) { m68k_dreg (regs, movem_index1[dmask]) = get_long (srca); srca += 4; dmask = movem_next[dmask]; }
	while (amask) { m68k_areg (regs, movem_index1[amask]) = get_long (srca); srca += 4; amask = movem_next[amask]; }
	regs.ir = regs.irc;
	get_word_prefetch (8);
}}}}	m68k_incpc (6);
endlabel2941: ;
return 14 * CYCLE_UNIT / 2;
} /* 20+ (5/0) */

/* TRAPQ.L #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_4e40_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 15);
{{	uae_u32 src = srcreg;
	m68k_incpc (2);
	Exception (src + 32);
}}return 0 * CYCLE_UNIT / 2;
} /* 4 (0/0) */

/* LINK.W An,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_4e50_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr olda;
	olda = m68k_areg (regs, 7) - 4;
	if (olda & 1) {
		exception3 (opcode, olda);
		goto endlabel2943;
	}
{	m68k_areg (regs, 7) = olda;
{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s16 offs = get_word_prefetch (4);
	put_long (olda, src);
	m68k_areg (regs, srcreg) = (m68k_areg (regs, 7));
	m68k_areg (regs, 7) += offs;
	regs.ir = regs.irc;
	get_word_prefetch (6);
}}}}}	m68k_incpc (4);
endlabel2943: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* UNLK.L An */
uae_u32 REGPARAM2 CPUFUNC(op_4e58_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s32 src = m68k_areg (regs, srcreg);
	m68k_areg (regs, 7) = src;
{	uaecptr olda = m68k_areg (regs, 7);
	if (olda & 1) {
		exception3 (opcode, olda);
		goto endlabel2944;
	}
{{	uae_s32 old = get_long (olda);
	m68k_areg (regs, 7) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_areg (regs, srcreg) = (old);
}}}}}	m68k_incpc (2);
endlabel2944: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MVR2USP.L An */
uae_u32 REGPARAM2 CPUFUNC(op_4e60_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel2945; }
{{	uae_s32 src = m68k_areg (regs, srcreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	regs.usp = src;
}}}	m68k_incpc (2);
endlabel2945: ;
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* MVUSP2R.L An */
uae_u32 REGPARAM2 CPUFUNC(op_4e68_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{if (!regs.s) { Exception (8); goto endlabel2946; }
{{	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_areg (regs, srcreg) = (regs.usp);
}}}	m68k_incpc (2);
endlabel2946: ;
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* RESET.L  */
uae_u32 REGPARAM2 CPUFUNC(op_4e70_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2947; }
{	regs.ir = regs.irc;
	get_word_prefetch (4);
	cpureset ();
	m68k_incpc (2);
	get_word_prefetch (2);
}}endlabel2947: ;
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* NOP.L  */
uae_u32 REGPARAM2 CPUFUNC(op_4e71_11)(uae_u32 opcode)
{
{	regs.ir = regs.irc;
	get_word_prefetch (4);
}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* STOP.L #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_4e72_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2949; }
{	regs.sr = regs.irc;
	MakeFromSR ();
	m68k_setstopped ();
	m68k_incpc (4);
}}endlabel2949: ;
return 0 * CYCLE_UNIT / 2;
} /* 4 (0/0) */

/* RTE.L  */
uae_u32 REGPARAM2 CPUFUNC(op_4e73_11)(uae_u32 opcode)
{
{if (!regs.s) { Exception (8); goto endlabel2950; }
{{	uaecptr sra = m68k_areg (regs, 7);
	if (sra & 1) {
		exception3 (opcode, sra);
		goto endlabel2950;
	}
{{	uae_s16 sr = get_word (sra);
	m68k_areg (regs, 7) += 2;
{	uaecptr pca = m68k_areg (regs, 7);
	if (pca & 1) {
		exception3 (opcode, pca);
		goto endlabel2950;
	}
{{	uae_s32 pc = get_long (pca);
	m68k_areg (regs, 7) += 4;
	regs.sr = sr;
	m68k_setpc (pc);
	MakeFromSR ();
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}}}endlabel2950: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* RTD.L #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_4e74_11)(uae_u32 opcode)
{
{{	uaecptr pca = m68k_areg (regs, 7);
	if (pca & 1) {
		exception3 (opcode, pca);
		goto endlabel2951;
	}
{{	uae_s32 pc = get_long (pca);
	m68k_areg (regs, 7) += 4;
{	uae_s16 offs = get_word_prefetch (4);
	m68k_areg (regs, 7) += offs;
	if (pc & 1) {
		exception3i (0x4E74, pc);
		goto endlabel2951;
	}
	if (pc & 1) {
		exception3i (0x4E74, pc);
		goto endlabel2951;
	}
	m68k_setpc (pc);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}endlabel2951: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* RTS.L  */
uae_u32 REGPARAM2 CPUFUNC(op_4e75_11)(uae_u32 opcode)
{
{	uaecptr pc = m68k_getpc ();
	m68k_do_rts ();
	if (m68k_getpc () & 1) {
		uaecptr faultpc = m68k_getpc ();
		m68k_setpc (pc);
		exception3i (0x4E75, faultpc);
	}
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}return 8 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* TRAPV.L  */
uae_u32 REGPARAM2 CPUFUNC(op_4e76_11)(uae_u32 opcode)
{
{	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	if (GET_VFLG ()) {
		Exception (7);
		goto endlabel2953;
	}
}endlabel2953: ;
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* RTR.L  */
uae_u32 REGPARAM2 CPUFUNC(op_4e77_11)(uae_u32 opcode)
{
{	uaecptr oldpc = m68k_getpc ();
	MakeSR ();
{	uaecptr sra = m68k_areg (regs, 7);
	if (sra & 1) {
		exception3 (opcode, sra);
		goto endlabel2954;
	}
{{	uae_s16 sr = get_word (sra);
	m68k_areg (regs, 7) += 2;
{	uaecptr pca = m68k_areg (regs, 7);
	if (pca & 1) {
		exception3 (opcode, pca);
		goto endlabel2954;
	}
{{	uae_s32 pc = get_long (pca);
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
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}}}}}endlabel2954: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* JSR.L (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4e90_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uaecptr oldpc = m68k_getpc () + 2;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2955;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_long (m68k_areg (regs, 7), oldpc);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel2955: ;
return 8 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* JSR.L (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4ea8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)regs.irc;
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2956;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_long (m68k_areg (regs, 7), oldpc);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel2956: ;
return 8 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* JSR.L (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4eb0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), regs.irc);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2957;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_long (m68k_areg (regs, 7), oldpc);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel2957: ;
return 8 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* JSR.L (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4eb8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)regs.irc;
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2958;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_long (m68k_areg (regs, 7), oldpc);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel2958: ;
return 8 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* JSR.L (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4eb9_11)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_prefetch (4) << 16;
	srca |= regs.irc;
{	uaecptr oldpc = m68k_getpc () + 6;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2959;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_long (m68k_areg (regs, 7), oldpc);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel2959: ;
return 12 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* JSR.L (d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_4eba_11)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)regs.irc;
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2960;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_long (m68k_areg (regs, 7), oldpc);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel2960: ;
return 8 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* JSR.L (d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4ebb_11)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, regs.irc);
{	uaecptr oldpc = m68k_getpc () + 4;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2961;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	m68k_areg (regs, 7) -= 4;
	put_long (m68k_areg (regs, 7), oldpc);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel2961: ;
return 8 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* JMP.L (An) */
uae_u32 REGPARAM2 CPUFUNC(op_4ed0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2962;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel2962: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* JMP.L (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_4ee8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2963;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel2963: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* JMP.L (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4ef0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2964;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel2964: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* JMP.L (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_4ef8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2965;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel2965: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* JMP.L (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_4ef9_11)(uae_u32 opcode)
{
{{	uaecptr srca;
	srca = get_word_prefetch (4) << 16;
	srca |= regs.irc;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2966;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel2966: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* JMP.L (d16,PC) */
uae_u32 REGPARAM2 CPUFUNC(op_4efa_11)(uae_u32 opcode)
{
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)regs.irc;
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2967;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel2967: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* JMP.L (d8,PC,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_4efb_11)(uae_u32 opcode)
{
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3i (opcode, srca);
		goto endlabel2968;
	}
	m68k_setpc (srca);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel2968: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADDQ.B #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDQ.B #<data>,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADDQ.B #<data>,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADDQ.B #<data>,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADDQ.B #<data>,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_5028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADDQ.B #<data>,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_5030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADDQ.B #<data>,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_5038_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADDQ.B #<data>,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_5039_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ADDQ.W #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5040_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDAQ.W #<data>,An */
uae_u32 REGPARAM2 CPUFUNC(op_5048_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDQ.W #<data>,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5050_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2979;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel2979: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADDQ.W #<data>,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5058_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2980;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel2980: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADDQ.W #<data>,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5060_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2981;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel2981: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADDQ.W #<data>,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_5068_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2982;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2982: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADDQ.W #<data>,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_5070_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2983;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2983: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

#endif

#ifdef PART_5
/* ADDQ.W #<data>,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_5078_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2984;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2984: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADDQ.W #<data>,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_5079_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2985;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2985: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ADDQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDAQ.L #<data>,An */
uae_u32 REGPARAM2 CPUFUNC(op_5088_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDQ.L #<data>,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2988;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel2988: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* ADDQ.L #<data>,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2989;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel2989: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* ADDQ.L #<data>,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_50a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2990;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel2990: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* ADDQ.L #<data>,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_50a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2991;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2991: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* ADDQ.L #<data>,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_50b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2992;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2992: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* ADDQ.L #<data>,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_50b8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2993;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel2993: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* ADDQ.L #<data>,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_50b9_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel2994;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel2994: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_50c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (0) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_50c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (0)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel2996;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel2996: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_50d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (0) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_50d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (0) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_50e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (0) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_50e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (0) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_50f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (0) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_50f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (0) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_50f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (0) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUBQ.B #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBQ.B #<data>,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUBQ.B #<data>,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUBQ.B #<data>,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUBQ.B #<data>,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_5128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUBQ.B #<data>,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_5130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUBQ.B #<data>,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_5138_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUBQ.B #<data>,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_5139_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUBQ.W #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5140_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBAQ.W #<data>,An */
uae_u32 REGPARAM2 CPUFUNC(op_5148_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBQ.W #<data>,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3014;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3014: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUBQ.W #<data>,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3015;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3015: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUBQ.W #<data>,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3016;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3016: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUBQ.W #<data>,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_5168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3017;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3017: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUBQ.W #<data>,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_5170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3018;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3018: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUBQ.W #<data>,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_5178_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3019;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3019: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUBQ.W #<data>,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_5179_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3020;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3020: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUBQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5180_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBAQ.L #<data>,An */
uae_u32 REGPARAM2 CPUFUNC(op_5188_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBQ.L #<data>,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3023;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3023: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* SUBQ.L #<data>,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3024;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3024: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* SUBQ.L #<data>,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_51a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3025;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3025: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* SUBQ.L #<data>,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_51a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3026;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3026: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* SUBQ.L #<data>,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_51b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 src = srcreg;
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3027;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3027: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* SUBQ.L #<data>,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_51b8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3028;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3028: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* SUBQ.L #<data>,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_51b9_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
{{	uae_u32 src = srcreg;
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3029;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3029: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_51c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (1) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_51c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (1)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3031;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3031: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_51d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (1) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_51d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (1) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_51e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (1) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_51e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (1) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_51f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (1) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_51f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (1) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_51f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (1) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_52c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (2) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_52c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (2)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3040;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3040: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_52d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (2) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_52d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (2) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_52e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (2) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_52e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (2) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_52f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (2) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_52f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (2) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_52f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (2) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_53c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (3) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_53c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (3)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3049;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3049: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_53d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (3) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_53d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (3) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_53e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (3) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_53e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (3) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_53f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (3) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_53f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (3) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_53f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (3) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_54c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (4) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_54c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (4)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3058;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3058: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_54d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (4) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_54d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (4) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_54e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (4) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_54e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (4) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_54f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (4) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_54f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (4) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_54f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (4) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_55c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (5) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_55c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (5)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3067;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3067: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_55d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (5) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_55d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (5) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_55e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (5) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_55e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (5) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_55f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (5) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_55f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (5) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_55f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (5) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_56c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (6) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_56c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (6)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3076;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3076: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_56d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (6) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_56d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (6) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_56e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (6) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_56e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (6) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_56f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (6) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_56f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (6) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_56f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (6) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_57c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (7) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_57c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (7)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3085;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3085: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_57d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (7) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_57d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (7) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_57e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (7) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_57e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (7) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_57f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (7) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_57f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (7) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_57f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (7) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_58c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (8) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_58c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (8)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3094;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3094: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_58d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (8) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_58d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (8) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_58e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (8) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_58e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (8) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_58f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (8) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_58f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (8) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_58f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (8) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_59c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (9) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_59c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (9)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3103;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3103: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_59d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (9) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_59d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (9) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_59e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (9) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_59e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (9) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_59f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (9) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_59f8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (9) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_59f9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (9) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5ac0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (10) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_5ac8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (10)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3112;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3112: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_5ad0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (10) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5ad8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (10) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5ae0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (10) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_5ae8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (10) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_5af0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (10) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_5af8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (10) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_5af9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (10) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5bc0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (11) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_5bc8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (11)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3121;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3121: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_5bd0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (11) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5bd8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (11) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5be0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (11) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_5be8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (11) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_5bf0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (11) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_5bf8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (11) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_5bf9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (11) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5cc0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (12) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_5cc8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (12)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3130;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3130: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_5cd0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (12) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5cd8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (12) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5ce0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (12) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_5ce8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (12) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_5cf0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (12) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_5cf8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (12) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_5cf9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (12) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5dc0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (13) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_5dc8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (13)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3139;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3139: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_5dd0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (13) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5dd8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (13) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5de0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (13) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_5de8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (13) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_5df0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (13) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_5df8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (13) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_5df9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (13) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5ec0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (14) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_5ec8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (14)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3148;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3148: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_5ed0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (14) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5ed8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (14) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5ee0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (14) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_5ee8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (14) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_5ef0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (14) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_5ef8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (14) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_5ef9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (14) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Scc.B Dn */
uae_u32 REGPARAM2 CPUFUNC(op_5fc0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (15) ? 0xff : 0;
	m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xff) | ((val) & 0xff);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* DBcc.W Dn,#<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_5fc8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 offs = regs.irc;
	uaecptr oldpc = m68k_getpc ();
	if (!cctrue (15)) {
	m68k_incpc ((uae_s32)offs + 2);
		get_word_prefetch (0);
		m68k_dreg (regs, srcreg) = (m68k_dreg (regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
		if (src) {
			if (offs & 1) {
				exception3i (opcode, m68k_getpc () + 2 + (uae_s32)offs + 2);
				goto endlabel3157;
			}
	regs.ir = regs.irc;
	get_word_prefetch (2);
			return 12 * CYCLE_UNIT / 2;
		}
	} else {
	}
	m68k_setpc (oldpc + 4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}endlabel3157: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* Scc.B (An) */
uae_u32 REGPARAM2 CPUFUNC(op_5fd0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (15) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_5fd8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (15) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_5fe0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	regs.ir = regs.irc;
	get_word_prefetch (4);
{	int val = cctrue (15) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* Scc.B (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_5fe8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (15) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_5ff0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (15) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_5ff8_11)(uae_u32 opcode)
{
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (6);
{	int val = cctrue (15) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* Scc.B (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_5ff9_11)(uae_u32 opcode)
{
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	regs.ir = regs.irc;
	get_word_prefetch (8);
{	int val = cctrue (15) ? 0xff : 0;
	put_byte (srca, val);
}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6000_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3165;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3165: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6001_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3166;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3166: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_60ff_11)(uae_u32 opcode)
{
{	if (cctrue (0)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3167;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3167;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (0)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3167;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3167: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* BSR.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6100_11)(uae_u32 opcode)
{
{	uae_s32 s;
{	uae_s16 src = regs.irc;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3 (opcode, m68k_getpc () + s, 0, 1, m68k_getpc () + s);
		goto endlabel3168;
	}
	m68k_do_bsr (m68k_getpc () + 4, s);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3168: ;
return 8 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* BSRQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6101_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{	uae_s32 s;
{	uae_u32 src = srcreg;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3 (opcode, m68k_getpc () + s, 0, 1, m68k_getpc () + s);
		goto endlabel3169;
	}
	m68k_do_bsr (m68k_getpc () + 2, s);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3169: ;
return 8 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

/* BSR.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_61ff_11)(uae_u32 opcode)
{
{	uae_s32 s;
	uae_u32 src = 0xffffffff;
	s = (uae_s32)src + 2;
	if (src & 1) {
		exception3 (opcode, m68k_getpc () + s, 0, 1, m68k_getpc () + s);
		goto endlabel3170;
	}
	m68k_do_bsr (m68k_getpc () + 2, s);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}endlabel3170: ;
return 8 * CYCLE_UNIT / 2;
} /* 16 (2/2) */

#endif

#ifdef PART_6
/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6200_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3171;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3171: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6201_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3172;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3172: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_62ff_11)(uae_u32 opcode)
{
{	if (cctrue (2)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3173;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3173;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (2)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3173;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3173: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6300_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3174;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3174: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6301_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3175;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3175: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_63ff_11)(uae_u32 opcode)
{
{	if (cctrue (3)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3176;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3176;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (3)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3176;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3176: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6400_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3177;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3177: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6401_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3178;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3178: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_64ff_11)(uae_u32 opcode)
{
{	if (cctrue (4)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3179;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3179;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (4)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3179;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3179: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6500_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3180;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3180: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6501_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3181;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3181: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_65ff_11)(uae_u32 opcode)
{
{	if (cctrue (5)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3182;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3182;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (5)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3182;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3182: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6600_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3183;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3183: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6601_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3184;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3184: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_66ff_11)(uae_u32 opcode)
{
{	if (cctrue (6)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3185;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3185;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (6)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3185;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3185: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6700_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3186;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3186: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6701_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3187;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3187: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_67ff_11)(uae_u32 opcode)
{
{	if (cctrue (7)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3188;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3188;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (7)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3188;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3188: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6800_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3189;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3189: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6801_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3190;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3190: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_68ff_11)(uae_u32 opcode)
{
{	if (cctrue (8)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3191;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3191;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (8)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3191;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3191: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6900_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3192;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3192: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6901_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3193;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3193: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_69ff_11)(uae_u32 opcode)
{
{	if (cctrue (9)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3194;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3194;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (9)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3194;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3194: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6a00_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3195;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3195: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6a01_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3196;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3196: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_6aff_11)(uae_u32 opcode)
{
{	if (cctrue (10)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3197;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3197;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (10)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3197;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3197: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6b00_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3198;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3198: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6b01_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3199;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3199: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_6bff_11)(uae_u32 opcode)
{
{	if (cctrue (11)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3200;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3200;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (11)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3200;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3200: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6c00_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3201;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3201: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6c01_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3202;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3202: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_6cff_11)(uae_u32 opcode)
{
{	if (cctrue (12)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3203;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3203;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (12)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3203;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3203: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6d00_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3204;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3204: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6d01_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3205;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3205: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_6dff_11)(uae_u32 opcode)
{
{	if (cctrue (13)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3206;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3206;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (13)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3206;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3206: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6e00_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3207;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3207: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6e01_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3208;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3208: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_6eff_11)(uae_u32 opcode)
{
{	if (cctrue (14)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3209;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3209;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (14)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3209;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3209: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* Bcc.W #<data>.W */
uae_u32 REGPARAM2 CPUFUNC(op_6f00_11)(uae_u32 opcode)
{
{{	uae_s16 src = regs.irc;
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3210;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3210: ;
return 12 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* BccQ.B #<data> */
uae_u32 REGPARAM2 CPUFUNC(op_6f01_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
{{	uae_u32 src = srcreg;
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3211;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3211: ;
return 8 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* Bcc.L #<data>.L */
uae_u32 REGPARAM2 CPUFUNC(op_6fff_11)(uae_u32 opcode)
{
{	if (cctrue (15)) {
		exception3i (opcode, m68k_getpc () + 1);
		goto endlabel3212;
	}
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	goto endlabel3212;
{	uae_s32 src;
	src = get_word_prefetch (2) << 16;
	src |= regs.irc;
	if (!cctrue (15)) goto didnt_jump;
	if (src & 1) {
		exception3i (opcode, m68k_getpc () + 2 + (uae_s32)src);
		goto endlabel3212;
	}
	m68k_incpc ((uae_s32)src + 2);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
	return 10 * CYCLE_UNIT / 2;
didnt_jump:;
	m68k_incpc (4);
	get_word_prefetch (0);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}endlabel3212: ;
return 12 * CYCLE_UNIT / 2;
} /* 24 (6/0) */

/* MOVEQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_7000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_u32 src = srcreg;
{	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	m68k_dreg (regs, dstreg) = (src);
	m68k_incpc (2);
	regs.ir = regs.irc;
	get_word_prefetch (2);
}}}return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* OR.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* OR.B (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* OR.B (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* OR.B -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* OR.B (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.B (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.B (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8038_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.B (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8039_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* OR.B (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_803a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.B (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_803b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_803c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* OR.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8040_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* OR.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8050_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3226;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel3226: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* OR.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8058_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3227;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel3227: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* OR.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8060_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3228;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel3228: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* OR.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8068_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3229;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel3229: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8070_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3230;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel3230: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8078_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3231;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel3231: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8079_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3232;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (6);
endlabel3232: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* OR.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_807a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3233;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel3233: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_807b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3234;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel3234: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_807c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* OR.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* OR.L (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3237;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel3237: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.L (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3238;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel3238: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.L -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3239;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel3239: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* OR.L (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3240;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel3240: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* OR.L (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3241;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel3241: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* OR.L (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3242;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel3242: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* OR.L (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3243;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (6);
endlabel3243: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* OR.L (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3244;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel3244: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* OR.L (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3245;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel3245: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* OR.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (src);
}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* DIVU.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (2);
		Exception (5);
		goto endlabel3247;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}endlabel3247: ;
return 110 * CYCLE_UNIT / 2;
} /* 4+ (1/0) */

/* DIVU.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3248;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (2);
		Exception (5);
		goto endlabel3248;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}endlabel3248: ;
return 114 * CYCLE_UNIT / 2;
} /* 8+ (2/0) */

/* DIVU.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3249;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (2);
		Exception (5);
		goto endlabel3249;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}endlabel3249: ;
return 114 * CYCLE_UNIT / 2;
} /* 8+ (2/0) */

/* DIVU.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3250;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (2);
		Exception (5);
		goto endlabel3250;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}endlabel3250: ;
return 116 * CYCLE_UNIT / 2;
} /* 8+ (2/0) */

/* DIVU.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3251;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3251;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}endlabel3251: ;
return 118 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* DIVU.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3252;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3252;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}endlabel3252: ;
return 120 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* DIVU.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3253;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3253;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}endlabel3253: ;
return 118 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* DIVU.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3254;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (6);
		Exception (5);
		goto endlabel3254;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}endlabel3254: ;
return 122 * CYCLE_UNIT / 2;
} /* 16+ (4/0) */

/* DIVU.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3255;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3255;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}endlabel3255: ;
return 118 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* DIVU.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3256;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3256;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}endlabel3256: ;
return 120 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* DIVU.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_80fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3257;
	} else {
		uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
		uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}endlabel3257: ;
return 114 * CYCLE_UNIT / 2;
} /* 8+ (2/0) */

/* SBCD.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_8100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SBCD.B -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_8108_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
	put_byte (dsta, newv);
}}}}}}	m68k_incpc (2);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.B Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_8110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_byte (dsta, src);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* OR.B Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_8118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_byte (dsta, src);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* OR.B Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_8120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_byte (dsta, src);
}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* OR.B Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_8128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.B Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_8130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
{	uae_s8 dst = get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.B Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_8138_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.B Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_8139_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* OR.W Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_8150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3267;
	}
{{	uae_s16 dst = get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_word (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3267: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* OR.W Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_8158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3268;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_word (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3268: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* OR.W Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_8160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3269;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_word (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3269: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* OR.W Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_8168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3270;
	}
{{	uae_s16 dst = get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3270: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.W Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_8170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3271;
	}
{{	uae_s16 dst = get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3271: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.W Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_8178_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3272;
	}
{{	uae_s16 dst = get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3272: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* OR.W Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_8179_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3273;
	}
{{	uae_s16 dst = get_word (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel3273: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* OR.L Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_8190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3274;
	}
{{	uae_s32 dst = get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_long (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3274: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* OR.L Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_8198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3275;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_long (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3275: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* OR.L Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_81a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3276;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_long (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3276: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* OR.L Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_81a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3277;
	}
{{	uae_s32 dst = get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3277: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* OR.L Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_81b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3278;
	}
{{	uae_s32 dst = get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3278: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* OR.L Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_81b8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3279;
	}
{{	uae_s32 dst = get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3279: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* OR.L Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_81b9_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3280;
	}
{{	uae_s32 dst = get_long (dsta);
	src |= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel3280: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* DIVS.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (2);
		Exception (5);
		goto endlabel3281;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}endlabel3281: ;
return 142 * CYCLE_UNIT / 2;
} /* 4+ (1/0) */

/* DIVS.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3282;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (2);
		Exception (5);
		goto endlabel3282;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}endlabel3282: ;
return 146 * CYCLE_UNIT / 2;
} /* 8+ (2/0) */

/* DIVS.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3283;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (2);
		Exception (5);
		goto endlabel3283;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}endlabel3283: ;
return 146 * CYCLE_UNIT / 2;
} /* 8+ (2/0) */

/* DIVS.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3284;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (2);
		Exception (5);
		goto endlabel3284;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}endlabel3284: ;
return 148 * CYCLE_UNIT / 2;
} /* 8+ (2/0) */

/* DIVS.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3285;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3285;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}endlabel3285: ;
return 150 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* DIVS.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3286;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3286;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}endlabel3286: ;
return 152 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* DIVS.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3287;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3287;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}endlabel3287: ;
return 150 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* DIVS.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3288;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (6);
		Exception (5);
		goto endlabel3288;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}endlabel3288: ;
return 154 * CYCLE_UNIT / 2;
} /* 16+ (4/0) */

/* DIVS.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3289;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3289;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}endlabel3289: ;
return 150 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* DIVS.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3290;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3290;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}endlabel3290: ;
return 152 * CYCLE_UNIT / 2;
} /* 12+ (3/0) */

/* DIVS.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_81fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	CLEAR_CZNV ();
	if (src == 0) {
	m68k_incpc (4);
		Exception (5);
		goto endlabel3291;
	} else {
		uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
		uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}endlabel3291: ;
return 146 * CYCLE_UNIT / 2;
} /* 8+ (2/0) */

/* SUB.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUB.B (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUB.B (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUB.B -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}	m68k_incpc (2);
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUB.B (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.B (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.B (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9038_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.B (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9039_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUB.B (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_903a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.B (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_903b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_903c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUB.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9040_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUB.W An,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9048_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUB.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9050_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3305;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3305: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUB.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9058_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3306;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3306: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUB.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9060_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3307;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3307: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUB.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9068_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3308;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3308: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9070_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3309;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3309: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9078_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3310;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3310: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9079_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3311;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}}}}	m68k_incpc (6);
endlabel3311: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUB.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_907a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3312;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3312: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_907b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3313;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3313: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_907c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUB.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUB.L An,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9088_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUB.L (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3317;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3317: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.L (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3318;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3318: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.L -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_90a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3319;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3319: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUB.L (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_90a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3320;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3320: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUB.L (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_90b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3321;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3321: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUB.L (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_90b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3322;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3322: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUB.L (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_90b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3323;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}}}}	m68k_incpc (6);
endlabel3323: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* SUB.L (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_90ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3324;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3324: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUB.L (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_90bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3325;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3325: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUB.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_90bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUBA.W Dn,An */
uae_u32 REGPARAM2 CPUFUNC(op_90c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBA.W An,An */
uae_u32 REGPARAM2 CPUFUNC(op_90c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBA.W (An),An */
uae_u32 REGPARAM2 CPUFUNC(op_90d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3329;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3329: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUBA.W (An)+,An */
uae_u32 REGPARAM2 CPUFUNC(op_90d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3330;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3330: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUBA.W -(An),An */
uae_u32 REGPARAM2 CPUFUNC(op_90e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3331;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3331: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUBA.W (d16,An),An */
uae_u32 REGPARAM2 CPUFUNC(op_90e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3332;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3332: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUBA.W (d8,An,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_90f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3333;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3333: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUBA.W (xxx).W,An */
uae_u32 REGPARAM2 CPUFUNC(op_90f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3334;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3334: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUBA.W (xxx).L,An */
uae_u32 REGPARAM2 CPUFUNC(op_90f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3335;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel3335: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUBA.W (d16,PC),An */
uae_u32 REGPARAM2 CPUFUNC(op_90fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3336;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3336: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUBA.W (d8,PC,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_90fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3337;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3337: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUBA.W #<data>.W,An */
uae_u32 REGPARAM2 CPUFUNC(op_90fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* SUBX.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBX.B -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_9108_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.B Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_9110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUB.B Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_9118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUB.B Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_9120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUB.B Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_9128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.B Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_9130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.B Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_9138_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.B Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_9139_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUBX.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9140_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBX.W -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_9148_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3349;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3349;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word (dsta, newv);
}}}}}}}}}	m68k_incpc (2);
endlabel3349: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.W Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_9150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3350;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3350: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUB.W Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_9158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3351;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3351: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUB.W Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_9160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3352;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3352: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* SUB.W Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_9168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3353;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3353: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.W Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_9170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3354;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3354: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.W Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_9178_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3355;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3355: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* SUB.W Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_9179_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3356;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3356: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* SUBX.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_9180_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBX.L -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_9188_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3358;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3358;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src - (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgo) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long (dsta, newv);
}}}}}}}}}	m68k_incpc (2);
endlabel3358: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* SUB.L Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_9190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3359;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3359: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* SUB.L Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_9198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3360;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3360: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* SUB.L Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_91a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3361;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3361: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* SUB.L Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_91a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3362;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3362: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* SUB.L Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_91b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3363;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3363: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* SUB.L Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_91b8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3364;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3364: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* SUB.L Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_91b9_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3365;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgo) & (flgn ^ flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3365: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* SUBA.L Dn,An */
uae_u32 REGPARAM2 CPUFUNC(op_91c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBA.L An,An */
uae_u32 REGPARAM2 CPUFUNC(op_91c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* SUBA.L (An),An */
uae_u32 REGPARAM2 CPUFUNC(op_91d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3368;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3368: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUBA.L (An)+,An */
uae_u32 REGPARAM2 CPUFUNC(op_91d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3369;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3369: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUBA.L -(An),An */
uae_u32 REGPARAM2 CPUFUNC(op_91e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3370;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3370: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* SUBA.L (d16,An),An */
uae_u32 REGPARAM2 CPUFUNC(op_91e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3371;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3371: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUBA.L (d8,An,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_91f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3372;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3372: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUBA.L (xxx).W,An */
uae_u32 REGPARAM2 CPUFUNC(op_91f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3373;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3373: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUBA.L (xxx).L,An */
uae_u32 REGPARAM2 CPUFUNC(op_91f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3374;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel3374: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* SUBA.L (d16,PC),An */
uae_u32 REGPARAM2 CPUFUNC(op_91fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3375;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3375: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUBA.L (d8,PC,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_91fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3376;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3376: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* SUBA.L #<data>.L,An */
uae_u32 REGPARAM2 CPUFUNC(op_91fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = dst - src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMP.B (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMP.B (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMP.B -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (2);
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMP.B (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.B (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.B (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b038_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.B (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b039_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.B (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b03a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.B (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b03b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b03c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMP.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b040_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMP.W An,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b048_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMP.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b050_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3391;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3391: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMP.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b058_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3392;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3392: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMP.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b060_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3393;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3393: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMP.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b068_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3394;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3394: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b070_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3395;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3395: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b078_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3396;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3396: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b079_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3397;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3397: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b07a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3398;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3398: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b07b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3399;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3399: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b07c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMP.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMP.L An,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b088_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

#endif

#ifdef PART_7
/* CMP.L (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3403;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3403: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.L (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3404;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3404: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.L -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b0a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3405;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3405: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMP.L (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b0a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3406;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3406: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.L (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b0b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3407;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3407: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.L (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b0b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3408;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3408: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.L (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b0b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3409;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3409: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* CMP.L (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b0ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3410;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3410: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.L (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b0bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3411;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3411: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMP.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b0bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMPA.W Dn,An */
uae_u32 REGPARAM2 CPUFUNC(op_b0c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMPA.W An,An */
uae_u32 REGPARAM2 CPUFUNC(op_b0c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMPA.W (An),An */
uae_u32 REGPARAM2 CPUFUNC(op_b0d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3415;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3415: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMPA.W (An)+,An */
uae_u32 REGPARAM2 CPUFUNC(op_b0d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3416;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3416: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMPA.W -(An),An */
uae_u32 REGPARAM2 CPUFUNC(op_b0e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3417;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3417: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* CMPA.W (d16,An),An */
uae_u32 REGPARAM2 CPUFUNC(op_b0e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3418;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3418: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMPA.W (d8,An,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_b0f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3419;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3419: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMPA.W (xxx).W,An */
uae_u32 REGPARAM2 CPUFUNC(op_b0f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3420;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3420: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMPA.W (xxx).L,An */
uae_u32 REGPARAM2 CPUFUNC(op_b0f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3421;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3421: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMPA.W (d16,PC),An */
uae_u32 REGPARAM2 CPUFUNC(op_b0fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3422;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3422: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMPA.W (d8,PC,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_b0fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3423;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3423: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMPA.W #<data>.W,An */
uae_u32 REGPARAM2 CPUFUNC(op_b0fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* EOR.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMPM.B (An)+,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_b108_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u8)(src)) > ((uae_u8)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* EOR.B Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_b110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_byte (dsta, src);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* EOR.B Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_b118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_byte (dsta, src);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* EOR.B Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_b120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_byte (dsta, src);
}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* EOR.B Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_b128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.B Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_b130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
{	uae_s8 dst = get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.B Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_b138_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.B Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_b139_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EOR.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b140_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMPM.W (An)+,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_b148_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3435;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3435;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u16)(src)) > ((uae_u16)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}}}	m68k_incpc (2);
endlabel3435: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* EOR.W Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_b150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3436;
	}
{{	uae_s16 dst = get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_word (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3436: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* EOR.W Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_b158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3437;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_word (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3437: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* EOR.W Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_b160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3438;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_word (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3438: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* EOR.W Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_b168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3439;
	}
{{	uae_s16 dst = get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3439: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.W Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_b170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3440;
	}
{{	uae_s16 dst = get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3440: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.W Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_b178_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3441;
	}
{{	uae_s16 dst = get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3441: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* EOR.W Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_b179_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3442;
	}
{{	uae_s16 dst = get_word (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel3442: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EOR.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_b180_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMPM.L (An)+,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_b188_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3444;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3444;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}}}	m68k_incpc (2);
endlabel3444: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* EOR.L Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_b190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3445;
	}
{{	uae_s32 dst = get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_long (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3445: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* EOR.L Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_b198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3446;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_long (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3446: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* EOR.L Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_b1a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3447;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_long (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3447: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* EOR.L Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_b1a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3448;
	}
{{	uae_s32 dst = get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3448: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* EOR.L Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_b1b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3449;
	}
{{	uae_s32 dst = get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3449: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* EOR.L Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_b1b8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3450;
	}
{{	uae_s32 dst = get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3450: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* EOR.L Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_b1b9_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3451;
	}
{{	uae_s32 dst = get_long (dsta);
	src ^= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel3451: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* CMPA.L Dn,An */
uae_u32 REGPARAM2 CPUFUNC(op_b1c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMPA.L An,An */
uae_u32 REGPARAM2 CPUFUNC(op_b1c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* CMPA.L (An),An */
uae_u32 REGPARAM2 CPUFUNC(op_b1d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3454;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3454: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMPA.L (An)+,An */
uae_u32 REGPARAM2 CPUFUNC(op_b1d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3455;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3455: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMPA.L -(An),An */
uae_u32 REGPARAM2 CPUFUNC(op_b1e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3456;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (2);
endlabel3456: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* CMPA.L (d16,An),An */
uae_u32 REGPARAM2 CPUFUNC(op_b1e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3457;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3457: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMPA.L (d8,An,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_b1f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3458;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3458: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMPA.L (xxx).W,An */
uae_u32 REGPARAM2 CPUFUNC(op_b1f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3459;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3459: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMPA.L (xxx).L,An */
uae_u32 REGPARAM2 CPUFUNC(op_b1f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3460;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (6);
endlabel3460: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* CMPA.L (d16,PC),An */
uae_u32 REGPARAM2 CPUFUNC(op_b1fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3461;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3461: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMPA.L (d8,PC,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_b1fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3462;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}}}	m68k_incpc (4);
endlabel3462: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* CMPA.L #<data>.L,An */
uae_u32 REGPARAM2 CPUFUNC(op_b1fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs != flgo) && (flgn != flgo));
	SET_CFLG (((uae_u32)(src)) > ((uae_u32)(dst)));
	SET_NFLG (flgn != 0);
}}}}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* AND.B (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* AND.B (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* AND.B -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (2);
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* AND.B (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.B (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.B (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c038_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.B (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c039_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* AND.B (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c03a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.B (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c03b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c03c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xff) | ((src) & 0xff);
}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* AND.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c040_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* AND.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c050_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3476;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel3476: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* AND.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c058_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3477;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel3477: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* AND.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c060_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3478;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (2);
endlabel3478: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* AND.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c068_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3479;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel3479: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c070_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3480;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel3480: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c078_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3481;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel3481: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c079_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3482;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (6);
endlabel3482: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* AND.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c07a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3483;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel3483: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c07b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3484;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}}}	m68k_incpc (4);
endlabel3484: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c07c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (m68k_dreg (regs, dstreg) & ~0xffff) | ((src) & 0xffff);
}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* AND.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* AND.L (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3487;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel3487: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.L (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3488;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel3488: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.L -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3489;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (2);
endlabel3489: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* AND.L (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3490;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel3490: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* AND.L (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3491;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel3491: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* AND.L (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3492;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel3492: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* AND.L (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3493;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (6);
endlabel3493: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* AND.L (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3494;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel3494: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* AND.L (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3495;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	m68k_dreg (regs, dstreg) = (src);
}}}}}	m68k_incpc (4);
endlabel3495: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* AND.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	m68k_dreg (regs, dstreg) = (src);
}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* MULU.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}return 58 * CYCLE_UNIT / 2;
} /* 38+ (1/0) */

/* MULU.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3498;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}}endlabel3498: ;
return 62 * CYCLE_UNIT / 2;
} /* 42+ (2/0) */

/* MULU.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3499;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}}endlabel3499: ;
return 62 * CYCLE_UNIT / 2;
} /* 42+ (2/0) */

/* MULU.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3500;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (2);
}}}}}}endlabel3500: ;
return 64 * CYCLE_UNIT / 2;
} /* 42+ (2/0) */

/* MULU.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3501;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel3501: ;
return 66 * CYCLE_UNIT / 2;
} /* 46+ (3/0) */

/* MULU.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3502;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel3502: ;
return 68 * CYCLE_UNIT / 2;
} /* 46+ (3/0) */

/* MULU.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3503;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel3503: ;
return 66 * CYCLE_UNIT / 2;
} /* 46+ (3/0) */

/* MULU.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3504;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (6);
}}}}}}endlabel3504: ;
return 70 * CYCLE_UNIT / 2;
} /* 50+ (4/0) */

/* MULU.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3505;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel3505: ;
return 66 * CYCLE_UNIT / 2;
} /* 46+ (3/0) */

/* MULU.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3506;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}}}endlabel3506: ;
return 68 * CYCLE_UNIT / 2;
} /* 46+ (3/0) */

/* MULU.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c0fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
	m68k_incpc (4);
}}}}return 62 * CYCLE_UNIT / 2;
} /* 42+ (2/0) */

/* ABCD.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ABCD.B -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_c108_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
	put_byte (dsta, newv);
}}}}}}	m68k_incpc (2);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.B Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_c110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_byte (dsta, src);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* AND.B Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_c118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_byte (dsta, src);
}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* AND.B Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_c120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_byte (dsta, src);
}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* AND.B Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_c128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.B Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_c130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
{	uae_s8 dst = get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.B Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_c138_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_byte (dsta, src);
}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.B Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_c139_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s8)(src)) == 0);
	SET_NFLG   (((uae_s8)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_byte (dsta, src);
}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EXG.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c140_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, srcreg) = (dst);
	m68k_dreg (regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* EXG.L An,An */
uae_u32 REGPARAM2 CPUFUNC(op_c148_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_areg (regs, srcreg) = (dst);
	m68k_areg (regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* AND.W Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_c150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3519;
	}
{{	uae_s16 dst = get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_word (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3519: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* AND.W Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_c158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3520;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_word (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3520: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* AND.W Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_c160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3521;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_word (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3521: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* AND.W Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_c168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3522;
	}
{{	uae_s16 dst = get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3522: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.W Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_c170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3523;
	}
{{	uae_s16 dst = get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3523: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.W Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_c178_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3524;
	}
{{	uae_s16 dst = get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_word (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3524: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* AND.W Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_c179_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3525;
	}
{{	uae_s16 dst = get_word (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(src)) == 0);
	SET_NFLG   (((uae_s16)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_word (dsta, src);
}}}}}	m68k_incpc (6);
endlabel3525: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* EXG.L Dn,An */
uae_u32 REGPARAM2 CPUFUNC(op_c188_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	m68k_dreg (regs, srcreg) = (dst);
	m68k_areg (regs, dstreg) = (src);
}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* AND.L Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_c190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3527;
	}
{{	uae_s32 dst = get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_long (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3527: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* AND.L Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_c198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3528;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_long (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3528: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* AND.L Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_c1a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3529;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (4);
	put_long (dsta, src);
}}}}}	m68k_incpc (2);
endlabel3529: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* AND.L Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_c1a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3530;
	}
{{	uae_s32 dst = get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3530: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* AND.L Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_c1b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3531;
	}
{{	uae_s32 dst = get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3531: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* AND.L Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_c1b8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3532;
	}
{{	uae_s32 dst = get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (6);
	put_long (dsta, src);
}}}}}	m68k_incpc (4);
endlabel3532: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* AND.L Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_c1b9_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3533;
	}
{{	uae_s32 dst = get_long (dsta);
	src &= dst;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(src)) == 0);
	SET_NFLG   (((uae_s32)(src)) < 0);
	regs.ir = regs.irc;
	get_word_prefetch (8);
	put_long (dsta, src);
}}}}}	m68k_incpc (6);
endlabel3533: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* MULS.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 58 * CYCLE_UNIT / 2;
} /* 38+ (1/0) */

/* MULS.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3535;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3535: ;
return 62 * CYCLE_UNIT / 2;
} /* 42+ (2/0) */

/* MULS.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3536;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3536: ;
return 62 * CYCLE_UNIT / 2;
} /* 42+ (2/0) */

/* MULS.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3537;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3537: ;
return 64 * CYCLE_UNIT / 2;
} /* 42+ (2/0) */

/* MULS.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3538;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3538: ;
return 66 * CYCLE_UNIT / 2;
} /* 46+ (3/0) */

/* MULS.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3539;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3539: ;
return 68 * CYCLE_UNIT / 2;
} /* 46+ (3/0) */

/* MULS.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3540;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3540: ;
return 66 * CYCLE_UNIT / 2;
} /* 46+ (3/0) */

/* MULS.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3541;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel3541: ;
return 70 * CYCLE_UNIT / 2;
} /* 50+ (4/0) */

/* MULS.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3542;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3542: ;
return 66 * CYCLE_UNIT / 2;
} /* 46+ (3/0) */

/* MULS.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3543;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3543: ;
return 68 * CYCLE_UNIT / 2;
} /* 46+ (3/0) */

/* MULS.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_c1fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s32)(newv)) == 0);
	SET_NFLG   (((uae_s32)(newv)) < 0);
	m68k_dreg (regs, dstreg) = (newv);
}}}}	m68k_incpc (4);
return 62 * CYCLE_UNIT / 2;
} /* 42+ (2/0) */

/* ADD.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADD.B (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADD.B (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) += areg_byteinc[srcreg];
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}	m68k_incpc (2);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADD.B -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}	m68k_incpc (2);
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADD.B (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.B (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.B (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d038_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.B (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d039_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}}}	m68k_incpc (6);
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADD.B (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d03a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.B (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d03b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
{	uae_s8 src = get_byte (srca);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}	m68k_incpc (4);
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.B #<data>.B,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d03c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = (uae_u8)get_word_prefetch (4);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}	m68k_incpc (4);
return 12 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADD.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d040_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADD.W An,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d048_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADD.W (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d050_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3558;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3558: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADD.W (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d058_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3559;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3559: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADD.W -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d060_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3560;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3560: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADD.W (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d068_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3561;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3561: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.W (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d070_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3562;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3562: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.W (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d078_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3563;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3563: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.W (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d079_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3564;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}}}}	m68k_incpc (6);
endlabel3564: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADD.W (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d07a_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3565;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3565: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.W (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d07b_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3566;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3566: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.W #<data>.W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d07c_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADD.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADD.L An,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d088_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADD.L (An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3570;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3570: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.L (An)+,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3571;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3571: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.L -(An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d0a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3572;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}}}}	m68k_incpc (2);
endlabel3572: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADD.L (d16,An),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d0a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3573;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3573: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADD.L (d8,An,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d0b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3574;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3574: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADD.L (xxx).W,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d0b8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3575;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3575: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADD.L (xxx).L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d0b9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3576;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}}}}	m68k_incpc (6);
endlabel3576: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* ADD.L (d16,PC),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d0ba_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3577;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3577: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADD.L (d8,PC,Xn),Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d0bb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3578;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
}}}}}}}}	m68k_incpc (4);
endlabel3578: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADD.L #<data>.L,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d0bc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
}}}}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADDA.W Dn,An */
uae_u32 REGPARAM2 CPUFUNC(op_d0c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDA.W An,An */
uae_u32 REGPARAM2 CPUFUNC(op_d0c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDA.W (An),An */
uae_u32 REGPARAM2 CPUFUNC(op_d0d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3582;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3582: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADDA.W (An)+,An */
uae_u32 REGPARAM2 CPUFUNC(op_d0d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3583;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) += 2;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3583: ;
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADDA.W -(An),An */
uae_u32 REGPARAM2 CPUFUNC(op_d0e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3584;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3584: ;
return 10 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADDA.W (d16,An),An */
uae_u32 REGPARAM2 CPUFUNC(op_d0e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3585;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3585: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADDA.W (d8,An,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_d0f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3586;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3586: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADDA.W (xxx).W,An */
uae_u32 REGPARAM2 CPUFUNC(op_d0f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3587;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3587: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADDA.W (xxx).L,An */
uae_u32 REGPARAM2 CPUFUNC(op_d0f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3588;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel3588: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADDA.W (d16,PC),An */
uae_u32 REGPARAM2 CPUFUNC(op_d0fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3589;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3589: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADDA.W (d8,PC,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_d0fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3590;
	}
{{	uae_s16 src = get_word (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3590: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADDA.W #<data>.W,An */
uae_u32 REGPARAM2 CPUFUNC(op_d0fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = get_word_prefetch (4);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (4);
return 8 * CYCLE_UNIT / 2;
} /* 8 (2/0) */

/* ADDX.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uae_s8 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDX.B -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_d108_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - areg_byteinc[srcreg];
{	uae_s8 src = get_byte (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src + (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s8)(newv)) == 0));
	SET_NFLG (((uae_s8)(newv)) < 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.B Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_d110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADD.B Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_d118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) += areg_byteinc[dstreg];
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADD.B Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_d120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - areg_byteinc[dstreg];
{	uae_s8 dst = get_byte (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (2);
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADD.B Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_d128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.B Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_d130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.B Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_d138_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (4);
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.B Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_d139_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s8 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
{	uae_s8 dst = get_byte (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
{	int flgs = ((uae_s8)(src)) < 0;
	int flgo = ((uae_s8)(dst)) < 0;
	int flgn = ((uae_s8)(newv)) < 0;
	SET_ZFLG (((uae_s8)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u8)(~dst)) < ((uae_u8)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_byte (dsta, newv);
}}}}}}}	m68k_incpc (6);
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ADDX.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d140_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uae_s16 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDX.W -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_d148_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 2;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3602;
	}
{{	uae_s16 src = get_word (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3602;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src + (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s16)(newv)) == 0));
	SET_NFLG (((uae_s16)(newv)) < 0);
	put_word (dsta, newv);
}}}}}}}}}	m68k_incpc (2);
endlabel3602: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.W Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_d150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3603;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3603: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADD.W Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_d158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3604;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3604: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADD.W Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_d160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 2;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3605;
	}
{{	uae_s16 dst = get_word (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3605: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ADD.W Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_d168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3606;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3606: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.W Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_d170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3607;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3607: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.W Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_d178_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3608;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3608: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ADD.W Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_d179_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s16 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3609;
	}
{{	uae_s16 dst = get_word (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
{	int flgs = ((uae_s16)(src)) < 0;
	int flgo = ((uae_s16)(dst)) < 0;
	int flgn = ((uae_s16)(newv)) < 0;
	SET_ZFLG (((uae_s16)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u16)(~dst)) < ((uae_u16)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_word (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3609: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ADDX.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_d180_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDX.L -(An),-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_d188_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3611;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3611;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src + (GET_XFLG () ? 1 : 0);
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
	COPY_CARRY ();
	SET_ZFLG (GET_ZFLG () & (((uae_s32)(newv)) == 0));
	SET_NFLG (((uae_s32)(newv)) < 0);
	put_long (dsta, newv);
}}}}}}}}}	m68k_incpc (2);
endlabel3611: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* ADD.L Dn,(An) */
uae_u32 REGPARAM2 CPUFUNC(op_d190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3612;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3612: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* ADD.L Dn,(An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_d198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3613;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) += 4;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3613: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* ADD.L Dn,-(An) */
uae_u32 REGPARAM2 CPUFUNC(op_d1a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = m68k_areg (regs, dstreg) - 4;
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3614;
	}
{{	uae_s32 dst = get_long (dsta);
	m68k_areg (regs, dstreg) = dsta;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (2);
endlabel3614: ;
return 22 * CYCLE_UNIT / 2;
} /* 20 (3/2) */

/* ADD.L Dn,(d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_d1a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = m68k_areg (regs, dstreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3615;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3615: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* ADD.L Dn,(d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_d1b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta;
	dsta = get_disp_ea_000 (m68k_areg (regs, dstreg), get_word_prefetch (4));
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3616;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3616: ;
return 26 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* ADD.L Dn,(xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_d1b8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3617;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (4);
endlabel3617: ;
return 24 * CYCLE_UNIT / 2;
} /* 24 (4/2) */

/* ADD.L Dn,(xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_d1b9_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uaecptr dsta = get_long_prefetch (4);
	if (dsta & 1) {
		exception3 (opcode, dsta);
		goto endlabel3618;
	}
{{	uae_s32 dst = get_long (dsta);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{{uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
{	int flgs = ((uae_s32)(src)) < 0;
	int flgo = ((uae_s32)(dst)) < 0;
	int flgn = ((uae_s32)(newv)) < 0;
	SET_ZFLG (((uae_s32)(newv)) == 0);
	SET_VFLG ((flgs ^ flgn) & (flgo ^ flgn));
	SET_CFLG (((uae_u32)(~dst)) < ((uae_u32)(src)));
	COPY_CARRY ();
	SET_NFLG (flgn != 0);
	put_long (dsta, newv);
}}}}}}}}	m68k_incpc (6);
endlabel3618: ;
return 28 * CYCLE_UNIT / 2;
} /* 28 (5/2) */

/* ADDA.L Dn,An */
uae_u32 REGPARAM2 CPUFUNC(op_d1c0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_dreg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDA.L An,An */
uae_u32 REGPARAM2 CPUFUNC(op_d1c8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = m68k_areg (regs, srcreg);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ADDA.L (An),An */
uae_u32 REGPARAM2 CPUFUNC(op_d1d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3621;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3621: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADDA.L (An)+,An */
uae_u32 REGPARAM2 CPUFUNC(op_d1d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3622;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) += 4;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3622: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADDA.L -(An),An */
uae_u32 REGPARAM2 CPUFUNC(op_d1e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = m68k_areg (regs, srcreg) - 4;
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3623;
	}
{{	uae_s32 src = get_long (srca);
	m68k_areg (regs, srcreg) = srca;
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (2);
endlabel3623: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ADDA.L (d16,An),An */
uae_u32 REGPARAM2 CPUFUNC(op_d1e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3624;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3624: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADDA.L (d8,An,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_d1f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca;
	srca = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3625;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3625: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADDA.L (xxx).W,An */
uae_u32 REGPARAM2 CPUFUNC(op_d1f8_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3626;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3626: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADDA.L (xxx).L,An */
uae_u32 REGPARAM2 CPUFUNC(op_d1f9_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = get_long_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3627;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (6);
endlabel3627: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (5/0) */

/* ADDA.L (d16,PC),An */
uae_u32 REGPARAM2 CPUFUNC(op_d1fa_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr srca = m68k_getpc () + 2;
	srca += (uae_s32)(uae_s16)get_word_prefetch (4);
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3628;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3628: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADDA.L (d8,PC,Xn),An */
uae_u32 REGPARAM2 CPUFUNC(op_d1fb_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uaecptr tmppc;
	uaecptr srca;
	tmppc = m68k_getpc () + 2;
	srca = get_disp_ea_000 (tmppc, get_word_prefetch (4));
	if (srca & 1) {
		exception3 (opcode, srca);
		goto endlabel3629;
	}
{{	uae_s32 src = get_long (srca);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}}}	m68k_incpc (4);
endlabel3629: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (4/0) */

/* ADDA.L #<data>.L,An */
uae_u32 REGPARAM2 CPUFUNC(op_d1fc_11)(uae_u32 opcode)
{
	uae_u32 dstreg = (opcode >> 9) & 7;
{{	uae_s32 src = get_long_prefetch (4);
{	uae_s32 dst = m68k_areg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 newv = dst + src;
	m68k_areg (regs, dstreg) = (newv);
}}}}	m68k_incpc (6);
return 12 * CYCLE_UNIT / 2;
} /* 12 (3/0) */

/* ASRQ.B #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e000_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSRQ.B #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e008_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXRQ.B #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e010_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* RORQ.B #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e018_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASR.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e020_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSR.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e028_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXR.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e030_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

#endif

#ifdef PART_8
/* ROR.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e038_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASRQ.W #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e040_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSRQ.W #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e048_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXRQ.W #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e050_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* RORQ.W #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e058_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASR.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e060_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSR.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e068_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXR.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e070_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROR.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e078_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASRQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e080_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSRQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e088_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXRQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e090_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* RORQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e098_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASR.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e0a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSR.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e0a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXR.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e0b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROR.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e0b8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASRW.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_e0d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3655;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3655: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ASRW.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_e0d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3656;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3656: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ASRW.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_e0e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3657;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3657: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ASRW.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_e0e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3658;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3658: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ASRW.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_e0f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3659;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3659: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ASRW.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_e0f8_11)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3660;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3660: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ASRW.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_e0f9_11)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3661;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 val = (uae_u16)data;
	uae_u32 sign = 0x8000 & val;
	uae_u32 cflg = val & 1;
	val = (val >> 1) | sign;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (cflg);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (6);
endlabel3661: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ASLQ.B #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e100_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSLQ.B #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e108_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXLQ.B #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e110_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROLQ.B #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e118_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASL.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e120_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSL.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e128_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXL.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e130_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROL.B Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e138_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s8 cnt = m68k_dreg (regs, srcreg);
{	uae_s8 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASLQ.W #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e140_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSLQ.W #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e148_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXLQ.W #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e150_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROLQ.W #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e158_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASL.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e160_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSL.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e168_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXL.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e170_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROL.W Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e178_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s16 cnt = m68k_dreg (regs, srcreg);
{	uae_s16 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASLQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e180_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSLQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e188_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXLQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e190_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROLQ.L #<data>,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e198_11)(uae_u32 opcode)
{
	uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
	uae_u32 dstreg = opcode & 7;
{{	uae_u32 cnt = srcreg;
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASL.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e1a0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* LSL.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e1a8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROXL.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e1b0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ROL.L Dn,Dn */
uae_u32 REGPARAM2 CPUFUNC(op_e1b8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = ((opcode >> 9) & 7);
	uae_u32 dstreg = opcode & 7;
{{	uae_s32 cnt = m68k_dreg (regs, srcreg);
{	uae_s32 data = m68k_dreg (regs, dstreg);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
}}}}	m68k_incpc (2);
return 4 * CYCLE_UNIT / 2;
} /* 4 (1/0) */

/* ASLW.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_e1d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3686;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3686: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ASLW.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_e1d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3687;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3687: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ASLW.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_e1e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3688;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_prefetch (4);
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
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3688: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ASLW.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_e1e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3689;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3689: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ASLW.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_e1f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3690;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3690: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ASLW.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_e1f8_11)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3691;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
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
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3691: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ASLW.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_e1f9_11)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3692;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (8);
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
	put_word (dataa, val);
}}}}}	m68k_incpc (6);
endlabel3692: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* LSRW.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_e2d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3693;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3693: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* LSRW.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_e2d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3694;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3694: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* LSRW.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_e2e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3695;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3695: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* LSRW.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_e2e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3696;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3696: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* LSRW.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_e2f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3697;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3697: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* LSRW.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_e2f8_11)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3698;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3698: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* LSRW.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_e2f9_11)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3699;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u32 val = (uae_u16)data;
	uae_u32 carry = val & 1;
	val >>= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (6);
endlabel3699: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* LSLW.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_e3d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3700;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3700: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* LSLW.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_e3d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3701;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3701: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* LSLW.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_e3e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3702;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3702: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* LSLW.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_e3e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3703;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3703: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* LSLW.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_e3f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3704;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3704: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* LSLW.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_e3f8_11)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3705;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3705: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* LSLW.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_e3f9_11)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3706;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (6);
endlabel3706: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ROXRW.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_e4d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3707;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3707: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ROXRW.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_e4d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3708;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3708: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ROXRW.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_e4e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3709;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3709: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ROXRW.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_e4e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3710;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3710: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ROXRW.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_e4f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3711;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3711: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ROXRW.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_e4f8_11)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3712;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3712: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ROXRW.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_e4f9_11)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3713;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (GET_XFLG ()) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (6);
endlabel3713: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ROXLW.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_e5d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3714;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3714: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ROXLW.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_e5d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3715;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3715: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ROXLW.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_e5e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3716;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3716: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ROXLW.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_e5e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3717;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3717: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ROXLW.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_e5f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3718;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3718: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ROXLW.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_e5f8_11)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3719;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3719: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ROXLW.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_e5f9_11)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3720;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (GET_XFLG ()) val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	COPY_CARRY ();
	put_word (dataa, val);
}}}}}	m68k_incpc (6);
endlabel3720: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* RORW.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_e6d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3721;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3721: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* RORW.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_e6d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3722;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3722: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* RORW.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_e6e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3723;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3723: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* RORW.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_e6e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3724;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3724: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* RORW.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_e6f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3725;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3725: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* RORW.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_e6f8_11)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3726;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3726: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* RORW.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_e6f9_11)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3727;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u16 val = data;
	uae_u32 carry = val & 1;
	val >>= 1;
	if (carry) val |= 0x8000;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry);
	put_word (dataa, val);
}}}}}	m68k_incpc (6);
endlabel3727: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

/* ROLW.W (An) */
uae_u32 REGPARAM2 CPUFUNC(op_e7d0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3728;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3728: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ROLW.W (An)+ */
uae_u32 REGPARAM2 CPUFUNC(op_e7d8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3729;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) += 2;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3729: ;
return 12 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ROLW.W -(An) */
uae_u32 REGPARAM2 CPUFUNC(op_e7e0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = m68k_areg (regs, srcreg) - 2;
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3730;
	}
{{	uae_s16 data = get_word (dataa);
	m68k_areg (regs, srcreg) = dataa;
	regs.ir = regs.irc;
	get_word_prefetch (4);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word (dataa, val);
}}}}}	m68k_incpc (2);
endlabel3730: ;
return 14 * CYCLE_UNIT / 2;
} /* 12 (2/1) */

/* ROLW.W (d16,An) */
uae_u32 REGPARAM2 CPUFUNC(op_e7e8_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa = m68k_areg (regs, srcreg) + (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3731;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3731: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ROLW.W (d8,An,Xn) */
uae_u32 REGPARAM2 CPUFUNC(op_e7f0_11)(uae_u32 opcode)
{
	uae_u32 srcreg = (opcode & 7);
{{	uaecptr dataa;
	dataa = get_disp_ea_000 (m68k_areg (regs, srcreg), get_word_prefetch (4));
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3732;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3732: ;
return 18 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ROLW.W (xxx).W */
uae_u32 REGPARAM2 CPUFUNC(op_e7f8_11)(uae_u32 opcode)
{
{{	uaecptr dataa = (uae_s32)(uae_s16)get_word_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3733;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (6);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word (dataa, val);
}}}}}	m68k_incpc (4);
endlabel3733: ;
return 16 * CYCLE_UNIT / 2;
} /* 16 (3/1) */

/* ROLW.W (xxx).L */
uae_u32 REGPARAM2 CPUFUNC(op_e7f9_11)(uae_u32 opcode)
{
{{	uaecptr dataa = get_long_prefetch (4);
	if (dataa & 1) {
		exception3 (opcode, dataa);
		goto endlabel3734;
	}
{{	uae_s16 data = get_word (dataa);
	regs.ir = regs.irc;
	get_word_prefetch (8);
{	uae_u16 val = data;
	uae_u32 carry = val & 0x8000;
	val <<= 1;
	if (carry)  val |= 1;
	CLEAR_CZNV ();
	SET_ZFLG   (((uae_s16)(val)) == 0);
	SET_NFLG   (((uae_s16)(val)) < 0);
	SET_CFLG (carry >> 15);
	put_word (dataa, val);
}}}}}	m68k_incpc (6);
endlabel3734: ;
return 20 * CYCLE_UNIT / 2;
} /* 20 (4/1) */

#endif

