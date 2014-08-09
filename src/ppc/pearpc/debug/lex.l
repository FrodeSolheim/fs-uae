%{
#define USE_PURE_PARSER

#ifdef USE_PURE_PARSER
#define YY_DECL int yylex (YYSTYPE *lvalp)
#define YY_LVALP lvalp
#else
#define YY_LVALP (&yylval)
#endif

/* get current pos (offset from start of line) */
#define YY_USER_ACTION lex_pos=yy_bp - YY_CURRENT_BUFFER->yy_ch_buf;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "debug/debugtype.h"
#include "debugparse.h"
#include "parsehelper.h"

static int lex_pos;

static int char2int(char a, int base)
{
	int i;
	if ((a>='A') && (a<='Z')) {
		i=a-'A'+10;
	} else if ((a>='a') && (a<='z')) {
		i=a-'a'+10;
	} else if ((a>='0') && (a<='9')) {
		i=a-'0';
	} else return -1;
	if (i>=base) return -1;
	return i;
}

static int parse_integer(struct eval_scalar *i, char *num, int base, int lenmod)
{
	uint64 k = 0;
	int l = strlen(num)+lenmod;
	while (l--) {
		int c = char2int(*num, base);
		if (c==-1) return 0;
		k *= base;
		k += c;
		num++;
	}
	i->type = SCALAR_INT;
	i->scalar.integer.value = k;
	i->scalar.integer.type = TYPE_UNKNOWN;
	return 1;
}

static int parse_gpr(struct eval_scalar *i, char *s)
{
	int r = 0;
	if (tolower(*s) != 'r') return 0;
	s++;
	do {
		int c = char2int(*s, 10);
		if (c == -1) return 0;
		r *= 10;
		r += c;
		s++;
	} while (*s);
	if (r>31) return 0;
	i->type = SCALAR_REG;
	i->scalar.reg.type = REG_GPR;
	i->scalar.reg.num = r;
	return 1;
}

static int parse_fpr(struct eval_scalar *i, char *s)
{
	int r = 0;
	if (tolower(*s) != 'f') return 0;
	s++;
	if (tolower(*s) != 'r') return 0;
	s++;
	do {
		int c = char2int(*s, 10);
		if (c == -1) return 0;
		r *= 10;
		r += c;
		s++;
	} while (*s);
	if (r>31) return 0;
	i->type = SCALAR_REG;
	i->scalar.reg.type = REG_FPR;
	i->scalar.reg.num = r;
	return 1;
}

static int parse_float(struct eval_scalar *f, char *fpn)
{
	char *end;
	double d;
	d=strtod(fpn, &end);
	if (*end==0) {
		f->type=SCALAR_FLOAT;
		f->scalar.floatnum.value=d;
		return 1;
	}
	return 0;		
}

static int parse_cstring(struct eval_scalar *r, char *s, int len)
{
	char *result;

	int alloclen=len;
	if (!len) alloclen=1;

	r->type=SCALAR_STR;
	r->scalar.str.value=(char*)malloc(alloclen);
	/* translate */
	result=r->scalar.str.value;
	
	while (s && *s && len) {
		if (*s == '\\') {
			s++;len--;if (!len) break;
			switch (*s) {
				case '0':
					*result++='\0';
					break;
				case 'a':
					*result++='\a';
					break;
				case 'b':
					*result++='\b';
					break;
				case 'e':
					*result++='\e';
					break;
				case 'f':
					*result++='\f';
					break;
				case 'n':
					*result++='\n';
					break;
				case 'r':
					*result++='\r';
					break;
				case 't':
					*result++='\t';
					break;
				case 'v':
					*result++='\v';
					break;
				case '\"':
					*result++='"';
					break;
				case '\\':
					*result++='\\';
					break;
				case 'x': {
					int p, q;
					s++;len--;if (!len) break;
					p=char2int(*s, 16);
					if (p==-1)  return 0;
					s++;len--;if (!len) break;
					q=char2int(*s, 16);
					if (q==-1) return 0;
					*result++=(char)p*16+q;
					break;
				}
				default:
					*result++='\\';
					if (len) *result++=*s;
					break;
			}
		} else {
			*result++ = *s;
		}
		s++;len--;
	}
	
	r->scalar.str.len=result-r->scalar.str.value;
	
	return 1;
}

static int parse_pstring(struct eval_scalar *s, char *cstr, int len)
{
	int alloclen = len;
	if (!len) alloclen = 1;
	
	s->type = SCALAR_STR;
	s->scalar.str.value = (char*)malloc(alloclen);
	memmove(s->scalar.str.value, cstr, len);
	s->scalar.str.len = len;
	return 1;
}

void *lex_current_buffer()
{
	return (void*)YY_CURRENT_BUFFER;
}

int lex_current_buffer_pos()
{
	return lex_pos;
}

void lex_switch_buffer(void *buffer)
{
	yy_switch_to_buffer(buffer);	
}

void lex_delete_buffer(void *buffer)
{
	yy_delete_buffer(buffer);	
}

void *lex_scan_string_buffer(const char *str)
{
	return yy_scan_string(str);
}

%}

%option noyywrap

%%
[ \t]+          		/* nop */
\"(\\\"|[^"])*\"		if (parse_cstring(&YY_LVALP->scalar, yytext+1, strlen(yytext+1)-1)) return EVAL_STR;
'[^']*'				if (parse_pstring(&YY_LVALP->scalar, yytext+1, strlen(yytext+1)-1)) return EVAL_STR;
\*\*				return EVAL_POW;
\<\<				return EVAL_SHL;
\>\>				return EVAL_SHR;
\<				return EVAL_LT;
\<\=				return EVAL_LE;
\>				return EVAL_GT;
\>\=				return EVAL_GE;
\=\=				return EVAL_EQ;
\!\=				return EVAL_NE;
\&\&				return EVAL_LAND;
\|\|				return EVAL_LOR;
\^\^				return EVAL_LXOR;
print				|
p				|
eval				return EVAL_PRINT;
regs				return EVAL_REGS;
f                               |
fprs                            |
floats                          return EVAL_FLOATS;
setreg				return EVAL_SETREG;
break				|
b				return EVAL_BREAK;
list_break			|
lb				return EVAL_LIST_BREAK;
step				return EVAL_STEP;
s				return EVAL_STEP;
next				return EVAL_NEXT;
n				return EVAL_NEXT;
continue			|
cont				|
c				return EVAL_CONTINUE;
quit                            |
bye				return EVAL_QUIT;
virt_to_phys			|
ea_to_pa			|
e2p				|
v2p				return EVAL_E2P;
inspect_byte			|
ib				return EVAL_INSPECT_BYTE;
inspect_half			|
ih				return EVAL_INSPECT_HALF;
inspect_word			|
iw				return EVAL_INSPECT_WORD;
inspect_dword			|
id				return EVAL_INSPECT_DWORD;
inspect_string			|
is				return EVAL_INSPECT_STRING;
inspect_memory			|
im				return EVAL_INSPECT_MEM;
watch                           |
w				return EVAL_WATCH;
watch_byte			|
wb				return EVAL_WATCH_BYTE;
watch_half			|
wh				return EVAL_WATCH_HALF;
watch_word			|
ww				return EVAL_WATCH_WORD;
watch_dword			|
wd				return EVAL_WATCH_DWORD;
delete_watch			|
dw				return EVAL_DELETE_WATCH;
dump				|
d				return EVAL_DUMP;
disasm				|
unasm				|
u				return EVAL_DISASM;
help				|
h				return EVAL_HELP;
[#][^\n]*			/* comment */
[pP][cC]                        { YY_LVALP->scalar.type=SCALAR_REG; YY_LVALP->scalar.scalar.reg.type=REG_PC; return EVAL_REG_PC; }
[cC][rR]			{ YY_LVALP->scalar.type=SCALAR_REG; YY_LVALP->scalar.scalar.reg.type=REG_CR; return EVAL_REG_CR; }
[lL][rR]			{ YY_LVALP->scalar.type=SCALAR_REG; YY_LVALP->scalar.scalar.reg.type=REG_LR; return EVAL_REG_LR; }
[xX][eE][rR]			{ YY_LVALP->scalar.type=SCALAR_REG; YY_LVALP->scalar.scalar.reg.type=REG_XER; return EVAL_REG_XER; }
[cC][tT][rR]			{ YY_LVALP->scalar.type=SCALAR_REG; YY_LVALP->scalar.scalar.reg.type=REG_CTR; return EVAL_REG_CTR; }
[mM][sS][rR]			{ YY_LVALP->scalar.type=SCALAR_REG; YY_LVALP->scalar.scalar.reg.type=REG_MSR; return EVAL_REG_MSR; }
[sS][rR][rR]0			{ YY_LVALP->scalar.type=SCALAR_REG; YY_LVALP->scalar.scalar.reg.type=REG_SRR0; return EVAL_REG_SRR0; }
[sS][rR][rR]1			{ YY_LVALP->scalar.type=SCALAR_REG; YY_LVALP->scalar.scalar.reg.type=REG_SRR1; return EVAL_REG_SRR1; }
[rR][0-3]?[0-9]			if (parse_gpr(&YY_LVALP->scalar, yytext)) return EVAL_GPR;
[fF][rR][0-3]?[0-9]		if (parse_fpr(&YY_LVALP->scalar, yytext)) return EVAL_FPR;
[$@a-zA-Z_][a-zA-Z0-9_]*	YY_LVALP->ident=strdup(yytext); return EVAL_IDENT;
[0-9]+\.[0-9]+([eE][+-]?[0-9]+)?	if (parse_float(&YY_LVALP->scalar, yytext)) return EVAL_FLOAT;
[0-9]+				if (parse_integer(&YY_LVALP->scalar, yytext, 10, 0)) return EVAL_INT;
0x[0-9a-fA-F]+			if (parse_integer(&YY_LVALP->scalar, yytext+2, 16, 0)) return EVAL_INT;
[0-9][0-9a-fA-F]*h		if (parse_integer(&YY_LVALP->scalar, yytext, 16, -1)) return EVAL_INT;
[0-9]+d				if (parse_integer(&YY_LVALP->scalar, yytext, 10, -1)) return EVAL_INT;
[0-7]+o				if (parse_integer(&YY_LVALP->scalar, yytext, 8, -1)) return EVAL_INT;
[0-1]+b				if (parse_integer(&YY_LVALP->scalar, yytext, 2, -1)) return EVAL_INT;
\n				return '\n';
.				return *yytext;

%%

