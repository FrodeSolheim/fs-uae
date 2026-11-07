import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC_TYPE, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY, SDL_ENUM
from .SDL_video import SDL_Window, SDL_WindowID, SDL_DisplayID
from .SDL_sensor import SDL_SensorID
from .SDL_joystick import SDL_JoystickID
from .SDL_keyboard import SDL_KeyboardID
from .SDL_keycode import SDL_Keycode, SDL_Keymod
from .SDL_scancode import SDL_Scancode
from .SDL_mouse import SDL_MouseID, SDL_MouseWheelDirection, SDL_MouseButtonFlags
from .SDL_power import SDL_PowerState
from .SDL_audio import SDL_AudioDeviceID
from .SDL_camera import SDL_CameraID
from .SDL_pen import SDL_PenID, SDL_PenInputFlags, SDL_PenAxis
from .SDL_touch import SDL_TouchID, SDL_FingerID

SDL_EventType: typing.TypeAlias = SDL_TYPE["SDL_EventType", SDL_ENUM]

SDL_EVENT_FIRST: int = 0x0

SDL_EVENT_QUIT, SDL_EVENT_TERMINATING, SDL_EVENT_LOW_MEMORY, SDL_EVENT_WILL_ENTER_BACKGROUND, \
    SDL_EVENT_DID_ENTER_BACKGROUND, SDL_EVENT_WILL_ENTER_FOREGROUND, SDL_EVENT_DID_ENTER_FOREGROUND, \
        SDL_EVENT_LOCALE_CHANGED, SDL_EVENT_SYSTEM_THEME_CHANGED = range(0x100, 0x109)

SDL_EVENT_DISPLAY_ORIENTATION, SDL_EVENT_DISPLAY_ADDED, SDL_EVENT_DISPLAY_REMOVED, SDL_EVENT_DISPLAY_MOVED, \
    SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED, SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED, SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED = range(0x151, 0x158)

SDL_EVENT_DISPLAY_FIRST, SDL_EVENT_DISPLAY_LAST = \
    SDL_EVENT_DISPLAY_ORIENTATION, SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED

SDL_EVENT_WINDOW_SHOWN, SDL_EVENT_WINDOW_HIDDEN, SDL_EVENT_WINDOW_EXPOSED, SDL_EVENT_WINDOW_MOVED, SDL_EVENT_WINDOW_RESIZED, \
    SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED, SDL_EVENT_WINDOW_METAL_VIEW_RESIZED, SDL_EVENT_WINDOW_MINIMIZED, SDL_EVENT_WINDOW_MAXIMIZED, \
        SDL_EVENT_WINDOW_RESTORED, SDL_EVENT_WINDOW_MOUSE_ENTER, SDL_EVENT_WINDOW_MOUSE_LEAVE, SDL_EVENT_WINDOW_FOCUS_GAINED, SDL_EVENT_WINDOW_FOCUS_LOST, \
            SDL_EVENT_WINDOW_CLOSE_REQUESTED, SDL_EVENT_WINDOW_HIT_TEST, SDL_EVENT_WINDOW_ICCPROF_CHANGED, SDL_EVENT_WINDOW_DISPLAY_CHANGED, SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED, \
                SDL_EVENT_WINDOW_SAFE_AREA_CHANGED, SDL_EVENT_WINDOW_OCCLUDED, SDL_EVENT_WINDOW_ENTER_FULLSCREEN, SDL_EVENT_WINDOW_LEAVE_FULLSCREEN, \
                    SDL_EVENT_WINDOW_DESTROYED, SDL_EVENT_WINDOW_HDR_STATE_CHANGED = range(0x202, 0x21B)

SDL_EVENT_WINDOW_FIRST, SDL_EVENT_WINDOW_LAST = \
    SDL_EVENT_WINDOW_SHOWN, SDL_EVENT_WINDOW_HDR_STATE_CHANGED

SDL_EVENT_KEY_DOWN, SDL_EVENT_KEY_UP, SDL_EVENT_TEXT_EDITING, SDL_EVENT_TEXT_INPUT, SDL_EVENT_KEYMAP_CHANGED, \
    SDL_EVENT_KEYBOARD_ADDED, SDL_EVENT_KEYBOARD_REMOVED, SDL_EVENT_TEXT_EDITING_CANDIDATES = range(0x300, 0x308)

SDL_EVENT_MOUSE_MOTION, SDL_EVENT_MOUSE_BUTTON_DOWN, SDL_EVENT_MOUSE_BUTTON_UP, \
    SDL_EVENT_MOUSE_WHEEL, SDL_EVENT_MOUSE_ADDED, SDL_EVENT_MOUSE_REMOVED = range(0x400, 0x406)

SDL_EVENT_JOYSTICK_AXIS_MOTION, SDL_EVENT_JOYSTICK_BALL_MOTION, SDL_EVENT_JOYSTICK_HAT_MOTION, SDL_EVENT_JOYSTICK_BUTTON_DOWN, SDL_EVENT_JOYSTICK_BUTTON_UP, \
    SDL_EVENT_JOYSTICK_ADDED, SDL_EVENT_JOYSTICK_REMOVED, SDL_EVENT_JOYSTICK_BATTERY_UPDATED, SDL_EVENT_JOYSTICK_UPDATE_COMPLETE = range(0x600, 0x609)

SDL_EVENT_GAMEPAD_AXIS_MOTION, SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_EVENT_GAMEPAD_BUTTON_UP, SDL_EVENT_GAMEPAD_ADDED, SDL_EVENT_GAMEPAD_REMOVED, \
    SDL_EVENT_GAMEPAD_REMAPPED, SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN, SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION, SDL_EVENT_GAMEPAD_TOUCHPAD_UP, SDL_EVENT_GAMEPAD_SENSOR_UPDATE, \
        SDL_EVENT_GAMEPAD_UPDATE_COMPLETE, SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED = range(0x650, 0x65C)

SDL_EVENT_FINGER_DOWN, SDL_EVENT_FINGER_UP, SDL_EVENT_FINGER_MOTION, \
    SDL_EVENT_FINGER_CANCELED = range(0x700, 0x704)

SDL_EVENT_CLIPBOARD_UPDATE: int = 0x900

SDL_EVENT_DROP_FILE, SDL_EVENT_DROP_TEXT, SDL_EVENT_DROP_BEGIN, \
    SDL_EVENT_DROP_COMPLETE, SDL_EVENT_DROP_POSITION = range(0x1000, 0x1005)

SDL_EVENT_AUDIO_DEVICE_ADDED, SDL_EVENT_AUDIO_DEVICE_REMOVED, \
    SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED = range(0x1100, 0x1103)

SDL_EVENT_SENSOR_UPDATE: int = 0x1200

SDL_EVENT_PEN_PROXIMITY_IN, SDL_EVENT_PEN_PROXIMITY_OUT, SDL_EVENT_PEN_DOWN, SDL_EVENT_PEN_UP, SDL_EVENT_PEN_BUTTON_DOWN, \
    SDL_EVENT_PEN_BUTTON_UP, SDL_EVENT_PEN_MOTION, SDL_EVENT_PEN_AXIS = range(0x1300, 0x1308)

SDL_EVENT_CAMERA_DEVICE_ADDED, SDL_EVENT_CAMERA_DEVICE_REMOVED, \
    SDL_EVENT_CAMERA_DEVICE_APPROVED, SDL_EVENT_CAMERA_DEVICE_DENIED = range(0x1400, 0x1404)

SDL_EVENT_RENDER_TARGETS_RESET, SDL_EVENT_RENDER_DEVICE_RESET, \
    SDL_EVENT_RENDER_DEVICE_LOST = range(0x2000, 0x2003)

SDL_EVENT_PRIVATE0, SDL_EVENT_PRIVATE1, SDL_EVENT_PRIVATE2, \
    SDL_EVENT_PRIVATE3 = range(0x4000, 0x4004)

SDL_EVENT_POLL_SENTINEL: int = 0x7F00
SDL_EVENT_USER: int = 0x8000
SDL_EVENT_LAST: int = 0xFFFF
SDL_EVENT_ENUM_PADDING: int = 0x7FFFFFFF

class SDL_CommonEvent(ctypes.Structure):
    _fields_ = [
        ("type", ctypes.c_uint32),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64)
    ]

class SDL_DisplayEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("displayID", SDL_DisplayID),
        ("data1", ctypes.c_int32),
        ("data2", ctypes.c_int32)
    ]

class SDL_WindowEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("data1", ctypes.c_int32),
        ("data2", ctypes.c_int32)
    ]

class SDL_KeyboardDeviceEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_KeyboardID)
    ]

class SDL_KeyboardEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("which", SDL_KeyboardID),
        ("scancode", SDL_Scancode),
        ("key", SDL_Keycode),
        ("mod", SDL_Keymod),
        ("raw", ctypes.c_uint16),
        ("down", ctypes.c_bool),
        ("repeat", ctypes.c_bool)
    ]

class SDL_TextEditingEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("text", ctypes.c_char_p),
        ("start", ctypes.c_int32),
        ("length", ctypes.c_int32)
    ]

class SDL_TextEditingCandidatesEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("candidates", SDL_POINTER[ctypes.c_char_p]),
        ("num_candidates", ctypes.c_int32),
        ("selected_candidate", ctypes.c_int32),
        ("horizontal", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8)
    ]

class SDL_TextInputEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("text", ctypes.c_char_p)
    ]

class SDL_MouseDeviceEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_MouseID)
    ]

class SDL_MouseMotionEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("which", SDL_MouseID),
        ("state", SDL_MouseButtonFlags),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("xrel", ctypes.c_float),
        ("yrel", ctypes.c_float)
    ]

class SDL_MouseButtonEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("which", SDL_MouseID),
        ("button", ctypes.c_uint8),
        ("down", ctypes.c_bool),
        ("clicks", ctypes.c_uint8),
        ("padding", ctypes.c_uint8),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float)
    ]

class SDL_MouseWheelEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("which", SDL_MouseID),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("direction", SDL_MouseWheelDirection),
        ("mouse_x", ctypes.c_float),
        ("mouse_y", ctypes.c_float),
        ("integer_x", ctypes.c_int32),
        ("integer_y", ctypes.c_int32)
    ]

class SDL_JoyAxisEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID),
        ("axis", ctypes.c_uint8),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8),
        ("value", ctypes.c_int16),
        ("padding4", ctypes.c_uint16)
    ]

class SDL_JoyBallEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID),
        ("ball", ctypes.c_uint8),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8),
        ("xrel", ctypes.c_int16),
        ("yrel", ctypes.c_int16)
    ]

class SDL_JoyHatEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID),
        ("hat", ctypes.c_uint8),
        ("value", ctypes.c_uint8),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8)
    ]

class SDL_JoyButtonEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID),
        ("button", ctypes.c_uint8),
        ("down", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8)
    ]

class SDL_JoyDeviceEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID)
    ]

class SDL_JoyBatteryEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID),
        ("state", SDL_PowerState),
        ("percent", ctypes.c_int)
    ]

class SDL_GamepadAxisEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID),
        ("axis", ctypes.c_uint8),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8),
        ("value", ctypes.c_int16),
        ("padding4", ctypes.c_uint16)
    ]

class SDL_GamepadButtonEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID),
        ("button", ctypes.c_uint8),
        ("down", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8)
    ]

class SDL_GamepadDeviceEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID)
    ]

class SDL_GamepadTouchpadEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID),
        ("touchpad", ctypes.c_int32),
        ("finger", ctypes.c_int32),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("pressure", ctypes.c_float)
    ]

class SDL_GamepadSensorEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_JoystickID),
        ("sensor", ctypes.c_int32),
        ("data", ctypes.c_float * 3),
        ("sensor_timestamp", ctypes.c_uint64)
    ]

class SDL_AudioDeviceEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_AudioDeviceID),
        ("recording", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8)
    ]

class SDL_CameraDeviceEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_CameraID)
    ]

class SDL_RenderEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID)
    ]

class SDL_TouchFingerEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("touchID", SDL_TouchID),
        ("fingerID", SDL_FingerID),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("dx", ctypes.c_float),
        ("dy", ctypes.c_float),
        ("pressure", ctypes.c_float),
        ("windowID", SDL_WindowID)
    ]

class SDL_PenProximityEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("which", SDL_PenID)
    ]

class SDL_PenMotionEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("which", SDL_PenID),
        ("pen_state", SDL_PenInputFlags),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float)
    ]

class SDL_PenTouchEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("which", SDL_PenID),
        ("pen_state", SDL_PenInputFlags),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("eraser", ctypes.c_bool),
        ("down", ctypes.c_bool)
    ]

class SDL_PenButtonEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("which", SDL_PenID),
        ("pen_state", SDL_PenInputFlags),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("button", ctypes.c_uint8),
        ("down", ctypes.c_bool)
    ]

class SDL_PenAxisEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("which", SDL_PenID),
        ("pen_state", SDL_PenInputFlags),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("axis", SDL_PenAxis),
        ("value", ctypes.c_float)
    ]

class SDL_DropEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("source", ctypes.c_char_p),
        ("data", ctypes.c_char_p)
    ]

class SDL_ClipboardEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("owner", ctypes.c_bool),
        ("num_mime_types", ctypes.c_int32),
        ("mime_types", SDL_POINTER[ctypes.c_char_p])
    ]

class SDL_SensorEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("which", SDL_SensorID),
        ("data", ctypes.c_float * 6),
        ("sensor_timestamp", ctypes.c_uint64)
    ]

class SDL_QuitEvent(ctypes.Structure):
    _fields_ = [
        ("type", SDL_EventType),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64)
    ]

class SDL_UserEvent(ctypes.Structure):
    _fields_ = [
        ("type", ctypes.c_uint32),
        ("reserved", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
        ("windowID", SDL_WindowID),
        ("code", ctypes.c_int32),
        ("data1", ctypes.c_void_p),
        ("data2", ctypes.c_void_p)
    ]

class SDL_Event(ctypes.Union):
    _fields_ = [
        ("type", ctypes.c_uint32),
        ("common", SDL_CommonEvent),
        ("display", SDL_DisplayEvent),
        ("window", SDL_WindowEvent),
        ("kdevice", SDL_KeyboardDeviceEvent),
        ("key", SDL_KeyboardEvent),
        ("edit", SDL_TextEditingEvent),
        ("edit_candidates", SDL_TextEditingCandidatesEvent),
        ("text", SDL_TextInputEvent),
        ("mdevice", SDL_MouseDeviceEvent),
        ("motion", SDL_MouseMotionEvent),
        ("button", SDL_MouseButtonEvent),
        ("wheel", SDL_MouseWheelEvent),
        ("jdevice", SDL_JoyDeviceEvent),
        ("jaxis", SDL_JoyAxisEvent),
        ("jball", SDL_JoyBallEvent),
        ("jhat", SDL_JoyHatEvent),
        ("jbutton", SDL_JoyButtonEvent),
        ("jbattery", SDL_JoyBatteryEvent),
        ("gdevice", SDL_GamepadDeviceEvent),
        ("gaxis", SDL_GamepadAxisEvent),
        ("gbutton", SDL_GamepadButtonEvent),
        ("gtouchpad", SDL_GamepadTouchpadEvent),
        ("gsensor", SDL_GamepadSensorEvent),
        ("adevice", SDL_AudioDeviceEvent),
        ("cdevice", SDL_CameraDeviceEvent),
        ("sensor", SDL_SensorEvent),
        ("quit", SDL_QuitEvent),
        ("user", SDL_UserEvent),
        ("tfinger", SDL_TouchFingerEvent),
        ("pproximity", SDL_PenProximityEvent),
        ("ptouch", SDL_PenTouchEvent),
        ("pmotion", SDL_PenMotionEvent),
        ("pbutton", SDL_PenButtonEvent),
        ("paxis", SDL_PenAxisEvent),
        ("render", SDL_RenderEvent),
        ("drop", SDL_DropEvent),
        ("clipboard", SDL_ClipboardEvent),
        ("padding", ctypes.c_uint8 * 128)
    ]

SDL_PumpEvents: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PumpEvents", None, [], SDL_BINARY]

SDL_EventAction: typing.TypeAlias = SDL_TYPE["SDL_EventAction", SDL_ENUM]

SDL_ADDEVENT, SDL_PEEKEVENT, SDL_GETEVENT = range(3)

SDL_PeepEvents: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PeepEvents", ctypes.c_int, [SDL_POINTER[SDL_Event], ctypes.c_int, SDL_EventAction, ctypes.c_uint32, ctypes.c_uint32], SDL_BINARY]
SDL_HasEvent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasEvent", ctypes.c_bool, [ctypes.c_uint32], SDL_BINARY]
SDL_HasEvents: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasEvents", ctypes.c_bool, [ctypes.c_uint32, ctypes.c_uint32], SDL_BINARY]
SDL_FlushEvent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_FlushEvent", None, [ctypes.c_uint32], SDL_BINARY]
SDL_FlushEvents: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_FlushEvents", None, [ctypes.c_uint32, ctypes.c_uint32], SDL_BINARY]
SDL_PollEvent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PollEvent", ctypes.c_bool, [SDL_POINTER[SDL_Event]], SDL_BINARY]
SDL_WaitEvent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitEvent", ctypes.c_bool, [SDL_POINTER[SDL_Event]], SDL_BINARY]
SDL_WaitEventTimeout: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitEventTimeout", ctypes.c_bool, [SDL_POINTER[SDL_Event], ctypes.c_int32], SDL_BINARY]
SDL_PushEvent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PushEvent", ctypes.c_bool, [SDL_POINTER[SDL_Event]], SDL_BINARY]

SDL_EventFilter: typing.TypeAlias = SDL_FUNC_TYPE["SDL_EventFilter", ctypes.c_bool, [ctypes.c_void_p, SDL_POINTER[SDL_Event]]]

SDL_SetEventFilter: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetEventFilter", None, [SDL_EventFilter, ctypes.c_void_p], SDL_BINARY]
SDL_GetEventFilter: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetEventFilter", ctypes.c_bool, [SDL_POINTER[SDL_EventFilter], SDL_POINTER[ctypes.c_void_p]], SDL_BINARY]
SDL_AddEventWatch: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AddEventWatch", ctypes.c_bool, [SDL_EventFilter, ctypes.c_void_p], SDL_BINARY]
SDL_RemoveEventWatch: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RemoveEventWatch", None, [SDL_EventFilter, ctypes.c_void_p], SDL_BINARY]
SDL_FilterEvents: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_FilterEvents", None, [SDL_EventFilter, ctypes.c_void_p], SDL_BINARY]
SDL_SetEventEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetEventEnabled", None, [ctypes.c_uint32, ctypes.c_bool], SDL_BINARY]
SDL_EventEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EventEnabled", ctypes.c_bool, [ctypes.c_uint32], SDL_BINARY]
SDL_RegisterEvents: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RegisterEvents", ctypes.c_uint32, [ctypes.c_int], SDL_BINARY]
SDL_GetWindowFromEvent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowFromEvent", SDL_POINTER[SDL_Window], [SDL_POINTER[SDL_Event]], SDL_BINARY]