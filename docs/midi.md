# MIDI support

> **Note:** This new MIDI support requires FS-UAE 4.0.0 or newer.

You currently need to specify the MIDI device you want to connect to using the
`serial_port` option and the "name" of the sequencer, like this:

    serial_port = midi:<name>

> **Note:** The method of configuring MIDI and/or choosing MIDI device might
> change in future versions of FS-UAE!

## Finding the MIDI device name

You can list the midi device names with fs-uae-device-helper:

    ./fs-uae-device-helper list-portmidi-devices

If you are using FS-UAE Launcher, you can go to Prefs -> MIDI and this will
also list the MIDI device names for you!

## Using Munt for MT-32 emulation

With `mt32emu-qt` from Munt on Linux, use option `serial_port = midi:Standard`
to enable MIDI output via emulated serial port and route to the ALSA sequencer
set up by mt32emu-qt.

On macOS, the MIDI device is different, so you can use the option
`serial_port = midi:Mt32EmuPort`.

On Windows, yet another name is used:
`serial_port = midi:Mt-32 Synth Emulator`.

But double-check that the device name is correct for yor system. See the
section "Finding the MIDI device name".

For FS-UAE 4.0.XXX FIXME:

Alternatively, you can split the option into two:

    serial_port = mt32:
    mt32_device = Mt-32 Synth Emulator

You can also in many cases drop specifying `mt32_device`. In this case, FS-UAE
will try the following devices depending on platform: "Standard" (Linux), "Mt32EmuPort" (macOS), "Mt-32 Synth Emulator" (Windows).

## Linux

You can run `acconnect -o` to give a listing of ALSA MIDI sequencer names.
Example output while having both fluidsynth and munt/mt32 running:

    client 14: 'Midi Through' [type=kernel]
        0 'Midi Through Port-0'
    client 128: 'FLUID Synth (11883)' [type=user,pid=11883]
        0 'Synth input port (11883:0)'
    client 129: 'Munt MT-32' [type=user,pid=11907]
        0 'Standard

The names you need to use with the `serial_port` option _with this exact setup_
is 'Synth input port (11883:0)' and 'Standard'. You might want to rename the
port name of FluidSynth. See the "FluidSynth" section for more information.

Note, you can now also use `fs-uae-device-helper list-portmidi-devices` to find
the device names.

## FluidSynth

The default FluidSynth sequencer name on Linux includes the process PID, which
will change each time FluidSynth is started. It is a good idea to run
FluidSynth with a static name, for example `--portname=FluidSynth`:

    fluidsynth -a alsa -m alsa_seq -l -i -s --portname=FluidSynth \
    /usr/share/sounds/sf2/FluidR3_GM.sf2

In this case, the device will always remain the same, and you can then use the
option `serial_port = midi:FluidSynth` and it will work across reboots.

On macOS, a relatively easy way to get fluidsynth installed is to install it
via Homebrew:

    brew install fluidsynth

You need a soundfount as well. A good choice is FluidR3_GM.sf2.
FIXME: Download link for FluidR3_GM.zip on fs-uae.net

    fluidsynth -l -i -s --portname=FluidSynth FluidR3_GM.sf2

## Suggestions for future improvements

Split midi device name out from the serial port option? `serial_port_type` ?

    serial_port_type = serial|midi|mt32 ?
    midi_device = xxx ?
    mt32_device = yyy ?

Maybe also allow `midi_device = fluidsynth` and dynamically try to deduce the
port name ("Synth input port" on Linux, etc). Even more, with midi_device
missing, perhaps try to default to FluidSynth on Linux (if found), the default
software midi sequencer on Windows, and (???) on macOS?

### Package FluidSynth as a plugin?

Maybe include as Utilities/FluidSynth? Soundfonts can be included in the
FluidSynth package.

### Notes

Default device names (process numbers are examples):

Fluidsynth:

    Linux: Synth input port (547769:0)
    macOS: FluidSynth virtual port (17220)

MT32Emu-qt:

    Linux: Standard
    macOS: Mt32EmuPort

## Credits

MIDI support via the PortMidi library contributed by Christian Vogelgsang.
