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
is 'Synth input port (11883:0)' and 'Standard'.

Note, you can now also use `fs-uae-device-helper list-portmidi-devices` to find
the device names.

## Fluidsynth

The default fluidsynth sequencer name on Linux includes the process PID, which
will change each time fluidsynth is started. It is a good idea to run
fluidsynth with a static name, for example `--portname=Fluidsynth`:

    fluidsynth -a alsa -m alsa_seq -l -i /usr/share/sounds/sf2/FluidR3_GM.sf2 \
        -s --portname=Fluidsynth

You can then use the option `serial_port = midi:Fluidsynth`.

## Suggestions for future improvements

Split midi device name out from the serial port option? `serial_port_type` ?

    serial_port_type = serial|midi|midi-mt32 ?
    midi_device = xxx ?
    midi_mt32_device = yyy ?

## Credits

MIDI support via the PortMidi library contributed by Christian Vogelgsang.
