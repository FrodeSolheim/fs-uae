/*
* UAE - The Un*x Amiga Emulator
*
* Debug Info (from Hunk Files)
*
* Written by Christian Vogelgsang <chris@vogelgsang.org>
*/

#include <stdio.h>
#include <stdlib.h>

#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"

#include "uae/debuginfo.h"

#define HUNK_HEADER     1011
#define HUNK_END        1010
#define HUNK_SYMBOL     1008
#define HUNK_DEBUG      1009
#define HUNK_CODE       1001
#define HUNK_DATA       1002
#define HUNK_BSS        1003
#define HUNK_ABSRELOC32 1004

static uae_u32 read_long(FILE *fh)
{
    uae_u8 buf[4];
    size_t num = fread(buf,4,1,fh);
    if(num == 1) {
        return ((uae_u32)buf[0] << 24) |
               ((uae_u32)buf[1] << 16) |
               ((uae_u32)buf[2] << 8) |
                (uae_u32)buf[3];
    } else {
        return 0;
    }
}

static int read_name_size(FILE *fh, char **ptr, uae_u32 num_longs)
{
    uae_u32 len = num_longs * 4;
    char *str = xmalloc(char,len+1);
    if(str == NULL) {
        return -2;
    }
    str[len] = '\0';
    size_t num = fread(str,len,1,fh);
    if(num == 1) {
        *ptr = str;
        return len;
    } else {
        xfree(str);
        return -3;
    }
}

static int read_name(FILE *fh, char **ptr)
{
    uae_u32 len = read_long(fh);
    if(ferror(fh)) {
        return -1;
    }
    if(len == 0) {
        return 0;
    }
    return read_name_size(fh,ptr,len);
}

/* hunk header:
   lib table size (0)
   first hunk
   last_hunk */
static int read_header(FILE *fh)
{
    uae_u32 resname_len = read_long(fh); /* resident lib names -> 0 on Amiga */
    if(resname_len != 0) {
        return 0;
    }
    read_long(fh); /* table size (incl. overlay hunks) */
    uae_u32 first_hunk = read_long(fh);
    uae_u32 last_hunk = read_long(fh);
    if(ferror(fh)) {
        return 0;
    }
    return last_hunk - first_hunk + 1;
}

static int compare_symbols(const void *a, const void *b)
{
    debug_symbol *da = (debug_symbol *)a;
    debug_symbol *db = (debug_symbol *)b;
    return da->offset - db->offset;
}

static int parse_symbols(FILE *fh, debug_segment *segment)
{
    /* first parse to find out number of symbols */
    long pos = ftell(fh);
    int num_symbols = 0;

    /* parse symbol table */
    while(!feof(fh)) {
        /* read name and offset */
        char *name;
        int len = read_name(fh,&name);
        if(len < 0) {
            return len;
        } else if(len == 0) {
            break;
        }
        /*uae_u32 offset =*/ read_long(fh);
        num_symbols ++;
    }

    /* alloc symbols */
    debug_symbol *symbols = xmalloc(debug_symbol, num_symbols);
    if(symbols == NULL) {
        return -1;
    }

    /* now read symbols */
    fseek(fh, pos, SEEK_SET);
    debug_symbol *s = symbols;
    while(!feof(fh)) {
        /* read name and offset */
        char *name;
        int len = read_name(fh,&name);
        if(len < 0) {
            xfree(symbols);
            return len;
        } else if(len == 0) {
            break;
        }
        uae_u32 offset = read_long(fh);

        s->name = name;
        s->offset = offset;
        s++;
    }

    /* sort symbols */
    qsort(symbols, num_symbols, sizeof(debug_symbol), compare_symbols);

    segment->symbols = symbols;
    segment->num_symbols = num_symbols;
    return num_symbols;
}

#define DEBUG_LINE 0x4c494e45

static int parse_debug(FILE *fh, debug_segment *segment)
{
    uae_u32 num_longs = read_long(fh);
    uae_u32 base_offset = read_long(fh);
    uae_u32 debug_tag = read_long(fh);
    num_longs -= 2; /* skip base offset and debug tag */

    if(debug_tag == DEBUG_LINE) {
        uae_u32 num_name_longs = read_long(fh);
        if(num_name_longs > 0) {
            char *src_name;
            int len = read_name_size(fh, &src_name, num_name_longs);
            if(len > 0) {
                /* alloc debug src file */
                debug_src_file *sf = xmalloc(debug_src_file, 1);
                if(sf == NULL) {
                    xfree(src_name);
                    return -1;
                }
                sf->src_file = src_name;

                uae_u32 num_lines = (num_longs - num_name_longs - 1) / 2;

                /* alloc line info */
                debug_src_line *lines = xmalloc(debug_src_line, num_lines);
                if(lines == NULL) {
                    xfree(src_name);
                    xfree(sf);
                    return -2;
                }
                sf->num_lines = num_lines;
                sf->lines = lines;
                sf->base_offset = base_offset;

                /* read line info */
                debug_src_line *sl = lines;
                for(int i = 0;i<num_lines;i++) {
                    uae_u32 line_no = read_long(fh);
                    uae_u32 offset = read_long(fh);
                    /* SAS/C seems to use highest byte for extra info -> mask out */
                    line_no &= 0xffffff;
                    sl->line = line_no;
                    sl->offset = base_offset + offset;
                    sl++;
                }

                /* add source file to segment */
                sf->next = segment->src_files;
                segment->src_files = sf;
                segment->num_src_files++;
            }
        }
    } else {
        /* skip unknown debug */
        uae_u32 num_bytes = num_longs * 4;
        fseek(fh, num_bytes, SEEK_CUR);
    }
    return 0;
}

static int parse_hunk(FILE *fh, uae_u32 hunk_type, debug_segment *s)
{
    /* mask out pot. mem flags */
    hunk_type = hunk_type & 0x3fffffff;
    switch(hunk_type) {
        case HUNK_SYMBOL:
            return parse_symbols(fh, s);
        case HUNK_DEBUG:
            return parse_debug(fh, s);
        /* skip BSS */
        case HUNK_BSS:
            read_long(fh); /* skip BSS size */
            s->seg_type = hunk_type;
            break;
        /* CODE or DATA */
        case HUNK_CODE:
        case HUNK_DATA:
            {
                uae_u32 num_longs = read_long(fh);
                uae_u32 num_bytes = num_longs * 4;
                fseek(fh, num_bytes, SEEK_CUR);
                s->seg_type = hunk_type;
            }
            break;
        /* RELOC32 */
        case HUNK_ABSRELOC32:
            {
                while(!feof(fh)) {
                    uae_u32 num_entries = read_long(fh);
                    if(num_entries == 0) {
                        break;
                    }
                    /*uae_u32 to_seg =*/ read_long(fh);
                    fseek(fh, num_entries*4, SEEK_CUR);
                }
            }
            break;
        /* other hunk */
        default:
            /* skip unknown hunk */
            uae_u32 num_longs = read_long(fh);
            uae_u32 num_bytes = num_longs * 4;
            fseek(fh, num_bytes, SEEK_CUR);
            break;
    }
    return ferror(fh);
}

debug_file *debug_info_load_hunks(const char *file_name)
{
    /* open hunk file */
    FILE *fh = fopen(file_name, "rb");
    if(fh == NULL) {
        return NULL;
    }

    /* expect HUNK_HEADER */
    uae_u32 id = read_long(fh);
    if(id != HUNK_HEADER) {
        puts("no header!");
        fclose(fh);
        return NULL;
    }
    int num_segs = read_header(fh);
    if(num_segs <= 0) {
        puts("wrong header!");
        fclose(fh);
        return NULL;
    }

    /* alloc and fill debug info */
    debug_file *file = xmalloc(debug_file, 1);
    if(file == NULL) {
        fclose(fh);
        return NULL;
    }
    debug_segment *segs = xmalloc(debug_segment, num_segs);
    file->num_segments = num_segs;
    file->segments = segs;
    file->file_name = my_strdup(file_name);

    /* read sizes for each segment from header */
    debug_segment *s = segs;
    for(int i=0;i<num_segs;i++) {
        uae_u32 size = read_long(fh);
        s->size = size * 4; /* in bytes */
        s->num_symbols = 0;
        s->symbols = NULL;
        s->num_src_files = 0;
        s->src_files = NULL;
        s++;
    }

    /* main loop */
    s = segs;
    int num = 0;
    while(!feof(fh)) {
        uae_u32 hunk_type = read_long(fh);
        /* end of current hunk */
        if(hunk_type == HUNK_END) {
            s++;
            num++;
            /* should never happen: */
            if(num >= num_segs) {
                break;
            }
        }
        /* non end block */
        else {
            int ok = parse_hunk(fh, hunk_type, s);
            /* error occurred */
            if(ok < 0) {
                debug_info_free_file(file);
                file = NULL;
                break;
            }
        }
    }

    fclose(fh);
    return file;
}

static void free_segment(debug_segment *segment)
{
    /* free symbols */
    debug_symbol *symbol = segment->symbols;
    for(int i=0;i<segment->num_symbols;i++) {

        xfree(symbol->name);

        symbol++;
    }
    xfree(segment->symbols);

    /* free src files */
    debug_src_file *file = segment->src_files;
    for(int i=0;i<segment->num_src_files;i++) {
        debug_src_file *next = file->next;

        xfree(file->src_file);
        xfree(file->lines);

        xfree(file);
        file = next;
    }
}

void debug_info_free_file(debug_file *file)
{
    /* free segments */
    if(file->segments != NULL) {
        debug_segment *s = file->segments;
        for(int i=0;i<file->num_segments;i++) {
            free_segment(s);
            s++;
        }
        xfree(file->segments);
    }

    if(file->file_name != NULL) {
        xfree(file->file_name);
    }
    xfree(file);
}

static const char *get_seg_type_str(uae_u32 seg_type)
{
    switch(seg_type) {
        case HUNK_CODE:
            return "CODE";
        case HUNK_DATA:
            return "DATA";
        case HUNK_BSS:
            return "BSS ";
        default:
            return "????";
    }
}

void debug_info_dump_file(debug_file *file)
{
    printf("file '%s': %d segments\n", file->file_name, file->num_segments);
    for(int i=0;i<file->num_segments;i++) {
        /* show segment */
        debug_segment *s = &file->segments[i];
        printf("  segment #%02d: %s [%08x]  %3d symbols, %3d src files\n",
            i, get_seg_type_str(s->seg_type), s->size, s->num_symbols, s->num_src_files);
        /* show symbols in segment */
        debug_symbol *sym = s->symbols;
        for(int j=0;j<s->num_symbols;j++) {
            printf("    %08x  %s\n", sym->offset, sym->name);
            sym++;
        }
        /* show source files */
        debug_src_file *sf = s->src_files;
        for(int j=0;j<s->num_src_files;j++) {
            printf("    %s: #%d\n", sf->src_file, sf->num_lines);

#if 0
            debug_src_line *sl = sf->lines;
            for(int k=0;k<sf->num_lines;k++) {
                printf("      @%08x: %5d/%x\n", sl->offset, sl->line, sl->line);
                sl++;
            }
#endif

            sf = sf->next;
        }
    }
}

int debug_info_find_symbol(const debug_segment *seg, uae_u32 offset,
                           debug_symbol **ret_symbol, uae_u32 *ret_reloff)
{
    *ret_symbol = NULL;
    *ret_reloff = 0;

    /* outside of segment */
    if((offset < 0) || (offset >= seg->size)) {
        return -1;
    }

    /* no symbols found */
    if(seg->symbols == NULL) {
        return 0;
    }

    /* search for symbol that preceeds the given offset */
    debug_symbol *sym = seg->symbols;
    debug_symbol *last_sym = NULL;
    for(int i=0;i<seg->num_symbols;i++) {
        if(sym->offset > offset) {
            if(last_sym != NULL) {
                break;
            } else {
                return 0;
            }
        }
        last_sym = sym;
        sym++;
    }
    /* offset is beyond last symbol */
    *ret_symbol = last_sym;
    *ret_reloff = offset - last_sym->offset;
    return 1;
}

int debug_info_find_src_line(const debug_segment *seg, uae_u32 offset,
                             debug_src_file **ret_file,
                             debug_src_line **ret_line, uae_u32 *ret_reloff)
{
    *ret_file = NULL;
    *ret_line = NULL;
    *ret_reloff = 0;

    /* outside of segment */
    if((offset < 0) || (offset >= seg->size)) {
        return -1;
    }

    /* no src info found */
    if(seg->src_files == NULL) {
        return 0;
    }

    /* search for src_file that contains offset */
    debug_src_file *file = seg->src_files;
    while(file != NULL) {
        debug_src_file *next = file->next;
        if(next != NULL) {
            if(offset < next->base_offset) {
                break;
            }
        } else {
            break;
        }
        file = next;
    }
    if(file == NULL) {
        return 0;
    }

    /* search in src_file for line */
    debug_src_line *line = file->lines;
    debug_src_line *last_line = NULL;
    for(int i=0;i<file->num_lines;i++) {
        if(line->offset > offset) {
            if(last_line != NULL) {
                break;
            } else {
                return 0;
            }
        }
        last_line = line;
        line++;
    }
    /* offset beyond last line info */
    *ret_file = file;
    *ret_line = last_line;
    *ret_reloff = offset - last_line->offset;
    return 1;
}
