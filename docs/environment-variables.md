# Environment variables

## Emulator variables

The SDL library used by FS-UAE supports some customization through hints which
can be specified as environment variables. See
[https://wiki.libsdl.org/CategoryHints](https://wiki.libsdl.org/CategoryHints)
for a complete list. Some examples are provided here.

If you want the first mouse click when focusing the window to be ignored by
FS-UAE (doing nothing except focusing the window), you can use the following
environment variable:

    SDL_MOUSE_FOCUS_CLICKTHROUGH=0

If you are on macOS, and you want the FS-UAE in fullscreen to open on a
separate desktop space, use:

    SDL_VIDEO_MACOS_FULLSCREEN_SPACES=1

By default, FS-UAE does not automatically minimize when it loses the focus.
Minizing on focus loss is actually the default behavior of SDL. If you want to
reinstate this behavior, you can set the environment variable:

    SDL_VIDEO_MINIMIZE_ON_FOCUS_LOSS=1

### Debugging

Some environment variables can be set to aid with debugging problems.

To make FS-UAE log more information about input events to the log file:

    FS_DEBUG_INPUT=1

To make FS-UAE log all file system operations (for mounted directories) to the
log file:

    FS_DEBUG_FILESYS=1

FIXME:

    FS_DEBUG_EVENTS=1

FIXME:

    FS_DEBUG_FLUSH=1

## Launcher variables

...
