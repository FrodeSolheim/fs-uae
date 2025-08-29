import atexit
import sys

import _fsapp_channel  # type: ignore
import _fsuae_main  # type: ignore
from fsapp.eventservice import EventService
from fsapp.tickservice import TickService
from fsemu.inputdeviceservice import InputDeviceService
from fsemu.inputportservice import InputPortService
from fsgui.windowmanager import WindowManager
from fsuae.amigacontext import AmigaContext
from fsuae.f12window import F12Window
from fsuae.fsuaemainwindow import FSUAEMainWindow
from fsuae.input.devicerobotservice import DeviceRobotService
from fsuae.input.inputdevicewindow import InputDeviceWindow
from fsuae.messages import (
    FSUAE_MESSAGE_ADD_ROM,
    FSUAE_MESSAGE_EARLY_STOP,
    FSUAE_MESSAGE_RESTART_WITH_CONFIG,
    post_fsuae_message,
    set_fsuae_channel,
    # process_fsuae_messages,
)
from fsuae.pathservice import PathService
from fsuae.roms.romservice import ROMService
from fsuae.servicecontainer import ServiceContainer
from fsuae.uaeconfigservice import UAEConfigService
from fsuae.workspace import init_fsuae_workspace
from uae.options.blacklisted import blacklisted_uae_options

from app.map_ports import map_ports


def on_exit() -> None:
    import _fsapp  # type: ignore

    _fsapp.quit()  # type: ignore


def app_init() -> None:
    atexit.register(on_exit)

    # FIXME: program name is not part of sys.argv (for some reason), injecting
    # a dummy program name now so later code works as expected.
    sys.argv.insert(0, "fs-uae")

    fullscreen = False
    for arg in sys.argv:
        if arg == "--fullscreen":
            fullscreen = True

    # Must create main window before starting emulation (at least currently)
    # due to fsemu_video_init being called when we do this.
    _main_window = FSUAEMainWindow(fullscreen=fullscreen)

    # FIXME: if app_init raises an exception, the entire app hangs
    # Improve this a bit!

    channel = _fsapp_channel.create()
    set_fsuae_channel(channel)
    _fsuae_main.init(channel)

    _window_manager = WindowManager.get()

    services = ServiceContainer().set_instance()

    services.event = EventService.instance()
    # UaeService?
    services.uae_config = UAEConfigService(services.event).set_instance()
    # DeviceService? Maybe to generic
    services.input_devices = InputDeviceService(services.event).set_instance()
    # PortService? Maybe to generic
    services.input_ports = InputPortService().set_instance()
    services.path = PathService().set_instance()
    services.rom = ROMService(services.path)
    services.tick = TickService().get()

    services.rom.scan_kickstarts_dir()

    for rom in services.rom.roms:
        post_fsuae_message(FSUAE_MESSAGE_ADD_ROM, f"{rom.crc32},{rom.sha1},{rom.path}")
    post_fsuae_message(FSUAE_MESSAGE_EARLY_STOP)

    # arc.services?
    # arc.config?

    # process_fsuae_messages()

    F12Window.setup(services)

    # FIXME: Messages must be processed before starting emulation (?)
    # Maybe fsuae_main.start() should do that...

    # sys.exit(0)

    # fsuae_roms.

    # FIXME: Maybe delay start even further?
    _fsuae_main.start()

    map_ports(services)

    # Create Amiga Context after mapping ports. FIXME: Better AmigaContext creation later
    ctx = AmigaContext(services)
    # FIXME: Disallow / remove this later
    AmigaContext._instance = ctx  # type: ignore

    services.device_robot = DeviceRobotService(
        # [ctx.ports[1], ctx.ports[0], ctx.ports[2], ctx.ports[3]],
        [ctx.ports[0], ctx.ports[1], ctx.ports[2], ctx.ports[3]],
        [
            ctx.config.joystick_port_1,
            ctx.config.joystick_port_0,
            ctx.config.joystick_port_2,
            ctx.config.joystick_port_3,
        ],
        services.uae_config.config2,
        services.input_ports,
        services.input_devices,
        services.tick,
    )

    init_fsuae_workspace()

    from fsuae.misc.clockapp import setup_clock_app

    setup_clock_app(services)

    # InputDeviceService must have been created first
    InputDeviceWindow().instance().show()

    last_arg = sys.argv[-1]
    if last_arg.endswith(".uae"):
        config: list[tuple[str, str]] = []
        with open(last_arg, "r") as f:
            for line in f.readlines():
                line = line.strip()
                try:
                    key, value = line.split("=", 1)
                except ValueError:
                    continue
                key = key.lower().strip()
                value = value.replace("\\", "/")
                config.append((key, value))

        # for key, value in config:
        #     import fsemu

        #     if key == "input.joymouse_speed_analog":
        #         continue

        #     fsemu.set(key, value)
        # fsemu.post(uae.INPUTEVENT_SPC_HARDRESET)

        new_config: list[str] = []
        for key, value in config:
            # if key == "input.joymouse_speed_analog":
            #     continue
            if key in blacklisted_uae_options:
                continue
            new_config.append(f"{key}={value}\n")

        new_config_str = "".join(new_config)

        post_fsuae_message(
            FSUAE_MESSAGE_RESTART_WITH_CONFIG,
            "".join(new_config_str),
        )
