from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import time
import pygame
from fengestad.gamecenter.config import Config
from .font import Font, BitmappedFont
from .items import MenuItem
from .constants import TOP_ITEM_LEFT, TOP_WIDTH
from .navigatable import Navigatable
from .render import Render
from .opengl import fs_emu_blending
from .state import State
from .sdl import *
from .texture import Texture
from fengestad.gamecenter.resources import resources, logger, ngettext, _
from .constants import TOP_ITEM_NOBORDER


class TopMenuItem(MenuItem):

    def update_size_right(self):
        self.w = 80

    def render_top_right(self, selected=False):
        self.render_top_background(selected, style=TOP_ITEM_LEFT)
        fs_emu_blending(True)
        if selected:
            texture = self.selected_texture
        else:
            texture = self.normal_texture
        texture.render(self.x, self.y, self.w, self.h)


class CloseItem(TopMenuItem):
    def __init__(self):
        TopMenuItem.__init__(self)
        self.normal_texture = Texture.close
        self.selected_texture = Texture.close_selected

    def activate(self, menu):
        pygame.event.post(pygame.event.Event(pygame.QUIT))


class ShutdownItem(TopMenuItem):
    def __init__(self):
        TopMenuItem.__init__(self)
        self.normal_texture = Texture.shutdown
        self.selected_texture = Texture.shutdown_selected

    def activate(self, menu):
        command = Config.get("command/shutdown", "").strip()
        if command:
            os.system(command)
        pygame.event.post(pygame.event.Event(pygame.QUIT))


class MaximizeItem(TopMenuItem):
    def __init__(self):
        MenuItem.__init__(self)
        self.title = "Ma"


class MinimizeItem(TopMenuItem):
    def __init__(self):
        MenuItem.__init__(self)
        self.normal_texture = Texture.minimize
        self.selected_texture = Texture.minimize_selected

    def activate(self, menu):
        SDL_Minimize()
        # want item menu focused when window is restored
        set_current_menu(State.current_menu)


class ClockItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("00:00")
        self.enabled = False

    def update_size(self, text):
        #w, h = BitmappedFont.title_font.measure(text)
        #self.w = w + 40 # + 8
        #self.w = 134
        self.w = 148
        #print(self.w)

    def render_top_right(self, selected=False):
        self.render_top_background(selected, style=TOP_ITEM_NOBORDER)
        self.render_top(self.get_top_right_text(), selected, right_align=True)

    def get_top_right_text(self):
        return time.strftime("%H:%M")

    def activate(self, menu):
        pass


class TopMenu(Navigatable):

    def __init__(self):
        self.left = []
        self.right = []
        self.right.append(ClockItem())
        #if Render.allow_minimize:
        #    if Config.get_bool("menu/minimize", True):
        #        self.right.append(MinimizeItem())
        command = Config.get("command/shutdown", "").strip()
        if command:
            self.right.append(ShutdownItem())
        else:
            self.right.append(CloseItem())
        self._selected_index = 0

    def append_left(self, item):
        self.left.append(item)

    def append_right(self, item):
        self.right.insert(0, item)

    @property
    def selected_item(self):
        index = self._selected_index
        return self[index]

    def get_selected_index(self):
        return self._selected_index

    def set_selected_index(self, index, immediate=False):
        self._selected_index = index

    def go_left(self, count=1):
        index = self._selected_index - 1
        while index >= 0:
            if self[index].enabled:
                self._selected_index = index
                break
            index -= 1

    def go_right(self, count=1):
        index = min(len(self) - 1, self._selected_index + count)
        self._selected_index = index
        if not self[index].enabled:
            self.go_right()

    def go_up(self):
        # do nothing
        pass

    def activate(self):
        result = self.selected_item.activate(State.current_menu)
        # FIXME:
        from .menu import Menu
        if isinstance(result, Menu):
            # FIXME:
            from .window import enter_menu
            enter_menu(result)

    def __getitem__(self, index):
        if index < len(self.left):
            return self.left[index]
        return self.right[index - len(self.left)]

    def __len__(self):
        return len(self.left) + len(self.right)
