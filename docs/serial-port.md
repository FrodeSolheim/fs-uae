# Serial port

FS-UAE includes emulation of the Amiga serial port. The emulation is at least
good enough for debugging purposes.

## Enabling TCP/IP serial port

You can specify an IP address and a port to listen to, and FS-UAE will create a
TCP/IP socket for the serial port. You can then connect to this socket to read
from / write to the emulated serial port.

To listen to port 1234 on localhost, you can use the following option:

    serial_port = tcp://127.0.0.1:1234

It is also possible to have FS-UAE wait during boot until you connect to the
serial port socket. This is useful in order to be sure to capture all the
serial port output. Append /wait to the option value to enable this:

    serial_port = tcp://127.0.0.1:1234/wait

## Connecting to a serial device

On Linux and OS X (and other Unix-like systems, probably), you can use the
following option to emulate an Amiga serial port using a real device:

    serial_port = /dev/something

Note: I haven't tested using a real serial port, so I don't know if that'll
actually work. But what have been tested is using a tool like `socat` to create
virtual devices. For example, you can run something like this:

    socat pty,raw,echo=0,link=/tmp/virtual-serial-port -,raw,echo=0

And then set up FS-UAE to connect to this:

    serial_port = /tmp/virtual-serial-port

### Getting an Amiga shell in an external terminal

If you have configured a virtual serial port using `socat` like in the above
example, you should be able to run the following command from within the Amiga:

    newshell aux:

In your host terminal you should see the CLI prompt and you can start using
this host-based Amiga shell.

> **_Note:_** You may need to first enable the aux device within AmigaOS. To do
> this, copy or move the `AUX` icon from `Storage/DOSDrivers` to
> `Devs/DOSDrivers`.
