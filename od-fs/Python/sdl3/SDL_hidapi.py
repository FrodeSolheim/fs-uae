import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, SDL_FUNC, SDL_TYPE, SDL_BINARY

class SDL_hid_device(ctypes.c_void_p):
    ...

SDL_hid_bus_type: typing.TypeAlias = SDL_TYPE["SDL_hid_bus_type", SDL_ENUM]

SDL_HID_API_BUS_UNKNOWN, SDL_HID_API_BUS_USB, SDL_HID_API_BUS_BLUETOOTH, \
    SDL_HID_API_BUS_I2C, SDL_HID_API_BUS_SPI = range(5)

class SDL_hid_device_info(ctypes.Structure):
    _fields_ = [
        ("path", ctypes.c_char_p),
        ("vendor_id", ctypes.c_ushort),
        ("product_id", ctypes.c_ushort),
        ("serial_number", ctypes.c_wchar_p),
        ("release_number", ctypes.c_ushort),
        ("manufacturer_string", ctypes.c_wchar_p),
        ("product_string", ctypes.c_wchar_p),
        ("usage_page", ctypes.c_ushort),
        ("usage", ctypes.c_ushort),
        ("interface_number", ctypes.c_int),
        ("interface_class", ctypes.c_int),
        ("interface_subclass", ctypes.c_int),
        ("interface_protocol", ctypes.c_int),
        ("bus_type", SDL_hid_bus_type),
        ("next", SDL_POINTER[ctypes.c_void_p])
    ]

SDL_hid_init: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_init", ctypes.c_int, [], SDL_BINARY]
SDL_hid_exit: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_exit", ctypes.c_int, [], SDL_BINARY]
SDL_hid_device_change_count: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_device_change_count", ctypes.c_uint32, [], SDL_BINARY]
SDL_hid_enumerate: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_enumerate", SDL_POINTER[SDL_hid_device_info], [ctypes.c_ushort, ctypes.c_ushort], SDL_BINARY]
SDL_hid_free_enumeration: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_free_enumeration", None, [SDL_POINTER[SDL_hid_device_info]], SDL_BINARY]
SDL_hid_open: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_open", SDL_POINTER[SDL_hid_device], [ctypes.c_ushort, ctypes.c_ushort, ctypes.c_wchar_p], SDL_BINARY]
SDL_hid_open_path: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_open_path", SDL_POINTER[SDL_hid_device], [ctypes.c_char_p], SDL_BINARY]
SDL_hid_write: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_write", ctypes.c_int, [SDL_POINTER[SDL_hid_device], SDL_POINTER[ctypes.c_ubyte], ctypes.c_size_t], SDL_BINARY]
SDL_hid_read_timeout: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_read_timeout", ctypes.c_int, [SDL_POINTER[SDL_hid_device], SDL_POINTER[ctypes.c_ubyte], ctypes.c_size_t, ctypes.c_int], SDL_BINARY]
SDL_hid_read: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_read", ctypes.c_int, [SDL_POINTER[SDL_hid_device], SDL_POINTER[ctypes.c_ubyte], ctypes.c_size_t], SDL_BINARY]
SDL_hid_set_nonblocking: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_set_nonblocking", ctypes.c_int, [SDL_POINTER[SDL_hid_device], ctypes.c_int], SDL_BINARY]
SDL_hid_send_feature_report: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_send_feature_report", ctypes.c_int, [SDL_POINTER[SDL_hid_device], SDL_POINTER[ctypes.c_ubyte], ctypes.c_size_t], SDL_BINARY]
SDL_hid_get_feature_report: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_get_feature_report", ctypes.c_int, [SDL_POINTER[SDL_hid_device], SDL_POINTER[ctypes.c_ubyte], ctypes.c_size_t], SDL_BINARY]
SDL_hid_get_input_report: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_get_input_report", ctypes.c_int, [SDL_POINTER[SDL_hid_device], SDL_POINTER[ctypes.c_ubyte], ctypes.c_size_t], SDL_BINARY]
SDL_hid_close: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_close", ctypes.c_int, [SDL_POINTER[SDL_hid_device]], SDL_BINARY]
SDL_hid_get_manufacturer_string: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_get_manufacturer_string", ctypes.c_int, [SDL_POINTER[SDL_hid_device], ctypes.c_wchar_p, ctypes.c_size_t], SDL_BINARY]
SDL_hid_get_product_string: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_get_product_string", ctypes.c_int, [SDL_POINTER[SDL_hid_device], ctypes.c_wchar_p, ctypes.c_size_t], SDL_BINARY]
SDL_hid_get_serial_number_string: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_get_serial_number_string", ctypes.c_int, [SDL_POINTER[SDL_hid_device], ctypes.c_wchar_p, ctypes.c_size_t], SDL_BINARY]
SDL_hid_get_indexed_string: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_get_indexed_string", ctypes.c_int, [SDL_POINTER[SDL_hid_device], ctypes.c_int, ctypes.c_wchar_p, ctypes.c_size_t], SDL_BINARY]
SDL_hid_get_device_info: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_get_device_info", SDL_POINTER[SDL_hid_device_info], [SDL_POINTER[SDL_hid_device]], SDL_BINARY]
SDL_hid_get_report_descriptor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_get_report_descriptor", ctypes.c_int, [SDL_POINTER[SDL_hid_device], SDL_POINTER[ctypes.c_ubyte], ctypes.c_size_t], SDL_BINARY]
SDL_hid_ble_scan: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_hid_ble_scan", None, [ctypes.c_bool], SDL_BINARY]