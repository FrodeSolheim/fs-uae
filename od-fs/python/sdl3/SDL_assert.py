import os, inspect, ctypes, typing, collections.abc as abc, re

from . import SDL_BINARY, SDL_FUNC_TYPE, SDL_POINTER, SDL_FUNC, SDL_TYPE, SDL_ENUM

SDL_ASSERT_LEVEL: int = 2
SDL_NULL_WHILE_LOOP_CONDITION: int = 0

SDL_AssertState: typing.TypeAlias = SDL_TYPE["SDL_AssertState", SDL_ENUM]

SDL_ASSERTION_RETRY, SDL_ASSERTION_BREAK, SDL_ASSERTION_ABORT, \
    SDL_ASSERTION_IGNORE, SDL_ASSERTION_ALWAYS_IGNORE = range(5)

class SDL_AssertData(ctypes.Structure):
    _fields_ = [
        ("always_ignore", ctypes.c_bool),
        ("trigger_count", ctypes.c_uint),
        ("condition", ctypes.c_char_p),
        ("filename", ctypes.c_char_p),
        ("linenum", ctypes.c_int),
        ("function", ctypes.c_char_p),
        ("next", SDL_POINTER[ctypes.c_void_p])
    ]

SDL_ReportAssertion: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReportAssertion", SDL_AssertState, [SDL_POINTER[SDL_AssertData], ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int], SDL_BINARY]

SDL_TriggerBreakpoint: abc.Callable[[], None] = lambda: breakpoint()
SDL_AssertBreakpoint: abc.Callable[[], None] = lambda: SDL_TriggerBreakpoint()

def SDL_disabled_assert(condition: bool) -> None:
    """Do not call this function directly."""

def SDL_enabled_assert(condition: bool) -> None:
    """Do not call this function directly."""

    while not condition:
        data = SDL_AssertData()
        module = current = inspect.currentframe().f_back.f_back
        while module.f_code.co_name != "<module>": module = module.f_back
        match = re.search(r"\((.*?)\)", inspect.getsource(module).split("\n")[current.f_lineno - 1])
        data.condition = (match.group(1) if match else "<condition>").encode()
        state = SDL_ReportAssertion(ctypes.byref(data), current.f_code.co_name.encode(), os.path.split(current.f_code.co_filename)[-1].encode(), current.f_lineno)

        if state in [SDL_ASSERTION_RETRY]:
            continue

        if state in [SDL_ASSERTION_BREAK]:
            return SDL_AssertBreakpoint()

match SDL_ASSERT_LEVEL:
    case 0:
        SDL_assert: abc.Callable[[bool], None] = lambda condition: SDL_disabled_assert(condition)
        SDL_assert_release: abc.Callable[[bool], None] = lambda condition: SDL_disabled_assert(condition)
        SDL_assert_paranoid: abc.Callable[[bool], None] = lambda condition: SDL_disabled_assert(condition)

    case 1:
        SDL_assert: abc.Callable[[bool], None] = lambda condition: SDL_disabled_assert(condition)
        SDL_assert_release: abc.Callable[[bool], None] = lambda condition: SDL_enabled_assert(condition)
        SDL_assert_paranoid: abc.Callable[[bool], None] = lambda condition: SDL_disabled_assert(condition)

    case 2:
        SDL_assert: abc.Callable[[bool], None] = lambda condition: SDL_enabled_assert(condition)
        SDL_assert_release: abc.Callable[[bool], None] = lambda condition: SDL_enabled_assert(condition)
        SDL_assert_paranoid: abc.Callable[[bool], None] = lambda condition: SDL_disabled_assert(condition)

    case 3:
        SDL_assert: abc.Callable[[bool], None] = lambda condition: SDL_enabled_assert(condition)
        SDL_assert_release: abc.Callable[[bool], None] = lambda condition: SDL_enabled_assert(condition)
        SDL_assert_paranoid: abc.Callable[[bool], None] = lambda condition: SDL_enabled_assert(condition)

    case _:
        SDL_enabled_assert(False)

SDL_assert_always: abc.Callable[[bool], None] = lambda condition: SDL_enabled_assert(condition)
SDL_AssertionHandler: typing.TypeAlias = SDL_FUNC_TYPE["SDL_AssertionHandler", SDL_AssertState, [SDL_POINTER[SDL_AssertData], ctypes.c_void_p]]

SDL_SetAssertionHandler: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAssertionHandler", None, [SDL_AssertionHandler, ctypes.c_void_p], SDL_BINARY]
SDL_GetDefaultAssertionHandler: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDefaultAssertionHandler", SDL_AssertionHandler, [], SDL_BINARY]
SDL_GetAssertionHandler: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAssertionHandler", SDL_AssertionHandler, [SDL_POINTER[ctypes.c_void_p]], SDL_BINARY]
SDL_GetAssertionReport: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAssertionReport", SDL_POINTER[SDL_AssertData], [], SDL_BINARY]
SDL_ResetAssertionReport: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ResetAssertionReport", None, [], SDL_BINARY]