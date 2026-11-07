from fsuae.workspace.shellwindow import ShellWindow


class FSUAEDirectoryWindow(ShellWindow):
    _instance = None

    def __init__(self):
        size = (400, 300)
        super().__init__("System:FS-UAE", size=size)
        self.move((30, 310))
        self.style.background_color = (0xEE, 0xEE, 0xEE, 0xFF)
