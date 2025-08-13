# Debugging FS-UAE

This page contains some information on how you can better debug FS-UAE
problems. In general, checking the contents of `FS-UAE.log.txt` is a good
source of information, as is checking messages printed to stdout. You can make
FS-UAE print log output to stdout with:

    fs-uae --stdout

## Environment variables for more detailed logging

There is a separate page about
[environment variables](environment-variables.md), some of which can be used to
control the amount of logging.

## Compiling a debug version

If you experience a crash, it is helpful to have a stack trace for when the
crash occurs. To compile a version suitable for debugging crashes, you might
want to compile a version with debug symbols (`-g`) and with optimizations
tuned for debugging (`-Og`):

    cd fs-uae-(version)
    make clean
    make make CFLAGS="-g -Og" CXXFLAGS="-g -Og"

You can run FS-UAE from the source directory without installing it, just stay
in the top of the project directory and run (using Linux as an example):

    ./fs-uae <optional args>

You can run FS-UAE through gdb like this:

    gdb --args ./fs-uae <other args>

If the program crashes through gdb when input is grabbed, it can be a bit
awkward (you may need to switch to a virtual console and kill fs-uae/gdb), so
the following is highly recommended to avoid that from happening:

    gdb --args ./fs-uae --no-initial-input-grab <other args>

or, perhaps better: to make FS-UAE never grab input automatically - neither on
startup nor when clicking in the FS-UAE window:

    gdb --args ./fs-uae --no-automatic-input-grab <other args>
