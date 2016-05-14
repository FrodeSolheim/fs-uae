/*
* UAE - The Un*x Amiga Emulator
*
* Segment Tracker
*
* Written by Christian Vogelgsang <chris@vogelgsang.org>
*/

#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"

#include "uae/memory.h"
#include "autoconf.h"
#include "traps.h"
#include "uae/patch.h"
#include "newcpu.h"
#include "uae/debuginfo.h"
#include "uae/segtracker.h"

static uae_u32 init_addr;
static uae_u32 dos_base;

static patch_func pf_OpenLibrary;
static patch_func pf_LoadSeg;
static patch_func pf_UnLoadSeg;
static patch_func pf_NewLoadSeg;

#ifdef _WIN32
#define strcasestr strstr
#warning Needs strcasestr replacement
#endif

int segtracker_enabled = 0;

/* patch the interesting DOS functions.
   Since we call exec's SetFunction to perform the patch it will work on
   V36 and pre too as there the patch is not performed. */
static void patch_dos(TrapContext *ctx)
{
    uae_u16 ver = get_word(dos_base + 20);
    write_log("segtracker: dos V%d -> %s\n", ver, (ver <= 36) ? "can't patch":"patched");

    patch_func_set(ctx, dos_base, -150, &pf_LoadSeg);
    patch_func_set(ctx, dos_base, -156, &pf_UnLoadSeg);
    patch_func_set(ctx, dos_base, -768, &pf_NewLoadSeg);
}

/* patched EXEC: OpenLibrary call */
static uae_u32 REGPARAM2 OpenLibrary(TrapContext *ctx)
{
    uaecptr name_addr = m68k_areg (regs, 1); // a1 = lib name
    uaecptr lib_base = m68k_dreg( regs, 0 ); // d0 = lib base
    uae_u8 *name = get_real_address(name_addr);

    /* here comes a hack to patch dos.library calls:
       wait for the first one opening dos.library and then
       patch my functions... */
    if(dos_base == 0) {
        if(strcmp((const char *)name, "dos.library")==0) {
            dos_base = lib_base;
            patch_dos(ctx);
        }
    }

    return 0;
}

/* global list of all seglists */
seglist_pool segtracker_pool;

/* dump the segment list */
void segtracker_dump(const char *match)
{
    int sl_num = 0;
    seglist *sl = segtracker_pool.first;
    while(sl != NULL) {
        if( (match == NULL) || (strcasestr(sl->name, match)!=NULL) ) {
            printf("'%s' @%08x\n", sl->name, sl->addr);
            segment *s = sl->segments;
            int num = 0;
            while(s->addr != 0) {
                printf("  #%02d [%08x,%08x,%08x]", num,
                       s->addr, s->size, s->addr + s->size);

                /* show attached debug info */
                debug_segment *ds = s->debug;
                if(ds != NULL) {
                    printf("  %3d symbols,  %3d src files\n",
                           ds->num_symbols, ds->num_src_files);

                    /* dump symbols */
                    if(match != NULL) {
                        debug_symbol *symbol = ds->symbols;
                        for(int i=0;i<ds->num_symbols;i++) {
                            uae_u32 addr = s->addr + symbol->offset;
                            printf("    %08x  %s\n", addr, symbol->name);
                            symbol++;
                        }
                    }
                } else {
                    printf("\n");
                }

                s++;
                num++;
            }
            sl_num++;
        }
        sl = sl->next;
    }
    printf("found %d seglists.\n", sl_num);
}

/* search a segment by address */
int segtracker_search_address(uae_u32 addr, seglist **found_sl, int *num_seg)
{
    seglist *sl = segtracker_pool.first;
    while(sl != NULL) {
        int num = 0;
        segment *s = sl->segments;
        while(s->addr != 0) {

            uae_u32 end = s->addr + s->size;
            if((addr >= s->addr) && (addr < end)) {
                *found_sl = sl;
                *num_seg = num;
                return 1;
            }

            s++;
            num++;
        }
        sl = sl->next;
    }
    return 0;
}

static void add_seglist(const char *name, uae_u32 seglist_addr)
{
    if(seglist_addr == 0) {
        return;
    }

    /* first count the number of segments */
    int num_segs = 0;
    uae_u32 ptr = seglist_addr;
    while(ptr != 0) {
        ptr = get_long(ptr) << 2;
        num_segs ++;
    }

    /* alloc seglist */
    seglist *sl = xmalloc(seglist, 1);
    if(sl == NULL) {
        write_log("segtracker: NO seglist MEMORY!\n");
        return;
    }
    segment *segs = xmalloc(segment, num_segs + 1); // add an empty segment
    if(segs == NULL) {
        write_log("segtracker: NO segments MEMORY!\n");
        return;
    }

    /* fill seglist */
    sl->name = my_strdup(name);
    sl->addr = seglist_addr;
    sl->num_segments = num_segs;
    sl->segments = segs;
    sl->next = NULL;
    sl->prev = NULL;
    sl->debug = NULL;

    /* fill segments */
    ptr = seglist_addr;
    segment *s = segs;
    while(ptr != 0) {
        s->size = get_long(ptr - 4); // size of BPTR + segment
        s->size -= 8; // correct size
        s->addr = ptr + 4;
        s->debug = NULL;
        ptr = get_long(ptr) << 2; // BPTR to next segment
        s++;
    }
    /* last segment is zero */
    s->size = 0;
    s->addr = 0;

    /* link seglist */
    if(segtracker_pool.first == NULL) {
        segtracker_pool.first = sl;
        segtracker_pool.last = sl;
    } else {
        sl->prev = segtracker_pool.last;
        segtracker_pool.last->next = sl;
        segtracker_pool.last = sl;
    }
    segtracker_pool.num_seglists ++;
}

static void del_seglist(seglist *sl)
{
    if(sl->next != NULL) {
        sl->next->prev = sl->prev;
    }
    if(sl->prev != NULL) {
        sl->prev->next = sl->next;
    }
    if(sl == segtracker_pool.first) {
        segtracker_pool.first = sl->next;
    }
    if(sl == segtracker_pool.last) {
        segtracker_pool.last = sl->prev;
    }
    segtracker_pool.num_seglists --;

    if(sl->name != NULL) {
        xfree(sl->name);
    }
    if(sl->segments != NULL) {
        xfree(sl->segments);
    }

    if(sl->debug != NULL) {
        debug_info_free_file(sl->debug);
    }

    xfree(sl);
}

static void del_all_seglists(void)
{
    seglist *sl = segtracker_pool.first;
    while(sl != NULL) {
        seglist *next_sl = sl->next;
        del_seglist(sl);
        sl = next_sl;
    }
    segtracker_pool.num_seglists = 0;
}

static void rem_seglist(uae_u32 seglist_addr)
{
    /* similar to original SegTracker support partial segment unloading */
    uae_u32 ptr = seglist_addr;
    while(ptr != 0) {
        /* find segment in seglist */
        seglist *sl = segtracker_pool.first;
        int found = 0;
        while(sl != NULL) {
            /* check all segments */
            segment *s = sl->segments;
            int any_left = 0;
            while(s->addr != 0) {
                /* found segment -> set its size to 0 */
                if(s->addr == (ptr+4)) {
                    s->size = 0;
                    found = 1;
                }
                if(s->size != 0) {
                    any_left ++;
                }
                s++;
            }

            /* delete seglist */
            seglist *sl_next = sl->next;
            if(any_left == 0) {
                del_seglist(sl);
            }

            if(found) {
                break;
            }

            sl = sl_next;
        }

        /* segment not found?! */
        if(!found) {
            write_log("segtracker: segment %08x NOT found!\n", ptr);
        }

        /* next segment of input seglist */
        ptr = get_long(ptr) << 2;
    }
}

/* remove all tracked segment lists */
void segtracker_clear(void)
{
    del_all_seglists();
}

/* patched DOS: LoadSeg call */
static uae_u32 REGPARAM2 LoadSeg(TrapContext *ctx)
{
    uaecptr name_addr = m68k_areg(regs,0); // a0 = name (was d1)
    uaecptr seglist_baddr = m68k_dreg(regs,0); // d0 = seglist
    uaecptr seglist_addr = seglist_baddr << 2;

    const char *name = (char *)get_real_address(name_addr);
    //printf("LoadSeg(%s) -> %08x\n",name, seglist_addr);
    add_seglist(name, seglist_addr);

    return 0;
}

/* patched DOS: NewLoadSeg call */
static uae_u32 REGPARAM2 NewLoadSeg(TrapContext *ctx)
{
    uaecptr name_addr = m68k_areg(regs,0); // a0 = name (was d1)
    uaecptr seglist_baddr = m68k_dreg(regs,0); // d0 = seglist
    uaecptr seglist_addr = seglist_baddr << 2;

    const char *name = (char *)get_real_address(name_addr);
    //printf("NewLoadSeg(%s) -> %08x\n",name, seglist_addr);
    add_seglist(name, seglist_addr);

    return 0;
}

/* patched DOS: UnLoadSeg call */
static uae_u32 REGPARAM2 UnLoadSeg(TrapContext *ctx)
{
    uaecptr seglist_baddr = m68k_dreg(regs,1); // d1 = seglist
    uaecptr seglist_addr = seglist_baddr << 2;

    //printf("UnLoadSeg(%08x)\n", seglist_addr);
    rem_seglist(seglist_addr);

    return 0;
}

/* When the dummy resident is executed then call this function via trap.
   Our first patch is to track OpenLibrary calls so we can spot the
   dos.library */
static uae_u32 REGPARAM2 segtracker_init(TrapContext *ctx)
{
    /* patch exec funcs */
    uae_u32 exec_base = get_long(4);
    patch_func_set(ctx, exec_base, -552, &pf_OpenLibrary);
    write_log("segtracker: patched OpenLibrary\n");

    return 0;
}

/* main entry of SegmentTracker for each Amiga run: setting up AutoConf area...
   if tracker is enabled then install a dummy resident that does the
   function patching in its init function */
uaecptr segtracker_startup (uaecptr resaddr)
{
    /* reset dos patching and clear seglist */
    dos_base = 0;
    del_all_seglists();

    if(!segtracker_enabled) {
        return resaddr;
    }

    write_log("segtracker: startup\n");

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

/* initial entry of SegmentTracker: on first startup of UAE register some
   traps for the patched functions */
void segtracker_install (void)
{
    write_log("segtracker: install\n");

    // setup init trap to setup patches
    init_addr = here ();
    calltrap (deftrap2 (segtracker_init, TRAPFLAG_EXTRA_STACK, _T("segtracker_init")));
    dw (RTS);

    // exec
    patch_func_init_post_call(OpenLibrary, &pf_OpenLibrary,
                              TRAPFLAG_EXTRA_STACK, PATCH_SAVE_NONE);
    // dos
    patch_func_init_post_call(LoadSeg, &pf_LoadSeg, 0, PATCH_SAVE_D1_RESTORE_A0);
    patch_func_init_post_call(NewLoadSeg, &pf_NewLoadSeg, 0, PATCH_SAVE_D1_RESTORE_A0);
    patch_func_init_pre_call(UnLoadSeg, &pf_UnLoadSeg, 0);
}

seglist *segtracker_find_by_name(const char *name)
{
    seglist *sl = segtracker_pool.first;
    while(sl != NULL) {
        const char *sname = sl->name;
        if(strcasestr(sname, name)!=NULL) {
            return sl;
        }
        sl = sl->next;
    }
    return NULL;
}

int segtracker_add_debug_info(seglist *seglist, debug_file *file, const char **err)
{
    /* already has debug info? */
    if(seglist->debug != NULL) {
        if(err != NULL) {
            *err = "Already has debug info!";
        }
        return -3;
    }

    /* first validate if segments match in both seglist and file */
    if(seglist->num_segments != file->num_segments) {
        if(err != NULL) {
            *err = "Number of segments mismatch!";
        }
        return -1;
    }
    for(int i=0;i<seglist->num_segments;i++) {
        segment *s = &seglist->segments[i];
        debug_segment *ds = &file->segments[i];
        if(s->size != ds->size) {
            if(err != NULL) {
                *err = "Segment size mismatch!";
            }
            return -2;
        }
    }

    /* seems to be ok -> set infos */
    seglist->debug = file;
    for(int i=0;i<seglist->num_segments;i++) {
        segment *s = &seglist->segments[i];
        debug_segment *ds = &file->segments[i];
        s->debug = ds;
    }

    return 0;
}

int segtracker_find_symbol(const segment *seg, uae_u32 offset,
                           debug_symbol **ret_symbol, uae_u32 *ret_reloff)
{
    if(seg->debug != NULL) {
        return debug_info_find_symbol(seg->debug, offset, ret_symbol, ret_reloff);
    } else {
        return -1;
    }
}

int segtracker_find_src_line(const segment *seg, uae_u32 offset,
                             debug_src_file **ret_file,
                             debug_src_line **ret_line, uae_u32 *ret_reloff)
{
    if(seg->debug != NULL) {
        return debug_info_find_src_line(seg->debug, offset, ret_file, ret_line, ret_reloff);
    } else {
        return -1;
    }
}

void segtracker_dump_symbols(const char *name)
{
    int showed_seglist = 0;
    seglist *sl = segtracker_pool.first;
    while(sl != NULL) {
        /* has seglist debug info? */
        if(sl->debug != NULL) {
            segment *s = sl->segments;
            /* run through segments */
            for(int i=0;i<sl->num_segments;i++) {
                if(s->debug != NULL) {
                    debug_symbol *ds = s->debug->symbols;
                    int showed_segment = 0;
                    for(int j=0;j<s->debug->num_symbols;j++) {
                        /* does symbol name match? */
                        if(strcasestr(ds->name, name)!=NULL) {
                            if(!showed_seglist) {
                                showed_seglist = 1;
                                printf("'%s'\n", sl->name);
                            }
                            if(!showed_segment) {
                                showed_segment = 1;
                                printf("  #%02d\n", i);
                            }
                            uae_u32 addr = ds->offset + s->addr;
                            printf("    %08x:  %s\n", addr, ds->name);
                        }
                        ds++;
                    }
                }
                s++;
            }
        }
        showed_seglist = 0;
        sl = sl->next;
    }
}

void segtracker_dump_src_lines(const char *name, int line)
{
    int showed_seglist = 0;
    seglist *sl = segtracker_pool.first;
    while(sl != NULL) {
        /* has seglist debug info? */
        if(sl->debug != NULL) {
            segment *seg = sl->segments;
            /* run through segments */
            for(int i=0;i<sl->num_segments;i++) {
                debug_segment *ds = seg->debug;
                if(ds != NULL) {
                    int showed_segment = 0;

                    /* now look in src files */
                    debug_src_file *sf = ds->src_files;
                    while(sf != NULL) {
                        /* does src_file name match? */
                        if(strcasestr(sf->src_file, name)!=NULL) {
                            /* check line number */
                            debug_src_line *l = sf->lines;
                            for(int j=0;j<sf->num_lines;j++) {
                                /* found line */
                                if(l->line == line) {
                                    if(!showed_seglist) {
                                        showed_seglist = 1;
                                        printf("'%s'\n", sl->name);
                                    }
                                    if(!showed_segment) {
                                        showed_segment = 1;
                                        printf("  #%02d\n", i);
                                    }
                                    uae_u32 addr = l->offset + seg->addr;
                                    printf("    %08x:  %s:%d\n", addr, sf->src_file, l->line);
                                }
                                l++;
                            }
                        }
                        sf = sf->next;
                    }
                }
                seg++;
            }
        }
        showed_seglist = 0;
        sl = sl->next;
    }
}
