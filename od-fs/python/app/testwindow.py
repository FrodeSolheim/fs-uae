import os

import uae
import uae.config

import fsemu
from fsgui.button import Button
from fsgui.decoratedwindow import DecoratedWindow
from fsgui.label import Label
from fsgui.textfield import TextField
from fsuae.aboutwindow import AboutWindow


class TestWindow(DecoratedWindow):
    def __init__(self):
        size = (500, 200)
        super().__init__("Quick Start", size)

        self.set_position((20, 620))

        label = Label("DF0:", size=(180, 28))
        label.set_position((20, 20))

        def on_button_activate():
            from fsgui.filedialog import FileDialog

            # FIXME: Maybe on_accept should accept dialog as a param instead?
            # FIXME: Maybe all callbacks / signals should get the object
            # as first param?

            # EDIT: FIXED BY NOT REFERENCING THE DIALOG
            def on_accept(dialog):
                # nonlocal file_dialog
                path = dialog.get_file()
                text_field.set_text(os.path.basename(path))

                fsemu.set(uae.config.FLOPPY0, path)

                # This hack (and with nonlocal file_dialog) fixes the reference cycle
                # file_dialog = None

            # FIXME: file_dialog reference keeping on_accept + closure alive keeping
            # text_field alive (...) ?

            # file_dialog = FileDialog(None, on_accept=on_accept)
            # file_dialog.open()

            # FIXME: Or show()...
            FileDialog(self, on_accept=on_accept).open()

        button = Button("Browse...", parent=self, size=(80, 28))
        button.set_position((370, 20))
        button.on_activate = on_button_activate

        text_field = TextField("Text field...", parent=self, size=(230, 28))
        text_field.set_position((120, 20))

        # button = Button(self, "Destroy", size=(100, 28))
        # button.set_position((20, 122))
        # button.on_activate = on_destroy_button_activate

        button = Button("Reset", parent=self, size=(80, 28))
        button.set_position((20, 80))
        # button.on_activate = on_reset_button_activate
        # button.on_activate = lambda: fsemu.post(uae.INPUTEVENT_SPC_SOFTRESET)
        button.on_activate = lambda: fsemu.post(uae.INPUTEVENT_SPC_HARDRESET)

        button = Button("About", parent=self, size=(80, 28))
        button.set_position((20, 120))
        button.on_activate = lambda: AboutWindow.instance().show()

        button = Button("Å", parent=self, size=(28, 28))
        button.set_position((20, 160))

        button = Button("g", parent=self, size=(28, 28))
        button.set_position((60, 160))

        button = Button("Åg", parent=self, size=(28, 28))
        button.set_position((100, 160))
