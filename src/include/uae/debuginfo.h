/*
 * DebugInfo data types and functions
 */

/* entry for a symbol: name and offset in segment */
typedef struct {
    char *name;
    uae_u32 offset;
} debug_symbol;

/* entry for a source line entry: line number and offset in segment */
typedef struct {
    uae_u32 line;
    uae_u32 offset;
} debug_src_line;

/* entry for a source file */
typedef struct debug_src_file_s {
    struct debug_src_file_s *next;

    uae_u32 base_offset;
    char *src_file;
    debug_src_line *lines;
    int num_lines;
} debug_src_file;

/* entry for a segment */
typedef struct {
    int seg_type;
    uae_u32 size;

    int num_symbols;
    debug_symbol *symbols;

    int num_src_files;
    debug_src_file *src_files;
} debug_segment;

/* entry for the debug info of a (hunk) file */
typedef struct {
    char *file_name;
    int num_segments;
    debug_segment *segments;
} debug_file;

extern debug_file *debug_info_load_hunks(const char *file_name);
extern void debug_info_free_file(debug_file *file);
extern void debug_info_dump_file(debug_file *file);

extern int debug_info_find_symbol(const debug_segment *seg, uae_u32 offset,
                                  debug_symbol **ret_symbol, uae_u32 *ret_reloff);
extern int debug_info_find_src_line(const debug_segment *seg, uae_u32 offset,
                                    debug_src_file **ret_file,
                                    debug_src_line **ret_line, uae_u32 *ret_reloff);
