# fsapp

Copyright © 2025 Frode Solheim

fsapp is the base of FS-UAE's modules: generic utilities and the runtime (the main thread and the UI thread, the host window, surfaces, events, text and image drawing, channels and file dialogs). It knows nothing about emulation, and may not depend on any module above it (fsgui, fsemu or the Amiga-specific top). Its C++ side is here, its Python side in `od-fs/Python/fsapp`.

The modules and their order (fsapp → fsgui → fsemu → the top) are described in [Modules.md](../Docs/Development/Modules.md), with the check that enforces the order.
