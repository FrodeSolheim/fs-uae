from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from fsgs import fsgs
from fengestad.gamecenter.gamecontext import GameContext
from fengestad.gamecenter.GameHandler import GameHandler
from .navigatable import Navigatable
from .opengl import *
from .font import BitmappedFont
from .menu import Menu
from .items import GameCenterItem, HomeItem, MenuItem
from .util import sort_configurations
from .state import State
from .render import Render
from .texture import Texture
from .inputmenu import InputMenu


# FIXME: REMOVE?
class Transition:
    value = 0.0
    start = 0
    end = 0
    on_finish = None


def render_wall(brightness=1.0):
    #glClearColor(0.1, 0.1, 0.1, 1.0)
    #glClear(GL_DEPTH_BUFFER_BIT);
    Render.hd_perspective()
    #fs_emu_ortho();
    #fs_emu_blending(FALSE);
    #fs_emu_texturing(FALSE);
    z = -0.999
    
    #glPushMatrix()
    #glTranslate(0.0, 0.0, 1.0)
    
    # transition y-coordinate between floor and wall
    splt = 361
    fs_emu_blending(False)
    fs_emu_texturing(False)
    glBegin(GL_QUADS)
    
    glColor3f(39.0 / 255.0, 44.0 / 255.0, 51.0 / 255.0)
    glVertex3f(   0, splt, z)
    glVertex3f(1920, splt, z)
    color = 0
    glColor3f(color, color, color)
    glVertex3f(1920, 1020, z)
    glVertex3f(   0, 1020, z)

    glVertex3f(   0, 1020, z)
    glVertex3f(1920, 1020, z)
    glVertex3f(1920, 1080, z)
    glVertex3f(   0, 1080, z)
    
    color = 0
    glColor3f(color, color, color)
    glVertex3f(   0,    0, z)
    glVertex3f(1920,    0, z)
    glColor3f(20.0 / 255.0, 22.0 / 255.0, 26.0 / 255.0)
    glVertex3f(1920, splt, z)
    glVertex3f(   0, splt, z)
    
    glEnd()
    #fs_emu_texturing(True)

    #fs_emu_perspective();
    #double t_x = t0_x + (t1_x - t0_x) * g_menu_transition;
    #double t_y = t0_y + (t1_y - t0_y) * g_menu_transition;
    #double t_z = t0_z + (t1_z - t0_z) * g_menu_transition;
    #double r_a = r0_a + (r1_a - r0_a) * g_menu_transition;
    #
    #glScaled(16.0 / 9.0, 1.0, 1.0);
    #glTranslated(t_x, t_y, t_z);
    #glRotated(r_a, 0.0, 1.0, 0.0);


class GameMenu(Menu):

    def __init__(self, item):
        Menu.__init__(self)

        self.temp_fix_configs(item)

        self.items.append(item)
        self.top.left.append(GameCenterItem())
        self.top.left.append(HomeItem())
        self.top.left.append(MenuItem(item.title))
        
        #self.controller = self.create_controller()
        self.controller = None
        self.create_context()
        self.create_controller()
        print("GameMenu.__init__: controller is", self.controller)

        self.config_list = GameConfigList(self.controller, item)
        self.info_panel = GameInfoPanel()

        self.config_list.info_panel = self.info_panel
        #self.config_list.controller = self.controller
        self.info_panel.config_list = self.config_list

        self.navigatable = self.config_list
        
        # reset transition
        Transition.start = 0

    def temp_fix_configs(self, item):
        from fengestad.game.LocalGameDatabase import LocalGameDatabase
        local_game_database = LocalGameDatabase.get_instance()
        game_database = fsgs.get_game_database()
        variants = local_game_database.find_game_variants_new(
                game_uuid=item.uuid)
        print(variants)
        ordered_list = []
        for variant in variants:

            variant["like_rating"], variant["work_rating"] = \
                game_database.get_ratings_for_game(variant["uuid"])
            variant["personal_rating"], ignored = \
                local_game_database.get_ratings_for_game(variant["uuid"])

            #user_rating = variant[5] or 0
            #global_rating = variant[3] or 0
            #user_rating = 0
            #global_rating = 0

            variant_uuid = variant["uuid"]  # variant[2]
            variant_name = variant["name"]  # variant[1]
            variant_name = variant_name.replace("\n", " (")
            variant_name = variant_name.replace(" \u00b7 ", ", ")
            variant_name += ")"
            ordered_list.append(
                ((1 - bool(variant["have"]),
                  1000 - variant["personal_rating"],
                  1000 - variant["like_rating"]),
                (variant_uuid, variant_name)))
        ordered_list.sort()
        print("ordered variant list:")
        for variant in ordered_list:
            print("-", variant[1][1])
        item.configurations = [x[1] for x in ordered_list]

    def create_context(self):
        item = self.items[0]

        #print("\n\ncreate_context")
        #print("configurations: ", item.configurations)
        #configs = sort_configurations(item.configurations)
        configs = item.configurations
        config = configs[0]
        self.context = GameContext.create_for_game(item.platform, item.name,
                config)

    def create_controller(self):
        old_controller = self.controller
        self.controller = GameHandler.create_for_game_context(
                self.context)

    def recreate_controller(self):
        #self.create_controller()
        #print("***** recreate_controller, new controller is",
        #        id(self.controlleGame Controlr))
        #self.config_list.set_controller(self.controller)
        print("recreate_controller, actually, just recreating game context")
        self.create_context()
        self.controller.context = self.context

    def render(self):
        Transition.value = 0.0
        if Transition.start > 0:
            Transition.value = (State.time - Transition.start) / \
                    (Transition.end - Transition.start)
            # prevent render from stopping when animating
            Render.dirty = True
        # transition goes from 1.0 ... 0.0
        Transition.value = 1.0 - Transition.value
        #finished = 0
        if Transition.value <= 0.0:
            Transition.value = 0.0
            #finished = 1.0
            Transition.start = 0
            Transition.on_finish()
            return
        
        last_menu = State.history[-2]
        self.last_menu_data = last_menu.render()
        
        #self.top_menu_transition = transition
        
        #glClear(GL_DEPTH_BUFFER_BIT)
        #print(transition)
        #glEnable(GL_DEPTH_TEST)
        #glDepthMask(True)
        
        #render_wall(transition)
        #render_screen(transition)
        #if finished:
        #    # FIXME: defer to later via an event or similar?
        return None

    def render_transparent(self, data):
        last_menu = State.history[-2]
        last_menu.render_transparent(self.last_menu_data)
        
        if Transition.value < 1.0:
            opacity = 1.0 - Transition.value
            opacity = opacity * 1.25
            if opacity > 1.0:
                opacity = 1.0
            Render.hd_perspective()
            glDisable(GL_DEPTH_TEST)
            fs_emu_texturing(False)
            fs_emu_blending(True)
            glBegin(GL_QUADS)
            glColor4f(0.0, 0.0, 0.0, opacity)
            glVertex2f(0.0, 0.0)
            glVertex2f(1920.0, 0.0)
            glVertex2f(1920.0, 1020.0)
            glVertex2f(0.0, 1020.0)
            glEnd()
            glEnable(GL_DEPTH_TEST)
        
        self.config_list.render(Transition.value)

    def go_up(self):
        print("GameMenu.go_up")

    def go_down(self):
        print("GameMenu.go_down")

    def go_left(self, count=1):
        print("GameMenu.go_left")

    def go_right(self, count=1):
        print("GameMenu.go_right")

    def activate(self):
        print("GameMenu.activate")


class GameConfigList(Navigatable):

    def __init__(self, controller, game_item):
        self.index = 0
        self.game_item = game_item
        self.items = []
        self.set_controller(controller)

    def set_controller(self, controller):
        self.items = controller.options
        self.index = 0
        # set index to first non-group item
        for i, item in enumerate(self.items):
            if not item.group:
                self.index = i
                break
        self.controller = controller
        print("GameConfigList.set_controller", id(self.controller))

    def go_up(self):
        old_index = self.index
        if self.index > 0:
            new_index = self.index - 1
            while new_index > 0 and self.items[new_index].group:
                new_index -= 1
        else:
            new_index = 0
        if new_index != old_index and not self.items[new_index].group:
            self.index = new_index
        else:
            Navigatable.go_up(self)

    def go_down(self):
        old_index = self.index
        if self.index < len(self.items) - 1:
            new_index = self.index + 1
            while new_index < len(self.items) - 1 and \
                    self.items[new_index].group:
                new_index += 1
        else:
            new_index = len(self.items) - 1
        if new_index != old_index and not self.items[new_index].group:
            self.index = new_index

    def go_left(self, count=1):
        print("GameConfigList.go_left")
        pass

    def go_right(self, count=1):
        pass

    def activate(self):
        result = self.items[self.index].activate()
        if result == "PLAY":
            @staticmethod
            def show_input():
                print("Create input menu, controller = ", id(self.controller))
                new_menu = InputMenu(self.game_item, self.controller)
                State.history.append(new_menu)
                # FIXME
                from .window import set_current_menu
                set_current_menu(new_menu)
                #run_game({"controller": self.controller})
            Transition.start = State.time
            Transition.end = Transition.start + 0.4
            Transition.on_finish = show_input

    def render(self, transition=1.0):
        Render.hd_perspective()
        
        w = 560
        h = 92
        x = 1920 - w * transition
        #y = 1080 - 60 - h
        y = 1080 - 60 - h - 44
        z = -0.9
        
        item_top = 1080 - 90
        item_left = 1920 - 560 + 40
        
        item_left = item_left + Texture.sidebar_background_shadow.w * (
                1.0 - transition)
        
        glPushMatrix()
        glTranslate(0.0, 0.0, 0.7)
        #glTranslated((1.0 - transition) * g_tex_sidebar_background->width, 0, 0)
        #fs_emu_set_texture(g_tex_sidebar_background)
        #fs_emu_render_texture_with_size(g_tex_sidebar_background,
        #        1920 - g_tex_sidebar_background->width,
        #        0, g_tex_sidebar_background->width,
        #        g_tex_sidebar_background->height)
        #fs_emu_render_sidebar()
        fs_emu_blending(True)
        fs_emu_texturing(True)
        glDepthMask(False)
        Texture.sidebar_background_shadow.render(
                1920 - Texture.sidebar_background_shadow.w * transition,
                0, Texture.sidebar_background_shadow.w,
                Texture.sidebar_background_shadow.h)
        glDepthMask(True)
        glPopMatrix()
        
        #x = x + 70
        #w = w - 70
        x = item_left
        y = item_top - 60
        w = 560 - 40 - 40
        h = 60
        
        for i, item in enumerate(self.items):
            if item.group:
                if i > 0:
                    # skip one half row before group heading
                    y -= h / 2
            selected = i == self.index and State.navigatable == self
            z = 0.71
            fs_emu_texturing(False)
            
            if selected:
                fg_color = [1.0, 1.0, 1.0, 1.0]
                glBegin(GL_QUADS)

                glColor3f(0.00, 0x99 / 0xff, 0xcc / 0xff)
                glVertex3f(x, y + 4, z)
                glVertex3f(x + w, y + 4, z)
                glVertex3f(x + w, y + h - 4, z)
                glVertex3f(x, y + h - 4, z)
                #glColor3f(0.6, 0.6, 0.6)
                #glVertex3f(x, y + 4, z)
                #glVertex3f(x + w, y + 4, z)
                #glVertex3f(x + w, y + h, z)
                #glVertex3f(x, y + h, z)
                glEnd()
            else:
                fg_color = [1.0, 1.0, 1.0, 1.0]

            #glBegin(GL_QUADS)
            #glColor3f(0.4, 0.4, 0.4)
            #glVertex3f(x, y, z)
            #glVertex3f(x + w, y, z)
            #glVertex3f(x + w, y + 4, z)
            #glVertex3f(x, y + 4, z)
            #glEnd()
            #fs_emu_texturing(True)

            if i > 0:
                fg_color[3] = fg_color[3] * 0.35

            text = item.title
            
            # FIXME: REMOVE
            #fs_emu_blending(False)
            #fs_emu_blending(True)
            
            glDisable(GL_DEPTH_TEST)            
            if item.group:
                #print("group")
                BitmappedFont.menu_font.render(text, x + 20, y + 14,
                        r=0.0, g=0x99 / 0xff, b=0xcc / 0xff)
            else:
                #print("no --- group")
                BitmappedFont.menu_font.render(text, x + 20, y + 14)
            glEnable(GL_DEPTH_TEST)
            #text = item.title.upper()
            #tw, th = Render.text(text, Font.main_path_font,
            #        x + 40, y + 43, color=fg_color)
            #text = item.subtitle.upper()
            #fg_color[3] = fg_color[3] * 0.4
            #text = item.title.upper()
            #tw, th = Render.text(text, Font.list_subtitle_font,
            #        x + 40, y + 18, color=fg_color)
            y -= h


class GameInfoPanel(Navigatable):

    def __init__(self):
        self.index = 0
        self.position = 0

    def go_up(self):
        print("GameInfoPanel.go_up")
        if self.index == 0 and self.position == 0:
            Navigatable.go_up(self)

    def go_down(self):
        print("GameInfoPanel.go_down")

    def go_left(self, count=1):
        print("GameInfoPanel.go_left")

    def go_right(self, count=1):
        State.navigatable = self.config_list

    def render(self):
        z = -0.999
        fs_emu_texturing(False)
        fs_emu_blending(False)
        glBegin(GL_QUADS)
        glColor3f(0.0, 0.0, 0.0)
        #glVertex3f(0, 365, z)
        #glVertex3f(1270, 365, z)
        glVertex3f(0, 400, z)
        glVertex3f(1270, 400, z)
        glColor3f(0.25, 0.25, 0.25)
        glVertex3f(1270, 1080 - 60, z)
        glVertex3f(0, 1080 - 60, z)
        glEnd()
        #fs_emu_texturing(True)
