/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     EVAL_INT = 258,
     EVAL_GPR = 259,
     EVAL_FPR = 260,
     EVAL_REG_PC = 261,
     EVAL_REG_CR = 262,
     EVAL_REG_LR = 263,
     EVAL_REG_CTR = 264,
     EVAL_REG_XER = 265,
     EVAL_REG_MSR = 266,
     EVAL_REG_SRR0 = 267,
     EVAL_REG_SRR1 = 268,
     EVAL_STR = 269,
     EVAL_FLOAT = 270,
     EVAL_IDENT = 271,
     EVAL_PRINT = 272,
     EVAL_SETREG = 273,
     EVAL_REGS = 274,
     EVAL_FLOATS = 275,
     EVAL_BREAK = 276,
     EVAL_LIST_BREAK = 277,
     EVAL_STEP = 278,
     EVAL_NEXT = 279,
     EVAL_CONTINUE = 280,
     EVAL_QUIT = 281,
     EVAL_E2P = 282,
     EVAL_INSPECT_BYTE = 283,
     EVAL_INSPECT_HALF = 284,
     EVAL_INSPECT_WORD = 285,
     EVAL_INSPECT_DWORD = 286,
     EVAL_INSPECT_STRING = 287,
     EVAL_INSPECT_MEM = 288,
     EVAL_WATCH = 289,
     EVAL_WATCH_BYTE = 290,
     EVAL_WATCH_HALF = 291,
     EVAL_WATCH_WORD = 292,
     EVAL_WATCH_DWORD = 293,
     EVAL_DELETE_WATCH = 294,
     EVAL_DUMP = 295,
     EVAL_DISASM = 296,
     EVAL_HELP = 297,
     EVAL_LAND = 298,
     EVAL_LXOR = 299,
     EVAL_LOR = 300,
     EVAL_EQ = 301,
     EVAL_NE = 302,
     EVAL_LT = 303,
     EVAL_LE = 304,
     EVAL_GT = 305,
     EVAL_GE = 306,
     EVAL_SHL = 307,
     EVAL_SHR = 308,
     NEG = 309,
     EVAL_POW = 310
   };
#endif
/* Tokens.  */
#define EVAL_INT 258
#define EVAL_GPR 259
#define EVAL_FPR 260
#define EVAL_REG_PC 261
#define EVAL_REG_CR 262
#define EVAL_REG_LR 263
#define EVAL_REG_CTR 264
#define EVAL_REG_XER 265
#define EVAL_REG_MSR 266
#define EVAL_REG_SRR0 267
#define EVAL_REG_SRR1 268
#define EVAL_STR 269
#define EVAL_FLOAT 270
#define EVAL_IDENT 271
#define EVAL_PRINT 272
#define EVAL_SETREG 273
#define EVAL_REGS 274
#define EVAL_FLOATS 275
#define EVAL_BREAK 276
#define EVAL_LIST_BREAK 277
#define EVAL_STEP 278
#define EVAL_NEXT 279
#define EVAL_CONTINUE 280
#define EVAL_QUIT 281
#define EVAL_E2P 282
#define EVAL_INSPECT_BYTE 283
#define EVAL_INSPECT_HALF 284
#define EVAL_INSPECT_WORD 285
#define EVAL_INSPECT_DWORD 286
#define EVAL_INSPECT_STRING 287
#define EVAL_INSPECT_MEM 288
#define EVAL_WATCH 289
#define EVAL_WATCH_BYTE 290
#define EVAL_WATCH_HALF 291
#define EVAL_WATCH_WORD 292
#define EVAL_WATCH_DWORD 293
#define EVAL_DELETE_WATCH 294
#define EVAL_DUMP 295
#define EVAL_DISASM 296
#define EVAL_HELP 297
#define EVAL_LAND 298
#define EVAL_LXOR 299
#define EVAL_LOR 300
#define EVAL_EQ 301
#define EVAL_NE 302
#define EVAL_LT 303
#define EVAL_LE 304
#define EVAL_GT 305
#define EVAL_GE 306
#define EVAL_SHL 307
#define EVAL_SHR 308
#define NEG 309
#define EVAL_POW 310




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 18 "debugparse.y"

	struct eval_command command;
	struct eval_scalar scalar;
	eval_commandtype commandtoken;
	char *ident;
	struct eval_scalarlist scalars;



/* Line 2068 of yacc.c  */
#line 170 "debugparse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




