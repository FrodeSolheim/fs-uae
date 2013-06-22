from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division


import os
import sys
#import math
import time
import random
import pygame
try:
    # force include by py2exe
    import pygame._view
except ImportError:
    pass
import threading
from collections import deque
import Image

from fengestad import fs
#from fengestad.gamecenter.gamecontext import GameContext
from fengestad.gamecenter.gamecenter import GameCenter
from fengestad.gamecenter.gamescanner import GameScanner
#from fengestad.gamecenter.config import Config
from fengestad.gamecenter.main import Main
from fengestad.gamecenter.notification import Notification
#from fengestad.gamecenter.database import Database
#from fengestad.gamecenter.nameutil import NameUtil
#from fengestad.gamecenter.gamecontroller import GameController
#from fengestad.gamecenter.glui.history import GameHistory
from fengestad.gamecenter.glui.settings import Settings
from .opengl import *
from .errordialog import show_exception
from .imageloader import ImageLoader
#from .launchframe import LaunchFrame, LaunchBackFrame, InfoFrame
from .gamerunner import GameRunner
from .menu import Menu
from .itemmenu import ItemMenu
from .navigatable import Navigatable
from .displaylists import DisplayLists
from .bezier import Bezier
from .font import Font, BitmappedFont
from .input import InputHandler
from .animation import AnimationSystem, AnimationSequence
from .animation import AnimateValueLinear, AnimateValueBezier
#from .barcode import BarcodeSupport
from .text import TextRenderer
from .state import State
from .render import Render
from .notification import NotificationRender
from .util import sort_configurations
from fengestad.gamecenter.resources import resources, logger, ngettext, _
from .sdl import *
from .items import *

ENABLE_VSYNC = Config.get_bool("Menu/VSync", True)
ENABLE_VSYNC = False
IDLE = Config.get_bool("Menu/Idle", 1)
#Render.display_fps = pyapp.user.ini.get_bool("Menu/ShowFPS", False)
LIGHTING = Config.get_bool("Menu/Lighting", False)
RENDER_DEBUG_SQUARES = 0

def get_current_time():
    return time.time()

force_display_ratio = None
display = None
real_display_height = 0
current_menu = None
last_menu = None
last_game = None
barcode_mode = False
barcode_scanner = False

def set_current_menu(menu):
    global current_menu
    if len(menu) == 0:
        menu.append(NoItem())
    current_menu = menu
    State.current_menu = menu
    if menu.navigatable:
        State.navigatable = menu.navigatable
    else:
        State.navigatable = menu
    State.top_menu = menu.top
    State.down_navigatable = None


from .constants import COVER_POS, TOP_HEIGHT, TOP_HEIGHT_VISIBLE

#WINDOWED_SIZE = [1024, 640] # 16:10
#WINDOWED_SIZE = (1024, 576) # 16:9
#WINDOWED_SIZE = (1024, 768) # 4:3
WINDOWED_SIZE = [960, 540] # 16:9


RENDER_GAME = ["IMAGE", "REFLECTIONS", "SCREENSHOTS"]
RENDER_GAME_OVERLAY = ["TITLE", "HEADER"]

#USE_MENU_TRANSITIONS = pyapp.user.ini.get_bool("Menu/Transitions", True)
#if "--disable-shaders" in sys.argv:
#    USE_MENU_TRANSITIONS = False
USE_MENU_TRANSITIONS = False


class MenuGameTransition:
    value = 0.0


class MenuForwardTransition:
    # must be initially set to 1 because this indicates that
    # the transition is done / not in progress
    value = 1.0

class MenuBackwardTransition:
    # must be initially set to 1 because this indicates that
    # the transition is done / not in progress
    value = 1.0

class ScreenshotTransition:
    anim = None
    # must be initially set to 1 because this indicates that
    # the transition is done / not in progress
    value = 1.0

class RunTransition:
    value = 0.0



FIELD_COLOR = 0.1
WALL_COLOR = 0.0
IDLE_EVENT = pygame.NUMEVENTS - 1
DEFAULT_ITEM_BRIGHTNESS = 0.8


class Mouse:

    items = []
    focus = None

    @classmethod
    def set_visible(cls, visible=True):
        #if visible:
        #    import traceback
        #    traceback.print_stack()
        if State.mouse_visible == visible:
            return
        if not visible:
            cls.focus = None
        pygame.mouse.set_visible(visible)
        State.mouse_visible = visible


def set_items_brightness(brightness, duration=1.0, delay=0.0):
    State.items_brightness_anim = AnimateValueBezier(
            (State, "items_brightness"),
            State.items_brightness, State.time + delay,
            State.items_brightness, State.time + delay,
            brightness, State.time + delay + duration,
            brightness, State.time + delay + duration)


def compile_shader(source, shader_type):
    shader = glCreateShaderObjectARB(shader_type)
    glShaderSourceARB(shader, source)
    glCompileShaderARB(shader)
    try:
        status = ctypes.c_int()
        glGetShaderiv(shader, GL_COMPILE_STATUS, ctypes.byref(status))
        status = status.value
    except TypeError:
        status = glGetShaderiv(shader, GL_COMPILE_STATUS)
    if not status:
        print_log(shader)
        glDeleteObjectARB(shader)
        raise ValueError, "Shader compilation failed"
    return shader


def compile_program(vertex_source, fragment_source):
    vertex_shader = None
    fragment_shader = None
    program = glCreateProgram()
    if vertex_source:
        print("compile vertex shader")
        vertex_shader = compile_shader(vertex_source, GL_VERTEX_SHADER)
        glAttachShader(program, vertex_shader)
    if fragment_source:
        print("compile fragment shader")
        fragment_shader = compile_shader(fragment_source, GL_FRAGMENT_SHADER)
        glAttachShader(program, fragment_shader)
    glLinkProgram(program)
    status = glGetProgramiv(program, GL_LINK_STATUS)
    if status == GL_FALSE:
        print("could not link shader program")
        print glGetProgramInfoLog(program)
        sys.exit(1)
    if vertex_shader:
        glDeleteShader(vertex_shader)
    if fragment_shader:
        glDeleteShader(fragment_shader)
    return program


def print_log(shader):
    length = ctypes.c_int()
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, ctypes.byref(length))
    if length.value > 0:
        log = glGetShaderInfoLog(shader)
        print(log)
        sys.exit(1)


def compile_programs():
    global texture_program, color_program, premultiplied_texture_program

    vertex_shader = None
    color_program = compile_program(vertex_shader, ["""
void main()
{
vec4 s = gl_Color;
float extra_alpha = 1.0;
//float extra_alpha = gl_TextureEnvColor[0].a;
float a = s.a * extra_alpha;
gl_FragColor.a = a;
gl_FragColor.r = s.r*a;
gl_FragColor.g = s.g*a;
gl_FragColor.b = s.b*a;
}
            """])
    texture_program = compile_program(vertex_shader, ["""
uniform sampler2D texture;

void main()
{
vec4 s = texture2D(texture,gl_TexCoord[0].st);
//float extra_alpha = 1.0;
//float extra_alpha = gl_TextureEnvColor[0].a;
float opacity = gl_Color.a;
float a = s.a * opacity;
gl_FragColor.a = a;
gl_FragColor.r = s.r * a;
gl_FragColor.g = s.g * a;
gl_FragColor.b = s.b * a;
}
            """])
    premultiplied_texture_program = compile_program(vertex_shader, ["""
uniform sampler2D texture;

void main()
{
vec4 s = texture2D(texture,gl_TexCoord[0].st);
//float extra_alpha = 1.0;
//float extra_alpha = gl_TextureEnvColor[0].a;
float opacity = gl_Color.a;
gl_FragColor.a = s.a * opacity;
gl_FragColor.r = s.r * opacity;
gl_FragColor.g = s.g * opacity;
gl_FragColor.b = s.b * opacity;
}
            """])


def enable_texture_shader():
    fs_emu_blending(True)
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)
    glUseProgram(texture_program)


def disable_shader():
    glUseProgram(0)


def set_program(program):
    if not program:
        glUseProgram(0)
    else:
        glUseProgram(program)


class FrameBufferObject(object):#Renderable, RendererBase):

    def __init__(self, width=100, height=100):
        self._transparent = True
        self._has_transparency = True
        self._width = width
        self._height = height
        self._fb = None
        self._depth_rb = None
        self._stencil_rb = None
        self._texture = None

    def __del__(self):
        self.free()

    def free(self):
        if self._fb:
            glDeleteFramebuffersEXT(1, self._fb)
        if self._depth_rb:
            glDeleteRenderbuffersEXT(1, self._depth_rb)
        if self._stencil_rb:
            glDeleteRenderbuffersEXT(1, self._stencil_rb)
        if self._texture:
            glDeleteTextures([self._texture])

    def _create(self):
        if self._fb is not None:
            return

        w = self._width
        h = self._height
        if self._transparent:
            texture_format = GL_RGBA
        else:
            texture_format = GL_RGBA
        texture_target = GL_TEXTURE_2D
        fb = GLuint()
        depth_rb = GLuint()
        stencil_rb = GLuint()

        glGenFramebuffersEXT(1, ctypes.byref(fb))
        color_tex = Render.create_texture()
        glGenRenderbuffersEXT(1, ctypes.byref(depth_rb))
        glGenRenderbuffersEXT(1, ctypes.byref(stencil_rb))

        fb = fb.value
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb)

        # initialize color texture
        glBindTexture(texture_target, color_tex)
        glTexParameterf(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameterf(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glTexImage2D(texture_target, 0, texture_format, w, h, 0, GL_RGB,
                GL_INT, None)
        try:
            # FIXME: An error seems to be thrown here on Windows, even
            # though it seems to work...
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
                    GL_COLOR_ATTACHMENT0_EXT, texture_target, color_tex, 0)
        except Exception:
            pass

        # initialize depth renderbuffer
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb)
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,
                GL_DEPTH_COMPONENT24, w, h)
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
                GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb)
        # initialize stencil renderbuffer
        #glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, stencil_rb)
        #glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX,
        #        512, 512)
        #glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
        #        GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, stencil_rb)
        glBindTexture(texture_target, 0)
        # Check framebuffer completeness at the end of initialization.
        status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)
        assert status == GL_FRAMEBUFFER_COMPLETE_EXT, str(status)
        self._fb = fb
        self._depth_rb = depth_rb
        self._stencil_rb = stencil_rb
        self._texture = color_tex

    def bind(self):
        if self._fb is None:
            self._create()
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, self._fb)
        #glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, self._depth_rb)

    def unbind(self):
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0)
        #glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0)

    def set_viewport(self):
        glViewport(0, 0, self._width, self._height)
        #pass

    def render(self, opacity=1.0):
        render_texture(self._texture, -1.0, -1.0, 0, 2.0,
                2.0, opacity=opacity,
                transparent=False, is_premultiplied=True)


def render_texture(texture, x, y, z, w, h, opacity=1.0, transparent=False,
        is_premultiplied=False):
    if is_premultiplied:
        set_program(premultiplied_texture_program)
    else:
        set_program(texture_program)
    fs_emu_texturing(True)
    glBindTexture(GL_TEXTURE_2D, texture)
    glBegin(GL_QUADS)
    glColor(1.0, 1.0, 1.0, opacity)
    glTexCoord2f(0.0, 0.0)
    glVertex3f(x, y, z)
    glTexCoord2f(1.0, 0.0)
    glVertex3f(x + w, y, z)
    glTexCoord2f(1.0, 1.0)
    glVertex3f(x + w, y + h, z)
    glTexCoord2f(0.0, 1.0)
    glVertex3f(x, y + h, z)
    glEnd()
    set_program(None)


class ConfigMenu(Navigatable):
    def __init__(self):
        self.game = {}
        self.items = []
        self._index = 0
        self.position = 0

    def sort_items(self):
        self.items = sort_configurations(self.items)
        """
        # Cracked games score better, since these need no
        # manual lookup, etc.
        score_table = [
            [" AGA ", -(2**16)], # amiga AGA version
            [" cr ", -(2**15)], # cracked version
            [" NTSC ", -(2**14)], # prefer NTSC version
        ]
        scored = []
        for item in self.items:
            check = " " + item.replace(",", " ") + " "
            score = 0
            for p, s in score_table:
                if p in check:
                    #scored.append((0, item))
                    score += s
            # prefer shortest configs, all else being equal
            score += len(item)
            scored.append((score, item))
            #scored.append(1, item)
        scored.sort()
        self.items[:] = [x[1] for x in scored]
        """

    def get_index(self):
        return self._index % len(self.items)
    def set_index(self, index):
        self._index = index
    index = property(get_index, set_index)

    def go_up(self):
        if self._index == 0:
            State.down_navigatable = self
            State.navigatable = State.top_menu
        else:
            self._index -= 1

    def go_down(self):
        if self._index < len(self.items) - 1:
            self._index += 1

    def go_left(self, count=1):
        pass

    def go_right(self, count=1):
        pass

    def activate(self):
        print("run game here")
        game = self.game
        game["config"] = self.items[self.index]
        run_game(game)


def enter_menu(result):
    print("enter_menu", result)
    print("   menu parent_menu", result.parent_menu)

    current_menu = State.history[-1]
    result.parents[:] = current_menu.parents
    result.parents.append(current_menu)
    print("   menu parents    ", result.parents)
    State.history.append(result)
    set_current_menu(result)


def get_last_created_games(max_count):
    return [
        "Heisann",
        "og",
        "hoppsann",
        "og",
        "fallerallera",
        "for",
        "vi",
        "er",
        "salige",
    ]


def create_main_menu():
    new_menu = AllMenuItem().activate(None)
    if len(new_menu) == 0:
        new_menu.append(NoItem())
    return new_menu


def recreate_main_menu_if_necessary():
    new_menu = create_main_menu()
    State.history.append(new_menu)
    set_current_menu(new_menu)

def open_terminal():
    import subprocess
    p = subprocess.Popen(["xterm"])


def show():
    global barcode_mode
    global barcode_scanner
    global current_menu
    if "--barcode-mode" in sys.argv:
        barcode_mode = True
        barcode_scanner = True
    #pyapp.app.require_wx_app()
    # fade_from is used on init_display, so we initialize this
    # color here. Set alpha to 2.0 to force 1 second of solid
    # colir in combination with 2 sec. animation below
    if False and fs.windows and not Settings.fullscreen_menu:
        State.fade_from = (1.0, 1.0, 1.0, 2.0)
        State.fade_to = (1.0, 1.0, 1.0, 0.0)
    else:
        State.fade_from = (0.0, 0.0, 0.0, 2.0)
        State.fade_to = (0.0, 0.0, 0.0, 0.0)
    init_display()
    if USE_MENU_TRANSITIONS:
        compile_programs()
    init_input()
    on_resize((Render.display_width, Render.display_height))
    image_loader = ImageLoader()
    image_loader.start()
    new_menu = create_main_menu()
    State.history.append(new_menu)
    set_current_menu(new_menu)
    if len(new_menu) == 1:
        # only n/a item showing, possibly
        if (len(AllMenuItem().activate(None)) == 0):
            # no games, initiate game scan
            rescan_games()

    State.fade_start = get_current_time()
    State.fade_end = get_current_time() + 2.000
    # make a timer so that update events are sent to modules at least once
    # every second
    pygame.time.set_timer(IDLE_EVENT, 1000)
    #pygame.event.set_blocked(pygame.MOUSEMOTION)
    State.start_time = get_current_time()
    try:
        the_main_loop()
    finally:
        image_loader.stop()
    print(" --- start fade_quit ---")
    fade_quit()
    print(" --- show function is done ---")


def init_fonts():
    NotificationRender.init()
    liberation_sans_bold = resources.resource_filename(
            "LiberationSans-Bold.ttf")
    liberation_sans_narrow_bold = resources.resource_filename(
            "LiberationSansNarrow-Bold.ttf")
    vera_font_path = resources.resource_filename(
            "VeraBd.ttf")
    
    Font.title_font = pygame.font.Font(liberation_sans_bold,
            int(0.04 * Render.display_height))
    
    Font.subtitle_font = pygame.font.Font(liberation_sans_bold,
            int(0.025 * Render.display_height))
    #subtitle_font = pygame.font.Font(liberation_sans_narrow_bold,
    #        int(0.03 * Render.display_height))
    Font.small_font = pygame.font.Font(liberation_sans_narrow_bold,
            int(0.025 * Render.display_height))
    #subtitle_font = pygame.font.Font(vera_font_path,
    #        int(0.05 * Render.display_height))
    Font.main_path_font = pygame.font.Font(liberation_sans_bold,
            int(0.025 * Render.display_height))
    Font.list_subtitle_font = pygame.font.Font(liberation_sans_bold,
            int(0.020 * Render.display_height))
    Font.header_font = pygame.font.Font(vera_font_path,
            int(0.06 * Render.display_height))
#    InfoFrame.status_font = pygame.font.Font(vera_font_path,
#            int(0.05 * Render.display_height))

    BitmappedFont.title_font = BitmappedFont("title_font")
    BitmappedFont.menu_font = BitmappedFont("menu_font")

char_buffer = ""
char_buffer_last_updated = 0


def character_press(char):
    return
"""
    global char_buffer
    global char_buffer_last_updated
    char_buffer_last_updated = State.time
    print("character press", repr(char), "(search is DISABLED)")
    return
    Render.dirty = True
    if char == "RETURN":
        print char_buffer, len(char_buffer)
        if len(char_buffer) == 9 and barcode_scanner:
            game_info = BarcodeSupport.process(char_buffer)
            if game_info:
                activate_barcode_game(game_info)
            char_buffer = ""
            return True
        return False
    elif char == "BACKSPACE" or char == u"\b":
        char_buffer = char_buffer[:-1]
        if len(char_buffer) <= 2:
            char_buffer = ""
        return True
    elif len(char) == 1:
        char_buffer += char
        return True
    else:
        raise Exception(repr(char))
"""


class SearchTextItem(MenuItem):
    
    def __init__(self, title):
        MenuItem.__init__(self)
        self.path_title = title


def create_search_results_menu(text):
    global current_menu
    try:
        if text == current_menu.search_text:
            return
    except AttributeError:
        pass
    new_menu = ItemMenu(_("Search Results"))
    new_menu.search_text = text
    words = [x.strip() for x in text.lower().split(" ")]
    print "Creating search results for", words
    new_menu.top.append_left(SearchTextItem(_("Search for '{0}'".format(
            text))))
    clause = []
    args = []
    for word in words:
        clause.append("AND name like ?")
        args.append("%{0}%".format(word))
    clause = " ".join(clause)
    for item in create_game_items(clause, args):
        new_menu.append(item)
    if len(new_menu) == 0:
        new_menu.append(NoItem("No Search Results"))
    if hasattr(current_menu, "search_text"):
        # replace current search menu, not append to path
        new_menu.parent_menu = current_menu.parent_menu
    else:
        new_menu.parent_menu = current_menu
    print("create search results menu")
    set_current_menu(new_menu)


def rescan_games():
    #global current_menu
    print("rescan games -- currently disabled")
    #GameScanner.scan()
    #Render.dirty = True
    pass

def go_back():
    State.history.pop()
    set_current_menu(State.history[-1])


def default_input_func(button):
    if button == "LEFT":
        State.navigatable.go_left()
    elif button == "RIGHT":
        State.navigatable.go_right()
    elif button == "UP":
        State.navigatable.go_up()
    elif button == "DOWN":
        State.navigatable.go_down()
    elif button == "SKIP_LEFT":
        State.navigatable.go_left(10)
    elif button == "SKIP_RIGHT":
        State.navigatable.go_right(10)
    elif button == "PRIMARY":
        State.navigatable.activate()
    elif button == "BACK":
        if State.config_menu:
            State.config_menu = None
            set_current_menu(State.current_menu)
            set_items_brightness(0.66, duration=0.500)
        elif State.current_game is None and char_buffer:
            InputHandler.get_button() # clear OK status
            character_press("BACKSPACE")
        elif State.current_game:
            State.current_game = None
            #if barcode_mode:
            #    barcode_animation = None
            #
            #else:
            game_fade_animation = AnimateValueBezier(
                    (MenuGameTransition, "value"),
                    1.0, State.time,
                    1.0, State.time + 0.133,
                    0.0, State.time + 0.133,
                    0.0, State.time + 0.400)

        #elif can_navigate and current_menu.parent_menu:
        #elif can_navigate and len(State.history) > 1:
        elif len(State.history) > 1:
            go_back()
    elif button == "QUIT":
        State.quit = True


def render_top():
    Render.hd_perspective()
    glPushMatrix()
    transition = State.current_menu.top_menu_transition
    glTranslate(0.0, (1.0 - transition) * 90, 0.9)

    """
    fs_emu_blending(True)
    fs_emu_texturing(True)
    Texture.top_background.bind()
    glBegin(GL_QUADS)
    glColor3f(1.0, 1.0, 1.0)
    glTexCoord2f(0.0, 1.0)
    glVertex3f(0, 990, 0.0)
    glTexCoord2f(1.0, 1.0)
    glVertex3f(1920, 990, 0.0)
    glTexCoord2f(1.0, 0.0)
    glVertex3f(1920, 1080, 0.0)
    glTexCoord2f(0.0, 0.0)
    glVertex3f(0, 1080, 0.0)
    glEnd()
    """

    glTranslate(0.0, 0.0, 0.05)

    if State.top_menu == State.navigatable:
        selected_index = State.top_menu.get_selected_index()
    else:
        selected_index = -1
    x = State.gl_left
    for item in State.top_menu.left:
        item.update_size_left()
        item.x = x
        item.y = 1080 - TOP_HEIGHT
        item.h = TOP_HEIGHT
        x += item.w

    index = len(State.top_menu.left) - 1
    for item in reversed(State.top_menu.left):
        item.render_top_left(selected=(index == selected_index))
        index -= 1

    index = len(State.top_menu) - 1
    x = State.gl_right
    for item in reversed(State.top_menu.right):
        item.update_size_right()
        x -= item.w
        item.x = x 
        item.y = 1080 - TOP_HEIGHT
        item.h = TOP_HEIGHT
        if Mouse.focus:
            selected = (Mouse.focus == item)
        else:
            selected = (index == selected_index)
        item.render_top_right(selected=selected)
        Mouse.items.append(item)
        index -= 1
    glPopMatrix()
    #fs_emu_blending(False)


def barcode_games_add_random():
    #for i in range(5):
    #    # Try up to 5 times to get a "unique" game
    #    game_info = GameList.get().get_random_game()
    #    if game_info not in barcode_games:
    #        break
    ## game_info can be None if there is no registered game
    #if game_info is not None:
    #    barcode_games.append(game_info)
    return


barcode_games = []
#barcode_time = 0.0
class BarcodePosition:
    value = -2.0
#barcode_position = -2.0
barcode_last_ticks = 0
barcode_animation = None
def render_barcode_menu():
    #global barcode_position
    global barcode_time
    global barcode_last_ticks
    render_background()
    Render.standard_perspective()

    t = get_current_time()
    dt = t - barcode_last_ticks
    if dt > 500:
        dt = 0
    barcode_last_ticks = t

    while len(barcode_games) < 10:
        try:
            barcode_games_add_random()
        except IndexError:
            # No games
            break

    if not barcode_animation:
        BarcodePosition.value = BarcodePosition.value - dt / 4000.0
        while BarcodePosition.value < -3.2:
            BarcodePosition.value = BarcodePosition.value + 1.2
            if len(barcode_games) > 0:
                del barcode_games[0]

    image_list = []
    for game_info in barcode_games:
        image_list.append(game_info.get_image_path())
    TextureManager.get().load_images(image_list)

    glPushMatrix()
    #glRotatef(35.0, 0.0, 1.0, 0.0)
    glTranslate(0.0, 0.0, -2.5)

    angle = 20.0
    if State.current_game:
        angle = 20.0 - 20 * MenuGameTransition.value
    glRotatef(angle, 0.0, 1.0, 0.0)
    #glRotatef(20.0, 0.0, 1.0, 0.0)

    for i, game_info in enumerate(barcode_games):
        p = BarcodePosition.value + i * 1.2
        #print i, game_info, p
        glPushMatrix()
        #glTranslate(p, 0.0, -2.5)
        glTranslate(p, 0.0, 0.0)
        #glTranslate(image_offset[0], 0.0, 0.0)
        item = GameItem(game_info)
        item.render()
        glPopMatrix()

    glPopMatrix()


text_cache_history = []
text_cache = {}


def render_text(text, font, x, y, w=0, h=0, color=(1.0, 1.0, 1.0, 1.0),
        shadow=False, shadow_color=(0, 0, 0)):

    if not text:
        return 0, 0
    #if len(color) == 3:
    #    color = (color[0], color[1], color[2], 1.0
    try:
        alpha = color[3]
    except IndexError:
        alpha = 1.0
    color = (int(round(color[0] * 255)),
            int(round(color[1] * 255)),
            int(round(color[2] * 255)))

    cache_key = (text, hash(font), color, alpha)
    try:
        text_cache_history.remove(cache_key)
    except ValueError:
        texture = None
    else:
        texture, txtsize = text_cache[cache_key]

    fs_emu_blending(True)
    glDisable(GL_DEPTH_TEST)

    if texture:
        glBindTexture(GL_TEXTURE_2D_ARB, texture)
    else:
        txtdata, txtsize = TextRenderer(font).Render.text(text, color)
        texture = Render.create_texture()
        glBindTexture(GL_TEXTURE_2D_ARB, texture)
        glTexImage2D(GL_TEXTURE_2D_ARB, 0, GL_RGBA, txtsize[0],
                txtsize[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, txtdata)
        #glTexParameteri(GL_TEXTURE_2D_ARB, GL_TEXTURE_MIN_FILTER,
        #        GL_LINEAR)
        #glTexParameteri(GL_TEXTURE_2D_ARB, GL_TEXTURE_MAG_FILTER,
        #        GL_LINEAR)
    tw, th = txtsize[0] * State.ortho_pscalex, txtsize[1] * State.ortho_pscaley

    tx = x
    ty = y
    if w > 0:
        tx = tx + (w - tw) / 2
    if h > 0:
        ty = ty + (h - th) / 2
    ts = 4 / Render.display_height # Step


    #glTexEnv(GL_TEXTURE_2D_ARB, GL_MODULAE)
    glBegin(GL_QUADS)
    glColor4f(alpha, alpha, alpa, alpha)

    glTexCoord2f(0.0, 0.0)
    glVertex2f(tx, ty)
    glTexCoord2f(txtsize[0], 0.0)
    glVertex2f(tx + tw, ty)
    glTexCoord2f(txtsize[0], txtsize[1])
    glVertex2f(tx + tw, ty + th)
    glTexCoord2f(0.0, txtsize[1])
    glVertex2f(tx, ty + th)


    #glRasterPos2f(tx, ty)
    #glDrawPixels(txtsize[0], txtsize[1], GL_RGBA, GL_UNSIGNED_BYTE, txtdata)
    glEnd()
    
    glBindTexture(GL_TEXTURE_2D, 0)
    #fs_emu_blending(False)
    glEnable(GL_DEPTH_TEST)

    text_cache_history.append(cache_key)
    text_cache[cache_key] = texture, txtsize
    if len(text_cache) > 50:
        cache_key = text_cache_history.pop(0)
        texture, txtsize = text_cache.pop(cache_key)
        glDeleteTextures([texture])

    """
    # FIXME:
    shadow = False

    glDisable(GL_DEPTH_TEST)
    fs_emu_blending(True)
    #text = current_menu.selected_item.title
    #if shadow:
    txtdata, txtsize = TextRenderer(font).Render.text(text, shadow_color)
    tw, th = txtsize[0] * ortho_pscalex, txtsize[1] * ortho_pscaley
    tx = x
    ty = y
    if w > 0:
        tx = tx + (w - tw) / 2
    if h > 0:
        ty = ty + (h - th) / 2
    #tx = 0 - tw / 2
    #ty = -0.67
    ts = 4 / Render.display_height # Step
    if shadow:
        glPixelTransferf(GL_ALPHA_SCALE, 0.04)
        for fx, fy in [(1, 1), (-1, -1), (1, -1), (-1, 1), (1, 0), (-1, 0),
                (0, -1), (0, 1)]:
            glRasterPos2f(tx - fx * ts, ty - fy * ts)
            glDrawPixels(txtsize[0], txtsize[1], GL_RGBA, GL_UNSIGNED_BYTE,
                    txtdata)
        glPixelTransferf(GL_ALPHA_SCALE, 0.01)
        for fx, fy in [(0, 2), (2, 0), (0, -2), (-2, 0),
                (1, 2), (2, 1), (-1, 2), (-2, 1), (1, -2),
                (2, -1), (-1, -2), (-2, -1)]:
            glRasterPos2f(tx - fx * ts, ty - fy * ts)
            glDrawPixels(txtsize[0], txtsize[1], GL_RGBA, GL_UNSIGNED_BYTE,
                    txtdata)
    glPixelTransferf(GL_ALPHA_SCALE, alpha)
    rendered = font.render(text, True, color)
    txtsize = rendered.get_size()
    txtdata = pygame.image.tostring(rendered, "RGBA", 1)
    glRasterPos2f(tx, ty)
    glDrawPixels(txtsize[0], txtsize[1], GL_RGBA, GL_UNSIGNED_BYTE, txtdata)
    #glPopAttrib()
    glPixelTransferf(GL_ALPHA_SCALE, 1.0)
    #fs_emu_blending(False)
    glEnable(GL_DEPTH_TEST)
    """
    return tw, th


def render_config_menu():
    if not State.config_menu:
        return
    Render.ortho_perspective()
    CONFIG_SEPARATION = 0.15
    config_menu = State.config_menu
    #text = config_menu.items[config_menu.index]
    #otw, th = Render.text(text, title_font,
    #        -1.0, -0.93, 2.0, color=(1.0, 1.0, 1.0, 0.36 * strength))
    #x = 0.0 + otw / 2 + CONFIG_SEPARATION
    #for i in range(config_menu.index + 1, len(config_menu.items)):
    #    text = config_menu.items[i]
    #    tw, th = Render.text(text, title_font,
    #            x, -0.93, color=(1.0, 1.0, 1.0, 0.36 * strength))
    #    x += tw + CONFIG_SEPARATION
    #x = 0.0 - otw / 2 - CONFIG_SEPARATION
    x = -0.55
    y = 0.8
    for i in range(len(config_menu.items)):
        text = config_menu.items[i].upper()
        #tw, th = Render.measure_text(text, title_font)
        #x -= tw + CONFIG_SEPARATION
        y -= 0.15
        if i == config_menu.index and config_menu == State.navigatable:
            color = (1.0, 1.0, 1.0, 1.0)
        else:
            color = (1.0, 1.0, 1.0, 0.33)
        Render.text(text, Font.subtitle_font, x, y, color=color)


def render_scanning_status():
    #Render.standard_perspective()
    Render.hd_perspective()
    text = GameScanner.get_status()
    #print(text)
    Render.dirty = True

    fs_emu_texturing(False)

    z = 0.0

    glBegin(GL_QUADS)
    glColor3f(0.0, 0.0, 0.0)
    glVertex3f(   0, 500, z)
    glVertex3f(1920, 500, z)
    glVertex3f(1920, 700, z)
    glVertex3f(   0, 700, z)
    glEnd()

    Render.text(text, Font.title_font, 200, 600, color=(1.0, 1.0, 1.0, 1.0))

    glBegin(GL_QUADS)
    glColor3f(1.0, 1.0, 1.0)
    x = 200
    y = 500
    z = 0.9
    x2 = 200 + 1520 * GameScanner.progress
    glVertex3f(x, y, z)
    glVertex3f(x2, y, z)
    glVertex3f(x2, y + 20, z)
    glVertex3f(x, y + 20, z)
    glEnd()
    fs_emu_texturing(True)


def do_render():
    if RunTransition.value > 0.99:
        # do not render anything when running a game
        return

    # try to exploit parallellism by uploading texture while rendering
    TextureManager.get().load_textures(1)
    
    # clear mouseover rects -these will be calculated during rendering
    Mouse.items[:] = []
    Render.standard_perspective()
    scanning = GameScanner.scanning
    data = current_menu.render()
    current_menu.render_transparent(data)

    if GameScanner.is_scanning():
        render_scanning_status()
        State.was_scanning = True
    else:
        render_config_menu()
        if State.was_scanning:
            print("State.was_scanning")
            State.was_scanning = False
            # reload current menu
            #if current_menu.parent_menu:
            #    result = current_menu.parent_menu.selected_item.activate(
            #            current_menu.parent_menu)
            #    if isinstance(result, Menu):
            #        #if len(result) == 0:
            #        #    result.append(NoItem())
            #        result.parent_menu = current_menu.parent_menu
            #        print("set new menu (rescanned games)")
            #        set_current_menu(result)
            recreate_main_menu_if_necessary()

    render_top()

    if State.dialog:
        State.dialog.render()

    render_global_fade()
    NotificationRender.render()
    if RunTransition.value > 0.0:
        render_fade(a=RunTransition.value)


def render_fade(r=0.0, g=0.0, b=0.0, a=0.0):
    Render.hd_perspective()
    fs_emu_blending(True)
    fs_emu_texturing(False)
    #glDisable(GL_DEPTH_TEST)
    glBegin(GL_QUADS)
    glColor4f(r * a, g * a, b * a, a)
    #glVertex2f(State.gl_left, -1.0)
    #glVertex2f(State.gl_right, -1.0)
    #glVertex2f(State.gl_right,  1.0)
    #glVertex2f(State.gl_left,  1.0)
    glVertex2f(0, 0)
    glVertex2f(1920, 0)
    glVertex2f(1920, 1080)
    glVertex2f(0, 1080)
    glEnd()
    #glEnable(GL_DEPTH_TEST)
    #fs_emu_blending(False)


def render_global_fade():
    #glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT)
    #glPushMatrix()
    #glTranslatef(0.0, 0.0, 0.99999)
    #render_fade(1, 0, 0, 1.0)
    #glPopMatrix()
    #return

    t = State.time
    if t <= State.fade_end and t >= State.fade_start:
        a = (t - State.fade_start) / \
                (State.fade_end - State.fade_start)
        if a < 0.0:
            a = 0.0
        elif a > 1.0:
            a = 1.0
        #print(a)
        #glClear(GL_DEPTH_BUFFER_BIT)
        Render.hd_perspective()
        glPushMatrix()
        glTranslatef(0.0, 0.0, 0.99999)
        fs_emu_blending(True)
        fs_emu_texturing(True)
        Texture.splash.render((1920 - Texture.splash.w) // 2,
                (1080 - Texture.splash.h) // 2, Texture.splash.w,
                Texture.splash.h, opacity=(1.0 - a))
        
        c = [0, 0, 0, (1.0 - a)]
        #for i in range(4):
        #    c[i] = State.fade_from[i] + \
        #            (State.fade_to[i] - State.fade_from[i]) * a * (a)
        render_fade(*c)
        glPopMatrix()
        Render.dirty = True

#if Render.display_fps:
FPS_FRAMES = 100
render_times = deque()
for i in range(FPS_FRAMES):
    render_times.append(0)

fps_str = ""
debug_x = 0
def render_debug_square():
    global debug_x
    Render.hd_perspective()
    fs_emu_texturing(False)
    glBegin(GL_QUADS)
    glColor3f(1.0, 1.0, 1.0)
    x = debug_x
    debug_x += 1
    if debug_x >= 1920:
        debug_x = 0
    y = 989
    z = 0.99
    glVertex3f(x, y, z)
    glVertex3f(x + 20, y, z)
    glVertex3f(x + 20, y + 20, z)
    glVertex3f(x, y + 20, z)
    glEnd()
    fs_emu_texturing(True)

debug_x_2 = 0
def render_debug_square_2():
    global debug_x_2
    Render.hd_perspective()
    fs_emu_texturing(False)
    glBegin(GL_QUADS)
    glColor3f(0.2, 0.2, 0.2)
    x = debug_x_2
    debug_x_2 += 1
    if debug_x_2 >= 1920:
        debug_x_2 = 0
    y = 989 + 5
    z = 0.99
    glVertex3f(x, y, z)
    glVertex3f(x + 20, y, z)
    glVertex3f(x + 20, y + 10, z)
    glVertex3f(x, y + 10, z)
    glEnd()
    fs_emu_texturing(True)


def swap_buffers():
    global fps_str

    Render.ortho_perspective()
    #glPushMatrix()
    #glTranslatef(0.0, 0.0, 0.5)


    #if not fps_str:
    #    fps_str = "WHY?"
    if Render.display_fps or True:
        if fps_str:
            Render.text(fps_str, Font.main_path_font,
                    -1.74, 0.82, h=0.1, color=(0.25, 0.25, 0.25, 1.0))
    #glPopMatrix()

    # FIXME: Why does not minimize from fullscreen work on ATI unless we render
    # something here?
    glBindTexture(GL_TEXTURE_2D, 0)
    glPushMatrix()
    glTranslate(2000.0, 0.0, 0.0)
    glBegin(GL_QUADS)
    glVertex2f(0.0, 0.0)
    glVertex2f(1.0, 0.0)
    glVertex2f(1.0, 1.0)
    glVertex2f(0.0, 1.0)
    glEnd()
    glPopMatrix()

    #fs_emu_blending(False)
    #glEnable(GL_DEPTH_TEST)

    #if Render.display_sync:
    #    glFinish()
    
    pygame.display.flip()
    if Render.display_sync:
        glFinish()

    if Render.display_fps:
        t = get_current_time()
        render_times.append(t)
        t0 = render_times.popleft()
        if t0 > 0 and State.frame_number % 5 == 0:
            timediff = t - t0
            #print("{0:0.2f}".format(300.0 / timediff))
            fps = FPS_FRAMES / timediff
            if fps >= 100:
                fps_str = "FPS:  {0:0.0f}".format(fps)
            else:
                fps_str = "FPS: {0:0.1f}".format(fps)
            #Render.text(fps_str, Font.title_font,
            #        -1.0, 0.90, 2.0, shadow=True)
    State.frame_number += 1
    Render.delete_textures()


def render_screen():
    # set Render.dirty to False here, so that render functions can
    # request a new render frame by setting dirty
    Render.dirty = False
    #glEnable(GL_SCISSOR_TEST)
    #can_idle =
    if SDL_IsMinimized():
        time.sleep(0.01)
        return
    #Render.dirty = True
    #glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)# | GL_STENCIL_BUFFER_BIT)
    do_render()
    if Render.display_fps:
        Render.dirty = True
    if RENDER_DEBUG_SQUARES:
        render_debug_square()


def init_display():
    global display
    global real_display_height, display_yoffset
    #global banner_texture, shadow_texture, gloss_texture
    #global top_texture, top_logo_texture, logo_texture
    #global missing_cover_texture, default_item_texture
    global backdrop_texture

    logger.debug("Init OpenGL menu display")
    #glClearColor(0.0, 0.0, 0.0, 1.0)
    pygame.display.init()
    pygame.font.init()
    #resolution =(0, 0)
    #flags = pygame.OPENGL | pygame.FULLSCREEN | pygame.DOUBLEBUF

    DisplayLists.clear()


    #on_resize()
    depth = 0
    # FIXME: HACK / TESTING
    #if not Settings.fullscreen_menu:
    #    if fs.windows:
    #        os.environ["SDL_VIDEO_WINDOW_POS"] = "3,29"
    #    else:
    #        os.environ["SDL_VIDEO_WINDOW_POS"] = "0,0"
    maximize_window = not Settings.fullscreen_menu and \
            Settings.windowed_size is None
    #dw, dh = wx.GetDisplaySize()
    display_info = pygame.display.Info()
    dw = display_info.current_w
    dh = display_info.current_h
    
    if Settings.fullscreen_menu:
        print("fullscreen is True")
        if fs.windows:
            #resolution = (0, 0)
            #flags = pygame.OPENGL | pygame.DOUBLEBUF | pygame.NOFRAME \
            #        | pygame.FULLSCREEN
            os.environ["SDL_VIDEO_WINDOW_POS"] = "0,0"
            flags = pygame.OPENGL | pygame.DOUBLEBUF | pygame.NOFRAME
            #flags = flags | pygame.FULLSCREEN
            #if fs.linux:
            #    pass
            #else:
            if dw > dh * 2:
                # Assume dual monitor setup - hack for Linux / SDL
                resolution = (dw / 2, dh)
            else:
                resolution = (dw, dh)
        else: # fullscreen, but not microsoft windows
            #resolution = (0, 0)
            flags = pygame.OPENGL | pygame.DOUBLEBUF | pygame.NOFRAME #| pygame.FULLSCREEN
            if dw > dh * 2:
                # Assume dual monitor setup - hack for Linux / SDL
                resolution = (dw / 2, dh)
            else:
                resolution = (dw, dh)
            if fs.linux:
                overscan = Config.get("display/overscan", "0,0,0,0")
                try:
                    overscan = overscan.split(",")
                    overscan = [int(x.strip()) for x in overscan]
                    print("using overscan", overscan)
                except Exception, e:
                    print("error parsing overscan from config:", repr(e))
                    overscan = [0, 0, 0, 0]
                os.environ["SDL_VIDEO_WINDOW_POS"] = "{0},{1}".format(
                        overscan[0], overscan[1])
                resolution = (resolution[0] - overscan[0] - overscan[2],
                        resolution[1] - overscan[1] - overscan[3])
            elif fs.macosx:
                # FIXME: fullscreen mode does not work very well. -When
                # opening a fullscreen emulator from fullscreen, the emulator
                # crashes on glViewport. Tested with fs-amiga.
                #flags |= pygame.FULLSCREEN

                # for now, we create an almost maximized window, works
                # quite well when the dock is set to auto-hide
                #resolution = (resolution[0], resolution[1] - 22)

                # FIXME: trying LSUIPresentationMode
                os.environ["SDL_VIDEO_WINDOW_POS"] = "0,0"

                kUIModeNormal = 0
                kUIModeContentSuppressed = 1
                kUIModeContentHidden = 2
                kUIModeAllSuppressed = 4
                kUIModeAllHidden = 3
                kUIOptionAutoShowMenuBar = 1 << 0
                kUIOptionDisableAppleMenu = 1 << 2
                kUIOptionDisableProcessSwitch = 1 << 3
                kUIOptionDisableForceQuit = 1 << 4
                kUIOptionDisableSessionTerminate = 1 << 5
                kUIOptionDisableHide = 1 << 6

                import objc
                from Foundation import NSBundle
                bundle = NSBundle.bundleWithPath_(
                        "/System/Library/Frameworks/Carbon.framework")
                objc.loadBundleFunctions(bundle, globals(), (
                        ("SetSystemUIMode", "III", ""),))

                SetSystemUIMode(kUIModeAllHidden, kUIOptionAutoShowMenuBar)

    else:
        if fs.windows and maximize_window and \
                not Settings.window_decorations:
            import ctypes 
            SPI_GETWORKAREA=48 
            class RECT(ctypes.Structure): 
                _fields_ = [("left",ctypes.c_ulong), 
                        ("top",ctypes.c_ulong), 
                        ("right",ctypes.c_ulong), 
                        ("bottom",ctypes.c_ulong)] 
            m = ctypes.windll.user32 
            r = RECT() 
            m.SystemParametersInfoA(SPI_GETWORKAREA, 0, ctypes.byref(r), 0)
            x = int(r.left)
            y = int(r.top)
            w = int(r.right) - int(r.left)
            h = int(r.bottom) - int(r.top)
            print(x, y, w, h)
            WINDOWED_SIZE[0] = w
            WINDOWED_SIZE[1] = h
            os.environ["SDL_VIDEO_WINDOW_POS"] = "{0},{1}".format(x, y)
            State.allow_minimize = False

        if Settings.windowed_size:
            WINDOWED_SIZE[0] = Settings.windowed_size[0]
            WINDOWED_SIZE[1] = Settings.windowed_size[1]
            Render.display_width = WINDOWED_SIZE[0]
            Render.display_height = WINDOWED_SIZE[1]
            if dw > 1400:
                Render.display_width = 1280
                Render.display_height = 720
        else:
            Render.display_width = WINDOWED_SIZE[0]
            Render.display_height = WINDOWED_SIZE[1]
        resolution = (Render.display_width, Render.display_height)
        if Settings.window_decorations:
            flags = pygame.OPENGL | pygame.DOUBLEBUF | pygame.RESIZABLE
        else:
            flags = pygame.OPENGL | pygame.DOUBLEBUF | pygame.NOFRAME
    display_yoffset = 0
    #print Render.display_height, display_yoffset, real_display_height
    #resolution = (1280, 1024)
    #pygame.mouse.set_visible(False)
    Mouse.set_visible(False)

    pygame.display.gl_set_attribute(pygame.GL_STENCIL_SIZE, 8)
    pygame.display.gl_set_attribute(pygame.GL_DEPTH_SIZE, 16)
    
    Render.display_sync = ENABLE_VSYNC
    if Render.display_sync:
        print("enabling vertical sync")
        os.environ["__GL_SYNC_TO_VBLANK"] = "1"
        pygame.display.gl_set_attribute(pygame.GL_SWAP_CONTROL, 1)
    else:
        os.environ["__GL_SYNC_TO_VBLANK"] = "0"
        pygame.display.gl_set_attribute(pygame.GL_SWAP_CONTROL, 0)
    pygame.display.gl_set_attribute(pygame.GL_DOUBLEBUFFER, 1)
    fsaa = Config.get_int("video/fsaa", 0)
    if fsaa:
        pygame.display.gl_set_attribute(pygame.GL_MULTISAMPLEBUFFERS, 1)
        pygame.display.gl_set_attribute(pygame.GL_MULTISAMPLESAMPLES, fsaa)
    print("pygame set display mode", resolution, flags, depth)
    display = pygame.display.set_mode(resolution, flags, depth)
    if not Settings.fullscreen_game:
        try:
            del os.environ["SDL_VIDEO_WINDOW_POS"]
        except KeyError:
            pass

#    if Settings.windowed_size is None:
    pygame.display.set_caption("FS Game Center")

    #if not Settings.fullscreen_menu
    #maximize_window = not Settings.fullscreen_menu

    # FIXME: DISABLING MAXIMIZE FOR DEBUGGING
    #maximize_window = False
    if maximize_window:
        print("maximizing window")
        SDL_Maximize()
        for event in pygame.event.get():
            if event.type == pygame.VIDEORESIZE:
                #WINDOWED_SIZE[0] = event.w
                #WINDOWED_SIZE[1] = event.h
                on_resize((event.w, event.h))
        print "DISPLAY.GET_SIZE", display.get_size()
    else:
        on_resize(display.get_size())

    #glScissor(0, display_yoffset, Render.display_width, Render.display_height)
    glMatrixMode(GL_MODELVIEW)

    #glClearColor(0.0, 0.0, 0.0, 1.0)
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)
    glClearColor(*State.fade_from)
    #glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    #glClear(GL_DEPTH_BUFFER_BIT)
    fs_emu_texturing(True)
    Texture.splash = Texture("splash.png")
    
    #for i in range(10):
    #    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    #    Render.hd_perspective()
    #    Texture.splash.render((1920 - Texture.splash.w) // 2,
    #            (1080 - Texture.splash.h) // 2, Texture.splash.w,
    #            Texture.splash.h)
    #    pygame.display.flip()
    
    #glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    #pygame.display.flip()
    #glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    #pygame.display.flip()
    #Render.hd_perspective()
    #Texture.splash.render((1920 - Texture.splash.w) // 2,
    #        (1080 - Texture.splash.h) // 2, Texture.splash.w,
    #        Texture.splash.h)
    #pygame.display.flip()
    for i in range(2):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        Render.hd_perspective()
        Texture.splash.render((1920 - Texture.splash.w) // 2,
                (1080 - Texture.splash.h) // 2, Texture.splash.w,
                Texture.splash.h)
        glFinish()
        pygame.display.flip()
        glFinish()
    
    #glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    #render_background()
    #pygame.display.flip()

    glEnable(GL_DEPTH_TEST)
    #glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

    Texture.shadow = Texture.from_resource("shadow.png")
    Texture.shadow2 = Texture.from_resource("shadow2.png")
    #Texture.gloss = Texture.from_resource("gloss.png")
    Texture.gloss = Texture("gloss.png")
    Texture.screen_gloss = Texture("screen_gloss.png")
    Texture.static = Texture("preview_static0.png")
    Texture.default_item = Texture("default_item.png")
    Texture.missing_screenshot = Texture("missing_screenshot.png")
    Texture.missing_cover = Texture("missing_cover.png")
    path = os.path.join(fs.get_data_dir(), "logo.png")
    if os.path.exists(path):
        im = Image.open(path)
        Texture.logo = Texture.load(im)
    else:
        Texture.logo = Texture.from_resource("logo.png")
    Texture.top = Texture.from_resource("top.png")
    Texture.top_logo = Texture("top_logo.png")
    Texture.top_logo_selected = Texture("top_logo_selected.png")

    Texture.add = Texture("add.png")
    Texture.add_selected = Texture("add_selected.png")
    Texture.home = Texture("home.png")
    Texture.home_selected = Texture("home_selected.png")
    Texture.minimize = Texture("minimize.png")
    Texture.minimize_selected = Texture("minimize_selected.png")
    Texture.close = Texture("close.png")
    Texture.close_selected = Texture("close_selected.png")
    Texture.shutdown = Texture("shutdown.png")
    Texture.shutdown_selected = Texture("shutdown_selected.png")

    Texture.bottom_bar = Texture("bottom_bar.png")
    Texture.screen_border_1 = Texture("screen_border_1.png")
    Texture.screen_border_2 = Texture("screen_border_2.png")
    Texture.top_background = Texture("top_background.png")
    Texture.top_item = Texture("top_item.png")
    Texture.top_item_selected = Texture("top_item_selected.png")
    Texture.top_item_left = Texture("top_item_left.png")
    Texture.top_item_left_selected = Texture("top_item_left_selected.png")
    Texture.top_item_right = Texture("top_item_right.png")
    Texture.top_item_arrow = Texture("top_item_arrow.png")
    Texture.top_item_arrow_selected = Texture("top_item_arrow_selected.png")
    
    Texture.sidebar_background_shadow = Texture(
            "sidebar_background_shadow.png")
    Texture.glow_top = Texture("glow_top.png")
    Texture.glow_top_left = Texture("glow_top_left.png")
    Texture.glow_left = Texture("glow_left.png")

    # FIXME: TEMPORARY - FOR TESTING, ONLY
    path = "c:\\git\\fs-game-database\\Backdrops\\ffx.png"
    if os.path.exists(path):
        im = Image.open(path)
        #im = resources.get_resource_image_pil("shadow.png")
        assert im.size == (1024, 1024)
        imdata = im.tostring("raw", "RGB")
        backdrop_texture = Render.create_texture()
        glBindTexture(GL_TEXTURE_2D, backdrop_texture)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im.size[0], im.size[1], 0,
                GL_RGB, GL_UNSIGNED_BYTE, imdata)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    else:
        backdrop_texture = 0

    #glActiveTextureARB(GL_TEXTURE1_ARB)
    #glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL)
    #glActiveTextureARB(GL_TEXTURE0_ARB)
    if LIGHTING:
        setup_lighting()



def setup_lighting():
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE)
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE)
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR)

    light_position = (0.0, 0.0, 3.0, 1.0)
    glLightfv(GL_LIGHT0, GL_POSITION, light_position)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, (1.0, 1.0, 1.0, 1.0))
    glLightfv(GL_LIGHT0, GL_SPECULAR, (0.0, 0.0, 0.0, 1.0))
    glEnable(GL_LIGHT0)

    glLightfv(GL_LIGHT1, GL_DIFFUSE, (0.0, 0.0, 0.0, 1.0))
    glLightfv(GL_LIGHT1, GL_SPECULAR, (1.0, 1.0, 1.0, 1.0))
    glEnable(GL_LIGHT1)

    glLightfv(GL_LIGHT2, GL_DIFFUSE, (0.0, 0.0, 0.0, 1.0))
    glLightfv(GL_LIGHT2, GL_SPECULAR, (0.5, 0.5, 0.5, 1.0))
    glEnable(GL_LIGHT2)

    glMaterialfv(GL_FRONT, GL_AMBIENT, (0.1, 0.1, 0.1, 1.0))
    glMaterialfv(GL_FRONT, GL_SHININESS, (10,))


def handle_videoresize_event(event):
    # event.resize.w
    # event.resize.h
    WINDOWED_SIZE[0] = event.w
    WINDOWED_SIZE[1] = event.h
    on_resize((event.w, event.h))


def on_resize(display_size):
    print("on_resize", display_size)
    global display
    global real_display_height, display_yoffset
    global browse_curve, header_curve#, screenshot_curve

    DisplayLists.clear()

    Render.display_width, Render.display_height = display_size #display.get_size()
    State.display_aspect = Render.display_width / Render.display_height
    print WINDOWED_SIZE
    Settings.windowed_size = tuple(WINDOWED_SIZE)

    real_display_height = Render.display_height
    #if Render.display_width / Render.display_height < 4 / 3:
    #    Render.display_height = int(Render.display_width / 4 * 3)
    #    display_yoffset = (real_display_height - Render.display_height) // 2
    #else:
    display_yoffset = 0
    #glViewport(0, 0, Render.display_width, real_display_height)
    #glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    #swap_buffers()
    #glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    #swap_buffers()
    print display_yoffset

    print(0, display_yoffset, Render.display_width, Render.display_height)
    glViewport(0, display_yoffset, Render.display_width, Render.display_height)

    init_fonts()

    aspect_ratio = max(4 / 3, (Render.display_width / Render.display_height))
    factor = (Render.display_width / Render.display_height) / (1024 / 600)
    browse_curve = Bezier.bezier(
            (-5.0 * factor, -10.0),
            (-1.7 * factor, -0.0),
            ( 1.7 * factor, -0.0),
            ( 5.0 * factor, -10.0)
    )
    header_curve = Bezier.bezier(
            (-2.0 * factor, 0.00),
            (-1.0 * factor, 0.075),
            ( 1.0 * factor, 0.075),
            ( 2.0 * factor, 0.00),
            steps=50
    )
    #screenshot_curve = Bezier.bezier(
    #        (0.0, 0.0),
    #        (0.0, 0.25),
    #        (1.0, 0.75),
    #        (1.0, 1.0),
    #        steps=50
    #)
    if USE_MENU_TRANSITIONS:
        State.fbo = FrameBufferObject(Render.display_width,
                Render.display_height)
    Render.dirty = True 


def close_display():
    logger.debug("Close OpenGL menu display")
    #glDeleteTextures([banner_texture])
    pygame.display.quit()


def close_input():
    InputHandler.close()


def init_input():
    InputHandler.open()


def close_before_running_game():
    logger.debug("close_before_running_game")
    TextureManager.get().unload_textures()
    close_display()
    close_input()
    logger.debug("close_before_running_game - done")


def get_window_position():
    return (0, 0)


def get_window_size():
    #return (800, 600)
    return (Render.display_width, real_display_height)


def run_game(controller, on_status):
    global ku_background_path

    #logger.info("Initiate game launch %s" % (game_info.full_id))
    print("run game", controller, on_status)
    #close_display()
    #while True:
    #    pyapp.app.main_loop()
    #pygame.quit()
    #app = wx.PySimpleApp()

    #GameHistory().add_recently_played(game_info)

    internal = True

    if internal:
        run_game_internal(controller, on_status)
    else:
        #run_game_with_launch_frame(controller, on_status)
        print("FIXME: NOT IMPLEMENTED")


def render_running_status(process, status):

    #glClearColor(0.0, 0.0, 0.0, 1.0)
    #glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    """
    render_background()
    what = RENDER_GAME[:]
    what.remove("IMAGE")
    render_game(current_game, what=what)
    render_game(current_game, what=["SCREENSHOT_SHADOWS"])

    glClear(GL_DEPTH_BUFFER_BIT)
    render_background(alpha)
    glClear(GL_DEPTH_BUFFER_BIT)
    Render.standard_perspective()
    glEnable(GL_DEPTH_TEST)
    """

    render_screen()

    Render.ortho_perspective()
    Render.text(status, Font.title_font,
            -1.0, -0.67, 2.0, shadow=True)
    Render.text(status, Font.subtitle_font,
            -1.0, -0.83, 2.0, color=(1.0, 1.0, 1.0, 0.36))

    swap_buffers()

class LogoStrength:
    anim = None
    value = 1.0


def run_game_internal(controller, on_status):

    game_runner = GameRunner(controller=controller)

    from .rundialog import RunDialog
    State.dialog = RunDialog()
    State.dialog_time = State.time

    t1 = get_current_time()

    # Using dictionary here to get correct scoping with variable access
    # from sub-functions
    # state_vars = {}
    #state_vars["finished"] = False

    run_state = {}
    run_state["stop_activity_thread"] = False

    def activity_thread():
        print("------------------- activity thread")
        while not run_state["stop_activity_thread"]:
            GameCenter.register_user_activity()
            time.sleep(5.0)
    threading.Thread(target=activity_thread).start()

    def show_run_error():
        RunTransition.value = 0.0
        RunTransition.anim = None
        if State.dialog:
            State.dialog.destroy()
        from .errordialog import ErrorDialog
        State.dialog = ErrorDialog(game_runner.error,
                game_runner.backtrace)
        show_error_state = {"stop": False}
        while not show_error_state["stop"]:
            def input_func(button):
                #print(State.quit)
                if button == "BACK":
                    show_error_state["stop"] = True
                #if State.quit:
                #    print("show_run_error.input_func, State.quit is True")
                #    show_error_state["stop"] = True
            if main_loop_iteration(input_func=input_func):
                return back_to_menu()
            #event = pygame.event.wait()
            #main_loop_work()
            #InputHandler.handle_event(event)
            #if InputHandler.get_button() == "PRIMARY":
            #    break
            #if event.type == pygame.QUIT:
            #    # FIXME: What should we do here?
            #    sys.exit(1)
            #render_screen()
            #swap_buffers()

            #elif event.type == pygame.VIDEOEXPOSE:
            #    render_running_status(process, status)
            #elif status != old_status:
            #    render_running_status(process, status)
            #    old_status = status


    #def render_loop(process, abortable=False):
    #    old_status = None
    #    while not game_runner.done:
    #        #print("...")
    #        status = game_runner.status
    #        event = pygame.event.wait()
    #        main_loop_work()
    #        InputHandler.handle_event(event)
    #        if abortable:
    #            if InputHandler.get_button() == "ABORT":
    #                return "ABORT"
    #        if event.type == pygame.QUIT:
    #            # FIXME: What should we do here?
    #            sys.exit(1)
    #        elif event.type == pygame.VIDEOEXPOSE:
    #            render_running_status(process, status)
    #        elif status != old_status:
    #            render_running_status(process, status)
    #            old_status = status

    def back_to_menu():
        run_state["stop_activity_thread"] = True

        if State.dialog:
            State.dialog.destroy()
        State.dialog = None
        State.dialog_time = State.time

        State.config_menu = None
        set_current_menu(State.current_menu)
        set_items_brightness(0.66, duration=0.500)

        RunTransition.value = 0.0
        RunTransition.anim = None
        LogoStrength.anim = None
        LogoStrength.value = 1.0

        Render.zoom = 1.0
        Render.offset_x = 0.0
        Render.offset_y = 0.0

        State.fade_start = get_current_time()
        State.fade_end = get_current_time() + 2.0
        # Use 2.0 here to force full black for 1 second
        State.fade_from = (0.0, 0.0, 0.0, 2.0)
        State.fade_to = (0.0, 0.0, 0.0, 0.0)

        #while (State.time - State.dialog_time) < 0.5:
        #    main_loop_work()
        #    AnimationSystem.update()
        #    render_screen()
        #    swap_buffers()

    def input_func(button):
        #print("first_input_func")
        if button == "BACK":
            run_state["stop"] = True

    #ANIMATION_SETTLE_TIME_1 = 0.5
    #ANIMATION_SETTLE_TIME_2 = 0.5
    ANIMATION_SETTLE_TIME_1 = 1.0
    ANIMATION_SETTLE_TIME_2 = 2.0

    print("> waiting for animation to settle...")
    # main loop while waiting for animation to settle
    run_state["stop"] = False
    while (State.time - State.dialog_time) < ANIMATION_SETTLE_TIME_1:
        #print(State.time > State.dialog_time)
        Render.dirty = True
        if main_loop_iteration(input_func=input_func):
            return back_to_menu()
        if run_state["stop"]:
            # FIXME: will abort cancel prepare step also?
            game_runner.abort()
            print("cancelled game start")
            return back_to_menu()

    # prepare will unpack the game and prepare game files
    game_runner.prepare()

    print("> preparing...")
    # main loop while preparing
    while not game_runner.done:
        Render.dirty = True
        if main_loop_iteration(input_func=input_func):
            return back_to_menu()
        if game_runner.error:
            show_run_error()
            run_state["stop"] = True
        if run_state["stop"]:
            # FIXME: will abort cancel prepare step also?
            game_runner.abort()
            print("cancelled game start")
            return back_to_menu()

    # FIXME: ABORT IS NOT SUPPOSED TO BE USED HERE - IS USED
    # FOR SPECIAL ABORT EMULATOR BUTTON COMBO. USE BACK
    # TO STOP PREPARE PROCESS, ETC
    #result = render_loop("Preparing", abortable=True)
    #if result == "ABORT":
    #    game_runner.abort()
    #if game_runner.error:
    #    show_error_message()
    #while (State.time - State.dialog_time) < 1.0:
    #    main_loop_work()
    #    AnimationSystem.update()
    #    render_screen()
    #    swap_buffers()

    print("> waiting for animation to settle (2)...")
    # main loop while waiting for animation to settle
    run_state["stop"] = False
    while (State.time - State.dialog_time) < ANIMATION_SETTLE_TIME_2:
        Render.dirty = True
        if main_loop_iteration(input_func=input_func):
            return back_to_menu()
        if run_state["stop"]:
            # FIXME: will abort cancel prepare step also?
            game_runner.abort()
            print("cancelled game start")
            return back_to_menu()

    #fade_run()

    game_runner.configure()

    print("> configuring...")
    # main loop while configuring
    while not game_runner.done:
        Render.dirty = True
        if main_loop_iteration(input_func=input_func):
            return back_to_menu()
        if game_runner.error:
            show_run_error()
            run_state["stop"] = True
        if run_state["stop"]:
            # FIXME: will abort cancel configure step also?
            game_runner.abort()
            print("cancelled game start")
            return back_to_menu()

    #State.fade_start = get_current_time()
    #State.fade_end = get_current_time() + 1.0
    #State.fade_from = (0.0, 0.0, 0.0, 0.0)
    #State.fade_to = (0.0, 0.0, 0.0, 1.0)

    t = get_current_time()
    RunTransition.anim = AnimateValueBezier(
            (RunTransition, "value"),
            0.0, t + 0.000,
            0.0, t + 0.000,
            1.0, t + 1.000,
            1.0, t + 1.000)

    print("> fading screen...")
    # main loop while fading screen
    while RunTransition.value < 0.99:
        Render.dirty = True
        if main_loop_iteration(input_func=input_func):
            return back_to_menu()
        #if game_runner.error:
        #    show_run_error()
        #    run_state["stop"] = True
        if run_state["stop"]:
            # FIXME: will abort cancel configure step also?
            game_runner.abort()
            print("cancelled game start")
            return back_to_menu()

    # new input_func to be used when running game, responds to
    # abort sequence only
    def input_func(button):
        #print("BUTTON", button)
        if button == "ABORT":
            run_state["stop"] = True

    print("> running the game...")
    # now, run the game and wait until the game is done
    game_runner.run()
    t = get_current_time()
    while not game_runner.done:
        if get_current_time() - t < 15.0:
            # On Windows w/ATI, it seems that the emulator window will not
            # be displayed until the game center window has been drawn at
            # least once after the emulator has opened a fullscreen window.
            # This does not happen on nVIDIA hardware. As a workaround, we
            # render the game center screen for a while after starting
            # the emulator
            Render.dirty = True
            if main_loop_iteration(input_func=input_func):
                return back_to_menu()
            if game_runner.error:
                show_run_error()
                run_state["stop"] = True
            if run_state["stop"]:
                print("aborting game")
                game_runner.abort()
                return back_to_menu()
        else:
            # FIXME: POSSIBLY RACE CONDITION HERE WHERE WE COULD WAIT
            # ENDLESSLY FOR GAME TO END?
            event = pygame.event.wait()
            if event.type == pygame.VIDEOEXPOSE:
                glClear(GL_COLOR_BUFFER_BIT)
                swap_buffers()
                
            #time.sleep(0.5)
    #    #result = render_loop("Running game", abortable=True)
    #    #if result == "ABORT":
    #    #    print("abort when in running game state")
    #    #    game_runner.abort()
    #    #    continue
    #    #break
    #if game_runner.error:
    #    show_error_message()

    # game completed successfully
    return back_to_menu()


def run_game_with_launch_frame(game_info):

    banner_bg = fade_run()
    """
    dc = wx.ScreenDC()
    dc.DrawBitmap(banner_bg, 1000, 0)
    del dc
    #wx.CallLater(100, wx.GetApp().ExitMainLoop)
    #pyapp.app.main_loop()
    """

    """
    def paint_handler(event):
        dc = wx.PaintDC(frame)
        dc.DrawBitmap(banner_bg, 0, 0)
        # And now display the real frame
        wx.CallAfter(frame.Destroy)
        #wx.CallAfter(wx.GetApp().)
    frame = wx.Frame(None, pos=(3000, 0), size=(1000, 1000), style=0)
    frame.Bind(wx.EVT_PAINT, paint_handler)
    frame.Show()
    pyapp.app.main_loop()
    time.sleep(0.3)
    """

    bgframe = LaunchBackFrame(pos=get_window_position(),
            size=get_window_size(),
            fullscreen=Settings.fullscreen_menu,
            background=banner_bg)
    wx.CallAfter(bgframe.show)
    logger.debug("Creating launch frame")
    frame = LaunchFrame(game_info, #on_visible=close_before_running_game,
            pos=get_window_position(),
            size=get_window_size(), fullscreen=Settings.fullscreen_menu,
            background=banner_bg)
    logger.debug("Showing launch frame")
    wx.CallLater(1000, frame.show)
    #dummy = wx.Frame(None)
    #wx.CallAfter(dummy.Show)
    #wx.CallAfter(dummy.Destroy)
    logger.debug("Entering main loop")
    pyapp.app.main_loop()
    #while True:
    #    #wx.GetApp().ProcessPendingEvents()
    #    wx.GetApp().Yield()
    logger.debug("Exited main loop")
    #init_display()
    #TextureManager.get().load_textures(10)
    ##init_input()
    #logger.debug("recreate main menu if necessary")
    #recreate_main_menu_if_necessary()
    ##render()
    ##fade_back(True)
    ##fade_back(True)
    State.fade_start = get_current_time()
    State.fade_end = get_current_time() + 1.0
    State.fade_from = (0.0, 0.0, 0.0, 1.0)
    State.fade_to = (0.0, 0.0, 0.0, 0.0)


    #for i in range(0, 100):
    #    fade_back(True)
    #    time.sleep(0.1)

    #frame.Destroy()
    def destroy_bgframe():
        try:
            bgframe.Destroy()
        except Exception:
            pass
    wx.CallAfter(destroy_bgframe)
    #wx.CallAfter(wx.GetApp().ExitMainLoop)
    # Enter main loop again to allow bgframe destruction to be processed
    logger.debug("enter main loop again...")
    pyapp.app.main_loop()
    logger.debug("Exited main loop (final)")

    if ku_background_path:
        try:
            os.unlink(ku_background_path)
        except Exception:
            pass

    #fade_back()


def fade_run():
    global ku_background_path

    class JumpOffset:
        value = 0.0

    t = get_current_time()
    #RunTransition.anim = AnimateValueBezier(
    #        (RunTransition, "value"),
    #        0.0, t + 0.0,
    #        0.0, t + 0.0,
    #        1.0, t + 1500.0,
    #        1.0, t + 1500.0)
    RunTransition.anim = AnimateValueBezier(
            (RunTransition, "value"),
            0.0, t + 0.000,
            0.7, t + 0.250,
            1.0, t + 0.500,
            1.0, t + 1.500)

    start = t
    duration = 1.500
    alpha = 0.0
    last_time = False
    while True:
        alpha = min(1.0, (get_current_time() - start) / duration)
        AnimationSystem.update()
        #glClear(GL_DEPTH_BUFFER_BIT)

        render_screen()

        if last_time:
            # Do not flip here -- want to read pixels
            break
        if alpha >= 1.0:
            last_time = True
        swap_buffers()

    #glPopAttrib()
    #glDeleteTextures([banner_texture])

    # FIXME: Distored image with GL_RGB -- why?
    """
    screenpixels = glReadPixels(0, 0, display_width, real_display_height,
            GL_RGBA, GL_UNSIGNED_BYTE)

    from PIL import Image
    im = Image.fromstring("RGBA", size=(display_width, real_display_height),
            data=screenpixels)
    im = im.convert("RGB")
    im = im.transpose(Image.FLIP_TOP_BOTTOM)

    handle, ku_background_path = tempfile.mkstemp(".bmp")
    os.close(handle)
    im.save(ku_background_path)
    os.environ["KGS_BACKGROUND"] = ku_background_path
    screenpixels = im.tostring("raw", "RGB")
    #print type(screenpixels)
    #print len(screenpixels)

    image = wx.ImageFromData(display_width, real_display_height, screenpixels)
    bitmap = wx.BitmapFromImage(image)
    return bitmap
    #import sys
    #sys.exit(1)
    """


def fade_quit():
    duration = 0.500
    alpha = 0.0
    start = get_current_time()
    while True:
        alpha = min(1.0, (get_current_time() - start) / duration)
        def render_func():
            render_screen()
            Render.ortho_perspective()
            fs_emu_blending(True)
            fs_emu_texturing(False)
            glDisable(GL_DEPTH_TEST)
            glBegin(GL_QUADS)
            glColor4f(0.0, 0.0, 0.0, alpha)
            glVertex2f(-10.0, -1.0)
            glVertex2f( 10.0, -1.0)
            glVertex2f( 10.0,  1.0)
            glVertex2f(-10.0,  1.0)
            glEnd()
            glEnable(GL_DEPTH_TEST)
            #fs_emu_blending(False)
            fs_emu_texturing(True)
            swap_buffers()
            Render.dirty = True
        main_loop_iteration(input_func=None, render_func=render_func)
        if alpha >= 1.0:
            break


def default_render_func():
    rendered = False 
    
    time_str = time.strftime("%H:%M")
    if time_str != Render.last_time_str:
        # clock must be updated, at least
        Render.dirty = True
        Render.last_time_str = time_str
    
    if Render.dirty:
        #print(Render.frame_number)
        render_screen()
        rendered = True
        Render.twice = False
        #Render.twice = True
    else:
        if not Render.twice:
            Render.twice = True
            render_screen()
            rendered = True
    if RENDER_DEBUG_SQUARES:
        render_debug_square_2()
    #if not rendered:
    #    pass
    #time.sleep(0.01)
    swap_buffers()


def main_loop_iteration(input_func=default_input_func,
        render_func=default_render_func):
    #print("main loop iteration")
    #time.sleep(0.1)
    #stop_loop = False

    #if State.idle_from and State.idle_from < get_current_time():
    #if not Render.dirty:
    #    events = [pygame.event.wait()]
    #else:
    #    events = pygame.event.get()
    
    events = pygame.event.get()

    State.time = get_current_time()
    
    
    Main.process()
    

    t = State.time

    if len(char_buffer) > 2 and t - char_buffer_last_updated > 0.5:
        create_search_results_menu(char_buffer)
    if len(char_buffer) <= 2:
        # collapse search menu
        try:
            current_menu.search_text # Check if menu is search results
            print("setting current_menu (something with search)")
            set_current_menu(current_menu.parent_menu)
        except AttributeError:
            pass

    if State.hide_mouse_time and State.time > State.hide_mouse_time:
        if Mouse.focus:
            # keep cursor visible if mouse has focus
            pass
        else:
            State.hide_mouse_time = 0
            Mouse.set_visible(False)

    idle_events_only = True

    for event in events:
        
        if event.type == pygame.MOUSEMOTION:
            if State.time - State.start_time < 1.0:
                # ignore initial events
                pass
            elif not State.mouse_visible:
                Mouse.set_visible(True)
            State.hide_mouse_time = State.time + 0.250
            mouse_x = event.pos[0] / Render.display_width * 1920
            mouse_y = 1080 - event.pos[1] / Render.display_height * 1080
            last_focus = Mouse.focus
            for item in Mouse.items:
                if not item.enabled:
                    continue
                if mouse_x >= item.x and mouse_x <= item.x + item.w and \
                        mouse_y >= item.y and mouse_y <= item.y + item.h:
                    Mouse.focus = item
                    break
            else:
                Mouse.focus = None
            if last_focus != Mouse.focus:
                Render.dirty = True

        elif event.type == pygame.MOUSEBUTTONDOWN:
            if State.mouse_visible:
                if Mouse.focus:
                    Mouse.focus.activate(State.current_menu)
                    Render.dirty = True

        if event.type == pygame.QUIT:
            #command = pyapp.user.ini.get("Command/Shutdown", "").strip()
            #if command:
            #    # Do not allow ESC to quit if this causes a shutdown
            #    pass
            #else:
            print("State.quit = True")
            State.quit = True

        InputHandler.handle_event(event)

        if event.type == pygame.VIDEORESIZE:
            handle_videoresize_event(event)
        elif event.type == pygame.VIDEOEXPOSE:
            Render.dirty = True

        if event.type == pygame.KEYDOWN:
            # hack to make this work only when using the default
            # input func
            if input_func == default_input_func:
                if event.key == pygame.K_F10:
                    if event.mod & pygame.KMOD_LCTRL:
                        open_terminal()
                if event.key == pygame.K_F5:
                    rescan_games()
                if event.key == pygame.K_F6:
                    Render.display_fps = not Render.display_fps
                    Render.dirty = True
                    Notification("Display FPS:\n" +
                            ("Enabled" if Render.display_fps else "Disabled" ))
                #elif char_buffer and event.key == pygame.K_RETURN:
                #    if character_press("RETURN"):
                #        # was handled
                #        InputHandler.get_button() # clear OK status
                #elif current_game is None and char_buffer and event.key == pygame.K_SPACE:
                #    InputHandler.get_button() # clear OK status
                #    character_press(" ")
                #elif current_game is None and char_buffer and event.key == pygame.K_BACKSPACE:
                #    InputHandler.get_button() # clear OK status
                #    character_press("BACKSPACE")
                #elif barcode_scanner and event.key == pygame.K_LSHIFT:
                #    InputHandler.get_button() # clear SHIFT status
                elif event.unicode and event.unicode in \
                        u"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
                        "abcdefghijklmnopqrstuvwxyz" \
                        "0123456789 -:.,":
                #elif event.unicode:
                    #if event.key < pygame.K_KP0 or event.key > pygame.K_KP9:
                    #    #InputHandler.get_button() # clear SHIFT/button status
                    #    character_press(event.unicode)

                    # key was handled as a virtual button, only allow this
                    # character if already started typing something, or else
                    # navigating with X-Arcade may start searching for games
                    #print(InputHandler.peek_button())
                    if InputHandler.peek_button():
                        if len(char_buffer) > 0:
                            character_press(event.unicode)
                            # reset InputHandler so that virtual button
                            # is not processed, since we handled this press
                            # as a char
                            InputHandler.get_button()
                    else:
                        character_press(event.unicode)

    InputHandler.update()
    AnimationSystem.update()
    NotificationRender.update()

    if idle_events_only:
        # do not update State.idle_from
        pass
    elif len(events) > 0:
        if IDLE:
            State.idle_from = State.time + IDLE

    render_func()

    button = InputHandler.get_button()
    if button:
        #print("InputHandler.get_button", button, "input_func", input_func)
        GameCenter.register_user_activity()
        if input_func:
            input_func(button)
        Render.dirty = True
    if InputHandler.repeat_info:
        Render.dirty = True
    
    #if IDLE:
    if AnimationSystem.is_active():
        #State.idle_from = None
        Render.dirty = True
        #elif State.idle_from is None:
        #    State.idle_from = State.time + IDLE
    if not IDLE:
        Render.dirty = True
    #except KeyboardInterrupt:
    #    print "KeyboardInterrupt"
    #    return
    #return stop_loop

    if not Render.display_sync:
        t = time.time()
        diff = t - Render.display_last_iteration
        #print(diff)
        frame_time = 1 / 60.0
        if diff < frame_time:
            # FIXME: use real refresh rate / frame time
            sleep_time = frame_time - diff
            time.sleep(sleep_time)
        Render.display_last_iteration = t

    return State.quit


def the_main_loop():
    while not State.quit:
        main_loop_iteration()
