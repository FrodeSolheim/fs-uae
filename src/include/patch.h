/* patch library helper */

/* define a function patch */
typedef struct {
    uae_u32 new_entry_addr;
    uae_u32 old_entry_jump;
    uae_u32 old_entry_addr;
} patch_func;

#define PATCH_SAVE_NONE             0
#define PATCH_SAVE_ALL_BUT_D0       1
#define PATCH_SAVE_D1_RESTORE_A0    2

/* similar to Exec's SetFunction */
extern void patch_func_init_pre_call(TrapHandler func, patch_func *pf, int flags);
extern void patch_func_init_post_call(TrapHandler func, patch_func *pf,
                                      int flags, int save);
extern void patch_func_set(TrapContext *ctx, uaecptr lib_base, uae_s16 func_lvo,
                           patch_func *pf);
