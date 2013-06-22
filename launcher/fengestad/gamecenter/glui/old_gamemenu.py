from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

from fengestad.gamecenter.gamecontext import GameContext
from fengestad.gamecenter.gamecontroller import GameController
from .navigatable import Navigatable
from .opengl import *
from .font import Font
from .menu import Menu
from .items import GameCenterItem, HomeItem, MenuItem
from .util import sort_configurations
from .state import State
from .render import Render
from .texture import Texture
from .inputmenu import InputMenu


class Transition:
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
    fs_emu_texturing(True)

    #fs_emu_perspective();
    #double t_x = t0_x + (t1_x - t0_x) * g_menu_transition;
    #double t_y = t0_y + (t1_y - t0_y) * g_menu_transition;
    #double t_z = t0_z + (t1_z - t0_z) * g_menu_transition;
    #double r_a = r0_a + (r1_a - r0_a) * g_menu_transition;
    #
    #glScaled(16.0 / 9.0, 1.0, 1.0);
    #glTranslated(t_x, t_y, t_z);
    #glRotated(r_a, 0.0, 1.0, 0.0);


def render_gloss(alpha):
    Texture.screen_gloss.bind()
    #fs_emu_set_texture(g_tex_gloss);
    fs_emu_texturing(True)
    fs_emu_blending(True)
    #glDisable(GL_DEPTH_TEST)
    #fs_emu_blending(TRUE);
    glColor4f(alpha, alpha, alpha, alpha)
    #/glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 1.0)
    glVertex2f(-1.0, -1.0)
    glTexCoord2d(1.0, 1.0)
    glVertex2f( 1.0, -1.0)
    glTexCoord2d(1.0, 0.0)
    glVertex2f( 1.0,  1.0)
    glTexCoord2d(0.0, 0.0)
    glVertex2f(-1.0,  1.0)
    glEnd()
    #glEnable(GL_DEPTH_TEST)


def render_frame(brightness, alpha = 1.0):
    #float t = g_snes_height / 512.0;
    #fs_emu_log("%d %d %d %d\n", g_crop.x, g_crop.y, g_crop.w, g_crop.h);
    #double s1;
    #double s2;
    #double t1;
    #double t2;
    #double emu_aspect;
    #if (g_fs_emu_video_crop_mode) {
    #    s1 = (double) g_crop.x / g_texture_width;
    #    s2 = (double) (g_crop.x + g_crop.w) / g_texture_width;
    #    t1 = (double) g_crop.y / g_texture_height;
    #    t2 = (double) (g_crop.y + g_crop.h) / g_texture_height;
    #    emu_aspect = (double) g_crop.w / (double) g_crop.h;
    #}
    #else {
    if True:
        s1 = 0.0
        s2 = 1.0 #(double) g_frame_width / g_texture_width;
        t1 = 0.0
        t2 = 1.0 #(double)  g_frame_height / g_texture_height;
    #    emu_aspect = (double) g_frame_width / (double) g_frame_height;
    #}

    aspect = Render.display_width / Render.display_height

    #emu_aspect *= g_video_aspect_ratio;

    x1 = -1.0
    x2 =  1.0
    y1 = -1.0
    y2 =  1.0
    #if (fs_emu_video_get_aspect_correction()) {
    #    double screen_aspect = (double) fs_ml_video_width() /
    #            (double) fs_ml_video_height();
    #    if (emu_aspect > screen_aspect) {
    #        // emu video is wider than screen
    #        double h = screen_aspect / emu_aspect;
    #        //double padding = (2.0 - 2.0 * h) / 2.0;
    #        double padding = 1.0 - h;
    #        y1 += padding;
    #        y2 -= padding;
    #    }
    #    else {
    #        double w = emu_aspect / screen_aspect;
    #        //double padding = (2.0 - 2.0 * w) / 2.0;
    #        double padding = 1.0 - w;
    #        x1 += padding;
    #        x2 -= padding;
    #    }
    #    //x1 = -0.5;
    #    //x2 =  0.5;
    #}

    # render left side in 3d mode
    # FIXME: only render when needed
    fs_emu_blending(True)
    if x1 > -1.0:
        # emu screen does not reach screen edge - side will be black
        fs_emu_texturing(False)
        #glColor4f(0.0, 0.0, 0.0, alpha);
    else:
        # no texture yet
        #fs_emu_texturing(False)
        fs_emu_texturing(True)
        #fs_emu_set_texture(&g_frame_texture);
        #//glColor4f(0.33 * alpha, 0.33 * alpha, 0.33 * alpha, alpha);

    # FIXME:
    #fs_emu_set_texture(&g_frame_texture);
    fs_emu_texturing(True)
    if alpha < 1.0:
        fs_emu_blending(True)
        #fs_emu_blending(TRUE);
        glColor4f(brightness * alpha, brightness * alpha, brightness * alpha,
                alpha)
    else:
        #fs_emu_blending(FALSE);
        fs_emu_blending(False)
        glColor3f(brightness, brightness, brightness)

    glBegin(GL_QUADS)
    glTexCoord2d(s1, t2)
    glVertex2f(x1, y1)
    glTexCoord2d(s2, t2)
    glVertex2f(x2, y1)
    glTexCoord2d(s2, t1)
    glVertex2f(x2,  y2)
    glTexCoord2d(s1, t1)
    glVertex2f(x1,  y2)
    glEnd()

    # if video is not stretched, we render black rectangles to cover
    # the rest of the screen
    if alpha < 1.0:
        fs_emu_blending(True)
        #fs_emu_blending(TRUE)
        glColor4f(0.0, 0.0, 0.0, alpha)
    else:
        fs_emu_blending(True)
        #fs_emu_blending(FALSE);
        glColor3f(0.0, 0.0, 0.0);

    if x1 > -1.0:
        fs_emu_texturing(False)
        #fs_emu_texturing(FALSE);
        glBegin(GL_QUADS)
        glVertex2f(-1.0, y1)
        glVertex2f(  x1, y1)
        glVertex2f(  x1, y2)
        glVertex2f(-1.0, y2)
        glEnd();

    if x2 < 1.0:
        fs_emu_texturing(False)
        #fs_emu_texturing(FALSE);
        glBegin(GL_QUADS)
        glVertex2f(  x2, y1)
        glVertex2f( 1.0, y1)
        glVertex2f( 1.0, y2)
        glVertex2f(  x2, y2)
        glEnd()

    if y1 > -1.0:
        fs_emu_texturing(False)
        #fs_emu_texturing(FALSE);
        glBegin(GL_QUADS)
        glVertex2f(x1, -1.0)
        glVertex2f(x2, -1.0)
        glVertex2f(x2,   y1)
        glVertex2f(x1,   y1)
        # left side (3D)
        glVertex3f(-1.0, -1.0, -0.1)
        glVertex3f(-1.0, -1.0,  0.0)
        glVertex3f(-1.0,   y1,  0.0)
        glVertex3f(-1.0,   y1, -0.1)
        glEnd()

    if y2 < 1.0:
        fs_emu_texturing(False)
        #fs_emu_texturing(FALSE);
        glBegin(GL_QUADS)
        glVertex2f(x1,   y2)
        glVertex2f(x2,   y2)
        glVertex2f(x2,  1.0)
        glVertex2f(x1,  1.0)
        # left side (3D)
        glVertex3f(-1.0,  y2, -0.1)
        glVertex3f(-1.0,  y2,  0.0)
        glVertex3f(-1.0, 1.0,  0.0)
        glVertex3f(-1.0, 1.0, -0.1)
        glEnd()
    """
    if (!g_fs_emu_video_crop_mode) {
        fs_emu_texturing(FALSE);
        fs_emu_blending(TRUE);
        double x1 = (double) g_crop.x / g_frame_width;
        double x2 = (double) (g_crop.x + g_crop.w) / g_frame_width;
        double y1 = (double) g_crop.y / g_frame_height;
        double y2 = (double) (g_crop.y + g_crop.h) / g_frame_height;
        x1 = -1.0 + x1 * 2;
        x2 = -1.0 + x2 * 2;
        y1 = 1.0 - y1 * 2;
        y2 = 1.0 - y2 * 2;

        glColor4f(0.33, 0.0, 0.0, 0.33);

        // left
        glBegin(GL_QUADS);
        glVertex2f(-1.0, -1.0);
        glVertex2f(  x1, -1.0);
        glVertex2f(  x1,  1.0);
        glVertex2f(-1.0,  1.0);
        glEnd();

        // right
        glBegin(GL_QUADS);
        glVertex2f(  x2, -1.0);
        glVertex2f( 1.0, -1.0);
        glVertex2f( 1.0,  1.0);
        glVertex2f(  x2,  1.0);
        glEnd();

        // top
        glBegin(GL_QUADS);
        glVertex2f(  x1,   y1);
        glVertex2f(  x2,   y1);
        glVertex2f(  x2,  1.0);
        glVertex2f(  x1,  1.0);
        glEnd();

        // bottom
        glBegin(GL_QUADS);
        glVertex2f(  x1, -1.0);
        glVertex2f(  x2, -1.0);
        glVertex2f(  x2,   y2);
        glVertex2f(  x1,   y2);
        glEnd();
    }
    """

    fs_emu_texturing(True)
    glBegin(GL_QUADS)
    if x1 > -1.0:
        # emu screen does not reach screen edge - side will be black
        glColor4f(0.0, 0.0, 0.0, alpha) #, 1.)
    else:
        level = 0.33 * brightness * alpha
        glColor4f(level, level, level, alpha) #, alpha)

    glTexCoord2d(s1, t2)
    glVertex3f(-1.0, y1, -0.1)
    glTexCoord2d(s1, t2)
    glVertex2f(-1.0, y1)
    glTexCoord2d(s1, t1)
    glVertex2f(-1.0, y2)
    glTexCoord2d(s1, t1)
    glVertex3f(-1.0, y2, -0.1)
    glEnd()


def render_glow(opacity):
    #const double dx = 0.1 * 9.0 / 16.0;
    dx = 0.15
    dy = 0.15 * 16.0 / 9.0
    z = 0.0
    s = 0.65 * opacity
    glColor4f(s, s, s, s)
    #glColor3f(1.0, 1.0, 1.0);
    fs_emu_blending(True)
    fs_emu_texturing(True)
    # render top edge
    #fs_emu_set_texture(g_tex_glow_top);
    Texture.glow_top.bind()
    glBegin(GL_QUADS)
    glTexCoord2f(0.0, 1.0)
    glVertex3f(-1.0 + dx, 1.0 - dy, z)
    glTexCoord2f(1.0, 1.0)
    glVertex3f( 1.0 - dx, 1.0 - dy, z)
    glTexCoord2f(1.0, 0.0)
    glVertex3f( 1.0 - dx, 1.0 + dy, z)
    glTexCoord2f(0.0, 0.0)
    glVertex3f(-1.0 + dx, 1.0 + dy, z)
    glEnd()
    # render corners
    Texture.glow_top_left.bind()
    glBegin(GL_QUADS)
    # top left corner
    glTexCoord2f(0.0, 1.0)
    glVertex3f(-1.0 - dx, 1.0 - dy, z)
    glTexCoord2f(1.0, 1.0)
    glVertex3f(-1.0 + dx, 1.0 - dy, z)
    glTexCoord2f(1.0, 0.0)
    glVertex3f(-1.0 + dx, 1.0 + dy, z)
    glTexCoord2f(0.0, 0.0)
    glVertex3f(-1.0 - dx, 1.0 + dy, z)
    # top right corner
    glTexCoord2f(1.0, 1.0)
    glVertex3f( 1.0 - dx, 1.0 - dy, z)
    glTexCoord2f(0.0, 1.0)
    glVertex3f( 1.0 + dx, 1.0 - dy, z)
    glTexCoord2f(0.0, 0.0)
    glVertex3f( 1.0 + dx, 1.0 + dy, z)
    glTexCoord2f(1.0, 0.0)
    glVertex3f( 1.0 - dx, 1.0 + dy, z)
    glEnd()
    # render left and right edge
    Texture.glow_left.bind()
    glBegin(GL_QUADS)
    # left edge
    glTexCoord2f(0.0, 1.0)
    glVertex3f(-1.0 - dx, -0.5, z)
    glTexCoord2f(1.0, 1.0)
    glVertex3f(-1.0 + dx, -0.5, z)
    glTexCoord2f(1.0, 0.0)
    glVertex3f(-1.0 + dx, 1.0 - dy, z)
    glTexCoord2f(0.0, 0.0)
    glVertex3f(-1.0 - dx, 1.0 - dy, z)
    # right edge
    glTexCoord2f(1.0, 1.0)
    glVertex3f( 1.0 - dx, -0.5, z)
    glTexCoord2f(0.0, 1.0)
    glVertex3f( 1.0 + dx, -0.5, z)
    glVertex3f( 1.0 + dx, 1.0 - dy, z)
    glTexCoord2f(1.0, 0.0)
    glVertex3f( 1.0 - dx, 1.0 - dy, z)
    # left edge bottom
    glColor4f(0.0, 0.0, 0.0, 0.0)
    glTexCoord2f(0.0, 1.0)
    glVertex3f(-1.0 - dx, -1.0, z)
    glTexCoord2f(1.0, 1.0)
    glVertex3f(-1.0 + dx, -1.0, z)
    glColor4f(s, s, s, s)
    glTexCoord2f(1.0, 0.0)
    glVertex3f(-1.0 + dx, -0.5, z)
    glTexCoord2f(0.0, 0.0)
    glVertex3f(-1.0 - dx, -0.5, z)
    # right edge bottom
    glColor4f(0.0, 0.0, 0.0, 0.0)
    glTexCoord2f(1.0, 1.0)
    glVertex3f( 1.0 - dx, -1.0, z)
    glTexCoord2f(0.0, 1.0)
    glVertex3f( 1.0 + dx, -1.0, z)
    glColor4f(s, s, s, s)
    glVertex3f( 1.0 + dx, -0.5, z)
    glTexCoord2f(1.0, 0.0)
    glVertex3f( 1.0 - dx, -0.5, z)
    glEnd()


def render_screen(transition=1.0):
    Render.standard_perspective()
    
    t0_x = 0.0;
    t0_y = 0.0;
    t0_z = -2.42;
    r0_a = 0.0;

    t1_x = -0.31;
    t1_y = -0.08;
    t1_z = -3.7;
    r1_a = 30.0;

    perspective = 0
    
    if transition == 0.0:
        perspective = 0
        #fs_emu_ortho()
        #glTranslated(1920.0 / 2.0, 1080.0 / 2.0, 0.0)
        #glScaled(1920.0 / 2.0, 1080.0 / 2.0, 1.0)
    else:
        perspective = 1

        t_x = t0_x + (t1_x - t0_x) * transition
        t_y = t0_y + (t1_y - t0_y) * transition
        t_z = t0_z + (t1_z - t0_z) * transition
        r_a = r0_a + (r1_a - r0_a) * transition

        glScaled(16.0 / 9.0, 1.0, 1.0)
        glTranslated(t_x, t_y, t_z)
        glRotated(r_a, 0.0, 1.0, 0.0)

    #glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)

    if perspective:
        glDepthMask(False)
        render_glow(transition)
        glDepthMask(True)
    #glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

    item = State.current_menu.selected_item
    texture = item.get_screen_texture(1)
    if texture:
        coords = 0.0, 1.0, 1.0, 0.0
    else:
        # FIXME
        from .bottombar import static
        texture, coords = static()
    texture.bind()
    #glDepthMask(False)
    render_frame(1.0 * transition)
    if perspective:
        glDisable(GL_DEPTH_TEST)
        render_gloss(transition)
        glEnable(GL_DEPTH_TEST)
        glTranslatef(0.0, -2.0, 0.0)
        #glTranslatef(0.0, -1.0, 0.0)
        #glScalef(1.0, -1.0, 1.0)
        glScalef(1.0, -0.5, 1.0)
        glTranslatef(0.0, -1.0, 0.0)
        texture.bind()
        render_frame(1.0 * transition, 0.33)

    #glDepthMask(True)

    """
    fs_emu_texturing(False)
    fs_emu_blending(False)
    glColor3f(1.0, 0.0, 0.0)
    glBegin(GL_QUADS)
    glVertex2f(-1.0, -1.0)
    glVertex2f( 1.0, -1.0)
    glVertex2f( 1.0,  1.0)
    glVertex2f(-1.0, 1.0)
    glEnd()
    """


class GameMenu(Menu):

    def __init__(self, item):
        Menu.__init__(self)
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

    def create_context(self):
        item = self.items[0]

        #print("\n\ncreate_context")
        #print("configurations: ", item.configurations)
        configs = sort_configurations(item.configurations)
        config = configs[0]
        self.context = GameContext.create_for_game(item.platform, item.name,
                config)

    def create_controller(self):
        old_controller = self.controller
        self.controller = GameController.create_for_game_context(
                self.context)

    def recreate_controller(self):
        #self.create_controller()
        #print("***** recreate_controller, new controller is",
        #        id(self.controller))
        #self.config_list.set_controller(self.controller)
        print("recreate_controller, actually, just recreating game context")
        self.create_context()
        self.controller.context = self.context

    def render(self):
        transition = 0.0
        if Transition.start > 0:
            transition = (State.time - Transition.start) / \
                    (Transition.end - Transition.start)
            # prevent render from stopping when animating
            Render.dirty = True
        # transition goes from 1.0 ... 0.0
        transition = 1.0 - transition
        #finished = 0
        if transition <= 0.0:
            transition = 0.0
            #finished = 1.0
            Transition.start = 0
            Transition.on_finish()
            return
        
        #self.top_menu_transition = transition
        
        #glClear(GL_DEPTH_BUFFER_BIT)
        #print(transition)
        #glEnable(GL_DEPTH_TEST)
        #glDepthMask(True)
        
        render_wall(transition)
        render_screen(transition)
        self.config_list.render(transition)
        #if finished:
        #    # FIXME: defer to later via an event or similar?
        return None

    def render_transparent(self, data):
        #render_item_shadows(data)
        pass

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
        self.controller = controller
        print("GameConfigList.set_controller", id(self.controller))

    def go_up(self):
        if self.index > 0:
            self.index -= 1
        else:
            Navigatable.go_up(self)

    def go_down(self):
        if self.index < len(self.items) - 1:
            self.index += 1

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
        w = Texture.sidebar_background.w
        h = 92
        x = 1920 - w * transition
        #y = 1080 - 60 - h
        y = 1080 - 60 - h - 44
        z = -0.8
        
        glPushMatrix()
        #glTranslated((1.0 - transition) * g_tex_sidebar_background->width, 0, 0)
        #fs_emu_set_texture(g_tex_sidebar_background)
        #fs_emu_render_texture_with_size(g_tex_sidebar_background,
        #        1920 - g_tex_sidebar_background->width,
        #        0, g_tex_sidebar_background->width,
        #        g_tex_sidebar_background->height)
        #fs_emu_render_sidebar()
        fs_emu_blending(True)
        fs_emu_texturing(True)
        Texture.sidebar_background.render(
                1920 - Texture.sidebar_background.w * transition,
                0, Texture.sidebar_background.w, Texture.sidebar_background.h)
        glPopMatrix()
        
        x = x + 70
        w = w - 70
        
        for i, item in enumerate(self.items):
            selected = i == self.index and State.navigatable == self
            z = 0.1
            fs_emu_texturing(False)
            if selected:
                fg_color = [0.0, 0.0, 0.0, 1.0]
                glBegin(GL_QUADS)
                glColor3f(0.6, 0.6, 0.6)
                glVertex3f(x, y + 4, z)
                glVertex3f(x + w, y + 4, z)
                glColor3f(0.85, 0.85, 0.85)
                glVertex3f(x + w, y + h, z)
                glVertex3f(x, y + h, z)
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
            fs_emu_texturing(True)

            if i > 0:
                fg_color[3] = fg_color[3] * 0.35

            text = item.title.upper()
            tw, th = Render.text(text, Font.main_path_font,
                    x + 40, y + 43, color=fg_color)
            text = item.subtitle.upper()
            fg_color[3] = fg_color[3] * 0.4
            text = item.title.upper()
            tw, th = Render.text(text, Font.list_subtitle_font,
                    x + 40, y + 18, color=fg_color)
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
        fs_emu_texturing(True)
