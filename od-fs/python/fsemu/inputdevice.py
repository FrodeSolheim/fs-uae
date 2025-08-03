class InputDevice:
    TYPE_CONTROLLER = 103  # 'g'
    TYPE_KEYBOARD = 107  # 'k'
    TYPE_MOUSE = 109  # 'm'
    TYPE_JOYSTICK = 106  # 'j'

    def __init__(self, name: str, type: int) -> None:
        self.name = name
        self.type = type
