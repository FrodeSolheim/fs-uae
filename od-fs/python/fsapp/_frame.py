import ctypes
import logging

import _fsapp
import fsgui._frame
import sdl3

logger = logging.getLogger(__name__)

frame_counter = 0

# FIXME: log to logging!!
# traceback.print_exc()

# FIXME: How to show errors, how to stop execution?
# FIXME: Maybe a way to report errors to the main thread and let it display
# errors?
# FIXME: Maybe let the main thread show a recoverable error dialog? or
# and error occurred "tooltip"/notification or something


def frame2():
    # global frame_counter
    # frame_counter += 1
    # print("Frame", frame_counter)

    events = []

    event_capsules = _fsapp.get_events()  # type: ignore
    for event_capsule in event_capsules:
        event_c_pointer = _fsapp.get_event_pointer(event_capsule)
        pointer = ctypes.cast(event_c_pointer, ctypes.POINTER(sdl3.SDL_Event))
        event = pointer.contents

        # Tie capsule lifetime to event struct lifetime
        event._capsule = event_capsule

        events.append(event)

    # FIXME: In an ideal abstract world, fsgui would register with fsapp from frame callbacks.
    # But with only "one client", we call fsui directly for now.

    fsgui._frame.frame(events)

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
