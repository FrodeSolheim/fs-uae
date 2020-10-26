# MIDI support

> **Note:** This new MIDI support requires FS-UAE 3.9.0dev or newer.

Tested on Linux.
TODO: Test on macOS and Windows and document with some examples.

## Linux

You currently need to connect using the "name" of the sequencer with the option
`serial_port = midi:<name>`.

You can run `acconnect -o` to give a listing of ALSA MIDI sequencer names.
Example output while having both fluidsynth and munt/mt32 running:

    client 14: 'Midi Through' [type=kernel]
        0 'Midi Through Port-0'
    client 128: 'FLUID Synth (11883)' [type=user,pid=11883]
        0 'Synth input port (11883:0)'
    client 129: 'Munt MT-32' [type=user,pid=11907]
        0 'Standard  

The names you need to use with the `serial_port` option *with this exact setup*
is 'Synth input port (11883:0)' and 'Standard'.

### Fluidsynth

The default fluidsynth sequencer name includes the process PID, which will
change each time fluidsynth is started. It is a good idea to run fluidsynth
with a static name, for example `--portname=Fluidsynth`:

    fluidsynth -a alsa -m alsa_seq -l -i /usr/share/sounds/sf2/FluidR3_GM.sf2 \
        -s --portname=Fluidsynth

You can the use the option `serial_port = midi:Standard`.

### Munt / MT32

With `mt32emu-qt` from Munt on Linux, use option `serial_port = midi:Standard`
to enable MIDI output via emulated serial port and route to the ALSA sequencer
set up by mt32emu-qt.

## Credits

MIDI support via the PortMidi library contributed by Christian Vogelgsang.
