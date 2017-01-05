# JIT Compiler

FS-UAE includes a Just-in-Time (JIT) compiler for x86 and x86-64. The JIT
compiler is used to speed up emulation dramatically, at the expense of some
compatibility. The JIT compiler can only be used with 68020 or higher
emulated CPUs, and is not compatible with MMU emulation.

## Enabling the JIT Compiler

To enable the JIT compiler, use the following option:

    jit_compiler = 1

> **Note:** In previous versions of FS-UAE, you would enable the JIT compiler
> with the option `uae_cachesize = 8192`. This is no longer sufficient to
> enable the JIT compiler, and you also do not need to specify cache size any
> more.

## Toggling JIT on/off

It is possible to toggle the JIT compiler on/off during emulation. One
use case for this is to speed up your Workbench experience, but disable
the JIT before running games, for better compatibility.

**TODO: Write how**

> **Note:** Enabling JIT during emulation will not work unless FS-UAE was
> started with the JIT compiler initially on. This is because some FS-UAE
> initialization is only performed when JIT is enabled.

## Options

TODO: Links to option reference:

* jit_compiler
* jit_mempory

Additional low-level options:

* uae_cachesize
* uae_comp_trustbyte
* uae_comp_trustword
* uae_comp_trustnaddr
* uae_comp_trustlong
* uae_compfpu
