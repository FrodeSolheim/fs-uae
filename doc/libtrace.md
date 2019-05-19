# libtrace - A library call tracer for FS-UAE

written by Christian Vogelgsang
under the GNU Public License V2

## Introduction

**libtrace** is a debug and analysis feature for FS-UAE that allows you to
print out messages whenever a library call was executed in AmigaOS. If you
supply a .fd file for a library the trace message includes the function name,
all input parameters and also the return value.

You define a trace profile by writing a config file that gets loaded when
you enable libtrace. In the config file you specify the libraries to be traced,
the associated .fd file and a set of functions that shall be traced.

libtrace is controlled via the internal FS-UAE debugger and allows you to
load config files, enable/disable the feature, limit traces to a single task,
or write traces to a file.

## Installation

Currently, the libtrace feature is located in an own branch of my FS-UAE git
repository.

Clone the [libtrace branch][1] and build FS-UAE from source.

[1]: https://github.com/cnvogelg/fs-uae/tree/libtrace

## Configuration File

First you need to define the trace profile in a config file. Its typically
called **libtrace.cfg** and if it resides in the current directory when
launching FS-UAE it will be loaded and activated automatically.

The config file is a text file with a list of one or multiple libraries, the
associated `.fd` files (if any) and the set of functions to instrument for
tracing:

```
name1.library
name1_lib.fd
*
#
name2.library
name2_lib.fd
198
-30
AllocMem
#
```

First line is the library name as it used on the Amiga for OpenLibrary().

Next line gives a host local file system path where to find the associated
.fd file for this library. Only if this file is available you will see the
function names and the arguments. If you don't have a .fd file for this
library then leave the line blank but do not omit it!

Then a list of function names is given that is terminated with a hash **#**
line. Use asterisk * to trace all functions of a library.

You can either state the function name, the LVO (library vector offset) as
a positive or negative number for each entry.

Additionally, you can prefix each function entry with flag characters:

* **!** do not trace the given function. Useful if you first include all
  functions with * and then remove some functions again.
* **?** do not trace the result value. It speeds up tracing a bit and omits
  the second return line durin trace.
* **<** enter debugger of FS-UAE right before the function gets called
* **>** enter debugger of FS-UAE right after the function has been called
* **+** show extra information. right now WaitPkt() shows the DOS packet

## Example Config

```
dos.library
fd/dos_lib.fd
+*
!Open
#
exec.library
fd/exec_lib.fd
InitResident
#
```

This config traces both **dos.library** and **exec.library** and picks the
.fd files from a **fd** directory in the current directory.

While in dos library we trace all functions (except Open()) with extra flag
(+), in exec library only the InitResident() call is traced.

## Control libtrace

The library tracer is controlled from the FS-UAE debugger.
Make sure to enable it in the FS-UAE configuration with:

```
console_debugger = 1
```

and enter it with `F11 + d`. Now you can enter commands on the console.

The following commands are available for libtrace:

```
  L                     print libtrace status.
  Le [<0-1]             enable/disable/toggle libtrace
  Lc 'config'           set libtrace config file
  Lt 'task'             trace only task with this name
  Lo 'output'           write traces to output file
```

First set the config file with `Lc` command and enable the tracer with `Le 1`:

```
Lc 'mytrace.cfg'
Le 1
```

Now you need to exit the monitor (`x` command) and reset the virtual Amiga
(with `F11 + r`) to activate the trace.

By default library calls from all tasks running in AmigaOS are traced, but with
the `Lt` command you can limit the traces to a single task given by its name.

The traces are written to the console by default. If you use the `Lo` command
you can redirect the trace output to a file. Use `Lo ''` to reset output to
console again.

```
Lt 'CON'
Lo 'console.log'
```

## Libtrace Output

A function trace typically consists of two lines: an output before the call
is issued with the incoming parameters and a second line with the result of
the call (value of `d0`).

The output looks like:

```
@00fcc816  0000f8c8:Initial CLI       dos           FindSegment(name[d1]=00001e45, seg[d2]=00000000, system[d3]=00000000)
@00fcc816  0000f8c8:Initial CLI       dos           FindSegment -> d0=00000000
```

The first field on each line denotes the **current program counter (PC)**
location that entered the call.

The next field is a pair of pointer and name and refers to the **exec task**
that is currently running.

Then the **short name of the library** is given.

Finally the **function name** with the arguments. Each argument is given
by name, register and passed value.

The result line contains the value of `d0` after executing the call.

## Limitations

Currently, libtrace can only patch library entries that are coded as a `JMP`
jump to function. Special functions like cache clear or ancient BCPL
dos.library entries cannot be patched.

However, it handles external patching of libraries with `SetFunction() and
can trace all libraries including expansion, exec, and dos.

## Technical Details

The basic idea for library tracing here is to replace the `JMP` opcode in the
library jump tables with special **a-line** opcodes `$axxx` that allow you to
execute custom native code in FS-UAE when the opcode is triggered during m68k
CPU emulation. By default the CPU emulation allows to execute code on a-line
opcodes only inside the FS-UAE extension area, but for libtrace we have to
enable this feature for all memory regions.

Libtrace is initialized like many other FS-UAE extensions from a Resident
located in the virtual expansion area of FS-UAE. It is executed from expansion
library during system init. In this situation we first scan the existing
library list of the system if already libs are here that need to be
instrumented. Then we patch `(Old)OpenLibrary()` to track first opens of
future libs we want to instrument and also patch `SetFunction() to track
changes on instrumented libraries.

A library is instrumented as follows: First we allocate a new (simple) a-line
trap for this library. Then we internally assign the library name and base
address with this new trap. Now we replace the `JMP` instruction of each
function entry we want to patch with the **a-line opcode**. Finally, the
library checksum is re-calculated. The patched `SetFunction() call ensures
that if a function entry is changed it still has the correct a-line opcode
instead.

The approach is very compatible and also almost invisible to the running
system.

A library call is now performed as follows: The user code executes a `JSR` to
the location of the function in the function table of the library. Instead
of directly executing the function with `JMP address`, an instrumented call
hits the a-line and enters our **pre_call_trace** function in FS-UAE.

There the a-line opcode is retrieved from the current `PC`. With the a-line
opcode we can look up the library that is associated with it and already know
the library name and the library base address. Next we determine the **LVO
(library vector offset)** of the called function by subtracting the current
`PC` from the library base address. With the LVO we know what function was
called and can lookup function information from the .fd file.

The .fd file provides the function name, the name of the arguments and the
associated CPU registers. The values are retrieved from the emulated CPU
and the pre call line of the trace is generated and written to the console.

If we also want to trace the return value of the function then all information
on the current call including library and function details are filled into
a native malloc()ed structure and the pointer is pushed on the **Amiga**
stack(!).

Then the address of a pre-allocated location in Amiga memory is pushed on the
stack. It contains a reserved **a-line opcode** that triggers our native
**post_call_trace** function after calling the library function.

Finally we push the original function pointer found next to our a-line opcode
in the library on the stack. The trap is flagged as AUTO_RTS and that will
automatically execute and implicit `RTS` opcode when we leave our
``pre_call_trace`` function.

```
Stack without return value tracing:
 * user code return address (pushed by initial JSR to lib)
 * library function address (triggered by AUTO_RTS of lib a-line)

Stack with return value tracing:
 * user code return address (pushed by initial JSR to lib)
 * native pointer (32 or 64 bit) to call info structure
 * pointer to a-line opcode that call post_call_trace()
 * library function address (triggered by AUTO_RTS of lib a-line)
```

If we are not interested in the return value then only the function address
is pushed on the stack.

After leaving the pre call function, the AUTO_RTS will pop the function
address from the stack and finally execute the actual library function.

Without return value tracing the final `RTS` in the function code will
retrieve the user code return address and return to user land.

With enabled return value tracing the final `RTS` in the function code
pops the address of our post_call_trace() a-line opcode from the stack
and executes our native post_call_trcace() in FS-UAE.

The post function pops the native pointer for the call info structure from
the Amiga stack and now has all information about this call available. It
finally reads out the result value from CPU register `d0` and outputs the
post trace line to the console. The call info is freed.

After the post_call_trace() a-line an `RTS` is placed that retrieves the
user code return address from the stack and returns to user land.

Note: we always have a slightly larger stack foot print compared to a function
that was not instrumented. Usually you can neglect this. But you can reduce
the stack consumption by omitting the return value trace.

Note: exec's `StackSwap()` fiddles with the stack and therefore we enforce
that this call never traces the return value. While it is possible to place
our stack payload on the new stack, existing Amiga code places the new stack
right below the current stack and if we push extra values on the old stack to
pass on our native pointer we corrupt the new stack...

EOF










