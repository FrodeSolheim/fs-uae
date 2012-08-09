from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import sys
import logging
import codecs
import platform
import threading

# using this lock to serialize logging from different threads
lock = threading.Lock()

class NullOutput(object):

    def flush(self):
        pass

    def isatty(self):
        return False

    def write(self, msg):
        pass

class MultiOutput:
    
    def __init__(self, *files):
        self.files = files

    def flush(self):
        with lock:
            for f in self.files:
                try:
                    f.flush()
                except Exception:
                    pass

    def isatty(self):
        return False

    def write(self, msg):
        with lock:
            for f in self.files:
                try:
                    f.write(msg)
                except Exception:
                    pass

class FileOutput(object):

    def __init__(self, file_obj):
        self.file = file_obj

    def flush(self):
        return self.file.flush()

    def isatty(self):
        return False

    def write(self, msg):
        if isinstance(msg, unicode):
            self.file.write(msg.encode("UTF-8"))
        else:
            self.file.write(msg)

class SafeOutput(object):

    def __init__(self, file_obj, in_charset, out_charset):
        try:
            self.writer = codecs.getwriter(out_charset)
        except LookupError:
            self.writer = codecs.getwriter('ASCII')
        self.in_charset = in_charset
        self.outfile = self.writer(file_obj, errors='replace')
        self.thread_local = threading.local()

    def flush(self):
        if hasattr(self.thread_local, 'write_func'):
            return
        try:
            if hasattr(self.outfile, 'flush'):
                self.outfile.flush()
        except Exception:
            pass

    def isatty(self):
        if hasattr(self.thread_local, 'write_func'):
            return False
        return self.outfile.isatty()

    def redirect_thread_output(self, write_func):
        if write_func is None:
            del self.thread_local.write_func
        else:
            self.thread_local.write_func = write_func

    def write(self, msg):
        if hasattr(self.thread_local, 'write_func'):
            if isinstance(msg, unicode):
                self.thread_local.write_func(msg)
                return
            msg = unicode(str(msg), self.in_charset, 'replace')
            self.thread_local.write_func(msg)
            return
        try:
            if isinstance(msg, unicode):
                self.outfile.write(msg)
            else:
                msg = unicode(str(msg), self.in_charset, 'replace')
                self.outfile.write(msg)
        except Exception:
            try:
                self.outfile.write(repr(msg))
            except Exception, e:
                self.outfile.write("EXCEPTION IN SAFEOUTPUT: %s\n" % repr(e))

def fix_output():
    sys._replaced_stdout = sys.stdout
    sys._replaced_stderr = sys.stderr
    if sys.platform == 'win32' and not os.environ.has_key('FS_FORCE_STDOUT'):
        if hasattr(sys, 'frozen'):
            sys.stdout = NullOutput()
            sys.stderr = NullOutput()
            return
        import win32console
        if win32console.GetConsoleWindow() == 0:
            # not running under console
            sys.stdout = NullOutput()
            sys.stderr = NullOutput()
            return
    try:
        if sys.platform == 'win32':
            sys.stdout = SafeOutput(sys.stdout, 'mbcs', 'cp437')
        else:
            sys.stdout = SafeOutput(sys.stdout, 'ISO-8859-1',
                    locale.getpreferredencoding())
    except Exception:
        sys.stdout = SafeOutput(sys.stdout, 'ISO-8859-1', 'ASCII')
    try:
        if sys.platform == 'win32':
            sys.stderr = SafeOutput(sys.stderr, 'mbcs', 'cp437')
        else:
            sys.stderr = SafeOutput(sys.stderr, 'ISO-8859-1',
                    locale.getpreferredencoding())
    except Exception:
        sys.stderr = SafeOutput(sys.stderr, 'ISO-8859-1', 'ASCII')

def setup_logging():
    from .Settings import Settings
    logs_dir = Settings.get_logs_dir()
    log_file = os.path.join(logs_dir, "Launcher.log.txt")
    try:
        f = open(log_file, "wb")
    except Exception:
        print("could not open log file")
        # use MultiOutput here too, for the mutex handling
        sys.stdout = MultiOutput(sys.stdout)
        sys.stderr = MultiOutput(sys.stderr)
    else:
        sys.stdout = MultiOutput(FileOutput(f), sys.stdout)
        sys.stderr = MultiOutput(FileOutput(f), sys.stderr)

    logging.basicConfig(stream=sys.stdout, level=logging.NOTSET)

def main():
    fix_output()
    if "--joystick-config" in sys.argv:
        print("importing pygame")
        import pygame
        print("initializing pygame")
        pygame.init()
        pygame.joystick.init()
        from fs_uae_launcher.JoystickConfigDialog import joystick_config_main
        return joystick_config_main()

    if "--server" in sys.argv:
        from fs_uae_launcher.server.game import run_server
        return run_server()

    setup_logging()

    from .Version import Version
    print("FS-UAE Launcher {0}".format(Version.VERSION))
    print("System: {0}".format(repr(platform.uname())))

    from .ConfigChecker import ConfigChecker
    config_checker = ConfigChecker()

    from .FSUAELauncher import FSUAELauncher
    application = FSUAELauncher()
    application.run()
    application.save_settings()
