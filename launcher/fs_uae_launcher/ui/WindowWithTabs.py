from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ..I18N import _, ngettext
from .TabPanel import TabPanel
from .TabButton import TabButton
from .Constants import Constants
from .Skin import Skin

class WindowWithTabs(fsui.Window):

    def __init__(self, parent, title):
        fsui.Window.__init__(self, parent, title)
        Skin.set_background_color(self)

        if fsui.System.macosx:
            import wx
            self.toolbar = self.CreateToolBar(wx.TB_HORIZONTAL | wx.NO_BORDER
                    | wx.TB_FLAT)
            #        | wx.TB_FLAT | wx.TB_TEXT)
            self.toolbar.SetMargins((0, 0))
            self.toolbar.SetToolBitmapSize((32, 32))
            self.toolbar.Realize()
            def function():
                return self.toolbar
            self.toolbar.get_container = function
            self.tab_panel = None
        else:
            self.toolbar = None
            self.tab_panel = TabPanel(self)

        self.layout = fsui.VerticalLayout()
        if self.tab_panel:
            self.layout.add(self.tab_panel, fill=True)
        self.current_tab_group_id = 0
        self.tab_groups = [[]]
        if self.toolbar:
            self.Bind(wx.EVT_SIZE, self.__size_event)

    def __size_event(self, event):
        size = event.GetSize()
        print("WindowWithTabs size event, size =", size)
        """
        width = size[0]
        remaining_width = width
        expandables = 0
        for child in self.toolbar.GetChildren():
            print(child)
            if hasattr(child, "expandable") and child.expandable:
                expandables += 1
            else:
                w = child.GetSize()[0]
                print(w)
                remaining_width -= w
        for i, child in enumerate(self.toolbar.GetChildren()):
            if hasattr(child, "expandable"):
                if child.expandable:
                    width = remaining_width // expandables
                    remaining_width -= width
                    expandables -= 1
                    #self.toolbar.RemoveTool(child.GetId())
                    #self.toolbar.RemoveChild(child)
                    child.SetSize((width, 46))
                    child.SetSize((200, 46))
                    self.toolbar.InsertControl(i, child)
                    print("setting size to, ", width)
        self.toolbar.Realize()
        """
        event.Skip()

    def realize_tabs(self):
        if self.toolbar:
            self.toolbar.Realize()

    def new_tab_group(self):
        self.current_tab_group_id += 1
        self.tab_groups.append([])

    def set_content(self, content):
        self.layout.add(content, expand=True, fill=True)

    def select_tab(self, index, group):
        if self.toolbar:
            pass
        else:
            print("\n\n\nselect tab", index, group)
            self.tab_groups[group][index].select()
            #self.tab_groups[group].select_tab(index)

    def add_tab(self, function, icon, title="", tooltip=""):
        if not tooltip:
            tooltip = title
        if self.toolbar:
            import wx
            self.add_toolbar_spacer(2)
            tool_id = wx.NewId()
            self.toolbar.AddLabelTool(tool_id, title, icon.bitmap,
                    wx.NullBitmap, 0, tooltip)
            def event_handler(event):
                function()
            self.toolbar.Bind(wx.EVT_TOOL, event_handler, id=tool_id)
            self.add_toolbar_spacer(3)
            self.tab_groups[self.current_tab_group_id].append(tool_id)
        else:
            button = TabButton(self.tab_panel, icon)
            button.set_tooltip(tooltip)
            button.group_id = self.current_tab_group_id
            button.on_select = function
            self.tab_panel.add(button)
            self.tab_groups[self.current_tab_group_id].append(button)

    def add_tab_button(self, function, icon, title="", tooltip="",
            menu_function=None, left_padding=0, right_padding=0):
        if not tooltip:
            tooltip = title
        if self.toolbar:
            import wx
            self.add_toolbar_spacer(2)
            tool_id = wx.NewId()
            self.toolbar.AddLabelTool(tool_id, title, icon.bitmap,
                    wx.NullBitmap, 0, tooltip)
            def event_handler(event):
                if function:
                    function()
                else:
                    menu_function()
            self.toolbar.Bind(wx.EVT_TOOL, event_handler, id=tool_id)
            self.add_toolbar_spacer(3)
            self.tab_groups[self.current_tab_group_id].append(tool_id)
        else:
            button = TabButton(self.tab_panel, icon,
                    type=TabButton.TYPE_BUTTON, left_padding=left_padding,
                    right_padding=right_padding)
            button.set_tooltip(tooltip)
            button.group_id = self.current_tab_group_id
            if function:
                button.on_activate = function
            elif menu_function:
                def menu_wrapper():
                    menu_function()
                    button.check_hover()
                button.on_left_down = menu_wrapper
            self.tab_panel.add(button)
            self.tab_groups[self.current_tab_group_id].append(button)
            return button

    def add_tab_panel(self, class_, min_width=0):
        if self.toolbar:
            panel = class_(self.toolbar, toolbar_mode=True)
        else:
            #panel = class_(self.tab_panel, padding_bottom=2)
            panel = class_(self.tab_panel)
        panel.expandable = True
        panel.set_min_height(Constants.TAB_HEIGHT)
        if self.toolbar:
            panel.SetSize((min_width, 46))
            self.toolbar.AddControl(panel)
        else:
            self.tab_panel.add(panel, expand=1000000)

    def add_toolbar_spacer(self, width, height=46):
        import wx
        spacer_control = wx.Control(self.toolbar, size=(width, height))
        self.toolbar.AddControl(spacer_control)

    def add_tab_separator(self):
        if self.toolbar:
            self.toolbar.AddSeparator()

    def add_tab_spacer(self, spacer=0, expand=False):
        if self.toolbar:
            import wx
            spacer_control = wx.Control(self.toolbar, size=(spacer, 1))
            self.toolbar.AddControl(spacer_control)
        else:
            self.tab_panel.add_spacer(spacer, expand=expand)
