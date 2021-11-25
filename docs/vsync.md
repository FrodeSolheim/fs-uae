# Vsync tips and notes

TODO: Write more about the performance overlay and how to look for problems

- Use the performance overlay to spot problems

Linux:

- Try to close as many programs as possible, at least the ones that runs stuff,
  either periodically or all the time. Especially browsers.

- Closing programs that might otherwise have audio devices open can help audio
  performance and latency when using PulseAudio. Again, closing browsers can
  help greatly here.

- Make sure that the emulator is the active window. For example if you have
  more than two monitors and run the emulation in fullscreen on one of the
  monitors. vsync performance may be really bad if the emulator does not have
  focus.

- If you have more than one monitor, consider disabling all but the one you
  want to play on. This will allow your OpenGL driver to flip instead of
  blitting the final output, and will likely increase performance and
  regularity.

- For nVIDIA, you can "Enable Graphics API Visual Indicator" via nVIDIA
  settings. This will show on screen whether flipping is used or not.
