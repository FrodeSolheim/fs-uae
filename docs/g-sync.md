FIXME:

- Important to set desktop refresh rate above the emulation rate?
  I.e. for PAL, use at least 60 Hz desktop refresh rate, and
  at least 85 Hz for NTSC? To avoid g-sync driver throttling the speed?
  (Seems to be imported for Nvidia on Linux at least)

- Having extra monitors connected is problematic. More than one monitor
  disables G-Sync (on Linux). Disabling the monitor temporarily in
  Nvidia settings causes occasional stuttering - also in window mode
  when G-Sync is not active (some polling of the disabled display by the
  Nvidia driver interfering with the output??)

- Tip: Enable visual indicators and ensure that the indicators says
  G-SYNC in green and FLIP in green (not BLIT in red).
