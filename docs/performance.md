# Performance

1. Your device must be powerful enough to run the emulation.
2. The emulator must be allowed to run sufficiently often on the CPU.

The first point is rarely a problem, modern desktop and laptop computers are
usually powerful enough. The most important aspect is having a relatively fast
CPU, and single thread performance is especially important.

The second point is more tricky. Power saving features and other
resourcy-hungry applications can interfere with the emulation. Sometimes to the
point of the emulation not running fast enough, but more often the result is
stuttering and uneven emulation speed.

## Why is stable performance so important?

In order to get smooth video output without stuttering (and without audio
issues), the emulator must be able to both emulate frames _and_ display them on
screen very quickly (typically 50 or 60 times per second). Modern games running
on your system can typically deal with small delays and uneven performance, but
old computer systems ran at a fixed speed. So even quite small delays in
generating and displaying a video frame can cause noticable video stuttering
(or crackling audio).

## How to identify performance issues?

Write about the performance overlay (Mod+O).

## Make sure the the emulator window has focus

The application might get less run-time when it's not running in the foreground
(meaning not having mouse and keyboard focus) - depending on the system. So if
you notice uneven performance, make sure that the emulator window is focused.

Running in full-screen might further ensure that the emulator gets enough
resources.

## Switch to a performance power profile

Avoid powersaving, this can throttle down your CPU/GPU making them run much
slower than they can. Powersaving can also cause the emulator getting too
little run-time, for example by pausing the emulator slightly, but long enough
to cause stuttering.

If you have Linux; install the `gamemoded` daemon to allow the emulator to
automatically switch to the performance CPU governor while the emulation is
running.

## Plug your computer to external power

Depending on your system, your computer may be throttled when running on
battery alone. Switching to a performance power profile might alleviate this,
or maybe not. You can try running with or without external power and see if it
makes a difference.

## Close other applications

Especially web browsers can use more CPU and resources than you might think,
depending on the open tabs. On the other hand, if the applications are idle,
they should not interfere.

If you believe other applications might interfere, you can try closing them and
see if it helps. You can also use the task monitor of your operating system to
see what applications are using resources.

## Performance tips when insufficient host performance is the problem

You might recall running UAE on an old 200 Mhz Pentium processor, and wonder
how it is possible that performance is even a topic in 2020! The emulation
accuracy in modern UAE emulators is much better than it was 10-15 years ago,
and this increased accuracy comes at the cost of requiring more powerful
computers.

Emulating the Amiga 1200 is especially demanding, since the CPU is clocked
twice as high as the CPU Amiga 500, and the emulator spends most of its time
emulating the CPU.

### Use the slowest Amiga needed for the job

Don’t emulate an Amiga 1200 if an Amiga 500 or an Amiga 600 will do. The CPU in
Amiga 1200 is twice as fast as the one in Amiga 1000/500/500+/600, and you need
a much more powerful computer to emulate A1200 at highest accuracy.

For example, many people use Amiga 1200 settings when running WHDLoad-installed
games. But for most games, an Amiga 600 with Kickstart 2.0 will work equally
well (or better), but will require a lot less resources to emulate.

### Avoid using unnecessary features

FS-UAE requires more CPU and GPU performance when certain options are enabled.
For example GPU performance will be affected when using
[pixelshaders](shaders.md).

### Disabling video sync

FS-UAE will usually have better performance with video sync turned off, but
this will introduce some stuttering and/or screen tearing.

### Reducing the color depth

FS-UAE can use less color depth in the internal video buffers as well as in the
textures uploaded to the video card. For slower/older computers, this can give
a nice speed boost (but should not matter for modern PCs):

    video_format = rgb565

(This option will also automatically set `texture_format = rgb5`).
