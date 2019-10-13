Wait for Blitter

Default:
Example: automatic

Compatibility hack for programs that don't wait for the blitter correctly,
causing graphics corruption if CPU is too fast.

Value: disabled
Value: automatic
Value: noidleonly
Value: always

The default value depends on the configured Amiga model.
If the following two conditions are true, the option is initialized
to "automatic":
* CPU is 68020+ or CPU emulation is not cycle exact
* Immediate blitter is not enabled

Otherwise, the option is initialized to "disabled".

Immediate blitter [uae_immediate_blits] and waiting blits [uae_waiting_blits]
can't be enabled simultaneously. If immediate blitter is enabled, waiting
blits will be forcefully disabled.




waiting_blits=automatic



static const TCHAR *waitblits[] = { _T("disabled"), _T("automatic"), _T("noidleonly"), _T("always"), 0 };


        if ((p->cpu_model >= 68020 || !p->cpu_cycle_exact) && !p->immediate_blits)
                p->waiting_blits = 1;
