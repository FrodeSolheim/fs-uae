import logging
import time
import traceback

from app.app_init import app_init

import fsapp  # type: ignore
from fsgui.mainwindow import MainWindow
from fsgui.windowmanager import WindowManager
from fsuae.eventservice import EventService

logger = logging.getLogger(__name__)

# from PIL import Image, ImageDraw
# print(Image)
# print(ImageDraw)

global_quit = False
left_was_pressed = False
mouse_pos = (-1, -1)

# _uae_config = {}


# def uae_config_event(event):
#     lines = event["strdata"].split("\n")
#     for line in lines:
#         if line == "" or line.startswith(";"):
#             continue
#         try:
#             key, value = line.split("=", 1)
#         except ValueError as e:
#             print(line, e)
#         if _uae_config.get(key) != value:
#             _uae_config[key] = value
#             print("----->", key, "<-", value)


def python_thread():
    global mouse_pos, left_was_pressed
    logger.info("Python thread starting (Python side)")

    logger.info("Calling app_init")
    # FIXME: differentiate app_init_before_first_draw and
    # app_init_after_first_draw?
    app_init()

    window_manager = WindowManager.get()
    notification_service = EventService.instance()

    # window1 = MainTitleBar(size=(1280, 40))
    # # window1.style.background_color = (128, 128, 128, 255)

    # window2 = TestWindow()
    # # window2.style.background_color = (255, 0, 0, 255)
    # window2.set_position((20, 60))

    # window3 = Window(size=(640, 480))
    # window3.style.background_color = (0, 255, 0, 255)

    # window4 = Window(size=(28, 28))
    # window4.set_position((6, 6))
    # window4.style.background_color = (255, 255, 255, 255)

    # surface = fsapp.create_surface((512, 512))
    # print(surface)
    # fsapp.draw_filled_rectangle(surface, (0, 0), (512, 512), (255, 255, 255, 255))

    # frame_num = 0
    t1 = time.monotonic_ns()

    done = False
    while not done:
        if global_quit:
            done = True
            break

        # Order below looks "funny" - start from fsapp.update and follow the loop.
        # FIXME: Maybe reorder later...

        # (2) Then resize and redraw stuff

        try:
            for window in window_manager.get_windows():
                window.update()

        except Exception:
            # FIXME: Is it possible to print in colors?
            traceback.print_exc()

        # (3) Post surfaces

        main_window = MainWindow.get()
        origin = (main_window._left_border, main_window._top_border)

        surfaces = []
        for window in window_manager.get_windows():
            if window.is_visible():
                # surfaces.append(window.surface.get_surface_tuple())
                # surfaces.append()
                surface, surface_pos = window.surface.get_surface_tuple()
                surfaces.append(
                    (
                        surface,
                        (
                            surface_pos[0] + origin[0],
                            surface_pos[1] + origin[1],
                        ),
                    )
                )

        t2 = time.monotonic_ns()
        dt = (10 * (t2 - t1) // (1000 * 1000)) / 10
        if dt > 5:
            logger.warning("dt ui (python) %d > 5 msec", dt)
        # frame_num += 1
        # if frame_num % 60 == 0:
        #     logger.debug("dt %d", dt)
        (
            quit,
            new_mouse_pos,
            left_pressed,
            sdl_window_flags,
            window_size,
            mouse_grab,
        ) = fsapp.update(surfaces)

        # print(new_mouse_pos)

        new_mouse_pos = (
            new_mouse_pos[0] - origin[0],
            new_mouse_pos[1] - origin[1],
        )

        t1 = time.monotonic_ns()

        import fsapp_events  # type: ignore

        # FIXME: event handling subscription...
        events = fsapp_events.get()
        for event in events:
            notification_service.broadcast(event)

        # (1) Process events first...

        if mouse_grab:
            # Use previous position
            new_mouse_pos = mouse_pos
            # Force button depress (?)
            left_pressed = False

        try:
            if new_mouse_pos != mouse_pos:
                mouse_pos = new_mouse_pos
                window_manager.handle_mouse_moved(mouse_pos)
            # Must process this after mouse move
            if left_pressed != left_was_pressed:
                left_was_pressed = left_pressed
                window_manager.handle_left_mouse_button(left_pressed)

            main_window.update_state(window_size, sdl_window_flags)

            if quit:
                logger.info("Got quit signal - ending\n")
                done = True
        except Exception:
            # FIXME: Is it possible to print in colors?
            traceback.print_exc()

        # print(fsapp.numargs())
        # time.sleep(0.02)

    logger.info("Python thread ending (Python side)")


def python_thread_stop():
    global global_quit
    logger.info("python_thread_stop was called from C/C++")

    # GIL handles Python issues here...
    global_quit = True
