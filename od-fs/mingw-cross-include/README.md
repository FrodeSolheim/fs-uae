# MinGW cross-compile include shims

Shared WinUAE and 86Box code includes some Windows headers with mixed-case names, such as
`<Windows.h>`. This works on Windows (and macOS), where file names are case-insensitive, but
MinGW installs its headers with lowercase names (`windows.h`), so the includes fail when
cross-compiling on a case-sensitive file system such as Linux.

Each header here has the mixed-case name and just includes the lowercase MinGW header. This
lets the shared code stay unchanged.

`configure.ac` adds this directory with `-idirafter` when building for Windows and
`#include <Windows.h>` fails to preprocess. With `-idirafter` it is searched after the system
directories, so a real header with the exact name always wins.

To support another header, add a file with the mixed-case name that includes the lowercase one.
