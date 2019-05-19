/*
* UAE - The Un*x Amiga Emulator
*
* Library Info from FD Files
*
* Written by Christian Vogelgsang <chris@vogelgsang.org>
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "uae/libinfo.h"

typedef struct {
    int lvo;
    int pub;
    int line;
    lib_info_t *li;
    func_info_t *cur_func;
    str_p err_str;
} state_t;

static int parse_cmd(char *line, state_t *s)
{
    /* check for arg */
    char *arg = strchr(line, ' ');
    if(arg != NULL) {
        *arg = '\0';
        arg++;
    }

    if(strcmp(line, "base")==0) {
        /* ignore base name */
    }
    else if(strcmp(line, "bias")==0) {
        s->lvo = atoi(arg);
    }
    else if(strcmp(line, "public")==0) {
        s->pub = 1;
    }
    else if(strcmp(line, "private")==0) {
        s->pub = 0;
    }
    else if(strcmp(line, "end")==0) {
        /* ignore */
    }
    else {
        *s->err_str = "unknown command";
        return 1;
    }
    return 0;
}

#define MAX_ARG 10

static func_info_t *parse_func(char *line, state_t *s)
{
    char *arg_names[MAX_ARG];
    char *arg_regs[MAX_ARG];
    int num_arg_names;
    int num_arg_regs;

    /* scan name */
    char *func_name = line;
    while(*line != '\0') {
        if(*line == '(') {
            *line = '\0';
            line++;
            break;
        }
        else if(!isalnum(*line)) {
            *s->err_str = "func name?";
            return NULL;
        }
        line++;
    }

    /* scan arg names */
    int begin = 1;
    num_arg_names = 0;
    while(*line != '\0') {
        if(*line == ',') {
            begin = 1;
            *line = '\0';
            line++;
        }
        else if(*line == ')') {
            *line = '\0';
            line++;
            break;
        }
        else if(isalnum(*line) || *line == '_') {
            if(begin) {
                if(num_arg_names == MAX_ARG) {
                    *s->err_str = "too many arg names";
                    return NULL;
                }
                arg_names[num_arg_names] = line;
                num_arg_names++;
                begin = 0;
            }
            line++;
        }
        else {
            *s->err_str = "arg name?";
            return NULL;
        }
    }

    /* expect start of regs */
    if(*line != '(') {
        *s->err_str = "no arg reg begin?";
        return NULL;
    }
    line++;

    /* scan arg regs */
    begin = 1;
    num_arg_regs = 0;
    while(*line != '\0') {
        if(*line == ',' || *line == '/') {
            begin = 1;
            *line = '\0';
            line++;
        }
        else if(*line == ')') {
            *line = '\0';
            line++;
            break;
        }
        else if(isalnum(*line)) {
            if(begin) {
                if(num_arg_names == MAX_ARG) {
                    *s->err_str = "too many arg regs";
                    return NULL;
                }
                arg_regs[num_arg_regs] = line;
                num_arg_regs++;
                begin = 0;
            }
            line++;
        }
        else {
            *s->err_str = "arg reg?";
            return NULL;
        }
    }

    /* arg names regs size msimatch? */
    if(num_arg_regs != num_arg_names) {
        *s->err_str = "arg names vs regs?";
        return NULL;
    }

    /* fill func info */
    func_info_t *fi = (func_info_t *)malloc(sizeof(func_info_t));
    if(fi == NULL) {
        *s->err_str = "no mem";
        return NULL;
    }
    fi->next = NULL;
    fi->name = strdup(func_name);
    fi->lvo = s->lvo;
    fi->num_args = num_arg_regs;
    arg_info_t *args = (arg_info_t *)malloc(sizeof(arg_info_t) * num_arg_regs);
    fi->args = args;
    if(args == NULL) {
        *s->err_str = "no mem";
        free(fi);
        return NULL;
    }

    /* store args */
    for(int i=0;i<num_arg_regs;i++) {
        args[i].name = strdup(arg_names[i]);
        /* store reg */
        char *ar = arg_regs[i];
        int reg = 0;
        if(*ar == 'a' || *ar == 'A') {
            reg += 8;
        }
        reg += (ar[1] - '0') & 7;
        args[i].reg = reg;
    }

    return fi;
}

static int parse_fd_line(char *line, state_t *s)
{
    /* empty line */
    if(line[0] == 0) {
        return 0;
    }

    /* is it a comment line? */
    if(line[0] == '*') {
        return 0;
    }

    /* fd command ? */
    if(line[0] == '#' && line[1] == '#') {
        return parse_cmd(line+2, s);
    }

    /* func call */
    else if(isalpha(line[0])) {
        /* only public functions */
        if(s->pub) {
            func_info_t * fi = parse_func(line, s);
            if(fi != NULL) {
                /* store first func info in lib info */
                if(s->cur_func == NULL) {
                    s->li->funcs = fi;
                }
                else {
                    s->cur_func->next = fi;
                }
                s->cur_func = fi;
                s->li->num_funcs++;
            } else {
                return 1;
            }
        }
        /* update state */
        s->lvo += 6;
        return 0;
    }

    /* unknown line */
    else {
        *s->err_str = "unknown line";
        return 1;
    }
}

lib_info_t *libinfo_read_fd(const char *fd_file_name, str_p err_str)
{
    lib_info_t *li = (lib_info_t *)malloc(sizeof(lib_info_t));
    if(li == NULL) {
        return NULL;
    }
    li->num_funcs = 0;
    li->max_lvo = 0;
    li->funcs = NULL;
    li->name = strdup(fd_file_name);

    /* read fd file */
    FILE *fh = fopen(fd_file_name, "rb");
    if(fh == NULL) {
        *err_str = "error opening file";
        free(li->name);
        free(li);
        return NULL;
    }

    /* init state */
    state_t state = { 0, 0, 0 };
    state.li = li;
    state.cur_func = NULL;
    state.err_str = err_str;

    /* read file line by line */
    while(!feof(fh)) {
        char line[80];
        if(fgets(line, 80, fh) == NULL) {
            break;
        }

        /* remove lf */
        char *ptr = strchr(line, '\n');
        if(ptr != NULL) {
            *ptr = '\0';
        }

        if(parse_fd_line(line, &state) != 0) {
            libinfo_free(li);
            fclose(fh);
            return NULL;
        }
    }

    if(state.cur_func != NULL) {
        li->max_lvo = state.cur_func->lvo;
    }

    fclose(fh);
    return li;
}

void libinfo_dump(lib_info_t *li)
{
    printf("libinfo for '%s'\n", li->name);
    printf("num_funcs=%d max_lvo=%d\n", li->num_funcs, li->max_lvo);
    func_info_t *fi = li->funcs;
    while(fi != NULL) {
        printf("func %s, lvo=%d, num_args=%d\n", fi->name, fi->lvo, fi->num_args);
        arg_info_t *ai = fi->args;
        for(int i=0;i<fi->num_args;i++) {
            printf("  arg %s, reg=%d\n", ai[i].name, ai[i].reg);
        }
        fi = fi->next;
    }
}

void libinfo_free(lib_info_t *li)
{
    if(li == NULL) {
        return;
    }

    func_info_t *fi = li->funcs;
    while(fi != NULL) {
        func_info_t *next = fi->next;
        arg_info_t *ai = fi->args;
        for(int i=0;i<fi->num_args;i++) {
            free(ai[i].name);
        }
        free(fi->args);
        free(fi->name);
        free(fi);
        fi = next;
    }

    free(li->name);
    free(li);
}

func_info_t *libinfo_find_func(lib_info_t *li, const char *name)
{
    func_info_t *fi = li->funcs;
    while(fi != NULL) {
        if(strcmp(fi->name, name)==0) {
            return fi;
        }
        fi = fi->next;
    }
    return NULL;
}
