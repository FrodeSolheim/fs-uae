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
        logger.debug("(UAECONFIG2) SET  %r", value)
        self.value = value
        logger.debug("calling %r", self.listeners)
        for listener in self.listeners:
            listener(value)


class StrStateVar(StateVar):
    def __init__(self, value: str = "") -> None:
        super().__init__()
        self.value = value


class UaeString(StrStateVar):
    def __init__(
        self, config: "UAEConfig2", name: str, value: str = ""
    ) -> None:
        super().__init__()
        self.config = config
        self.name = name
        self.value = value

    def set(self, value: str, internal: bool = False) -> None:
        if value == self.value:
            return
        super().set(value)
        # self.config.set_value(self.name, self.value)
        self.config._config[self.name] = self.value

        if not internal:
            import fsemu

            print("FIXME: Maybe send this from somewhere else")
            fsemu.set(self.name, self.value)


class UAEConfig2:
    def __init__(self):
        self._config = {}

        self.cpu_model = UaeString(self, "cpu_model")
        self.chipset_compatible = UaeString(self, "chipset_compatible")

        self.floppy0 = UaeString(self, "floppy0")
        self.floppy1 = UaeString(self, "floppy1")
        self.floppy2 = UaeString(self, "floppy2")
        self.floppy3 = UaeString(self, "floppy3")

        self.floppy_n = [
            self.floppy0,
            self.floppy1,
            self.floppy2,
            self.floppy3,
        ]

        self.mmu_model = UaeString(self, "mmu_model")

        self.gfx_center_vertical = UaeString(self, "gfx_center_vertical")
        self.gfx_center_horizontal = UaeString(self, "gfx_center_horizontal")

    def _update(self) -> None:
        for key in sorted(self._config):
            value = self._config[key]
            state_var = getattr(self, key, None)
            if state_var is not None:
                if state_var.value != value:
                    state_var.set(value, internal=True)

    def warning(self, message) -> None:
        logger.warning("%s", message)

    # The following functions are called whenever the corresponding config key
    # has a value change. The functions are called in asciibetical order after
    # *all* config key/values have been changed

    def _cpu_model_changed(self) -> None:
        logger.debug("%s", self._config["cpu_model"])

    def _fpu_model_changed(self) -> None:
        # self._config["fpu_model"]
        pass

    def _gfx_center_horizontal_changed(self) -> None:
        # none or smart (simple is old value)
        value = self._config["gfx_center_horizontal"]
        if value == "simple":
            self._config["gfx_center_horizontal"] = "smart"

    def _gfx_center_vertical_changed(self) -> None:
        # none or smart (simple is old value)
        value = self._config["gfx_center_vertical"]
        if value == "simple":
            self._config["gfx_center_vertical"] = "smart"

    def _mmu_model_changed(self) -> None:
        mmu_model = self._config["mmu_model"]
        cpu_model = self._config["cpu_model"]

        if "ec" in mmu_model:
            mmu_model = mmu_model.replace("ec", "")
            ec = True
        else:
            ec = False

        # MMU_NONE = 0
        # MMU_FULL = 1
        # MMU_EC = 2

        mmu_type = "MMU_NONE"

        if mmu_model == "":
            mmu_type = "MMU_NONE"
        elif mmu_model != cpu_model:
            self.warning(f"CPU ({cpu_model}) and MMU ({mmu_model})  mismatch")
            mmu_type = "MMU_NONE"  # Or maybe True. Invalid anyway.
        else:
            if ec:
                mmu_type = "MMU_EC"
            else:
                mmu_type = "MMU_FULL"

        self._config["x_mmu_type"] = mmu_type
        # FIXME: Update GUI state
