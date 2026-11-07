import sys, os, ctypes, atexit, types

from . import SDL_POINTER

from .SDL_main import SDL_EnterAppMainCallbacks, SDL_RunApp, SDL_main_func, \
    SDL_AppEvent_func, SDL_AppInit_func, SDL_AppIterate_func, SDL_AppQuit_func

if not int(os.environ.get("SDL_MAIN_HANDLED", "0")) > 0 and not int(os.environ.get("SDL_MAIN_NOIMPL", "0")) > 0:
    import __main__

    if int(os.environ.get("SDL_MAIN_USE_CALLBACKS", "0")) > 0:
        @SDL_main_func
        def SDL_main(argc: ctypes.c_int, argv: SDL_POINTER[ctypes.c_char_p]) -> ctypes.c_int:
            callbacks = [getattr(__main__, i, None) for i in ["SDL_AppInit", "SDL_AppIterate", "SDL_AppEvent", "SDL_AppQuit"]]

            for index, i in enumerate(callbacks):
                if i is not None and not isinstance(i, ctypes._CFuncPtr):
                    callbacks[index] = globals()[f"{i.__name__}_func"](i)

            return SDL_EnterAppMainCallbacks(argc, argv, *callbacks)
        
        os.environ["SDL_MAIN_CALLBACK_STANDARD"] = "1"
        setattr(__main__, "SDL_main", SDL_main)

    if (not int(os.environ.get("SDL_MAIN_USE_CALLBACKS", "0")) > 0 or int(os.environ.get("SDL_MAIN_CALLBACK_STANDARD", "0")) > 0) and not int(os.environ.get("SDL_MAIN_EXPORTED", "0")) > 0:
        @atexit.register
        def SDL_ATEXIT_HANDLER() -> None:
            if main := (getattr(__main__, "SDL_main", None) or getattr(__main__, "main", None)):
                if not isinstance(main, SDL_main_func):
                    if isinstance(main, types.ModuleType): return
                    if main.__name__ == "main": return
                    main = SDL_main_func(main)

                return SDL_RunApp(len(sys.argv), (ctypes.c_char_p * len(sys.argv))(*[i.encode() for i in sys.argv]), main, None)