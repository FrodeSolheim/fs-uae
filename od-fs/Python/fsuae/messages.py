from typing import Any

import _fsapp_channel

# This message would make sense for net play clients
FSUAE_MESSAGES_RESET = 10001

FSUAE_MESSAGES_RESTART_WITH_CONFIG = 10002

# Example of "local" message that does not make sense to broadcast for net play
FSUAE_MESSAGES_ADD_ROM = 10003
FSUAE_MESSAGES_EARLY_STOP = 10004

# Drive n is FSUAE_MESSAGES_WRITE_PROTECT_DRIVE_0 + n (and the same for removing the protection).
FSUAE_MESSAGES_WRITE_PROTECT_DRIVE_0 = 0x1000
FSUAE_MESSAGES_REMOVE_WRITE_PROTECT_DRIVE_0 = 0x1010

_channel = None


# FIXME: Could also be MessageService instead


def post_fsuae_message(type: int, data: str = "") -> None:
    _fsapp_channel.add_message(_channel, type, data)


# def process_fsuae_messages() -> None:
#     pass


def set_fsuae_channel(channel: Any) -> None:
    global _channel

    _channel = channel
