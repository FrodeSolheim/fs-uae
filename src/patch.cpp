/*
* UAE - The Un*x Amiga Emulator
*
* Patch System Utilities
*
* Written by Christian Vogelgsang <chris@vogelgsang.org>
*/

#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"

#include "uae/memory.h"
#include "traps.h"
#include "autoconf.h"
#include "uae/patch.h"
#include "newcpu.h"

#define JMP 0x4ef9
#define JSR 0x4eb9

void patch_func_init_pre_call(TrapHandler func, patch_func *pf, int flags)
{
    /* setup new function entry:
       trap to custom uae function
       and jump to old function */
    pf->new_entry_addr = here();
    calltrap(deftrap2(func, TRAPFLAG_NO_RETVAL, _T("")));
    dw(JMP);
    pf->old_entry_jump = here();
    dl(0); /* placeholder for old address. will be patched in setup() below */
}

void patch_func_init_post_call(TrapHandler func, patch_func *pf, int flags, int save)
{
    /* setup new function entry:
       call function via JSR
       finally trap
       and RTS */
    pf->new_entry_addr = here();

    /* save regs before call? */
    switch(save) {
        case PATCH_SAVE_ALL_BUT_D0:
            dw(0x48e7); dw(0x7ffe); // movem.l d1-d7/a0-a6,-(sp)
            break;
        case PATCH_SAVE_D1_RESTORE_A0:
            dw(0x2f01); // move.l d1,-(sp)
            break;
        default:
            break;
    }

    dw(JSR);
    pf->old_entry_jump = here();
    dl(0); /* placeholder for old address. will be patched in setup() below */

    /* restore state after call? */
    switch(save) {
        case PATCH_SAVE_ALL_BUT_D0:
            dw(0x4cdf); dw(0x7ffe); // movem.l (sp)+,d1-d7/a0-a6
            break;
        case PATCH_SAVE_D1_RESTORE_A0:
            dw(0x205f); // move.l (sp)+,a0
            break;
        default:
            break;
    }

    calltrap(deftrap2(func, flags | TRAPFLAG_NO_RETVAL, _T("")));
    dw(RTS);
}

static uae_u32 SetFunction(TrapContext *ctx, uae_u32 lib_base, uae_s16 offset, uae_u32 new_func)
{
    uae_u32 old_a1 = m68k_areg(regs,1);
    uae_u32 old_a0 = m68k_areg(regs,0);
    uae_u32 old_d0 = m68k_dreg(regs,0);

    m68k_areg(regs,1) = lib_base;
    m68k_areg(regs,0) = offset;
    m68k_dreg(regs,0) = new_func;

    CallLib(ctx, get_long(4), -420); // Exec: SetFunction
    uae_u32 old_func = m68k_dreg(regs,0);

    m68k_areg(regs,1) = old_a1;
    m68k_areg(regs,0) = old_a0;
    m68k_dreg(regs,0) = old_d0;

    return old_func;
}

void patch_func_set(TrapContext *ctx, uaecptr lib_base, uae_s16 func_lvo, patch_func *pf)
{
    /* make sure ctx is valid: needs to be an extended trap
       otherwise we can't call SetFunction in Exec */
    if(ctx == NULL) {
        write_log(_T("patch_func_set: No context found!\n"));
        return;
    }

    uae_u32 offset = lib_base + func_lvo;

    /* check library entry and make sure its a JMP */
    if(get_word(offset) != JMP) {
        write_log(_T("patch_func_set: No jump in library function!\n"));
        return;
    }

    /* patch lib function */
    uae_u32 old_func = SetFunction(ctx, lib_base, func_lvo, pf->new_entry_addr);
    pf->old_entry_addr = old_func;

    /* store old pointer in rt_area */
    uae_u32 save_org = here();
    org(pf->old_entry_jump);
    dl(old_func);
    org(save_org);
}
