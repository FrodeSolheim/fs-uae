from fsgui.checkbox import CheckBox
from fsgui.textfield import TextField

from fsuae.uaeconfig2 import UaeString


class UaeConfigCheckBox(CheckBox):
    # FIXME: Start first?
    def __init__(
        self,
        text: str,
        state: UaeString,
        enabled_value: str,
        disabled_value: str,
    ) -> None:
        super().__init__(text)

        self.enabled_value = enabled_value
        self.disabled_value = disabled_value

        self.state = state
        self.connect(self.state, self._on_state)

    def _on_state(self, value: str) -> None:
        if value == self.enabled_value:
            # FIXME: Or set_checked (?)
            # FIXME: Include a parameter to say if we want to broadcast events or not?
            self.set_value(True)
        else:
            self.set_value(False)

    # FIXME: on_check ? on_checked?
    def on_change(self) -> None:
        value = self.value
        if value:
            self.state.set(self.enabled_value)
        else:
            self.state.set(self.disabled_value)


class UaeConfigTextField(TextField):
    def __init__(self, state: UaeString) -> None:
        super().__init__("")

        self.state = state
        # self.connect(self.state, self._on_state)

        self.connect_str(state, self.set_text)

    def _on_state(self, value: str) -> None:
        self.set_text(value)

    # def on_text(self) -> None:
    #     self.state.set(self.text)
