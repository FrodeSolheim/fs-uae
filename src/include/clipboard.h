#ifndef UAE_CLIPBOARD_H
#define UAE_CLIPBOARD_H

#include "uae/types.h"
#include "traps.h"

extern int amiga_clipboard_want_data(TrapContext *ctx);
extern void amiga_clipboard_got_data(TrapContext *ctx, uaecptr data, uae_u32 size, uae_u32 actual);
extern void amiga_clipboard_die(TrapContext *ctx);
extern void amiga_clipboard_init(TrapContext *ctx);
extern uaecptr amiga_clipboard_proc_start(TrapContext *ctx);
extern void amiga_clipboard_task_start(TrapContext *ctx, uaecptr);
extern void clipboard_disable(bool);
extern void clipboard_vsync(void);

#ifdef FSUAE
void clipboard_init(void);
void clipboard_changed(void);
void clipboard_reset(void);
void clipboard_active(int active);
#endif

#endif /* UAE_CLIPBOARD_H */
