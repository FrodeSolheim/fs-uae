#ifndef EVALTYPE_H
#define EVALTYPE_H

#define MAX_EVALFUNC_PARAMS	8

#include "system/types.h"

/*
 *	Types
 */

typedef enum {
	TYPE_UNKNOWN,
	TYPE_BYTE,
	TYPE_HALF,
	TYPE_WORD,
	TYPE_DWORD,
} eval_inttype;

typedef enum {
	REG_GPR,
	REG_FPR,
	REG_PC,
	REG_CR,
	REG_LR,
	REG_CTR,
	REG_XER,
	REG_MSR,
	REG_SRR0,
	REG_SRR1,
} eval_regtype;

struct eval_int {
	sint64 value;
	eval_inttype type;
};

struct eval_float {
	double value;
};

struct eval_str {
	char *value;
	int len;
};

struct eval_reg {
	eval_regtype type;
	int num;
};

struct eval_scalar;

struct eval_function {
	char *name;
	int param_count;
	struct eval_scalar *params;
};

typedef enum {
	SCALAR_EMPTY = 0,
	SCALAR_INT,
	SCALAR_REG,
	SCALAR_STR,
	SCALAR_FLOAT,
	SCALAR_FUNCTION,
	SCALAR_ANY,
	SCALAR_VARARGS,
} eval_scalartype;

typedef union {
	struct eval_int integer;
	struct eval_str str;
	struct eval_reg reg;
	struct eval_float floatnum;
	struct eval_function function;
} eval_scalarbody;

struct eval_scalar {
	eval_scalartype type;
	eval_scalarbody scalar;
};

struct eval_scalarlist {
	int count;
	struct eval_scalar *scalars;
};

typedef enum {
	COMMAND_NOP = 0,
	COMMAND_PRINT,
	COMMAND_REGS,
	COMMAND_FLOATS,
	COMMAND_SETREG,
	COMMAND_BREAK,
	COMMAND_LIST_BREAK,
	COMMAND_STEP,
	COMMAND_NEXT,
	COMMAND_CONTINUE,
	COMMAND_QUIT,
	COMMAND_E2P,
	COMMAND_INSPECT_BYTE,
	COMMAND_INSPECT_HALF,
	COMMAND_INSPECT_WORD,
	COMMAND_INSPECT_DWORD,
	COMMAND_INSPECT_STRING,
	COMMAND_INSPECT_MEM,
	COMMAND_WATCH,
	COMMAND_WATCH_BYTE,
	COMMAND_WATCH_HALF,
	COMMAND_WATCH_WORD,
        COMMAND_WATCH_DWORD,
        COMMAND_DELETE_WATCH,
	COMMAND_DUMP,
	COMMAND_DISASM,
	COMMAND_HELP,
} eval_commandtype;

struct eval_command {
	eval_commandtype	type;
	int			paramcount;
	struct eval_scalar	param[3]; /* 3 ought to be enough for everybody ;) */
};

#endif /* EVALTYPE_H */
