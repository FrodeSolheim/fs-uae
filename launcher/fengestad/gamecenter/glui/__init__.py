from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import sys
import time
from fengestad import fs

"""
import subprocess
_Popen = subprocess.Popen
_Popen = subprocess.Popen
def CustomPopen(*args, **kwargs):
    if not "close_fds" in kwargs:
        raise Exception("missing close_fds, kwargs = ", repr(kwargs))
    if not kwargs["close_fds"]:
        raise Exception("close_fds is not True")
    return _Popen(*args, **kwargs)
subprocess.Popen = CustomPopen
"""

def main():
    from fengestad.gamecenter.glui.settings import Settings
    for arg in sys.argv:
        Settings.fullscreen_menu = True
        #if fs.macosx:
        #    Settings.fullscreen_menu = True
        Settings.fullscreen_game = True
        if arg == "--fullscreen":
            Settings.fullscreen_menu = True
        if arg == "--window":
            if not "--no-decorations" in sys.argv:
                Settings.window_decorations = True
            Settings.fullscreen_menu = False
            Settings.fullscreen_game = False
            Settings.windowed_size = (1024, 576)
        elif arg.startswith("--window="):
            Settings.fullscreen_menu = False
            Settings.fullscreen_game = False
            w, h = arg[9:].split("x")
            w = int(w)
            h = int(h)
            Settings.windowed_size = (w, h)
        elif arg == "--maximize":
            if not "--no-decorations" in sys.argv:
                Settings.window_decorations = True
            Settings.fullscreen_menu = False
            Settings.fullscreen_game = True
            Settings.windowed_size = None
        elif arg == "--no-decorations":
            Settings.window_decorations = False
        elif arg == "--decorations":
            Settings.window_decorations = True

    if "--glui2" in sys.argv:
        from no_fengestad_gamecenter_glui2.window import Window
        window = Window()
        window.show()
        from glui import GLMainLoop
        GLMainLoop.run()
    else:
        try:
            import fengestad.gamecenter.glui.window
            fengestad.gamecenter.glui.window.show()
        except Exception, e:
            import traceback
            print("\n" + "-" * 79 + "\n" + "EXCEPTION")
            traceback.print_exc()
            show_error(repr(e), traceback.format_exc(e))
    print(" --- fengestad.main.gamecenter.glui.main is done ---")

import pygame
from fengestad.gamecenter.glui.opengl import *
from fengestad.gamecenter.glui.render import Render

def show_error(message, backtrace):
    width = 16 / 9 * 2
    height = 2.0
    message = message
    backtrace = backtrace
    splitted = backtrace.split("\n")
    if not splitted[-1]:
        splitted = splitted[:-1]

    background_color = (0.0, 0.0, 0.0, 1.0)
    from fengestad.gamecenter.resources import resources
    liberation_mono_bold = resources.resource_filename(
            "LiberationMono-Regular.ttf")
    print(liberation_mono_bold)
    detail_font = pygame.font.Font(liberation_mono_bold,
            int(0.021 * Render.display_height))
    guru_font = pygame.font.Font(liberation_mono_bold,
            int(0.03 * Render.display_height))
    start_time = time.time()
    while True:
        events = pygame.event.get()
        stop = False
        for event in events:
            if event.type == pygame.QUIT:
                stop = True
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RETURN:
                    stop = True
                if event.key == pygame.K_ESCAPE:
                    stop = True
        if stop:
            pygame.display.quit()
            sys.exit(1)

        glClearColor(0.0, 0.0, 0.0, 0.0)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        Render.ortho_perspective()
        glPushMatrix()
        glTranslatef(-width / 2, -height / 2, 0.0)
        x1 = 0
        x2 = width
        y1 = 1.7
        y2 = 2.0
        w = 0.03

        #t = (pygame.time.get_ticks() - start_time) // 1000
        glDisable(GL_DEPTH_TEST)
        fs_emu_texturing(False)
        fs_emu_blending(True)
        #glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        alert_color = (1.0, 0.0, 0.0)
        t = int((time.time() - start_time) * 1.6)
        if t % 2 == 0:
            glBegin(GL_QUADS)
            glColor3f(*alert_color)
            glVertex2f(x1, y1)
            glVertex2f(x2, y1)
            glVertex2f(x2, y2)
            glVertex2f(x1, y2)
            glColor3f(0.0, 0.0, 0.0)
            glVertex2f(x1 + w, y1 + w)
            glVertex2f(x2 - w, y1 + w)
            glVertex2f(x2 - w, y2 - w)
            glVertex2f(x1 + w, y2 - w)
            glEnd()
        fs_emu_texturing(True)
        #glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)

        text = "Software Failure.  Press ENTER or ESCAPE key to quit."
        tw, th = Render.text(text, guru_font,
                0.2, 1.85, color=alert_color)
        text = splitted[-1]
        text = "Guru Meditation #{0}".format(text)
        tw, th = Render.text(text, guru_font,
                0.2, 1.77, color=alert_color)

        x = 0.2
        y = 0.15

        #print(backtrace)
        tw, th = Render.measure_text("M", detail_font)
        y += th
        lines = []
        #max_line_size = 80
        max_line_size = 129
        for line in splitted:
            line = line.rstrip()
            while len(line) > max_line_size:
                lines.append(line[:max_line_size])
                line = line[max_line_size:]
            lines.append(line)

        MAX_LINE = 30
        for i, line in enumerate(reversed(lines)):
            if i == MAX_LINE:
                break
            s = (MAX_LINE - i) / MAX_LINE
            tw, th = Render.text(line, detail_font,
                    x, y, color=(s, s, s, 1.0))
            y += th
        glPopMatrix()
        pygame.display.flip()

