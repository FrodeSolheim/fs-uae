# Performance

1. Your device must be powerful enough to run the emulation.
2. The emulator must be allowed to run sufficiently often.

The first point is rarely a problem, modern desktop and laptop computers are
usually powerful enough. The most important aspect is a relatively fast CPU,
and especially how fast a single thread can run.

The second point is more tricky. Power saving features and other
resourcy-hungry applications can interfere with the emulation. Sometimes to
the point of the emulation not running fast enough, but more often the result
is stuttering and un-even emulation speed.

## Why is stable performance so important?

In order to get smooth video output with stuttering (and without audio issues),
the emulator must be to both emulate frames *and* display then on screen very
quickly (typically 50 or 60 times per second). Modern games running on your
system can typically account for small delays and uneven performance, but old
computer systems ran at a fixed speed. So even quite small delays in generating
and displaying a a video frame can cause noticable video stuttering (or
crackling audio).

## How to identify performance issues?

Write about the performance overlay (Mod+O).

## Make sure the the emulator window has focus

On many systems, the application will get less run-time when it's not running
in the foreground (meaning not having mouse and keyboard focus). So if you
notice un-even performance, make sure that the emulator window is focused.

Running in full-screen might further ensure that the emulator gets enough
resources.

## Switch to a performance power profile

Avoid powersaving, this can throttle down your CPU/GPU making them run much
slower than they can. Powersaving can also cause the emulator getting too
little run-time, for example by pausing the emulator slightly, but long enough
to cause stuttering.

If you have Linux; install the gamemoded daemon to allow the emulator to
automatically switch to the performance CPU governor while the emulation
is running.

## Plug your computer to external power

Depending on your system, your computer may be throttled when running on
battery alone. Switching to a performance power profile might alleviate this,
or maybe not. You can try running with or without external power and see if
it makes a difference.

## Close other applications

Especially web browsers can use more CPU and resources than you might think,
depending on the open tabs. On the other hand, if the applications are idle,
they should not interfere.

If you believe other applications might interfere, you can try closing them
and see if it helps. You can also use the task monitor of your operating
system to see what applications are using resources.

## FIXME: Performance tips when insufficient host performance is the problem

- ...
