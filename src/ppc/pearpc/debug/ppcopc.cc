/*
 *	HT Editor
 *	ppcopc.cc
 *
 *	Copyright (C) 1999-2003 Sebastian Biallas (sb@biallas.net)
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

#include <cstdio>
#include "ppcopc.h"

/* The functions used to insert and extract complicated operands.  */

/* The BA field in an XL form instruction when it must be the same as
   the BT field in the same instruction.  This operand is marked FAKE.
   The insertion function just copies the BT field into the BA field,
   and the extraction function just checks that the fields are the
   same.  */

static uint32 extract_bat(uint32 insn, bool *invalid)
{
	if (invalid != NULL && ((insn >> 21) & 0x1f) != ((insn >> 16) & 0x1f)) *invalid = 1;
	return 0;
}

/* The BB field in an XL form instruction when it must be the same as
   the BA field in the same instruction.  This operand is marked FAKE.
   The insertion function just copies the BA field into the BB field,
   and the extraction function just checks that the fields are the
   same.  */

static uint32 extract_bba(uint32 insn, bool *invalid)
{
	if (invalid != NULL && ((insn >> 16) & 0x1f) != ((insn >> 11) & 0x1f)) *invalid = 1;
	return 0;
}

/* The BD field in a B form instruction.  The lower two bits are
   forced to zero.  */


static uint32 extract_bd(uint32 insn, bool *invalid)
{
	if ((insn & 0x8000) != 0) {
		return (insn & 0xfffc) - 0x10000;
	} else {
		return insn & 0xfffc;
	}
}

/* The BD field in a B form instruction when the - modifier is used.
   This modifier means that the branch is not expected to be taken.
   For chips built to versions of the architecture prior to version 2
   (ie. not Power4 compatible), we set the y bit of the BO field to 1
   if the offset is negative.  When extracting, we require that the y
   bit be 1 and that the offset be positive, since if the y bit is 0
   we just want to print the normal form of the instruction.
   Power4 compatible targets use two bits, "a", and "t", instead of
   the "y" bit.  "at" == 00 => no hint, "at" == 01 => unpredictable,
   "at" == 10 => not taken, "at" == 11 => taken.  The "t" bit is 00001
   in BO field, the "a" bit is 00010 for branch on CR(BI) and 01000
   for branch on CTR.  We only handle the taken/not-taken hint here.  */

static uint32 extract_bdm(uint32 insn, bool *invalid)
{
	if (invalid) {
		if ((insn & (0x17 << 21)) != (0x06 << 21)
		 && (insn & (0x1d << 21)) != (0x18 << 21))
			*invalid = true;
	}
	return ((insn & 0xfffc) ^ 0x8000) - 0x8000;
}

/* The BD field in a B form instruction when the + modifier is used.
   This is like BDM, above, except that the branch is expected to be
   taken.  */

static uint32 extract_bdp(uint32 insn, bool *invalid)
{
	if (invalid) {
		if ((insn & (0x17 << 21)) != (0x07 << 21)
		&& (insn & (0x1d << 21)) != (0x19 << 21))
			*invalid = true;
	}
	return ((insn & 0xfffc) ^ 0x8000) - 0x8000;
}

/* Check for legal values of a BO field.  */

static bool valid_bo(uint32 value)
{
	/* Certain encodings have bits that are required to be zero.
	   These are (z must be zero, a & t may be anything):
		0000z
		0001z
		0100z
		0101z
		001at
		011at
		1a00t
		1a01t
		1z1zz
	*/
	switch (value & 0x14) {
		case 0:
			return (value & 1) == 0;
		case 0x4:
		case 0x10:
			return true;
		case 0x14:
			return value == 0x14;
	}
	return false;
}

/* The BO field in a B form instruction.  Warn about attempts to set
   the field to an illegal value.  */

static uint32 extract_bo(uint32 insn, bool *invalid)
{
	uint32 value;

	value = (insn >> 21) & 0x1f;
	if (invalid != NULL && !valid_bo(value))
		*invalid = true;
	return value;
}

/* The BO field in a B form instruction when the + or - modifier is
   used.  This is like the BO field, but it must be even.  When
   extracting it, we force it to be even.  */
   
static uint32 extract_boe(uint32 insn, bool *invalid)
{
	uint32 value;

	value = (insn >> 21) & 0x1f;
	if (invalid != NULL && !valid_bo(value)) *invalid = true;
	return value & 0x1e;
}

/* The DS field in a DS form instruction.  This is like D, but the
   lower two bits are forced to zero.  */


static uint32 extract_ds(uint32 insn, bool *invalid)
{
	if ((insn & 0x8000) != 0) {
		return (insn & 0xfffc) - 0x10000;
	} else {
		return insn & 0xfffc;
	}
}

/* The LI field in an I form instruction.  The lower two bits are
   forced to zero.  */


static uint32 extract_li(uint32 insn, bool *invalid)
{
	if ((insn & 0x2000000) != 0) {
		return (insn & 0x3fffffc) - 0x4000000;
	} else {
		return insn & 0x3fffffc;
	}
}

/* The MB and ME fields in an M form instruction expressed as a single
   operand which is itself a bitmask.  The extraction function always
   marks it as invalid, since we never want to recognize an
   instruction which uses a field of this type.  */

static uint32 extract_mbe(uint32 insn, bool *invalid)
{
	uint32 ret;
	int mb, me;
	int i;

	if (invalid != NULL) *invalid = 1;

	ret = 0;
	mb = (insn >> 6) & 0x1f;
	me = (insn >> 1) & 0x1f;
	for (i = mb; i < me; i++) ret |= 1 << (31 - i);
	return ret;
}

static uint32 extract_mbe_special1(uint32 insn, bool *invalid)
{
	// slwi
	if (invalid) {
		int sh = (insn >> 11) & 0x1f;
		int mb = (insn >> 6) & 0x1f;
		int me = (insn >> 1) & 0x1f;
		*invalid = !(mb == 0 && me == 31-sh);
	}
	return 0;
}

static uint32 extract_mbe_special2(uint32 insn, bool *invalid)
{
	// srwi
	if (invalid) {
		int sh = (insn >> 11) & 0x1f;
		int mb = (insn >> 6) & 0x1f;
		int me = (insn >> 1) & 0x1f;
		*invalid = !(sh == 32-mb && me == 31);
	}
	return 0;
}

/* The MB or ME field in an MD or MDS form instruction.  The high bit
   is wrapped to the low end.  */


static uint32 extract_mb6(uint32 insn, bool *invalid)
{
	return ((insn >> 6) & 0x1f) | (insn & 0x20);
}

/* The NB field in an X form instruction.  The value 32 is stored as
   0.  */


static uint32 extract_nb(uint32 insn, bool *invalid)
{
	uint32 ret;

	ret = (insn >> 11) & 0x1f;
	if (ret == 0) ret = 32;
	return ret;
}

/* The NSI field in a D form instruction.  This is the same as the SI
   field, only negated.  The extraction function always marks it as
   invalid, since we never want to recognize an instruction which uses
   a field of this type.  */

static uint32 extract_nsi(uint32 insn, bool *invalid)
{
	if (invalid != NULL) *invalid = 1;
	if ((insn & 0x8000) != 0) {
		return - ((insn & 0xffff) - 0x10000);
	} else {
		return - (insn & 0xffff);
	}
}

/* The RA field in a D or X form instruction which is an updating
   load, which means that the RA field may not be zero and may not
   equal the RT field.  */

/* The RB field in an X form instruction when it must be the same as
   the RS field in the instruction.  This is used for extended
   mnemonics like mr.  This operand is marked FAKE.  The insertion
   function just copies the BT field into the BA field, and the
   extraction function just checks that the fields are the same.  */

static uint32 extract_rbs(uint32 insn, bool *invalid)
{
  if (invalid != NULL
	 && ((insn >> 21) & 0x1f) != ((insn >> 11) & 0x1f))
    *invalid = true;
  return 0;
}

static uint32 extract_vab(uint32 insn, bool *invalid)
{
  if (invalid != NULL
	 && ((insn >> 16) & 0x1f) != ((insn >> 11) & 0x1f))
    *invalid = true;
  return 0;
}

/* The SH field in an MD form instruction.  This is split.  */

static uint32 extract_sh6(uint32 insn, bool *invalid)
{
  return ((insn >> 11) & 0x1f) | ((insn << 4) & 0x20);
}

/* The SPR field in an XFX form instruction.  This is flipped--the
   lower 5 bits are stored in the upper 5 and vice- versa.  */

static uint32 extract_spr(uint32 insn, bool *invalid)
{
  return ((insn >> 16) & 0x1f) | ((insn >> 6) & 0x3e0);
}

/* The TBR field in an XFX instruction.  This is just like SPR, but it
   is optional.  When TBR is omitted, it must be inserted as 268 (the
   magic number of the TB register).  These functions treat 0
   (indicating an omitted optional operand) as 268.  This means that
   ``mftb 4,0'' is not handled correctly.  This does not matter very
   much, since the architecture manual does not define mftb as
   accepting any values other than 268 or 269.  */

#define TB (268)

static uint32 extract_tbr(uint32 insn, bool *invalid)
{
  uint32 ret = ((insn >> 16) & 0x1f) | ((insn >> 6) & 0x3e0);
  if (ret == TB) ret = 0;
  return ret;
}

static uint32 extract_vds128(uint32 insn, bool *invalid)
{
	return ((insn<<3) & 0x60) | ((insn>>21) & 0x1f);
}

static uint32 extract_va128(uint32 insn, bool *invalid)
{
	return ((insn>>4) & 0x40) | (insn & 0x20) | ((insn>>16) & 0x1f);
}

static uint32 extract_vb128(uint32 insn, bool *invalid)
{
	return ((insn<<5) & 0x60) | ((insn>>11) & 0x1f);
}

static uint32 extract_vperm(uint32 insn, bool *invalid)
{
	return ((insn>>1) & 0xe0) | ((insn>>16) & 0x1f);
}

/* The operands table.

   The fields are bits, shift, signed, extract, flags.  */

#undef UNUSED

const struct powerpc_operand powerpc_operands[] =
{
  /* The zero index is used to indicate the end of the list of
     operands.  */
#define UNUSED 0
  { 0, 0, 0, 0 },

  /* The BA field in an XL form instruction.  */
#define BA UNUSED + 1
#define BA_MASK (0x1f << 16)
  { 5, 16, 0, PPC_OPERAND_CR },

  /* The BA field in an XL form instruction when it must be the same
     as the BT field in the same instruction.  */
#define BAT BA + 1
  { 5, 16, extract_bat, PPC_OPERAND_FAKE },

  /* The BB field in an XL form instruction.  */
#define BB BAT + 1
#define BB_MASK (0x1f << 11)
  { 5, 11, 0, PPC_OPERAND_CR },

  /* The BB field in an XL form instruction when it must be the same
     as the BA field in the same instruction.  */
#define BBA BB + 1
  { 5, 11, extract_bba, PPC_OPERAND_FAKE },

  /* The BD field in a B form instruction.  The lower two bits are
     forced to zero.  */
#define BD BBA + 1
  { 16, 0, extract_bd, PPC_OPERAND_RELATIVE | PPC_OPERAND_SIGNED },

  /* The BD field in a B form instruction when absolute addressing is
     used.  */
#define BDA BD + 1
  { 16, 0, extract_bd, PPC_OPERAND_ABSOLUTE | PPC_OPERAND_SIGNED },

  /* The BD field in a B form instruction when the - modifier is used.
     This sets the y bit of the BO field appropriately.  */
#define BDM BDA + 1
  { 16, 0, extract_bdm,
      PPC_OPERAND_RELATIVE | PPC_OPERAND_SIGNED },

  /* The BD field in a B form instruction when the - modifier is used
     and absolute address is used.  */
#define BDMA BDM + 1
  { 16, 0, extract_bdm,
      PPC_OPERAND_ABSOLUTE | PPC_OPERAND_SIGNED },

  /* The BD field in a B form instruction when the + modifier is used.
     This sets the y bit of the BO field appropriately.  */
#define BDP BDMA + 1
  { 16, 0, extract_bdp,
      PPC_OPERAND_RELATIVE | PPC_OPERAND_SIGNED },

  /* The BD field in a B form instruction when the + modifier is used
     and absolute addressing is used.  */
#define BDPA BDP + 1
  { 16, 0, extract_bdp,
      PPC_OPERAND_ABSOLUTE | PPC_OPERAND_SIGNED },

  /* The BF field in an X or XL form instruction.  */
#define BF BDPA + 1
  { 3, 23, 0, PPC_OPERAND_CR },

  /* An optional BF field.  This is used for comparison instructions,
     in which an omitted BF field is taken as zero.  */
#define OBF BF + 1
  { 3, 23, 0, PPC_OPERAND_CR | PPC_OPERAND_OPTIONAL },

  /* The BFA field in an X or XL form instruction.  */
#define BFA OBF + 1
  { 3, 18, 0, PPC_OPERAND_CR },

  /* The BI field in a B form or XL form instruction.  */
#define BI BFA + 1
#define BI_MASK (0x1f << 16)
  { 5, 16, 0, PPC_OPERAND_CR },

  /* The BO field in a B form instruction.  Certain values are
     illegal.  */
#define BO BI + 1
#define BO_MASK (0x1f << 21)
  { 5, 21, extract_bo, 0 },

  /* The BO field in a B form instruction when the + or - modifier is
     used.  This is like the BO field, but it must be even.  */
#define BOE BO + 1
  { 5, 21, extract_boe, 0 },

  /* The BT field in an X or XL form instruction.  */
#define BT BOE + 1
  { 5, 21, 0, PPC_OPERAND_CR },

  /* The condition register number portion of the BI field in a B form
     or XL form instruction.  This is used for the extended
     conditional branch mnemonics, which set the lower two bits of the
     BI field.  This field is optional.  */
#define CR BT + 1
  { 3, 18, 0, PPC_OPERAND_CR | PPC_OPERAND_OPTIONAL },

  /* The CRB field in an X form instruction.  */
#define CRB CR + 1
  { 5, 6, 0, 0 },

  /* The CRFD field in an X form instruction.  */
#define CRFD CRB + 1
  { 3, 23, 0, PPC_OPERAND_CR },

  /* The CRFS field in an X form instruction.  */
#define CRFS CRFD + 1
  { 3, 0, 0, PPC_OPERAND_CR },

  /* The CT field in an X form instruction.  */
#define CT CRFS + 1
  { 5, 21, 0, PPC_OPERAND_OPTIONAL },

  /* The D field in a D form instruction.  This is a displacement off
     a register, and implies that the next operand is a register in
     parentheses.  */
#define D CT + 1
  { 16, 0, 0, PPC_OPERAND_PARENS | PPC_OPERAND_SIGNED },

  /* The DS field in a DS form instruction.  This is like D, but the
     lower two bits are forced to zero.  */
#define DS D + 1
  { 16, 0, extract_ds,
      PPC_OPERAND_PARENS | PPC_OPERAND_SIGNED | PPC_OPERAND_DS },

  /* The FLM field in an XFL form instruction.  */
#define FLM DS + 1
  { 8, 17, 0, 0 },

  /* The FRA field in an X or A form instruction.  */
#define FRA FLM + 1
#define FRA_MASK (0x1f << 16)
  { 5, 16, 0, PPC_OPERAND_FPR },

  /* The FRB field in an X or A form instruction.  */
#define FRB FRA + 1
#define FRB_MASK (0x1f << 11)
  { 5, 11, 0, PPC_OPERAND_FPR },

  /* The FRC field in an A form instruction.  */
#define FRC FRB + 1
#define FRC_MASK (0x1f << 6)
  { 5, 6, 0, PPC_OPERAND_FPR },

  /* The FRS field in an X form instruction or the FRT field in a D, X
     or A form instruction.  */
#define FRS FRC + 1
#define FRT FRS
  { 5, 21, 0, PPC_OPERAND_FPR },

  /* The FXM field in an XFX instruction.  */
#define FXM FRS + 1
#define FXM_MASK (0xff << 12)
  { 8, 12, 0, 0 },

  /* The L field in a D or X form instruction.  */
#define L FXM + 1
  { 1, 21, 0, PPC_OPERAND_OPTIONAL },

  /* The LI field in an I form instruction.  The lower two bits are
     forced to zero.  */
#define LI L + 1
  { 26, 0, extract_li, PPC_OPERAND_RELATIVE | PPC_OPERAND_SIGNED },

  /* The LI field in an I form instruction when used as an absolute
     address.  */
#define LIA LI + 1
  { 26, 0, extract_li, PPC_OPERAND_ABSOLUTE | PPC_OPERAND_SIGNED },

  /* The LS field in an X (sync) form instruction.  */
#define LS LIA + 1
  { 2, 21, 0, PPC_OPERAND_OPTIONAL },

  /* The MB field in an M form instruction.  */
#define MB LS + 1
#define MB_MASK (0x1f << 6)
  { 5, 6, 0, 0 },

  /* The ME field in an M form instruction.  */
#define ME MB + 1
#define ME_MASK (0x1f << 1)
  { 5, 1, 0, 0 },

  /* The MB and ME fields in an M form instruction expressed a single
     operand which is a bitmask indicating which bits to select.  This
     is a two operand form using PPC_OPERAND_NEXT.  See the
     description in opcode/ppc.h for what this means.  */
#define MBE ME + 1
  { 5, 6, 0, PPC_OPERAND_OPTIONAL | PPC_OPERAND_NEXT },
  { 32, 0, extract_mbe, 0 },

  /* The MB or ME field in an MD or MDS form instruction.  The high
     bit is wrapped to the low end.  */
#define MB6 MBE + 2
#define ME6 MB6
#define MB6_MASK (0x3f << 5)
  { 6, 5, extract_mb6, 0 },

#define MSLWI MB6 + 1
  { 0, 0, extract_mbe_special1, PPC_OPERAND_FAKE },

#define MSRWI MSLWI + 1
  { 0, 0, extract_mbe_special2, PPC_OPERAND_FAKE },

  /* The MO field in an mbar instruction.  */
#define MO MSRWI + 1
  { 5, 21, 0, 0 },

  /* The NB field in an X form instruction.  The value 32 is stored as
     0.  */
#define NB MO + 1
  { 6, 11, extract_nb, 0 },

  /* The NSI field in a D form instruction.  This is the same as the
     SI field, only negated.  */
#define NSI NB + 1
  { 16, 0, extract_nsi,
      PPC_OPERAND_NEGATIVE | PPC_OPERAND_SIGNED },

  /* The RA field in an D, DS, X, XO, M, or MDS form instruction.  */
#define RA NSI + 1
#define RA_MASK (0x1f << 16)
  { 5, 16, 0, PPC_OPERAND_GPR },

  /* As above, but 0 in the RA field means zero, not r0.  */
#define RA0 RA + 1
  { 5, 16, 0, PPC_OPERAND_GPR_0 },

  /* The RA field in a D or X form instruction which is an updating
     load, which means that the RA field may not be zero and may not
     equal the RT field.  */
#define RAL RA0 + 1
  { 5, 16, 0, PPC_OPERAND_GPR },

  /* The RA field in an lmw instruction, which has special value
     restrictions.  */
#define RAM RAL + 1
  { 5, 16, 0, PPC_OPERAND_GPR },

  /* The RA field in a D or X form instruction which is an updating
     store or an updating floating point load, which means that the RA
     field may not be zero.  */
#define RAS RAM + 1
  { 5, 16, 0, PPC_OPERAND_GPR },

  /* The RB field in an X, XO, M, or MDS form instruction.  */
#define RB RAS + 1
#define RB_MASK (0x1f << 11)
  { 5, 11, 0, PPC_OPERAND_GPR },

  /* The RB field in an X form instruction when it must be the same as
     the RS field in the instruction.  This is used for extended
     mnemonics like mr.  */
#define RBS RB + 1
  { 5, 1, extract_rbs, PPC_OPERAND_FAKE },

  /* The RS field in a D, DS, X, XFX, XS, M, MD or MDS form
     instruction or the RT field in a D, DS, X, XFX or XO form
     instruction.  */
#define RS RBS + 1
#define RT RS
#define RT_MASK (0x1f << 21)
  { 5, 21, 0, PPC_OPERAND_GPR },

  /* The SH field in an X or M form instruction.  */
#define SH RS + 1
#define SH_MASK (0x1f << 11)
  { 5, 11, 0, 0 },

  /* The SH field in an MD form instruction.  This is split.  */
#define SH6 SH + 1
#define SH6_MASK ((0x1f << 11) | (1 << 1))
  { 6, 1, extract_sh6, 0 },

  /* The SI field in a D form instruction.  */
#define SI SH6 + 1
  { 16, 0, 0, PPC_OPERAND_SIGNED },

  /* The SI field in a D form instruction when we accept a wide range
     of positive values.  */
#define SISIGNOPT SI + 1
  { 16, 0, 0, PPC_OPERAND_SIGNED | PPC_OPERAND_SIGNOPT },

  /* The SPR field in an XFX form instruction.  This is flipped--the
     lower 5 bits are stored in the upper 5 and vice- versa.  */
#define SPR SISIGNOPT + 1
#define PMR SPR
#define SPR_MASK (0x3ff << 11)
  { 10, 11, extract_spr, 0 },

  /* The BAT index number in an XFX form m[ft]ibat[lu] instruction.  */
#define SPRBAT SPR + 1
#define SPRBAT_MASK (0x3 << 17)
  { 2, 17, 0, 0 },

  /* The SPRG register number in an XFX form m[ft]sprg instruction.  */
#define SPRG SPRBAT + 1
#define SPRG_MASK (0x3 << 16)
  { 2, 16, 0, 0 },

  /* The SR field in an X form instruction.  */
#define SR SPRG + 1
  { 4, 16, 0, 0 },

  /* The STRM field in an X AltiVec form instruction.  */
#define STRM SR + 1
#define STRM_MASK (0x3 << 21)
  { 2, 21, 0, 0 },

  /* The SV field in a POWER SC form instruction.  */
#define SV STRM + 1
  { 14, 2, 0, 0 },

  /* The TBR field in an XFX form instruction.  This is like the SPR
     field, but it is optional.  */
#define TBR SV + 1
  { 10, 11, extract_tbr, PPC_OPERAND_OPTIONAL },

  /* The TO field in a D or X form instruction.  */
#define TO TBR + 1
#define TO_MASK (0x1f << 21)
  { 5, 21, 0, 0 },

  /* The U field in an X form instruction.  */
#define U TO + 1
  { 4, 12, 0, 0 },

  /* The UI field in a D form instruction.  */
#define UI U + 1
  { 16, 0, 0, 0 },

  /* The VA field in a VA, VX or VXR form instruction. */
#define VA UI + 1
#define VA_MASK	(0x1f << 16)
  { 5, 16, 0, PPC_OPERAND_VR },

  /* The VB field in a VA, VX or VXR form instruction. */
#define VB VA + 1
#define VB_MASK (0x1f << 11)
  { 5, 11, 0, PPC_OPERAND_VR },

  /* The VB field in a VA, VX or VXR form instruction. */
#define VAB VB + 1
#define VAB_MASK (0x1f << 11)
  { 5, 11, extract_vab, PPC_OPERAND_FAKE },

  /* The VC field in a VA form instruction. */
#define VC VAB + 1
#define VC_MASK (0x1f << 6)
  { 5, 6, 0, PPC_OPERAND_VR },

  /* The VD or VS field in a VA, VX, VXR or X form instruction. */
#define VD VC + 1
#define VS VD
#define VD_MASK (0x1f << 21)
  { 5, 21, 0, PPC_OPERAND_VR },

  /* The VD or VS field in a VA, VX, VXR or X form instruction. */
#define VD128 VD + 1
#define VS128 VD128
#define VD128_MASK (0x1f << 21)
  { 0, 0, extract_vds128, PPC_OPERAND_VR },

  /* The VD or VS field in a VA, VX, VXR or X form instruction. */
#define VA128 VD128 + 1
#define VA128_MASK (0x1f << 21)
  { 0, 0, extract_va128, PPC_OPERAND_VR },

  /* The VD or VS field in a VA, VX, VXR or X form instruction. */
#define VB128 VA128 + 1
#define VB128_MASK (0x1f << 21)
  { 0, 0, extract_vb128, PPC_OPERAND_VR },

  /* The VD or VS field in a VA, VX, VXR or X form instruction. */
#define VC128 VB128 + 1
#define VC128_MASK (0x1f << 21)
  { 3, 6, 0, PPC_OPERAND_VR },

#define VPERM128 VC128 + 1
#define VPERM_MASK (0x1f << 21)
  { 0, 0, extract_vperm, 0 },

#define VD3D0 VPERM128 + 1
  { 3, 18, 0, 0 },

#define VD3D1 VD3D0 + 1
  { 2, 16, 0, 0 },

#define VD3D2 VD3D1 + 1
  { 2, 6, 0, 0 },

  /* The SIMM field in a VX form instruction. */
#define SIMM VD3D2 + 1
  { 5, 16, 0, PPC_OPERAND_SIGNED},

  /* The UIMM field in a VX form instruction. */
#define UIMM SIMM + 1
  { 5, 16, 0, 0 },

  /* The SHB field in a VA form instruction. */
#define SHB UIMM + 1
  { 4, 6, 0, 0 },

  /* The WS field.  */
#define WS SHB + 1
#define WS_MASK (0x7 << 11)
  { 3, 11, 0, 0 },

  /* The L field in an mtmsrd instruction */
#define MTMSRD_L WS + 1
  { 1, 16, 0, PPC_OPERAND_OPTIONAL },
};


/* Macros used to form opcodes.  */

/* The main opcode.  */
#define OP(x) ((((unsigned long)(x)) & 0x3f) << 26)
#define OP_MASK OP (0x3f)

/* The main opcode combined with a trap code in the TO field of a D
   form instruction.  Used for extended mnemonics for the trap
   instructions.  */
#define OPTO(x,to) (OP (x) | ((((unsigned long)(to)) & 0x1f) << 21))
#define OPTO_MASK (OP_MASK | TO_MASK)

/* The main opcode combined with a comparison size bit in the L field
   of a D form or X form instruction.  Used for extended mnemonics for
   the comparison instructions.  */
#define OPL(x,l) (OP (x) | ((((unsigned long)(l)) & 1) << 21))
#define OPL_MASK OPL (0x3f,1)

/* An A form instruction.  */
#define A(op, xop, rc) (OP (op) | ((((unsigned long)(xop)) & 0x1f) << 1) | (((unsigned long)(rc)) & 1))
#define A_MASK A (0x3f, 0x1f, 1)

/* An A_MASK with the FRB field fixed.  */
#define AFRB_MASK (A_MASK | FRB_MASK)

/* An A_MASK with the FRC field fixed.  */
#define AFRC_MASK (A_MASK | FRC_MASK)

/* An A_MASK with the FRA and FRC fields fixed.  */
#define AFRAFRC_MASK (A_MASK | FRA_MASK | FRC_MASK)

/* A B form instruction.  */
#define B(op, aa, lk) (OP (op) | ((((unsigned long)(aa)) & 1) << 1) | ((lk) & 1))
#define B_MASK B (0x3f, 1, 1)

/* A B form instruction setting the BO field.  */
#define BBO(op, bo, aa, lk) (B ((op), (aa), (lk)) | ((((unsigned long)(bo)) & 0x1f) << 21))
#define BBO_MASK BBO (0x3f, 0x1f, 1, 1)

/* A BBO_MASK with the y bit of the BO field removed.  This permits
   matching a conditional branch regardless of the setting of the y
   bit.  Similarly for the 'at' bits used for power4 branch hints.  */
#define Y_MASK   (((unsigned long) 1) << 21)
#define AT1_MASK (((unsigned long) 3) << 21)
#define AT2_MASK (((unsigned long) 9) << 21)
#define BBOY_MASK  (BBO_MASK &~ Y_MASK)
#define BBOAT_MASK (BBO_MASK &~ AT1_MASK)

/* A B form instruction setting the BO field and the condition bits of
   the BI field.  */
#define BBOCB(op, bo, cb, aa, lk) \
  (BBO ((op), (bo), (aa), (lk)) | ((((unsigned long)(cb)) & 0x3) << 16))
#define BBOCB_MASK BBOCB (0x3f, 0x1f, 0x3, 1, 1)

/* A BBOCB_MASK with the y bit of the BO field removed.  */
#define BBOYCB_MASK (BBOCB_MASK &~ Y_MASK)
#define BBOATCB_MASK (BBOCB_MASK &~ AT1_MASK)
#define BBOAT2CB_MASK (BBOCB_MASK &~ AT2_MASK)

/* A BBOYCB_MASK in which the BI field is fixed.  */
#define BBOYBI_MASK (BBOYCB_MASK | BI_MASK)
#define BBOATBI_MASK (BBOAT2CB_MASK | BI_MASK)

/* An Context form instruction.  */
#define CTX(op, xop)   (OP (op) | (((unsigned long)(xop)) & 0x7))
#define CTX_MASK       CTX(0x3f, 0x7)

/* An User Context form instruction.  */
#define UCTX(op, xop)  (OP (op) | (((unsigned long)(xop)) & 0x1f))
#define UCTX_MASK      UCTX(0x3f, 0x1f)

/* The main opcode mask with the RA field clear.  */
#define DRA_MASK (OP_MASK | RA_MASK)

/* A DS form instruction.  */
#define DSO(op, xop) (OP (op) | ((xop) & 0x3))
#define DS_MASK DSO (0x3f, 3)

/* A DE form instruction.  */
#define DEO(op, xop) (OP (op) | ((xop) & 0xf))
#define DE_MASK DEO (0x3e, 0xf)

/* An EVSEL form instruction.  */
#define EVSEL(op, xop) (OP (op) | (((unsigned long)(xop)) & 0xff) << 3)
#define EVSEL_MASK EVSEL(0x3f, 0xff)

/* An M form instruction.  */
#define M(op, rc) (OP (op) | ((rc) & 1))
#define M_MASK M (0x3f, 1)

/* An M form instruction with the ME field specified.  */
#define MME(op, me, rc) (M ((op), (rc)) | ((((unsigned long)(me)) & 0x1f) << 1))

/* An M_MASK with the MB and ME fields fixed.  */
#define MMBME_MASK (M_MASK | MB_MASK | ME_MASK)

/* An M_MASK with the SH and ME fields fixed.  */
#define MSHME_MASK (M_MASK | SH_MASK | ME_MASK)

/* An MD form instruction.  */
#define MD(op, xop, rc) (OP (op) | ((((unsigned long)(xop)) & 0x7) << 2) | ((rc) & 1))
#define MD_MASK MD (0x3f, 0x7, 1)

/* An MD_MASK with the MB field fixed.  */
#define MDMB_MASK (MD_MASK | MB6_MASK)

/* An MD_MASK with the SH field fixed.  */
#define MDSH_MASK (MD_MASK | SH6_MASK)

/* An MDS form instruction.  */
#define MDS(op, xop, rc) (OP (op) | ((((unsigned long)(xop)) & 0xf) << 1) | ((rc) & 1))
#define MDS_MASK MDS (0x3f, 0xf, 1)

/* An MDS_MASK with the MB field fixed.  */
#define MDSMB_MASK (MDS_MASK | MB6_MASK)

/* An SC form instruction.  */
#define SC(op, sa, lk) (OP (op) | ((((unsigned long)(sa)) & 1) << 1) | ((lk) & 1))
#define SC_MASK (OP_MASK | (((unsigned long)0x3ff) << 16) | (((unsigned long)1) << 1) | 1)

/* An VX form instruction. */
#define VX(op, xop) (OP (op) | (((unsigned long)(xop)) & 0x7ff))

/* The mask for an VX form instruction. */
#define VX_MASK	VX(0x3f, 0x7ff)

/* The mask for an VX form instruction. */
#define VX_MASK	VX(0x3f, 0x7ff)

/* An VA form instruction. */
#define VXA(op, xop) (OP (op) | (((unsigned long)(xop)) & 0x03f))

/* The mask for an VA form instruction. */
#define VXA_MASK VXA(0x3f, 0x3f)

/* An VXR form instruction. */
#define VXR(op, xop, rc) (OP (op) | (((rc) & 1) << 10) | (((unsigned long)(xop)) & 0x3ff))

/* The mask for a VXR form instruction. */
#define VXR_MASK VXR(0x3f, 0x3ff, 1)

/* An VX128 form instruction. */
#define VX128(op, xop) (OP (op) | (((unsigned long)(xop)) & 0x3d0))

/* The mask for an VX form instruction. */
#define VX128_MASK	VX(0x3f, 0x3d0)

/* An VX128 form instruction. */
#define VX128_1(op, xop) (OP (op) | (((unsigned long)(xop)) & 0x7f3))

/* The mask for an VX form instruction. */
#define VX128_1_MASK	VX(0x3f, 0x7f3)

/* An VX128 form instruction. */
#define VX128_2(op, xop) (OP (op) | (((unsigned long)(xop)) & 0x210))

/* The mask for an VX form instruction. */
#define VX128_2_MASK	VX(0x3f, 0x210)

/* An VX128 form instruction. */
#define VX128_3(op, xop) (OP (op) | (((unsigned long)(xop)) & 0x7f0))

/* The mask for an VX form instruction. */
#define VX128_3_MASK	VX(0x3f, 0x7f0)

#define VX128_P(op, xop) (OP (op) | (((unsigned long)(xop)) & 0x630))
#define VX128_P_MASK	VX(0x3f, 0x630)

#define VX128_4(op, xop) (OP (op) | (((unsigned long)(xop)) & 0x730))
#define VX128_4_MASK	VX(0x3f, 0x730)

#define VX128_5(op, xop) (OP (op) | (((unsigned long)(xop)) & 0x10))
#define VX128_5_MASK	VX(0x3f, 0x10)

/* An X form instruction.  */
#define X(op, xop) (OP (op) | ((((unsigned long)(xop)) & 0x3ff) << 1))

/* An X form instruction with the RC bit specified.  */
#define XRC(op, xop, rc) (X ((op), (xop)) | ((rc) & 1))

/* The mask for an X form instruction.  */
#define X_MASK XRC (0x3f, 0x3ff, 1)

/* An X_MASK with the RA field fixed.  */
#define XRA_MASK (X_MASK | RA_MASK)

/* An X_MASK with the RB field fixed.  */
#define XRB_MASK (X_MASK | RB_MASK)

/* An X_MASK with the RT field fixed.  */
#define XRT_MASK (X_MASK | RT_MASK)

/* An X_MASK with the RA and RB fields fixed.  */
#define XRARB_MASK (X_MASK | RA_MASK | RB_MASK)

/* An XRARB_MASK, but with the L bit clear. */
#define XRLARB_MASK (XRARB_MASK & ~((unsigned long) 1 << 16))

/* An X_MASK with the RT and RA fields fixed.  */
#define XRTRA_MASK (X_MASK | RT_MASK | RA_MASK)

/* An XRTRA_MASK, but with L bit clear.  */
#define XRTLRA_MASK (XRTRA_MASK & ~((unsigned long) 1 << 21))

/* An X form comparison instruction.  */
#define XCMPL(op, xop, l) (X ((op), (xop)) | ((((unsigned long)(l)) & 1) << 21))

/* The mask for an X form comparison instruction.  */
#define XCMP_MASK (X_MASK | (((unsigned long)1) << 22))

/* The mask for an X form comparison instruction with the L field
   fixed.  */
#define XCMPL_MASK (XCMP_MASK | (((unsigned long)1) << 21))

/* An X form instruction with the L bit specified.  */
#define XOPL(op, xop, l) (X ((op), (xop)) | ((((unsigned long)(l)) & 1) << 21))

/* An X form trap instruction with the TO field specified.  */
#define XTO(op, xop, to) (X ((op), (xop)) | ((((unsigned long)(to)) & 0x1f) << 21))
#define XTO_MASK (X_MASK | TO_MASK)

/* An X form tlb instruction with the SH field specified.  */
#define XTLB(op, xop, sh) (X ((op), (xop)) | ((((unsigned long)(sh)) & 0x1f) << 11))
#define XTLB_MASK (X_MASK | SH_MASK)

/* An X form sync instruction.  */
#define XSYNC(op, xop, l) (X ((op), (xop)) | ((((unsigned long)(l)) & 3) << 21))

/* An X form sync instruction with everything filled in except the LS field.  */
#define XSYNC_MASK (0xff9fffff)

/* An X form AltiVec dss instruction.  */
#define XDSS(op, xop, a) (X ((op), (xop)) | ((((unsigned long)(a)) & 1) << 25))
#define XDSS_MASK XDSS(0x3f, 0x3ff, 1)

/* An XFL form instruction.  */
#define XFL(op, xop, rc) (OP (op) | ((((unsigned long)(xop)) & 0x3ff) << 1) | (((unsigned long)(rc)) & 1))
#define XFL_MASK (XFL (0x3f, 0x3ff, 1) | (((unsigned long)1) << 25) | (((unsigned long)1) << 16))

/* An X form isel instruction.  */
#define XISEL(op, xop)  (OP (op) | ((((unsigned long)(xop)) & 0x1f) << 1))
#define XISEL_MASK      XISEL(0x3f, 0x1f)

/* An XL form instruction with the LK field set to 0.  */
#define XL(op, xop) (OP (op) | ((((unsigned long)(xop)) & 0x3ff) << 1))

/* An XL form instruction which uses the LK field.  */
#define XLLK(op, xop, lk) (XL ((op), (xop)) | ((lk) & 1))

/* The mask for an XL form instruction.  */
#define XL_MASK XLLK (0x3f, 0x3ff, 1)

/* An XL form instruction which explicitly sets the BO field.  */
#define XLO(op, bo, xop, lk) \
  (XLLK ((op), (xop), (lk)) | ((((unsigned long)(bo)) & 0x1f) << 21))
#define XLO_MASK (XL_MASK | BO_MASK)

/* An XL form instruction which explicitly sets the y bit of the BO
   field.  */
#define XLYLK(op, xop, y, lk) (XLLK ((op), (xop), (lk)) | ((((unsigned long)(y)) & 1) << 21))
#define XLYLK_MASK (XL_MASK | Y_MASK)

/* An XL form instruction which sets the BO field and the condition
   bits of the BI field.  */
#define XLOCB(op, bo, cb, xop, lk) \
  (XLO ((op), (bo), (xop), (lk)) | ((((unsigned long)(cb)) & 3) << 16))
#define XLOCB_MASK XLOCB (0x3f, 0x1f, 0x3, 0x3ff, 1)

/* An XL_MASK or XLYLK_MASK or XLOCB_MASK with the BB field fixed.  */
#define XLBB_MASK (XL_MASK | BB_MASK)
#define XLYBB_MASK (XLYLK_MASK | BB_MASK)
#define XLBOCBBB_MASK (XLOCB_MASK | BB_MASK)

/* An XL_MASK with the BO and BB fields fixed.  */
#define XLBOBB_MASK (XL_MASK | BO_MASK | BB_MASK)

/* An XL_MASK with the BO, BI and BB fields fixed.  */
#define XLBOBIBB_MASK (XL_MASK | BO_MASK | BI_MASK | BB_MASK)

/* An XO form instruction.  */
#define XO(op, xop, oe, rc) \
  (OP (op) | ((((unsigned long)(xop)) & 0x1ff) << 1) | ((((unsigned long)(oe)) & 1) << 10) | (((unsigned long)(rc)) & 1))
#define XO_MASK XO (0x3f, 0x1ff, 1, 1)

/* An XO_MASK with the RB field fixed.  */
#define XORB_MASK (XO_MASK | RB_MASK)

/* An XS form instruction.  */
#define XS(op, xop, rc) (OP (op) | ((((unsigned long)(xop)) & 0x1ff) << 2) | (((unsigned long)(rc)) & 1))
#define XS_MASK XS (0x3f, 0x1ff, 1)

/* A mask for the FXM version of an XFX form instruction.  */
#define XFXFXM_MASK (X_MASK | (((unsigned long)1) << 20) | (((unsigned long)1) << 11))
#define XFXFXM_MASK2 (X_MASK | (((unsigned long)1) << 20))

/* An XFX form instruction with the FXM field filled in.  */
#define XFXM(op, xop, fxm) \
  (X ((op), (xop)) | ((((unsigned long)(fxm)) & 0xff) << 12))

/* An XFX form instruction with the SPR field filled in.  */
#define XSPR(op, xop, spr) \
  (X ((op), (xop)) | ((((unsigned long)(spr)) & 0x1f) << 16) | ((((unsigned long)(spr)) & 0x3e0) << 6))
#define XSPR_MASK (X_MASK | SPR_MASK)

/* An XFX form instruction with the SPR field filled in except for the
   SPRBAT field.  */
#define XSPRBAT_MASK (XSPR_MASK &~ SPRBAT_MASK)

/* An XFX form instruction with the SPR field filled in except for the
   SPRG field.  */
#define XSPRG_MASK (XSPR_MASK &~ SPRG_MASK)

/* An X form instruction with everything filled in except the E field.  */
#define XE_MASK (0xffff7fff)

/* An X form user context instruction.  */
#define XUC(op, xop)  (OP (op) | (((unsigned long)(xop)) & 0x1f))
#define XUC_MASK      XUC(0x3f, 0x1f)

/* The BO encodings used in extended conditional branch mnemonics.  */
#define BODNZF	(0x0)
#define BODNZFP	(0x1)
#define BODZF	(0x2)
#define BODZFP	(0x3)
#define BODNZT	(0x8)
#define BODNZTP	(0x9)
#define BODZT	(0xa)
#define BODZTP	(0xb)

#define BOF	(0x4)
#define BOFP	(0x5)
#define BOFM4	(0x6)
#define BOFP4	(0x7)
#define BOT	(0xc)
#define BOTP	(0xd)
#define BOTM4	(0xe)
#define BOTP4	(0xf)

#define BODNZ	(0x10)
#define BODNZP	(0x11)
#define BODZ	(0x12)
#define BODZP	(0x13)
#define BODNZM4 (0x18)
#define BODNZP4 (0x19)
#define BODZM4	(0x1a)
#define BODZP4	(0x1b)

#define BOU	(0x14)

/* The BI condition bit encodings used in extended conditional branch
   mnemonics.  */
#define CBLT	(0)
#define CBGT	(1)
#define CBEQ	(2)
#define CBSO	(3)

/* The TO encodings used in extended trap mnemonics.  */
#define TOLGT	(0x1)
#define TOLLT	(0x2)
#define TOEQ	(0x4)
#define TOLGE	(0x5)
#define TOLNL	(0x5)
#define TOLLE	(0x6)
#define TOLNG	(0x6)
#define TOGT	(0x8)
#define TOGE	(0xc)
#define TONL	(0xc)
#define TOLT	(0x10)
#define TOLE	(0x14)
#define TONG	(0x14)
#define TONE	(0x18)
#define TOU	(0x1f)

/* Smaller names for the flags so each entry in the opcodes table will
   fit on a single line.  */
#undef	PPC
#define PPC     PPC_OPCODE_PPC
#define PPCCOM	PPC_OPCODE_PPC | PPC_OPCODE_COMMON
#define NOPOWER4 PPC_OPCODE_NOPOWER4 | PPCCOM
#define POWER4	PPC_OPCODE_POWER4
#define POWER5	PPC_OPCODE_POWER5
#define PPC32   PPC_OPCODE_32 | PPC_OPCODE_PPC
#define PPC64   PPC_OPCODE_64 | PPC_OPCODE_PPC
#define PPC403	PPC_OPCODE_403
#define PPC405	PPC403
#define PPC440	PPC_OPCODE_440
#define PPC750	PPC
#define PPC860	PPC
#define PPCVEC	PPC_OPCODE_ALTIVEC
#define	POWER   PPC_OPCODE_POWER
#define	POWER2	PPC_OPCODE_POWER | PPC_OPCODE_POWER2
#define PPCPWR2	PPC_OPCODE_PPC | PPC_OPCODE_POWER | PPC_OPCODE_POWER2
#define	POWER32	PPC_OPCODE_POWER | PPC_OPCODE_32
#define PPCONLY	PPC_OPCODE_PPC
#define	COM     PPC_OPCODE_POWER | PPC_OPCODE_PPC | PPC_OPCODE_COMMON
#define	COM32   PPC_OPCODE_POWER | PPC_OPCODE_PPC | PPC_OPCODE_COMMON | PPC_OPCODE_32
#define	M601    PPC_OPCODE_POWER | PPC_OPCODE_601
#define PWRCOM	PPC_OPCODE_POWER | PPC_OPCODE_601 | PPC_OPCODE_COMMON
#define	MFDEC1	PPC_OPCODE_POWER
#define	MFDEC2	PPC_OPCODE_PPC | PPC_OPCODE_601 | PPC_OPCODE_BOOKE
#define BOOKE	PPC_OPCODE_BOOKE
#define BOOKE64	PPC_OPCODE_BOOKE64
#define CLASSIC	PPC_OPCODE_CLASSIC
#define PPCE300 PPC_OPCODE_E300
#define PPCSPE	PPC_OPCODE_SPE
#define PPCISEL	PPC_OPCODE_ISEL
#define PPCEFS	PPC_OPCODE_EFS
#define PPCBRLK	PPC_OPCODE_BRLOCK
#define PPCPMR	PPC_OPCODE_PMR
#define PPCCHLK	PPC_OPCODE_CACHELCK
#define PPCCHLK64	PPC_OPCODE_CACHELCK | PPC_OPCODE_BOOKE64
#define PPCRFMCI	PPC_OPCODE_RFMCI

/* The opcode table.

   The format of the opcode table is:

   NAME	     OPCODE	MASK		FLAGS		{ OPERANDS }

   NAME is the name of the instruction.
   OPCODE is the instruction opcode.
   MASK is the opcode mask; this is used to tell the disassembler
     which bits in the actual opcode must match OPCODE.
   FLAGS are flags indicated what processors support the instruction.
   OPERANDS is the list of operands.

   The disassembler reads the table in order and prints the first
   instruction which matches, so this table is sorted to put more
   specific instructions before more general instructions.  It is also
   sorted by major opcode.  */

const struct powerpc_opcode powerpc_opcodes[] = {

{ "tdlgti",  OPTO(2,TOLGT), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdllti",  OPTO(2,TOLLT), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdeqi",   OPTO(2,TOEQ), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdlgei",  OPTO(2,TOLGE), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdlnli",  OPTO(2,TOLNL), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdllei",  OPTO(2,TOLLE), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdlngi",  OPTO(2,TOLNG), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdgti",   OPTO(2,TOGT), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdgei",   OPTO(2,TOGE), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdnli",   OPTO(2,TONL), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdlti",   OPTO(2,TOLT), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdlei",   OPTO(2,TOLE), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdngi",   OPTO(2,TONG), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdnei",   OPTO(2,TONE), OPTO_MASK,	PPC64,		{ RA, SI } },
{ "tdi",     OP(2),	OP_MASK,	PPC64,		{ TO, RA, SI } },

{ "twlgti",  OPTO(3,TOLGT), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tlgti",   OPTO(3,TOLGT), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twllti",  OPTO(3,TOLLT), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tllti",   OPTO(3,TOLLT), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "tweqi",   OPTO(3,TOEQ), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "teqi",    OPTO(3,TOEQ), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twlgei",  OPTO(3,TOLGE), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tlgei",   OPTO(3,TOLGE), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twlnli",  OPTO(3,TOLNL), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tlnli",   OPTO(3,TOLNL), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twllei",  OPTO(3,TOLLE), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tllei",   OPTO(3,TOLLE), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twlngi",  OPTO(3,TOLNG), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tlngi",   OPTO(3,TOLNG), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twgti",   OPTO(3,TOGT), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tgti",    OPTO(3,TOGT), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twgei",   OPTO(3,TOGE), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tgei",    OPTO(3,TOGE), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twnli",   OPTO(3,TONL), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tnli",    OPTO(3,TONL), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twlti",   OPTO(3,TOLT), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tlti",    OPTO(3,TOLT), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twlei",   OPTO(3,TOLE), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tlei",    OPTO(3,TOLE), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twngi",   OPTO(3,TONG), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tngi",    OPTO(3,TONG), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twnei",   OPTO(3,TONE), OPTO_MASK,	PPCCOM,		{ RA, SI } },
{ "tnei",    OPTO(3,TONE), OPTO_MASK,	PWRCOM,		{ RA, SI } },
{ "twi",     OP(3),	OP_MASK,	PPCCOM,		{ TO, RA, SI } },
{ "ti",      OP(3),	OP_MASK,	PWRCOM,		{ TO, RA, SI } },

{ "mfvscr",  VX(4, 1540), VX_MASK,	PPCVEC,		{ VD } },
{ "mtvscr",  VX(4, 1604), VX_MASK,	PPCVEC,		{ VB } },
{ "vaddcuw", VX(4,  384), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vaddfp",  VX(4,   10), VX_MASK, 	PPCVEC,		{ VD, VA, VB } },
{ "vaddsbs", VX(4,  768), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vaddshs", VX(4,  832), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vaddsws", VX(4,  896), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vaddubm", VX(4,    0), VX_MASK, 	PPCVEC,		{ VD, VA, VB } },
{ "vaddubs", VX(4,  512), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vadduhm", VX(4,   64), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vadduhs", VX(4,  576), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vadduwm", VX(4,  128), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vadduws", VX(4,  640), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vand",    VX(4, 1028), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vandc",   VX(4, 1092), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vavgsb",  VX(4, 1282), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vavgsh",  VX(4, 1346), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vavgsw",  VX(4, 1410), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vavgub",  VX(4, 1026), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vavguh",  VX(4, 1090), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vavguw",  VX(4, 1154), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vcfsx",   VX(4,  842), VX_MASK,	PPCVEC,		{ VD, VB, UIMM } },
{ "vcfux",   VX(4,  778), VX_MASK,	PPCVEC,		{ VD, VB, UIMM } },
{ "vcmpbfp",   VXR(4, 966, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpbfp.",  VXR(4, 966, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpeqfp",  VXR(4, 198, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpeqfp.", VXR(4, 198, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpequb",  VXR(4,   6, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpequb.", VXR(4,   6, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpequh",  VXR(4,  70, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpequh.", VXR(4,  70, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpequw",  VXR(4, 134, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpequw.", VXR(4, 134, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgefp",  VXR(4, 454, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgefp.", VXR(4, 454, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtfp",  VXR(4, 710, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtfp.", VXR(4, 710, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtsb",  VXR(4, 774, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtsb.", VXR(4, 774, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtsh",  VXR(4, 838, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtsh.", VXR(4, 838, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtsw",  VXR(4, 902, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtsw.", VXR(4, 902, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtub",  VXR(4, 518, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtub.", VXR(4, 518, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtuh",  VXR(4, 582, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtuh.", VXR(4, 582, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtuw",  VXR(4, 646, 0), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vcmpgtuw.", VXR(4, 646, 1), VXR_MASK, PPCVEC,	{ VD, VA, VB } },
{ "vctsxs",    VX(4,  970), VX_MASK,	PPCVEC,		{ VD, VB, UIMM } },
{ "vctuxs",    VX(4,  906), VX_MASK,	PPCVEC,		{ VD, VB, UIMM } },
{ "vexptefp",  VX(4,  394), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vlogefp",   VX(4,  458), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vmaddfp",   VXA(4,  46), VXA_MASK,	PPCVEC,		{ VD, VA, VC, VB } },
{ "vmaxfp",    VX(4, 1034), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmaxsb",    VX(4,  258), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmaxsh",    VX(4,  322), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmaxsw",    VX(4,  386), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmaxub",    VX(4,    2), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmaxuh",    VX(4,   66), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmaxuw",    VX(4,  130), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmhaddshs", VXA(4,  32), VXA_MASK,	PPCVEC,		{ VD, VA, VB, VC } },
{ "vmhraddshs", VXA(4, 33), VXA_MASK,	PPCVEC,		{ VD, VA, VB, VC } },
{ "vminfp",    VX(4, 1098), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vminsb",    VX(4,  770), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vminsh",    VX(4,  834), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vminsw",    VX(4,  898), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vminub",    VX(4,  514), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vminuh",    VX(4,  578), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vminuw",    VX(4,  642), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmladduhm", VXA(4,  34), VXA_MASK,	PPCVEC,		{ VD, VA, VB, VC } },
{ "vmrghb",    VX(4,   12), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmrghh",    VX(4,   76), VX_MASK,    PPCVEC,		{ VD, VA, VB } },
{ "vmrghw",    VX(4,  140), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmrglb",    VX(4,  268), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmrglh",    VX(4,  332), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmrglw",    VX(4,  396), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmsummbm",  VXA(4,  37), VXA_MASK,	PPCVEC,		{ VD, VA, VB, VC } },
{ "vmsumshm",  VXA(4,  40), VXA_MASK,	PPCVEC,		{ VD, VA, VB, VC } },
{ "vmsumshs",  VXA(4,  41), VXA_MASK,	PPCVEC,		{ VD, VA, VB, VC } },
{ "vmsumubm",  VXA(4,  36), VXA_MASK,   PPCVEC,		{ VD, VA, VB, VC } },
{ "vmsumuhm",  VXA(4,  38), VXA_MASK,   PPCVEC,		{ VD, VA, VB, VC } },
{ "vmsumuhs",  VXA(4,  39), VXA_MASK,   PPCVEC,		{ VD, VA, VB, VC } },
{ "vmulesb",   VX(4,  776), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmulesh",   VX(4,  840), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmuleub",   VX(4,  520), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmuleuh",   VX(4,  584), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmulosb",   VX(4,  264), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmulosh",   VX(4,  328), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmuloub",   VX(4,    8), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmulouh",   VX(4,   72), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vnmsubfp",  VXA(4,  47), VXA_MASK,	PPCVEC,		{ VD, VA, VC, VB } },
{ "vnot",      VX(4, 1284), VX_MASK,	PPCVEC,		{ VD, VA, VAB } },
{ "vnor",      VX(4, 1284), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vmr",       VX(4, 1156), VX_MASK,	PPCVEC,		{ VD, VA, VAB } },
{ "vor",       VX(4, 1156), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vperm",     VXA(4,  43), VXA_MASK,	PPCVEC,		{ VD, VA, VB, VC } },
{ "vpkpx",     VX(4,  782), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vpkshss",   VX(4,  398), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vpkshus",   VX(4,  270), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vpkswss",   VX(4,  462), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vpkswus",   VX(4,  334), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vpkuhum",   VX(4,   14), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vpkuhus",   VX(4,  142), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vpkuwum",   VX(4,   78), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vpkuwus",   VX(4,  206), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vrefp",     VX(4,  266), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vrfim",     VX(4,  714), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vrfin",     VX(4,  522), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vrfip",     VX(4,  650), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vrfiz",     VX(4,  586), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vrlb",      VX(4,    4), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vrlh",      VX(4,   68), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vrlw",      VX(4,  132), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vrsqrtefp", VX(4,  330), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vsel",      VXA(4,  42), VXA_MASK,	PPCVEC,		{ VD, VA, VB, VC } },
{ "vsl",       VX(4,  452), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vslb",      VX(4,  260), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsldoi",    VXA(4,  44), VXA_MASK,	PPCVEC,		{ VD, VA, VB, SHB } },
{ "vslh",      VX(4,  324), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vslo",      VX(4, 1036), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vslw",      VX(4,  388), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vspltb",    VX(4,  524), VX_MASK,	PPCVEC,		{ VD, VB, UIMM } },
{ "vsplth",    VX(4,  588), VX_MASK,	PPCVEC,		{ VD, VB, UIMM } },
{ "vspltisb",  VX(4,  780), VX_MASK,	PPCVEC,		{ VD, SIMM } },
{ "vspltish",  VX(4,  844), VX_MASK,	PPCVEC,		{ VD, SIMM } },
{ "vspltisw",  VX(4,  908), VX_MASK,	PPCVEC,		{ VD, SIMM } },
{ "vspltw",    VX(4,  652), VX_MASK,	PPCVEC,		{ VD, VB, UIMM } },
{ "vsr",       VX(4,  708), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsrab",     VX(4,  772), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsrah",     VX(4,  836), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsraw",     VX(4,  900), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsrb",      VX(4,  516), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsrh",      VX(4,  580), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsro",      VX(4, 1100), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsrw",      VX(4,  644), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsubcuw",   VX(4, 1408), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsubfp",    VX(4,   74), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsubsbs",   VX(4, 1792), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsubshs",   VX(4, 1856), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsubsws",   VX(4, 1920), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsububm",   VX(4, 1024), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsububs",   VX(4, 1536), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsubuhm",   VX(4, 1088), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsubuhs",   VX(4, 1600), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsubuwm",   VX(4, 1152), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsubuws",   VX(4, 1664), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsumsws",   VX(4, 1928), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsum2sws",  VX(4, 1672), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsum4sbs",  VX(4, 1800), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsum4shs",  VX(4, 1608), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vsum4ubs",  VX(4, 1544), VX_MASK,	PPCVEC,		{ VD, VA, VB } },
{ "vupkhpx",   VX(4,  846), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vupkhsb",   VX(4,  526), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vupkhsh",   VX(4,  590), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vupklpx",   VX(4,  974), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vupklsb",   VX(4,  654), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vupklsh",   VX(4,  718), VX_MASK,	PPCVEC,		{ VD, VB } },
{ "vxor",      VX(4, 1220), VX_MASK,	PPCVEC,		{ VD, VA, VB } },

{ "vsldoi128", VX128_5(4,   16), VX128_5_MASK, PPCVEC, { VS128, VA128, VB128, SHB } },
{ "lvsl128",   VX128_1(4,    3), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "lvsr128",   VX128_1(4,   67), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "lvewx128",  VX128_1(4,  131), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "lvx128",    VX128_1(4,  195), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "stvewx128", VX128_1(4,  387), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "stvx128",   VX128_1(4,  451), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "lvxl128",   VX128_1(4,  707), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "stvxl128",  VX128_1(4,  963), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "lvlx128",   VX128_1(4, 1027), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "lvrx128",   VX128_1(4, 1091), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "stvlx128",  VX128_1(4, 1283), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "stvrx128",  VX128_1(4, 1347), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "lvlxl128",  VX128_1(4, 1539), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "lvrxl128",  VX128_1(4, 1603), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "stvlxl128", VX128_1(4, 1795), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },
{ "stvrxl128", VX128_1(4, 1859), VX128_1_MASK, PPCVEC, { VS128, RA, RB } },

{ "vperm128",	VX128_2(5, 0), VX128_2_MASK, PPCVEC, { VD128, VA128, VB128, VC128 } },
{ "vaddfp128",	VX128(5,  16), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vsubfp128",	VX128(5,  80), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vmulfp128",	VX128(5, 144), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vmaddfp128",	VX128(5, 208), VX128_MASK, PPCVEC, { VD128, VA128, VB128, VS128 } },
{ "vmaddcfp128",VX128(5, 272), VX128_MASK, PPCVEC, { VD128, VA128, VS128, VB128 } },
{ "vnmsubfp128",VX128(5, 336), VX128_MASK, PPCVEC, { VD128, VA128, VB128, VS128 } },
{ "vmsum3fp128",VX128(5, 400), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vmsum4fp128",VX128(5, 464), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vpkshss128", VX128(5, 512), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vand128",    VX128(5, 528), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vpkshus128", VX128(5, 576), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vandc128",   VX128(5, 592), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vpkswss128", VX128(5, 640), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vnor128",    VX128(5, 656), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vpkswus128", VX128(5, 704), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vor128",     VX128(5, 720), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vpkuhum128",	VX128(5, 768), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vxor128",	VX128(5, 784), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vpkuhus128",	VX128(5, 832), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vsel128",	VX128(5, 848), VX128_MASK, PPCVEC, { VD128, VA128, VB128, VS128 } },
{ "vpkuwum128",	VX128(5, 896), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vslo128",	VX128(5, 912), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vpkuwus128",	VX128(5, 960), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vsro128",	VX128(5, 976), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },

{ "vpermwi128",	 VX128_P(6,  528), VX128_P_MASK, PPCVEC, { VD128, VB128, VPERM128 } },
{ "vcfpsxws128", VX128_3(6,  560), VX128_3_MASK, PPCVEC, { VD128, VB128, SIMM } },
{ "vcfpuxws128", VX128_3(6,  624), VX128_3_MASK, PPCVEC, { VD128, VB128, UIMM } },
{ "vcsxwfp128",  VX128_3(6,  688), VX128_3_MASK, PPCVEC, { VD128, VB128, SIMM } },
{ "vcuxwfp128",  VX128_3(6,  752), VX128_3_MASK, PPCVEC, { VD128, VB128, UIMM } },
{ "vrfim128",	 VX128_3(6,  816), VX128_3_MASK, PPCVEC, { VD128, VB128 } },
{ "vrfin128",	 VX128_3(6,  880), VX128_3_MASK, PPCVEC, { VD128, VB128 } },
{ "vrfip128",	 VX128_3(6,  944), VX128_3_MASK, PPCVEC, { VD128, VB128 } },
{ "vrfiz128",	 VX128_3(6, 1008), VX128_3_MASK, PPCVEC, { VD128, VB128 } },
{ "vpkd3d128",	 VX128_4(6, 1552), VX128_4_MASK, PPCVEC, { VD128, VB128, VD3D0, VD3D1, VD3D2} },
{ "vrefp128",	 VX128_3(6, 1584), VX128_3_MASK, PPCVEC, { VD128, VB128 } },
{ "vrsqrtefp128",VX128_3(6, 1648), VX128_3_MASK, PPCVEC, { VD128, VB128 } },
{ "vexptefp128", VX128_3(6, 1712), VX128_3_MASK, PPCVEC, { VD128, VB128 } },
{ "vlogefp128",	 VX128_3(6, 1776), VX128_3_MASK, PPCVEC, { VD128, VB128 } },
{ "vrlimi128",	 VX128_4(6, 1808), VX128_4_MASK, PPCVEC, { VD128, VB128, UIMM, VD3D2} },
{ "vspltw128",	 VX128_3(6, 1840), VX128_3_MASK, PPCVEC, { VD128, VB128, UIMM } },
{ "vspltisw128", VX128_3(6, 1904), VX128_3_MASK, PPCVEC, { VD128, VB128, SIMM } },
{ "vupkd3d128",	 VX128_3(6, 2032), VX128_3_MASK, PPCVEC, { VD128, VB128, UIMM } },
{ "vcmpeqfp128", VX128(6,   0), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vcmpeqfp128.",VX128(6,  64), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vrlw128",     VX128(6,  80), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vcmpgefp128", VX128(6, 128), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vcmpgefp128.",VX128(6, 192), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vslw128",     VX128(6, 208), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vcmpgtfp128", VX128(6, 256), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vcmpgtfp128.",VX128(6, 320), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vsraw128",    VX128(6, 336), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vcmpbfp128",  VX128(6, 384), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vcmpbfp128.", VX128(6, 448), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vsrw128",     VX128(6, 464), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vcmpequw128", VX128(6, 512), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vcmpequw128.",VX128(6, 576), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vmaxfp128",   VX128(6, 640), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vminfp128",   VX128(6, 704), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vmrghw128",   VX128(6, 768), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vmrglw128",   VX128(6, 832), VX128_MASK, PPCVEC, { VD128, VA128, VB128 } },
{ "vupkhsb128",  VX128(6, 896), VX128_MASK, PPCVEC, { VD128, VB128 } },
{ "vupklsb128",  VX128(6, 960), VX128_MASK, PPCVEC, { VD128, VB128 } },

{ "mulli",   OP(7),	OP_MASK,	PPCCOM,		{ RT, RA, SI } },

{ "subfic",  OP(8),	OP_MASK,	PPCCOM,		{ RT, RA, SI } },

{ "cmplwi",  OPL(10,0),	OPL_MASK,	PPCCOM,		{ OBF, RA, UI } },
{ "cmpldi",  OPL(10,1), OPL_MASK,	PPC64,		{ OBF, RA, UI } },
{ "cmpli",   OP(10),	OP_MASK,	PPCONLY,	{ BF, L, RA, UI } },

{ "cmpwi",   OPL(11,0),	OPL_MASK,	PPCCOM,		{ OBF, RA, SI } },
{ "cmpdi",   OPL(11,1),	OPL_MASK,	PPC64,		{ OBF, RA, SI } },
{ "cmpi",    OP(11),	OP_MASK,	PPCONLY,	{ BF, L, RA, SI } },

{ "addic",   OP(12),	OP_MASK,	PPCCOM,		{ RT, RA, SI } },
{ "subic",   OP(12),	OP_MASK,	PPCCOM,		{ RT, RA, NSI } },

{ "addic.",  OP(13),	OP_MASK,	PPCCOM,		{ RT, RA, SI } },
{ "subic.",  OP(13),	OP_MASK,	PPCCOM,		{ RT, RA, NSI } },

{ "li",	     OP(14),	DRA_MASK,	PPCCOM,		{ RT, SI } },
{ "addi",    OP(14),	OP_MASK,	PPCCOM,		{ RT, RA, SI } },
{ "subi",    OP(14),	OP_MASK,	PPCCOM,		{ RT, RA, NSI } },
{ "la",	     OP(14),	OP_MASK,	PPCCOM,		{ RT, D, RA } },

{ "lis",     OP(15),	DRA_MASK,	PPCCOM,		{ RT, SISIGNOPT } },
{ "addis",   OP(15),	OP_MASK,	PPCCOM,		{ RT,RA,SISIGNOPT } },
{ "subis",   OP(15),	OP_MASK,	PPCCOM,		{ RT, RA, NSI } },

{ "bdnz-",   BBO(16,BODNZ,0,0), BBOATBI_MASK, PPCCOM,	{ BDM } },
{ "bdnz+",   BBO(16,BODNZ,0,0), BBOATBI_MASK, PPCCOM,	{ BDP } },
{ "bdnz",    BBO(16,BODNZ,0,0), BBOATBI_MASK, PPCCOM,	{ BD } },
{ "bdnzl-",  BBO(16,BODNZ,0,1), BBOATBI_MASK, PPCCOM,	{ BDM } },
{ "bdnzl+",  BBO(16,BODNZ,0,1), BBOATBI_MASK, PPCCOM,	{ BDP } },
{ "bdnzl",   BBO(16,BODNZ,0,1), BBOATBI_MASK, PPCCOM,	{ BD } },
{ "bdnza-",  BBO(16,BODNZ,1,0), BBOATBI_MASK, PPCCOM,	{ BDMA } },
{ "bdnza+",  BBO(16,BODNZ,1,0), BBOATBI_MASK, PPCCOM,	{ BDPA } },
{ "bdnza",   BBO(16,BODNZ,1,0), BBOATBI_MASK, PPCCOM,	{ BDA } },
{ "bdnzla-", BBO(16,BODNZ,1,1), BBOATBI_MASK, PPCCOM,	{ BDMA } },
{ "bdnzla+", BBO(16,BODNZ,1,1), BBOATBI_MASK, PPCCOM,	{ BDPA } },
{ "bdnzla",  BBO(16,BODNZ,1,1), BBOATBI_MASK, PPCCOM,	{ BDA } },
{ "bdz-",    BBO(16,BODZ,0,0),  BBOATBI_MASK, PPCCOM,	{ BDM } },
{ "bdz+",    BBO(16,BODZ,0,0),  BBOATBI_MASK, PPCCOM,	{ BDP } },
{ "bdz",     BBO(16,BODZ,0,0),  BBOATBI_MASK, COM,	{ BD } },
{ "bdzl-",   BBO(16,BODZ,0,1),  BBOATBI_MASK, PPCCOM,	{ BDM } },
{ "bdzl+",   BBO(16,BODZ,0,1),  BBOATBI_MASK, PPCCOM,	{ BDP } },
{ "bdzl",    BBO(16,BODZ,0,1),  BBOATBI_MASK, COM,	{ BD } },
{ "bdza-",   BBO(16,BODZ,1,0),  BBOATBI_MASK, PPCCOM,	{ BDMA } },
{ "bdza+",   BBO(16,BODZ,1,0),  BBOATBI_MASK, PPCCOM,	{ BDPA } },
{ "bdza",    BBO(16,BODZ,1,0),  BBOATBI_MASK, COM,	{ BDA } },
{ "bdzla-",  BBO(16,BODZ,1,1),  BBOATBI_MASK, PPCCOM,	{ BDMA } },
{ "bdzla+",  BBO(16,BODZ,1,1),  BBOATBI_MASK, PPCCOM,	{ BDPA } },
{ "bdzla",   BBO(16,BODZ,1,1),  BBOATBI_MASK, COM,	{ BDA } },
{ "blt-",    BBOCB(16,BOT,CBLT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "blt+",    BBOCB(16,BOT,CBLT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "blt",     BBOCB(16,BOT,CBLT,0,0), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bltl-",   BBOCB(16,BOT,CBLT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bltl+",   BBOCB(16,BOT,CBLT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bltl",    BBOCB(16,BOT,CBLT,0,1), BBOATCB_MASK, COM,		{ CR, BD } },
{ "blta-",   BBOCB(16,BOT,CBLT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "blta+",   BBOCB(16,BOT,CBLT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "blta",    BBOCB(16,BOT,CBLT,1,0), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bltla-",  BBOCB(16,BOT,CBLT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bltla+",  BBOCB(16,BOT,CBLT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bltla",   BBOCB(16,BOT,CBLT,1,1), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bgt-",    BBOCB(16,BOT,CBGT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bgt+",    BBOCB(16,BOT,CBGT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bgt",     BBOCB(16,BOT,CBGT,0,0), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bgtl-",   BBOCB(16,BOT,CBGT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bgtl+",   BBOCB(16,BOT,CBGT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bgtl",    BBOCB(16,BOT,CBGT,0,1), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bgta-",   BBOCB(16,BOT,CBGT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bgta+",   BBOCB(16,BOT,CBGT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bgta",    BBOCB(16,BOT,CBGT,1,0), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bgtla-",  BBOCB(16,BOT,CBGT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bgtla+",  BBOCB(16,BOT,CBGT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bgtla",   BBOCB(16,BOT,CBGT,1,1), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "beq-",    BBOCB(16,BOT,CBEQ,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "beq+",    BBOCB(16,BOT,CBEQ,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "beq",     BBOCB(16,BOT,CBEQ,0,0), BBOATCB_MASK, COM,		{ CR, BD } },
{ "beql-",   BBOCB(16,BOT,CBEQ,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "beql+",   BBOCB(16,BOT,CBEQ,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "beql",    BBOCB(16,BOT,CBEQ,0,1), BBOATCB_MASK, COM,		{ CR, BD } },
{ "beqa-",   BBOCB(16,BOT,CBEQ,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "beqa+",   BBOCB(16,BOT,CBEQ,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "beqa",    BBOCB(16,BOT,CBEQ,1,0), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "beqla-",  BBOCB(16,BOT,CBEQ,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "beqla+",  BBOCB(16,BOT,CBEQ,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "beqla",   BBOCB(16,BOT,CBEQ,1,1), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bso-",    BBOCB(16,BOT,CBSO,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bso+",    BBOCB(16,BOT,CBSO,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bso",     BBOCB(16,BOT,CBSO,0,0), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bsol-",   BBOCB(16,BOT,CBSO,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bsol+",   BBOCB(16,BOT,CBSO,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bsol",    BBOCB(16,BOT,CBSO,0,1), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bsoa-",   BBOCB(16,BOT,CBSO,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bsoa+",   BBOCB(16,BOT,CBSO,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bsoa",    BBOCB(16,BOT,CBSO,1,0), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bsola-",  BBOCB(16,BOT,CBSO,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bsola+",  BBOCB(16,BOT,CBSO,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bsola",   BBOCB(16,BOT,CBSO,1,1), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bun-",    BBOCB(16,BOT,CBSO,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bun+",    BBOCB(16,BOT,CBSO,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bun",     BBOCB(16,BOT,CBSO,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BD } },
{ "bunl-",   BBOCB(16,BOT,CBSO,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bunl+",   BBOCB(16,BOT,CBSO,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bunl",    BBOCB(16,BOT,CBSO,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BD } },
{ "buna-",   BBOCB(16,BOT,CBSO,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "buna+",   BBOCB(16,BOT,CBSO,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "buna",    BBOCB(16,BOT,CBSO,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDA } },
{ "bunla-",  BBOCB(16,BOT,CBSO,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bunla+",  BBOCB(16,BOT,CBSO,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bunla",   BBOCB(16,BOT,CBSO,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDA } },
{ "bge-",    BBOCB(16,BOF,CBLT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bge+",    BBOCB(16,BOF,CBLT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bge",     BBOCB(16,BOF,CBLT,0,0), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bgel-",   BBOCB(16,BOF,CBLT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bgel+",   BBOCB(16,BOF,CBLT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bgel",    BBOCB(16,BOF,CBLT,0,1), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bgea-",   BBOCB(16,BOF,CBLT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bgea+",   BBOCB(16,BOF,CBLT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bgea",    BBOCB(16,BOF,CBLT,1,0), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bgela-",  BBOCB(16,BOF,CBLT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bgela+",  BBOCB(16,BOF,CBLT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bgela",   BBOCB(16,BOF,CBLT,1,1), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bnl-",    BBOCB(16,BOF,CBLT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bnl+",    BBOCB(16,BOF,CBLT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bnl",     BBOCB(16,BOF,CBLT,0,0), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bnll-",   BBOCB(16,BOF,CBLT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bnll+",   BBOCB(16,BOF,CBLT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bnll",    BBOCB(16,BOF,CBLT,0,1), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bnla-",   BBOCB(16,BOF,CBLT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bnla+",   BBOCB(16,BOF,CBLT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bnla",    BBOCB(16,BOF,CBLT,1,0), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bnlla-",  BBOCB(16,BOF,CBLT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bnlla+",  BBOCB(16,BOF,CBLT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bnlla",   BBOCB(16,BOF,CBLT,1,1), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "ble-",    BBOCB(16,BOF,CBGT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "ble+",    BBOCB(16,BOF,CBGT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "ble",     BBOCB(16,BOF,CBGT,0,0), BBOATCB_MASK, COM,		{ CR, BD } },
{ "blel-",   BBOCB(16,BOF,CBGT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "blel+",   BBOCB(16,BOF,CBGT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "blel",    BBOCB(16,BOF,CBGT,0,1), BBOATCB_MASK, COM,		{ CR, BD } },
{ "blea-",   BBOCB(16,BOF,CBGT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "blea+",   BBOCB(16,BOF,CBGT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "blea",    BBOCB(16,BOF,CBGT,1,0), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "blela-",  BBOCB(16,BOF,CBGT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "blela+",  BBOCB(16,BOF,CBGT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "blela",   BBOCB(16,BOF,CBGT,1,1), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bng-",    BBOCB(16,BOF,CBGT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bng+",    BBOCB(16,BOF,CBGT,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bng",     BBOCB(16,BOF,CBGT,0,0), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bngl-",   BBOCB(16,BOF,CBGT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bngl+",   BBOCB(16,BOF,CBGT,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bngl",    BBOCB(16,BOF,CBGT,0,1), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bnga-",   BBOCB(16,BOF,CBGT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bnga+",   BBOCB(16,BOF,CBGT,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bnga",    BBOCB(16,BOF,CBGT,1,0), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bngla-",  BBOCB(16,BOF,CBGT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bngla+",  BBOCB(16,BOF,CBGT,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bngla",   BBOCB(16,BOF,CBGT,1,1), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bne-",    BBOCB(16,BOF,CBEQ,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bne+",    BBOCB(16,BOF,CBEQ,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bne",     BBOCB(16,BOF,CBEQ,0,0), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bnel-",   BBOCB(16,BOF,CBEQ,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bnel+",   BBOCB(16,BOF,CBEQ,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bnel",    BBOCB(16,BOF,CBEQ,0,1), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bnea-",   BBOCB(16,BOF,CBEQ,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bnea+",   BBOCB(16,BOF,CBEQ,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bnea",    BBOCB(16,BOF,CBEQ,1,0), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bnela-",  BBOCB(16,BOF,CBEQ,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bnela+",  BBOCB(16,BOF,CBEQ,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bnela",   BBOCB(16,BOF,CBEQ,1,1), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bns-",    BBOCB(16,BOF,CBSO,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bns+",    BBOCB(16,BOF,CBSO,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bns",     BBOCB(16,BOF,CBSO,0,0), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bnsl-",   BBOCB(16,BOF,CBSO,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bnsl+",   BBOCB(16,BOF,CBSO,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bnsl",    BBOCB(16,BOF,CBSO,0,1), BBOATCB_MASK, COM,		{ CR, BD } },
{ "bnsa-",   BBOCB(16,BOF,CBSO,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bnsa+",   BBOCB(16,BOF,CBSO,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bnsa",    BBOCB(16,BOF,CBSO,1,0), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bnsla-",  BBOCB(16,BOF,CBSO,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bnsla+",  BBOCB(16,BOF,CBSO,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bnsla",   BBOCB(16,BOF,CBSO,1,1), BBOATCB_MASK, COM,		{ CR, BDA } },
{ "bnu-",    BBOCB(16,BOF,CBSO,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bnu+",    BBOCB(16,BOF,CBSO,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bnu",     BBOCB(16,BOF,CBSO,0,0), BBOATCB_MASK, PPCCOM,	{ CR, BD } },
{ "bnul-",   BBOCB(16,BOF,CBSO,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDM } },
{ "bnul+",   BBOCB(16,BOF,CBSO,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BDP } },
{ "bnul",    BBOCB(16,BOF,CBSO,0,1), BBOATCB_MASK, PPCCOM,	{ CR, BD } },
{ "bnua-",   BBOCB(16,BOF,CBSO,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bnua+",   BBOCB(16,BOF,CBSO,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bnua",    BBOCB(16,BOF,CBSO,1,0), BBOATCB_MASK, PPCCOM,	{ CR, BDA } },
{ "bnula-",  BBOCB(16,BOF,CBSO,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDMA } },
{ "bnula+",  BBOCB(16,BOF,CBSO,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDPA } },
{ "bnula",   BBOCB(16,BOF,CBSO,1,1), BBOATCB_MASK, PPCCOM,	{ CR, BDA } },
{ "bdnzt-",  BBO(16,BODNZT,0,0), BBOY_MASK, NOPOWER4,	{ BI, BDM } },
{ "bdnzt+",  BBO(16,BODNZT,0,0), BBOY_MASK, NOPOWER4,	{ BI, BDP } },
{ "bdnzt",   BBO(16,BODNZT,0,0), BBOY_MASK, PPCCOM,	{ BI, BD } },
{ "bdnztl-", BBO(16,BODNZT,0,1), BBOY_MASK, NOPOWER4,	{ BI, BDM } },
{ "bdnztl+", BBO(16,BODNZT,0,1), BBOY_MASK, NOPOWER4,	{ BI, BDP } },
{ "bdnztl",  BBO(16,BODNZT,0,1), BBOY_MASK, PPCCOM,	{ BI, BD } },
{ "bdnzta-", BBO(16,BODNZT,1,0), BBOY_MASK, NOPOWER4,	{ BI, BDMA } },
{ "bdnzta+", BBO(16,BODNZT,1,0), BBOY_MASK, NOPOWER4,	{ BI, BDPA } },
{ "bdnzta",  BBO(16,BODNZT,1,0), BBOY_MASK, PPCCOM,	{ BI, BDA } },
{ "bdnztla-",BBO(16,BODNZT,1,1), BBOY_MASK, NOPOWER4,	{ BI, BDMA } },
{ "bdnztla+",BBO(16,BODNZT,1,1), BBOY_MASK, NOPOWER4,	{ BI, BDPA } },
{ "bdnztla", BBO(16,BODNZT,1,1), BBOY_MASK, PPCCOM,	{ BI, BDA } },
{ "bdnzf-",  BBO(16,BODNZF,0,0), BBOY_MASK, NOPOWER4,	{ BI, BDM } },
{ "bdnzf+",  BBO(16,BODNZF,0,0), BBOY_MASK, NOPOWER4,	{ BI, BDP } },
{ "bdnzf",   BBO(16,BODNZF,0,0), BBOY_MASK, PPCCOM,	{ BI, BD } },
{ "bdnzfl-", BBO(16,BODNZF,0,1), BBOY_MASK, NOPOWER4,	{ BI, BDM } },
{ "bdnzfl+", BBO(16,BODNZF,0,1), BBOY_MASK, NOPOWER4,	{ BI, BDP } },
{ "bdnzfl",  BBO(16,BODNZF,0,1), BBOY_MASK, PPCCOM,	{ BI, BD } },
{ "bdnzfa-", BBO(16,BODNZF,1,0), BBOY_MASK, NOPOWER4,	{ BI, BDMA } },
{ "bdnzfa+", BBO(16,BODNZF,1,0), BBOY_MASK, NOPOWER4,	{ BI, BDPA } },
{ "bdnzfa",  BBO(16,BODNZF,1,0), BBOY_MASK, PPCCOM,	{ BI, BDA } },
{ "bdnzfla-",BBO(16,BODNZF,1,1), BBOY_MASK, NOPOWER4,	{ BI, BDMA } },
{ "bdnzfla+",BBO(16,BODNZF,1,1), BBOY_MASK, NOPOWER4,	{ BI, BDPA } },
{ "bdnzfla", BBO(16,BODNZF,1,1), BBOY_MASK, PPCCOM,	{ BI, BDA } },
{ "bt-",     BBO(16,BOT,0,0), BBOAT_MASK, PPCCOM,	{ BI, BDM } },
{ "bt+",     BBO(16,BOT,0,0), BBOAT_MASK, PPCCOM,	{ BI, BDP } },
{ "bt",	     BBO(16,BOT,0,0), BBOAT_MASK, PPCCOM,	{ BI, BD } },
{ "btl-",    BBO(16,BOT,0,1), BBOAT_MASK, PPCCOM,	{ BI, BDM } },
{ "btl+",    BBO(16,BOT,0,1), BBOAT_MASK, PPCCOM,	{ BI, BDP } },
{ "btl",     BBO(16,BOT,0,1), BBOAT_MASK, PPCCOM,	{ BI, BD } },
{ "bta-",    BBO(16,BOT,1,0), BBOAT_MASK, PPCCOM,	{ BI, BDMA } },
{ "bta+",    BBO(16,BOT,1,0), BBOAT_MASK, PPCCOM,	{ BI, BDPA } },
{ "bta",     BBO(16,BOT,1,0), BBOAT_MASK, PPCCOM,	{ BI, BDA } },
{ "btla-",   BBO(16,BOT,1,1), BBOAT_MASK, PPCCOM,	{ BI, BDMA } },
{ "btla+",   BBO(16,BOT,1,1), BBOAT_MASK, PPCCOM,	{ BI, BDPA } },
{ "btla",    BBO(16,BOT,1,1), BBOAT_MASK, PPCCOM,	{ BI, BDA } },
{ "bf-",     BBO(16,BOF,0,0), BBOAT_MASK, PPCCOM,	{ BI, BDM } },
{ "bf+",     BBO(16,BOF,0,0), BBOAT_MASK, PPCCOM,	{ BI, BDP } },
{ "bf",	     BBO(16,BOF,0,0), BBOAT_MASK, PPCCOM,	{ BI, BD } },
{ "bfl-",    BBO(16,BOF,0,1), BBOAT_MASK, PPCCOM,	{ BI, BDM } },
{ "bfl+",    BBO(16,BOF,0,1), BBOAT_MASK, PPCCOM,	{ BI, BDP } },
{ "bfl",     BBO(16,BOF,0,1), BBOAT_MASK, PPCCOM,	{ BI, BD } },
{ "bfa-",    BBO(16,BOF,1,0), BBOAT_MASK, PPCCOM,	{ BI, BDMA } },
{ "bfa+",    BBO(16,BOF,1,0), BBOAT_MASK, PPCCOM,	{ BI, BDPA } },
{ "bfa",     BBO(16,BOF,1,0), BBOAT_MASK, PPCCOM,	{ BI, BDA } },
{ "bfla-",   BBO(16,BOF,1,1), BBOAT_MASK, PPCCOM,	{ BI, BDMA } },
{ "bfla+",   BBO(16,BOF,1,1), BBOAT_MASK, PPCCOM,	{ BI, BDPA } },
{ "bfla",    BBO(16,BOF,1,1), BBOAT_MASK, PPCCOM,	{ BI, BDA } },
{ "bdzt-",   BBO(16,BODZT,0,0), BBOY_MASK, NOPOWER4,	{ BI, BDM } },
{ "bdzt+",   BBO(16,BODZT,0,0), BBOY_MASK, NOPOWER4,	{ BI, BDP } },
{ "bdzt",    BBO(16,BODZT,0,0), BBOY_MASK, PPCCOM,	{ BI, BD } },
{ "bdztl-",  BBO(16,BODZT,0,1), BBOY_MASK, NOPOWER4,	{ BI, BDM } },
{ "bdztl+",  BBO(16,BODZT,0,1), BBOY_MASK, NOPOWER4,	{ BI, BDP } },
{ "bdztl",   BBO(16,BODZT,0,1), BBOY_MASK, PPCCOM,	{ BI, BD } },
{ "bdzta-",  BBO(16,BODZT,1,0), BBOY_MASK, NOPOWER4,	{ BI, BDMA } },
{ "bdzta+",  BBO(16,BODZT,1,0), BBOY_MASK, NOPOWER4,	{ BI, BDPA } },
{ "bdzta",   BBO(16,BODZT,1,0), BBOY_MASK, PPCCOM,	{ BI, BDA } },
{ "bdztla-", BBO(16,BODZT,1,1), BBOY_MASK, NOPOWER4,	{ BI, BDMA } },
{ "bdztla+", BBO(16,BODZT,1,1), BBOY_MASK, NOPOWER4,	{ BI, BDPA } },
{ "bdztla",  BBO(16,BODZT,1,1), BBOY_MASK, PPCCOM,	{ BI, BDA } },
{ "bdzf-",   BBO(16,BODZF,0,0), BBOY_MASK, NOPOWER4,	{ BI, BDM } },
{ "bdzf+",   BBO(16,BODZF,0,0), BBOY_MASK, NOPOWER4,	{ BI, BDP } },
{ "bdzf",    BBO(16,BODZF,0,0), BBOY_MASK, PPCCOM,	{ BI, BD } },
{ "bdzfl-",  BBO(16,BODZF,0,1), BBOY_MASK, NOPOWER4,	{ BI, BDM } },
{ "bdzfl+",  BBO(16,BODZF,0,1), BBOY_MASK, NOPOWER4,	{ BI, BDP } },
{ "bdzfl",   BBO(16,BODZF,0,1), BBOY_MASK, PPCCOM,	{ BI, BD } },
{ "bdzfa-",  BBO(16,BODZF,1,0), BBOY_MASK, NOPOWER4,	{ BI, BDMA } },
{ "bdzfa+",  BBO(16,BODZF,1,0), BBOY_MASK, NOPOWER4,	{ BI, BDPA } },
{ "bdzfa",   BBO(16,BODZF,1,0), BBOY_MASK, PPCCOM,	{ BI, BDA } },
{ "bdzfla-", BBO(16,BODZF,1,1), BBOY_MASK, NOPOWER4,	{ BI, BDMA } },
{ "bdzfla+", BBO(16,BODZF,1,1), BBOY_MASK, NOPOWER4,	{ BI, BDPA } },
{ "bdzfla",  BBO(16,BODZF,1,1), BBOY_MASK, PPCCOM,	{ BI, BDA } },
{ "bc-",     B(16,0,0),	B_MASK,		PPCCOM,		{ BOE, BI, BDM } },
{ "bc+",     B(16,0,0),	B_MASK,		PPCCOM,		{ BOE, BI, BDP } },
{ "bc",	     B(16,0,0),	B_MASK,		COM,		{ BO, BI, BD } },
{ "bcl-",    B(16,0,1),	B_MASK,		PPCCOM,		{ BOE, BI, BDM } },
{ "bcl+",    B(16,0,1),	B_MASK,		PPCCOM,		{ BOE, BI, BDP } },
{ "bcl",     B(16,0,1),	B_MASK,		COM,		{ BO, BI, BD } },
{ "bca-",    B(16,1,0),	B_MASK,		PPCCOM,		{ BOE, BI, BDMA } },
{ "bca+",    B(16,1,0),	B_MASK,		PPCCOM,		{ BOE, BI, BDPA } },
{ "bca",     B(16,1,0),	B_MASK,		COM,		{ BO, BI, BDA } },
{ "bcla-",   B(16,1,1),	B_MASK,		PPCCOM,		{ BOE, BI, BDMA } },
{ "bcla+",   B(16,1,1),	B_MASK,		PPCCOM,		{ BOE, BI, BDPA } },
{ "bcla",    B(16,1,1),	B_MASK,		COM,		{ BO, BI, BDA } },

{ "sc",      SC(17,1,0), 0xffffffff,	PPC,		{ 0 } },

{ "b",	     B(18,0,0),	B_MASK,		COM,		{ LI } },
{ "bl",      B(18,0,1),	B_MASK,		COM,		{ LI } },
{ "ba",      B(18,1,0),	B_MASK,		COM,		{ LIA } },
{ "bla",     B(18,1,1),	B_MASK,		COM,		{ LIA } },

{ "mcrf",    XL(19,0),	XLBB_MASK|(3<<21)|(3<<16), COM,	{ BF, BFA } },

{ "blr",     XLO(19,BOU,16,0), XLBOBIBB_MASK, PPCCOM,	{ 0 } },
{ "blrl",    XLO(19,BOU,16,1), XLBOBIBB_MASK, PPCCOM,	{ 0 } },
{ "bdnzlr",  XLO(19,BODNZ,16,0), XLBOBIBB_MASK, PPCCOM,	{ 0 } },
{ "bdnzlr-", XLO(19,BODNZ,16,0), XLBOBIBB_MASK, NOPOWER4,	{ 0 } },
{ "bdnzlr+", XLO(19,BODNZP,16,0), XLBOBIBB_MASK, NOPOWER4,	{ 0 } },
{ "bdnzlr-", XLO(19,BODNZM4,16,0), XLBOBIBB_MASK, POWER4,	{ 0 } },
{ "bdnzlr+", XLO(19,BODNZP4,16,0), XLBOBIBB_MASK, POWER4,	{ 0 } },
{ "bdnzlrl", XLO(19,BODNZ,16,1), XLBOBIBB_MASK, PPCCOM,	{ 0 } },
{ "bdnzlrl-",XLO(19,BODNZ,16,1), XLBOBIBB_MASK, NOPOWER4,	{ 0 } },
{ "bdnzlrl+",XLO(19,BODNZP,16,1), XLBOBIBB_MASK, NOPOWER4,	{ 0 } },
{ "bdnzlrl-",XLO(19,BODNZM4,16,1), XLBOBIBB_MASK, POWER4,	{ 0 } },
{ "bdnzlrl+",XLO(19,BODNZP4,16,1), XLBOBIBB_MASK, POWER4,	{ 0 } },
{ "bdzlr",   XLO(19,BODZ,16,0), XLBOBIBB_MASK, PPCCOM,	{ 0 } },
{ "bdzlr-",  XLO(19,BODZ,16,0), XLBOBIBB_MASK, NOPOWER4,	{ 0 } },
{ "bdzlr+",  XLO(19,BODZP,16,0), XLBOBIBB_MASK, NOPOWER4,	{ 0 } },
{ "bdzlr-",  XLO(19,BODZM4,16,0), XLBOBIBB_MASK, POWER4,	{ 0 } },
{ "bdzlr+",  XLO(19,BODZP4,16,0), XLBOBIBB_MASK, POWER4,	{ 0 } },
{ "bdzlrl",  XLO(19,BODZ,16,1), XLBOBIBB_MASK, PPCCOM,	{ 0 } },
{ "bdzlrl-", XLO(19,BODZ,16,1), XLBOBIBB_MASK, NOPOWER4,	{ 0 } },
{ "bdzlrl+", XLO(19,BODZP,16,1), XLBOBIBB_MASK, NOPOWER4,	{ 0 } },
{ "bdzlrl-", XLO(19,BODZM4,16,1), XLBOBIBB_MASK, POWER4,	{ 0 } },
{ "bdzlrl+", XLO(19,BODZP4,16,1), XLBOBIBB_MASK, POWER4,	{ 0 } },
{ "bltlr",   XLOCB(19,BOT,CBLT,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bltlr-",  XLOCB(19,BOT,CBLT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bltlr+",  XLOCB(19,BOTP,CBLT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bltlr-",  XLOCB(19,BOTM4,CBLT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bltlr+",  XLOCB(19,BOTP4,CBLT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bltr",    XLOCB(19,BOT,CBLT,16,0), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bltlrl",  XLOCB(19,BOT,CBLT,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bltlrl-", XLOCB(19,BOT,CBLT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bltlrl+", XLOCB(19,BOTP,CBLT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bltlrl-", XLOCB(19,BOTM4,CBLT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bltlrl+", XLOCB(19,BOTP4,CBLT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bltrl",   XLOCB(19,BOT,CBLT,16,1), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bgtlr",   XLOCB(19,BOT,CBGT,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bgtlr-",  XLOCB(19,BOT,CBGT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgtlr+",  XLOCB(19,BOTP,CBGT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgtlr-",  XLOCB(19,BOTM4,CBGT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgtlr+",  XLOCB(19,BOTP4,CBGT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgtr",    XLOCB(19,BOT,CBGT,16,0), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bgtlrl",  XLOCB(19,BOT,CBGT,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bgtlrl-", XLOCB(19,BOT,CBGT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgtlrl+", XLOCB(19,BOTP,CBGT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgtlrl-", XLOCB(19,BOTM4,CBGT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgtlrl+", XLOCB(19,BOTP4,CBGT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgtrl",   XLOCB(19,BOT,CBGT,16,1), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "beqlr",   XLOCB(19,BOT,CBEQ,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "beqlr-",  XLOCB(19,BOT,CBEQ,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "beqlr+",  XLOCB(19,BOTP,CBEQ,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "beqlr-",  XLOCB(19,BOTM4,CBEQ,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "beqlr+",  XLOCB(19,BOTP4,CBEQ,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "beqr",    XLOCB(19,BOT,CBEQ,16,0), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "beqlrl",  XLOCB(19,BOT,CBEQ,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "beqlrl-", XLOCB(19,BOT,CBEQ,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "beqlrl+", XLOCB(19,BOTP,CBEQ,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "beqlrl-", XLOCB(19,BOTM4,CBEQ,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "beqlrl+", XLOCB(19,BOTP4,CBEQ,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "beqrl",   XLOCB(19,BOT,CBEQ,16,1), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bsolr",   XLOCB(19,BOT,CBSO,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bsolr-",  XLOCB(19,BOT,CBSO,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bsolr+",  XLOCB(19,BOTP,CBSO,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bsolr-",  XLOCB(19,BOTM4,CBSO,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bsolr+",  XLOCB(19,BOTP4,CBSO,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bsor",    XLOCB(19,BOT,CBSO,16,0), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bsolrl",  XLOCB(19,BOT,CBSO,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bsolrl-", XLOCB(19,BOT,CBSO,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bsolrl+", XLOCB(19,BOTP,CBSO,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bsolrl-", XLOCB(19,BOTM4,CBSO,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bsolrl+", XLOCB(19,BOTP4,CBSO,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bsorl",   XLOCB(19,BOT,CBSO,16,1), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bunlr",   XLOCB(19,BOT,CBSO,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bunlr-",  XLOCB(19,BOT,CBSO,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bunlr+",  XLOCB(19,BOTP,CBSO,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bunlr-",  XLOCB(19,BOTM4,CBSO,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bunlr+",  XLOCB(19,BOTP4,CBSO,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bunlrl",  XLOCB(19,BOT,CBSO,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bunlrl-", XLOCB(19,BOT,CBSO,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bunlrl+", XLOCB(19,BOTP,CBSO,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bunlrl-", XLOCB(19,BOTM4,CBSO,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bunlrl+", XLOCB(19,BOTP4,CBSO,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgelr",   XLOCB(19,BOF,CBLT,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bgelr-",  XLOCB(19,BOF,CBLT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgelr+",  XLOCB(19,BOFP,CBLT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgelr-",  XLOCB(19,BOFM4,CBLT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgelr+",  XLOCB(19,BOFP4,CBLT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bger",    XLOCB(19,BOF,CBLT,16,0), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bgelrl",  XLOCB(19,BOF,CBLT,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bgelrl-", XLOCB(19,BOF,CBLT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgelrl+", XLOCB(19,BOFP,CBLT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgelrl-", XLOCB(19,BOFM4,CBLT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgelrl+", XLOCB(19,BOFP4,CBLT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgerl",   XLOCB(19,BOF,CBLT,16,1), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bnllr",   XLOCB(19,BOF,CBLT,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bnllr-",  XLOCB(19,BOF,CBLT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnllr+",  XLOCB(19,BOFP,CBLT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnllr-",  XLOCB(19,BOFM4,CBLT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnllr+",  XLOCB(19,BOFP4,CBLT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnlr",    XLOCB(19,BOF,CBLT,16,0), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bnllrl",  XLOCB(19,BOF,CBLT,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bnllrl-", XLOCB(19,BOF,CBLT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnllrl+", XLOCB(19,BOFP,CBLT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnllrl-", XLOCB(19,BOFM4,CBLT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnllrl+", XLOCB(19,BOFP4,CBLT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnlrl",   XLOCB(19,BOF,CBLT,16,1), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "blelr",   XLOCB(19,BOF,CBGT,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "blelr-",  XLOCB(19,BOF,CBGT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "blelr+",  XLOCB(19,BOFP,CBGT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "blelr-",  XLOCB(19,BOFM4,CBGT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "blelr+",  XLOCB(19,BOFP4,CBGT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bler",    XLOCB(19,BOF,CBGT,16,0), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "blelrl",  XLOCB(19,BOF,CBGT,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "blelrl-", XLOCB(19,BOF,CBGT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "blelrl+", XLOCB(19,BOFP,CBGT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "blelrl-", XLOCB(19,BOFM4,CBGT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "blelrl+", XLOCB(19,BOFP4,CBGT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "blerl",   XLOCB(19,BOF,CBGT,16,1), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bnglr",   XLOCB(19,BOF,CBGT,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bnglr-",  XLOCB(19,BOF,CBGT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnglr+",  XLOCB(19,BOFP,CBGT,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnglr-",  XLOCB(19,BOFM4,CBGT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnglr+",  XLOCB(19,BOFP4,CBGT,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bngr",    XLOCB(19,BOF,CBGT,16,0), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bnglrl",  XLOCB(19,BOF,CBGT,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bnglrl-", XLOCB(19,BOF,CBGT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnglrl+", XLOCB(19,BOFP,CBGT,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnglrl-", XLOCB(19,BOFM4,CBGT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnglrl+", XLOCB(19,BOFP4,CBGT,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bngrl",   XLOCB(19,BOF,CBGT,16,1), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bnelr",   XLOCB(19,BOF,CBEQ,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bnelr-",  XLOCB(19,BOF,CBEQ,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnelr+",  XLOCB(19,BOFP,CBEQ,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnelr-",  XLOCB(19,BOFM4,CBEQ,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnelr+",  XLOCB(19,BOFP4,CBEQ,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bner",    XLOCB(19,BOF,CBEQ,16,0), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bnelrl",  XLOCB(19,BOF,CBEQ,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bnelrl-", XLOCB(19,BOF,CBEQ,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnelrl+", XLOCB(19,BOFP,CBEQ,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnelrl-", XLOCB(19,BOFM4,CBEQ,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnelrl+", XLOCB(19,BOFP4,CBEQ,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnerl",   XLOCB(19,BOF,CBEQ,16,1), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bnslr",   XLOCB(19,BOF,CBSO,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bnslr-",  XLOCB(19,BOF,CBSO,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnslr+",  XLOCB(19,BOFP,CBSO,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnslr-",  XLOCB(19,BOFM4,CBSO,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnslr+",  XLOCB(19,BOFP4,CBSO,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnsr",    XLOCB(19,BOF,CBSO,16,0), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bnslrl",  XLOCB(19,BOF,CBSO,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bnslrl-", XLOCB(19,BOF,CBSO,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnslrl+", XLOCB(19,BOFP,CBSO,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnslrl-", XLOCB(19,BOFM4,CBSO,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnslrl+", XLOCB(19,BOFP4,CBSO,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnsrl",   XLOCB(19,BOF,CBSO,16,1), XLBOCBBB_MASK, PWRCOM, { CR } },
{ "bnulr",   XLOCB(19,BOF,CBSO,16,0), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bnulr-",  XLOCB(19,BOF,CBSO,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnulr+",  XLOCB(19,BOFP,CBSO,16,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnulr-",  XLOCB(19,BOFM4,CBSO,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnulr+",  XLOCB(19,BOFP4,CBSO,16,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnulrl",  XLOCB(19,BOF,CBSO,16,1), XLBOCBBB_MASK, PPCCOM, { CR } },
{ "bnulrl-", XLOCB(19,BOF,CBSO,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnulrl+", XLOCB(19,BOFP,CBSO,16,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnulrl-", XLOCB(19,BOFM4,CBSO,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnulrl+", XLOCB(19,BOFP4,CBSO,16,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "btlr",    XLO(19,BOT,16,0), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "btlr-",   XLO(19,BOT,16,0), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "btlr+",   XLO(19,BOTP,16,0), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "bbtr",    XLO(19,BOT,16,0), XLBOBB_MASK, PWRCOM,	{ BI } },
{ "btlrl",   XLO(19,BOT,16,1), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "btlrl-",  XLO(19,BOT,16,1), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "btlrl+",  XLO(19,BOTP,16,1), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "bbtrl",   XLO(19,BOT,16,1), XLBOBB_MASK, PWRCOM,	{ BI } },
{ "bflr",    XLO(19,BOF,16,0), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bflr-",   XLO(19,BOF,16,0), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "bflr+",   XLO(19,BOFP,16,0), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "bflrl",   XLO(19,BOF,16,1), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bflrl-",  XLO(19,BOF,16,1), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "bflrl+",  XLO(19,BOFP,16,1), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "bflrl-",  XLO(19,BOFM4,16,1), XLBOBB_MASK, POWER4,	{ BI } },
{ "bflrl+",  XLO(19,BOFP4,16,1), XLBOBB_MASK, POWER4,	{ BI } },
{ "bdnztlr", XLO(19,BODNZT,16,0), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bdnztlr-",XLO(19,BODNZT,16,0), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdnztlr+",XLO(19,BODNZTP,16,0), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdnztlrl",XLO(19,BODNZT,16,1), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bdnztlrl-",XLO(19,BODNZT,16,1), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdnztlrl+",XLO(19,BODNZTP,16,1), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdnzflr", XLO(19,BODNZF,16,0), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bdnzflr-",XLO(19,BODNZF,16,0), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdnzflr+",XLO(19,BODNZFP,16,0), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdnzflrl",XLO(19,BODNZF,16,1), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bdnzflrl-",XLO(19,BODNZF,16,1), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdnzflrl+",XLO(19,BODNZFP,16,1), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdztlr",  XLO(19,BODZT,16,0), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bdztlr-", XLO(19,BODZT,16,0), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "bdztlr+", XLO(19,BODZTP,16,0), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdztlrl", XLO(19,BODZT,16,1), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bdztlrl-",XLO(19,BODZT,16,1), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "bdztlrl+",XLO(19,BODZTP,16,1), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdzflr",  XLO(19,BODZF,16,0), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bdzflr-", XLO(19,BODZF,16,0), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "bdzflr+", XLO(19,BODZFP,16,0), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bdzflrl", XLO(19,BODZF,16,1), XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bdzflrl-",XLO(19,BODZF,16,1), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "bdzflrl+",XLO(19,BODZFP,16,1), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bclr",    XLLK(19,16,0), XLYBB_MASK,	PPCCOM,		{ BO, BI } },
{ "bclrl",   XLLK(19,16,1), XLYBB_MASK,	PPCCOM,		{ BO, BI } },
{ "bclr+",   XLYLK(19,16,1,0), XLYBB_MASK, PPCCOM,	{ BOE, BI } },
{ "bclrl+",  XLYLK(19,16,1,1), XLYBB_MASK, PPCCOM,	{ BOE, BI } },
{ "bclr-",   XLYLK(19,16,0,0), XLYBB_MASK, PPCCOM,	{ BOE, BI } },
{ "bclrl-",  XLYLK(19,16,0,1), XLYBB_MASK, PPCCOM,	{ BOE, BI } },
{ "bcr",     XLLK(19,16,0), XLBB_MASK,	PWRCOM,		{ BO, BI } },
{ "bcrl",    XLLK(19,16,1), XLBB_MASK,	PWRCOM,		{ BO, BI } },

{ "rfid",    XL(19,18),	0xffffffff,	PPC64,		{ 0 } },

{ "crnot",   XL(19,33), XL_MASK,	PPCCOM,		{ BT, BA, BBA } },
{ "crnor",   XL(19,33),	XL_MASK,	COM,		{ BT, BA, BB } },
// XXX
{ "rfmci",   X(19,38),  0xffffffff,	PPCRFMCI,	{ 0 } },

{ "rfi",     XL(19,50),	0xffffffff,	COM,		{ 0 } },

{ "crandc",  XL(19,129), XL_MASK,	COM,		{ BT, BA, BB } },

{ "isync",   XL(19,150), 0xffffffff,	PPCCOM,		{ 0 } },

{ "crclr",   XL(19,193), XL_MASK,	PPCCOM,		{ BT, BAT, BBA } },
{ "crxor",   XL(19,193), XL_MASK,	COM,		{ BT, BA, BB } },

{ "crnand",  XL(19,225), XL_MASK,	COM,		{ BT, BA, BB } },

{ "crand",   XL(19,257), XL_MASK,	COM,		{ BT, BA, BB } },

{ "crset",   XL(19,289), XL_MASK,	PPCCOM,		{ BT, BAT, BBA } },
{ "creqv",   XL(19,289), XL_MASK,	COM,		{ BT, BA, BB } },

{ "crorc",   XL(19,417), XL_MASK,	COM,		{ BT, BA, BB } },

{ "crmove",  XL(19,449), XL_MASK,	PPCCOM,		{ BT, BA, BBA } },
{ "cror",    XL(19,449), XL_MASK,	COM,		{ BT, BA, BB } },

{ "bctr",    XLO(19,BOU,528,0), XLBOBIBB_MASK, COM,	{ 0 } },
{ "bctrl",   XLO(19,BOU,528,1), XLBOBIBB_MASK, COM,	{ 0 } },
{ "bltctr",  XLOCB(19,BOT,CBLT,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bltctr-", XLOCB(19,BOT,CBLT,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bltctr+", XLOCB(19,BOTP,CBLT,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bltctr-", XLOCB(19,BOTM4,CBLT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bltctr+", XLOCB(19,BOTP4,CBLT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bltctrl", XLOCB(19,BOT,CBLT,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bltctrl-",XLOCB(19,BOT,CBLT,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bltctrl+",XLOCB(19,BOTP,CBLT,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bltctrl-",XLOCB(19,BOTM4,CBLT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bltctrl+",XLOCB(19,BOTP4,CBLT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgtctr",  XLOCB(19,BOT,CBGT,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bgtctr-", XLOCB(19,BOT,CBGT,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgtctr+", XLOCB(19,BOTP,CBGT,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgtctr-", XLOCB(19,BOTM4,CBGT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgtctr+", XLOCB(19,BOTP4,CBGT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgtctrl", XLOCB(19,BOT,CBGT,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bgtctrl-",XLOCB(19,BOT,CBGT,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgtctrl+",XLOCB(19,BOTP,CBGT,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgtctrl-",XLOCB(19,BOTM4,CBGT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgtctrl+",XLOCB(19,BOTP4,CBGT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "beqctr",  XLOCB(19,BOT,CBEQ,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "beqctr-", XLOCB(19,BOT,CBEQ,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "beqctr+", XLOCB(19,BOTP,CBEQ,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "beqctr-", XLOCB(19,BOTM4,CBEQ,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "beqctr+", XLOCB(19,BOTP4,CBEQ,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "beqctrl", XLOCB(19,BOT,CBEQ,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "beqctrl-",XLOCB(19,BOT,CBEQ,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "beqctrl+",XLOCB(19,BOTP,CBEQ,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "beqctrl-",XLOCB(19,BOTM4,CBEQ,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "beqctrl+",XLOCB(19,BOTP4,CBEQ,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bsoctr",  XLOCB(19,BOT,CBSO,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bsoctr-", XLOCB(19,BOT,CBSO,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bsoctr+", XLOCB(19,BOTP,CBSO,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bsoctr-", XLOCB(19,BOTM4,CBSO,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bsoctr+", XLOCB(19,BOTP4,CBSO,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bsoctrl", XLOCB(19,BOT,CBSO,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bsoctrl-",XLOCB(19,BOT,CBSO,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bsoctrl+",XLOCB(19,BOTP,CBSO,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bsoctrl-",XLOCB(19,BOTM4,CBSO,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bsoctrl+",XLOCB(19,BOTP4,CBSO,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bunctr",  XLOCB(19,BOT,CBSO,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bunctr-", XLOCB(19,BOT,CBSO,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bunctr+", XLOCB(19,BOTP,CBSO,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bunctr-", XLOCB(19,BOTM4,CBSO,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bunctr+", XLOCB(19,BOTP4,CBSO,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bunctrl", XLOCB(19,BOT,CBSO,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bunctrl-",XLOCB(19,BOT,CBSO,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bunctrl+",XLOCB(19,BOTP,CBSO,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bunctrl-",XLOCB(19,BOTM4,CBSO,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bunctrl+",XLOCB(19,BOTP4,CBSO,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgectr",  XLOCB(19,BOF,CBLT,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bgectr-", XLOCB(19,BOF,CBLT,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgectr+", XLOCB(19,BOFP,CBLT,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgectr-", XLOCB(19,BOFM4,CBLT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgectr+", XLOCB(19,BOFP4,CBLT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgectrl", XLOCB(19,BOF,CBLT,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bgectrl-",XLOCB(19,BOF,CBLT,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgectrl+",XLOCB(19,BOFP,CBLT,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bgectrl-",XLOCB(19,BOFM4,CBLT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bgectrl+",XLOCB(19,BOFP4,CBLT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnlctr",  XLOCB(19,BOF,CBLT,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bnlctr-", XLOCB(19,BOF,CBLT,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnlctr+", XLOCB(19,BOFP,CBLT,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnlctr-", XLOCB(19,BOFM4,CBLT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnlctr+", XLOCB(19,BOFP4,CBLT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnlctrl", XLOCB(19,BOF,CBLT,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bnlctrl-",XLOCB(19,BOF,CBLT,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnlctrl+",XLOCB(19,BOFP,CBLT,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnlctrl-",XLOCB(19,BOFM4,CBLT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnlctrl+",XLOCB(19,BOFP4,CBLT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "blectr",  XLOCB(19,BOF,CBGT,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "blectr-", XLOCB(19,BOF,CBGT,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "blectr+", XLOCB(19,BOFP,CBGT,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "blectr-", XLOCB(19,BOFM4,CBGT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "blectr+", XLOCB(19,BOFP4,CBGT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "blectrl", XLOCB(19,BOF,CBGT,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "blectrl-",XLOCB(19,BOF,CBGT,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "blectrl+",XLOCB(19,BOFP,CBGT,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "blectrl-",XLOCB(19,BOFM4,CBGT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "blectrl+",XLOCB(19,BOFP4,CBGT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bngctr",  XLOCB(19,BOF,CBGT,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bngctr-", XLOCB(19,BOF,CBGT,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bngctr+", XLOCB(19,BOFP,CBGT,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bngctr-", XLOCB(19,BOFM4,CBGT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bngctr+", XLOCB(19,BOFP4,CBGT,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bngctrl", XLOCB(19,BOF,CBGT,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bngctrl-",XLOCB(19,BOF,CBGT,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bngctrl+",XLOCB(19,BOFP,CBGT,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bngctrl-",XLOCB(19,BOFM4,CBGT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bngctrl+",XLOCB(19,BOFP4,CBGT,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnectr",  XLOCB(19,BOF,CBEQ,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bnectr-", XLOCB(19,BOF,CBEQ,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnectr+", XLOCB(19,BOFP,CBEQ,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnectr-", XLOCB(19,BOFM4,CBEQ,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnectr+", XLOCB(19,BOFP4,CBEQ,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnectrl", XLOCB(19,BOF,CBEQ,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bnectrl-",XLOCB(19,BOF,CBEQ,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnectrl+",XLOCB(19,BOFP,CBEQ,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnectrl-",XLOCB(19,BOFM4,CBEQ,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnectrl+",XLOCB(19,BOFP4,CBEQ,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnsctr",  XLOCB(19,BOF,CBSO,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bnsctr-", XLOCB(19,BOF,CBSO,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnsctr+", XLOCB(19,BOFP,CBSO,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnsctr-", XLOCB(19,BOFM4,CBSO,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnsctr+", XLOCB(19,BOFP4,CBSO,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnsctrl", XLOCB(19,BOF,CBSO,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bnsctrl-",XLOCB(19,BOF,CBSO,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnsctrl+",XLOCB(19,BOFP,CBSO,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnsctrl-",XLOCB(19,BOFM4,CBSO,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnsctrl+",XLOCB(19,BOFP4,CBSO,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnuctr",  XLOCB(19,BOF,CBSO,528,0),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bnuctr-", XLOCB(19,BOF,CBSO,528,0),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnuctr+", XLOCB(19,BOFP,CBSO,528,0), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnuctr-", XLOCB(19,BOFM4,CBSO,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnuctr+", XLOCB(19,BOFP4,CBSO,528,0), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnuctrl", XLOCB(19,BOF,CBSO,528,1),  XLBOCBBB_MASK, PPCCOM,	{ CR } },
{ "bnuctrl-",XLOCB(19,BOF,CBSO,528,1),  XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnuctrl+",XLOCB(19,BOFP,CBSO,528,1), XLBOCBBB_MASK, NOPOWER4, { CR } },
{ "bnuctrl-",XLOCB(19,BOFM4,CBSO,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "bnuctrl+",XLOCB(19,BOFP4,CBSO,528,1), XLBOCBBB_MASK, POWER4, { CR } },
{ "btctr",   XLO(19,BOT,528,0),  XLBOBB_MASK, PPCCOM,	{ BI } },
{ "btctr-",  XLO(19,BOT,528,0),  XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "btctr+",  XLO(19,BOTP,528,0), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "btctr-",  XLO(19,BOTM4,528,0), XLBOBB_MASK, POWER4, { BI } },
{ "btctr+",  XLO(19,BOTP4,528,0), XLBOBB_MASK, POWER4, { BI } },
{ "btctrl",  XLO(19,BOT,528,1),  XLBOBB_MASK, PPCCOM,	{ BI } },
{ "btctrl-", XLO(19,BOT,528,1),  XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "btctrl+", XLO(19,BOTP,528,1), XLBOBB_MASK, NOPOWER4,	{ BI } },
{ "btctrl-", XLO(19,BOTM4,528,1), XLBOBB_MASK, POWER4, { BI } },
{ "btctrl+", XLO(19,BOTP4,528,1), XLBOBB_MASK, POWER4, { BI } },
{ "bfctr",   XLO(19,BOF,528,0),  XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bfctr-",  XLO(19,BOF,528,0),  XLBOBB_MASK, NOPOWER4, { BI } },
{ "bfctr+",  XLO(19,BOFP,528,0), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bfctr-",  XLO(19,BOFM4,528,0), XLBOBB_MASK, POWER4, { BI } },
{ "bfctr+",  XLO(19,BOFP4,528,0), XLBOBB_MASK, POWER4, { BI } },
{ "bfctrl",  XLO(19,BOF,528,1),  XLBOBB_MASK, PPCCOM,	{ BI } },
{ "bfctrl-", XLO(19,BOF,528,1),  XLBOBB_MASK, NOPOWER4, { BI } },
{ "bfctrl+", XLO(19,BOFP,528,1), XLBOBB_MASK, NOPOWER4, { BI } },
{ "bfctrl-", XLO(19,BOFM4,528,1), XLBOBB_MASK, POWER4, { BI } },
{ "bfctrl+", XLO(19,BOFP4,528,1), XLBOBB_MASK, POWER4, { BI } },
{ "bcctr",   XLLK(19,528,0),     XLYBB_MASK,  PPCCOM,	{ BO, BI } },
{ "bcctr-",  XLYLK(19,528,0,0),  XLYBB_MASK,  PPCCOM,	{ BOE, BI } },
{ "bcctr+",  XLYLK(19,528,1,0),  XLYBB_MASK,  PPCCOM,	{ BOE, BI } },
{ "bcctrl",  XLLK(19,528,1),     XLYBB_MASK,  PPCCOM,	{ BO, BI } },
{ "bcctrl-", XLYLK(19,528,0,1),  XLYBB_MASK,  PPCCOM,	{ BOE, BI } },
{ "bcctrl+", XLYLK(19,528,1,1),  XLYBB_MASK,  PPCCOM,	{ BOE, BI } },
{ "bcc",     XLLK(19,528,0),     XLBB_MASK,   PWRCOM,	{ BO, BI } },
{ "bccl",    XLLK(19,528,1),     XLBB_MASK,   PWRCOM,	{ BO, BI } },
{ "bcctre",  XLLK(19,529,0),     XLYBB_MASK,  BOOKE64,	{ BO, BI } },
{ "bcctrel", XLLK(19,529,1),     XLYBB_MASK,  BOOKE64,	{ BO, BI } },

{ "rlwimi",  M(20,0),	M_MASK,		PPCCOM,		{ RA,RS,SH,MBE,ME } },

{ "rlwimi.", M(20,1),	M_MASK,		PPCCOM,		{ RA,RS,SH,MBE,ME } },

{ "rotlwi",  MME(21,31,0), MMBME_MASK,	PPCCOM,		{ RA, RS, SH } },
{ "clrlwi",  MME(21,31,0), MSHME_MASK,	PPCCOM,		{ RA, RS, MB } },
{ "slwi",    M(21, 0), M_MASK,    	PPCCOM,		{ RA, RS, SH, MSLWI } },
{ "srwi",    M(21, 0), M_MASK,    	PPCCOM,		{ RA, RS, MB, MSRWI } },
{ "rlwinm",  M(21,0),	M_MASK,		PPCCOM,		{ RA,RS,SH,MBE,ME } },
{ "rotlwi.", MME(21,31,1), MMBME_MASK,	PPCCOM,		{ RA,RS,SH } },
{ "clrlwi.", MME(21,31,1), MSHME_MASK,	PPCCOM,		{ RA, RS, MB } },
{ "slwi.",   M(21,1), M_MASK,     	PPCCOM,		{ RA, RS, SH, MSLWI } },
{ "srwi.",   M(21,1), M_MASK,    	PPCCOM,		{ RA, RS, MB, MSRWI } },
{ "rlwinm.", M(21,1),	M_MASK,		PPCCOM,		{ RA,RS,SH,MBE,ME } },

{ "rotlw",   MME(23,31,0), MMBME_MASK,	PPCCOM,		{ RA, RS, RB } },
{ "rlwnm",   M(23,0),	M_MASK,		PPCCOM,		{ RA,RS,RB,MBE,ME } },
{ "rotlw.",  MME(23,31,1), MMBME_MASK,	PPCCOM,		{ RA, RS, RB } },
{ "rlwnm.",  M(23,1),	M_MASK,		PPCCOM,		{ RA,RS,RB,MBE,ME } },

{ "nop",     OP(24),	0xffffffff,	PPCCOM,		{ 0 } },
{ "ori",     OP(24),	OP_MASK,	PPCCOM,		{ RA, RS, UI } },

{ "oris",    OP(25),	OP_MASK,	PPCCOM,		{ RA, RS, UI } },

{ "xori",    OP(26),	OP_MASK,	PPCCOM,		{ RA, RS, UI } },

{ "xoris",   OP(27),	OP_MASK,	PPCCOM,		{ RA, RS, UI } },

{ "andi.",   OP(28),	OP_MASK,	PPCCOM,		{ RA, RS, UI } },

{ "andis.",  OP(29),	OP_MASK,	PPCCOM,		{ RA, RS, UI } },

{ "rotldi",  MD(30,0,0), MDMB_MASK,	PPC64,		{ RA, RS, SH6 } },
{ "clrldi",  MD(30,0,0), MDSH_MASK,	PPC64,		{ RA, RS, MB6 } },
{ "rldicl",  MD(30,0,0), MD_MASK,	PPC64,		{ RA, RS, SH6, MB6 } },
{ "rotldi.", MD(30,0,1), MDMB_MASK,	PPC64,		{ RA, RS, SH6 } },
{ "clrldi.", MD(30,0,1), MDSH_MASK,	PPC64,		{ RA, RS, MB6 } },
{ "rldicl.", MD(30,0,1), MD_MASK,	PPC64,		{ RA, RS, SH6, MB6 } },

{ "rldicr",  MD(30,1,0), MD_MASK,	PPC64,		{ RA, RS, SH6, ME6 } },
{ "rldicr.", MD(30,1,1), MD_MASK,	PPC64,		{ RA, RS, SH6, ME6 } },

{ "rldic",   MD(30,2,0), MD_MASK,	PPC64,		{ RA, RS, SH6, MB6 } },
{ "rldic.",  MD(30,2,1), MD_MASK,	PPC64,		{ RA, RS, SH6, MB6 } },

{ "rldimi",  MD(30,3,0), MD_MASK,	PPC64,		{ RA, RS, SH6, MB6 } },
{ "rldimi.", MD(30,3,1), MD_MASK,	PPC64,		{ RA, RS, SH6, MB6 } },

{ "rotld",   MDS(30,8,0), MDSMB_MASK,	PPC64,		{ RA, RS, RB } },
{ "rldcl",   MDS(30,8,0), MDS_MASK,	PPC64,		{ RA, RS, RB, MB6 } },
{ "rotld.",  MDS(30,8,1), MDSMB_MASK,	PPC64,		{ RA, RS, RB } },
{ "rldcl.",  MDS(30,8,1), MDS_MASK,	PPC64,		{ RA, RS, RB, MB6 } },

{ "rldcr",   MDS(30,9,0), MDS_MASK,	PPC64,		{ RA, RS, RB, ME6 } },
{ "rldcr.",  MDS(30,9,1), MDS_MASK,	PPC64,		{ RA, RS, RB, ME6 } },

{ "cmpw",    XCMPL(31,0,0), XCMPL_MASK, PPCCOM,		{ OBF, RA, RB } },
{ "cmpd",    XOPL(31,0,1), XCMPL_MASK,  PPC64,		{ OBF, RA, RB } },
{ "cmp",     X(31,0),	XCMP_MASK,	PPCONLY,	{ BF, L, RA, RB } },

{ "twlgt",   XTO(31,4,TOLGT), XTO_MASK, PPCCOM,		{ RA, RB } },
{ "twllt",   XTO(31,4,TOLLT), XTO_MASK, PPCCOM,		{ RA, RB } },
{ "tweq",    XTO(31,4,TOEQ), XTO_MASK,	PPCCOM,		{ RA, RB } },
{ "twlge",   XTO(31,4,TOLGE), XTO_MASK, PPCCOM,		{ RA, RB } },
{ "twlnl",   XTO(31,4,TOLNL), XTO_MASK, PPCCOM,		{ RA, RB } },
{ "twlle",   XTO(31,4,TOLLE), XTO_MASK, PPCCOM,		{ RA, RB } },
{ "twlng",   XTO(31,4,TOLNG), XTO_MASK, PPCCOM,		{ RA, RB } },
{ "twgt",    XTO(31,4,TOGT), XTO_MASK,	PPCCOM,		{ RA, RB } },
{ "twge",    XTO(31,4,TOGE), XTO_MASK,	PPCCOM,		{ RA, RB } },
{ "twnl",    XTO(31,4,TONL), XTO_MASK,	PPCCOM,		{ RA, RB } },
{ "twlt",    XTO(31,4,TOLT), XTO_MASK,	PPCCOM,		{ RA, RB } },
{ "twle",    XTO(31,4,TOLE), XTO_MASK,	PPCCOM,		{ RA, RB } },
{ "twng",    XTO(31,4,TONG), XTO_MASK,	PPCCOM,		{ RA, RB } },
{ "twne",    XTO(31,4,TONE), XTO_MASK,	PPCCOM,		{ RA, RB } },
{ "trap",    XTO(31,4,TOU), 0xffffffff,	PPCCOM,		{ 0 } },
{ "tw",      X(31,4),	X_MASK,		PPCCOM,		{ TO, RA, RB } },

{ "subfc",   XO(31,8,0,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "subc",    XO(31,8,0,0), XO_MASK,	PPC,		{ RT, RB, RA } },
{ "subfc.",  XO(31,8,0,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "subc.",   XO(31,8,0,1), XO_MASK,	PPCCOM,		{ RT, RB, RA } },
{ "subfco",  XO(31,8,1,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "subco",   XO(31,8,1,0), XO_MASK,	PPC,		{ RT, RB, RA } },
{ "subfco.", XO(31,8,1,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "subco.",  XO(31,8,1,1), XO_MASK,	PPC,		{ RT, RB, RA } },

{ "mulhdu",  XO(31,9,0,0), XO_MASK,	PPC64,		{ RT, RA, RB } },
{ "mulhdu.", XO(31,9,0,1), XO_MASK,	PPC64,		{ RT, RA, RB } },

{ "addc",    XO(31,10,0,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "addc.",   XO(31,10,0,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "addco",   XO(31,10,1,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "addco.",  XO(31,10,1,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },

{ "mulhwu",  XO(31,11,0,0), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "mulhwu.", XO(31,11,0,1), XO_MASK,	PPC,		{ RT, RA, RB } },

{ "mfcr",    X(31,19),	XRARB_MASK,	COM,		{ RT } },
{ "mfocrf",  X(31,19),	X_MASK,		PPCVEC,		{ RT, FXM } },

{ "lwarx",   X(31,20),	X_MASK,		PPC,		{ RT, RA, RB } },

{ "ldx",     X(31,21),	X_MASK,		PPC64,		{ RT, RA0, RB } },

{ "lwzx",    X(31,23),	X_MASK,		PPCCOM,		{ RT, RA, RB } },

{ "slw",     XRC(31,24,0), X_MASK,	PPCCOM,		{ RA, RS, RB } },
{ "slw.",    XRC(31,24,1), X_MASK,	PPCCOM,		{ RA, RS, RB } },

{ "cntlzw",  XRC(31,26,0), XRB_MASK,	PPCCOM,		{ RA, RS } },
{ "cntlzw.", XRC(31,26,1), XRB_MASK,	PPCCOM,		{ RA, RS } },

{ "sld",     XRC(31,27,0), X_MASK,	PPC64,		{ RA, RS, RB } },
{ "sld.",    XRC(31,27,1), X_MASK,	PPC64,		{ RA, RS, RB } },

{ "and",     XRC(31,28,0), X_MASK,	COM,		{ RA, RS, RB } },
{ "and.",    XRC(31,28,1), X_MASK,	COM,		{ RA, RS, RB } },

{ "cmplw",   XCMPL(31,32,0), XCMPL_MASK, PPCCOM,	{ OBF, RA, RB } },
{ "cmpld",   XOPL(31,32,1), XCMPL_MASK,  PPC64,		{ OBF, RA, RB } },
{ "cmpl",    X(31,32),	XCMP_MASK,	 PPCONLY,	{ BF, L, RA, RB } },

{ "subf",    XO(31,40,0,0), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "sub",     XO(31,40,0,0), XO_MASK,	PPC,		{ RT, RB, RA } },
{ "subf.",   XO(31,40,0,1), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "sub.",    XO(31,40,0,1), XO_MASK,	PPC,		{ RT, RB, RA } },
{ "subfo",   XO(31,40,1,0), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "subo",    XO(31,40,1,0), XO_MASK,	PPC,		{ RT, RB, RA } },
{ "subfo.",  XO(31,40,1,1), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "subo.",   XO(31,40,1,1), XO_MASK,	PPC,		{ RT, RB, RA } },

{ "ldux",    X(31,53),	X_MASK,		PPC64,		{ RT, RAL, RB } },

{ "dcbst",   X(31,54),	XRT_MASK,	PPC,		{ RA, RB } },

{ "lwzux",   X(31,55),	X_MASK,		PPCCOM,		{ RT, RAL, RB } },

{ "cntlzd",  XRC(31,58,0), XRB_MASK,	PPC64,		{ RA, RS } },
{ "cntlzd.", XRC(31,58,1), XRB_MASK,	PPC64,		{ RA, RS } },

{ "andc",    XRC(31,60,0), X_MASK,	COM,		{ RA, RS, RB } },
{ "andc.",   XRC(31,60,1), X_MASK,	COM,		{ RA, RS, RB } },

{ "tdlgt",   XTO(31,68,TOLGT), XTO_MASK, PPC64,		{ RA, RB } },
{ "tdllt",   XTO(31,68,TOLLT), XTO_MASK, PPC64,		{ RA, RB } },
{ "tdeq",    XTO(31,68,TOEQ), XTO_MASK,  PPC64,		{ RA, RB } },
{ "tdlge",   XTO(31,68,TOLGE), XTO_MASK, PPC64,		{ RA, RB } },
{ "tdlnl",   XTO(31,68,TOLNL), XTO_MASK, PPC64,		{ RA, RB } },
{ "tdlle",   XTO(31,68,TOLLE), XTO_MASK, PPC64,		{ RA, RB } },
{ "tdlng",   XTO(31,68,TOLNG), XTO_MASK, PPC64,		{ RA, RB } },
{ "tdgt",    XTO(31,68,TOGT), XTO_MASK,  PPC64,		{ RA, RB } },
{ "tdge",    XTO(31,68,TOGE), XTO_MASK,  PPC64,		{ RA, RB } },
{ "tdnl",    XTO(31,68,TONL), XTO_MASK,  PPC64,		{ RA, RB } },
{ "tdlt",    XTO(31,68,TOLT), XTO_MASK,  PPC64,		{ RA, RB } },
{ "tdle",    XTO(31,68,TOLE), XTO_MASK,  PPC64,		{ RA, RB } },
{ "tdng",    XTO(31,68,TONG), XTO_MASK,  PPC64,		{ RA, RB } },
{ "tdne",    XTO(31,68,TONE), XTO_MASK,  PPC64,		{ RA, RB } },
{ "td",	     X(31,68),	X_MASK,		 PPC64,		{ TO, RA, RB } },

{ "mulhd",   XO(31,73,0,0), XO_MASK,	 PPC64,		{ RT, RA, RB } },
{ "mulhd.",  XO(31,73,0,1), XO_MASK,	 PPC64,		{ RT, RA, RB } },

{ "mulhw",   XO(31,75,0,0), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "mulhw.",  XO(31,75,0,1), XO_MASK,	PPC,		{ RT, RA, RB } },

{ "mtsrd",   X(31,82),	XRB_MASK|(1<<20), PPC64,	{ SR, RS } },

{ "mfmsr",   X(31,83),	XRARB_MASK,	COM,		{ RT } },

{ "ldarx",   X(31,84),	X_MASK,		PPC64,		{ RT, RA0, RB } },

{ "dcbf",    X(31,86),	XRT_MASK,	PPC,		{ RA, RB } },

{ "lbzx",    X(31,87),	X_MASK,		COM,		{ RT, RA, RB } },

{ "neg",     XO(31,104,0,0), XORB_MASK,	COM,		{ RT, RA } },
{ "neg.",    XO(31,104,0,1), XORB_MASK,	COM,		{ RT, RA } },
{ "nego",    XO(31,104,1,0), XORB_MASK,	COM,		{ RT, RA } },
{ "nego.",   XO(31,104,1,1), XORB_MASK,	COM,		{ RT, RA } },

{ "mtsrdin", X(31,114),	XRA_MASK,	PPC64,		{ RS, RB } },

{ "lbzux",   X(31,119),	X_MASK,		COM,		{ RT, RAL, RB } },

{ "not",     XRC(31,124,0), X_MASK,	COM,		{ RA, RS, RBS } },
{ "nor",     XRC(31,124,0), X_MASK,	COM,		{ RA, RS, RB } },
{ "not.",    XRC(31,124,1), X_MASK,	COM,		{ RA, RS, RBS } },
{ "nor.",    XRC(31,124,1), X_MASK,	COM,		{ RA, RS, RB } },

{ "subfe",   XO(31,136,0,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "subfe.",  XO(31,136,0,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "subfeo",  XO(31,136,1,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "subfeo.", XO(31,136,1,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },

{ "adde",    XO(31,138,0,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "adde.",   XO(31,138,0,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "addeo",   XO(31,138,1,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "addeo.",  XO(31,138,1,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },

{ "mtcr",    XFXM(31,144,0xff), XFXFXM_MASK|FXM_MASK, COM,	{ RS }},
{ "mtcrf",   X(31,144),	XFXFXM_MASK,	COM,		{ FXM, RS } },
{ "mtocrf",  X(31,144)|(1<<20),XFXFXM_MASK2,	COM,	{ FXM, RS } },

{ "mtmsr",   X(31,146),	XRARB_MASK,	COM,		{ RS } },

{ "stdx",    X(31,149), X_MASK,		PPC64,		{ RS, RA0, RB } },

{ "stwcx.",  XRC(31,150,1), X_MASK,	PPC,		{ RS, RA, RB } },

{ "stwx",    X(31,151), X_MASK,		PPCCOM,		{ RS, RA, RB } },

{ "mtmsrd",  X(31,178),	XRLARB_MASK,	PPC64,		{ RS, MTMSRD_L } },

{ "stdux",   X(31,181),	X_MASK,		PPC64,		{ RS, RAS, RB } },

{ "stwux",   X(31,183),	X_MASK,		PPCCOM,		{ RS, RAS, RB } },

{ "subfze",  XO(31,200,0,0), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "subfze.", XO(31,200,0,1), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "subfzeo", XO(31,200,1,0), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "subfzeo.",XO(31,200,1,1), XORB_MASK, PPCCOM,		{ RT, RA } },

{ "addze",   XO(31,202,0,0), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "addze.",  XO(31,202,0,1), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "addzeo",  XO(31,202,1,0), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "addzeo.", XO(31,202,1,1), XORB_MASK, PPCCOM,		{ RT, RA } },

{ "mtsr",    X(31,210),	XRB_MASK|(1<<20), COM32,	{ SR, RS } },

{ "stdcx.",  XRC(31,214,1), X_MASK,	PPC64,		{ RS, RA, RB } },

{ "stbx",    X(31,215),	X_MASK,		COM,		{ RS, RA, RB } },

{ "subfme",  XO(31,232,0,0), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "subfme.", XO(31,232,0,1), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "subfmeo", XO(31,232,1,0), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "subfmeo.",XO(31,232,1,1), XORB_MASK, PPCCOM,		{ RT, RA } },

{ "mulld",   XO(31,233,0,0), XO_MASK,	PPC64,		{ RT, RA, RB } },
{ "mulld.",  XO(31,233,0,1), XO_MASK,	PPC64,		{ RT, RA, RB } },
{ "mulldo",  XO(31,233,1,0), XO_MASK,	PPC64,		{ RT, RA, RB } },
{ "mulldo.", XO(31,233,1,1), XO_MASK,	PPC64,		{ RT, RA, RB } },

{ "addme",   XO(31,234,0,0), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "addme.",  XO(31,234,0,1), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "addmeo",  XO(31,234,1,0), XORB_MASK, PPCCOM,		{ RT, RA } },
{ "addmeo.", XO(31,234,1,1), XORB_MASK, PPCCOM,		{ RT, RA } },

{ "mullw",   XO(31,235,0,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "mullw.",  XO(31,235,0,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "mullwo",  XO(31,235,1,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "mullwo.", XO(31,235,1,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },

{ "mtsrin",  X(31,242),	XRA_MASK,	PPC32,		{ RS, RB } },
{ "mtsri",   X(31,242),	XRA_MASK,	POWER32,	{ RS, RB } },

{ "dcbtst",  X(31,246),	XRT_MASK,	PPC,		{ CT, RA, RB } },

{ "stbux",   X(31,247),	X_MASK,		COM,		{ RS, RAS, RB } },

{ "add",     XO(31,266,0,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "add.",    XO(31,266,0,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "addo",    XO(31,266,1,0), XO_MASK,	PPCCOM,		{ RT, RA, RB } },
{ "addo.",   XO(31,266,1,1), XO_MASK,	PPCCOM,		{ RT, RA, RB } },

{ "dcbt",    X(31,278),	XRT_MASK,	PPC,		{ CT, RA, RB } },

{ "lhzx",    X(31,279),	X_MASK,		COM,		{ RT, RA, RB } },

{ "eqv",     XRC(31,284,0), X_MASK,	COM,		{ RA, RS, RB } },
{ "eqv.",    XRC(31,284,1), X_MASK,	COM,		{ RA, RS, RB } },

{ "tlbie",   X(31,306),	XRTLRA_MASK,	PPC,		{ RB, L } },

{ "eciwx",   X(31,310), X_MASK,		PPC,		{ RT, RA, RB } },

{ "lhzux",   X(31,311),	X_MASK,		COM,		{ RT, RAL, RB } },

{ "xor",     XRC(31,316,0), X_MASK,	COM,		{ RA, RS, RB } },
{ "xor.",    XRC(31,316,1), X_MASK,	COM,		{ RA, RS, RB } },

{ "mfxer",      XSPR(31,339,1),    XSPR_MASK, COM,	{ RT } },
{ "mfrtcu",     XSPR(31,339,4),    XSPR_MASK, COM,	{ RT } },
{ "mfrtcl",     XSPR(31,339,5),    XSPR_MASK, COM,	{ RT } },
{ "mfdec",      XSPR(31,339,6),    XSPR_MASK, MFDEC1,	{ RT } },
{ "mflr",       XSPR(31,339,8),    XSPR_MASK, COM,	{ RT } },
{ "mfctr",      XSPR(31,339,9),    XSPR_MASK, COM,	{ RT } },
{ "mfdsisr",    XSPR(31,339,18),   XSPR_MASK, COM,	{ RT } },
{ "mfdar",      XSPR(31,339,19),   XSPR_MASK, COM,	{ RT } },
{ "mfdec",      XSPR(31,339,22),   XSPR_MASK, MFDEC2,	{ RT } },
{ "mfsdr1",     XSPR(31,339,25),   XSPR_MASK, COM,	{ RT } },
{ "mfsrr0",     XSPR(31,339,26),   XSPR_MASK, COM,	{ RT } },
{ "mfsrr1",     XSPR(31,339,27),   XSPR_MASK, COM,	{ RT } },
{ "mfvrsave",   XSPR(31,339,256),  XSPR_MASK, PPCVEC,	{ RT } },
{ "mfsprg",     XSPR(31,339,272),  XSPRG_MASK, PPC,	{ RT, SPRG } },
{ "mfsprg0",    XSPR(31,339,272),  XSPR_MASK, PPC,	{ RT } },
{ "mfsprg1",    XSPR(31,339,273),  XSPR_MASK, PPC,	{ RT } },
{ "mfsprg2",    XSPR(31,339,274),  XSPR_MASK, PPC,	{ RT } },
{ "mfsprg3",    XSPR(31,339,275),  XSPR_MASK, PPC,	{ RT } },
{ "mfasr",      XSPR(31,339,280),  XSPR_MASK, PPC64,	{ RT } },
{ "mfear",      XSPR(31,339,282),  XSPR_MASK, PPC,	{ RT } },
{ "mfpvr",      XSPR(31,339,287),  XSPR_MASK, PPC,	{ RT } },
{ "mfibatu",    XSPR(31,339,528),  XSPRBAT_MASK, PPC,	{ RT, SPRBAT } },
{ "mfibatl",    XSPR(31,339,529),  XSPRBAT_MASK, PPC,	{ RT, SPRBAT } },
{ "mfdbatu",    XSPR(31,339,536),  XSPRBAT_MASK, PPC,	{ RT, SPRBAT } },
{ "mfdbatl",    XSPR(31,339,537),  XSPRBAT_MASK, PPC,	{ RT, SPRBAT } },
{ "mfummcr0",   XSPR(31,339,936),  XSPR_MASK, PPC750,   { RT } },
{ "mfupmc1",    XSPR(31,339,937),  XSPR_MASK, PPC750,   { RT } },
{ "mfupmc2",    XSPR(31,339,938),  XSPR_MASK, PPC750,   { RT } },
{ "mfusia",     XSPR(31,339,939),  XSPR_MASK, PPC750,   { RT } },
{ "mfummcr1",   XSPR(31,339,940),  XSPR_MASK, PPC750,   { RT } },
{ "mfupmc3",    XSPR(31,339,941),  XSPR_MASK, PPC750,   { RT } },
{ "mfupmc4",    XSPR(31,339,942),  XSPR_MASK, PPC750,   { RT } },
{ "mfmmcr0",	XSPR(31,339,952),  XSPR_MASK, PPC750,	{ RT } },
{ "mfpmc1",	XSPR(31,339,953),  XSPR_MASK, PPC750,	{ RT } },
{ "mfpmc2",	XSPR(31,339,954),  XSPR_MASK, PPC750,	{ RT } },
{ "mfsia",	XSPR(31,339,955),  XSPR_MASK, PPC750,	{ RT } },
{ "mfmmcr1",	XSPR(31,339,956),  XSPR_MASK, PPC750,	{ RT } },
{ "mfpmc3",	XSPR(31,339,957),  XSPR_MASK, PPC750,	{ RT } },
{ "mfpmc4",	XSPR(31,339,958),  XSPR_MASK, PPC750,	{ RT } },
{ "mfl2cr",     XSPR(31,339,1017), XSPR_MASK, PPC750,   { RT } },
{ "mfictc",     XSPR(31,339,1019), XSPR_MASK, PPC750,   { RT } },
{ "mfthrm1",    XSPR(31,339,1020), XSPR_MASK, PPC750,   { RT } },
{ "mfthrm2",    XSPR(31,339,1021), XSPR_MASK, PPC750,   { RT } },
{ "mfthrm3",    XSPR(31,339,1022), XSPR_MASK, PPC750,   { RT } },
{ "mfspr",      X(31,339),	   X_MASK,    COM,	{ RT, SPR } },

{ "lwax",    X(31,341),	X_MASK,		PPC64,		{ RT, RA, RB } },

{ "dst",     XDSS(31,342,0), XDSS_MASK,	PPCVEC,		{ RA, RB, STRM } },
{ "dstt",    XDSS(31,342,1), XDSS_MASK,	PPCVEC,		{ RA, RB, STRM } },

{ "lhax",    X(31,343),	X_MASK,		COM,		{ RT, RA, RB } },

{ "dstst",   XDSS(31,374,0), XDSS_MASK,	PPCVEC,		{ RA, RB, STRM } },
{ "dststt",  XDSS(31,374,1), XDSS_MASK,	PPCVEC,		{ RA, RB, STRM } },

{ "tlbia",   X(31,370),	0xffffffff,	PPC,		{ 0 } },

{ "mftbl",   XSPR(31,371,268), XSPR_MASK, CLASSIC,	{ RT } },
{ "mftbu",   XSPR(31,371,269), XSPR_MASK, CLASSIC,	{ RT } },
{ "mftb",    X(31,371),	X_MASK,		CLASSIC,	{ RT, TBR } },

{ "lwaux",   X(31,373),	X_MASK,		PPC64,		{ RT, RAL, RB } },
{ "lhaux",   X(31,375),	X_MASK,		COM,		{ RT, RAL, RB } },

{ "slbmte",  X(31,402), XRA_MASK,	PPC64,		{ RS, RB } },

{ "sthx",    X(31,407),	X_MASK,		COM,		{ RS, RA, RB } },

{ "orc",     XRC(31,412,0), X_MASK,	COM,		{ RA, RS, RB } },
{ "orc.",    XRC(31,412,1), X_MASK,	COM,		{ RA, RS, RB } },

{ "sradi",   XS(31,413,0), XS_MASK,	PPC64,		{ RA, RS, SH6 } },
{ "sradi.",  XS(31,413,1), XS_MASK,	PPC64,		{ RA, RS, SH6 } },

{ "slbie",   X(31,434),	XRTRA_MASK,	PPC64,		{ RB } },

{ "ecowx",   X(31,438),	X_MASK,		PPC,		{ RT, RA, RB } },

{ "sthux",   X(31,439),	X_MASK,		COM,		{ RS, RAS, RB } },

{ "mr",	     XRC(31,444,0), X_MASK,	COM,		{ RA, RS, RBS } },
{ "or",      XRC(31,444,0), X_MASK,	COM,		{ RA, RS, RB } },
{ "mr.",     XRC(31,444,1), X_MASK,	COM,		{ RA, RS, RBS } },
{ "or.",     XRC(31,444,1), X_MASK,	COM,		{ RA, RS, RB } },

{ "divdu",   XO(31,457,0,0), XO_MASK,	PPC64,		{ RT, RA, RB } },
{ "divdu.",  XO(31,457,0,1), XO_MASK,	PPC64,		{ RT, RA, RB } },
{ "divduo",  XO(31,457,1,0), XO_MASK,	PPC64,		{ RT, RA, RB } },
{ "divduo.", XO(31,457,1,1), XO_MASK,	PPC64,		{ RT, RA, RB } },

{ "divwu",   XO(31,459,0,0), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "divwu.",  XO(31,459,0,1), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "divwuo",  XO(31,459,1,0), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "divwuo.", XO(31,459,1,1), XO_MASK,	PPC,		{ RT, RA, RB } },

{ "mtxer",     XSPR(31,467,1),    XSPR_MASK, COM,	{ RS } },
{ "mtlr",      XSPR(31,467,8),    XSPR_MASK, COM,	{ RS } },
{ "mtctr",     XSPR(31,467,9),    XSPR_MASK, COM,	{ RS } },
{ "mtdsisr",   XSPR(31,467,18),   XSPR_MASK, COM,	{ RS } },
{ "mtdar",     XSPR(31,467,19),   XSPR_MASK, COM,	{ RS } },
{ "mtrtcu",    XSPR(31,467,20),   XSPR_MASK, COM,	{ RS } },
{ "mtrtcl",    XSPR(31,467,21),   XSPR_MASK, COM,	{ RS } },
{ "mtdec",     XSPR(31,467,22),   XSPR_MASK, COM,	{ RS } },
{ "mtsdr1",    XSPR(31,467,25),   XSPR_MASK, COM,	{ RS } },
{ "mtsrr0",    XSPR(31,467,26),   XSPR_MASK, COM,	{ RS } },
{ "mtsrr1",    XSPR(31,467,27),   XSPR_MASK, COM,	{ RS } },
{ "mtvrsave",  XSPR(31,467,256),  XSPR_MASK, PPCVEC,	{ RT } },
{ "mtsprg",    XSPR(31,467,272),  XSPRG_MASK,PPC,	{ SPRG, RS } },
{ "mtsprg0",   XSPR(31,467,272),  XSPR_MASK, PPC,	{ RT } },
{ "mtsprg1",   XSPR(31,467,273),  XSPR_MASK, PPC,	{ RT } },
{ "mtsprg2",   XSPR(31,467,274),  XSPR_MASK, PPC,	{ RT } },
{ "mtsprg3",   XSPR(31,467,275),  XSPR_MASK, PPC,	{ RT } },
{ "mtasr",     XSPR(31,467,280),  XSPR_MASK, PPC64,	{ RS } },
{ "mtear",     XSPR(31,467,282),  XSPR_MASK, PPC,	{ RS } },
{ "mttbl",     XSPR(31,467,284),  XSPR_MASK, PPC,	{ RS } },
{ "mttbu",     XSPR(31,467,285),  XSPR_MASK, PPC,	{ RS } },
{ "mtibatu",   XSPR(31,467,528),  XSPRBAT_MASK, PPC,	{ SPRBAT, RS } },
{ "mtibatl",   XSPR(31,467,529),  XSPRBAT_MASK, PPC,	{ SPRBAT, RS } },
{ "mtdbatu",   XSPR(31,467,536),  XSPRBAT_MASK, PPC,	{ SPRBAT, RS } },
{ "mtdbatl",   XSPR(31,467,537),  XSPRBAT_MASK, PPC,	{ SPRBAT, RS } },
{ "mtummcr0",  XSPR(31,467,936),  XSPR_MASK, PPC750,    { RT } },
{ "mtupmc1",   XSPR(31,467,937),  XSPR_MASK, PPC750,    { RT } },
{ "mtupmc2",   XSPR(31,467,938),  XSPR_MASK, PPC750,    { RT } },
{ "mtusia",    XSPR(31,467,939),  XSPR_MASK, PPC750,    { RT } },
{ "mtummcr1",  XSPR(31,467,940),  XSPR_MASK, PPC750,    { RT } },
{ "mtupmc3",   XSPR(31,467,941),  XSPR_MASK, PPC750,    { RT } },
{ "mtupmc4",   XSPR(31,467,942),  XSPR_MASK, PPC750,    { RT } },
{ "mtmmcr0",   XSPR(31,467,952),  XSPR_MASK, PPC750,    { RT } },
{ "mtpmc1",    XSPR(31,467,953),  XSPR_MASK, PPC750,    { RT } },
{ "mtpmc2",    XSPR(31,467,954),  XSPR_MASK, PPC750,    { RT } },
{ "mtsia",     XSPR(31,467,955),  XSPR_MASK, PPC750,    { RT } },
{ "mtmmcr1",   XSPR(31,467,956),  XSPR_MASK, PPC750,    { RT } },
{ "mtpmc3",    XSPR(31,467,957),  XSPR_MASK, PPC750,    { RT } },
{ "mtpmc4",    XSPR(31,467,958),  XSPR_MASK, PPC750,    { RT } },
{ "mtl2cr",    XSPR(31,467,1017), XSPR_MASK, PPC750,    { RT } },
{ "mtictc",    XSPR(31,467,1019), XSPR_MASK, PPC750,    { RT } },
{ "mtthrm1",   XSPR(31,467,1020), XSPR_MASK, PPC750,    { RT } },
{ "mtthrm2",   XSPR(31,467,1021), XSPR_MASK, PPC750,    { RT } },
{ "mtthrm3",   XSPR(31,467,1022), XSPR_MASK, PPC750,    { RT } },
{ "mtspr",     X(31,467),	  X_MASK,    COM,	{ SPR, RS } },

{ "dcbi",    X(31,470),	XRT_MASK,	PPC,		{ RA, RB } },

{ "nand",    XRC(31,476,0), X_MASK,	COM,		{ RA, RS, RB } },
{ "nand.",   XRC(31,476,1), X_MASK,	COM,		{ RA, RS, RB } },

{ "divd",    XO(31,489,0,0), XO_MASK,	PPC64,		{ RT, RA, RB } },
{ "divd.",   XO(31,489,0,1), XO_MASK,	PPC64,		{ RT, RA, RB } },
{ "divdo",   XO(31,489,1,0), XO_MASK,	PPC64,		{ RT, RA, RB } },
{ "divdo.",  XO(31,489,1,1), XO_MASK,	PPC64,		{ RT, RA, RB } },

{ "divw",    XO(31,491,0,0), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "divw.",   XO(31,491,0,1), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "divwo",   XO(31,491,1,0), XO_MASK,	PPC,		{ RT, RA, RB } },
{ "divwo.",  XO(31,491,1,1), XO_MASK,	PPC,		{ RT, RA, RB } },

{ "slbia",   X(31,498),	0xffffffff,	PPC64,		{ 0 } },

{ "cli",     X(31,502), XRB_MASK,	POWER,		{ RT, RA } },

{ "mcrxr",   X(31,512),	XRARB_MASK|(3<<21), COM,	{ BF } },

{ "lswx",    X(31,533),	X_MASK,		PPCCOM,		{ RT, RA, RB } },

{ "lwbrx",   X(31,534),	X_MASK,		PPCCOM,		{ RT, RA, RB } },

{ "lfsx",    X(31,535),	X_MASK,		COM,		{ FRT, RA, RB } },

{ "srw",     XRC(31,536,0), X_MASK,	PPCCOM,		{ RA, RS, RB } },
{ "srw.",    XRC(31,536,1), X_MASK,	PPCCOM,		{ RA, RS, RB } },

{ "srd",     XRC(31,539,0), X_MASK,	PPC64,		{ RA, RS, RB } },
{ "srd.",    XRC(31,539,1), X_MASK,	PPC64,		{ RA, RS, RB } },

{ "tlbsync", X(31,566),	0xffffffff,	PPC,		{ 0 } },

{ "lfsux",   X(31,567),	X_MASK,		COM,		{ FRT, RAS, RB } },

{ "mfsr",    X(31,595),	XRB_MASK|(1<<20), COM32,	{ RT, SR } },

{ "lswi",    X(31,597),	X_MASK,		PPCCOM,		{ RT, RA, NB } },

{ "lwsync",  XSYNC(31,598,1), 0xffffffff, PPCONLY,	{ 0 } },
{ "ptesync", XSYNC(31,598,2), 0xffffffff, PPC64,	{ 0 } },
{ "sync",    X(31,598), XSYNC_MASK,	PPCCOM,		{ LS } },

{ "lfdx",    X(31,599), X_MASK,		COM,		{ FRT, RA, RB } },

{ "lfdux",   X(31,631), X_MASK,		COM,		{ FRT, RAS, RB } },

{ "mfsrin",  X(31,659), XRA_MASK,	PPC32,		{ RT, RB } },

{ "stswx",   X(31,661), X_MASK,		PPCCOM,		{ RS, RA, RB } },

{ "stwbrx",  X(31,662), X_MASK,		PPCCOM,		{ RS, RA, RB } },

{ "stfsx",   X(31,663), X_MASK,		COM,		{ FRS, RA, RB } },

{ "stfsux",  X(31,695),	X_MASK,		COM,		{ FRS, RAS, RB } },

{ "stswi",   X(31,725),	X_MASK,		PPCCOM,		{ RS, RA, NB } },

{ "stfdx",   X(31,727),	X_MASK,		COM,		{ FRS, RA, RB } },

{ "stfdux",  X(31,759),	X_MASK,		COM,		{ FRS, RAS, RB } },

{ "lhbrx",   X(31,790),	X_MASK,		COM,		{ RT, RA, RB } },

{ "sraw",    XRC(31,792,0), X_MASK,	PPCCOM,		{ RA, RS, RB } },
{ "sraw.",   XRC(31,792,1), X_MASK,	PPCCOM,		{ RA, RS, RB } },

{ "srad",    XRC(31,794,0), X_MASK,	PPC64,		{ RA, RS, RB } },
{ "srad.",   XRC(31,794,1), X_MASK,	PPC64,		{ RA, RS, RB } },

{ "dss",     XDSS(31,822,0), XDSS_MASK,	PPCVEC,		{ STRM } },
{ "dssall",  XDSS(31,822,1), XDSS_MASK,	PPCVEC,		{ 0 } },

{ "srawi",   XRC(31,824,0), X_MASK,	PPCCOM,		{ RA, RS, SH } },
{ "srawi.",  XRC(31,824,1), X_MASK,	PPCCOM,		{ RA, RS, SH } },

{ "slbmfev", X(31,851), XRA_MASK,	PPC64,		{ RT, RB } },

{ "eieio",   X(31,854),	0xffffffff,	PPC,		{ 0 } },

{ "slbmfee", X(31,915), XRA_MASK,	PPC64,		{ RT, RB } },

{ "sthbrx",  X(31,918),	X_MASK,		COM,		{ RS, RA, RB } },

{ "extsh",   XRC(31,922,0), XRB_MASK,	PPCCOM,		{ RA, RS } },
{ "extsh.",  XRC(31,922,1), XRB_MASK,	PPCCOM,		{ RA, RS } },

{ "extsb",   XRC(31,954,0), XRB_MASK,	PPC,		{ RA, RS} },
{ "extsb.",  XRC(31,954,1), XRB_MASK,	PPC,		{ RA, RS} },

{ "tlbld",   X(31,978),	XRTRA_MASK,	PPC,		{ RB } },

{ "icbi",    X(31,982),	XRT_MASK,	PPC,		{ RA, RB } },

{ "stfiwx",  X(31,983),	X_MASK,		PPC,		{ FRS, RA, RB } },

{ "extsw",   XRC(31,986,0), XRB_MASK,	PPC64 | BOOKE64,{ RA, RS } },
{ "extsw.",  XRC(31,986,1), XRB_MASK,	PPC64,		{ RA, RS } },

{ "tlbli",   X(31,1010), XRTRA_MASK,	PPC,		{ RB } },

{ "dcbz",    X(31,1014), XRT_MASK,	PPC,		{ RA, RB } },
{ "dcbz128", X(31,1014)|(1<<21), XRT_MASK,	PPC,		{ RA, RB } },

{ "lvebx",   X(31,   7), X_MASK,	PPCVEC,		{ VD, RA, RB } },
{ "lvehx",   X(31,  39), X_MASK,	PPCVEC,		{ VD, RA, RB } },
{ "lvewx",   X(31,  71), X_MASK,	PPCVEC,		{ VD, RA, RB } },
{ "lvsl",    X(31,   6), X_MASK,	PPCVEC,		{ VD, RA, RB } },
{ "lvsr",    X(31,  38), X_MASK,	PPCVEC,		{ VD, RA, RB } },
{ "lvx",     X(31, 103), X_MASK,	PPCVEC,		{ VD, RA, RB } },
{ "lvxl",    X(31, 359), X_MASK,	PPCVEC,		{ VD, RA, RB } },
{ "lvlx",    X(31, 519), X_MASK,	PPCVEC,		{ VD, RA, RB } },
{ "lvrx",    X(31, 551), X_MASK,	PPCVEC,		{ VD, RA, RB } },
{ "stvebx",  X(31, 135), X_MASK,	PPCVEC,		{ VS, RA, RB } },
{ "stvehx",  X(31, 167), X_MASK,	PPCVEC,		{ VS, RA, RB } },
{ "stvewx",  X(31, 199), X_MASK,	PPCVEC,		{ VS, RA, RB } },
{ "stvx",    X(31, 231), X_MASK,	PPCVEC,		{ VS, RA, RB } },
{ "stvxl",   X(31, 487), X_MASK,	PPCVEC,		{ VS, RA, RB } },

{ "lwz",     OP(32),	OP_MASK,	PPCCOM,		{ RT, D, RA } },

{ "lwzu",    OP(33),	OP_MASK,	PPCCOM,		{ RT, D, RAL } },

{ "lbz",     OP(34),	OP_MASK,	COM,		{ RT, D, RA } },

{ "lbzu",    OP(35),	OP_MASK,	COM,		{ RT, D, RAL } },

{ "stw",     OP(36),	OP_MASK,	PPCCOM,		{ RS, D, RA } },

{ "stwu",    OP(37),	OP_MASK,	PPCCOM,		{ RS, D, RAS } },

{ "stb",     OP(38),	OP_MASK,	COM,		{ RS, D, RA } },

{ "stbu",    OP(39),	OP_MASK,	COM,		{ RS, D, RAS } },

{ "lhz",     OP(40),	OP_MASK,	COM,		{ RT, D, RA } },

{ "lhzu",    OP(41),	OP_MASK,	COM,		{ RT, D, RAL } },

{ "lha",     OP(42),	OP_MASK,	COM,		{ RT, D, RA } },

{ "lhau",    OP(43),	OP_MASK,	COM,		{ RT, D, RAL } },

{ "sth",     OP(44),	OP_MASK,	COM,		{ RS, D, RA } },

{ "sthu",    OP(45),	OP_MASK,	COM,		{ RS, D, RAS } },

{ "lmw",     OP(46),	OP_MASK,	PPCCOM,		{ RT, D, RAM } },

{ "stmw",    OP(47),	OP_MASK,	PPCCOM,		{ RS, D, RA } },

{ "lfs",     OP(48),	OP_MASK,	COM,		{ FRT, D, RA } },

{ "lfsu",    OP(49),	OP_MASK,	COM,		{ FRT, D, RAS } },

{ "lfd",     OP(50),	OP_MASK,	COM,		{ FRT, D, RA } },

{ "lfdu",    OP(51),	OP_MASK,	COM,		{ FRT, D, RAS } },

{ "stfs",    OP(52),	OP_MASK,	COM,		{ FRS, D, RA } },

{ "stfsu",   OP(53),	OP_MASK,	COM,		{ FRS, D, RAS } },

{ "stfd",    OP(54),	OP_MASK,	COM,		{ FRS, D, RA } },

{ "stfdu",   OP(55),	OP_MASK,	COM,		{ FRS, D, RAS } },

{ "ld",      DSO(58,0),	DS_MASK,	PPC64,		{ RT, DS, RA0 } },

{ "ldu",     DSO(58,1), DS_MASK,	PPC64,		{ RT, DS, RAL } },

{ "lwa",     DSO(58,2), DS_MASK,	PPC64,		{ RT, DS, RA0 } },

{ "fdivs",   A(59,18,0), AFRC_MASK,	PPC,		{ FRT, FRA, FRB } },
{ "fdivs.",  A(59,18,1), AFRC_MASK,	PPC,		{ FRT, FRA, FRB } },

{ "fsubs",   A(59,20,0), AFRC_MASK,	PPC,		{ FRT, FRA, FRB } },
{ "fsubs.",  A(59,20,1), AFRC_MASK,	PPC,		{ FRT, FRA, FRB } },

{ "fadds",   A(59,21,0), AFRC_MASK,	PPC,		{ FRT, FRA, FRB } },
{ "fadds.",  A(59,21,1), AFRC_MASK,	PPC,		{ FRT, FRA, FRB } },

{ "fsqrts",  A(59,22,0), AFRAFRC_MASK,	PPC,		{ FRT, FRB } },
{ "fsqrts.", A(59,22,1), AFRAFRC_MASK,	PPC,		{ FRT, FRB } },

{ "fres",    A(59,24,0), AFRAFRC_MASK,	PPC,		{ FRT, FRB } },
{ "fres.",   A(59,24,1), AFRAFRC_MASK,	PPC,		{ FRT, FRB } },

{ "fmuls",   A(59,25,0), AFRB_MASK,	PPC,		{ FRT, FRA, FRC } },
{ "fmuls.",  A(59,25,1), AFRB_MASK,	PPC,		{ FRT, FRA, FRC } },

{ "fmsubs",  A(59,28,0), A_MASK,	PPC,		{ FRT,FRA,FRC,FRB } },
{ "fmsubs.", A(59,28,1), A_MASK,	PPC,		{ FRT,FRA,FRC,FRB } },

{ "fmadds",  A(59,29,0), A_MASK,	PPC,		{ FRT,FRA,FRC,FRB } },
{ "fmadds.", A(59,29,1), A_MASK,	PPC,		{ FRT,FRA,FRC,FRB } },

{ "fnmsubs", A(59,30,0), A_MASK,	PPC,		{ FRT,FRA,FRC,FRB } },
{ "fnmsubs.",A(59,30,1), A_MASK,	PPC,		{ FRT,FRA,FRC,FRB } },

{ "fnmadds", A(59,31,0), A_MASK,	PPC,		{ FRT,FRA,FRC,FRB } },
{ "fnmadds.",A(59,31,1), A_MASK,	PPC,		{ FRT,FRA,FRC,FRB } },

{ "std",     DSO(62,0),	DS_MASK,	PPC64,		{ RS, DS, RA0 } },

{ "stdu",    DSO(62,1),	DS_MASK,	PPC64,		{ RS, DS, RAS } },

{ "fcmpu",   X(63,0),	X_MASK|(3<<21),	COM,		{ BF, FRA, FRB } },

{ "frsp",    XRC(63,12,0), XRA_MASK,	COM,		{ FRT, FRB } },
{ "frsp.",   XRC(63,12,1), XRA_MASK,	COM,		{ FRT, FRB } },

{ "fctiw",   XRC(63,14,0), XRA_MASK,	PPCCOM,		{ FRT, FRB } },
{ "fctiw.",  XRC(63,14,1), XRA_MASK,	PPCCOM,		{ FRT, FRB } },

{ "fctiwz",  XRC(63,15,0), XRA_MASK,	PPCCOM,		{ FRT, FRB } },
{ "fctiwz.", XRC(63,15,1), XRA_MASK,	PPCCOM,		{ FRT, FRB } },

{ "fdiv",    A(63,18,0), AFRC_MASK,	PPCCOM,		{ FRT, FRA, FRB } },
{ "fdiv.",   A(63,18,1), AFRC_MASK,	PPCCOM,		{ FRT, FRA, FRB } },

{ "fsub",    A(63,20,0), AFRC_MASK,	PPCCOM,		{ FRT, FRA, FRB } },
{ "fsub.",   A(63,20,1), AFRC_MASK,	PPCCOM,		{ FRT, FRA, FRB } },

{ "fadd",    A(63,21,0), AFRC_MASK,	PPCCOM,		{ FRT, FRA, FRB } },
{ "fadd.",   A(63,21,1), AFRC_MASK,	PPCCOM,		{ FRT, FRA, FRB } },

{ "fsqrt",   A(63,22,0), AFRAFRC_MASK,	PPCPWR2,	{ FRT, FRB } },
{ "fsqrt.",  A(63,22,1), AFRAFRC_MASK,	PPCPWR2,	{ FRT, FRB } },

{ "fsel",    A(63,23,0), A_MASK,	PPC,		{ FRT,FRA,FRC,FRB } },
{ "fsel.",   A(63,23,1), A_MASK,	PPC,		{ FRT,FRA,FRC,FRB } },

{ "fmul",    A(63,25,0), AFRB_MASK,	PPCCOM,		{ FRT, FRA, FRC } },
{ "fmul.",   A(63,25,1), AFRB_MASK,	PPCCOM,		{ FRT, FRA, FRC } },

{ "frsqrte", A(63,26,0), AFRAFRC_MASK,	PPC,		{ FRT, FRB } },
{ "frsqrte.",A(63,26,1), AFRAFRC_MASK,	PPC,		{ FRT, FRB } },

{ "fmsub",   A(63,28,0), A_MASK,	PPCCOM,		{ FRT,FRA,FRC,FRB } },
{ "fmsub.",  A(63,28,1), A_MASK,	PPCCOM,		{ FRT,FRA,FRC,FRB } },

{ "fmadd",   A(63,29,0), A_MASK,	PPCCOM,		{ FRT,FRA,FRC,FRB } },
{ "fmadd.",  A(63,29,1), A_MASK,	PPCCOM,		{ FRT,FRA,FRC,FRB } },

{ "fnmsub",  A(63,30,0), A_MASK,	PPCCOM,		{ FRT,FRA,FRC,FRB } },
{ "fnmsub.", A(63,30,1), A_MASK,	PPCCOM,		{ FRT,FRA,FRC,FRB } },

{ "fnmadd",  A(63,31,0), A_MASK,	PPCCOM,		{ FRT,FRA,FRC,FRB } },
{ "fnmadd.", A(63,31,1), A_MASK,	PPCCOM,		{ FRT,FRA,FRC,FRB } },

{ "fcmpo",   X(63,32),	X_MASK|(3<<21),	COM,		{ BF, FRA, FRB } },

{ "mtfsb1",  XRC(63,38,0), XRARB_MASK,	COM,		{ BT } },
{ "mtfsb1.", XRC(63,38,1), XRARB_MASK,	COM,		{ BT } },

{ "fneg",    XRC(63,40,0), XRA_MASK,	COM,		{ FRT, FRB } },
{ "fneg.",   XRC(63,40,1), XRA_MASK,	COM,		{ FRT, FRB } },

{ "mcrfs",   X(63,64),	XRB_MASK|(3<<21)|(3<<16), COM,	{ BF, BFA } },

{ "mtfsb0",  XRC(63,70,0), XRARB_MASK,	COM,		{ BT } },
{ "mtfsb0.", XRC(63,70,1), XRARB_MASK,	COM,		{ BT } },

{ "fmr",     XRC(63,72,0), XRA_MASK,	COM,		{ FRT, FRB } },
{ "fmr.",    XRC(63,72,1), XRA_MASK,	COM,		{ FRT, FRB } },

{ "mtfsfi",  XRC(63,134,0), XRA_MASK|(3<<21)|(1<<11), COM, { BF, U } },
{ "mtfsfi.", XRC(63,134,1), XRA_MASK|(3<<21)|(1<<11), COM, { BF, U } },

{ "fnabs",   XRC(63,136,0), XRA_MASK,	COM,		{ FRT, FRB } },
{ "fnabs.",  XRC(63,136,1), XRA_MASK,	COM,		{ FRT, FRB } },

{ "fabs",    XRC(63,264,0), XRA_MASK,	COM,		{ FRT, FRB } },
{ "fabs.",   XRC(63,264,1), XRA_MASK,	COM,		{ FRT, FRB } },

{ "mffs",    XRC(63,583,0), XRARB_MASK,	COM,		{ FRT } },
{ "mffs.",   XRC(63,583,1), XRARB_MASK,	COM,		{ FRT } },

{ "mtfsf",   XFL(63,711,0), XFL_MASK,	COM,		{ FLM, FRB } },
{ "mtfsf.",  XFL(63,711,1), XFL_MASK,	COM,		{ FLM, FRB } },

{ "fctid",   XRC(63,814,0), XRA_MASK,	PPC64,		{ FRT, FRB } },
{ "fctid.",  XRC(63,814,1), XRA_MASK,	PPC64,		{ FRT, FRB } },

{ "fctidz",  XRC(63,815,0), XRA_MASK,	PPC64,		{ FRT, FRB } },
{ "fctidz.", XRC(63,815,1), XRA_MASK,	PPC64,		{ FRT, FRB } },

{ "fcfid",   XRC(63,846,0), XRA_MASK,	PPC64,		{ FRT, FRB } },
{ "fcfid.",  XRC(63,846,1), XRA_MASK,	PPC64,		{ FRT, FRB } },

};

const int powerpc_num_opcodes =
  sizeof (powerpc_opcodes) / sizeof (powerpc_opcodes[0]);

