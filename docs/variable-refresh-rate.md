# Variable Refresh Rate

FS-UAE can be used with variable refresh rate hardware:

- nVIDIA G-SYNC / G-SYNC Compatible
- AMD FreeSync (VESA Adaptive-Sync)

This allows for smooth and even rendering of the Amiga display, similar
to classic v-sync, but with lower latency, and without the occasional
hiccups due to missing a frame.

If you can afford it, I highly recommend getting a setup with G-SYNC or
FreeSync for FS-UAE (and other emulators). This is definitively the best
way to experience FS-UAE.


## Desktop refresh rate setting

Due to the way variable refresh rate work and based on experience, you get
most reliable results when your desktop refresh rate is *higher* than the
refresh rate of the emulated system.

So for example, if you want to emulate both 50/60 Hz systems, your desktop
refresh rate should be *more* than 60 Hz, for example 85 Hz. But you can
also just go as high as your monitor allows, for example 144 Hz. The important
thing is to use > 60 Hz (or > 50 Hz if you only want to emulate PAL systems).

Note, when you want to visually verify that G-SYNC / FreeSync is working
(see the next section), any stuttering due to it *not working* will be more
obvious with a lower refresh rate, so 85 Hz is a quite good one for this
purpose.


## Visually verifying that G-SYNC / FreeSync is working

The best way to see if G-SYNC / FreeSync is actually working is to use
your eyes. Start the emulator with a game / program which scrolls with
constant speed. For example:

    War Zone (1991)(Core)[cr Flashtro][t +26 Flashtro]

This cracktro has a horizontally scrolling text with fixed speed. If you
run in windowed mode, you should see micro-stuttering (uneven movement
of the scrolling text). When you change to fullscreen, if G-SYNC / FreeSync
is working properly, the scrolling should be perfectly smooth in comparison.


## Linux

In general, variable refresh rate will only work in fullscreen (when
FS-UAE covers the entire X display). This means that you must likely only
have one monitor enabled in a multi-monitor setup, and you cannot have
any overlapping windows (for example, desktop notifications appearing will
temporarily disable variable refresh rate).


## nVIDIA G-Sync on Linux

Use NVIDIA X Server Settings to enable "Allow G-SYNC/G-SYNC Compatible" under
OpenGL Settings.

To verify that G-SYNC is working/enabled, you can also enable 
"Enable G-SYNC/G-SYNC Compatible Visual Indicator" which adds a green G-SYNC
logo in the upper right corner of the screen when FS-UAE is running and
G-SYNC is active.

Please note that G-SYNC will only work (at the time of this writing) of you
have a single monitor enabled. If you have more than one monitor, disconnect
the other one(s) before running FS-UAE. Just disabling the other monitors 
should also work, but the author has experienced stuttering every few seconds
when other monitors are connected, even if they are disabled.


### AMD FreeSync on Linux

Please make sure you have Linux kernel 5.0 and greater. You need to modify
your Xorg configuration to enable freesync. Create a file called
`/etc/X11/xorg.conf.d/10-amdgpu-variable-refresh.conf`
(create /etc/X11/xorg.conf.d/ if needed) with the content:

    Section "Device"
        Identifier "AMD"
        Driver "amdgpu"
        Option "DRI" "3"
        Option "VariableRefresh" "true"
    EndSection

To verify that variable refresh rate is enabled, you can check the output
of `xrandr --prop`, you should see something like this:

    ...
    DisplayPort-1 connected primary 1920x1080+0+0 [...]
    ...
            vrr_capable: 1 
                    range: (0, 1)

If your connected display supports FreeSync, you should see vrr_capable: 1.

Additionally, to check that variable refresh rate is enabled in the driver,
you can check for the following line in $HOME/.local/share/xorg/Xorg.0.log:

    AMDGPU(0): VariableRefresh: enabled


# macOS

At the time of writing, the author is not aware of support for variable
refresh rate in macOS.
