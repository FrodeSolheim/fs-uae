import _fsapp  # type: ignore
import _fsapp_channel  # type: ignore

_main_channel: object


def init_fsapp_main():
    global _main_channel
    _main_channel = _fsapp.get_main_channel()  # type: ignore


def post_to_fsapp_main(type: int, data: str) -> None:
    _fsapp_channel.add_message(_main_channel, type, data)
