/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 4 "debugparse.y"


#define YYPARSE_PARAM resultptr

#include "debugtype.h"
#include "parsehelper.h"

void yyerror (char *s)
{
	set_eval_error(s);
}



/* Line 268 of yacc.c  */
#line 86 "debugparse.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 293 of yacc.c  */
#line 18 "debugparse.y"

	struct eval_command command;
	struct eval_scalar scalar;
	eval_commandtype commandtoken;
	char *ident;
	struct eval_scalarlist scalars;



/* Line 293 of yacc.c  */
#line 242 "debugparse.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 254 "debugparse.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  73
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   486

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  10
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNRULES -- Number of states.  */
#define YYNSTATES  143

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      68,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    65,    52,     2,
      70,    71,    63,    62,    43,    61,    46,    64,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    45,     2,
       2,    69,     2,    44,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    51,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    50,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    47,    48,
      49,    53,    54,    55,    56,    57,    58,    59,    60,    66,
      67
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    14,    16,    18,    23,
      27,    29,    32,    35,    37,    39,    42,    45,    48,    51,
      54,    57,    61,    65,    69,    73,    77,    81,    84,    88,
      92,    95,    96,    98,    99,   102,   104,   106,   108,   110,
     112,   114,   116,   118,   120,   122,   124,   126,   128,   130,
     134,   138,   142,   146,   150,   154,   158,   162,   166,   170,
     174,   178,   182,   186,   190,   194,   198,   202,   206,   210,
     213,   217,   223,   227,   232,   233,   235,   237,   241
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      73,     0,    -1,    68,    -1,    74,    68,    -1,    78,    68,
      -1,    17,    78,    -1,    19,    -1,    20,    -1,    18,    77,
      69,    78,    -1,    21,    78,    76,    -1,    22,    -1,    23,
      75,    -1,    24,    75,    -1,    25,    -1,    26,    -1,    27,
      78,    -1,    28,    78,    -1,    29,    78,    -1,    30,    78,
      -1,    31,    78,    -1,    32,    78,    -1,    33,    78,    76,
      -1,    34,    78,    76,    -1,    35,    78,    76,    -1,    36,
      78,    76,    -1,    37,    78,    76,    -1,    38,    78,    76,
      -1,    39,    78,    -1,    40,    78,    76,    -1,    41,    75,
      76,    -1,    42,    75,    -1,    -1,    78,    -1,    -1,    43,
      78,    -1,     4,    -1,     5,    -1,     6,    -1,     7,    -1,
       8,    -1,     9,    -1,    10,    -1,    11,    -1,    12,    -1,
      13,    -1,     3,    -1,    77,    -1,    15,    -1,    14,    -1,
      78,    63,    78,    -1,    78,    64,    78,    -1,    78,    65,
      78,    -1,    78,    62,    78,    -1,    78,    61,    78,    -1,
      78,    52,    78,    -1,    78,    50,    78,    -1,    78,    51,
      78,    -1,    78,    67,    78,    -1,    78,    59,    78,    -1,
      78,    60,    78,    -1,    78,    53,    78,    -1,    78,    54,
      78,    -1,    78,    57,    78,    -1,    78,    58,    78,    -1,
      78,    55,    78,    -1,    78,    56,    78,    -1,    78,    47,
      78,    -1,    78,    48,    78,    -1,    78,    49,    78,    -1,
      61,    78,    -1,    70,    78,    71,    -1,    78,    44,    78,
      45,    78,    -1,    78,    46,    78,    -1,    81,    70,    79,
      71,    -1,    -1,    80,    -1,    78,    -1,    80,    43,    80,
      -1,    16,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   102,   102,   103,   104,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   135,   140,   143,   148,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   199,   205,   208,   209,   217
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "EVAL_INT", "EVAL_GPR", "EVAL_FPR",
  "EVAL_REG_PC", "EVAL_REG_CR", "EVAL_REG_LR", "EVAL_REG_CTR",
  "EVAL_REG_XER", "EVAL_REG_MSR", "EVAL_REG_SRR0", "EVAL_REG_SRR1",
  "EVAL_STR", "EVAL_FLOAT", "EVAL_IDENT", "EVAL_PRINT", "EVAL_SETREG",
  "EVAL_REGS", "EVAL_FLOATS", "EVAL_BREAK", "EVAL_LIST_BREAK", "EVAL_STEP",
  "EVAL_NEXT", "EVAL_CONTINUE", "EVAL_QUIT", "EVAL_E2P",
  "EVAL_INSPECT_BYTE", "EVAL_INSPECT_HALF", "EVAL_INSPECT_WORD",
  "EVAL_INSPECT_DWORD", "EVAL_INSPECT_STRING", "EVAL_INSPECT_MEM",
  "EVAL_WATCH", "EVAL_WATCH_BYTE", "EVAL_WATCH_HALF", "EVAL_WATCH_WORD",
  "EVAL_WATCH_DWORD", "EVAL_DELETE_WATCH", "EVAL_DUMP", "EVAL_DISASM",
  "EVAL_HELP", "','", "'?'", "':'", "'.'", "EVAL_LAND", "EVAL_LXOR",
  "EVAL_LOR", "'|'", "'^'", "'&'", "EVAL_EQ", "EVAL_NE", "EVAL_LT",
  "EVAL_LE", "EVAL_GT", "EVAL_GE", "EVAL_SHL", "EVAL_SHR", "'-'", "'+'",
  "'*'", "'/'", "'%'", "NEG", "EVAL_POW", "'\\n'", "'='", "'('", "')'",
  "$accept", "input", "command", "optional_scalar1", "optional_scalar2",
  "reg", "scalar", "scalarlist_or_null", "scalarlist", "identifier", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,    44,    63,    58,    46,   298,   299,   300,
     124,    94,    38,   301,   302,   303,   304,   305,   306,   307,
     308,    45,    43,    42,    47,    37,   309,   310,    10,    61,
      40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    72,    73,    73,    73,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    75,    75,    76,    76,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    79,    79,    80,    80,    81
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     2,     1,     1,     4,     3,
       1,     2,     2,     1,     1,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     2,     3,     3,
       2,     0,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       3,     5,     3,     4,     0,     1,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    45,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    48,    47,    78,     0,     0,     6,     7,     0,
      10,    31,    31,    13,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      31,     0,     2,     0,     0,     0,    46,     0,     0,     5,
       0,    33,    11,    32,    12,    15,    16,    17,    18,    19,
      20,    33,    33,    33,    33,    33,    33,    27,    33,    33,
      30,    69,     0,     1,     3,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,    74,     0,
       0,     9,    21,    22,    23,    24,    25,    26,    28,    29,
      70,     0,    72,    66,    67,    68,    55,    56,    54,    60,
      61,    64,    65,    62,    63,    58,    59,    53,    52,    49,
      50,    51,    57,    76,     0,    75,     8,    34,     0,    73,
       0,    71,    77
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    44,    45,    52,   101,    46,    53,   134,   135,    48
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -70
static const yytype_int16 yypact[] =
{
     138,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,
     -70,   -70,   -70,   -70,   -70,    -2,   447,   -70,   -70,    -2,
     -70,    -2,    -2,   -70,   -70,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,   -70,    -2,    21,   -46,   -70,   163,   -30,   235,
     -45,   189,   -70,   235,   -70,   235,   235,   235,   235,   235,
     235,   189,   189,   189,   189,   189,   189,   235,   189,   -20,
     -70,   -25,    57,   -70,   -70,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,   -70,    -2,    -2,
      -2,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,
     -70,   213,   256,   133,   275,   293,   310,   326,   341,   356,
     370,   383,   -12,    72,     2,   402,   419,   -47,   -47,   -25,
     -25,   -25,   -70,   235,   -17,    15,   235,   235,    -2,   -70,
      -2,   235,   -70
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -70,   -70,   -70,    17,   409,    54,     0,   -70,   -69,   -70
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      47,     1,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    49,    93,    94,    95,    51,
      96,    73,    74,   100,    99,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    54,
      98,    71,    96,    72,    -1,    87,    88,    89,    90,    91,
      92,    93,    94,    95,   139,    96,    69,    70,   140,    41,
      -1,    89,    90,    91,    92,    93,    94,    95,    43,    96,
      50,   142,     0,     0,     0,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,     0,   133,   136,
     137,    75,     0,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,     0,    96,     0,     0,     0,   110,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,   141,    96,
     133,     1,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    41,
      96,     0,     0,     0,     0,     0,    42,    75,    43,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     0,
      96,    97,   100,    75,     0,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     0,    96,    75,   138,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    75,
      96,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,     0,    96,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,     0,    96,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,     0,    96,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     0,
      96,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,     0,    96,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,     0,    96,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,     0,    96,    -1,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,     0,    96,    -1,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,     0,    96,    -1,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     0,
      96,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    90,    91,    92,    93,    94,    95,     0,    96,
     102,   103,   104,   105,   106,   107,     0,   108,   109,    -1,
      91,    92,    93,    94,    95,     0,    96
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-70))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-1))

static const yytype_int16 yycheck[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    15,    63,    64,    65,    19,
      67,     0,    68,    43,    69,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    22,
      70,    41,    67,    43,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    71,    67,    39,    40,    43,    61,
      58,    59,    60,    61,    62,    63,    64,    65,    70,    67,
      16,   140,    -1,    -1,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    -1,    98,    99,
     100,    44,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    67,    -1,    -1,    -1,    71,    57,
      58,    59,    60,    61,    62,    63,    64,    65,   138,    67,
     140,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    61,
      67,    -1,    -1,    -1,    -1,    -1,    68,    44,    70,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      67,    68,    43,    44,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    67,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    44,
      67,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      67,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    67,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    67,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      67,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    59,    60,    61,    62,    63,    64,    65,    -1,    67,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    60,
      61,    62,    63,    64,    65,    -1,    67
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    61,    68,    70,    73,    74,    77,    78,    81,    78,
      77,    78,    75,    78,    75,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    75,
      75,    78,    78,     0,    68,    44,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    67,    68,    70,    69,
      43,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      71,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    79,    80,    78,    78,    45,    71,
      43,    78,    80
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 102 "debugparse.y"
    { create_command((struct eval_command*)resultptr, COMMAND_NOP, 0); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 103 "debugparse.y"
    { *(struct eval_command*)resultptr = (yyvsp[(1) - (2)].command); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 104 "debugparse.y"
    { create_command((struct eval_command*)resultptr, COMMAND_PRINT, 1, &(yyvsp[(1) - (2)].scalar)); }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 107 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_PRINT, 1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 108 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_REGS, 0); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 109 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_FLOATS, 0); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 110 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_SETREG, 2, &(yyvsp[(2) - (4)].scalar), &(yyvsp[(4) - (4)].scalar)); }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 111 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_BREAK, 2, &(yyvsp[(2) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 112 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_LIST_BREAK, 0); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 113 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_STEP, 1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 114 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_NEXT, 1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 115 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_CONTINUE, 0); }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 116 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_QUIT, 0); }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 117 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_E2P, 1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 118 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_INSPECT_BYTE, 1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 119 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_INSPECT_HALF, 1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 120 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_INSPECT_WORD, 1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 121 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_INSPECT_DWORD, 1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 122 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_INSPECT_STRING, 1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 123 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_INSPECT_MEM, 1, &(yyvsp[(2) - (3)].scalar)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 124 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_WATCH, 2, &(yyvsp[(2) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 125 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_WATCH_BYTE, 2, &(yyvsp[(2) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 126 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_WATCH_HALF, 2, &(yyvsp[(2) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 127 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_WATCH_WORD, 2, &(yyvsp[(2) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 128 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_WATCH_DWORD, 2, &(yyvsp[(2) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 129 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_DELETE_WATCH, 1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 130 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_DUMP, 2, &(yyvsp[(2) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 131 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_DISASM, 2, &(yyvsp[(2) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 132 "debugparse.y"
    { create_command(&(yyval.command), COMMAND_HELP, 0); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 135 "debugparse.y"
    {
						struct eval_scalar s;
						s.type = SCALAR_EMPTY;
						(yyval.scalar) = s;
					}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 140 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 143 "debugparse.y"
    {
						struct eval_scalar s;
						s.type = SCALAR_EMPTY;
						(yyval.scalar) = s;
					}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 148 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(2) - (2)].scalar); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 152 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 153 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 154 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 155 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 156 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 157 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 158 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 159 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 160 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 161 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 164 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 165 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 166 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 167 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(1) - (1)].scalar); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 168 "debugparse.y"
    { create_func_call(&(yyval.scalar), "*",  2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 169 "debugparse.y"
    { create_func_call(&(yyval.scalar), "/",  2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 170 "debugparse.y"
    { create_func_call(&(yyval.scalar), "%",  2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 171 "debugparse.y"
    { create_func_call(&(yyval.scalar), "+",  2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 172 "debugparse.y"
    { create_func_call(&(yyval.scalar), "-",  2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 173 "debugparse.y"
    { create_func_call(&(yyval.scalar), "&",  2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 174 "debugparse.y"
    { create_func_call(&(yyval.scalar), "|",  2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 175 "debugparse.y"
    { create_func_call(&(yyval.scalar), "^",  2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 176 "debugparse.y"
    { create_func_call(&(yyval.scalar), "**", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 177 "debugparse.y"
    { create_func_call(&(yyval.scalar), "<<", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 178 "debugparse.y"
    { create_func_call(&(yyval.scalar), ">>", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 179 "debugparse.y"
    { create_func_call(&(yyval.scalar), "==", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 180 "debugparse.y"
    { create_func_call(&(yyval.scalar), "!=", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 181 "debugparse.y"
    { create_func_call(&(yyval.scalar), ">",  2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 182 "debugparse.y"
    { create_func_call(&(yyval.scalar), ">=", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 183 "debugparse.y"
    { create_func_call(&(yyval.scalar), "<",  2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 184 "debugparse.y"
    { create_func_call(&(yyval.scalar), "<=", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 185 "debugparse.y"
    { create_func_call(&(yyval.scalar), "&&", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 186 "debugparse.y"
    { create_func_call(&(yyval.scalar), "||", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 187 "debugparse.y"
    { create_func_call(&(yyval.scalar), "^^", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 188 "debugparse.y"
    { create_func_call(&(yyval.scalar), "-",  1, &(yyvsp[(2) - (2)].scalar)); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 189 "debugparse.y"
    { (yyval.scalar) = (yyvsp[(2) - (3)].scalar); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 190 "debugparse.y"
    { create_func_call(&(yyval.scalar), "?:", 3, &(yyvsp[(1) - (5)].scalar), &(yyvsp[(3) - (5)].scalar), &(yyvsp[(5) - (5)].scalar)); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 191 "debugparse.y"
    { create_func_call(&(yyval.scalar), ".", 2, &(yyvsp[(1) - (3)].scalar), &(yyvsp[(3) - (3)].scalar)); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 193 "debugparse.y"
    {	create_func_call_list(&(yyval.scalar), (yyvsp[(1) - (4)].ident), &(yyvsp[(3) - (4)].scalars)); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 199 "debugparse.y"
    {
			struct eval_scalarlist s;
			s.count = 0;
			s.scalars = NULL;
			(yyval.scalars) = s;
		}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 205 "debugparse.y"
    { (yyval.scalars) = (yyvsp[(1) - (1)].scalars); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 208 "debugparse.y"
    { scalarlist_set(&(yyval.scalars), &(yyvsp[(1) - (1)].scalar)); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 210 "debugparse.y"
    {
			scalarlist_concat(&(yyval.scalars), &(yyvsp[(1) - (3)].scalars), &(yyvsp[(3) - (3)].scalars));
			scalarlist_destroy_flat(&(yyvsp[(1) - (3)].scalars));
			scalarlist_destroy_flat(&(yyvsp[(3) - (3)].scalars));
		}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 217 "debugparse.y"
    { (yyval.ident) = (yyvsp[(1) - (1)].ident); }
    break;



/* Line 1806 of yacc.c  */
#line 2235 "debugparse.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 220 "debugparse.y"


