
typedef struct {
    uae_u32   addr;
    uae_u32   size;

    debug_segment *debug; /* optional debug info */
} segment;

struct seglist_s {
    char     *name;
    uae_u32   addr;
    int       num_segments;
    segment  *segments;

    debug_file *debug; /* optional debug info */

    struct seglist_s *prev, *next;
};
typedef struct seglist_s seglist;

typedef struct {
    seglist *first;
    seglist *last;
    int      num_seglists;
} seglist_pool;

extern seglist_pool segtracker_pool;

extern uaecptr segtracker_startup(uaecptr resaddr);
extern void segtracker_install(void);

extern void segtracker_dump(const char *match);
extern int segtracker_search_address(uae_u32 addr, seglist **found_sl, int *num_seg);
extern void segtracker_clear(void);

extern seglist *segtracker_find_by_name(const char *name);
extern int segtracker_add_debug_info(seglist *seglist, debug_file *file, const char **err);

extern int segtracker_find_symbol(const segment *seg, uae_u32 offset,
                                  debug_symbol **ret_symbol, uae_u32 *ret_reloff);
extern int segtracker_find_src_line(const segment *seg, uae_u32 offset,
                                    debug_src_file **ret_file,
                                    debug_src_line **ret_line, uae_u32 *ret_reloff);

extern void segtracker_dump_symbols(const char *name);
extern void segtracker_dump_src_lines(const char *name, int line);

extern int segtracker_enabled;
