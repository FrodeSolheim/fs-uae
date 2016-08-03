/*
* UAE - The Un*x Amiga Emulator
*
* Library Tracer
*
* Written by Christian Vogelgsang <chris@vogelgsang.org>
*/

#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"

#include "uae/memory.h"
#include "autoconf.h"
#include "traps.h"
#include "newcpu.h"
#include "uae/libtrace.h"
#include "uae/libinfo.h"
#include "uae/patch.h"
#include "debug.h"

#define JSR 0x4eb9
#define JMP 0x4ef9

#define MAX_LIBS 10

#define LIB_TAG_NONE 0
#define LIB_TAG_EXEC 1
#define LIB_TAG_DOS  2

#define FUNC_FLAG_VOID        1
#define FUNC_FLAG_PRE_DEBUG   2
#define FUNC_FLAG_POST_DEBUG  4
#define FUNC_FLAG_INVERT      8
#define FUNC_FLAG_ALL         16
#define FUNC_FLAG_EXTRA       32

typedef struct {
    char       *name;
    int         reg;
} func_arg_t;

typedef struct {
    uae_u32      func_addr;
    int          flags;
    func_info_t *info;
} func_entry_t;

typedef struct lvo_entry {
    struct lvo_entry *next;
    const char  *name;
    int          flags;
} lvo_entry_t;

typedef struct lib_entry {
    struct lib_entry *next;
    /* config section */
    const char   *lib_name;
    const char   *fd_name;
    lvo_entry_t  *lvo_names;
    /* init */
    char         *short_name;
    lib_info_t   *lib_info;
    int           tag;
    uae_u16       trap_code;
    /* runtime */
    uae_u32       lib_base;
    uae_u32       neg_size;
    int           num_funcs;
    func_entry_t *funcs;
} lib_entry_t;

typedef struct {
    lib_entry_t  *lib;
    func_entry_t *func;
    uae_u32       lvo;
    uae_u32       func_ptr;
    uae_u32       this_task;
    uae_u32       callee_pc;
    uae_u32       regs[16];
    uae_u32       return_regs[2];
    const char   *task_name;
} call_info_t;

// globals
static int enable;
static int reload;
static FILE *out_fh;
static const char *cfg_file;
static const char *task_name;
static const char *out_file;
static uae_u32 exec_base;
static lib_entry_t *libs;
static uae_u16      lib_traps[MAX_LIBS];

// addresses for traps and old lib addresses
static uae_u32 init_addr;
static uae_u32 setfunc_addr;
static uae_u32 old_setfunc_addr;
static uae_u32 oldopenlib_addr;
static uae_u32 old_oldopenlib_addr;
static uae_u32 openlib_addr;
static uae_u32 old_openlib_addr;
static uae_u32 log_post_addr;

static uae_u32 REGPARAM2 trap_init(TrapContext *ctx);
static uae_u32 REGPARAM2 trap_setfunc(TrapContext *ctx);
static uae_u32 REGPARAM2 trap_openlib(TrapContext *ctx);
static uae_u32 REGPARAM2 trap_trace_call_pre(TrapContext *ctx);
static uae_u32 REGPARAM2 trap_trace_call_post(TrapContext *ctx);

static int patch_lib_func(lib_entry_t *lib, uae_u32 lvo)
{
    uae_u32 ptr = lib->lib_base - lvo;

    // make sure its a valid lvo
    if(lvo % 6 != 0) {
        printf("libtrace: patch failed: invalid lvo=%d modulo! '%s'\n",
               lvo, lib->lib_name);
        return 0;
    }
    if(lvo > lib->neg_size) {
        printf("libtrace: patch failed: lvo=%d out of range! '%s'\n",
               lvo, lib->lib_name);
        return 0;
    }

    // only patch lib entry that has a JMP in the table
    if(get_word(ptr) != JMP) {
        printf("libtrace: patch failed: No JMP in library table! '%s' lib_base=%08x, lvo=%d\n",
               lib->lib_name, lib->lib_base, lvo);
        return 0;
    }

    /* place trap of library instead of JMP */
    put_word(ptr, lib->trap_code);

#if 0
    printf("libtrace patch lvo: %d func: %08x: old_addr=%08x  jmp=%04x\n",
           lvo, ptr, get_long(ptr+2), get_word(ptr));
#endif

    /* store address of lib pointer in func */
    func_entry_t *func = lib->funcs;
    int idx = (lvo - 6) / 6;
    func[idx].func_addr = ptr + 2;

    return 1;
}

static void pre_patch(TrapContext *ctx)
{
    /* Forbid */
    CallLib(ctx, exec_base, -132);
}

static void post_patch(TrapContext *ctx, uae_u32 lib_base)
{
    /* set LIBF_CHANGED(2) in lib_Flags */
    put_byte(lib_base + 14, get_byte(lib_base + 14) | 2);

    /* SumLibrary a1=lib */
    uae_u32 old_a1 = m68k_areg(regs, 1);
    m68k_areg(regs, 1) = lib_base;
    CallLib(ctx, exec_base, -426);
    m68k_areg(regs, 1) = old_a1;

    /* Permit */
    CallLib(ctx, exec_base, -138);
}

static uae_u32 get_lvo_for_name(lib_info_t *li, const char *lvo_name)
{
    /* check if its a name */
    if(li != NULL) {
        func_info_t *fi = libinfo_find_func(li, lvo_name);
        if(fi != NULL) {
            return fi->lvo;
        }
    }
    /* try numveric conversion */
    return atoi(lvo_name);
}

static void mark_func(lib_entry_t *lib, uae_u32 lvo, int on, int flags)
{
    func_entry_t *func = lib->funcs;
    int idx = (lvo - 6) / 6;
    func[idx].func_addr = on ? 0xffffffff : 0;
    func[idx].flags = flags;
}

static int mark_patch_funcs(lib_entry_t *lib)
{
    /* is info available? */
    lib_info_t *li = lib->lib_info;

    /* get lvo entries from config */
    lvo_entry_t *lvo_ptr = lib->lvo_names;
    if(lvo_ptr == NULL)
    {
        return 0;
    }

    /* run through config */
    int num_patches = 0;
    while(lvo_ptr != NULL) {
        int flags = lvo_ptr->flags;

        /* patch all entries? */
        if(flags & FUNC_FLAG_ALL) {
            /* if info is available then patch only known functions */
            if(li != NULL) {
                func_info_t *finfo = li->funcs;
                while(finfo!=NULL) {
                    mark_func(lib, finfo->lvo, 1, flags);
                    num_patches++;
                    finfo = finfo->next;
                }
            }
            /* otherwise patch all */
            else {
                uae_u32 lvo = 30;
                while(lvo < lib->neg_size) {
                    mark_func(lib, lvo, 1, flags);
                    num_patches++;
                    lvo += 6;
                }
            }
        }
        /* normal entry with name and flags */
        else {
            const char *lvo_name = lvo_ptr->name;
            uae_u32 lvo = get_lvo_for_name(li, lvo_name);
            /* patch? */
            if(lvo > 0) {
                int on = (flags & FUNC_FLAG_INVERT) == 0;
                mark_func(lib, lvo, on, flags);
                num_patches++;
            }
        }
        lvo_ptr = lvo_ptr->next;
    }
    return num_patches;
}

static void patch_lib(TrapContext *ctx, lib_entry_t *lib)
{
    printf("libtrace: patch lib @%08x '%s' #%d\n",
           lib->lib_base, lib->lib_name, lib->num_funcs);

    /* make functions to be patched */
    int num_patches = mark_patch_funcs(lib);
    if(num_patches == 0) {
        printf("libtrace: NO PATCHES FOUND!\n");
        return;
    }

    pre_patch(ctx);

    /* perform patching */
    uae_u32 lvo = 6;
    func_entry_t *func = lib->funcs;
    for(int i=0; i < lib->num_funcs; i++) {
        if(func->func_addr != 0) {
            patch_lib_func(lib, lvo);
        }
        lvo += 6;
        func++;
    }

    /* if patching exec's StackSwap() function then it is crucial
       to set the FUNC_FLAG_VOID to avoid the post trace call.
       It won't work after changing the stack! */
    if(lib->tag == LIB_TAG_EXEC) {
        // LVO=-732 -> offset 121
        if(lib->funcs[121].func_addr != 0) {
            lib->funcs[121].flags |= FUNC_FLAG_VOID;
        }
    }

    post_patch(ctx, lib->lib_base);
}

static void unpatch_lib(lib_entry_t *lib)
{
    func_entry_t *func = lib->funcs;
    for(int i=0;i<lib->num_funcs;i++) {
        if(func->func_addr != 0) {
            uae_u32 addr = func->func_addr - 2;
            put_word(addr, JMP);
            func->func_addr = 0;
        }
        func++;
    }
}

static void activate_libtrace(TrapContext *ctx, lib_entry_t *lib, uae_u32 lib_base)
{
    // store base
    lib->lib_base = lib_base;

    // number of functions
    lib->neg_size = get_word(lib_base + 16);
    lib->num_funcs = lib->neg_size / 6; // neg_size

    printf("libtrace: activate '%s': funcs=%d\n", lib->lib_name, lib->num_funcs);

    // allocate funcs
    size_t num_bytes = sizeof(func_entry_t) * lib->num_funcs;
    lib->funcs = (func_entry_t *)malloc(num_bytes);
    memset(lib->funcs, 0, num_bytes);

    // attach func info to funcs (if available)
    func_info_t *finfo = NULL;
    if(lib->lib_info != NULL) {
        finfo = lib->lib_info->funcs;
    }
    func_entry_t *f = lib->funcs;
    int lvo = 6;
    for(int i=0; i < lib->num_funcs; i++) {
        // skip to next lvo
        while((finfo != NULL) && (finfo->lvo < lvo)) {
            finfo = finfo->next;
        }
        // if lvo has info then store it in func
        if((finfo != NULL) && (finfo->lvo == lvo)) {
            f->info = finfo;
        }
        f++;
        lvo+=6;
    }

    // finally patch lib
    if(lib->funcs != NULL) {
        patch_lib(ctx, lib);
    }
}

static void deactivate_libtrace(lib_entry_t *lib, int unpatch)
{
    if(lib->lib_base != 0) {
        printf("libtrace: deactivate '%s'\n", lib->lib_name);

        // set base to 0 to free entry
        lib->lib_base = 0;
        lib->num_funcs = 0;
        if(lib->funcs != NULL) {
            // really unpatch and remove TRAPs from lib jump table?
            if(unpatch) {
                unpatch_lib(lib);
            }
            free(lib->funcs);
            lib->funcs = NULL;
        }
    }
}

static void deactivate_libtraces(int unpatch)
{
    lib_entry_t *lib = libs;
    while(lib != NULL) {
        deactivate_libtrace(lib, unpatch);
        lib = lib->next;
    }
}

static lib_entry_t *find_lib_by_trap_code(uae_u16 trap_code)
{
    lib_entry_t *lib = libs;
    while(lib != NULL) {
        if(lib->trap_code == trap_code) {
            return lib;
        }
        lib = lib->next;
    }
    return NULL;
}

static lib_entry_t *find_lib_by_name(const char *name)
{
    lib_entry_t *lib = libs;
    while(lib != NULL) {
        if(strcmp(lib->lib_name, name)==0) {
            return lib;
        }
        lib = lib->next;
    }
    return NULL;
}

// ---------- CONFIG --------------------------------------------------------

static int read_line(char *buf, int size, FILE *fh)
{
    char *result = fgets(buf, size, fh);
    if(result == NULL) {
        return -1;
    }
    int n = strlen(buf);
    // stip newline/return
    if(n > 0) {
        if(buf[n-1] == '\n') {
            buf[n-1] = '\0';
            n--;
        }
    }
    if(n > 0) {
        if(buf[n-1] == '\r') {
            buf[n-1] = '\0';
            n--;
        }
    }
    return n;
}

static lib_entry_t *read_config(const char *cfg_file)
{
    char line[80];

    printf("libtrace: reading config '%s'\n", cfg_file);
    FILE *fh = fopen(cfg_file, "r");
    if(fh == NULL) {
        return NULL;
    }

    // read libs
    lib_entry_t *lib = NULL;
    int num_libs = 0;
    while(1) {
        // read lib name
        int n = read_line(line, 79, fh);
        if(n<0) {
            break;
        }
        // end of config
        if((n==0) || (line[0]=='#')) {
            break;
        }

        // limit number of libs
        if(num_libs >= MAX_LIBS) {
            printf("libtrace:cfg: too many libs!\n");
            break;
        }
        num_libs++;

        // alloc new lib
        lib_entry_t *new_lib = (lib_entry_t *)malloc(sizeof(lib_entry_t));
        if(new_lib == NULL) {
            printf("libtrace:cfg: no mem!\n");
            break;
        }
        memset(new_lib, 0, sizeof(lib_entry_t));

        new_lib->lib_name = strdup(line);
        new_lib->next = lib;

        // read fd name
        n = read_line(line, 69, fh);
        if(n<0) {
            printf("libtrace:cfg: no fd name!\n");
            break;
        }
        if(n<2) {
            new_lib->fd_name = NULL;
        } else {
            new_lib->fd_name = strdup(line);
        }

        // read lvos
        new_lib->lvo_names = NULL;
        while(1) {
            int n = read_line(line, 79, fh);
            if(n<0) {
                printf("libtrace:cfg: no lvo name!\n");
                break;
            }
            // parse lvo flags
            char *ptr = line;
            int flags = 0;
            while(n>0) {
                char c = ptr[0];
                if(c == '!') {
                    flags |= FUNC_FLAG_INVERT;
                }
                else if(c == '<') {
                    flags |= FUNC_FLAG_PRE_DEBUG;
                }
                else if(c == '>') {
                    flags |= FUNC_FLAG_POST_DEBUG;
                }
                else if(c == '?') {
                    flags |= FUNC_FLAG_VOID;
                }
                else if(c == '+') {
                    flags |= FUNC_FLAG_EXTRA;
                }
                else if(c == '-') {
                    /* simply ignore negative offset */
                }
                else {
                    /* unknown flag -> stop */
                    break;
                }
                ptr++;
                n--;
            }
            char *name = NULL;
            if(n==1) {
                // all lvos?
                if (*ptr == '*') {
                    flags |= FUNC_FLAG_ALL;
                }
                // end of lvo?
                else if (*ptr == '#') {
                    break;
                }
                else {
                    name = strdup(ptr);
                }
            }
            else if(n==0) {
                break;
            }
            else {
                name = strdup(ptr);
            }
            // alloc lvo
            lvo_entry_t *lvo = (lvo_entry_t *)malloc(sizeof(lvo_entry_t));
            if(lvo == NULL) {
                printf("libtrace:cfg: no lvo mem!\n");
                break;
            }
            lvo->name = name;
            lvo->flags = flags;
            lvo->next = new_lib->lvo_names;
            new_lib->lvo_names = lvo;
        }
        lib = new_lib;
    }

    fclose(fh);
    return lib;
}

static void setup_lib(lib_entry_t *lib, int offset)
{
    printf("libtrace: setup '%s'\n", lib->lib_name);

    // set trap code for this library
    lib->trap_code = lib_traps[offset];

    // read fd file?
    if(lib->fd_name != NULL) {
        printf("libtrace: load fd from '%s': ", lib->fd_name);
        const char *err_str = NULL;
        lib->lib_info = libinfo_read_fd(lib->fd_name, &err_str);
        if(lib->lib_info == NULL) {
            printf("FAILED: %s\n", err_str);
        } else {
            printf("ok.\n");
        }
    }

    // set a lib tag if the lib is known (for quicker access)
    if(strcmp(lib->lib_name,"exec.library")==0) {
        lib->tag = LIB_TAG_EXEC;
    }
    else if(strcmp(lib->lib_name,"dos.library")==0) {
        lib->tag = LIB_TAG_DOS;
    }
    else {
        lib->tag = LIB_TAG_NONE;
    }

    // get short name
    const char *ptr = strchr(lib->lib_name, '.');
    if(ptr != NULL) {
        int short_len = ptr - lib->lib_name;
        char *name = (char *)malloc(short_len+1);
        strncpy(name, lib->lib_name, short_len);
        name[short_len] = '\0';
        lib->short_name = name;
    } else {
        lib->short_name = strdup(lib->lib_name);
    }
}

static void setup_libs(lib_entry_t *lib)
{
    int offset = 0;
    while(lib != NULL) {
        setup_lib(lib, offset);
        offset++;
        lib = lib->next;
    }
}

static void free_lib(lib_entry_t *lib)
{
    printf("libtrace: free '%s'\n", lib->lib_name);

    // free fd file?
    if(lib->lib_info != NULL) {
        libinfo_free(lib->lib_info);
        lib->lib_info = NULL;
    }

    // free lvo config
    lvo_entry_t *lvo = lib->lvo_names;
    while(lvo != NULL) {
        lvo_entry_t *next = lvo->next;
        free(lvo);
        lvo = next;
    }

    // free dynmic part
    deactivate_libtrace(lib, 0);

    // free static part
    free(lib->short_name);
    free((void *)lib->lib_name);
    free((void *)lib->fd_name);

    // finally free lib itself
    free(lib);
}

static void free_libs(void)
{
    // free libs
    lib_entry_t *lib = libs;
    while(lib != NULL) {
        lib_entry_t *next_lib = lib->next;
        free_lib(lib);
        lib = next_lib;
    }
    libs = NULL;
}

// ---------- STARTUP -------------------------------------------------------

/* first stage init: once at program startup */
void libtrace_install (void)
{
    printf("libtrace: install\n");

    // setup initial config
    cfg_file = strdup("libtrace.cfg");
    reload = 1;

    // setup init trap to setup patches
    init_addr = here ();
    calltrap(deftrap2 (trap_init, TRAPFLAG_EXTRA_STACK, _T("libtrace_init")));
    dw(RTS);

    // patch SetFunction
    setfunc_addr = here ();
    calltrap (deftrap2 (trap_setfunc, TRAPFLAG_EXTRA_STACK, _T("libtrace_setfunc")));
    dw(RTS);

    // share trap for OldOpenLibrary/OpenLibrary
    unsigned int open_lib_trap = deftrap2 (trap_openlib, TRAPFLAG_EXTRA_STACK, _T("libtrace_openlib"));

    // patch OldOpenLibrary
    oldopenlib_addr = here ();
    dw(JSR);
    dl(0); // patch original call here
    calltrap(open_lib_trap);
    dw(RTS);

    // patch OpenLibrary
    openlib_addr = here ();
    dw(JSR);
    dl(0); // patch original call here
    calltrap(open_lib_trap);
    dw(RTS);

    // pre-allocate call_pre traps for libs
    for(int i=0;i<MAX_LIBS;i++) {
        // setup a unique trap for this lib
        lib_traps[i] = 0xa000 | deftrap2(trap_trace_call_pre,
                                         TRAPFLAG_DORET | TRAPFLAG_NO_RETVAL,
                                         _T("libtrace_trace_call_pre"));

        // dummy code to move here() to get new traps...
        dw(RTS);
    }

    // allocate one call_post trap
    log_post_addr = here();
    calltrap (deftrap2 (trap_trace_call_post,
                        TRAPFLAG_DORET | TRAPFLAG_NO_RETVAL,
                        _T("libtrace_trace_call_post")));

    // dummy code to move here() to get new traps...
    dw(RTS);
}

/* second stage: every Amiga reset */
uaecptr libtrace_startup (uaecptr resaddr)
{
    printf("libtrace: startup\n");

    /* free old lib instances (if any) */
    deactivate_libtraces(0);

    // reload config
    if(reload) {
        reload = 0;

        // free old libs (if any)
        free_libs();

        // read new config
        if(cfg_file != NULL) {
            libs = read_config(cfg_file);
            if(libs != NULL) {
                setup_libs(libs);
                // auto activate
                enable = 1;
            }
        }
    }

    /* is trace enabled? */
    if(!enable || (libs == NULL)) {
        printf("libtrace: disabled\n");
        traps_everywhere = 0;
        return resaddr;
    }

    // enable traps everywhere in CPU emulation
    printf("libtrace: enabled\n");
    traps_everywhere = 1;

    // open new file?
    if((out_fh == stdout) && (out_file != NULL)) {
        printf("libtrace: log to file '%s'\n", out_file);
        out_fh = fopen(out_file, "w");
    } else {
        out_fh = stdout;
    }

    /* setup a fake resident resident */
    put_word (resaddr + 0x0, 0x4AFC);
    put_long (resaddr + 0x2, resaddr);
    put_long (resaddr + 0x6, resaddr + 0x1A); /* Continue scan here */
    put_word (resaddr + 0xA, 0x0101); /* RTF_COLDSTART; Version 1 */
    put_word (resaddr + 0xC, 0x0078); /* NT_UNKNOWN; pri 05 */
    put_long (resaddr + 0xE, 0); /* no name */
    put_long (resaddr + 0x12, 0); /* no id */
    put_long (resaddr + 0x16, init_addr);
    resaddr += 0x1A;

    return resaddr;
}

/* on program shutdown */
void libtrace_cleanup(void)
{
    printf("libtrace: cleanup\n");

    free_libs();

    // free task name (if any)
    if(task_name != NULL) {
        free((void *)task_name);
        task_name = NULL;
    }

    // free config file name
    if(cfg_file != NULL) {
        free((void *)cfg_file);
        cfg_file = NULL;
    }

    // free outfile
    if(out_file != NULL) {
        free((void *)out_file);
        out_file = NULL;
    }

    // close log file
    if(out_fh != stdout) {
        fclose(out_fh);
        out_fh = stdout;
    }
}

// --------- TRACE OUTPUT ---------------------------------------------------

static void gen_arg_str(call_info_t *ci, char *buf, int max)
{
    func_info_t *fi = ci->func->info;
    int pos = 0;
    int len = max;
    arg_info_t *arg = fi->args;
    for(int i=0;i<fi->num_args;i++) {
        // sep
        if(i>0) {
            if(len < 2) {
                break;
            }
            *buf++ = ',';
            *buf++ = ' ';
            len -= 2;
        }

        // name
        int n = strlen(arg->name);
        if(n > len) {
            break;
        }
        strcpy(buf, arg->name);
        buf += n;
        len -= n;

        // register value
        int reg = arg->reg;
        uae_u32 val = ci->regs[reg];

        // reg name
        if(len < 5) {
            break;
        }
        *buf++ = '[';
        if(reg > 7) {
            *buf++ = 'a';
            reg -= 8;
        } else {
            *buf++ = 'd';
        }
        sprintf(buf, "%d", reg);
        buf += 1;
        *buf++ = ']';
        *buf++ = '=';
        len -= 5;

        // value
        if(len < 8) {
            break;
        }
        sprintf(buf, "%08x", val);
        buf += 8;
        len -= 8;

        // next arg
        arg++;
    }
    *buf = '\0';
}

static void show_dos_pkt(const char *prefix, uae_u32 ptr)
{
    uae_u32 dp_Type = get_long(ptr+8);
    uae_u32 dp_Res1 = get_long(ptr+12);
    uae_u32 dp_Res2 = get_long(ptr+16);
    uae_u32 dp_Arg1 = get_long(ptr+20);
    uae_u32 dp_Arg2 = get_long(ptr+24);
    uae_u32 dp_Arg3 = get_long(ptr+28);
    uae_u32 dp_Arg4 = get_long(ptr+32);
    fprintf(out_fh, "%s    [DosPkt: Type=%08x  Args=%08x,%08x,%08x,%08x  Res=%08x,%08x]\n",
            prefix, dp_Type, dp_Arg1, dp_Arg2, dp_Arg3, dp_Arg4, dp_Res1, dp_Res2);
}

static void handle_extra(const char *prefix, call_info_t *ci, int pre)
{
    if(ci->lib->tag == LIB_TAG_DOS) {
        /* WaitPkt() -> d0 */
        if((ci->lvo == 252) && !pre) {
            show_dos_pkt(prefix, ci->return_regs[0]);
        }
    }
}

static void handle_call(call_info_t *ci, int pre)
{
    /* get func name or lvo */
    char buf[8];
    const char *fname = buf;
    if(ci->func->info != NULL) {
        fname = ci->func->info->name;
    } else {
        snprintf(buf, 7, "-%03d", ci->lvo);
    }

    /* setup prefix string */
    char prefix[80];
    snprintf(prefix, 79, "@%08x  %08x:%-16s  %-10s  ",
             ci->callee_pc, ci->this_task, ci->task_name, ci->lib->short_name);

    if(pre) {
        /* get arg string */
        char arg_buf[256];
        const char *args = "?";
        if(ci->func->info != NULL) {
            gen_arg_str(ci, arg_buf, 255);
            args = arg_buf;
        }

        fprintf(out_fh, "%s  %s(%s)\n",
                prefix, fname, args);
    } else {
        /* post call: show return value */
        fprintf(out_fh, "%s  %s -> d0=%08x\n",
                prefix, fname, ci->return_regs[0]);
    }

    /* extra output? */
    if(ci->func->flags & FUNC_FLAG_EXTRA) {
        handle_extra(prefix, ci, pre);
    }
}

// ---------- TRAPS ---------------------------------------------------------

/* third stage: resident is run with InitCode from autoconf of UAE board */
static uae_u32 REGPARAM2 trap_init(TrapContext *ctx)
{
    /* patch exec funcs */
    exec_base = get_long(4);
    printf("libtrace: init exec_base=%08x\n", exec_base);

    /* patch OldOpenLibrary */
    old_oldopenlib_addr = patch_SetFunction(ctx, exec_base, -408, openlib_addr);

    /* store old address */
    uae_u32 save_org = here();
    org(oldopenlib_addr+2);
    dl(old_oldopenlib_addr);
    org(save_org);

    /* patch OpenLibrary */
    old_openlib_addr = patch_SetFunction(ctx, exec_base, -552, openlib_addr);

    /* store old address */
    save_org = here();
    org(openlib_addr+2);
    dl(old_openlib_addr);
    org(save_org);

    /* finally patch SetFunction itself */
    old_setfunc_addr = patch_SetFunction(ctx, exec_base, -420, setfunc_addr);

    /* walk through initial lib list */
    uaecptr list = exec_base + 378;
    uaecptr lib_base = get_long (list);
    while (get_long(lib_base)) {
        // get library name
        char *lib_name = (char*)get_real_address(get_long(lib_base + 10));
        // do we have the lib?
        lib_entry_t *lib = find_lib_by_name(lib_name);
        if(lib != NULL) {
            // setup lib for tracing
            activate_libtrace(ctx, lib, lib_base);
        }

        lib_base = get_long(lib_base);
    }

    printf("libtrace: init done\n");
    return 0;
}

/* SetFunction */
static uae_u32 REGPARAM2 trap_setfunc(TrapContext *ctx)
{
    uae_u32 lib_base = m68k_areg(regs, 1);
    uae_u32 raw_lvo = m68k_areg(regs, 0);
    uae_u32 new_func = m68k_dreg(regs, 0);

    uae_s16 lvo = (uae_s16)(raw_lvo & 0xffff);

    uae_u32 lvo_addr = lib_base + lvo;
    uae_u16 opcode = get_word(lvo_addr);

#ifdef DEBUG_SETFUNC
    /* get callee info */
    uae_u32 this_task = get_long(exec_base + 276);
    uae_u32 sp = m68k_areg(regs, 7);
    uae_u32 callee_pc = get_long(sp);

    printf("libtrace: @%08x t=%08x: SetFunction lib_base=%08x, lvo=%d, opcode=%04x, new_func=%08x -> ",
           callee_pc, this_task, lib_base, lvo, opcode, new_func);
#endif

    /* is it a function we patched? */
    if((opcode & 0xf000) == 0xa000) {
        /* Simulate SetFunction but do not touch trap before address! */
        uae_u32 old_func = get_long(lvo_addr+2);
        pre_patch(ctx);
        put_long(lvo_addr+2, new_func);
        post_patch(ctx, lib_base);
#ifdef DEBUG_SETFUNC
        printf("patched. old_func=%08x\n", old_func);
#endif
        return old_func;
    }
    else {
        /* lib is not patched: use regular SetFunction call */
        uae_u32 old_func = CallFunc(ctx, old_setfunc_addr);
#ifdef DEBUG_SETFUNC
        printf("original. old_func=%08x\n", old_func);
#endif
        return old_func;
    }
}

/* called after OpenLibrary(a1=libName) -> d0=lib_base or NULL */
static uae_u32 REGPARAM2 trap_openlib(TrapContext *ctx)
{
    /* get lib_base */
    uae_u32 lib_base = m68k_dreg(regs, 0);
    if(lib_base == 0) {
        return 0;
    }

    // get library name
    char *lib_name = (char*)get_real_address(get_long(lib_base + 10));
    //printf("libtrace: openlib '%s'\n", lib_name);

    // do we have the lib patched already?
    lib_entry_t *lib = find_lib_by_name(lib_name);
    if(lib != NULL) {
        if(lib->lib_base == 0) {
            uae_u16 open_cnt = get_word(lib_base + 0x20);
            printf("libtrace: patching '%s' open_cnt=%d\n", lib_name, open_cnt);
            // setup lib for tracing
            activate_libtrace(ctx, lib, lib_base);
        }
    }

    return lib_base;
}

static void stack_push_addr(uae_u32 addr)
{
    uae_u32 sp = m68k_areg(regs, 7);
    sp -= 4;
    m68k_areg(regs, 7) = sp;
    put_long(sp, addr);
    //printf("@%08x: push addr %08x\n", sp, addr);
}

static void stack_push_pointer(void *ptr)
{
    uae_u32 sp = m68k_areg(regs, 7);
    sp -= sizeof(void *);
    m68k_areg(regs, 7) = sp;
    put_pointer(sp, ptr);
    //printf("@%08x: push %p\n", sp, ptr);
}

static void *stack_pop_pointer(void)
{
    uae_u32 sp = m68k_areg(regs, 7);
    void *ptr = get_pointer(sp);
    //printf("@%08x: pop  %p\n", sp, ptr);
    sp += sizeof(void *);
    m68k_areg(regs, 7) = sp;
    return ptr;
}

/* trap: trace call */
static uae_u32 REGPARAM2 trap_trace_call_pre(TrapContext *ctx)
{
    /* get trap code and func_ptr stored in jump table of lib */
    uae_u32 pc = m68k_getpc();
    uae_u16 trap_code = get_word(pc - 2);
    uae_u32 func_ptr = get_long(pc);

    /* find context: lib and func structure */
    lib_entry_t *lib = find_lib_by_trap_code(trap_code);
    if(lib == NULL) {
        printf("libtrace: log: FATAL LIB==NULL! pc=%08x, func_ptr=%08x\n", pc, func_ptr);
        stack_push_addr(func_ptr);
        return 0;
    }

    /* calc function offset */
    uae_s32 lvo = lib->lib_base - pc + 2;
    int func_id = (lvo - 6) / 6;
    if((func_id < 0) || (func_id >= lib->num_funcs)) {
        printf("libtrace: log: FATAL INVALID FUNC ID! pc=%08x, func_ptr=%08x, lvo=%d, id=%d\n",
               pc, func_ptr, lvo, func_id);
        stack_push_addr(func_ptr);
        return 0;
    }

    /* check original function pointer */
    func_entry_t *func = &lib->funcs[func_id];
    uae_u32 func_addr = func->func_addr;
    if(func_addr != pc) {
        printf("libtrace: log: FATAL: wrong func entry?\n");
        stack_push_addr(func_ptr);
        return 0;
    }

    /* get some caller info */
    uae_u32 this_task = get_long(exec_base + 276);
    uae_u32 sp = m68k_areg(regs, 7);
    uae_u32 callee_pc = get_long(sp);

    /* get task name (if any) */
    char *my_task_name = (char*)get_real_address(get_long(this_task + 10));

    /* filter by task name? */
    if((task_name != NULL) && (strcmp(task_name, my_task_name)!=0)) {
        stack_push_addr(func_ptr);
        return 0;
    }

    /* create call info object */
    call_info_t *ci = (call_info_t *)malloc(sizeof(call_info_t));
    if(ci == NULL) {
        printf("libtrace: log: FATAL: no memory!\n");
        stack_push_addr(func_ptr);
        return 0;
    }

    /* fill in call info */
    ci->lib = lib;
    ci->func = func;
    ci->lvo = lvo;
    ci->this_task = this_task;
    ci->callee_pc = callee_pc;
    ci->func_ptr = func_ptr;
    ci->task_name = (my_task_name != NULL) ? strdup(my_task_name) : "<None>";
    for(int i=0;i<8;i++) {
        ci->regs[i] = m68k_dreg(regs, i);
        ci->regs[i+8] = m68k_areg(regs, i);
    }

    /* report call before actual calling the function (pre) */
    handle_call(ci, 1);

    /* do we need to record return values? */
    int trace_return = (func->flags & FUNC_FLAG_VOID) == 0;
    if(trace_return) {
        /* push native call info pointer on m68k stack to get it in log_post call */
        stack_push_pointer(ci);
        /* push log post address to be called after function returns with RTS */
        stack_push_addr(log_post_addr);
    } else {
        /* no post call -> free call info now */
        free((void *)ci->task_name);
        free(ci);
    }

    /* push func address on stack so the implicit RTS of trap will call it */
    stack_push_addr(func_ptr);

    /* pre debug? */
    if(func->flags & FUNC_FLAG_PRE_DEBUG) {
        debug();
    }

    /* return value is ignored */
    return 0;
}

/* trap: called after a library call if return values are requested */
static uae_u32 REGPARAM2 trap_trace_call_post(TrapContext *ctx)
{
    /* retrieve call info from m68k stack */
    call_info_t *ci = (call_info_t *)stack_pop_pointer();

    if(ci != NULL) {
        /* get results in d0/d1 */
        for(int i=0;i<2;i++) {
            ci->return_regs[i] = m68k_dreg(regs, i);
        }

        /* report call after library call (post) */
        handle_call(ci, 0);

        /* post debug? */
        if(ci->func->flags & FUNC_FLAG_POST_DEBUG) {
            debug();
        }

        /* free call info */
        free((void *)ci->task_name);
        free(ci);
    }

    /* return value is ignored */
    return 0;
}

// ----------- API ----------

void libtrace_set_cfg_file(const char *new_cfg_file)
{
    if(cfg_file != NULL) {
        free((void *)cfg_file);
        cfg_file = NULL;
    }
    if(new_cfg_file != NULL) {
        cfg_file = strdup(new_cfg_file);
    }
    reload = 1;
}

const char *libtrace_get_cfg_file(void)
{
    return cfg_file;
}

void libtrace_enable(int new_enable)
{
    enable = new_enable;
}

int  libtrace_is_enabled(void)
{
    return enable;
}

void libtrace_set_task_name(const char *name)
{
    if(task_name != NULL) {
        free((void *)task_name);
        task_name = NULL;
    }
    if(name != NULL) {
        task_name = strdup(name);
    }
}

const char *libtrace_get_task_name(void)
{
    return task_name;
}

void libtrace_set_output_file(const char *new_out_file)
{
    // first close open file
    if(out_fh != stdout) {
        fclose(out_fh);
        out_fh = stdout;
    }
    // free old
    if(out_file != NULL) {
        free((void *)out_file);
        out_file = NULL;
    }
    // set new
    if(new_out_file != NULL) {
        out_file = strdup(new_out_file);
    }
}

const char *libtrace_get_output_file(void)
{
    return out_file;
}
