# FSGUI

Copyright © 2025 Frode Solheim

fsgui is FS-UAE's Python UI toolkit, built on fsapp: its windows are made from fsapp's surfaces, and it uses fsapp's events and drawing. It sits above fsapp and below fsemu and the Amiga-specific top, and may only depend on fsapp. fsgui has only a Python side: native code that the toolkit needs (such as text and image drawing, fsapp's `_fsapp_font` and `_fsapp_image` extension modules) goes into fsapp's API.

The modules and their order (fsapp → fsgui → fsemu → the top) are described in [Modules.md](../../Docs/Development/Modules.md), with the check that enforces the order.
