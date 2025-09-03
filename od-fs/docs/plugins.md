# Plugins

You can install plugins to increase the capabilities of FS-UAE. It currently supports the following plugins:

- `CAPSImg` - To add support for `.ipf` floppy images
- `QEMU-UAE` - To add PPC accelerator support.
- `FloppyBridge` (soon?) - To add support for real floppy drives.

You can download plugins from https://fs-uae.net/download and extract the contents into Data/Plugins. When extracted correctly, you'll then end up with something like this (plus some more files):
```
Data/Plugins/CAPSImg/
Data/Plugins/CAPSImg/Plugin.ini
Data/Plugins/CAPSImg/Linux/x86-64/CAPSImg.so
Data/Plugins/CAPSImg/Windows/x86-64/CAPSImg.dll
(...)
```

## Plugin library search path

Plugins are looked up first in `Data/Plugins`, and then in `(ApplicationDir)`, where `(ApplicationDir)` represents the directory containing the FS-UAE executable.

Plugin libraries can be wrapped in plugin directories (`CAPSImg/Windows/x86-64/CAPSImg.dll`) - which is what you get if you download the plugins from [fs-uae.net](https://fs-uae.net) - or not.

Here is an example to illustrate search order and where FS-UAE looks for plugin libraries. In this case, the operation system is Linux (x86-64), and the plugin to look for is `CAPSImg`.  Also note that on case sensitive file systems - and for historical reasons - FS-UAE also checks for the lowercase version of the library name.

```
Data/Plugins/CAPSImg/Linux/x86-64/CAPSImg.so
Data/Plugins/CAPSImg/Linux/x86-64/capsimg.so
Data/Plugins/CAPSImg.so
Data/Plugins/capsimg.so
(ApplicationDir)/CAPSImg/Linux/x86-64/CAPSImg.so
(ApplicationDir)/CAPSImg/Linux/x86-64/capsimg.so
(ApplicationDir)/CAPSImg.so
(ApplicationDir)/capsimg.so
```

To provide some compatibility with WinUAE, on 64-bit Windows, `CAPSImg_x64.dll` is preferred over `CAPSImg.dll` when looking in  `Data/Plugins/` and `(ApplicationDir)`. So on 64-bit Windows, the search order is:

```
Data/Plugins/CAPSImg/Windows/x86-64/CAPSImg.dll
Data/Plugins/CAPSImg_x64.dll
Data/Plugins/CAPSImg.dll
(ApplicationDir)/CAPSImg/Windows/x86-64/CAPSImg.dll
(ApplicationDir)/CAPSImg_x64.dll
(ApplicationDir)/CAPSImg.dll
```
