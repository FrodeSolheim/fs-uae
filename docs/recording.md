# Recording

FS-UAE supports writing input events to a recording file. This allows playing
back a session automatically later, and also to create a video of the playback
(not yet).

NOTE: THIS FEATURE IS A WORK IN PROGRESS

## Configurations

The configuration used during playback must be compatible with the one used
during recording. This means that any configuration which affects the emulated
machine must be identical, while other options may be different.

Examples of things that need to be identical for recording and playback:

- Amiga model and other Amiga hardware options must be the same.
- Joystick port types must be identical.
- Floppy disk images must have identical content.

Examples of things that do not need to be identical:

- FS-UAE display options, such as v-sync, zoom and fullscreen options.
- Devices inserted into emulated joystick ports (only type needs to be same).
- The location and name of floppy disk images.

## Recording and playback modes

FS-UAE includes three modes related to recording and playback.

### Recording mode

In recording mode, FS-UAE records everything (relevant) that happens to 
a recording file. Deterministic mode is automatically enabled.

FIXME: Shortcut to stop recording?

### Playback mode

In playback mode, FS-UAE injects the events that happened during recording at
the correct moment while playing back the recording. Deterministic mode is
automatically enabled.

User input (affecting emulation) is disabled in playback mode, since this
would otherwise tamper with the playback.

FIXME: Shortcut to stop playback?
FIXME: Shortcut to enter (re-)recording mode?

### Re-recording mode

Re-recording mode is a combination of recording and playback mode. In this
mode, FS-UAE will play back the recording (if it exists), but also allow the
user to take control at any time and resume *recording* from that point on.

If you load a savestate which is a previous state of your current state,
FS-UAE will keep the current recording and start playing back from the
position of the loaded state. You can then take control and assume recording
at any time as usual.

If you load a savestate which is *not* a previous state of your current state,
then FS-UAE will load the recording from the savestate and resume recording
from that point.

FIXME: How does the user take control? A specific shortcut? Or any input using
the currently selected input device? For example UP or FIRE.

## Deterministic mode

FS-UAE runs in deterministic mode when recording or playback is enabled. This
means that FS-UAE tries to ensure that everything that happened during
recording happens the same way when playing back. This affects for example the
use of random numbers (which cannot be random in the playback) and real-time
clocks.

## State verification and desync

Desync will happen if the configuration used in the playback is not compatible
with the configuration used when recording - and can also if there are bugs
with the deterministic behavior, or if you use FS-UAE features which are not
compatible with deterministic mode.

The recording file includes checksums of the emulated state for each emulated
video frame. FS-UAE will use this to warn if the emulation is out of sync when
playing back the recording. If the warning is displayed, the playback is not
guaranteed to be identical to the recording.

If a desync warning is not displayed while playing back the recording, FS-UAE
more or less guarantees that the playback is identical to the recording.

## Limited support for savestates

When savestates are using during recording, the recording (up to that point)
is saved alongside the savestate. This means that you can load a previous
save, and continue recording from that point.

In order for this work reliably, the savestate saving and loading must be
deterministic, and the emulation state after load must be identical to the
pre-save state. This is generally not supported by UAE savestates, where tiny
differences in state causes slight timing differences, which in turn can cause
butterfly effects and the emulation gets out of sync. In other words, the
playback is not guaranteed to match the recording.

However, FS-UAE has additional support for deterministic savestates in some
configurations.

Supported configuration for using savestates with recording:

- Amiga 500 / 68000 cycle-exact
- Floppy images

Support for additional configurations can be added over time, for example
support for 68020 and directory hard drives should be fairly easy to do.

You can try to use savestates for other configurations, but this might cause
desyncs in playback. This _will_ be detected and warned about - but can only
be detected in playback mode and not while recording.

An additional warning about savestates: The complete contents of all media
(floppy images, hard drives) are not current stored in the savestate. This
means that you can get desyncs due to states being loaded/saved while the
media is modified. Whether this a problem depends on the game and exact usage
of saves and loads.
