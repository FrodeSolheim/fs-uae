from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
import threading
import traceback
from .Signal import Signal

main_thread_id = threading.current_thread().ident
already_run = False


class Debug:

    @classmethod
    def is_running_in_debugger(cls):
        return bool(sys.gettrace())

    @classmethod
    def enable_exception_handler(cls, force=False):
        global already_run
        if already_run:
            return
        already_run = True

        if not force and cls.is_running_in_debugger():
            print("running in debugger, not enabling exception handler "
                  "after all.")
            cls._let_thread_exceptions_be_unhandled()
            return False

        print("enabling except hook")
        sys.excepthook = handle_exception
        cls._enable_thread_exception_handler()
        return True

    @classmethod
    def _let_thread_exceptions_be_unhandled(cls):
        """Replace method in Thread class to let exceptions be picked up by
        the debugger. This is done by replacing the bootstrap function,
        which normally catches all exceptions."""

        print("let thread exceptions be unhandled (for debugger)")
        if sys.version.startswith("2.7"):
            threading.Thread._Thread__bootstrap = _Thread__bootstrap_2_7
        else:
            print("WARNING: no Thread bootstrap replacement for this "
                  "python version. The debugger will not break on unhandled"
                  "exceptions in threads other than the main thread.")

    @classmethod
    def _enable_thread_exception_handler(cls):
        print("enable tread exception handler")

        def run():
            self = threading.current_thread()
            try:
                self._run_()
            except:
                handle_exception(*sys.exc_info())

        def set_ident(self):
            #self = threading.current_thread()
            self._run_ = self.run
            self.run = run
            self._set_ident_()

        threading.Thread._set_ident_ = threading.Thread._set_ident
        threading.Thread._set_ident = set_ident


def handle_exception(exc_type, exc_value, exc_traceback):
    if issubclass(exc_type, KeyboardInterrupt):
        print("handle_exception: KeyboardInterrupt caught")
        sys.exit(1)

    filename, line, function, dummy = traceback.extract_tb(exc_traceback).pop()
    try:
        filename = filename.decode(sys.getfilesystemencoding())
    except:
        pass
    try:
        filename = unicode(filename)
    except:
        pass
    filename = os.path.basename(filename)
    thread = threading.currentThread()
    error_id = "{0}:{1}:{2}:{3}".format(
        exc_type.__name__, filename, function, line)

    #QtGui.QMessageBox.critical(None, "Error",
    #description = "<html>A critical error has occured.<br/> "
    #                           + "<b>%s</b><br/><br/>" % error
    #                           + "It occurred at <b>line %d</b> of file "
    #                             "<b>%s</b>.<br/>" % (
    #                           line, filename)
    #                           + "</html>")
    #

    backtrace_string = "".join(traceback.format_exception(
        exc_type, exc_value, exc_traceback))

    message = "\nUnhandled exception detected in thread {0}:\n  {1}\n".format(
        thread.getName(), error_id)

    print(message)
    print(backtrace_string)

    print(message, file=sys.stderr)
    print(backtrace_string, file=sys.stderr)

    if thread.ident == main_thread_id:
        try:
            import wx
            app = wx.GetApp()
            if app is not None:
                wx.MessageBox(message + "\n" + backtrace_string)
                if app.IsMainLoopRunning():
                    return
                print("The application will now close due to the above error")
                print("calling app.Exit")
                app.Exit()

                Signal("quit").notify()
        except Exception, e:
            print(repr(e))

        #sys.exit(1)


class AdditionalInfo(object):
    pass


def _Thread__bootstrap_2_7(self):
    """This is a replacement "method" for the Thread class in Python 2.7,
    designed to let an exception fall through to the debugger."""

    from threading import _active_limbo_lock, _active, _limbo, _trace_hook
    from threading import _profile_hook, _sys, _get_ident

    try:
        self._set_ident()
        self._Thread__started.set()
        with _active_limbo_lock:
            _active[self._Thread__ident] = self
            del _limbo[self]
        if __debug__:
            self._note("%s.__bootstrap(): thread started", self)

        #if _trace_hook:
        #    self._note("%s.__bootstrap(): registering trace hook", self)
        #    _sys.settrace(_trace_hook)
        if _profile_hook:
            self._note("%s.__bootstrap(): registering profile hook", self)
            _sys.setprofile(_profile_hook)

        try:
            self.run()
        except SystemExit:
            if __debug__:
                self._note("%s.__bootstrap(): raised SystemExit", self)
        else:
            if __debug__:
                self._note("%s.__bootstrap(): normal return", self)
        finally:
            # Prevent a race in
            # test_threading.test_no_refcycle_through_target when
            # the exception keeps the target alive past when we
            # assert that it's dead.
            self._Thread__exc_clear()
    finally:
        with _active_limbo_lock:
            self._Thread__stop()
            try:
                # We don't call self.__delete() because it also
                # grabs _active_limbo_lock.
                del _active[_get_ident()]
            except:
                pass
