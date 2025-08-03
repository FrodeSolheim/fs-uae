from fsgui.borderlesswindow import BorderlessWindow


class SpecialWindow(BorderlessWindow):
    def __init__(self, *, size, special):
        super().__init__("", size=size, special=special)
