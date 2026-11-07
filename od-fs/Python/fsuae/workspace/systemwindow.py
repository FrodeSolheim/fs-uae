from fsuae.workspace.shellwindow import ShellWindow


class SystemWindow(ShellWindow):
    def __init__(self):
        super().__init__("System:")
        self.move((160, 120))
        self.style.background_color = (0xEE, 0xEE, 0xEE, 0xFF)
