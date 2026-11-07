import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC, SDL_TYPE, SDL_ENUM, SDL_SHADERCROSS_BINARY
from .SDL_gpu import SDL_GPUDevice, SDL_GPUShaderFormat, SDL_GPUShader, SDL_GPUComputePipeline
from .SDL_properties import SDL_PropertiesID

SDL_SHADERCROSS_MAJOR_VERSION, SDL_SHADERCROSS_MINOR_VERSION, SDL_SHADERCROSS_MICRO_VERSION = 3, 0, 0

SDL_ShaderCross_IOVarType: typing.TypeAlias = SDL_TYPE["SDL_ShaderCross_IOVarType", SDL_ENUM]

SDL_SHADERCROSS_IOVAR_TYPE_UNKNOWN, SDL_SHADERCROSS_IOVAR_TYPE_INT8, SDL_SHADERCROSS_IOVAR_TYPE_UINT8, SDL_SHADERCROSS_IOVAR_TYPE_INT16, SDL_SHADERCROSS_IOVAR_TYPE_UINT16, \
    SDL_SHADERCROSS_IOVAR_TYPE_INT32, SDL_SHADERCROSS_IOVAR_TYPE_UINT32, SDL_SHADERCROSS_IOVAR_TYPE_INT64, SDL_SHADERCROSS_IOVAR_TYPE_UINT64, \
        SDL_SHADERCROSS_IOVAR_TYPE_FLOAT16, SDL_SHADERCROSS_IOVAR_TYPE_FLOAT32, SDL_SHADERCROSS_IOVAR_TYPE_FLOAT64 = range(12)

SDL_ShaderCross_ShaderStage: typing.TypeAlias = SDL_TYPE["SDL_ShaderCross_ShaderStage", SDL_ENUM]

SDL_SHADERCROSS_SHADERSTAGE_VERTEX, SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT, SDL_SHADERCROSS_SHADERSTAGE_COMPUTE = range(3)

class SDL_ShaderCross_IOVarMetadata(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char_p),
        ("location", ctypes.c_uint32),
        ("vector_type", SDL_ShaderCross_IOVarType),
        ("vector_size", ctypes.c_uint32)
    ]

class SDL_ShaderCross_GraphicsShaderMetadata(ctypes.Structure):
    _fields_ = [
        ("num_samplers", ctypes.c_uint32),
        ("num_storage_textures", ctypes.c_uint32),
        ("num_storage_buffers", ctypes.c_uint32),
        ("num_uniform_buffers", ctypes.c_uint32),
        ("num_inputs", ctypes.c_uint32),
        ("inputs", SDL_POINTER[SDL_ShaderCross_IOVarMetadata]),
        ("num_outputs", ctypes.c_uint32),
        ("outputs", SDL_POINTER[SDL_ShaderCross_IOVarMetadata])
    ]

class SDL_ShaderCross_ComputePipelineMetadata(ctypes.Structure):
    _fields_ = [
        ("num_samplers", ctypes.c_uint32),
        ("num_readonly_storage_textures", ctypes.c_uint32),
        ("num_readonly_storage_buffers", ctypes.c_uint32),
        ("num_readwrite_storage_textures", ctypes.c_uint32),
        ("num_readwrite_storage_buffers", ctypes.c_uint32),
        ("num_uniform_buffers", ctypes.c_uint32),
        ("threadcount_x", ctypes.c_uint32),
        ("threadcount_y", ctypes.c_uint32),
        ("threadcount_z", ctypes.c_uint32)
    ]

class SDL_ShaderCross_SPIRV_Info(ctypes.Structure):
    _fields_ = [
        ("bytecode", SDL_POINTER[ctypes.c_uint8]),
        ("bytecode_size", ctypes.c_size_t),
        ("entrypoint", ctypes.c_char_p),
        ("shader_stage", SDL_ShaderCross_ShaderStage),
        ("enable_debug", ctypes.c_bool),
        ("name", ctypes.c_char_p),
        ("props", SDL_PropertiesID)
    ]

SDL_SHADERCROSS_PROP_SPIRV_PSSL_COMPATIBILITY: bytes = "SDL.shadercross.spirv.pssl.compatibility".encode()
SDL_SHADERCROSS_PROP_SPIRV_MSL_VERSION: bytes = "SDL.shadercross.spirv.msl.version".encode()

class SDL_ShaderCross_HLSL_Define(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char_p),
        ("value", ctypes.c_char_p)
    ]

class SDL_ShaderCross_HLSL_Info(ctypes.Structure):
    _fields_ = [
        ("source", ctypes.c_char_p),
        ("entrypoint", ctypes.c_char_p),
        ("include_dir", ctypes.c_char_p),
        ("defines", SDL_POINTER[SDL_ShaderCross_HLSL_Define]),
        ("shader_stage", SDL_ShaderCross_ShaderStage),
        ("enable_debug", ctypes.c_bool),
        ("name", ctypes.c_char_p),
        ("props", SDL_PropertiesID)
    ]

SDL_ShaderCross_Init: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_Init", ctypes.c_bool, [], SDL_SHADERCROSS_BINARY]
SDL_ShaderCross_Quit: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_Quit", None, [], SDL_SHADERCROSS_BINARY]

SDL_ShaderCross_GetSPIRVShaderFormats: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_GetSPIRVShaderFormats", SDL_GPUShaderFormat, [], SDL_SHADERCROSS_BINARY]

SDL_ShaderCross_TranspileMSLFromSPIRV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_TranspileMSLFromSPIRV", ctypes.c_void_p, [SDL_POINTER[SDL_ShaderCross_SPIRV_Info]], SDL_SHADERCROSS_BINARY]
SDL_ShaderCross_TranspileHLSLFromSPIRV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_TranspileHLSLFromSPIRV", ctypes.c_void_p, [SDL_POINTER[SDL_ShaderCross_SPIRV_Info]], SDL_SHADERCROSS_BINARY]
SDL_ShaderCross_CompileDXBCFromSPIRV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_CompileDXBCFromSPIRV", ctypes.c_void_p, [SDL_POINTER[SDL_ShaderCross_SPIRV_Info], SDL_POINTER[ctypes.c_size_t]], SDL_SHADERCROSS_BINARY]
SDL_ShaderCross_CompileDXILFromSPIRV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_CompileDXILFromSPIRV", ctypes.c_void_p, [SDL_POINTER[SDL_ShaderCross_SPIRV_Info], SDL_POINTER[ctypes.c_size_t]], SDL_SHADERCROSS_BINARY]

SDL_ShaderCross_CompileGraphicsShaderFromSPIRV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_CompileGraphicsShaderFromSPIRV", SDL_POINTER[SDL_GPUShader], [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_ShaderCross_SPIRV_Info], SDL_POINTER[SDL_ShaderCross_GraphicsShaderMetadata], SDL_PropertiesID], SDL_SHADERCROSS_BINARY]
SDL_ShaderCross_CompileComputePipelineFromSPIRV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_CompileComputePipelineFromSPIRV", SDL_POINTER[SDL_GPUComputePipeline], [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_ShaderCross_SPIRV_Info], SDL_POINTER[SDL_ShaderCross_ComputePipelineMetadata], SDL_PropertiesID], SDL_SHADERCROSS_BINARY]

SDL_ShaderCross_ReflectGraphicsSPIRV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_ReflectGraphicsSPIRV", SDL_POINTER[SDL_ShaderCross_GraphicsShaderMetadata], [SDL_POINTER[ctypes.c_uint8], ctypes.c_size_t, SDL_PropertiesID], SDL_SHADERCROSS_BINARY]
SDL_ShaderCross_ReflectComputeSPIRV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_ReflectComputeSPIRV", SDL_POINTER[SDL_ShaderCross_ComputePipelineMetadata], [SDL_POINTER[ctypes.c_uint8], ctypes.c_size_t, SDL_PropertiesID], SDL_SHADERCROSS_BINARY]

SDL_ShaderCross_GetHLSLShaderFormats: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_GetHLSLShaderFormats", SDL_GPUShaderFormat, [], SDL_SHADERCROSS_BINARY]

SDL_ShaderCross_CompileDXBCFromHLSL: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_CompileDXBCFromHLSL", ctypes.c_void_p, [SDL_POINTER[SDL_ShaderCross_HLSL_Info], SDL_POINTER[ctypes.c_size_t]], SDL_SHADERCROSS_BINARY]
SDL_ShaderCross_CompileDXILFromHLSL: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_CompileDXILFromHLSL", ctypes.c_void_p, [SDL_POINTER[SDL_ShaderCross_HLSL_Info], SDL_POINTER[ctypes.c_size_t]], SDL_SHADERCROSS_BINARY]
SDL_ShaderCross_CompileSPIRVFromHLSL: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShaderCross_CompileSPIRVFromHLSL", ctypes.c_void_p, [SDL_POINTER[SDL_ShaderCross_HLSL_Info], SDL_POINTER[ctypes.c_size_t]], SDL_SHADERCROSS_BINARY]