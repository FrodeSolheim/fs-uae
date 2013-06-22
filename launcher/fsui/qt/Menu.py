from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QMenu


class Menu:

    def __init__(self):
        self.qmenu = QMenu()
        #self._menu = wx.Menu()
        #self._ids = []
        ##self._functions = []
        pass

    def add_item(self, text, function, item_id=-1):
        text = text.replace("&", "&&")

        action = self.qmenu.addAction(text)
        action.triggered.connect(function)
    	
    	#if item_id == -1:
    	#	item_id = wx.NewId()
    	#self._ids.append(item_id)
        ##self._functions.append(function)
        #self._menu.Append(item_id, text)
        #def function_wrapper(event):
        #	# the reason we use CallAfter here is so control will return
        #	# to the application after having popped up a menu, so the
        #	# button opening a menu can draw its correct state when the
        #	# menu is closed, before for example a modal dialog is opened.
        #	wx.CallAfter(function)
        #self._menu.Bind(wx.EVT_MENU, function_wrapper, id=item_id)

    def add_about_item(self, text, function):
        self.add_item(text, function)

    def add_preferences_item(self, text, function):
        self.add_item(text, function)

    def add_separator(self):
    	self.qmenu.addSeparator()
