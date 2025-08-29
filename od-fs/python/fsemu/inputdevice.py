class InputDevice:
    # FIXME: InputDeviceType?
    TYPE_GAMEPAD = 103  # 'g'
    TYPE_JOYSTICK = 106  # 'j'
    TYPE_KEYBOARD = 107  # 'k'
    TYPE_MOUSE = 109  # 'm'

    def __init__(self, name: str, type: int, *, instance_id: int = 0) -> None:
        self.name = name
        self.type = type

        self.instance_id = instance_id

    @property
    def is_mouse(self) -> bool:
        return self.type == InputDevice.TYPE_MOUSE

    @property
    def is_keyboard(self) -> bool:
        return self.type == InputDevice.TYPE_KEYBOARD

    def __repr__(self) -> str:
        return f"<InputDevice '{self.name}' ({self.instance_id})>"
