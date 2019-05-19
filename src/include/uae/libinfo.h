typedef struct {
    char *name;
    int reg;
} arg_info_t;

typedef struct func_info {
    struct func_info  *next;
    char *name;
    int   lvo;
    int   num_args;
    arg_info_t *args;
} func_info_t;

typedef struct {
    char *name;
    int   num_funcs;
    int   max_lvo;
    func_info_t *funcs;
} lib_info_t;

typedef const char ** str_p;

extern lib_info_t *libinfo_read_fd(const char *fd_file_name, str_p err_str);
extern void libinfo_dump(lib_info_t *li);
extern void libinfo_free(lib_info_t *li);
extern func_info_t *libinfo_find_func(lib_info_t *li, const char *name);
