from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import pygame
import fengestad.fs as fs

if fs.windows:
    from pygame.locals  import *
    from ctypes import windll
    user32      = windll.user32
    ShowWindow  = user32.ShowWindow
    IsZoomed    = user32.IsZoomed
    IsIconic    = user32.IsIconic
    SW_MAXIMIZE =   3
    SW_MINIMIZE =   6
    SW_RESTORE  =   9

    def getSDLWindow():
        return pygame.display.get_wm_info()["window"]

    def SDL_Maximize():
        return ShowWindow(getSDLWindow(), SW_MAXIMIZE)

    def SDL_Restore():
        return ShowWindow(getSDLWindow(), SW_RESTORE)

    def SDL_Minimize():
        return ShowWindow(getSDLWindow(), SW_MINIMIZE)

    def SDL_IsMaximized():
        return IsZoomed(getSDLWindow())

    def SDL_IsMinimized():
        return IsIconic(getSDLWindow())
else:
    def SDL_Maximize():
        pass

    def SDL_Restore():
        pass

    def SDL_Minimize():
        pass

    def SDL_IsMaximized():
        False

    def SDL_IsMinimized():
        False