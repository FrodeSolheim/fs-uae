from typing import Self

from fsgui.button import Button
from fsgui.checkbox import CheckBox
from fsgui.choice import Choice
from fsgui.heading import Heading
from fsgui.label import Label
from fsgui.layout import HorizontalLayout, VerticalLayout
from fsgui.slider import DiscreteSlider
from fsgui.textfield import TextField
from fsgui.window import Window


class WidgetTestWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("WidgetTest", padding=24, size=(600, 400))

        # self.move((100, 100))
        self.center_window()

        HorizontalLayout(gap=24)

        with VerticalLayout(gap=24):
            Heading("Heading")
            Button("Button")
            Label("Label")
            CheckBox("Check box")
            DiscreteSlider(10)
            Choice(["Choice item 1", "Choice item 2", "Choice item 3"])
            TextField("Text field").fill()

        with VerticalLayout(gap=24):
            Heading("Heading").disable()
            Button("Button").disable()
            Label("Label").disable()
            CheckBox("Check box").disable()
            DiscreteSlider(10).disable()
            Choice(["Choice Item i", "Choice item 2", "Choice item 3"]).disable()
            TextField("Text field").disable().fill()
