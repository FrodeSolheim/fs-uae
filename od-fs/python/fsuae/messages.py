from typing import Any

import fsapp_channel  # type: ignore

# This message would make sense for net play clients
FSUAE_MESSAGE_RESET = 10001

FSUAE_MESSAGE_RESTART_WITH_CONFIG = 10002

# Example of "local" message that does not make sense to broadcast for net play
FSUAE_MESSAGE_ADD_ROM = 10003

_channel = None


# FIXME: Could also be MessageService instead


def post_fsuae_message(type: int, data: str = "") -> None:
    fsapp_channel.add_message(_channel, type, data)


# def process_fsuae_messages() -> None:
#     pass


def set_fsuae_channel(channel: Any) -> None:
    global _channel

    _channel = channel
