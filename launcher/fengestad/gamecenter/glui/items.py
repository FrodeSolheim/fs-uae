from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import math
import random
from numpy import array, float32
import fengestad.fs as fs
from fengestad.gamecenter.database import Database
from fengestad.gamecenter.config import Config
from fengestad.gamecenter.nameutil import NameUtil
from fengestad.gamecenter.resources import resources, logger, ngettext, _
from .font import Font, BitmappedFont
from .opengl import *
from .render import Render
from .state import State
from .texture import Texture
from .texturemanager import TextureManager
from .errordialog import show_exception

from fengestad.game.LocalGameDatabase import LocalGameDatabase

from .constants import TOP_ITEM_ARROW, TOP_ITEM_LEFT
from .constants import TOP_ITEM_RIGHT, ROW_NAME, ROW_PLATFORM
from .constants import TOP_HEIGHT_VISIBLE

LIGHTING = Config.get_bool("Menu/Lighting", False)
DIRTY_WHILE_NOT_LOADED = True

def create_item_menu(name):
    from .itemmenu import ItemMenu
    return ItemMenu(name)

def create_mesh(width, height, upsidedown):
    hw = width / 2
    hh = height / 2
    if LIGHTING:
        res = 200
        radius = 0.018
    else:
        res = 2
        radius = 0.0
    #radius = 0.1
    mesh = []
    for y in range(0, res + 1):
        y = -hh + y * height / res
        line = []
        mesh.append(line)
        for x in range(0, res + 1):
            x = -hw + x * width / res
            #d = min(x, y, res- x, res -y) / (res / 2.0)
            #rx = x * 2.0 / res
            z = radius
            nx = 0.0
            ny = 0.0
            nz = 1.0
            #edge = False
            # using max to account for rounding errors - d cannot be negative
            d = max(0, hw - abs(x))
            #print(hw, x, d)
            if d < radius:
                d = radius - d
                #d = radius - 1.0 - x
                #print(d, d / radius)
                a = math.acos(d / radius)
                #print(a)
                z = math.sin(a) * radius
                nx = -d if x < 0 else d
                nz = z
                #edge = True
            # using max to account for rounding errors - d cannot be negative
            d = max(0, hh - abs(y))
            if d < radius:
                d = radius - d
                #d = radius - 1.0 - x
                #print(d, d / radius)
                a = math.acos(d / radius)
                #print(a)
                z = min(z, math.sin(a) * radius)
                ny = -d if y < 0 else d
                nz = z
                #edge = True
            nl = math.sqrt(nx ** 2 + ny ** 2 + nz ** 2)
            nx = nx / nl
            ny = ny / nl
            nz = nz / nl
            line.append((x, y, z, nx, ny, nz))
            #if edge:
            #    line.append((x, y, -z, nx, ny, -nz))
    return mesh

@fs.memoize
def get_vbo_for_cover(width, height):
    floats = []
    upsidedown=False
    mesh = create_mesh(width, height, upsidedown)
    hw = width / 2
    hh = height / 2
    mesh_size = len(mesh)
    print(mesh_size)
    count = 0
    for iy in range(mesh_size - 1):
        for ix in range(mesh_size - 1):
            def vertex(ix, iy):
                x, y, z, nx, ny, nz = mesh[iy][ix]
                if upsidedown:
                    s, t = (x + hw) / width, (y + hh) / height
                else:
                    s, t = (x + hw) / width, 1.0 - (y + hh) / height
                floats.extend([s, t, nx, ny, nz, x, y, z])
            vertex(ix, iy)
            vertex(ix + 1, iy)
            vertex(ix + 1, iy + 1)
            vertex(ix, iy + 1)
            count += 4
    # FIXME: specify usage with usage kw arg (specify as "static" data, used often)
    vbo = VBO(array(floats, dtype=float32))
    return vbo, count

def render_cover(texture, width, height):
    if not texture:
        texture = Texture.missing_cover
    texture.bind()
    x = -width / 2.0
    #y = height / 2.0 - 0.5 - height / 2.0
    y = -0.5
    #if reflection:
    
    glBegin(GL_QUADS)
    glColor3f(0.33, 0.33, 0.33)
    glTexCoord2f(0.0, 1.0)
    glVertex2f(x, y)
    glTexCoord2f(1.0, 1.0)
    glVertex2f(x + width, y)
    glTexCoord2f(1.0, 0.0)
    glVertex2f(x + width, y - height)
    glTexCoord2f(0.0, 0.0)
    glVertex2f(x, y - height)
    glEnd()
    
    #else:
    glBegin(GL_QUADS)
    glColor3f(1.0, 1.0, 1.0)
    glTexCoord2f(0.0, 1.0)
    glVertex2f(x, y)
    glTexCoord2f(1.0, 1.0)
    glVertex2f(x + width, y)
    glTexCoord2f(1.0, 0.0)
    glVertex2f(x + width, y + height)
    glTexCoord2f(0.0, 0.0)
    glVertex2f(x, y + height)
    glEnd()

    #glPushMatrix()
    #glTranslatef(0.0, height / 2.0 - 0.5, 0.0)
    #vbo, count = get_vbo_for_cover(width, height)
    #vbo.bind()
    #glInterleavedArrays(GL_T2F_N3F_V3F, 0, None)
    #if reflection:
    #    glScalef(1.0, -1.0, 1.0)
    #    glTranslatef(0.0, height, 0.0)
    #    if LIGHTING:
    #        # No point in drawing everything
    #        glDrawArrays(GL_QUADS, 0, count // 4)
    #    else:
    #        glDrawArrays(GL_QUADS, 0, count)
    #else:
    #    glDrawArrays(GL_QUADS, 0, count)
    #vbo.unbind()
    #glPopMatrix()


class MenuItem(object):

    def __init__(self, title=""):
        self.ratio = 0.75
        self.title = title
        self.supertitle = ""
        self.subtitle = ""
        self.path_title = title
        self.path_title_active = ""
        # x, y, w and h are used by top menu items
        self.x = 0
        self.y = 0
        self.w = 0
        self.h = 0
        self.enabled = True

    def activate(self, menu):
        print("Activate not overidden")

    @fs.memoize
    def get_image_files(self):
        path = self.get_image_path()
        if path:
            return [path]
        return []

    def get_default_texture(self):
        return Texture.default_item

    def get_texture(self):
        path = self.get_image_path()
        if path:
            try:
                texture = TextureManager.get().get_texture(path)
            except KeyError:
                texture = None
            if texture:
                if texture is Texture.default_item:
                    return Texture.missing_cover
                return texture
            else:
                Render.dirty = DIRTY_WHILE_NOT_LOADED
        return Texture.missing_cover
        #return self.get_default_texture()

    @fs.memoize
    def get_image_path(self):
        name = self.get_resource_name()
        if name:
            try:
                return resources.resource_filename(
                        os.path.join(u"item", name + u".png"))
            except Exception:
                pass
        return None

    def get_resource_name(self):
        return None

    def render(self, width, height, border_color=(0.0, 0.0, 0.0, 1.0),
            inner_border_color=(0.92, 0.92, 0.92), height_offset=0.0,
            brightness=1.0, ratio=None, area=None):
        render_cover(self.get_texture(), width, height)

    def create_menu_path(self, menu):
        path = []
        #while menu:
        if not menu:
            return path
        path.append(menu.selected_item)
        for p in reversed(menu.parents):
            #path.insert(0, menu.selected_item)
            path.insert(0, p.selected_item)
            #menu = menu.parent_menu
        print("create_menu_path", path)
        for p in path:
            print(p.path_title)
        return path

    def get_category_filter(self):
        return None

    def get_game_filter(self):
        return None

    def generate_category_filters(self, menu_path):
        for item in menu_path:
            flt = item.get_category_filter()
            if flt:
                yield flt

    def generate_game_filters(self, menu_path):
        for item in menu_path:
            flt = item.get_game_filter()
            if flt:
                yield flt

    def generate_category_items(self, menu_path):
        category_filters = list(self.generate_category_filters(menu_path))
        def check(cat):
            for flt in category_filters:
                if flt(cat) == False:
                    return False
            return True
        categories = [
                PlatformMenuItem(),
                LetterMenuItem(),
                ShuffleMenuItem(),
                ListMenuItem(),
                #YearMenuItem(),
                #KeywordMenuItem(),
                ]
        if len(menu_path) == 0:
            categories.insert(0, AllMenuItem())
        return [item for item in categories if check(item)]

    def generate_game_items(self, menu_path):
        args = []
        clause = []
        for item in menu_path:
            c, a = item.get_filter_clause()
            if c:
                clause.append(c)
                args.extend(a)
        clause = " ".join(clause)
        
        filters = list(self.generate_game_filters(menu_path))
        print("filters for", menu_path, "-", filters)
        return self.create_game_items(clause, args, filters)

    @classmethod
    def create_game_items(cls, clause, args, filters=[]):
        #query = "SELECT id, name, config, platform, " \
        #        "imageratio, screenratio, year, publisher, developer, subtitle " \
        #       "FROM Game WHERE 1 = 1 {0} ORDER BY sort_key".format(clause)
        #print(query, args)
        #Database.cursor.execute(query, args)
        #item_list = []
        #last_item = None
        #for row in Database.cursor:
        #    if last_item:
        #        if last_item.name == row[1] and last_item.platform == row[3]:
        #            last_item.configurations.append(row[2])
        #            continue
        #    for filter in filters:
        #        if not filter(row):
        #            break
        #    else:
        #        item = GameItem(id=row[0], name=row[1], configurations=[row[2]],
        #                platform=row[3], ratio=row[4], screenratio=row[5],
        #                year=row[6], publisher=row[7], developer=row[8],
        #                subtitlepos=row[9])
        #        item_list.append(item)
        #        last_item = item
        #return item_list

        item_list = []
        local_game_database = LocalGameDatabase.get_instance()
        for game in local_game_database.find_games_new():
            item = GameItem(game)
            item_list.append(item)
        return item_list

    def get_filter_clause(self):
        return "", []

    def update_size_left(self):
        self.update_size(self.get_top_left_text())

    def update_size_right(self):
        self.update_size(self.get_top_right_text())

    def update_size(self, text):
        #w, h = Render.measure_text(text, Font.main_path_font)
        w, h = BitmappedFont.title_font.measure(text)
        self.w = w + 20 + 20

    def get_top_left_text(self):
        return self.path_title_active.upper() or self.path_title.upper()

    def get_top_right_text(self):
        return self.title.upper()

    def render_top_background(self, selected, style=TOP_ITEM_ARROW):
        x, y, w, h = self.x, self.y, self.w, self.h
        z = -0.01 - 0.01 * x / 1920
        if selected:
            fs_emu_texturing(False)
            fs_emu_blending(False)
            glBegin(GL_QUADS)
            glColor3f(0.00, 0x99 / 0xff, 0xcc / 0xff)
            glVertex3f(x + 4, y + 4, z)
            glVertex3f(x + w - 4, y + 4, z)
            glVertex3f(x + w - 4, y + h - 4, z)
            glVertex3f(x + 4, y + h - 4, z)
            glEnd()
            
        """
        fs_emu_blending(True)
        glColor3f(1.0, 1.0, 1.0)

        #z = -0.01

        if style == TOP_ITEM_LEFT:
            if selected:
                texture = Texture.top_item_left_selected
            else:
                texture = Texture.top_item_left
            texture.bind()
            tw, th = texture.size
            w = 60

            glBegin(GL_QUADS)
            glTexCoord2f(0.0, 1.0)
            glVertex3f(x, y, z)
            glTexCoord2f(1.0, 1.0)
            glVertex3f(x + w, y, z)
            glTexCoord2f(1.0, 0.0)
            glVertex3f(x + w, y + h, z)
            glTexCoord2f(0.0, 0.0)
            glVertex3f(x, y + h, z)
            glEnd()
            x += 60

        w = self.w - 60
       
        if selected:
            texture = Texture.top_item_selected
            x = x - 60
            w = w + 60
        else:
            texture = Texture.top_item
        texture.bind()
        tw, th = texture.size
        glBegin(GL_QUADS)
        glTexCoord2f(0.0, 1.0)
        glVertex3f(x, y, z)
        glTexCoord2f(1.0, 1.0)
        glVertex3f(x + w, y, z)
        glTexCoord2f(1.0, 0.0)
        glVertex3f(x + w, y + h, z)
        glTexCoord2f(0.0, 0.0)
        glVertex3f(x, y + h, z)
        glEnd()

        if style == TOP_ITEM_ARROW or style == TOP_ITEM_RIGHT:
            if style == TOP_ITEM_ARROW:
                if selected:
                    texture = Texture.top_item_arrow_selected
                else:
                    texture = Texture.top_item_arrow
                texture.bind()
                tw, th = texture.size
            else:
                Texture.top_item_right.bind()
                tw, th = Texture.top_item_right.size
            x = x + w
            w = 60

            glBegin(GL_QUADS)
            glTexCoord2f(0.0, 1.0)
            glVertex3f(x, y, z)
            glTexCoord2f(1.0, 1.0)
            glVertex3f(x + w, y, z)
            glTexCoord2f(1.0, 0.0)
            glVertex3f(x + w, y + h, z)
            glTexCoord2f(0.0, 0.0)
            glVertex3f(x, y + h, z)
            glEnd()

        fs_emu_blending(False)
        """

    def render_top_left(self, selected=False):
        self.render_top_background(selected)
        text = self.get_top_left_text()
        self.render_top(text, selected)

    def render_top_right(self, selected=False):
        self.render_top_background(selected, style=TOP_ITEM_LEFT)
        self.render_top(self.get_top_right_text(), selected)

    def render_top(self, text="", selected=False, right_align=False):
        if right_align:
            tw, th = BitmappedFont.title_font.measure(text)
            x = self.x + self.w - 20 - tw
        else:
            x = self.x + 20
            #if selected:
            #    color = (0.0, 0.0, 0.0)
            #else:
            #    color = (0.85, 0.85, 0.85)
            #tw, th = Render.text(text, Font.main_path_font,
            #        x, self.y + 30, h=TOP_HEIGHT_VISIBLE, color=color)
        BitmappedFont.title_font.render(text, x, self.y + 14)
        #print(BitmappedFont.title_font.h)

    def get_screen_texture(self, n):
        return Texture.missing_screenshot
        #return None


class AutoExpandItem(MenuItem):

    def activate(self, menu):
        new_menu = create_item_menu(self.title)
        menu_path = self.create_menu_path(menu)
        new_menu.update_path(menu_path)
        want_selected = len(new_menu)
        for item in self.generate_game_items(menu_path):
            new_menu.append(item)
        if len(new_menu) > want_selected:
            new_menu.set_selected_index(want_selected, immediate=True)
        new_menu.add_add_item()
        return new_menu


class DummyItem(MenuItem):

    def __init__(self, title):
        MenuItem.__init__(self)
        self.title = title


class NoItem(MenuItem):

    def __init__(self, title="N/A"):
        MenuItem.__init__(self)
        self.title = title

    def get_default_texture(self):
        return Texture.missing_cover

    def activate(self, menu):
        return


class NoLastPlayedItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("No Last Played")


class GameCenterItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("Game   Center")
        self.path_title = self.title

    def activate(self, menu):
        pass

    #def update_size(self, text):
    #    MenuItem.update_size(text)
    #    #self.w = Texture.top_logo.w + 83

    def render_top_left(self, selected=False):
        #self.render_top_background(selected, style=TOP_ITEM_ARROW)
        MenuItem.render_top_left(self, selected=selected)
        glDisable(GL_DEPTH_TEST)
        fs_emu_blending(True)
        x = 138
        y = 14
        #if selected:
        #texture = Texture.top_logo_selected
        #else:
        texture = Texture.top_logo
        texture.render(x, 1080 - y - texture.h, texture.w, texture.h)
        #fs_emu_blending(False)
        glEnable(GL_DEPTH_TEST)

        
class HomeItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("Home")
        self.path_title = self.title

    def activate(self, menu):
        from .window import create_main_menu
        new_menu = create_main_menu()
        #State.history = [new_menu]
        State.history.append(new_menu)
        from .window import set_current_menu
        set_current_menu(new_menu)

    def update_size_left(self):
        self.w = 80

    def render_top_left(self, selected=False):
        self.render_top_background(selected)
        #fs_emu_blending(True)
        if selected:
            texture = Texture.home_selected
        else:
            texture = Texture.home
        texture.render(self.x, self.y, texture.w, texture.h)


def get_game_lists_dirs():
    if os.path.exists(os.expanduser("~/Games/Lists")):
        return [os.expanduser("~/Games/Lists")]
    return []


def get_game_lists():
    result = []
    lists_dirs = get_game_lists_dirs()
    for lists_dir in lists_dirs:
        for name in os.listdir(lists_dir):
            if name.endswith(".txt"):
                title = name[:-4]
                result.append((title, os.path.join(lists_dir, name)))
    return result


class ListMenuItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("List")
        self.path_title_active = "Choose List"

    def get_resource_name(self):
        return "list"

    def activate(self, menu):
        print("ListMenuItem.activate")
        new_menu = create_item_menu(_("Select List"))
        menu_path = self.create_menu_path(menu)
        new_menu.update_path(menu_path)
        #Database.cursor.execute("SELECT DISTINCT platform FROM Game "
        #        "ORDER BY platform")
        #items = []
        #for row in Database.cursor:
        #    item = PlatformItem(row[0])
        #    items.append((item.sort_title, item))
        #for sort_title, item in sorted(items):
        #   new_menu.append(item)
        
        for title, path in get_game_lists():
            item = ListItem(title, path)
            new_menu.append(item)
        return new_menu


class ListItem(AutoExpandItem):

    def __init__(self, title, path):
        AutoExpandItem.__init__(self)
        self.list_path = path
        self.title = title
        #self.platform = platform
        self.path_title = title
        self.subtitle = ""
        self.sort_title = self.title
        #self.game_filter_set = set()

    def get_category_filter(self):
        def category_filter(category):
            if category.__class__ == ListMenuItem:
                return False
            return True
        return category_filter

    @fs.memoize
    def get_list_contents(self):
        result = set()
        print("read_list_contents", self.list_path)
        with open(self.list_path, "rb") as f:
            for line in f.readlines():
                line = line.strip()
                if not line:
                    continue
                print(line)
                parts = line.rsplit("(", 1)
                name = parts[0]
                platform = None
                if len(parts) == 2:
                    parts = parts[1][:-1].split(",", 1)
                    platform = parts[0]
                    if len(parts) == 2:
                        # FIXME variant...
                        pass
                print(name, platform)
                cmp_game = NameUtil.create_cmpname(name)
                if platform:
                    cmp_platform = NameUtil.create_cmpname(platform)
                    result.add((cmp_game, cmp_platform))
                else:
                    result.add(cmp_game)
        return result

    def get_game_filter(self):
        def game_filter(row):
            #print(game_info)
            game_filter_set = self.get_list_contents()
            #print(self.game_filter_set)
            cmp_game = NameUtil.create_cmpname(row[ROW_NAME])
            #print("check -- ", cmp_game)
            if cmp_game in game_filter_set:
                return True
            cmp_platform = NameUtil.create_cmpname(row[ROW_PLATFORM])
            #print("check -- ", (cmp_game, cmp_platform))
            if (cmp_game, cmp_platform) in game_filter_set:
                return True
            return False
        return game_filter

    #@fs.memoize
    #def get_resource_name(self):
    #    name = ""
    #    for c in self._platform.lower():
    #        if c in "abcdefghijklmnopqrstuvwxyz0123456789":
    #            name = name + c
    #    return name

    #def get_filter_clause(self):
    #    # FIXME: NUMBERS
    #    return "AND platform = ?", [self._platform]
    #    print(name)


class PlatformMenuItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("Platform")
        self.path_title_active = "Choose Platform"

    def get_resource_name(self):
        return "platform"

    def activate(self, menu):
        print("PlatformMenuItem.activate")
        new_menu = create_item_menu(_("Select Platform"))
        menu_path = self.create_menu_path(menu)
        new_menu.update_path(menu_path)
        Database.cursor.execute("SELECT DISTINCT platform FROM Game "
                "ORDER BY platform")
        items = []
        for row in Database.cursor:
            item = PlatformItem(row[0])
            items.append((item.sort_title, item))
        for sort_title, item in sorted(items):
            new_menu.append(item)
        return new_menu


class PlatformItem(AutoExpandItem):

    def __init__(self, platform):
        AutoExpandItem.__init__(self)
        self._platform = platform
        # using self.platform for "description"
        self.sort_title = ""
        try:
            self.title, self.subtitle, self.sort_title = {
                "Commodore 64": (
                        "Commodore 64",
                        "",
                        "Commodore 1"),
                "Amiga": (
                        "Amiga",
                        "Commodore Amiga",
                        "Commodore 2"),
                "Amstrad CPC": (
                        "Amstrad CPC",
                        "Color Personal Computer",
                        ""),
                "Apple II": (
                        "Apple II",
                        "II/Plus/e/c",
                        ""),
                "Arcade": (
                        "Arcade Video Games",
                        "Multiple Arcade Machines",
                        ""),
                "Atari 2600": (
                        "Atari 2600",
                        "Video Computer System",
                        "Atari 1"),
                "Atari 8-Bit": (
                        "Atari 8-Bit",
                        "Atari 400/800/XL/XE",
                        "Atari 2"),
                "Atari 5200": (
                        "Atari 5200",
                        "Atari 5200 SuperSystem",
                        "Atari 3"),
                "Atari 7800": (
                        "Atari 7800",
                        "Atari 7800 ProSystem",
                        "Atari 4"),
                "Atari ST": (
                        "Atari ST",
                        "Atari ST/STE",
                        "Atari 5"),
                "Atari Lynx": (
                        "Atari Lynx",
                        "",
                        "Atari 6"),
                "BBC Micro": (
                        "BBC Micro",
                        "Acorn BCC Micro",
                        ""),
                "Nintendo": (
                        "Nintendo",
                        "Entertainment System",
                        "Nintendo 1"),
                "Game Boy": (
                        "Game Boy",
                        "",
                        "Nintendo 2"),
                "Super Nintendo": (
                        "Super Nintendo",
                        "Entertainment System",
                        "Nintendo 3"),
                "Nintendo 64": (
                        "Nintendo 64",
                        "",
                        "Nintendo 4"),
                "Game Boy Color": (
                        "Game Boy Color",
                        "",
                        "Nintendo 5"),
                "Game Boy Advance": (
                        "Game Boy Advance",
                        "",
                        "Nintendo 6"),
                "GameCube": (
                        "GameCube",
                        "Nintendo GameCube",
                        "Nintendo 7"),
                "Wii": (
                        "Wii",
                        "Nintendo Wii",
                        "Nintendo 8"),
                "DOS": (
                        "DOS",
                        "IBM PC Compatible",
                        "PC 1"),
                "Windows": (
                        "Windows",
                        "IBM PC Compatible",
                        "PC 2"),
                "PlayStation": (
                        "PlayStation",
                        "Sony PlayStation",
                        ""),
                "PlayStation 2": (
                        "PlayStation 2",
                        "Sony PlayStation 2",
                        ""),
                "Master System": (
                        "Master System",
                        "Sega Master System / Mark III",
                        "Sega 1"),
                "Mega Drive": (
                        "Mega Drive",
                        "Sega Mega Drive / Genesis",
                        "Sega 2"),
                "Game Gear": (
                        "Game Gear",
                        "Sega Game Gear",
                        "Sega 3"),
                "TurboGrafx-16": (
                        "TurboGrafx-16",
                        "Entertainment SuperSystem",
                        ""),
                "ZX Spectrum": (
                        "ZX Spectrum",
                        "Sinclair ZX Spectrum",
                        ""),
            }[platform]
        except KeyError:
            self.title = platform
        self.platform = platform
        self.path_title = platform

        self.title = ""
        self.subtitle = ""

        #if not self.sort_title:
        #    self.sort_title = self.title

        # FIXME: for now, test using just title as sort title
        self.sort_title = self.title

    def get_category_filter(self):
        def category_filter(category):
            if category.__class__ == PlatformMenuItem:
                return False
            return True
        return category_filter

    #def get_game_filter(self):
    #    def game_filter(game_info):
    #        if game_info.platform == self._platform:
    #            return True
    #        return False
    #    return game_filter

    @fs.memoize
    def get_resource_name(self):
        name = ""
        for c in self._platform.lower():
            if c in "abcdefghijklmnopqrstuvwxyz0123456789":
                name = name + c
        return name

    def get_filter_clause(self):
        # FIXME: NUMBERS
        return "AND platform = ?", [self._platform]
        print(name)


class AllMenuItem(AutoExpandItem):

    def __init__(self):
        AutoExpandItem.__init__(self)
        self.title = _("All")
        self.path_title = "All Games"

    def get_resource_name(self):
        return "all"

    def get_category_filter(self):
        def category_filter(category):
            return False
        return category_filter

    #def get_game_filter(self):
    #    def game_filter(game_info):
    #        return True
    #    return game_filter


class AddItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("Add")
        self.path_title = self.title

    def update_size_left(self):
        self.w = 80

    def render_top_left(self, selected=False):
        self.render_top_background(selected)
        #fs_emu_blending(True)
        if selected:
            texture = Texture.add_selected
        else:
            texture = Texture.add
        texture.render(self.x, self.y, texture.w, texture.h)

    def activate(self, menu):
        new_menu = create_item_menu(self.title)
        menu_path = self.create_menu_path(menu)
        new_menu.update_path(menu_path)
        for item in self.generate_category_items(menu_path):
            new_menu.append(item)
        return new_menu


class ShuffleMenuItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("Shuffle")
        self.path_title = "Shuffle"
        # FIXME: Sort game list in random order

    def get_resource_name(self):
        return "shuffle"

    def activate(self, menu):
        new_menu = create_item_menu(self.title)
        menu_path = self.create_menu_path(menu)
        new_menu.update_path(menu_path)
        game_items = list(self.generate_game_items(menu_path))
        random.shuffle(game_items)
        for item in game_items:
            new_menu.append(item)
        return new_menu


class LetterMenuItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("Letter")
        self.path_title_active = "Choose Letter"

    def get_resource_name(self):
        return "letter"

    def activate(self, menu):
        print("LetterMenuItem.activate")
        new_menu = create_item_menu(_("Select First Letter"))
        menu_path = self.create_menu_path(menu)
        new_menu.update_path(menu_path)
        for letter in "#ABCDEFGHIJKLMNOPQRSTUVWXYZ":
            item = LetterItem(letter)
            new_menu.append(item)
        return new_menu


class LetterItem(AutoExpandItem):

    def __init__(self, letter):
        AutoExpandItem.__init__(self)
        self.letter = letter
        self.title = letter
        self.path_title = letter

    def get_resource_name(self):
        if self.letter == "#":
            return "0"
        return self.letter.lower()

    def get_category_filter(self):
        def category_filter(category):
            if category.__class__ == LetterMenuItem:
                return False
            return True
        return category_filter

    #def get_game_filter(self):
    #    def game_filter(game_info):
    #        if game_info.sort_title[0] == self.letter:
    #            return True
    #        if self.letter == "#":
    #            if game_info.sort_title[0] in "0123456789":
    #                return True
    #        return False
    #    return game_filter

    def get_filter_clause(self):
        # FIXME: NUMBERS
        if self.letter == "#":
            clause = ["AND (0 = 1"]
            params = []
            for c in "0123456789":
                clause.append(" OR name LIKE ?")
                params.append("{0}%".format(c))
            clause.append(")")
            return "".join(clause), params
        return "AND name LIKE ?", ["{0}%".format(self.letter)]


class YearMenuItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("Year")
        self.path_title_active = "Choose Year"

    def activate(self, menu):
        print("YearMenuItem.activate")
        menu = create_item_menu(_("Select Year"))
        #for year in GameList().get().get_years(): # FIXME: GET YEARS FROM FILTERED GAME LIST
        #    item = YearItem(year)
        #    menu.append(item)
        return menu


class YearItem(AutoExpandItem):

    def __init__(self, year):
        AutoExpandItem.__init__(self)
        self.year = year
        self.title = str(year)
        self.path_title = str(year)

    def get_category_filter(self):
        def category_filter(category):
            if category.__class__ == YearMenuItem:
                return False
            return True
        return category_filter

    #def get_game_filter(self):
    #    def game_filter(game_info):
    #        if game_info.year == self.year:
    #            return True
    #        return False
    #    return game_filter


class KeywordMenuItem(MenuItem):

    def __init__(self):
        MenuItem.__init__(self)
        self.title = _("Keyword")
        self.path_title_active = "Choose Keyword"

    def activate(self, menu):
        print("KeywordMenuItem.activate")
        new_menu = create_item_menu(_("Select Keyword"))
        menu_path = self.create_menu_path(menu)
        new_menu.update_path(menu_path)
        category_filters = list(self.generate_category_filters(menu_path))
        def check(cat):
            for flt in category_filters:
                if flt(cat) == False:
                    return False
            return True
        #for keyword in GameList().get().get_keywords():
        #    item = KeywordItem(keyword)
        #    if check(item):
        #        new_menu.append(item)
        return new_menu


class KeywordItem(AutoExpandItem):

    def __init__(self, keyword):
        AutoExpandItem.__init__(self)
        self.keyword = keyword
        self.title = keyword
        self.path_title = keyword

    def get_category_filter(self):
        def category_filter(category):
            if category.__class__ == KeywordItem:
                if category.keyword == self.keyword:
                    return False
            return True
        return category_filter

    #def get_game_filter(self):
    #    def game_filter(game_info):
    #        if self.keyword in game_info.keywords:
    #            return True
    #        return False
    #    return game_filter


class GameItem(MenuItem):

    def __init__(self, game_info):
        MenuItem.__init__(self)
        self.game_info = game_info
        #self.id = id
        self.uuid = game_info[0]
        self.title = game_info[1]

        #id, name, configurations, platform, ratio=0.75,
        #    screenratio=1.33, year="", publisher="", developer="",
        #    subtitlepos=-1

        # when a game item is the selected item in the parent menu,
        # we are displaying category items
        self.path_title_active = "Add Filter"

        #self.name = name
        self.configurations = []
        #self.platform = platform
        #print("self.platform = ", self.platform)
        #self.title = name
        # FIXME: Get correct year
        #self.year = year
        #self.publisher = publisher
        #self.developer = developer
        self.subtitle = ""

        ratio = 0.75
        screenratio = 1.33

        if ratio:
            self.ratio = ratio
        else:
            """
            if self.platform == "Game Boy":
                self.ratio = 1.0
            elif self.platform == "Game Boy Color":
                self.ratio = 1.0
            elif self.platform == "Game Boy Advance":
                self.ratio = 1.0
            elif self.platform == "Super Nintendo":
                self.ratio = 1.33
            elif self.platform == "Nintendo 64":
                self.ratio = 1.33
            else:
                self.ratio = 0.75
            """
            self.ratio = 0.75
        self.screenratio = screenratio or 1.33

        # FIXME: REMOVE
        self.ratio = 0.75

        #if subtitlepos > 0:
        #    self.title = name[:subtitlepos].strip()
        #    self.subtitle = name[subtitlepos:].strip()
        #elif subtitlepos < 0:
        #    self.supertitle = name[:-subtitlepos].strip()
        #    self.title = name[-subtitlepos:].strip()
        #else:
        #    self.fix_titles()

    @property
    def name(self):
        return self.game_info[1]

    #@property
    #def title(self):
    #    return self.game_info[1]

    @property
    def platform(self):
        return self.game_info[2]

    @property
    def year(self):
        return self.game_info[3]

    @property
    def publisher(self):
        return self.game_info[4]

    def fix_titles(self):
        try:
            title, subtitle = self.title.split(":", 1)
        except ValueError:
            pass
        else:
            #if len(title) >= 8:
            self.title = title
            self.subtitle = subtitle
        if not self.subtitle:
            if "(" in self.title:
                if self.title[-1] == ")":
                    pos = self.title.index("(")
                    self.subtitle = self.title[pos+1:-1].strip()
                    self.title = self.title[:pos].strip()
        #if not self.subtitle:
        #    if len(self.title) > 30 and " in " in self.title:
        #        pos = self.title.find(" in ")
        #        if pos > 8:
        #            self.subtitle = self.title[pos+4:].strip()
        #            self.title = self.title[:pos+4].strip()
        #            self.subtitle = self.subtitle[0].upper() + self.subtitle[1:]
        if not self.subtitle:
            try:
                title, subtitle = self.title.split(" - ", 1)
            except ValueError:
                pass
            else:
                if len(title) >= 8:
                    self.title = title
                    self.subtitle = subtitle
        if not self.subtitle:
            pos = self.title.find(" and the ")
            if pos > 7:
                self.subtitle = self.title[pos:].strip()
                self.title = self.title[:pos].strip()
                #self.subtitle = self.subtitle[0].upper() + self.subtitle[1:]

    def activate(self, menu):
        try:
            from .gamemenu import GameMenu
            new_menu = GameMenu(self)
        except Exception:
            show_exception()
            print("returning None")
            return None
        return new_menu

    def get_default_texture(self):
        return Texture.missing_cover

    @fs.memoize
    def get_image_path(self):
        cmp_name = NameUtil.create_cmpname(self.name)
        # FIXME: FIXME: DISABLED TEMPORARILY
        # FIXME: USING 512-size only for now
        #if False and os.path.exists("../fs-game-database/game/texture"):
        #if os.path.exists("../fs-game-database/game/texture"):
        #    base_path = "../fs-game-database/game/texture"
        #    path = os.path.join(base_path, NameUtil.create_cmpname(
        #            self.platform), cmp_name, "front-512.jpg")
        #            #self.platform), cmp_name, "front-1024.jpg")
        #    if os.path.exists(path):
        #        return path
        ##else:
        #base_path = os.path.join(pyapp.app.get_data_dir(), "texture")
        #if os.path.exists("c:\\data\\game\\info"):
        #    base_path = "c:\\data\\game\\info"
        #else:
        #    base_path = os.path.join(pyapp.app.get_data_dir(), "info")
        #base_path = os.path.join(GameCenter.data_dir, "info")
        #path = os.path.join(base_path, NameUtil.create_cmpname(self.platform),
        #        cmp_name, "front.jpg")
        #if not os.path.exists(path):
        #    base_path = "c:\\data\\game\\info"
        #    path = os.path.join(base_path, NameUtil.create_cmpname(
        #            self.platform), cmp_name, "front.jpg")
        #        #cmp_name, "front-1024.jpg")
        #if not os.path.exists(path):
        #    path = os.path.join(base_path,
        #            NameUtil.create_cmpname(self.platform),
        #            cmp_name, "front-512.jpg")
        #path = os.path.join(NameUtil.create_cmpname(self.platform),
        #        cmp_name, "front.jpg")
        #return path
        #return self.game_info[5] + "?s=512&t=jpg"
        if not self.game_info[5]:
            return None
        return self.game_info[5] + "?w=480&h=640&t=lbcover&f=jpg"

    #@fs.memoize
    #def get_image_files(self):
    #    path = self.get_image_path()
    #    if os.path.exists(path):
    #        return [path]
    #    return []

    @fs.memoize
    def get_image_files(self):
        path = self.get_image_path()
        #if path and os.path.exists(path):
        if path:
            return [path]
        return []

    @fs.memoize
    def get_screen_path(self, n):
        #cmp_name = NameUtil.create_cmpname(self.name)
        #base_path = os.path.join(GameCenter.data_dir, "info")
        #path = os.path.join(base_path, NameUtil.create_cmpname(self.platform),
        #        cmp_name, "screen{0}.png".format(n))
        #if not os.path.exists(path):
        #    base_path = "c:\\data\\game\\info"
        #    path = os.path.join(base_path, NameUtil.create_cmpname(
        #            self.platform), cmp_name, "screen{0}.png".format(n))
        #if not os.path.exists(path):
        #    return None

        assert n >= 1 and n <= 5
        value = self.game_info[7 + n - 1]
        #if value:
        #    value = "sha1:" + value

        #"""
        #if n == 1:
        #    name = "screen1.png"
        #elif n == 2:
        #    name = "title.png"
        #else:
        #    name = "screen{0}.png".format(n - 1)
        #"""
        #name = "screen{0}.png".format(n)
        #path = os.path.join(NameUtil.create_cmpname(
        #        self.platform), cmp_name, name)
        if not value:
            return None
        return value + "?s=1x"

    def get_screen_texture(self, n):
        path = self.get_screen_path(n)
        #print("get_screen_texture", repr(path))

        if path:
            # FIXME: LOAD WITH THE REST OF THE COVERS!
            TextureManager.get().load_images([path])
            try:
                texture = TextureManager.get().get_texture(path)
            except KeyError:
                texture = None
            if not texture:
                Render.dirty = DIRTY_WHILE_NOT_LOADED
            if texture is Texture.default_item:
                return Texture.missing_screenshot
            return texture
        return Texture.missing_screenshot
