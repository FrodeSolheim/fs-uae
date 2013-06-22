from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import traceback
from threading import Thread as ThreadBase
from fengestad.base.Application import Application

class StopException:
	pass

class Thread(ThreadBase):

    def __init__(self):
        ThreadBase.__init__(self)
        self.__stop_flag = False

    def set_stop_flag(self):
    	self.__stop_flag = True

    def stop_check(self):
    	if self.__stop_flag:
    		raise StopException()
    	if Application.get().is_stopping():
    		raise StopException()

    def run(self):
    	try:
    		self.thread_function()
    	except StopException:
    		print("thread", self, "stopped with StopException")
    	except Exception:
    		traceback.print_exc()

    def thread_function(self):
    	pass
