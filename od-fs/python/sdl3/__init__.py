"""A pure Python wrapper for SDL3."""

__version__ = "0.9.8b8"

import os, sys, ctypes, platform, collections.abc as abc, typing, types

SDL_BINARY, SDL_IMAGE_BINARY, SDL_MIXER_BINARY, SDL_TTF_BINARY, SDL_RTF_BINARY, SDL_NET_BINARY, SDL_SHADERCROSS_BINARY, \
    *SDL_MODULES = ["SDL3", "SDL3_image", "SDL3_mixer", "SDL3_ttf", "SDL3_rtf", "SDL3_net", "SDL3_shadercross"] * 2

def SDL_FORMAT_ARCH(arch: str) -> str:
    """Format the architecture string."""
    if arch.lower() in ["x64", "x86_64", "amd64"]: return "AMD64"
    if arch.lower() in ["aarch64", "arm64"]: return "ARM64"
    assert False, "Unknown architecture."

SDL_SYSTEM, SDL_ARCH = platform.system(), SDL_FORMAT_ARCH(platform.machine())
SDL_BINARY_EXTENSIONS, SDL_BINARY_PATTERNS = {"Windows": ["dll"], "Darwin": ["dylib"], "Linux": ["so"]}, \
    {"Windows": ["{}.dll"], "Darwin": ["lib{}.dylib", "{0}.framework/{0}", "{0}.framework/Versions/Current/{0}"], "Linux": ["lib{}.so"]}

def SDL_PLATFORM_SPECIFIC(system: list[str] | None = None, arch: list[str] | None = None) -> bool:
    """Check if the current platform is inside the given platforms."""
    if int(os.environ.get("SDL_PLATFORM_AGNOSTIC", "0")) > 0: return True
    return (not system or SDL_SYSTEM in system) and (not arch or SDL_ARCH in arch)

__release__: bool = not int(os.environ.get("SDL_DEBUG", "0")) > 0
__frozen__: bool = getattr(sys, "frozen", False) or getattr(sys, "_MEIPASS", None) is not None

__doc_file__: str = os.path.join(os.path.dirname(__file__), "__doc__.py")
__doc_generator__: int = int(os.environ.get("SDL_DOC_GENERATOR", "0" if __frozen__ else "1")) > 0

__initialized__: bool = __name__.split(".")[0] in sys.modules if "." in __name__ else False
__module__: types.ModuleType = sys.modules[__name__.split(".")[0] if "." in __name__ else __name__]

class SDL_LOGGER:
    Info, Warning, Error = range(3)
    __level__: int = int(os.environ.get("SDL_LOG_LEVEL", "0"))
    __colors__: dict[int, str] = {Info: "\33[32m", Warning: "\33[35m", Error: "\33[31m"}
    __names__: dict[int, str] = {Info: "Info", Warning: "Warning", Error: "Error"}

    @classmethod
    def Log(cls, level: int, message: str | None = None, end: str | None = "\n") -> None:
        """Log a message at a specific level."""
        assert level in [cls.Error, cls.Warning, cls.Info], "Unknown log level."
        if cls.__level__ is None or level < cls.__level__: return

        if message is not None:
            sys.stdout.write(f"{cls.__colors__[level]}{cls.__names__[level]}: {message}\33[0m")

        if end is not None:
            sys.stdout.write(end)

        sys.stdout.flush()

def SDL_ARRAY(*values, type: typing.Any | None = None) -> tuple[typing.Any, int]:
    """Create a ctypes array from the given arguments."""
    return ((type or values[0].__class__) * len(values))(*values), len(values)

def SDL_DEREFERENCE(value: typing.Any) -> typing.Any:
    """Dereference a ctypes pointer or object."""
    if isinstance(value, ctypes._Pointer): return value.contents
    elif hasattr(value, "_obj"): return value._obj
    else: return value

def SDL_CACHE_FUNC(func: abc.Callable[..., typing.Any]) -> abc.Callable[..., typing.Any]:
    """Simple function cache decorator."""
    cache = {}

    def __inner__(*args: typing.Any, **kwargs: typing.Any) -> typing.Any:
        _hash = hash((args, tuple(frozenset(sorted(kwargs.items())))))
        if _hash not in cache: cache.update({_hash: func(*args, **kwargs)})
        return cache.get(_hash, None)

    return __inner__

def SDL_GET_BINARY(name: str) -> ctypes.CDLL | None:
    """Get an SDL3 binary by its name."""
    return __module__.binaryMap.get(name, None)

def SDL_NOT_IMPLEMENTED(name: str) -> abc.Callable[..., None]:
    return lambda *args, **kwargs: \
        SDL_LOGGER.Log(SDL_LOGGER.Error, f"Invoked an unimplemented function: '{name}'.")

class SDL_FUNC:
    def __class_getitem__(cls, key: tuple[str, type, list[type], str]) -> typing.Any:
        """Create a new ctypes function definition."""

        return None

class SDL_POINTER:
    def __class_getitem__(cls, key: type) -> type:
        """Create a ctypes pointer type from a ctypes type."""

        if not (__frozen__ or __release__):
            assert not isinstance(key, tuple), "Expected a single argument, got a tuple."
            assert isinstance(key, type), "Expected a type as the first argument."

        return ctypes.POINTER(key)

class SDL_CAST:
    def __class_getitem__(cls, key: tuple[typing.Any, type]) -> typing.Any:
        """Cast a ctypes pointer to an another type."""

        if not (__frozen__ or __release__):
            assert isinstance(key, tuple), "Expected a tuple, got a single argument."
            assert len(key) == 2, "Expected a tuple with length 2."
            assert isinstance(key[0], ctypes._Pointer), "Expected a pointer as the first argument."
            assert isinstance(key[1], type), "Expected a type as the second argument."

        return ctypes.cast(key[0], key[1])

class SDL_TYPE:
    def __class_getitem__(cls, key: tuple[str, type]) -> type:
        """Create a new type from a ctypes type."""

        if not (__frozen__ or __release__):
            assert isinstance(key, tuple), "Expected a tuple, got a single argument."
            assert len(key) == 2, "Expected a tuple with length 2."
            assert isinstance(key[0], str), "Expected a string as the first argument."
            assert isinstance(key[1], type), "Expected a type as the second argument."

        if hasattr(key[1], "contents"):
            return type(key[0], (ctypes._Pointer, ), {"_type_": key[1]._type_, "contents": key[1].contents})

        elif hasattr(key[1], "_fields_"):
            return type(key[0], (ctypes.Structure, ), {"_fields_": key[1]._fields_})

        else:
            return type(key[0], (ctypes._SimpleCData, ), {"_type_": key[1]._type_})

class SDL_FUNC_TYPE:
    def __class_getitem__(cls, key: tuple[str, type, list[type]]) -> type:
        """Create a new ctypes function type."""

        if not (__frozen__ or __release__):
            assert isinstance(key, tuple), "Expected a tuple, got a single argument."
            assert len(key) == 3, "Expected a tuple with length 3."
            assert isinstance(key[0], str), "Expected a string as the first argument."
            assert isinstance(key[1], type) or key[1] is None, "Expected a type as the second argument."
            assert isinstance(key[2], list), "Expected a list as the third argument."

        _, ctypes._c_functype_cache = ctypes._c_functype_cache, {} # type: ignore
        value = ctypes.CFUNCTYPE(key[1], *key[2])
        value.__name__, ctypes._c_functype_cache = key[0], _ # type: ignore
        return value

SDL_ENUM: typing.TypeAlias = SDL_TYPE["SDL_ENUM", ctypes.c_int]
SDL_VA_LIST: typing.TypeAlias = SDL_TYPE["SDL_VA_LIST", ctypes.c_char_p]

def SDL_GET_MODULE_BY_NAME(name: str) -> str | None:
    """Get the module of an SDL3 function/structure by its name."""

    for prefix, module in sorted({"SDL": "SDL3", "IMG": "SDL3_image", "Mix": "SDL3_mixer", "TTF": "SDL3_ttf", "RTF": "SDL3_rtf", "NET": "SDL3_net", "SDL_ShaderCross": "SDL3_shadercross"}.items(), key = lambda x: -len(x[0])):
        if name.startswith(prefix): return module

def SDL_PYTHONIZE_TYPE(type: str, name: str | None = None, globals: dict[str, typing.Any] = {}) -> str:
    """Pythonize C/C++ types."""
    array = (_ if str.isnumeric(_ := name.split("[")[1].split("]")[0]) else eval(_, globals)) if name is not None and name.count("[") == 1 and name.index("]") - name.index("[") != 1 else None
    type, count = type.replace("*", ""), type.count("*") + (0 if name is None or array is not None else name.count("["))

    if " " in type:
        type = " ".join([i for i in type.split(" ") if i and i not in ["const", "struct", "union", "enum", "SDLCALL"]])

    if type in (sdlTypes := {
        "Sint8": "ctypes.c_int8", "Uint8": "ctypes.c_uint8", "Sint16": "ctypes.c_int16", "Uint16": "ctypes.c_uint16",
        "Sint32": "ctypes.c_int32", "Uint32": "ctypes.c_uint32", "Sint64": "ctypes.c_int64", "Uint64": "ctypes.c_uint64"
    }): type = sdlTypes[type]

    if count and type in ["void", "char", "wchar_t"]:
        type, count = f"ctypes.c_{type.replace('_t', '')}_p", count - 1

    if type in (cTypes := {
        "int": "ctypes.c_int", "bool": "ctypes.c_bool", "float": "ctypes.c_float", "double": "ctypes.c_double",
        "char": "ctypes.c_char", "short": "ctypes.c_short", "long long": "ctypes.c_longlong", "long": "ctypes.c_long",
        "size_t": "ctypes.c_size_t", "ssize_t": "ctypes.c_ssize_t", "intptr_t": "ctypes.c_intptr_t", "uintptr_t": "ctypes.c_uintptr_t",
        "int8_t": "ctypes.c_int8", "uint8_t": "ctypes.c_uint8", "int16_t": "ctypes.c_int16", "uint16_t": "ctypes.c_uint16",
        "int32_t": "ctypes.c_int32", "uint32_t": "ctypes.c_uint32", "int64_t": "ctypes.c_int64", "uint64_t": "ctypes.c_uint64",
        "unsigned short": "ctypes.c_ushort", "unsigned long long": "ctypes.c_ulonglong", "unsigned long": "ctypes.c_ulong",
        "unsigned char": "ctypes.c_ubyte", "unsigned int": "ctypes.c_uint", "wchar_t": "ctypes.c_wchar", "va_list": "SDL_VA_LIST"
    }): type = cTypes[type]

    if not count and type in ["void"]: type = "None"
    if count and type.startswith("ctypes."): type = type.split(".")[1]
    return f"{'LP_' * count}{type}{'_Array_%s' % array if array else ''}"

if not __initialized__ and int(os.environ.get("SDL_CTYPES_ALIAS_FIX", "0" if __frozen__ or __release__ else "1")) > 0:
    for i in dir(ctypes):
        if i.startswith("c_") and getattr(ctypes, i).__name__ != i and hasattr(getattr(ctypes, i), "_type_"):
            setattr(ctypes, i, SDL_TYPE[i, getattr(ctypes, i)])

from .SDL import * # type: ignore

SDL_VERSIONNUM_STRING: abc.Callable[[int], str] = lambda num: str(None).lower() if not num else \
    f"{SDL_VERSIONNUM_MAJOR(num)}.{SDL_VERSIONNUM_MINOR(num)}.{SDL_VERSIONNUM_MICRO(num)}"
