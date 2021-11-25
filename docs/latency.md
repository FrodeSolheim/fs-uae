# Latency

FIXME: Write more about latency and latency-related issues

## Default setup

By default, FS-UAE does not try to reduce latency to the minimum, because this
can reduce performance and introduce issues on some systems. So instead, FS-UAE
defaults to a more conservative approach – a reasonable compromise between
performance and latency.

TODO: Write more about what options to try to enable if you want to try
reducing the latency of the emulation.

TODO: Write more about default busy wait behavior.

FIXME: Maybe only default to busy wait when FS-UAE cannot detect if the machine
is running in performance mode / game mode.
