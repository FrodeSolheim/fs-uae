import logging

import sdl3
from fsapp.tickservice import TickService
from fsuae.eventservice import EventService

from fsgui.mainwindow import MainWindow
from fsgui.windowmanager import WindowManager

logger = logging.getLogger(__name__)


class State:
    # mouse_x = -1.0
    # mouse_y = -1.0
    # last_mouse_x = -1.0
    # last_mouse_y = -1.0
    # mouse_pos = (-1.0, -1.0)
    mouse_pos: tuple[int, int] = (-1, -1)
    last_mouse_pos = (-1.0, -1.0)


state = State()
window_manager = WindowManager.get()


def maybe_move_mouse(window_manager: WindowManager):
    """Used to just move the mouse when needed, to reduce the number of events."""
    if state.mouse_pos != state.last_mouse_pos:
        main_window = MainWindow.get()
        # origin = (main_window._left_border, main_window._top_border)
        # FIXME: main_window.origin?
        origin = (main_window.border.left, main_window.border.top)

        window_manager.handle_mouse_moved(
            (state.mouse_pos[0] - origin[0], state.mouse_pos[1] - origin[1])
        )

        # window_manager.handle_mouse_moved(state.mouse_pos)
        state.last_mouse_pos = state.mouse_pos


def handle_event(event: sdl3.SDL_Event):
    if event.type == sdl3.SDL_EVENT_MOUSE_MOTION:
        # print("state.mouse_pos", event.motion.y, event.motion.yrel, event.motion.which)
        if event.motion.which:
            # relative mouse event, which we ignore
            pass
        else:
            state.mouse_pos = (int(event.motion.x), int(event.motion.y))
        return
    else:
        maybe_move_mouse(window_manager)

    if event.type == sdl3.SDL_EVENT_MOUSE_BUTTON_DOWN:
        print("SDL_MOUSEBUTTONDOWN")
        if event.button.button == 1:
            # If mouse has been moved, it must be processed before the click!
            # maybe_move_mouse(window_manager)
            window_manager.handle_left_mouse_button(True)
    elif event.type == sdl3.SDL_EVENT_MOUSE_BUTTON_UP:
        print("SDL_MOUSEBUTTONDOWN")
        if event.button.button == 1:
            # maybe_move_mouse(window_manager)
            window_manager.handle_left_mouse_button(False)
    elif event.type == sdl3.SDL_EVENT_KEY_DOWN:
        # maybe_move_mouse(window_manager)
        if event.key.key == sdl3.SDLK_F12:
            # FIXME: Remove hack
            notification_service = EventService.instance()
            notification_service.broadcast({"type": "FSAPP_KEY_PRESS", "intdata": 1073741893})
        pass
    elif event.type == sdl3.SDL_EVENT_KEY_UP:
        # maybe_move_mouse(window_manager)
        pass

    elif event.type == sdl3.SDL_EVENT_WINDOW_RESIZED:
        width = event.window.data1
        height = event.window.data2
        MainWindow.get().update_size((width, height))
    elif event.type == sdl3.SDL_EVENT_WINDOW_MAXIMIZED:
        MainWindow.get().update_maximized(True)
    elif event.type == sdl3.SDL_EVENT_WINDOW_RESTORED:
        MainWindow.get().update_maximized(False)
    elif event.type == sdl3.SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        MainWindow.get().update_fullscreen(True)
    elif event.type == sdl3.SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        MainWindow.get().update_fullscreen(False)


def frame(events: list[sdl3.SDL_Event]) -> None:
    for event in events:
        handle_event(event)
    maybe_move_mouse(window_manager)

    # FIXME: Not handling mouse grab right now. See bridge.py
    # Example combo box dropdowns

    # FIXME: fsapp or fsgui responsibility?
    TickService.get().broadcast()

    # Could in theory update the surfaces first, and then continue running Python code without
    # allowing surface updates. It is possible this could improve concurrency w.r.t. the main
    # thread, but as it is now, no big point (probably).

    z_index = 1
    main_window = MainWindow.get()
    # FIXME: main_window.origin?
    # origin = (main_window._left_border, main_window._top_border)
    origin = (main_window.border.left, main_window.border.top)
    for window in window_manager.get_windows():
        try:
            window.update()
            position = window._window_position
            if window.surface is not None:
                window.surface.set_z_index(z_index)
                surface_position = (position[0] + origin[0], position[1] + origin[1])
                # window.surface.set_position(window.surface.x)
                window.surface.set_position(surface_position)
                # print(window, z_index)
        except Exception:
            # FIXME: Make sure to propagate error level to SDL logger, and use custom
            # colors when printing to stderr on supported consoles???
            logger.exception("Error updating window")

        z_index = z_index + 1
