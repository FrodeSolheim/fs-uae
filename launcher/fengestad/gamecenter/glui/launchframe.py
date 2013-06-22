from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division

import os
import wx
import threading
import pyapp
from fengestad import fs
#from fengestad.gamecenter.glui.launcher import GameLauncher
from fengestad.gamecenter.database import Database
from fengestad.gamecenter.gamecontext import GameContext
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.glui.settings import Settings
from fengestad.gamecenter.resources import resources, logger, ngettext, _


C1 = 0xd0
C2 = 0xa0
TEXT_C1 = 0xa0
TEXT_C2 = 0xd0


class InfoFrame(wx.Frame):

    def __init__(self, parent, winid, title, pos=(-1, -1), size=(-1, -1),
                style=wx.DEFAULT_FRAME_STYLE):
        print "InfoFrame.__init__"
        wx.Frame.__init__(self, parent, winid, title, pos=pos, size=size,
                style=style)

        self.SetBackgroundColour(wx.Colour(0xa0, 0xa0, 0xa0))
        self.SetBackgroundStyle(wx.BG_STYLE_CUSTOM)
        self.SetCursor(wx.StockCursor(wx.CURSOR_BLANK))
        wx.SetCursor(wx.StockCursor(wx.CURSOR_BLANK))

        self.Bind(wx.EVT_PAINT, self.__paint_event)
        self.Bind(wx.EVT_ERASE_BACKGROUND, self.__erase_event)

        #lpos = (0, 0.39 * size[1])

        useheight = size[0] / max (size[0] / size[1], 4 / 3)
        yoffset = (size[1] - useheight) // 2

        lpos = (0.5 * size[0], yoffset + 0.44 * useheight)
        lsize = (0.5 * size[0], 0.05 * useheight)
        lstyle = 0 # wx.ALIGN_CENTRE

        #self.label1 = wx.StaticText(self, -1, "PREPARING",
        #        pos=lpos, size=lsize, style=lstyle)
        #self.label1.SetBackgroundColour(wx.Colour(C1, C1, C1))
        #self.label1.SetForegroundColour(wx.Colour(0x77, 0x77, 0x77))
        #font = wx.FFontFromPixelSize((0, 0.05 * useheight),
        #        wx.FONTFAMILY_SWISS, wx.FONTFLAG_BOLD)
        #self.label1.SetFont(font)

        #lpos = (0, 0.51 * size[1])
        lpos = (0.5 * size[0], yoffset + 0.51 * useheight)

        #self.label2 = wx.StaticText(self, -1, "Please Wait",
        #        pos=lpos, size=lsize, style=lstyle)
        #self.label2.SetBackgroundColour(wx.Colour(C2, C2, C2))
        #self.label2.SetFont(font)
        #self.label2.SetForegroundColour(wx.Colour(0xdd, 0xdd, 0xdd))

        self.status1 = "Preparing"
        self.status2 = "Please Wait"
        self.status1_rendered = (None, None, None)
        self.status2_rendered = (None, None, None)

    def __erase_event(self, event):
        print self, "__erase_event"
        if self.background:
            event.GetDC().DrawBitmap(self.background, 0, 0)
        else:
            event.Skip()

    def __paint_event(self, event):
        dc = wx.PaintDC(self)
        self._paint(dc)


    def _paint(self, dc):
        print "InfoFrame._paint"
        if self.status1_rendered[0] != self.status1:
            rendered = self.status_font.render(self.status1, True,
                    (TEXT_C1, TEXT_C1, TEXT_C1), (C1, C1, C1))
            txtsize = rendered.get_size()
            txtdata = pygame.image.tostring(rendered, "RGB")
            txtdata = wx.ImageFromData(txtsize[0], txtsize[1], txtdata)
            txtdata = txtdata.ConvertToBitmap()
            self.status1_rendered = (self.status1, txtsize, txtdata)
        else:
            text, txtsize, txtdata = self.status1_rendered
        x = self.GetClientSize()[0] // 2
        y = self.GetClientSize()[1] // 2 - txtsize[1] - \
                int(self.GetClientSize()[1] * 0.01)
        dc.DrawBitmap(txtdata, x, y)

        if self.status2_rendered[0] != self.status2:
            rendered = self.status_font.render(self.status2, True,
                    (TEXT_C2, TEXT_C2, TEXT_C2), (C2, C2, C2))
            txtsize = rendered.get_size()
            txtdata = pygame.image.tostring(rendered, "RGB")
            txtdata = wx.ImageFromData(txtsize[0], txtsize[1], txtdata)
            txtdata = txtdata.ConvertToBitmap()
            self.status2_rendered = (self.status2, txtsize, txtdata)
        else:
            text, txtsize, txtdata = self.status2_rendered
        x = self.GetClientSize()[0] // 2
        y = self.GetClientSize()[1] // 2 + \
                int(self.GetClientSize()[1] * 0.01)
        dc.DrawBitmap(txtdata, x, y)

    def show(self):
        print self, "show"
        if self.fullscreen:
            logger.debug("ShowFullScreen?")
            #self.Show()
            self.ShowFullScreen(True, wx.FULLSCREEN_ALL)
            logger.debug("ShowFullScreen (done)")
        else:
            self.Show()
        #self.SetCursor(wx.StockCursor(wx.CURSOR_BLANK))
        #self.SetCursor(wx.StockCursor(wx.CURSOR_SIZENS))


class LaunchBackFrame(InfoFrame):
    def __init__(self, pos=(-1, -1), size=(640, 480), fullscreen=False,
                background=None, stay_on_top=False):
        #size = (800, 600)
        if fullscreen:
            #style = wx.DEFAULT_FRAME_STYLE
            style = 0
            pos = (0, 0)
            size = wx.GetDisplaySize()
        else:
            style = wx.DEFAULT_FRAME_STYLE & ~wx.RESIZE_BORDER
        if stay_on_top:
           style = style | wx.STAY_ON_TOP
        InfoFrame.__init__(self, None, -1, "", pos=pos, size=size,
                style=style)
        self.background = background
        #self.SetBackgroundColour(wx.Colour(0x0, 0x0, 0x0))
        #self.SetBackgroundStyle(wx.BG_STYLE_CUSTOM)
        self.fullscreen=fullscreen
        if not self.fullscreen:
            self.SetClientSize(size)

        #self.label1.SetLabel("BUSY")
        #self.label2.SetLabel("Please Wait")

        self.status1 = "Busy"
        self.status2 = "Please Wait"

#    def show(self):
#        if self.fullscreen:
#            logger.debug("ShowFullScreen?")
#            #self.Show()
#            self.ShowFullScreen(True, wx.FULLSCREEN_ALL)
#            logger.debug("ShowFullScreen (done)")
#        else:
#            self.Show()


class LaunchFrame(InfoFrame):

    def __init__(self, game_info, pos=(-1, -1), size=(640, 480),
            fullscreen=False, on_visible=None, background=None):
        print "LaunchFrame.__init__"
        if fullscreen:
            #style = wx.DEFAULT_FRAME_STYLE |
            style = 0 # | wx.STAY_ON_TOP
            pos = (0, 0)
            size = wx.GetDisplaySize()
        else:
            style = wx.DEFAULT_FRAME_STYLE & ~wx.RESIZE_BORDER# | wx.STAY_ON_TOP
        #style = wx.DEFAULT_FRAME_STYLE
        style = style | wx.STAY_ON_TOP
        style = style | wx.FRAME_SHAPED
        InfoFrame.__init__(self, None, -1, "Launching Game", pos=pos,
                size=size, style=style)
        self.game_info = game_info
        self.Bind(wx.EVT_PAINT, self.__paint_event)
        self.Bind(wx.EVT_CHAR, self.__char_event)

        #if wx.Platform == "__WXGTK__":
        #    self.Bind(wx.EVT_WINDOW_CREATE, self.__window_create_event)


        global window_size
        global window_pos
        global window_background
        global window_fullscreen
        window_size = size
        window_pos = pos
        window_background = background
        window_fullscreen = fullscreen


        self.background = background

        if fs.windows:
            pass
        else:
            # Panel to receive key presses on wxGTK
            panel = wx.Panel(self, pos=(0, 0), size=(10, 10))
            panel.SetBackgroundColour(wx.Colour(0xd0, 0xd0, 0xd0))
            panel.Bind(wx.EVT_CHAR, self.__char_event)
            panel.SetFocus()
            # Add one more (quick hack -- to make sure the panel does not
            # get resized to cover the entire frame -- would prevent
            # paint events to the frame)
            panel = wx.Panel(self, pos=(10, 0), size=(10, 10))
            panel.SetBackgroundColour(wx.Colour(0xd0, 0xd0, 0xd0))
            panel.Bind(wx.EVT_CHAR, self.__char_event)
            panel.SetFocus()

        #self.Bind(wx.EVT_ERASE_BACKGROUND, self.__erase_event)

        #self.Bind(wx.EVT_IDLE, self.__idle_event)
        #self.Bind(wx.EVT_TIMER, self.__hide_timer_event)
        #self.Bind(wx.EVT_TIMER, self.__start_timer_event)
        #self.is_initialized = False
        self._initialize_scheduled = False
        #self.hide_timer = wx.Timer(self)
        #self.start_timer = wx.Timer(self)
        self.on_visible = on_visible
        self.error_occured = False
        self.fullscreen = fullscreen
        #self.back_frame = BackFrame()

        #if not fs.windows:
        #    from gtk import gdk
        #    gdk.window_set_back_pixmap(GetHandle ()->window, NULL, false);
        if not self.fullscreen:
            self.SetClientSize(size)

    #def __window_create_event(self, event):
    #    self.SetShape(wx.Region(0, 0, 10, 10))

#    def show(self, allow_hack=False):
#
#        if allow_hack and fs.linux:
#            # The point is to force the bitmap to be cached on the xserver
#            def paint_handler(event):
#                dc = wx.PaintDC(frame)
#                dc.DrawBitmap(self.background, 0, 0)
#                # And now display the real frame
#                wx.CallLater(2000, frame.Destroy)
#                wx.CallLater(3000, self.show, allow_hack=False)
#            frame = wx.Frame(None, pos=(10000, 0), size=(100, 100), style=0)
#            frame.Bind(wx.EVT_PAINT, paint_handler)
#            frame.Show()
#        else:
#            InfoFrame.show(self)
        print "LaunchFrame.__init__ is done"


    def _initialize(self):
        logger.debug("LaunchFrame._initialize")
        #self.is_initialized = True
        if self.on_visible:
            self.on_visible()
        logger.debug("CallLater LaunchFrame.start_game")
        #self.start_timer.Start(1500, wx.TIMER_ONE_SHOT)

        #if not self.fullscreen:
        #    print self.ClientToScreen((0, 0))
        #    #os.environ["SDL_VIDEO_WINDOW_POS"] = "%d,%d" % \
        #    #        tuple(self.ClientToScreen((0, 0)))
        #    if fs.windows:
        #        pass
        #    else:
        #        os.environ["SDL_VIDEO_WINDOW_POS"] = "0,0"

        wx.CallLater(1500, self.start_game)

    def __paint_event(self, event):
        print "LaunchFrame.__paint_event"
        dc = wx.PaintDC(self)
        if not self._initialize_scheduled:
            wx.CallLater(200, self._initialize)
            self._initialize_scheduled = True
        self._paint(dc)
        #self.SetCursor(wx.StockCursor(wx.CURSOR_BLANK))

    #def __erase_event(self, event):
    #    if self.background:
    #        event.GetDC().DrawBitmap(self.background, 0, 0)
    #    else:
    #        event.Skip()

    #def __hide_timer_event(self, event):
    #    self.Close()

    #def __start_timer_event(self, event):
    #    print "..."
    #    self.start_game()

    def __char_event(self, event):
        print "__char_event"
        if self.error_occured:
            self.hide()
            #wx.CallLater(100, wx.GetApp().ExitMainLoop)
            exit_sequence()
        else:
            event.Skip()

    #    def __idle_event(self, event):
    #    pass

    def hide(self):
        ##self.launcher.on_exit.disconnect(self.on_exit)
        #self.launcher.on_start.disconnect(self.on_start)
        #self.launcher.on_error.disconnect(self.on_error)
        #self.launcher.on_status.disconnect(self.on_status)
        self.Close()

    def on_exit(self):
        print "Game stopped"
        print "Closing launch frame"
        exit_sequence()
        #self.Close()

    @pyapp.thread.runs_in_main_blocking
    def on_status(self, status="", sub_status="", **kwargs):
        print "on_status", status, sub_status
        #import traceback
        #traceback.print_stack()
        if status:
            #self.label1.SetLabel(status)
            self.status1 = status#.upper()
        #self.label2.SetLabel(sub_status)
        self.status2 = sub_status
        self.Refresh(True)

    @pyapp.thread.runs_in_main_blocking
    def on_error(self, message="", **kwargs):
        print "on_error", message
        #self.label1.SetLabel("ERROR - PRESS ANY KEY")
        #self.label2.SetLabel(message)
        self.status1 = "ERROR (PRESS ANY KEY)"
        self.status2 = message
        self.error_occured = True
        self.Refresh(True)

    @pyapp.thread.runs_in_main_blocking
    def on_start(self, **kwargs):
        print "on_start"
        wx.CallLater(2000, self.hide)

    def start_game(self):
        logger.debug("LaunchFrame.start_game")
        #self.hide_timer.Start(3000, wx.TIMER_CONTINUOUS)

        # FIXME: CONFIGURATION
        # FIXME: USE GAME ID
        Database.cursor.execute("SELECT path FROM File, Game, GameFile "
                "WHERE File.id = GameFile.file AND "
                "GameFile.game = Game.id AND Game.name = ? AND "
                "Game.platform = ?", (self.game_info["name"],
                self.game_info["platform"]))
        row = Database.cursor.fetchone()
        if row:
            game_file = row[0]
            context = GameContext(game_file)
            threading.Thread(target=self._start_game, args=(context,)).start()
        else:
            pass
            # FIXME: COULD NOT RUN GAME

        #self.launcher = GameLauncher.get_launcher_for_game(self.game_info)
        #self.launcher.on_exit.connect(self.on_exit)
        #self.launcher.on_start.connect(self.on_start)
        #self.launcher.on_error.connect(self.on_error)
        #self.launcher.on_status.connect(self.on_status)
        #self.launcher.run(fullscreen=Settings.fullscreen_game)

    def _start_game(self, context):
        controller = GameController.create_for_game_context(context)
        configuration = {}
        controller.run_game(configuration)


def exit_sequence():
    fgframe = LaunchBackFrame(pos=window_pos, size=window_size,
            background=window_background, fullscreen=window_fullscreen,
            stay_on_top=True)
    fgframe.show()
    def destroy_fgframe():
        try:
            fgframe.Destroy()
        except Exception:
            pass
    wx.CallLater(1600, destroy_fgframe)
    wx.CallLater(1500, wx.GetApp().ExitMainLoop)

