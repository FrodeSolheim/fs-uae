from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import time
import subprocess
import threading
from fengestad import fs


class Joystick:

    if fs.windows:
        @classmethod
        def _get_joystick_hash(cls):
            # FIXME: WARNING: DISABLING FOR NOW, SINCE STARTING THIS CONSOLE APPLICATION
            # CAUSES NOTICABLE DELAYS (AND AN HOURGLASS)
            # - Try to spawn a "server" communicating with us via STDOUT, and let the server
            # periodically start a joystick polling app.
            return hash("FIXME")
            # using an external app here, because windows seems to cache device
            # information while the program is running...
            path = os.path.join(pyapp.plugins.get_plugin(
                    'no.fengestad.input.platform').get_bin_dir(),
                    'dinput8.exe')
            # Must specify all files (stdin/stdout/stderr) when running in GUI mode
            # on Windows

            si = subprocess.STARTUPINFO()
            si.dwFlags = subprocess.STARTF_USESHOWWINDOW
            si.wShowWindow = subprocess.SW_HIDE

            p = subprocess.Popen([path], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT, startupinfo=si)
            p.stdin.close()
            #lines = 0
            #for line in p.stdout.readlines():
            #    if line[0] != '\n' and line[0] != '\r':
            #        lines += 1
            ## dinput8.exe prints two lines per device
            #return lines // 2
            data = p.stdout.read()
            #print(data)
            return hash(data)

    elif fs.macosx:
        @classmethod
        def _get_joystick_hash(cls):
            # FIXME: Implement
            #return hash(0)
            return hash("FIXME")

    else:
        @classmethod
        def _get_joystick_hash(cls):
            if os.path.exists('/dev/input'):
                return hash(tuple(os.listdir('/dev/input')))
            return hash("FIXME")

    _hash = None

    @classmethod
    def get_joystick_hash(cls):
        #while cls._hash is None:
        #    time.sleep(0.01)
        if cls._hash is None:
            cls._hash = cls._get_joystick_hash()
            #print("FIXME: not starting thread in get_joystick_hash")
            t = threading.Thread(target=cls.thread)
            t.daemon = True
            t.start()
        return cls._hash

    @classmethod
    def thread(cls):
        try:
            cls._thread()
        except Exception:
            import traceback
            traceback.print_exc()

    @classmethod
    def _thread(cls):
        while True:
            time.sleep(1.0)
            cls._hash = cls._get_joystick_hash()

