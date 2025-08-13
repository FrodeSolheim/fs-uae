# import _fsuae_main
# import fsapp_channel  # type: ignore
# from uae.optionblacklist import option_blacklist

# from fsuae.fsuaemainwindow import FSUAEMainWindow
# from fsuae.messages import (
#     FSUAE_MESSAGE_ADD_ROM,
#     FSUAE_MESSAGE_EARLY_STOP,
#     FSUAE_MESSAGE_RESTART_WITH_CONFIG,
#     post_fsuae_message,
#     set_fsuae_channel,
#     # process_fsuae_messages,
# )
# from fsuae.pathservice import PathService
# from fsuae.roms.romservice import ROMService


def init():
    """This function is called from fsuae-main.py"""

    # raise Exception("test")

    from app.app_init import app_init

    app_init()

    # print("init!")

    # # import _fsapp

    # global _window

    # # title = "FS-UAE"
    # # real_size = (800, 600)
    # fullscreen = False
    # # print("a")
    # # _window = fsapp.create_window(title, real_size, fullscreen)  # type: ignore

    # # Must create main window before starting emulation (at least currently)
    # # due to fsemu_video_init being called when we do this.
    # _main_window = FSUAEMainWindow(fullscreen=fullscreen)
    # # print("b")

    # from fsgui.window import Window
    # Window(size=(200, 200)).show().move((100, 600))

    # F12Window.setup(services)

    # # fsapp.wait_for_window(_window)

    # channel = fsapp_channel.create()
    # set_fsuae_channel(channel)
    # _fsuae_main.init(channel)

    # path_service = PathService().set_instance()
    # rom_service = ROMService(path_service)

    # rom_service.scan_kickstarts_dir()

    # for rom in rom_service.roms:
    #     post_fsuae_message(FSUAE_MESSAGE_ADD_ROM, f"{rom.crc32},{rom.sha1},{rom.path}")
    # post_fsuae_message(FSUAE_MESSAGE_EARLY_STOP)

    # # FIXME: Maybe delay start even further?
    # _fsuae_main.start()

    # import sys

    # last_arg = sys.argv[-1]
    # if last_arg.endswith(".uae"):
    #     config = []
    #     with open(last_arg, "r") as f:
    #         for line in f.readlines():
    #             line = line.strip()
    #             try:
    #                 key, value = line.split("=", 1)
    #             except ValueError:
    #                 continue
    #             key = key.lower().strip()
    #             value = value.replace("\\", "/")
    #             config.append((key, value))

    #     # for key, value in config:
    #     #     import fsemu

    #     #     if key == "input.joymouse_speed_analog":
    #     #         continue

    #     #     fsemu.set(key, value)
    #     # fsemu.post(uae.INPUTEVENT_SPC_HARDRESET)

    #     new_config = []
    #     for key, value in config:
    #         # if key == "input.joymouse_speed_analog":
    #         #     continue
    #         if key in option_blacklist:
    #             continue
    #         new_config.append(f"{key}={value}\n")

    #     new_config_str = "".join(new_config)

    #     post_fsuae_message(
    #         FSUAE_MESSAGE_RESTART_WITH_CONFIG,
    #         "".join(new_config_str),
    #     )
