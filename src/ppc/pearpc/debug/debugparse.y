
%pure_parser

%{

#define YYPARSE_PARAM resultptr

#include "debugtype.h"
#include "parsehelper.h"

void yyerror (char *s)
{
	set_eval_error(s);
}

%}

%union {
	struct eval_command command;
	struct eval_scalar scalar;
	eval_commandtype commandtoken;
	char *ident;
	struct eval_scalarlist scalars;
};

%token <scalar> EVAL_INT
%token <scalar> EVAL_GPR
%token <scalar> EVAL_FPR
%token <scalar> EVAL_REG_PC
%token <scalar> EVAL_REG_CR
%token <scalar> EVAL_REG_LR
%token <scalar> EVAL_REG_CTR
%token <scalar> EVAL_REG_XER
%token <scalar> EVAL_REG_MSR
%token <scalar> EVAL_REG_SRR0
%token <scalar> EVAL_REG_SRR1
%token <scalar> EVAL_STR
%token <scalar> EVAL_FLOAT
%token <ident> EVAL_IDENT
%token <commandtoken> EVAL_PRINT
%token <commandtoken> EVAL_SETREG
%token <commandtoken> EVAL_REGS
%token <commandtoken> EVAL_FLOATS
%token <commandtoken> EVAL_BREAK
%token <commandtoken> EVAL_LIST_BREAK
%token <commandtoken> EVAL_STEP
%token <commandtoken> EVAL_NEXT
%token <commandtoken> EVAL_CONTINUE
%token <commandtoken> EVAL_QUIT
%token <commandtoken> EVAL_E2P
%token <commandtoken> EVAL_INSPECT_BYTE
%token <commandtoken> EVAL_INSPECT_HALF
%token <commandtoken> EVAL_INSPECT_WORD
%token <commandtoken> EVAL_INSPECT_DWORD
%token <commandtoken> EVAL_INSPECT_STRING
%token <commandtoken> EVAL_INSPECT_MEM
%token <commandtoken> EVAL_WATCH
%token <commandtoken> EVAL_WATCH_BYTE
%token <commandtoken> EVAL_WATCH_HALF
%token <commandtoken> EVAL_WATCH_WORD
%token <commandtoken> EVAL_WATCH_DWORD
%token <commandtoken> EVAL_DELETE_WATCH
%token <commandtoken> EVAL_DUMP
%token <commandtoken> EVAL_DISASM
%token <commandtoken> EVAL_HELP


%type <command> command
%type <scalar> scalar
%type <scalar> reg
%type <scalar> optional_scalar1
%type <scalar> optional_scalar2
%type <scalars> scalarlist
%type <scalars> scalarlist_or_null
%type <ident> identifier

%left ','
%right '?' ':'
%left '.'
%left EVAL_LAND
%left EVAL_LXOR
%left EVAL_LOR
%left '|'
%left '^'
%left '&'
%nonassoc EVAL_EQ
%nonassoc EVAL_NE
%nonassoc EVAL_LT
%nonassoc EVAL_LE
%nonassoc EVAL_GT
%nonassoc EVAL_GE
%nonassoc EVAL_SHL
%nonassoc EVAL_SHR
%left '-' '+'
%left '*' '/' '%'

%left NEG
%left EVAL_POW

%%

input:	  '\n'				{ create_command((struct eval_command*)resultptr, COMMAND_NOP, 0); }
	| command '\n'			{ *(struct eval_command*)resultptr = $1; }
	| scalar '\n'			{ create_command((struct eval_command*)resultptr, COMMAND_PRINT, 1, &$1); }
;

command:  EVAL_PRINT scalar				{ create_command(&$$, COMMAND_PRINT, 1, &$2); }
	| EVAL_REGS                                     { create_command(&$$, COMMAND_REGS, 0); }
	| EVAL_FLOATS					{ create_command(&$$, COMMAND_FLOATS, 0); }
	| EVAL_SETREG reg '=' scalar			{ create_command(&$$, COMMAND_SETREG, 2, &$2, &$4); }
	| EVAL_BREAK scalar optional_scalar2		{ create_command(&$$, COMMAND_BREAK, 2, &$2, &$3); }
	| EVAL_LIST_BREAK				{ create_command(&$$, COMMAND_LIST_BREAK, 0); }
	| EVAL_STEP optional_scalar1			{ create_command(&$$, COMMAND_STEP, 1, &$2); }
	| EVAL_NEXT optional_scalar1			{ create_command(&$$, COMMAND_NEXT, 1, &$2); }
	| EVAL_CONTINUE					{ create_command(&$$, COMMAND_CONTINUE, 0); }
	| EVAL_QUIT					{ create_command(&$$, COMMAND_QUIT, 0); }
	| EVAL_E2P scalar				{ create_command(&$$, COMMAND_E2P, 1, &$2); }
	| EVAL_INSPECT_BYTE scalar			{ create_command(&$$, COMMAND_INSPECT_BYTE, 1, &$2); }
	| EVAL_INSPECT_HALF scalar			{ create_command(&$$, COMMAND_INSPECT_HALF, 1, &$2); }
	| EVAL_INSPECT_WORD scalar			{ create_command(&$$, COMMAND_INSPECT_WORD, 1, &$2); }
	| EVAL_INSPECT_DWORD scalar			{ create_command(&$$, COMMAND_INSPECT_DWORD, 1, &$2); }
	| EVAL_INSPECT_STRING scalar			{ create_command(&$$, COMMAND_INSPECT_STRING, 1, &$2); }
	| EVAL_INSPECT_MEM scalar optional_scalar2	{ create_command(&$$, COMMAND_INSPECT_MEM, 1, &$2); }
	| EVAL_WATCH scalar optional_scalar2		{ create_command(&$$, COMMAND_WATCH, 2, &$2, &$3); }
	| EVAL_WATCH_BYTE scalar optional_scalar2	{ create_command(&$$, COMMAND_WATCH_BYTE, 2, &$2, &$3); }
	| EVAL_WATCH_HALF scalar optional_scalar2	{ create_command(&$$, COMMAND_WATCH_HALF, 2, &$2, &$3); }
	| EVAL_WATCH_WORD scalar optional_scalar2	{ create_command(&$$, COMMAND_WATCH_WORD, 2, &$2, &$3); }
	| EVAL_WATCH_DWORD scalar optional_scalar2	{ create_command(&$$, COMMAND_WATCH_DWORD, 2, &$2, &$3); }
	| EVAL_DELETE_WATCH scalar			{ create_command(&$$, COMMAND_DELETE_WATCH, 1, &$2); }
	| EVAL_DUMP scalar optional_scalar2		{ create_command(&$$, COMMAND_DUMP, 2, &$2, &$3); }
	| EVAL_DISASM optional_scalar1 optional_scalar2	{ create_command(&$$, COMMAND_DISASM, 2, &$2, &$3); }
	| EVAL_HELP optional_scalar1 			{ create_command(&$$, COMMAND_HELP, 0); }
;

optional_scalar1:	/* empty */	{
						struct eval_scalar s;
						s.type = SCALAR_EMPTY;
						$$ = s;
					}
	| scalar			{ $$ = $1; }
;

optional_scalar2:	/* empty */	{
						struct eval_scalar s;
						s.type = SCALAR_EMPTY;
						$$ = s;
					}
	| ',' scalar			{ $$ = $2; }
;

reg:
	  EVAL_GPR			{ $$ = $1; }
	| EVAL_FPR			{ $$ = $1; }
	| EVAL_REG_PC			{ $$ = $1; }
	| EVAL_REG_CR			{ $$ = $1; }
	| EVAL_REG_LR			{ $$ = $1; }
	| EVAL_REG_CTR			{ $$ = $1; }
	| EVAL_REG_XER			{ $$ = $1; }
	| EVAL_REG_MSR			{ $$ = $1; }
	| EVAL_REG_SRR0			{ $$ = $1; }
	| EVAL_REG_SRR1			{ $$ = $1; }
;

scalar:	  EVAL_INT			{ $$ = $1; }
	| reg				{ $$ = $1; }
	| EVAL_FLOAT			{ $$ = $1; }
	| EVAL_STR			{ $$ = $1; }
	| scalar '*' scalar		{ create_func_call(&$$, "*",  2, &$1, &$3); }
	| scalar '/' scalar		{ create_func_call(&$$, "/",  2, &$1, &$3); }
	| scalar '%' scalar		{ create_func_call(&$$, "%",  2, &$1, &$3); }
	| scalar '+' scalar		{ create_func_call(&$$, "+",  2, &$1, &$3); }
	| scalar '-' scalar		{ create_func_call(&$$, "-",  2, &$1, &$3); }
	| scalar '&' scalar		{ create_func_call(&$$, "&",  2, &$1, &$3); }
	| scalar '|' scalar		{ create_func_call(&$$, "|",  2, &$1, &$3); }
	| scalar '^' scalar		{ create_func_call(&$$, "^",  2, &$1, &$3); }
	| scalar EVAL_POW scalar	{ create_func_call(&$$, "**", 2, &$1, &$3); }
	| scalar EVAL_SHL scalar	{ create_func_call(&$$, "<<", 2, &$1, &$3); }
	| scalar EVAL_SHR scalar	{ create_func_call(&$$, ">>", 2, &$1, &$3); }
	| scalar EVAL_EQ scalar		{ create_func_call(&$$, "==", 2, &$1, &$3); }
	| scalar EVAL_NE scalar		{ create_func_call(&$$, "!=", 2, &$1, &$3); }
	| scalar EVAL_GT scalar		{ create_func_call(&$$, ">",  2, &$1, &$3); }
	| scalar EVAL_GE scalar		{ create_func_call(&$$, ">=", 2, &$1, &$3); }
	| scalar EVAL_LT scalar		{ create_func_call(&$$, "<",  2, &$1, &$3); }
	| scalar EVAL_LE scalar		{ create_func_call(&$$, "<=", 2, &$1, &$3); }
	| scalar EVAL_LAND scalar	{ create_func_call(&$$, "&&", 2, &$1, &$3); }
	| scalar EVAL_LXOR scalar	{ create_func_call(&$$, "||", 2, &$1, &$3); }
	| scalar EVAL_LOR scalar	{ create_func_call(&$$, "^^", 2, &$1, &$3); }
	| '-' scalar %prec NEG		{ create_func_call(&$$, "-",  1, &$2); }
	| '(' scalar ')'		{ $$ = $2; }
	| scalar '?' scalar ':' scalar	{ create_func_call(&$$, "?:", 3, &$1, &$3, &$5); }
	| scalar '.' scalar		{ create_func_call(&$$, ".", 2, &$1, &$3); }
	| identifier '(' scalarlist_or_null ')'
		{	create_func_call_list(&$$, $1, &$3); }
/*	| identifier
		{	create_func_call(&$$, $1, 0); } */
;

scalarlist_or_null:	/* empty */
		{
			struct eval_scalarlist s;
			s.count = 0;
			s.scalars = NULL;
			$$ = s;
		}
	| scalarlist			{ $$ = $1; }
;

scalarlist: scalar			{ scalarlist_set(&$$, &$1); }
	| scalarlist ',' scalarlist
		{
			scalarlist_concat(&$$, &$1, &$3);
			scalarlist_destroy_flat(&$1);
			scalarlist_destroy_flat(&$3);
		}
;

identifier: EVAL_IDENT			{ $$ = $1; }
;

%%
