import logging
from typing import Callable

logger = logging.getLogger(__name__)


class StateVar:
    def __init__(self) -> None:
        self.listeners: list[Callable] = []

    # FIXME: add filter to connect/listener
    def connect(self, listener: Callable) -> None:
        self.listeners.append(listener)

    def disconnect(self, listener: Callable) -> None:
        self.listeners.remove(listener)

    def get(self):
        return self.value

    def set(self, value):
        if value == self.value:
            return
        logger.debug("SET %r", value)
        self.value = value
        logger.debug("Calling %r", self.listeners)
        for listener in self.listeners:
            listener(value)


class BoolStateVar(StateVar):
    def __init__(self, value: bool = False) -> None:
        super().__init__()
        self.value = value


class IntStateVar(StateVar):
    def __init__(self, value: int = 0) -> None:
        super().__init__()
        self.value = value


class StrStateVar(StateVar):
    def __init__(self, value: str = "") -> None:
        super().__init__()
        self.value = value


class FilteredStateVar(StateVar):
    # FIXME: set_filter also

    def __init__(self, state, get_filter: Callable) -> None:
        super().__init__()
        # FIXME: No disconnect. Maybe OK

        state.connect(self.__on_state)
        self.get_filter = get_filter
        self.value = self.get_filter(state.value)

    # def filter_state(self, value)

    def __on_state(self, value):
        self.set(self.get_filter(value))


class UaeBool(IntStateVar):
    def __init__(self, config: "UAEConfig", name: str, value: bool = False) -> None:
        super().__init__()
        self.config = config
        self.name = name
        self.value = value

    def set(self, value: bool) -> None:
        if value == self.value:
            return
        super().set(value)
        # FIXME: When to convert to string?
        self.config.set_value(self.name, "true" if self.value else "false")


class UaeInt(IntStateVar):
    def __init__(self, config: "UAEConfig", name: str, value: int = 0) -> None:
        super().__init__()
        self.config = config
        self.name = name
        self.value = value

    def set(self, value: int) -> None:
        if value == self.value:
            return
        super().set(value)
        # FIXME: When to convert to string?
        self.config.set_value(self.name, str(self.value))


class UaeString(StrStateVar):
    def __init__(self, config: "UAEConfig", name: str, value: str = "") -> None:
        super().__init__()
        self.config = config
        self.name = name
        self.value = value

    def set(self, value: str) -> None:
        print("SET VALUE", self.name, self.value)
        if value == self.value:
            return
        super().set(value)
        self.config.set_value(self.name, self.value)


class UAEConfig:
    def set_value(self, name, value):
        import fsemu

        fsemu.set(name, value)

    def __init__(self):
        self.chipset_compatible = UaeString(self, "chipset_compatible")

        self.cpu_model = UaeInt(self, "cpu_model", 0)
        # cpu_type is legacy option that we're going to ignore (?)
        self.fpu_model = UaeInt(self, "cpu_model", 0)
        self.mmu_model = UaeString(self, "mmu_model", "")
        self.mmu_ec = UaeBool(self, "mmu_ec", 0)

        self.floppy0 = UaeString(self, "floppy0")
        self.floppy1 = UaeString(self, "floppy1")
        self.floppy2 = UaeString(self, "floppy2")
        self.floppy3 = UaeString(self, "floppy3")
        self.floppy = [self.floppy0, self.floppy1, self.floppy2, self.floppy3]

        # self.floppy0type =
        # self.floppy1type =
        # self.floppy2type =
        # self.floppy3type =
        self.floppy_type = [
            UaeInt(self, "floppy0type", 0),
            UaeInt(self, "floppy1type", -1),
            UaeInt(self, "floppy2type", -1),
            UaeInt(self, "floppy3type", -1),
        ]

        self.floppy0wp = UaeBool(self, "floppy0wp", False)
        self.floppy1wp = UaeBool(self, "floppy0wp", False)
        self.floppy2wp = UaeBool(self, "floppy0wp", False)
        self.floppy3wp = UaeBool(self, "floppy0wp", False)
        self.floppy_wp = [
            self.floppy0wp,
            self.floppy1wp,
            self.floppy2wp,
            self.floppy3wp,
        ]

        # self._floppy0enabled =
        # self._floppy1enabled =
        # self._floppy2enabled =
        # self._floppy3enabled =
        self.floppy_enabled = [
            FilteredStateVar(self.floppy_type[0], lambda x: x != -1),
            FilteredStateVar(self.floppy_type[1], lambda x: x != -1),
            FilteredStateVar(self.floppy_type[2], lambda x: x != -1),
            FilteredStateVar(self.floppy_type[3], lambda x: x != -1),
        ]

        # self._floppy0inserted = FilteredStateVar(self.floppy0type, lambda x: x != -1)
        # self._floppy1inserted = FilteredStateVar(self.floppy1type, lambda x: x != -1)
        # self._floppy2inserted = FilteredStateVar(self.floppy2type, lambda x: x != -1)
        # self._floppy3inserted = FilteredStateVar(self.floppy3type, lambda x: x != -1)
        self.floppy_inserted_filtered = [
            FilteredStateVar(self.floppy[0], lambda x: x != ""),
            FilteredStateVar(self.floppy[1], lambda x: x != ""),
            FilteredStateVar(self.floppy[2], lambda x: x != ""),
            FilteredStateVar(self.floppy[3], lambda x: x != ""),
        ]

        self.floppy_speed = UaeInt(self, "floppy_speed", 100)

        # FIXME: FS-UAE changes UAE default from 0 to 1...
        self.floppy0sound = UaeInt(self, "floppy0sound", 0)
        self.floppy1sound = UaeInt(self, "floppy1sound", 0)
        self.floppy2sound = UaeInt(self, "floppy2sound", 0)
        self.floppy3sound = UaeInt(self, "floppy3sound", 0)
        self.floppy_sound = [
            self.floppy0sound,
            self.floppy1sound,
            self.floppy2sound,
            self.floppy3sound,
        ]
