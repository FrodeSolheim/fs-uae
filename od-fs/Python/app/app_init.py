import atexit
import logging
import os
import sys

import _fsapp_channel  # type: ignore
import _fsuae_main  # type: ignore
import fsemu
from fsapp.eventservice import EventService
from fsapp.timerservice import TimerService
from fsemu.inputdeviceservice import InputDeviceService
from fsemu.inputportservice import InputPortService
from fsgui.windowmanager import WindowManager
from fsuae.amigacontext import AmigaContext
from fsuae.f12window import F12Window
from fsuae.fsuaemainwindow import FSUAEMainWindow
from fsuae.gui.inputgrabnotification import InputGrabNotification
from fsuae.gui.notificationservice import NotificationService
from fsuae.gui.notificationtype import NotificationType
from fsuae.input.devicerobotservice import DeviceRobotService

# from fsuae.input.inputdevicewindow import InputDeviceWindow
from fsuae.messages import (
    FSUAE_MESSAGES_ADD_ROM,
    FSUAE_MESSAGES_EARLY_STOP,
    FSUAE_MESSAGES_RESTART_WITH_CONFIG,
    post_fsuae_message,
    set_fsuae_channel,
    # process_fsuae_messages,
)
from fsuae.roms.romservice import ROMService
from fsuae.rundirectory import create_run_dir, delete_run_dir, remove_stale_run_dirs
from fsuae.servicecontainer import ServiceContainer
from fsuae.services.pathservice import PathService
from fsuae.svgicon import SvgIcon
from fsuae.uaeconfigservice import UAEConfigService
from fsuae.whdload.runpreparation import (
    DamagedArchiveError,
    KickstartMissingError,
    NotAWHDLoadArchiveError,
    WHDLoadRunError,
    prepare_whdload_run,
)
from fsuae.workspace import init_fsuae_workspace
from uae.options.blacklisted import blacklisted_uae_options

from app.map_ports import map_ports

logger = logging.getLogger(__name__)

# Width of error notifications (the description is wrapped to fit).
ERROR_NOTIFICATION_WIDTH = 560


class StartError(Exception):
    """The file given on the command line cannot be started; shown as an error notification."""

    def __init__(self, title: str, description: str) -> None:
        super().__init__(f"{title}: {description}")
        self.title = title
        self.description = description


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

    # Remove run directories left behind by sessions that did not end cleanly (crashes). Only
    # those whose owner process has ended are removed, so this is safe with other FS-UAE
    # instances running. A failure here must not stop start-up.
    try:
        remove_stale_run_dirs(services.path.cache_dir)
    except Exception:
        logger.exception("Could not remove stale run directories")

    services.rom.scan_kickstart_dirs()

    for rom in services.rom.roms:
        post_fsuae_message(FSUAE_MESSAGES_ADD_ROM, f"{rom.crc32},{rom.sha1},{rom.path}")

    # A WHDLoad archive is prepared before emulation starts. Its config is handed over among the
    # early messages, which the emulation thread processes before starting the emulation core, so
    # the core starts directly with it (instead of booting the default machine and restarting).
    # If it cannot be started, no emulation is started at all, and an error notification is shown
    # instead (further down, once the notification service exists). The user closes FS-UAE.
    start_error: StartError | None = None
    last_arg = sys.argv[-1]
    if last_arg.lower().endswith(".lha"):
        try:
            whdload_config = prepare_whdload_archive(last_arg, services)
        except StartError as e:
            start_error = e
        else:
            post_fsuae_message(FSUAE_MESSAGES_RESTART_WITH_CONFIG, whdload_config)

    post_fsuae_message(FSUAE_MESSAGES_EARLY_STOP)

    # arc.services?
    # arc.config?

    # process_fsuae_messages()

    F12Window.setup(services)

    # FIXME: Messages must be processed before starting emulation (?)
    # Maybe fsuae_main.start() should do that...

    # sys.exit(0)

    # fsuae_roms.

    # FIXME: Maybe delay start even further?
    if start_error is None:
        _fsuae_main.start()
    else:
        # The main loop must render the GUI without waiting for emulation video.
        _fsuae_main.run_without_emulation()

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
        TimerService.get(),
    )

    init_fsuae_workspace()

    from fsuae.misc.clockapp import setup_clock_app

    setup_clock_app(services)

    # InputDeviceService must have been created first
    # InputDeviceWindow.instance().show()

    if last_arg.endswith(".uae"):
        if os.path.exists(last_arg):
            load_config(last_arg)
    # else:
    #     F12Window.instance().show()

    # Initialize notification service
    notification_service = NotificationService().set_instance()

    shortcut_mod = "Cmd" if sys.platform == "darwin" else "Alt"

    print("---------------------------------------------------------------------------------------")

    notification_service.create_notification(
        NotificationType.MOUSE_GRABBED,
        "Mouse cursor grabbed",
        f"{shortcut_mod}+G or middle click to release",
        SvgIcon("svg-custom/mouse-locked.svg"),
    )

    InputGrabNotification(notification_service, services.event)
    # notification_service.show_notification(NotificationType.MOUSE_GRABBED)
    print("---------------------------------------------------------------------------------------")

    if start_error is not None:
        show_start_error(notification_service, start_error)


def show_start_error(notification_service: NotificationService, error: StartError) -> None:
    """Show an error notification that stays until FS-UAE is closed."""
    try:
        notification_service.create_notification(
            NotificationType.START_ERROR,
            error.title,
            error.description,
            SvgIcon("src/svg/alert.svg"),
            duration=0,
            width=ERROR_NOTIFICATION_WIDTH,
        )
        notification_service.show_notification(NotificationType.START_ERROR)
    except Exception:
        logger.exception("Could not show the error notification")


def prepare_whdload_archive(path: str, services: ServiceContainer) -> str:
    """Build a run directory for a WHDLoad archive and return the config to start it with.

    Raises `StartError` (after logging why) if the archive cannot be started; the run directory
    is removed again in that case. Otherwise, the run directory is deleted when FS-UAE exits,
    after the emulation thread has stopped.
    """
    run_dir = None
    try:
        archive_name = os.path.splitext(os.path.basename(path))[0]
        run_dir = create_run_dir(services.path.cache_dir, archive_name)
        run = prepare_whdload_run(path, run_dir, services.rom.roms, services.path.resources_dir)
    except Exception as e:
        if isinstance(e, (WHDLoadRunError, OSError)):
            logger.error("Cannot start %s: %s", path, e)
        else:
            logger.exception("Cannot start %s: unexpected error", path)
        if run_dir is not None:
            delete_run_dir(run_dir)
        title, description = whdload_error_message(path, e, services.path.kickstart_dir)
        raise StartError(title, description) from e
    logger.info("WHDLoad run directory: %s", run_dir)
    # Called on the main thread at exit, only once emulation has stopped and been joined (so the
    # core has flushed and closed the hard drive), on every quit path.
    _fsuae_main.set_exit_handler(lambda: delete_run_dir(run_dir))
    return run.config


def whdload_error_message(path: str, error: Exception, kickstart_dir: str) -> tuple[str, str]:
    """The title and description of the error notification for a WHDLoad archive."""
    name = os.path.basename(path)
    if isinstance(error, NotAWHDLoadArchiveError):
        return "Not a WHDLoad archive", f"{name} cannot be started because {error.reason}."
    if isinstance(error, KickstartMissingError):
        return (
            "Kickstart ROM missing",
            f"{name} needs {error.kickstart.name}. "
            f"Add the ROM to {kickstart_dir} and start FS-UAE again.",
        )
    if isinstance(error, DamagedArchiveError):
        return (
            "Damaged WHDLoad archive",
            f"{name} cannot be started because {error.member} cannot be read ({error.reason}).",
        )
    if isinstance(error, (WHDLoadRunError, OSError)):
        return "Cannot start WHDLoad archive", f"{name}: {error}"
    return (
        "Cannot start WHDLoad archive",
        f"{name}: unexpected error ({type(error).__name__}: {error}). See the log file.",
    )


def load_config(path: str) -> None:

    config: list[tuple[str, str]] = []
    with open(path, "r") as f:
        for line in f.readlines():
            line = line.strip()
            try:
                key, value = line.split("=", 1)
            except ValueError:
                continue
            key = key.lower().strip()
            value = value.replace("\\", "/")
            config.append((key, value))

    # FIXME: Temporary hack to make relative references from .uae work! (Needs to be done after
    # loading the .uae config, which might itself be given as a relative path).
    os.chdir(os.path.dirname(path) or ".")

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
        if key == "performance_gui":
            # FS-UAE's own option, for fsemu's performance graph (not for the emulation core).
            fsemu.set_performance_gui(value.strip())
            continue
        new_config.append(f"{key}={value}\n")

    new_config_str = "".join(new_config)

    post_fsuae_message(
        FSUAE_MESSAGES_RESTART_WITH_CONFIG,
        "".join(new_config_str),
    )
