# FS-UAE to-do file

This document is not an exhaustive list of things to do. Please consider it a
partial list of things that might happen to FS-UAE soon or in the future.

- Revisit / set up default shortcuts for state loading/saving.
- Revisit / set up default shortcuts for floppy selection/insertion.

- Move stuff from other to-do lists to this document :p

In progress:

- On-screen keyboard suitable for use with controllers with realistic
  1system-specific keyboards.

To-do:

- You cannot actually choose input devices before startup atm, but you can
  change them from the on-screen GUI.
- GUI rendering is not optimized yet, GUI element textures are created for each
  frame and not reused. Could affect performance on slower systems.
- Monitor selection does not work in all cases.
- CD audio does not work
- Themes are not implemented yet
- Some video scaling options may have no effect yet
- Some video and audio options may have no effect right now (for example GPU
  texture format or audio frequency).
- Net play support is not ready yet
- Support for multiple mice is not ready yet
- Joystick support may be limited to gamepad-like devices initially
- Support controlling the GUI with the mouse.

Probably:

- Built-in file browser with controller support (mainly for floppy selection),
  an oft-requested feature.

Other bonuses:

- Make it easier to specify (any) actions on game controllers in ports,
  independent of controller type but dependent on port mode. (e.g.
  port0_joy_lefttrigger = action_quit)
- Custom names and automatic screenshots for save states.

Done:

- Better audio subsystem with lower audio latency.
- Make better use of high-resolution timers to ensure the emulation runs
  smoothly also in fastest-possible CPU mode.
- Support hot-plugging game controllers.
- Create a better architecture which makes it easier to support different audio
  and video backends. Initial version with ALSA and SDL2 audio backends, and
  OpenGL and SDL2 video backends.
- Ensure that FS-UAE works really well with G-SYNC and FreeSync monitors on
  supported systems.
- Better internal book-keeping and visual graphs of performance and timing, to
  make it easier to identify issues and make sure the emulation runs smoothly.
- Truetype font rendering for better readability and internationalization.
- Better support for high-DPI displays.
- Make the user interface adapt better to non-16:9 displays.
- Make (lib)FSEMU better suitable for integration with different emulators.
- Use RTLD_DEEPBIND with dlopen on Linux when loading plugins, allows for new
  self-contained QEMU-UAE plugin to work around an issue with how Glib is used.
- Bridgeboard & VGA available and works with custom uae\_\* options.
- Better clipboard sharing integration.
- New built-in slirp implementation from QEMU 5.2.0 and reworked the slirp
  support in FS-UAE. Fixes random crashes and corrupt network data.
- Other things I forgot; I haven't been very good at keeping updated Changelog
  entries lately.
