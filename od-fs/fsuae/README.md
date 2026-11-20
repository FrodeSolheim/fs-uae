# FSUAE

This directory contains the main startup and glue code for FS-UAE. It depends on uae, fsemu, fsapp and fsgui.

fsuae is part of the top of FS-UAE's modules, everything Amiga-specific, together with the emulation core, the UAE glue in `od-fs` and the Python packages `uae` and `app`. The top may use every module below it (fsapp, fsgui and fsemu), and nothing below it may use the top. Its C++ side is here, its Python side in `od-fs/Python/fsuae`.

The modules and their order (fsapp → fsgui → fsemu → the top) are described in [Modules.md](../Docs/Development/Modules.md), with the check that enforces the order.
