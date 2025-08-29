import logging
from typing import Self

from fsemu.inputdeviceservice import InputDeviceService
from fsemu.inputportservice import InputPortService
from fsgui.layout import HorizontalLayout, VerticalLayout
from fsgui.widget import Widget
from fsgui.window import Window

from fsuae.input.inputdeviceselector import InputDeviceSelector
from fsuae.input.inputmodeselector import InputModeSelector
from fsuae.servicecontainer import ServiceContainer
from fsuae.uaeconfig2 import UAEConfig2

logger = logging.getLogger(__name__)


class InputControlWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("InputControl", padding=24)

        # self.move((660, 82))
        # 80)

        VerticalLayout(gap=24)

        # Heading("Input ports")

        # FIXME: Should layouts fill by default? Maybe...

        services = ServiceContainer.instance()

        # input_device_service = InputDeviceService.instance()
        # input_port_service = InputPortService.instance()
        # InputControlWidget(input_port_service, input_device_service, 0).fill()
        # InputControlWidget(input_port_service, input_device_service, 1).fill()

        for port_index in range(2):
            with VerticalLayout(gap=8).fill():
                InputModeSelector(services.input_ports, services.input_devices, port_index).fill()
                InputDeviceSelector(
                    services.uae_config.config2,
                    services.input_ports,
                    services.input_devices,
                    port_index,
                ).fill()

        self.resize_to_fit_content()
        # self.set_width(692)
        self.set_width(280)
        self.move((24, 586))


class InputControlWidget(Widget):
    def __init__(
        self,
        config: UAEConfig2,
        input_port_service: InputPortService,
        input_device_service: InputDeviceService,
        port_index: int,
    ) -> None:
        super().__init__()

        HorizontalLayout(gap=12)

        # FIXME: # .set_width(300) does not have any effect
        # FIXME: # .set_min_width instead?
        InputModeSelector(input_port_service, input_device_service, port_index).set_min_width(200)

        InputDeviceSelector(config, input_port_service, input_device_service, port_index).expand()
