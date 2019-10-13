"virtual parallel port protocol" (vpar) for Amiga Emulators
===========================================================

written by Christian Vogelgsang <chris@vogelgsang.org>

Introduction
------------

The vpar protocol is an I/O mechanism to communicate the state of the Amiga
parallel port (here emulated in FS-UAE) so that external applications can
simulate a device connected to this virtual parallel port. This application is
called a `device emulator` and can simulate e.g. a printer, a PLIP device, or a
video digitizer.

Since all lines of the parallel port can be programmed to be either an input or
output, the protocol must handle the update of output changes from FS-UAE's
Amiga parallel port to the device emulator and has also communicate changes of
inputs driven by the device emulator back to the Amiga emulator.

The parallel port consits of the 8 data lines and the control lines including
SELECT, POUT, and BUSY. Also the control signals STROBE and ACK with fixed 
direction are supported.

An external application can alter the state of input lines by sending commands
via the vpar protocol. Depending on the direction of each pin the protocol
allows to set all input pins with new values. Also the external application
can trigger the ACK line if desired.

Assumptions
-----------

It is assumed that both processes, the Amiga emulator and the device emulator
are connected with a fast bi-directional link and that each of the protocol's
message is transported in a timely fashion so that it does not introduce too
much latency. Otherwise the emulation accuracy is lost.

It is further assumed that the Amiga emulation runs in real time so that the
emulator processes the parallel port accesses with (almost) the same timing a
real Amiga would have processed them. This requires that the device emulator
also runs and processes vpar commands in real time.

Note that parallel port signal changes have a max frequency of around 50 KHz.
So a reasonable fast modern computer should handle them easily in real time
emulation.

Protocol
--------

### 1. Amiga Updates State

Every time the emulated Amiga (in FS-UAE) changes a signal (either data or
control lines) a state update is generated in the vpar protocol and send with
two bytes to the device emulator.

The first control byte holds the state of the control signal lines in the lower
nybble and some vpar flags in the upper nybble. The second byte holds the
current value of all 8 data lines of the parallel port.

            FS-UAE ---[ cb , db ]--> devemu
   
   * **cb**: control byte
   * **db**: data byte (value of data on parallel port)

The control byte has the following bits:

                0x01 0 = BUSY line set
                0x02 1 = POUT line set
                0x04 2 = SELECT line set
                0x08 3 = STROBE was triggered
                0x10 4 = VPAR_REPLY: is an reply to a devemu trigger (see section 2)
                0x20 5 = not used -> 0
                0x40 6 = VPAR_INIT: Amiga emulator is starting (first msg of session)
                0x80 7 = VPAR_EXIT: Amiga emulator is shutting down (last msg of session)
   
The `VPAR_REPLY` bit is only set if the byte message is reply to a devemu
trigger (see section 2). In an Amiga update this bit is never set.

`VPAR_INIT` is sent on the first Amiga update whenever the emulation starts. If
the virtual Amiga is reset then the `VPAR_INIT` is re-sent on next startup.

`VPAR_EXIT` is sent as a last update right before the Amiga emulator is shut down.

A device emulator can use `VPAR_INIT` and `VPAR_EXIT` to restart or quit its
operation in sync with the Amiga emulator.

### 2. Devemu Triggers a Change

The device emulator sends a 2 byte message to the Amiga emulator whenever it
wants to change an input line of Amiga's parallel port. The Amiga emulator
always confirms this trigger with a status update reflecting the change (see
section 1).

            FS-UAE <--[ cb1, db ]--- devemu
            FS-UAE ---[ cb2, db ]--> devemu

The control byte **cb1** sent from the device emulator has the following bits:

                0x01 0 = BUSY line value
                0x02 1 = POUT line value
                0x04 2 = SELECT line value
                0x08 3 = trigger ACK (and IRQ if enabled)
                0x10 4 = VPAR_DATA: set following data byte
                0x20 5 = VPAR_CTL: set line value as given in bits 0..2
                0x40 6 = VPAR_SET_CTL: set line bits given in bits 0..2
                0x80 7 = VPAR_CLR_CTL: clr line bits given in bits 0..2

The lower nybble holds the requested values for the parallel port's control
lines and the second byte holds the requested value for the data port.

The high nybble holds flags that decide what to do with the values given for
control and data port.

If the `VPAR_DATA` flag is set then the value of the second byte is actually
set as the new input value for the data port. Note that only the bits set as
input in the DDR of the data port are actually changed in the Amiga emulation.
All others are simply ignored. If the `VPAR_DATA` flag is not set then the data
port is not altered. However, you still must send a second byte with a dummy
value.

`VPAR_CTL`, `VPAR_SET_CTL`, and `VPAR_CLR_CTL` are three different ways to set
the signal levels on the control lines of the parallel port. While `VPAR_CTL`
directly sets the value for all three lines (`BUSY, POUT, SELECT`) as given in
bits 0-2, the `VPAR_SET_CTL` and `VPAR_CLR_CTL` only set/clear the bits set to
one in bits 0-2 of the control byte. The latter ones are most useful if you
want to toggle a single line of the port only.

Bit 3 is special and always triggers an ACK in the Amiga emulation if it is set.

Note: You can use this device trigger and send a pair of zero bytes (00, 00) to
query the state of the parallel port in the Amiga emulation without altering
anything. The emulator will answer with its current values. Use this if your
device emulator is not sure about the current state, e.g. if you start the
device emulator while the Amiga emulator is already running.

Each trigger send with 2 bytes from the device emulator is immediately answered
by a reply byte pair from the Amiga emulator. Wait for this reply in the device
emulator to make sure the Amiga emulator state has accepted and changed its
state accordingly.

The format of the reply is the same as used in a status update (see section 1).
This time the `VPAR_REPLY` bit is always set to denote that this update was
generated as a response to the device emulator change trigger.

### 3. Remarks

- It might be sensible to propagate changes in the data direction registers
of both control and data lines also via vpar to the device emulator. Currently,
DDR masks are only handled inside the Amiga emulator and external changes from
the device emulator only affects signals that are set to input in the DDR.

Implementation
--------------

The transport channel used there is realized with a pseudo
terminal (PTY) opened in the device emulator. This allows the FS-UAE to use 
the simple file API for input/output.

The first device emulator using this protocol is a SW emulator of the plipbox
PLIP-to-Ethernet device written in Python for the [plipbox][2] project.

[1]: https://github.com/cnvogelg/fs-uae-gles/tree/chris-devel
[2]: http://lallafa.de/blog/amiga-projects/plipbox
