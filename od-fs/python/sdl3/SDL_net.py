import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC, SDL_NET_BINARY
from .SDL_version import SDL_VERSIONNUM

SDL_NET_MAJOR_VERSION, SDL_NET_MINOR_VERSION, SDL_NET_MICRO_VERSION = 3, 0, 0
SDL_NET_VERSION: int = SDL_VERSIONNUM(SDL_NET_MAJOR_VERSION, SDL_NET_MINOR_VERSION, SDL_NET_MICRO_VERSION)

SDL_NET_VERSION_ATLEAST: abc.Callable[[int, int, int], bool] = lambda x, y, z: \
    (SDL_NET_MAJOR_VERSION >= x) and (SDL_NET_MAJOR_VERSION > x or SDL_NET_MINOR_VERSION >= y) and \
        (SDL_NET_MAJOR_VERSION > x or SDL_NET_MINOR_VERSION > y or SDL_NET_MICRO_VERSION >= z)

NET_GetVersion: abc.Callable[..., typing.Any] = SDL_FUNC["NET_GetVersion", ctypes.c_int, [], SDL_NET_BINARY]
NET_Init: abc.Callable[..., typing.Any] = SDL_FUNC["NET_Init", ctypes.c_bool, [], SDL_NET_BINARY]
NET_Quit: abc.Callable[..., typing.Any] = SDL_FUNC["NET_Quit", None, [], SDL_NET_BINARY]

class NET_Address(ctypes.c_void_p):
    ...

NET_ResolveHostname: abc.Callable[..., typing.Any] = SDL_FUNC["NET_ResolveHostname", SDL_POINTER[NET_Address], [ctypes.c_char_p], SDL_NET_BINARY]
NET_WaitUntilResolved: abc.Callable[..., typing.Any] = SDL_FUNC["NET_WaitUntilResolved", ctypes.c_int, [SDL_POINTER[NET_Address], ctypes.c_int32], SDL_NET_BINARY]
NET_GetAddressStatus: abc.Callable[..., typing.Any] = SDL_FUNC["NET_GetAddressStatus", ctypes.c_int, [SDL_POINTER[NET_Address]], SDL_NET_BINARY]
NET_GetAddressString: abc.Callable[..., typing.Any] = SDL_FUNC["NET_GetAddressString", ctypes.c_char_p, [SDL_POINTER[NET_Address]], SDL_NET_BINARY]
NET_RefAddress: abc.Callable[..., typing.Any] = SDL_FUNC["NET_RefAddress", SDL_POINTER[NET_Address], [SDL_POINTER[NET_Address]], SDL_NET_BINARY]
NET_UnrefAddress: abc.Callable[..., typing.Any] = SDL_FUNC["NET_UnrefAddress", None, [SDL_POINTER[NET_Address]], SDL_NET_BINARY]
NET_SimulateAddressResolutionLoss: abc.Callable[..., typing.Any] = SDL_FUNC["NET_SimulateAddressResolutionLoss", None, [ctypes.c_int], SDL_NET_BINARY]
NET_CompareAddresses: abc.Callable[..., typing.Any] = SDL_FUNC["NET_CompareAddresses", ctypes.c_int, [SDL_POINTER[NET_Address], SDL_POINTER[NET_Address]], SDL_NET_BINARY]
NET_GetLocalAddresses: abc.Callable[..., typing.Any] = SDL_FUNC["NET_GetLocalAddresses", SDL_POINTER[SDL_POINTER[NET_Address]], [SDL_POINTER[ctypes.c_int]], SDL_NET_BINARY]
NET_FreeLocalAddresses: abc.Callable[..., typing.Any] = SDL_FUNC["NET_FreeLocalAddresses", None, [SDL_POINTER[SDL_POINTER[NET_Address]]], SDL_NET_BINARY]

class NET_StreamSocket(ctypes.c_void_p):
    ...

NET_CreateClient: abc.Callable[..., typing.Any] = SDL_FUNC["NET_CreateClient", SDL_POINTER[NET_StreamSocket], [SDL_POINTER[NET_Address], ctypes.c_uint16], SDL_NET_BINARY]
NET_WaitUntilConnected: abc.Callable[..., typing.Any] = SDL_FUNC["NET_WaitUntilConnected", ctypes.c_int, [SDL_POINTER[NET_StreamSocket], ctypes.c_int32], SDL_NET_BINARY]

class NET_Server(ctypes.c_void_p):
    ...

NET_CreateServer: abc.Callable[..., typing.Any] = SDL_FUNC["NET_CreateServer", SDL_POINTER[NET_Server], [SDL_POINTER[NET_Address], ctypes.c_uint16], SDL_NET_BINARY]
NET_AcceptClient: abc.Callable[..., typing.Any] = SDL_FUNC["NET_AcceptClient", ctypes.c_bool, [SDL_POINTER[NET_Server], SDL_POINTER[SDL_POINTER[NET_StreamSocket]]], SDL_NET_BINARY]
NET_DestroyServer: abc.Callable[..., typing.Any] = SDL_FUNC["NET_DestroyServer", None, [SDL_POINTER[NET_Server]], SDL_NET_BINARY]
NET_GetStreamSocketAddress: abc.Callable[..., typing.Any] = SDL_FUNC["NET_GetStreamSocketAddress", SDL_POINTER[NET_Address], [SDL_POINTER[NET_StreamSocket]], SDL_NET_BINARY]
NET_GetConnectionStatus: abc.Callable[..., typing.Any] = SDL_FUNC["NET_GetConnectionStatus", ctypes.c_int, [SDL_POINTER[NET_StreamSocket]], SDL_NET_BINARY]
NET_WriteToStreamSocket: abc.Callable[..., typing.Any] = SDL_FUNC["NET_WriteToStreamSocket", ctypes.c_bool, [SDL_POINTER[NET_StreamSocket], ctypes.c_void_p, ctypes.c_int], SDL_NET_BINARY]
NET_GetStreamSocketPendingWrites: abc.Callable[..., typing.Any] = SDL_FUNC["NET_GetStreamSocketPendingWrites", ctypes.c_int, [SDL_POINTER[NET_StreamSocket]], SDL_NET_BINARY]
NET_WaitUntilStreamSocketDrained: abc.Callable[..., typing.Any] = SDL_FUNC["NET_WaitUntilStreamSocketDrained", ctypes.c_int, [SDL_POINTER[NET_StreamSocket], ctypes.c_int32], SDL_NET_BINARY]
NET_ReadFromStreamSocket: abc.Callable[..., typing.Any] = SDL_FUNC["NET_ReadFromStreamSocket", ctypes.c_int, [SDL_POINTER[NET_StreamSocket], ctypes.c_void_p, ctypes.c_int], SDL_NET_BINARY]
NET_SimulateStreamPacketLoss: abc.Callable[..., typing.Any] = SDL_FUNC["NET_SimulateStreamPacketLoss", None, [SDL_POINTER[NET_StreamSocket], ctypes.c_int], SDL_NET_BINARY]
NET_DestroyStreamSocket: abc.Callable[..., typing.Any] = SDL_FUNC["NET_DestroyStreamSocket", None, [SDL_POINTER[NET_StreamSocket]], SDL_NET_BINARY]

class NET_DatagramSocket(ctypes.c_void_p):
    ...

class NET_Datagram(ctypes.Structure):
    _fields_ = [
        ("addr", SDL_POINTER[NET_Address]),
        ("port", ctypes.c_uint16),
        ("buf", SDL_POINTER[ctypes.c_uint8]),
        ("buflen", ctypes.c_int)
    ]

NET_CreateDatagramSocket: abc.Callable[..., typing.Any] = SDL_FUNC["NET_CreateDatagramSocket", SDL_POINTER[NET_DatagramSocket], [SDL_POINTER[NET_Address], ctypes.c_uint16], SDL_NET_BINARY]
NET_SendDatagram: abc.Callable[..., typing.Any] = SDL_FUNC["NET_SendDatagram", ctypes.c_bool, [SDL_POINTER[NET_DatagramSocket], SDL_POINTER[NET_Address], ctypes.c_uint16, ctypes.c_void_p, ctypes.c_int], SDL_NET_BINARY]
NET_ReceiveDatagram: abc.Callable[..., typing.Any] = SDL_FUNC["NET_ReceiveDatagram", ctypes.c_bool, [SDL_POINTER[NET_DatagramSocket], SDL_POINTER[SDL_POINTER[NET_Datagram]]], SDL_NET_BINARY]
NET_DestroyDatagram: abc.Callable[..., typing.Any] = SDL_FUNC["NET_DestroyDatagram", None, [SDL_POINTER[NET_Datagram]], SDL_NET_BINARY]
NET_SimulateDatagramPacketLoss: abc.Callable[..., typing.Any] = SDL_FUNC["NET_SimulateDatagramPacketLoss", None, [SDL_POINTER[NET_DatagramSocket], ctypes.c_int], SDL_NET_BINARY]
NET_DestroyDatagramSocket: abc.Callable[..., typing.Any] = SDL_FUNC["NET_DestroyDatagramSocket", None, [SDL_POINTER[NET_DatagramSocket]], SDL_NET_BINARY]
NET_WaitUntilInputAvailable: abc.Callable[..., typing.Any] = SDL_FUNC["NET_WaitUntilInputAvailable", ctypes.c_int, [SDL_POINTER[ctypes.c_void_p], ctypes.c_int, ctypes.c_int32], SDL_NET_BINARY]