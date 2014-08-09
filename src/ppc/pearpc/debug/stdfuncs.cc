/*
 *	HT Editor
 *	stdfuncs.cc
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

#include "tools/except.h"
#include "debug/stdfuncs.h"

#define declareIntOperator1(NAME, CODE...) \
class NAME: public PFunction {\
public:\
virtual	EvalType	getReturnType() const {	return ET_INTEGER;}\
virtual	SInt64 *	evalInteger() const\
{\
	sint64 arg0 = ((Function *)mArgs[0])->evalInteger()->value;\
	CODE\
}\
};\
static Function *create##NAME() { return new NAME(); }

/**/
#define declareIntOperator2(NAME, CODE...) \
class NAME: public PFunction {\
public:\
virtual	EvalType	getReturnType() const {	return ET_INTEGER;}\
virtual	SInt64 *	evalInteger() const\
{\
	sint64 arg0 = ((Function *)mArgs[0])->evalInteger()->value;\
	sint64 arg1 = ((Function *)mArgs[1])->evalInteger()->value;\
	CODE\
}\
};\
static Function *create##NAME() { return new NAME(); }

/**/
#define declareFloatOperator2(NAME, CODE...) \
class NAME: public PFunction {\
public:\
virtual	EvalType	getReturnType() const {	return ET_FLOAT;}\
virtual	Float *		evalFloat() const\
{\
	double arg0 = ((Function *)mArgs[0])->evalFloat()->value;\
	double arg1 = ((Function *)mArgs[1])->evalFloat()->value;\
	CODE\
}\
};\
static Function *create##NAME() { return new NAME(); }

/**/
#define declareFloatOperator1(NAME, CODE...) \
class NAME: public PFunction {\
public:\
virtual	EvalType	getReturnType() const {	return ET_FLOAT;}\
virtual	Float *		evalFloat() const\
{\
	double arg0 = ((Function *)mArgs[0])->evalFloat()->value;\
	CODE\
}\
};\
static Function *create##NAME() { return new NAME(); }

/**/
#define declareIntFuncFloatFloat(NAME, CODE...) \
class NAME: public PFunction {\
public:\
virtual	EvalType	getReturnType() const {	return ET_INTEGER;}\
virtual	SInt64 *	evalInteger() const\
{\
	double arg0 = ((Function *)mArgs[0])->evalFloat()->value;\
	double arg1 = ((Function *)mArgs[1])->evalFloat()->value;\
	CODE\
}\
};\
static Function *create##NAME() { return new NAME(); }

/**/
#define declareStrOperator2(NAME, CODE...) \
class NAME: public PFunction {\
public:\
virtual	EvalType	getReturnType() const {	return ET_STRING;}\
virtual	String *	evalString() const\
{\
	String *arg0 = ((Function *)mArgs[0])->evalString();\
	String *arg1 = ((Function *)mArgs[1])->evalString();\
	CODE\
}\
};\
static Function *create##NAME() { return new NAME(); }
/**/

#define declareIntFuncStrStr(NAME, CODE...) \
class NAME: public PFunction {\
public:\
virtual	EvalType	getReturnType() const {	return ET_INTEGER;}\
virtual	SInt64 *	evalInteger() const\
{\
	String *arg0 = ((Function *)mArgs[0])->evalString();\
	String *arg1 = ((Function *)mArgs[1])->evalString();\
	CODE\
}\
};\
static Function *create##NAME() { return new NAME(); }
/**/

/* integer functions */
declareIntOperator2(MulFuncInt, return new SInt64(arg0 * arg1); );
declareIntOperator2(DivFuncInt,
	if (!arg1) throw MsgException("division by zero");
	return new SInt64(arg0 / arg1);
);
declareIntOperator2(RemFuncInt,
	if (!arg1) throw MsgException("division by zero");
	return new SInt64(arg0 % arg1);
);
declareIntOperator2(AddFuncInt, return new SInt64(arg0 + arg1); );
declareIntOperator2(SubFuncInt, return new SInt64(arg0 - arg1); );
declareIntOperator2(AndFuncInt, return new SInt64(arg0 & arg1); );
declareIntOperator2(OrFuncInt, return new SInt64(arg0 | arg1); );
declareIntOperator2(XorFuncInt, return new SInt64(arg0 ^ arg1); );
declareIntOperator2(ShlFuncInt, return new SInt64((uint64)arg0 << arg1); );
declareIntOperator2(ShrFuncInt, return new SInt64((uint64)arg0 >> arg1); );
declareIntOperator2(PowFuncInt,
	sint64 b = arg0;
	sint64 e = arg1;
	sint64 r = 1;
	int ln2_e = -1;
	uint64 uarg1 = arg1;
	while (uarg1) {
		uarg1 >>= 1;
		ln2_e ++;
	}
	uint64 mask = 1L << ln2_e;
	while (1) {
		if (e & mask) r *= b;
		mask >>= 1;
		if (!mask) break;
		r = r*r;
	}
	return new SInt64(r);
);
declareIntOperator2(MaxFuncInt, return new SInt64((arg1>arg0) ? arg1 : arg0); );
declareIntOperator2(MinFuncInt, return new SInt64((arg1<arg0) ? arg1 : arg0); );

declareIntOperator2(LAndFuncInt, return new SInt64(arg0 && arg1); );
declareIntOperator2(LOrFuncInt, return new SInt64(arg0 || arg1); );
declareIntOperator2(LXorFuncInt, return new SInt64((arg0 && !arg1) || (!arg0 && arg1)); );

declareIntOperator2(LTFuncInt, return new SInt64(arg0 < arg1); );
declareIntOperator2(GTFuncInt, return new SInt64(arg0 > arg1); );
declareIntOperator2(LEFuncInt, return new SInt64(arg0 <= arg1); );
declareIntOperator2(GEFuncInt, return new SInt64(arg0 >= arg1); );
declareIntOperator2(EQFuncInt, return new SInt64(arg0 == arg1); );
declareIntOperator2(NEFuncInt, return new SInt64(arg0 != arg1); );
declareIntOperator2(GcdFuncInt,
	while (arg1) {
		sint64 temp = arg1;
		arg1 = arg0 % arg1;
		arg0 = temp;
	}
	return new SInt64((arg0<0) ? -arg0 : arg0);
);

declareIntOperator2(BinCoeffFuncInt,
	// calculate (n k) = n! / (n-k)! / k!
	uint64 n = arg0;
	uint64 k = arg1;
	uint64 r = 1;
	for (uint64 i = n; i>n-k; i--) {
		r *= i;
	}
	for (uint64 i = 2; i<=k; i++) {
		r /= i;
	}
	return new SInt64(r);
);

declareIntOperator1(FacFuncInt,
	if (arg0 < 0) throw MsgException("invalid argument (must be >= 0)");
	// 0! = 1!
	if (arg0 == 0) arg0 = 1;
	uint64 r = arg0;
	while (arg0-- > 2) {
		r *= arg0;
	}
	return new SInt64(r);
);
declareIntOperator1(NegateFuncInt, return new SInt64(-arg0); );

/* float functions */
declareFloatOperator2(MulFuncFloat, return new Float(arg0 * arg1); );
declareFloatOperator2(DivFuncFloat,
	if (!arg1) throw MsgException("division by zero");
	return new Float(arg0 / arg1);
);
declareFloatOperator2(AddFuncFloat, return new Float(arg0 + arg1); );
declareFloatOperator2(SubFuncFloat, return new Float(arg0 - arg1); );
declareFloatOperator2(PowFuncFloat, return new Float(pow(arg0, arg1)); );

declareFloatOperator2(MaxFuncFloat, return new Float((arg1>arg0) ? arg1 : arg0); );
declareFloatOperator2(MinFuncFloat, return new Float((arg1<arg0) ? arg1 : arg0); );

declareFloatOperator1(SqrtFuncFloat, return new Float(sqrt(arg0)); );
declareFloatOperator1(ExpFuncFloat, return new Float(exp(arg0)); );
declareFloatOperator1(LogFuncFloat, return new Float(log(arg0)); );
declareFloatOperator1(CeilFuncFloat, return new Float(ceil(arg0)); );
declareFloatOperator1(FloorFuncFloat, return new Float(floor(arg0)); );
declareFloatOperator1(SinFuncFloat, return new Float(sin(arg0)); );
declareFloatOperator1(CosFuncFloat, return new Float(cos(arg0)); );
declareFloatOperator1(TanFuncFloat, return new Float(tan(arg0)); );
declareFloatOperator1(ASinFuncFloat, return new Float(asin(arg0)); );
declareFloatOperator1(ACosFuncFloat, return new Float(acos(arg0)); );
declareFloatOperator1(ATanFuncFloat, return new Float(atan(arg0)); );

declareFloatOperator1(NegateFuncFloat, return new Float(-arg0); );

declareIntFuncFloatFloat(LTFuncFloat, return new SInt64(arg0 < arg1); );
declareIntFuncFloatFloat(GTFuncFloat, return new SInt64(arg0 > arg1); );
declareIntFuncFloatFloat(LEFuncFloat, return new SInt64(arg0 <= arg1); );
declareIntFuncFloatFloat(GEFuncFloat, return new SInt64(arg0 >= arg1); );
declareIntFuncFloatFloat(EQFuncFloat, return new SInt64(arg0 == arg1); );
declareIntFuncFloatFloat(NEFuncFloat, return new SInt64(arg0 != arg1); );
/* string functions */
declareStrOperator2(ConcatFuncStr,
	String *r = new String(arg0);
	r->append(arg1);
	return r;
);

declareIntFuncStrStr(LTFuncStr, return new SInt64(arg0 < arg1); );
declareIntFuncStrStr(GTFuncStr, return new SInt64(arg0 > arg1); );
declareIntFuncStrStr(LEFuncStr, return new SInt64(arg0 <= arg1); );
declareIntFuncStrStr(GEFuncStr, return new SInt64(arg0 >= arg1); );
declareIntFuncStrStr(EQFuncStr, return new SInt64(arg0->compareTo(arg1) == 0); );
declareIntFuncStrStr(NEFuncStr, return new SInt64(arg0->compareTo(arg1) != 0); );

/* special functions */
class MiniIf: public PFunction {
public:
virtual	EvalType getReturnType() const
{
	if (mArgs.count() != 3) throw MsgException("exactly 3 parameters required");
	EvalType arg1t = ((Function *)mArgs[1])->getReturnType();
	EvalType arg2t = ((Function *)mArgs[2])->getReturnType();
	if ((arg1t == ET_STRING) || (arg2t == ET_STRING)) return ET_STRING;
	if ((arg1t == ET_FLOAT) || (arg2t == ET_FLOAT)) return ET_FLOAT;
	return ET_INTEGER;
}

virtual	SInt64 *evalInteger() const
{
	if (mArgs.count() != 3) throw MsgException("exactly 3 parameters required");
	sint64 arg0 = ((Function *)mArgs[0])->evalInteger()->value;
	return new SInt64(arg0 ?
		((Function *)mArgs[1])->evalInteger()->value :
		((Function *)mArgs[2])->evalInteger()->value);
}

virtual	Float *evalFloat() const
{
	if (mArgs.count() != 3) throw MsgException("exactly 3 parameters required");
	sint64 arg0 = ((Function *)mArgs[0])->evalInteger()->value;
	return new Float(arg0 ?
		((Function *)mArgs[1])->evalFloat()->value :
		((Function *)mArgs[2])->evalFloat()->value);
}

virtual	String *evalString() const
{
	if (mArgs.count() != 3) throw MsgException("exactly 3 parameters required");
	sint64 arg0 = ((Function *)mArgs[0])->evalInteger()->value;
	return new String(arg0 ?
		((Function *)mArgs[1])->evalString() :
		((Function *)mArgs[2])->evalString());
}
};
static Function *createMiniIf() { return new MiniIf(); }

/**/
class TypeOfInt: public PFunction {
public:
virtual	EvalType getReturnType() const { return ET_STRING; }
virtual	String *evalString() const { return new String("integer"); }
};
static Function *createTypeOfInt() { return new TypeOfInt(); }

/**/
class TypeOfFloat: public PFunction {
public:
virtual	EvalType getReturnType() const { return ET_STRING; }
virtual	String *evalString() const { return new String("float"); }
};
static Function *createTypeOfFloat() { return new TypeOfFloat(); }

/**/
class TypeOfStr: public PFunction {
public:
virtual	EvalType getReturnType() const { return ET_STRING; }
virtual	String *evalString() const { return new String("string"); }
};
static Function *createTypeOfStr() { return new TypeOfStr(); }

/**/
FunctionDesc gStdEvalFunctions[] =
{
	{ "*",  2, {ET_INTEGER, ET_INTEGER}, &createMulFuncInt },
	{ "/",  2, {ET_INTEGER, ET_INTEGER}, &createDivFuncInt },
	{ "%",  2, {ET_INTEGER, ET_INTEGER}, &createRemFuncInt },
	{ "+",  2, {ET_INTEGER, ET_INTEGER}, &createAddFuncInt },
	{ "-",  2, {ET_INTEGER, ET_INTEGER}, &createSubFuncInt },
	{ "&",  2, {ET_INTEGER, ET_INTEGER}, &createAndFuncInt },
	{ "|",  2, {ET_INTEGER, ET_INTEGER}, &createOrFuncInt },
	{ "^",  2, {ET_INTEGER, ET_INTEGER}, &createXorFuncInt },
	{ "**", 2, {ET_INTEGER, ET_INTEGER}, &createPowFuncInt },
	{ "max",2, {ET_INTEGER, ET_INTEGER}, &createMaxFuncInt },
	{ "min",2, {ET_INTEGER, ET_INTEGER}, &createMinFuncInt },
	{ "<<", 2, {ET_INTEGER, ET_INTEGER}, &createShlFuncInt },
	{ ">>", 2, {ET_INTEGER, ET_INTEGER}, &createShrFuncInt },
	{ "&&", 2, {ET_INTEGER, ET_INTEGER}, &createLAndFuncInt },
	{ "||", 2, {ET_INTEGER, ET_INTEGER}, &createLOrFuncInt },
	{ "^^", 2, {ET_INTEGER, ET_INTEGER}, &createLXorFuncInt },

	{ "<",  2, {ET_INTEGER, ET_INTEGER}, &createLTFuncInt },
	{ ">",  2, {ET_INTEGER, ET_INTEGER}, &createGTFuncInt },
	{ "<=", 2, {ET_INTEGER, ET_INTEGER}, &createLEFuncInt },
	{ ">=", 2, {ET_INTEGER, ET_INTEGER}, &createGEFuncInt },
	{ "==", 2, {ET_INTEGER, ET_INTEGER}, &createEQFuncInt },
	{ "!=", 2, {ET_INTEGER, ET_INTEGER}, &createNEFuncInt },
	{ "gcd",2, {ET_INTEGER, ET_INTEGER}, &createGcdFuncInt },
	{ "bincoeff",2, {ET_INTEGER, ET_INTEGER}, &createBinCoeffFuncInt },
	{ "-",  1, {ET_INTEGER}, &createNegateFuncInt },
	{ "fac",1, {ET_INTEGER}, &createFacFuncInt },

	{ "*",  2, {ET_FLOAT, ET_FLOAT}, &createMulFuncFloat },
	{ "/",  2, {ET_FLOAT, ET_FLOAT}, &createDivFuncFloat },
	{ "+",  2, {ET_FLOAT, ET_FLOAT}, &createAddFuncFloat },
	{ "-",  2, {ET_FLOAT, ET_FLOAT}, &createSubFuncFloat },
	{ "**", 2, {ET_FLOAT, ET_FLOAT}, &createPowFuncFloat },
	{ "max",2, {ET_FLOAT, ET_FLOAT}, &createMaxFuncFloat },
	{ "min",2, {ET_FLOAT, ET_FLOAT}, &createMinFuncFloat },

	{ "<",  2, {ET_FLOAT, ET_FLOAT}, &createLTFuncFloat },
	{ ">",  2, {ET_FLOAT, ET_FLOAT}, &createGTFuncFloat },
	{ "<=", 2, {ET_FLOAT, ET_FLOAT}, &createLEFuncFloat },
	{ ">=", 2, {ET_FLOAT, ET_FLOAT}, &createGEFuncFloat },
	{ "==", 2, {ET_FLOAT, ET_FLOAT}, &createEQFuncFloat },
	{ "!=", 2, {ET_FLOAT, ET_FLOAT}, &createNEFuncFloat },
	{ "sqrt",  1, {ET_FLOAT}, &createSqrtFuncFloat },
	{ "exp",   1, {ET_FLOAT}, &createExpFuncFloat },
	{ "log",   1, {ET_FLOAT}, &createLogFuncFloat },

	// FIXME: should return int, not float (use float2int)
	{ "ceil",  1, {ET_FLOAT}, &createCeilFuncFloat },
	// FIXME: should return int, not float (use float2int)
	{ "floor", 1, {ET_FLOAT}, &createFloorFuncFloat },

	{ "sin",   1, {ET_FLOAT}, &createSinFuncFloat },
	{ "cos",   1, {ET_FLOAT}, &createCosFuncFloat },
	{ "tan",   1, {ET_FLOAT}, &createTanFuncFloat },
	{ "asin",  1, {ET_FLOAT}, &createASinFuncFloat },
	{ "acos",  1, {ET_FLOAT}, &createACosFuncFloat },
	{ "atan",  1, {ET_FLOAT}, &createATanFuncFloat },
	{ "-",     1, {ET_FLOAT}, &createNegateFuncFloat },

	{ ".",  2, {ET_STRING, ET_STRING}, &createConcatFuncStr },
	{ "<",  2, {ET_STRING, ET_STRING}, &createLTFuncStr },
	{ ">",  2, {ET_STRING, ET_STRING}, &createGTFuncStr },
	{ "<=", 2, {ET_STRING, ET_STRING}, &createLEFuncStr },
	{ ">=", 2, {ET_STRING, ET_STRING}, &createGEFuncStr },
	{ "==", 2, {ET_STRING, ET_STRING}, &createEQFuncStr },
	{ "!=", 2, {ET_STRING, ET_STRING}, &createNEFuncStr },

	{ "?:", 2, {ET_INTEGER, ET_VARARGS}, &createMiniIf },
	{ "typeof", 1, {ET_INTEGER}, &createTypeOfInt },
	{ "typeof", 1, {ET_FLOAT}, &createTypeOfFloat },
	{ "typeof", 1, {ET_STRING}, &createTypeOfStr },

	{ NULL }
};
