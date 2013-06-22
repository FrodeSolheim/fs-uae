from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import random
from .font import Font
from .opengl import *
from .render import Render
from .state import State
from .texture import Texture


def rectangle(x, y, w, h, z=0.0):
    #glBegin(GL_QUADS)
    glVertex3f(x, y, z)
    glVertex3f(x + w, y, z)
    glVertex3f(x + w, y + h, z)
    glVertex3f(x, y + h, z)
    #glEnd()


def texture_rectangle(x, y, w, h, z=0.0, s1=0.0, s2=1.0, t1=1.0, t2=0.0):
    glBegin(GL_QUADS)
    glTexCoord(s1, t1)
    glVertex3f(x, y, z)
    glTexCoord(s2, t1)
    glVertex3f(x + w, y, z)
    glTexCoord(s2, t2)
    glVertex3f(x + w, y + h, z)
    glTexCoord(s1, t2)
    glVertex3f(x, y + h, z)
    glEnd()


def static():
    s1 = random.uniform(0.0, 0.65)
    s2 = s1 + 0.35
    t1 = random.uniform(0.0, 0.65)
    t2 = t1 + 0.35
    w, h = Texture.static.size
    #return Texture.static, (s1 * w, s2 * w, t1 * h, t2 * h)
    return Texture.static, (s1, s2, t1, t2)


last_item_id = None
item_time = 0

def render_bottom_bar_transparent():
    Render.hd_perspective()
    fs_emu_blending(True)
    Texture.bottom_bar.render(0, 0, 1920, 380, 0.3)
    item = State.current_menu.selected_item
    render_bottom_bar_text(item)


def render_bottom_bar():
    Render.hd_perspective()
    item = State.current_menu.selected_item
    render_bottom_bar_screens(item)


def render_bottom_bar_screens(item):
    global last_item_id
    global item_time
    if id(item) != last_item_id:
        last_item_id = id(item)
        item_time = State.time

    glColor3f(1.0, 1.0, 1.0)
    fs_emu_texturing(True)
    fs_emu_blending(False)

    COORDS = 0.0, 1.0, 1.0, 0.0
    for i in range(3, -1, -1):
        if State.time - item_time > 0.1 + 0.05 * i:
            texture = item.get_screen_texture(2 + i)
        else:
            texture = None
        if texture:
            #coords = (0, texture.w, texture.h, 0)
            coords = COORDS
        else:
            texture, coords = static()
        #print(repr(texture))
        #w, h = texture.size
        #glBindTexture(GL_TEXTURE_2D, texture)
        texture.bind()
        #coords = (coords[0] * w, coords[1] * w, coords[2] * h, coords[3] * h)
        texture_rectangle(4 + 539 + 345 * i, 34, 308, 193, 0.2, *coords)
    if State.time - item_time > 0.1:
        texture = item.get_screen_texture(1)
    else:
        texture = None
    if texture:
        coords = COORDS
        #coords = (0, texture.w, texture.h, 0)
    else:
        texture, coords = static()
    #w, h = texture.size
    #glBindTexture(GL_TEXTURE_2D, texture)
    texture.bind()
    #coords = (coords[0] * w, coords[1] * w, coords[2] * h, coords[3] * h)
    
    texture_rectangle(34, 34, 472, 295, 0.2, *coords)

    """
    fs_emu_blending(True)
    Texture.info_bar.render(0, 0, 1920, 380, opacity=1.0)

    Texture.screen_border_1.render(0, 0, 540, 363, 0.1)
    for i in range(4):
        Texture.screen_border_2.render(509 + 345 * i, 0, 376, 261,
                0.11 + 0.01 * i)

    glColor4f(0.7, 0.7, 0.7, 0.7)
    #glBindTexture(GL_TEXTURE_2D, Texture.gloss)
    Texture.gloss.bind()
    for i in range(4):
        texture_rectangle(4 + 539 + 345 * i, 34, 308, 193, 0.3)
    texture_rectangle(34, 34, 472, 295, 0.3)

    #if item_index == menu.get_selected_index():
    #    State.preview_screen.set_screen(item.get_screen_texture(1))
    """


def render_bottom_bar_text(item):
    #x = State.gl_left + 0.05
    strength = 0.9
    x = 544
    y = 290

    title = item.title.upper()
    #subtitle = State.current_menu.selected_item.subtitle.upper()
    #supertitle = State.current_menu.selected_item.supertitle.upper()
    if title:
        Render.text(title, Font.title_font, x, y, 1920 - x - 170, shadow=True,
                color=(1.0, 1.0, 1.0, 1.0 * strength))

    year_text = str(getattr(State.current_menu.selected_item, "year", "") or "").upper()
    if year_text:
        tw, th = Render.measure_text(year_text, Font.title_font)
        Render.text(year_text, Font.title_font, 1920 - 30 - tw, y, 0, shadow=True,
                color=(1.0, 1.0, 1.0, 1.0 * strength))

    #strength = 0.4
    color = (0x6e / 0xff, 0x8b / 0xff, 0x96 / 0xff, 1.0)
    #y = 253
    y = 258
    text_str = ""
    companies = set()
    publisher_text = (getattr(State.current_menu.selected_item, "publisher", "") or "").upper()
    for text in publisher_text.split("/"):
        text = text.strip()
        if text:
            if not text in companies:
                text_str = text_str + u" \u00b7 " + text
                companies.add(text)
    developer_text = (getattr(State.current_menu.selected_item, "developer", "") or "").upper()
    for text in developer_text.split("/"):
        text = text.strip()
        if text:
            if not text in companies:
                text_str = text_str + u" \u00b7 " + text
                companies.add(text)
    if len(text_str) > 3:
        text_str = text_str[3:] # remove initial middle dot
        Render.text(text_str, Font.subtitle_font, x, y, 0, shadow=True,
                color=color)

    platform_str = str(getattr(State.current_menu.selected_item, "platform", "") or "").upper()
    if len(platform_str) >= 3:
        tw, th = Render.measure_text(platform_str, Font.subtitle_font)
        Render.text(platform_str, Font.subtitle_font, 1920 - 30 - tw, y, 0, shadow=True,
                color=color)
    return
    """
    Render.ortho_perspective()
    #render_header(color=(0.5, 0.5, 0.5), text="Ku Game System",
    #         offset=header_offset)

    fs_emu_blending(True)
    glBindTexture(GL_TEXTURE_2D, Texture.bottom_bar)
    glColor4f(0.85, 0.85, 0.85, 0.85)
    glBegin(GL_QUADS)
    glTexCoord2f(0.0, 1.0)
    glVertex2f(State.gl_left, -1.0)
    glTexCoord2f(1.0, 1.0)
    glVertex2f(State.gl_right, -1.0)
    glTexCoord2f(1.0, 0.0)
    glVertex2f(State.gl_right, -0.75)
    glTexCoord2f(0.0, 0.0)
    glVertex2f(State.gl_left, -0.75)
    glEnd()

    top_mode = State.top_menu == State.navigatable
    config_mode = bool(State.config_menu)

    if top_mode:
        strength = 0.4
    else:
        strength = 1.0


    x = State.gl_left + 0.05
    y = -0.89

    title = State.current_menu.selected_item.title.upper()
    subtitle = State.current_menu.selected_item.subtitle.upper()
    supertitle = State.current_menu.selected_item.supertitle.upper()

    if supertitle:
        tw, th = Render.text(supertitle, Font.title_font,
                x, y, 0, shadow=True,
                color=(1.0, 1.0, 1.0, 1.0 * strength))
        x += tw + 0.03
    if title:
        tw, th = Render.text(title, Font.title_font,
                x, y, 0, shadow=True,
                color=(1.0, 1.0, 1.0, 1.0 * strength))
        x += tw + 0.03
    if subtitle:
        tw, th = Render.text(subtitle, Font.title_font,
                x, y, 0, shadow=True,
                color=(1.0, 1.0, 1.0, 1.0 * strength))
        x += tw + 0.03

    x = State.gl_left + 0.05
    y = -0.96


    options = {"dot": False,
        "x": State.gl_left + 0.05,
        "y": -0.96,
        "texts": set()
    }
    def text(t, measure=None):
        if not t:
            return
        if t in options["texts"]:
            return
        if options["dot"]:
            tw, th = Render.text(u"\u00b7", Font.subtitle_font,
                    options["x"], options["y"], 0, shadow=True,
                    color=(1.0, 1.0, 1.0, 1.0 * strength * 0.25))
            options["x"] += tw + 0.02
        tw, th = Render.text(t, Font.subtitle_font,
                options["x"], options["y"], 0, shadow=True,
                color=(1.0, 1.0, 1.0, 1.0 * strength * 0.33))
        if measure:
            tw, th = Render.measure_text(measure, Font.subtitle_font)
        options["x"] += tw + 0.02
        options["dot"] = True
        options["texts"].add(t)

    platform_text = str(getattr(State.current_menu.selected_item, "platform", "") or "").upper()
    text(platform_text)

    year_text = str(getattr(State.current_menu.selected_item, "year", "") or "").upper()
    text(year_text, measure="0000")
  
    publisher_text = (getattr(State.current_menu.selected_item, "publisher", "") or "").upper()
    for publisher_text in publisher_text.split("/"):
        text(publisher_text.strip())
    """
