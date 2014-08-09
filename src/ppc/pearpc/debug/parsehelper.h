/* 
 *	HT Editor
 *	parsehelper.h
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

#ifndef __PARSEHELPER_H__
#define __PARSEHELPER_H__

/*#define EVAL_DEBUG*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugtype.h"

#include "lex.h"

/*
 *
 */

#define MAX_FUNCNAME_LEN	16
#define MAX_SYMBOLNAME_LEN	32
#define MAX_ERRSTR_LEN		128

/*
 *	ERROR HANDLING
 */
void clear_eval_error();
int get_eval_error(char **str, int *pos);
void set_eval_error(char *errmsg);

/*
 *	SCALARLIST
 */
void scalarlist_set(struct eval_scalarlist *l, struct eval_scalar *s);
void scalarlist_concat(struct eval_scalarlist *l, struct eval_scalarlist *a, struct eval_scalarlist *b);
void scalarlist_destroy(struct eval_scalarlist *l);
void scalarlist_destroy_flat(struct eval_scalarlist *l);

/*
 *
 */
void create_command(struct eval_command *result, eval_commandtype type, int args, ...);
void create_func_call(struct eval_scalar *result, const char *name, int args, ...);
void create_func_call_list(struct eval_scalar *result, const char *name, struct eval_scalarlist *args);

/*
 *	SCALAR
 */
void scalar_destroy(struct eval_scalar *s);

int eval_parse(struct eval_command *r, const char *str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PARSEHELPER_H__ */
