# Recording savestates

This document describes how the savestate system in UAE is extended and
somewhat abused in order to make re-recording (*) work.

(*) Meaning recording a play session while also using savestates to go back
in time to correct mistakes.

FS-UAE has for a long time supported determistic execution, needed for netplay.
The problem that needed to be solved for re-recording purposes is that saving
a state and loading it again does not truly restore to the point where the
you were right before saving. Minor differences in the loaded state - which
may not matter when casually using save states to save progress - cause
butterfly effects with timing of UAE events, causing emulation to get out of
sync.

## Challenges with savestates

There are a few problems with the current save states in UAE which prevents
them from restoring state perfectly.

1. The first one is somewhat easy to solve - not all state is actually saved.
Presumably, this (missing) state is not critical to make save states work for
simply saving progress.

2. The second issue is that save states are saved and restored without
hsync/vsync-handlers - a natural place to do this, but a tricky problem is
that hsync handlers are often invoked by cycle waiting *in the middle of*
CPU instrunction handling. The program counter (PC) and other state is stored
in the state file, but when restoring this state, the current instruction
will be re-run from start. This means that in some cases, the effect of (parts)
of the instruction might be applied twice. If the instruction simply stores a
value to RAM, this is not a problem. But if the instruction is for example an
increment instruction, the destination could be increment twice instead of
once when loading the state, since the result of the first increment is already
stored in the save state.

3. A third issue is that even the act of saving state modifies the emulation
state somewhat. The blitter might be forced to finish, CIA emulation does some
stuff and CIA/event timers are updated to "current time" when saving. To
illustrate the issue, if you are playing back a recording, even just saving the
state will in some cases cause a desync.

## New way of saving states...

Currently, a workaround has been devised for FS-UAE. It is not a perfect
solution, more like a hack. And it only works for some configurations /
emulation features.

States are saved and loaded between vsync_handler_pre and vsync_handler_post.
When a state is staved, a second state is saved just after, when the current
instruction is done. This way, we have a consistent savestate with the results
of the last instruction applied, and the program counter (PC) is updated.

Additionally, another custom save state is also saved now; an FS-UAE-specific
state file containing more information than the original savestates. It was
done this way so that the original savestate system is modified as little as
possible, in order to avoid unintentional consequences.

Finally, any side effects of saving state is removed (CIA timers are not
updated, etc). The end result is that the statestave represents the state
after the instruction which triggered vsync has completed.

## ...and loading states

When loading a savestate, a normal load is performed first. Then, the
vsync_post_handler is called in order to configure the video/display/etc
correctly for the upcoming frame.

After this is done, we now load the second save state immediately, which
contains the result of waiting until the (then) currently executing
instructions had finished. We should now have a consistent state, and the vsync
post handler has also been executed.

Finally, we load the special state file which tries to correct the state and
make it identical to the state as it was when saved, by restoring a lot of
global variables containing state, and the original event tables.

So, why not only use the final custom state file which "fixes everything"?
Well, the current savestate system does a *lot* of things when restoring, it's
quite complicated. So it is a big advantage to be able to re-use this existing
functionality. This means that when loading the custom state file, everything
more or less initialized correctly, the loading routine just needs to tweak
stuff like timing variables, and we do not have to load stuff from disk or 
allocate and initialize chip RAM. That has already been taking care of.

## Only used when recording

This new way of saving states is only used when recording, since it is also
possible that saving/loading states this way could have unintended side efects,
and since the original savestate mechanism works well enough for other use
cases.
