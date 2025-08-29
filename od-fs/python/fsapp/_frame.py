import ctypes
import logging

import _fsapp
import fsgui._frame
import sdl3

from fsapp.event import FSAppEvent
from fsapp.eventname import EventName
from fsapp.eventservice import EventService

logger = logging.getLogger(__name__)

frame_counter = 0

# FIXME: log to logging!!
# traceback.print_exc()

# FIXME: How to show errors, how to stop execution?
# FIXME: Maybe a way to report errors to the main thread and let it display
# errors?
# FIXME: Maybe let the main thread show a recoverable error dialog? or
# and error occurred "tooltip"/notification or something

event_mapping = {
    sdl3.SDL_EVENT_GAMEPAD_ADDED: EventName.SDL_EVENT_GAMEPAD_ADDED,
    sdl3.SDL_EVENT_GAMEPAD_REMAPPED: EventName.SDL_EVENT_GAMEPAD_REMAPPED,
    sdl3.SDL_EVENT_GAMEPAD_REMOVED: EventName.SDL_EVENT_GAMEPAD_REMOVED,
    sdl3.SDL_EVENT_JOYSTICK_ADDED: EventName.SDL_EVENT_JOYSTICK_ADDED,
    sdl3.SDL_EVENT_JOYSTICK_REMOVED: EventName.SDL_EVENT_JOYSTICK_REMOVED,
}


def handle_user_event(event: sdl3.SDL_UserEvent, event_service: EventService):
    print("handle_user_event")
    print(event.code)
    str_value = ""
    if event.data2 is not None:
        c_string: ctypes.c_char_p = ctypes.cast(event.data2, ctypes.c_char_p)
        if c_string.value is None:
            logger.warning("ctypes c_char_p value was None")
        else:
            str_value = c_string.value.decode("UTF-8")
    int_value: int = event.data1
    print(repr(int_value))
    print(repr(str_value))

    event_name = FSAppEvent(event.code).name

    event_service = EventService.instance()
    event_service.broadcast({"type": event_name, "intdata": int_value, "strdata": str_value})


def handle_event(event: sdl3.SDL_Event, event_service: EventService):
    if event.type == sdl3.SDL_EVENT_USER:
        handle_user_event(event.user, event_service)
    else:
        try:
            event_name = event_mapping[event.type]
        except KeyError:
            pass
        else:
            event_service.sdl_event(event_name).emit(event)


def frame2():
    # global frame_counter
    # frame_counter += 1
    # print("Frame", frame_counter)

    # FIXME: event handling subscription...

    import fsapp_events

    events = fsapp_events.get()
    notification_service = EventService.instance()
    for event in events:
        # if event["type"] != "UAE_CONFIG":
        #     continue
        notification_service.broadcast(event)

    events: list[sdl3.SDL_Event] = []

    event_capsules = _fsapp.get_events()  # type: ignore
    for event_capsule in event_capsules:
        event_c_pointer = _fsapp.get_event_pointer(event_capsule)
        pointer = ctypes.cast(event_c_pointer, ctypes.POINTER(sdl3.SDL_Event))
        event = pointer.contents

        # Tie capsule lifetime to event struct lifetime
        event._capsule = event_capsule

        events.append(event)

    # FIXME: In an ideal abstract world, fsgui would register with fsapp from frame callbacks.
    # But with only "one client", we call fsui directly -for now.

    fsgui._frame.frame(events)

    event_service = EventService.instance()
    for event in events:
        handle_event(event, event_service)

    simulate_slow_frames = False
    if simulate_slow_frames:
        logger.info("Simulating slow frame")
        import time

        time.sleep(0.02)


def frame():
    try:
        frame2()
    except Exception:
        logger.exception("Exception handled during frame processing")
        # import traceback
        # traceback.print_exc()
