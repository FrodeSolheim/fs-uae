from typing import Self

from fsemu.inputdevice import InputDevice
from fsemu.inputdeviceservice import InputDeviceService
from fsgui.item import Item
from fsgui.layout import VerticalLayout
from fsgui.listview import ListView
from fsgui.window import Window


class InputDeviceWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("Input devices", padding=24, size=(300, 400))

        self.move((10, 46))

        VerticalLayout()

        self.list_view = ListView().fill().expand()

        # Add existing devices
        input_device_service = InputDeviceService.instance()
        for device in input_device_service.devices:
            self._add_device(device)

        # And make sure to get notifications about future changes
        self.on(input_device_service.device_added, self.__device_added)
        self.on(input_device_service.device_removed, self.__device_removed)

    def _add_device(self, device: InputDevice) -> None:
        self.list_view.add_item(Item(device.name, data=device))

    def __device_added(self, device: InputDevice) -> None:
        self.list_view.add_item(Item(device.name, data=device))

    def __device_removed(self, device: InputDevice) -> None:
        self.list_view.remove_item_by_data(device)
