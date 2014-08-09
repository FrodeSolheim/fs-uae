/*
 *	HT Editor
 *	parsehelper.c
 *
 *	Copyright (C) 2003 Stefan Weyergraf (stefan@weyergraf.de)
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

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "tools/snprintf.h"
#include "debugtype.h"
#include "parsehelper.h"
#include "lex.h"

/*
 *	ERROR HANDLING
 */
static int eval_error;
static int eval_error_pos;
static char eval_errstr[MAX_ERRSTR_LEN];

void clear_eval_error()
{
	eval_error = 0;
}

int get_eval_error(char **str, int *pos)
{
	if (eval_error) {
		if (str) *str = eval_errstr;
		if (pos) *pos = eval_error_pos;
		return eval_error;
	}
	if (str) *str = "?";
	if (pos) *pos = 0;
	return 0;
}

void set_eval_error(char *errmsg)
{
	strncpy(eval_errstr, errmsg, sizeof eval_errstr-1);
	eval_errstr[sizeof eval_errstr-1] = 0;
	eval_error_pos = lex_current_buffer_pos();
	eval_error = 1;
}

/*
 *	SCALARLIST
 */
void scalarlist_set(struct eval_scalarlist *l, struct eval_scalar *s)
{
	l->count = 1;
	l->scalars = (struct eval_scalar*)malloc(sizeof (struct eval_scalar) * l->count);
	l->scalars[0] = *s;
}

void scalarlist_concat(struct eval_scalarlist *l, struct eval_scalarlist *a, struct eval_scalarlist *b)
{
	l->count = a->count+b->count;
	l->scalars = (struct eval_scalar*)malloc(sizeof (struct eval_scalar) * l->count);
	memmove(l->scalars, a->scalars, sizeof (struct eval_scalar) * a->count);
	memmove(l->scalars+a->count, b->scalars, sizeof (struct eval_scalar) * b->count);
}

void scalarlist_destroy(struct eval_scalarlist *l)
{
	int i;
	if (l && l->scalars) {
		for (i=0; i < l->count; i++) {
			scalar_destroy(&l->scalars[i]);
		}
		free(l->scalars);
	}		
}

void scalarlist_destroy_flat(struct eval_scalarlist *l)
{
	if (l && l->scalars) free(l->scalars);
}

#ifdef EVAL_DEBUG

void scalarlist_dump(struct eval_scalarlist *l)
{
	int i;
	for (i=0; i < l->count; i++) {
		scalar_dump(&l->scalars[i]);
		if (i != l->count-1) {
			printf(", ");
		}
	}
}

#endif

/*
 *	STRING
 */
void string_destroy(struct eval_str *s)
{
	if (s->value) free(s->value);
}

/*
 *	SCALAR
 */
void scalar_destroy(struct eval_scalar *s)
{
	switch (s->type) {
		case SCALAR_STR:
			string_destroy(&s->scalar.str);
			break;
		default:
			break;
	}
}

/*
 *
 */
void create_command(struct eval_command *result, eval_commandtype type, int args, ...)
{
	va_list vargs;
	int i;
	memset(result, 0, sizeof (struct eval_command));
	result->type = type;
	va_start(vargs, args);
	result->paramcount = args;
	for (i=0; i<args; i++) {
		result->param[i] = *va_arg(vargs, struct eval_scalar*);
	}
	va_end(vargs);
}

void create_func_call(struct eval_scalar *result, const char *name, int args, ...)
{
	int i;
	va_list vargs;
	va_start(vargs, args);
	result->type = SCALAR_FUNCTION;
	result->scalar.function.name = strdup(name);
	result->scalar.function.param_count = args;
	result->scalar.function.params = (struct eval_scalar*)malloc(sizeof (struct eval_scalar)*args);
	for (i=0; i<args; i++) {
		result->scalar.function.params[i] = *va_arg(vargs, struct eval_scalar*);
	}
	va_end(vargs);
}

void create_func_call_list(struct eval_scalar *result, const char *name, struct eval_scalarlist *args)
{
	int i;
	result->type = SCALAR_FUNCTION;
	result->scalar.function.name = strdup(name);
	result->scalar.function.param_count = args->count;
	result->scalar.function.params = (struct eval_scalar*)malloc(sizeof (struct eval_scalar)*args->count);
	for (i=0; i<args->count; i++) {
		result->scalar.function.params[i] = args->scalars[i];
	}
}

/*
 *
 */
extern int yyparse(struct eval_command *result);
	
int eval_parse(struct eval_command *result, const char *str)
{
	void *oldbuffer = lex_current_buffer();
	void *strbuffer;
	strbuffer = lex_scan_string_buffer(str);

	clear_eval_error();

	result->type = COMMAND_NOP;
	yyparse(result);

	lex_delete_buffer(strbuffer);
	if (oldbuffer) lex_switch_buffer(oldbuffer);

	if (get_eval_error(NULL, NULL)) return 0;

	return 1;
}
