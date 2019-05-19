extern uaecptr libtrace_startup(uaecptr resaddr);
extern void libtrace_install(void);
extern void libtrace_cleanup(void);

extern void libtrace_set_cfg_file(const char *cfg_file);
extern const char *libtrace_get_cfg_file(void);
extern void libtrace_enable(int enable);
extern int  libtrace_is_enabled(void);
extern void libtrace_set_task_name(const char *task_name);
extern const char *libtrace_get_task_name(void);
extern void libtrace_set_output_file(const char *out_file);
extern const char *libtrace_get_output_file(void);
