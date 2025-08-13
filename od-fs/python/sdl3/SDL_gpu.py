import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_properties import SDL_PropertiesID
from .SDL_surface import SDL_FlipMode
from .SDL_video import SDL_Window
from .SDL_pixels import SDL_FColor
from .SDL_rect import SDL_Rect

class SDL_GPUDevice(ctypes.c_void_p):
    ...

class SDL_GPUBuffer(ctypes.c_void_p):
    ...

class SDL_GPUTransferBuffer(ctypes.c_void_p):
    ...

class SDL_GPUTexture(ctypes.c_void_p):
    ...

class SDL_GPUSampler(ctypes.c_void_p):
    ...

class SDL_GPUShader(ctypes.c_void_p):
    ...

class SDL_GPUComputePipeline(ctypes.c_void_p):
    ...

class SDL_GPUGraphicsPipeline(ctypes.c_void_p):
    ...

class SDL_GPUCommandBuffer(ctypes.c_void_p):
    ...

class SDL_GPURenderPass(ctypes.c_void_p):
    ...

class SDL_GPUComputePass(ctypes.c_void_p):
    ...

class SDL_GPUCopyPass(ctypes.c_void_p):
    ...

class SDL_GPUFence(ctypes.c_void_p):
    ...

SDL_GPUPrimitiveType: typing.TypeAlias = SDL_TYPE["SDL_GPUPrimitiveType", SDL_ENUM]

SDL_GPU_PRIMITIVETYPE_TRIANGLELIST, SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP, SDL_GPU_PRIMITIVETYPE_LINELIST, \
    SDL_GPU_PRIMITIVETYPE_LINESTRIP, SDL_GPU_PRIMITIVETYPE_POINTLIST = range(5)

SDL_GPULoadOp: typing.TypeAlias = SDL_TYPE["SDL_GPULoadOp", SDL_ENUM]

SDL_GPU_LOADOP_LOAD, SDL_GPU_LOADOP_CLEAR, SDL_GPU_LOADOP_DONT_CARE = range(3)

SDL_GPUStoreOp: typing.TypeAlias = SDL_TYPE["SDL_GPUStoreOp", SDL_ENUM]

SDL_GPU_STOREOP_STORE, SDL_GPU_STOREOP_DONT_CARE, \
    SDL_GPU_STOREOP_RESOLVE, SDL_GPU_STOREOP_RESOLVE_AND_STORE = range(4)

SDL_GPUIndexElementSize: typing.TypeAlias = SDL_TYPE["SDL_GPUIndexElementSize", SDL_ENUM]

SDL_GPU_INDEXELEMENTSIZE_16BIT, SDL_GPU_INDEXELEMENTSIZE_32BIT = range(2)

SDL_GPUTextureFormat: typing.TypeAlias = SDL_TYPE["SDL_GPUTextureFormat", SDL_ENUM]

SDL_GPU_TEXTUREFORMAT_INVALID, SDL_GPU_TEXTUREFORMAT_A8_UNORM, SDL_GPU_TEXTUREFORMAT_R8_UNORM, SDL_GPU_TEXTUREFORMAT_R8G8_UNORM, SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM, \
    SDL_GPU_TEXTUREFORMAT_R16_UNORM, SDL_GPU_TEXTUREFORMAT_R16G16_UNORM, SDL_GPU_TEXTUREFORMAT_R16G16B16A16_UNORM, SDL_GPU_TEXTUREFORMAT_R10G10B10A2_UNORM, \
        SDL_GPU_TEXTUREFORMAT_B5G6R5_UNORM, SDL_GPU_TEXTUREFORMAT_B5G5R5A1_UNORM, SDL_GPU_TEXTUREFORMAT_B4G4R4A4_UNORM, SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM, \
    \
    SDL_GPU_TEXTUREFORMAT_BC1_RGBA_UNORM, SDL_GPU_TEXTUREFORMAT_BC2_RGBA_UNORM, SDL_GPU_TEXTUREFORMAT_BC3_RGBA_UNORM, SDL_GPU_TEXTUREFORMAT_BC4_R_UNORM, \
        SDL_GPU_TEXTUREFORMAT_BC5_RG_UNORM, SDL_GPU_TEXTUREFORMAT_BC7_RGBA_UNORM, SDL_GPU_TEXTUREFORMAT_BC6H_RGB_FLOAT, SDL_GPU_TEXTUREFORMAT_BC6H_RGB_UFLOAT, \
    \
    SDL_GPU_TEXTUREFORMAT_R8_SNORM, SDL_GPU_TEXTUREFORMAT_R8G8_SNORM, SDL_GPU_TEXTUREFORMAT_R8G8B8A8_SNORM, \
        SDL_GPU_TEXTUREFORMAT_R16_SNORM, SDL_GPU_TEXTUREFORMAT_R16G16_SNORM, SDL_GPU_TEXTUREFORMAT_R16G16B16A16_SNORM, \
    \
    SDL_GPU_TEXTUREFORMAT_R16_FLOAT, SDL_GPU_TEXTUREFORMAT_R16G16_FLOAT, SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT, \
        SDL_GPU_TEXTUREFORMAT_R32_FLOAT, SDL_GPU_TEXTUREFORMAT_R32G32_FLOAT, SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT, \
    \
    SDL_GPU_TEXTUREFORMAT_R11G11B10_UFLOAT, \
    \
    SDL_GPU_TEXTUREFORMAT_R8_UINT, SDL_GPU_TEXTUREFORMAT_R8G8_UINT, SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UINT, \
        SDL_GPU_TEXTUREFORMAT_R16_UINT, SDL_GPU_TEXTUREFORMAT_R16G16_UINT, SDL_GPU_TEXTUREFORMAT_R16G16B16A16_UINT, \
            SDL_GPU_TEXTUREFORMAT_R32_UINT, SDL_GPU_TEXTUREFORMAT_R32G32_UINT, SDL_GPU_TEXTUREFORMAT_R32G32B32A32_UINT, \
    \
    SDL_GPU_TEXTUREFORMAT_R8_INT, SDL_GPU_TEXTUREFORMAT_R8G8_INT, SDL_GPU_TEXTUREFORMAT_R8G8B8A8_INT, \
        SDL_GPU_TEXTUREFORMAT_R16_INT, SDL_GPU_TEXTUREFORMAT_R16G16_INT, SDL_GPU_TEXTUREFORMAT_R16G16B16A16_INT, \
            SDL_GPU_TEXTUREFORMAT_R32_INT, SDL_GPU_TEXTUREFORMAT_R32G32_INT, SDL_GPU_TEXTUREFORMAT_R32G32B32A32_INT, \
    \
    SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM_SRGB, \
    \
    SDL_GPU_TEXTUREFORMAT_BC1_RGBA_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_BC2_RGBA_UNORM_SRGB, \
        SDL_GPU_TEXTUREFORMAT_BC3_RGBA_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_BC7_RGBA_UNORM_SRGB, \
    \
    SDL_GPU_TEXTUREFORMAT_D16_UNORM, SDL_GPU_TEXTUREFORMAT_D24_UNORM, SDL_GPU_TEXTUREFORMAT_D32_FLOAT, \
        SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT, SDL_GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT, \
    \
    SDL_GPU_TEXTUREFORMAT_ASTC_4x4_UNORM, SDL_GPU_TEXTUREFORMAT_ASTC_5x4_UNORM, SDL_GPU_TEXTUREFORMAT_ASTC_5x5_UNORM, SDL_GPU_TEXTUREFORMAT_ASTC_6x5_UNORM, \
        SDL_GPU_TEXTUREFORMAT_ASTC_6x6_UNORM, SDL_GPU_TEXTUREFORMAT_ASTC_8x5_UNORM, SDL_GPU_TEXTUREFORMAT_ASTC_8x6_UNORM, SDL_GPU_TEXTUREFORMAT_ASTC_8x8_UNORM, \
            SDL_GPU_TEXTUREFORMAT_ASTC_10x5_UNORM, SDL_GPU_TEXTUREFORMAT_ASTC_10x6_UNORM, SDL_GPU_TEXTUREFORMAT_ASTC_10x8_UNORM, SDL_GPU_TEXTUREFORMAT_ASTC_10x10_UNORM, \
                SDL_GPU_TEXTUREFORMAT_ASTC_12x10_UNORM, SDL_GPU_TEXTUREFORMAT_ASTC_12x12_UNORM, \
    \
    SDL_GPU_TEXTUREFORMAT_ASTC_4x4_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_ASTC_5x4_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_ASTC_5x5_UNORM_SRGB, \
        SDL_GPU_TEXTUREFORMAT_ASTC_6x5_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_ASTC_6x6_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_ASTC_8x5_UNORM_SRGB, \
            SDL_GPU_TEXTUREFORMAT_ASTC_8x6_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_ASTC_8x8_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_ASTC_10x5_UNORM_SRGB, \
                SDL_GPU_TEXTUREFORMAT_ASTC_10x6_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_ASTC_10x8_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_ASTC_10x10_UNORM_SRGB, \
                    SDL_GPU_TEXTUREFORMAT_ASTC_12x10_UNORM_SRGB, SDL_GPU_TEXTUREFORMAT_ASTC_12x12_UNORM_SRGB, \
    \
    SDL_GPU_TEXTUREFORMAT_ASTC_4x4_FLOAT, SDL_GPU_TEXTUREFORMAT_ASTC_5x4_FLOAT, SDL_GPU_TEXTUREFORMAT_ASTC_5x5_FLOAT, \
        SDL_GPU_TEXTUREFORMAT_ASTC_6x5_FLOAT, SDL_GPU_TEXTUREFORMAT_ASTC_6x6_FLOAT, SDL_GPU_TEXTUREFORMAT_ASTC_8x5_FLOAT, \
            SDL_GPU_TEXTUREFORMAT_ASTC_8x6_FLOAT, SDL_GPU_TEXTUREFORMAT_ASTC_8x8_FLOAT, SDL_GPU_TEXTUREFORMAT_ASTC_10x5_FLOAT, \
                SDL_GPU_TEXTUREFORMAT_ASTC_10x6_FLOAT, SDL_GPU_TEXTUREFORMAT_ASTC_10x8_FLOAT, SDL_GPU_TEXTUREFORMAT_ASTC_10x10_FLOAT, \
                    SDL_GPU_TEXTUREFORMAT_ASTC_12x10_FLOAT, SDL_GPU_TEXTUREFORMAT_ASTC_12x12_FLOAT = range(105)

SDL_GPUTextureUsageFlags: typing.TypeAlias = SDL_TYPE["SDL_GPUTextureUsageFlags", ctypes.c_uint32]

SDL_GPU_TEXTUREUSAGE_SAMPLER, SDL_GPU_TEXTUREUSAGE_COLOR_TARGET, SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET, \
     SDL_GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ, SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ, SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_WRITE, \
        SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_SIMULTANEOUS_READ_WRITE = 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6

SDL_GPUTextureType: typing.TypeAlias = SDL_TYPE["SDL_GPUTextureType", SDL_ENUM]

SDL_GPU_TEXTURETYPE_2D, SDL_GPU_TEXTURETYPE_2D_ARRAY, SDL_GPU_TEXTURETYPE_3D, \
    SDL_GPU_TEXTURETYPE_CUBE, SDL_GPU_TEXTURETYPE_CUBE_ARRAY = range(5)

SDL_GPUSampleCount: typing.TypeAlias = SDL_TYPE["SDL_GPUSampleCount", SDL_ENUM]

SDL_GPU_SAMPLECOUNT_1, SDL_GPU_SAMPLECOUNT_2, SDL_GPU_SAMPLECOUNT_4, SDL_GPU_SAMPLECOUNT_8 = range(4)

SDL_GPUCubeMapFace: typing.TypeAlias = SDL_TYPE["SDL_GPUCubeMapFace", SDL_ENUM]

SDL_GPU_CUBEMAPFACE_POSITIVEX, SDL_GPU_CUBEMAPFACE_NEGATIVEX, SDL_GPU_CUBEMAPFACE_POSITIVEY, SDL_GPU_CUBEMAPFACE_NEGATIVEY, \
    SDL_GPU_CUBEMAPFACE_POSITIVEZ, SDL_GPU_CUBEMAPFACE_NEGATIVEZ = range(6)

SDL_GPUBufferUsageFlags: typing.TypeAlias = SDL_TYPE["SDL_GPUBufferUsageFlags", ctypes.c_uint32]

SDL_GPU_BUFFERUSAGE_VERTEX, SDL_GPU_BUFFERUSAGE_INDEX, SDL_GPU_BUFFERUSAGE_INDIRECT, SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, \
    SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE = 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5

SDL_GPUTransferBufferUsage: typing.TypeAlias = SDL_TYPE["SDL_GPUTransferBufferUsage", SDL_ENUM]

SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD = range(2)

SDL_GPUShaderStage: typing.TypeAlias = SDL_TYPE["SDL_GPUShaderStage", SDL_ENUM]

SDL_GPU_SHADERSTAGE_VERTEX, SDL_GPU_SHADERSTAGE_FRAGMENT = range(2)

SDL_GPUShaderFormat: typing.TypeAlias = SDL_TYPE["SDL_GPUShaderFormat", ctypes.c_uint32]

SDL_GPU_SHADERFORMAT_INVALID, SDL_GPU_SHADERFORMAT_PRIVATE, SDL_GPU_SHADERFORMAT_SPIRV, SDL_GPU_SHADERFORMAT_DXBC, \
    SDL_GPU_SHADERFORMAT_DXIL, SDL_GPU_SHADERFORMAT_MSL, SDL_GPU_SHADERFORMAT_METALLIB = 0, 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5

SDL_GPUVertexElementFormat: typing.TypeAlias = SDL_TYPE["SDL_GPUVertexElementFormat", SDL_ENUM]

SDL_GPU_VERTEXELEMENTFORMAT_INVALID, SDL_GPU_VERTEXELEMENTFORMAT_INT, SDL_GPU_VERTEXELEMENTFORMAT_INT2, SDL_GPU_VERTEXELEMENTFORMAT_INT3, SDL_GPU_VERTEXELEMENTFORMAT_INT4, \
    SDL_GPU_VERTEXELEMENTFORMAT_UINT, SDL_GPU_VERTEXELEMENTFORMAT_UINT2, SDL_GPU_VERTEXELEMENTFORMAT_UINT3, SDL_GPU_VERTEXELEMENTFORMAT_UINT4, \
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, \
            SDL_GPU_VERTEXELEMENTFORMAT_BYTE2, SDL_GPU_VERTEXELEMENTFORMAT_BYTE4, SDL_GPU_VERTEXELEMENTFORMAT_UBYTE2, SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4, \
                SDL_GPU_VERTEXELEMENTFORMAT_BYTE2_NORM, SDL_GPU_VERTEXELEMENTFORMAT_BYTE4_NORM, SDL_GPU_VERTEXELEMENTFORMAT_UBYTE2_NORM, SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM, \
                    SDL_GPU_VERTEXELEMENTFORMAT_SHORT2, SDL_GPU_VERTEXELEMENTFORMAT_SHORT4, SDL_GPU_VERTEXELEMENTFORMAT_USHORT2, SDL_GPU_VERTEXELEMENTFORMAT_USHORT4, \
                        SDL_GPU_VERTEXELEMENTFORMAT_SHORT2_NORM, SDL_GPU_VERTEXELEMENTFORMAT_SHORT4_NORM, SDL_GPU_VERTEXELEMENTFORMAT_USHORT2_NORM, SDL_GPU_VERTEXELEMENTFORMAT_USHORT4_NORM, \
                            SDL_GPU_VERTEXELEMENTFORMAT_HALF2, SDL_GPU_VERTEXELEMENTFORMAT_HALF4 = range(31)

SDL_GPUVertexInputRate: typing.TypeAlias = SDL_TYPE["SDL_GPUVertexInputRate", SDL_ENUM]

SDL_GPU_VERTEXINPUTRATE_VERTEX, SDL_GPU_VERTEXINPUTRATE_INSTANCE = range(2)

SDL_GPUFillMode: typing.TypeAlias = SDL_TYPE["SDL_GPUFillMode", SDL_ENUM]

SDL_GPU_FILLMODE_FILL, SDL_GPU_FILLMODE_LINE = range(2)

SDL_GPUCullMode: typing.TypeAlias = SDL_TYPE["SDL_GPUCullMode", SDL_ENUM]

SDL_GPU_CULLMODE_NONE, SDL_GPU_CULLMODE_FRONT, SDL_GPU_CULLMODE_BACK = range(3)

SDL_GPUFrontFace: typing.TypeAlias = SDL_TYPE["SDL_GPUFrontFace", SDL_ENUM]

SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE, SDL_GPU_FRONTFACE_CLOCKWISE = range(2)

SDL_GPUCompareOp: typing.TypeAlias = SDL_TYPE["SDL_GPUCompareOp", SDL_ENUM]

SDL_GPU_COMPAREOP_INVALID, SDL_GPU_COMPAREOP_NEVER, SDL_GPU_COMPAREOP_LESS, SDL_GPU_COMPAREOP_EQUAL, SDL_GPU_COMPAREOP_LESS_OR_EQUAL, \
    SDL_GPU_COMPAREOP_GREATER, SDL_GPU_COMPAREOP_NOT_EQUAL, SDL_GPU_COMPAREOP_GREATER_OR_EQUAL, SDL_GPU_COMPAREOP_ALWAYS = range(9)

SDL_GPUStencilOp: typing.TypeAlias = SDL_TYPE["SDL_GPUStencilOp", SDL_ENUM]

SDL_GPU_STENCILOP_INVALID, SDL_GPU_STENCILOP_KEEP, SDL_GPU_STENCILOP_ZERO, SDL_GPU_STENCILOP_REPLACE, SDL_GPU_STENCILOP_INCREMENT_AND_CLAMP, \
    SDL_GPU_STENCILOP_DECREMENT_AND_CLAMP, SDL_GPU_STENCILOP_INVERT, SDL_GPU_STENCILOP_INCREMENT_AND_WRAP, SDL_GPU_STENCILOP_DECREMENT_AND_WRAP = range(9)

SDL_GPUBlendOp: typing.TypeAlias = SDL_TYPE["SDL_GPUBlendOp", SDL_ENUM]

SDL_GPU_BLENDOP_INVALID, SDL_GPU_BLENDOP_ADD, SDL_GPU_BLENDOP_SUBTRACT, \
    SDL_GPU_BLENDOP_REVERSE_SUBTRACT, SDL_GPU_BLENDOP_MIN, SDL_GPU_BLENDOP_MAX = range(6)

SDL_GPUBlendFactor: typing.TypeAlias = SDL_TYPE["SDL_GPUBlendFactor", SDL_ENUM]

SDL_GPU_BLENDFACTOR_INVALID, SDL_GPU_BLENDFACTOR_ZERO, SDL_GPU_BLENDFACTOR_ONE, SDL_GPU_BLENDFACTOR_SRC_COLOR, SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_COLOR, \
    SDL_GPU_BLENDFACTOR_DST_COLOR, SDL_GPU_BLENDFACTOR_ONE_MINUS_DST_COLOR, SDL_GPU_BLENDFACTOR_SRC_ALPHA, SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_GPU_BLENDFACTOR_DST_ALPHA, \
        SDL_GPU_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_GPU_BLENDFACTOR_CONSTANT_COLOR, SDL_GPU_BLENDFACTOR_ONE_MINUS_CONSTANT_COLOR, SDL_GPU_BLENDFACTOR_SRC_ALPHA_SATURATE = range(14)

SDL_GPUColorComponentFlags: typing.TypeAlias = SDL_TYPE["SDL_GPUColorComponentFlags", ctypes.c_uint8]

SDL_GPU_COLORCOMPONENT_R, SDL_GPU_COLORCOMPONENT_G, SDL_GPU_COLORCOMPONENT_B, \
    SDL_GPU_COLORCOMPONENT_A = 1 << 0, 1 << 1, 1 << 2, 1 << 3

SDL_GPUFilter: typing.TypeAlias = SDL_TYPE["SDL_GPUFilter", SDL_ENUM]

SDL_GPU_FILTER_NEAREST, SDL_GPU_FILTER_LINEAR = range(2)

SDL_GPUSamplerMipmapMode: typing.TypeAlias = SDL_TYPE["SDL_GPUSamplerMipmapMode", SDL_ENUM]

SDL_GPU_SAMPLERMIPMAPMODE_NEAREST, SDL_GPU_SAMPLERMIPMAPMODE_LINEAR = range(2)

SDL_GPUSamplerAddressMode: typing.TypeAlias = SDL_TYPE["SDL_GPUSamplerAddressMode", SDL_ENUM]

SDL_GPU_SAMPLERADDRESSMODE_REPEAT, SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT, SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE = range(3)

SDL_GPUPresentMode: typing.TypeAlias = SDL_TYPE["SDL_GPUPresentMode", SDL_ENUM]

SDL_GPU_PRESENTMODE_VSYNC, SDL_GPU_PRESENTMODE_IMMEDIATE, SDL_GPU_PRESENTMODE_MAILBOX = range(3)

SDL_GPUSwapchainComposition: typing.TypeAlias = SDL_TYPE["SDL_GPUSwapchainComposition", SDL_ENUM]

SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_SWAPCHAINCOMPOSITION_SDR_LINEAR, \
    SDL_GPU_SWAPCHAINCOMPOSITION_HDR_EXTENDED_LINEAR, SDL_GPU_SWAPCHAINCOMPOSITION_HDR10_ST2084 = range(4)

class SDL_GPUViewport(ctypes.Structure):
    _fields_ = [
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("w", ctypes.c_float),
        ("h", ctypes.c_float),
        ("min_depth", ctypes.c_float),
        ("max_depth", ctypes.c_float)
    ]

class SDL_GPUTextureTransferInfo(ctypes.Structure):
    _fields_ = [
        ("transfer_buffer", SDL_POINTER[SDL_GPUTransferBuffer]),
        ("offset", ctypes.c_uint32),
        ("pixels_per_row", ctypes.c_uint32),
        ("rows_per_layer", ctypes.c_uint32)
    ]

class SDL_GPUTransferBufferLocation(ctypes.Structure):
    _fields_ = [
        ("transfer_buffer", SDL_POINTER[SDL_GPUTransferBuffer]),
        ("offset", ctypes.c_uint32)
    ]

class SDL_GPUTextureLocation(ctypes.Structure):
    _fields_ = [
        ("texture", SDL_POINTER[SDL_GPUTexture]),
        ("mip_level", ctypes.c_uint32),
        ("layer", ctypes.c_uint32),
        ("x", ctypes.c_uint32),
        ("y", ctypes.c_uint32),
        ("z", ctypes.c_uint32)
    ]

class SDL_GPUTextureRegion(ctypes.Structure):
    _fields_ = [
        ("texture", SDL_POINTER[SDL_GPUTexture]),
        ("mip_level", ctypes.c_uint32),
        ("layer", ctypes.c_uint32),
        ("x", ctypes.c_uint32),
        ("y", ctypes.c_uint32),
        ("z", ctypes.c_uint32),
        ("w", ctypes.c_uint32),
        ("h", ctypes.c_uint32),
        ("d", ctypes.c_uint32)
    ]

class SDL_GPUBlitRegion(ctypes.Structure):
    _fields_ = [
        ("texture", SDL_POINTER[SDL_GPUTexture]),
        ("mip_level", ctypes.c_uint32),
        ("layer_or_depth_plane", ctypes.c_uint32),
        ("x", ctypes.c_uint32),
        ("y", ctypes.c_uint32),
        ("w", ctypes.c_uint32),
        ("h", ctypes.c_uint32)
    ]

class SDL_GPUBufferLocation(ctypes.Structure):
    _fields_ = [
        ("buffer", SDL_POINTER[SDL_GPUBuffer]),
        ("offset", ctypes.c_uint32)
    ]

class SDL_GPUBufferRegion(ctypes.Structure):
    _fields_ = [
        ("buffer", SDL_POINTER[SDL_GPUBuffer]),
        ("offset", ctypes.c_uint32),
        ("size", ctypes.c_uint32)
    ]

class SDL_GPUIndirectDrawCommand(ctypes.Structure):
    _fields_ = [
        ("num_vertices", ctypes.c_uint32),
        ("num_instances", ctypes.c_uint32),
        ("first_vertex", ctypes.c_uint32),
        ("first_instance", ctypes.c_uint32)
    ]

class SDL_GPUIndexedIndirectDrawCommand(ctypes.Structure):
    _fields_ = [
        ("num_indices", ctypes.c_uint32),
        ("num_instances", ctypes.c_uint32),
        ("first_index", ctypes.c_uint32),
        ("vertex_offset", ctypes.c_int32),
        ("first_instance", ctypes.c_uint32)
    ]

class SDL_GPUIndirectDispatchCommand(ctypes.Structure):
    _fields_ = [
        ("groupcount_x", ctypes.c_uint32),
        ("groupcount_y", ctypes.c_uint32),
        ("groupcount_z", ctypes.c_uint32)
    ]

class SDL_GPUSamplerCreateInfo(ctypes.Structure):
    _fields_ = [
        ("min_filter", SDL_GPUFilter),
        ("mag_filter", SDL_GPUFilter),
        ("mipmap_mode", SDL_GPUSamplerMipmapMode),
        ("address_mode_u", SDL_GPUSamplerAddressMode),
        ("address_mode_v", SDL_GPUSamplerAddressMode),
        ("address_mode_w", SDL_GPUSamplerAddressMode),
        ("mip_lod_bias", ctypes.c_float),
        ("max_anisotropy", ctypes.c_float),
        ("compare_op", SDL_GPUCompareOp),
        ("min_lod", ctypes.c_float),
        ("max_lod", ctypes.c_float),
        ("enable_anisotropy", ctypes.c_bool),
        ("enable_compare", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("props", SDL_PropertiesID)
    ]

class SDL_GPUVertexBufferDescription(ctypes.Structure):
    _fields_ = [
        ("slot", ctypes.c_uint32),
        ("pitch", ctypes.c_uint32),
        ("input_rate", SDL_GPUVertexInputRate),
        ("instance_step_rate", ctypes.c_uint32)
    ]

class SDL_GPUVertexAttribute(ctypes.Structure):
    _fields_ = [
        ("location", ctypes.c_uint32),
        ("buffer_slot", ctypes.c_uint32),
        ("format", SDL_GPUVertexElementFormat),
        ("offset", ctypes.c_uint32)
    ]

class SDL_GPUVertexInputState(ctypes.Structure):
    _fields_ = [
        ("vertex_buffer_descriptions", SDL_POINTER[SDL_GPUVertexBufferDescription]),
        ("num_vertex_buffers", ctypes.c_uint32),
        ("vertex_attributes", SDL_POINTER[SDL_GPUVertexAttribute]),
        ("num_vertex_attributes", ctypes.c_uint32)
    ]

class SDL_GPUStencilOpState(ctypes.Structure):
    _fields_ = [
        ("fail_op", SDL_GPUStencilOp),
        ("pass_op", SDL_GPUStencilOp),
        ("depth_fail_op", SDL_GPUStencilOp),
        ("compare_op", SDL_GPUCompareOp)
    ]

class SDL_GPUColorTargetBlendState(ctypes.Structure):
    _fields_ = [
        ("src_color_blendfactor", SDL_GPUBlendFactor),
        ("dst_color_blendfactor", SDL_GPUBlendFactor),
        ("color_blend_op", SDL_GPUBlendOp),
        ("src_alpha_blendfactor", SDL_GPUBlendFactor),
        ("dst_alpha_blendfactor", SDL_GPUBlendFactor),
        ("alpha_blend_op", SDL_GPUBlendOp),
        ("color_write_mask", SDL_GPUColorComponentFlags),
        ("enable_blend", ctypes.c_bool),
        ("enable_color_write_mask", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8)
    ]

class SDL_GPUShaderCreateInfo(ctypes.Structure):
    _fields_ = [
        ("code_size", ctypes.c_size_t),
        ("code", SDL_POINTER[ctypes.c_uint8]),
        ("entrypoint", ctypes.c_char_p),
        ("format", SDL_GPUShaderFormat),
        ("stage", SDL_GPUShaderStage),
        ("num_samplers", ctypes.c_uint32),
        ("num_storage_textures", ctypes.c_uint32),
        ("num_storage_buffers", ctypes.c_uint32),
        ("num_uniform_buffers", ctypes.c_uint32),
        ("props", SDL_PropertiesID)
    ]

class SDL_GPUTextureCreateInfo(ctypes.Structure):
    _fields_ = [
        ("type", SDL_GPUTextureType),
        ("format", SDL_GPUTextureFormat),
        ("usage", SDL_GPUTextureUsageFlags),
        ("width", ctypes.c_uint32),
        ("height", ctypes.c_uint32),
        ("layer_count_or_depth", ctypes.c_uint32),
        ("num_levels", ctypes.c_uint32),
        ("sample_count", SDL_GPUSampleCount),
        ("props", SDL_PropertiesID)
    ]

class SDL_GPUBufferCreateInfo(ctypes.Structure):
    _fields_ = [
        ("usage", SDL_GPUBufferUsageFlags),
        ("size", ctypes.c_uint32),
        ("props", SDL_PropertiesID)
    ]

class SDL_GPUTransferBufferCreateInfo(ctypes.Structure):
    _fields_ = [
        ("usage", SDL_GPUTransferBufferUsage),
        ("size", ctypes.c_uint32),
        ("props", SDL_PropertiesID)
    ]

class SDL_GPURasterizerState(ctypes.Structure):
    _fields_ = [
        ("fill_mode", SDL_GPUFillMode),
        ("cull_mode", SDL_GPUCullMode),
        ("front_face", SDL_GPUFrontFace),
        ("depth_bias_constant_factor", ctypes.c_float),
        ("depth_bias_clamp", ctypes.c_float),
        ("depth_bias_slope_factor", ctypes.c_float),
        ("enable_depth_bias", ctypes.c_bool),
        ("enable_depth_clip", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8)
    ]

class SDL_GPUMultisampleState(ctypes.Structure):
    _fields_ = [
        ("sample_count", SDL_GPUSampleCount),
        ("sample_mask", ctypes.c_uint32),
        ("enable_mask", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8)
    ]

class SDL_GPUDepthStencilState(ctypes.Structure):
    _fields_ = [
        ("compare_op", SDL_GPUCompareOp),
        ("back_stencil_state", SDL_GPUStencilOpState),
        ("front_stencil_state", SDL_GPUStencilOpState),
        ("compare_mask", ctypes.c_uint8),
        ("write_mask", ctypes.c_uint8),
        ("enable_depth_test", ctypes.c_bool),
        ("enable_depth_write", ctypes.c_bool),
        ("enable_stencil_test", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8)
    ]

class SDL_GPUColorTargetDescription(ctypes.Structure):
    _fields_ = [
        ("format", SDL_GPUTextureFormat),
        ("blend_state", SDL_GPUColorTargetBlendState)
    ]

class SDL_GPUGraphicsPipelineTargetInfo(ctypes.Structure):
    _fields_ = [
        ("color_target_descriptions", SDL_POINTER[SDL_GPUColorTargetDescription]),
        ("num_color_targets", ctypes.c_uint32),
        ("depth_stencil_format", SDL_GPUTextureFormat),
        ("has_depth_stencil_target", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8)
    ]

class SDL_GPUGraphicsPipelineCreateInfo(ctypes.Structure):
    _fields_ = [
        ("vertex_shader", SDL_POINTER[SDL_GPUShader]),
        ("fragment_shader", SDL_POINTER[SDL_GPUShader]),
        ("vertex_input_state", SDL_GPUVertexInputState),
        ("primitive_type", SDL_GPUPrimitiveType),
        ("rasterizer_state", SDL_GPURasterizerState),
        ("multisample_state", SDL_GPUMultisampleState),
        ("depth_stencil_state", SDL_GPUDepthStencilState),
        ("target_info", SDL_GPUGraphicsPipelineTargetInfo),
        ("props", SDL_PropertiesID)
    ]

class SDL_GPUComputePipelineCreateInfo(ctypes.Structure):
    _fields_ = [
        ("code_size", ctypes.c_size_t),
        ("code", SDL_POINTER[ctypes.c_uint8]),
        ("entrypoint", ctypes.c_char_p),
        ("format", SDL_GPUShaderFormat),
        ("num_samplers", ctypes.c_uint32),
        ("num_readonly_storage_textures", ctypes.c_uint32),
        ("num_readonly_storage_buffers", ctypes.c_uint32),
        ("num_readwrite_storage_textures", ctypes.c_uint32),
        ("num_readwrite_storage_buffers", ctypes.c_uint32),
        ("num_uniform_buffers", ctypes.c_uint32),
        ("threadcount_x", ctypes.c_uint32),
        ("threadcount_y", ctypes.c_uint32),
        ("threadcount_z", ctypes.c_uint32),
        ("props", SDL_PropertiesID)
    ]

class SDL_GPUColorTargetInfo(ctypes.Structure):
    _fields_ = [
        ("texture", SDL_POINTER[SDL_GPUTexture]),
        ("mip_level", ctypes.c_uint32),
        ("layer_or_depth_plane", ctypes.c_uint32),
        ("clear_color", SDL_FColor),
        ("load_op", SDL_GPULoadOp),
        ("store_op", SDL_GPUStoreOp),
        ("resolve_texture", SDL_POINTER[SDL_GPUTexture]),
        ("resolve_mip_level", ctypes.c_uint32),
        ("resolve_layer", ctypes.c_uint32),
        ("cycle", ctypes.c_bool),
        ("cycle_resolve_texture", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8)
    ]

class SDL_GPUDepthStencilTargetInfo(ctypes.Structure):
    _fields_ = [
        ("texture", SDL_POINTER[SDL_GPUTexture]),
        ("clear_depth", ctypes.c_float),
        ("load_op", SDL_GPULoadOp),
        ("store_op", SDL_GPUStoreOp),
        ("stencil_load_op", SDL_GPULoadOp),
        ("stencil_store_op", SDL_GPUStoreOp),
        ("cycle", ctypes.c_bool),
        ("clear_stencil", ctypes.c_uint8),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8)
    ]

class SDL_GPUBlitInfo(ctypes.Structure):
    _fields_ = [
        ("source", SDL_GPUBlitRegion),
        ("destination", SDL_GPUBlitRegion),
        ("load_op", SDL_GPULoadOp),
        ("clear_color", SDL_FColor),
        ("flip_mode", SDL_FlipMode),
        ("filter", SDL_GPUFilter),
        ("cycle", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8)
    ]

class SDL_GPUBufferBinding(ctypes.Structure):
    _fields_ = [
        ("buffer", SDL_POINTER[SDL_GPUBuffer]),
        ("offset", ctypes.c_uint32)
    ]

class SDL_GPUTextureSamplerBinding(ctypes.Structure):
    _fields_ = [
        ("texture", SDL_POINTER[SDL_GPUTexture]),
        ("sampler", SDL_POINTER[SDL_GPUSampler])
    ]

class SDL_GPUStorageBufferReadWriteBinding(ctypes.Structure):
    _fields_ = [
        ("buffer", SDL_POINTER[SDL_GPUBuffer]),
        ("cycle", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8)
    ]

class SDL_GPUStorageTextureReadWriteBinding(ctypes.Structure):
    _fields_ = [
        ("texture", SDL_POINTER[SDL_GPUTexture]),
        ("mip_level", ctypes.c_uint32),
        ("layer", ctypes.c_uint32),
        ("cycle", ctypes.c_bool),
        ("padding1", ctypes.c_uint8),
        ("padding2", ctypes.c_uint8),
        ("padding3", ctypes.c_uint8)
    ]

SDL_GPUSupportsShaderFormats: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GPUSupportsShaderFormats", ctypes.c_bool, [SDL_GPUShaderFormat, ctypes.c_char_p], SDL_BINARY]
SDL_GPUSupportsProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GPUSupportsProperties", ctypes.c_bool, [SDL_PropertiesID], SDL_BINARY]

SDL_CreateGPUDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateGPUDevice", SDL_POINTER[SDL_GPUDevice], [SDL_GPUShaderFormat, ctypes.c_bool, ctypes.c_char_p], SDL_BINARY]
SDL_CreateGPUDeviceWithProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateGPUDeviceWithProperties", SDL_POINTER[SDL_GPUDevice], [SDL_PropertiesID], SDL_BINARY]

SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN: bytes = "SDL.gpu.device.create.debugmode".encode()
SDL_PROP_GPU_DEVICE_CREATE_PREFERLOWPOWER_BOOLEAN: bytes = "SDL.gpu.device.create.preferlowpower".encode()
SDL_PROP_GPU_DEVICE_CREATE_NAME_STRING: bytes = "SDL.gpu.device.create.name".encode()
SDL_PROP_GPU_DEVICE_CREATE_SHADERS_PRIVATE_BOOLEAN: bytes = "SDL.gpu.device.create.shaders.private".encode()
SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN: bytes = "SDL.gpu.device.create.shaders.spirv".encode()
SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXBC_BOOLEAN: bytes = "SDL.gpu.device.create.shaders.dxbc".encode()
SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXIL_BOOLEAN: bytes = "SDL.gpu.device.create.shaders.dxil".encode()
SDL_PROP_GPU_DEVICE_CREATE_SHADERS_MSL_BOOLEAN: bytes = "SDL.gpu.device.create.shaders.msl".encode()
SDL_PROP_GPU_DEVICE_CREATE_SHADERS_METALLIB_BOOLEAN: bytes = "SDL.gpu.device.create.shaders.metallib".encode()
SDL_PROP_GPU_DEVICE_CREATE_D3D12_SEMANTIC_NAME_STRING: bytes = "SDL.gpu.device.create.d3d12.semantic".encode()

SDL_DestroyGPUDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyGPUDevice", None, [SDL_POINTER[SDL_GPUDevice]], SDL_BINARY]

SDL_GetNumGPUDrivers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumGPUDrivers", ctypes.c_int, [], SDL_BINARY]
SDL_GetGPUDriver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGPUDriver", ctypes.c_char_p, [ctypes.c_int], SDL_BINARY]
SDL_GetGPUDeviceDriver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGPUDeviceDriver", ctypes.c_char_p, [SDL_POINTER[SDL_GPUDevice]], SDL_BINARY]
SDL_GetGPUShaderFormats: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGPUShaderFormats", SDL_GPUShaderFormat, [SDL_POINTER[SDL_GPUDevice]], SDL_BINARY]

SDL_CreateGPUComputePipeline: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateGPUComputePipeline", SDL_POINTER[SDL_GPUComputePipeline], [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUComputePipelineCreateInfo]], SDL_BINARY]

SDL_PROP_GPU_COMPUTEPIPELINE_CREATE_NAME_STRING: bytes = "SDL.gpu.computepipeline.create.name".encode()

SDL_CreateGPUGraphicsPipeline: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateGPUGraphicsPipeline", SDL_POINTER[SDL_GPUGraphicsPipeline], [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUGraphicsPipelineCreateInfo]], SDL_BINARY]

SDL_PROP_GPU_GRAPHICSPIPELINE_CREATE_NAME_STRING: bytes = "SDL.gpu.graphicspipeline.create.name".encode()

SDL_CreateGPUSampler: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateGPUSampler", SDL_POINTER[SDL_GPUSampler], [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUSamplerCreateInfo]], SDL_BINARY]

SDL_PROP_GPU_SAMPLER_CREATE_NAME_STRING: bytes = "SDL.gpu.sampler.create.name".encode()

SDL_CreateGPUShader: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateGPUShader", SDL_POINTER[SDL_GPUShader], [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUShaderCreateInfo]], SDL_BINARY]

SDL_PROP_GPU_SHADER_CREATE_NAME_STRING: bytes = "SDL.gpu.shader.create.name".encode()

SDL_CreateGPUTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateGPUTexture", SDL_POINTER[SDL_GPUTexture], [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUTextureCreateInfo]], SDL_BINARY]

SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_R_FLOAT: bytes = "SDL.gpu.texture.create.d3d12.clear.r".encode()
SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_G_FLOAT: bytes = "SDL.gpu.texture.create.d3d12.clear.g".encode()
SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_B_FLOAT: bytes = "SDL.gpu.texture.create.d3d12.clear.b".encode()
SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_A_FLOAT: bytes = "SDL.gpu.texture.create.d3d12.clear.a".encode()
SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_DEPTH_FLOAT: bytes = "SDL.gpu.texture.create.d3d12.clear.depth".encode()
SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_STENCIL_NUMBER: bytes = "SDL.gpu.texture.create.d3d12.clear.stencil".encode()
SDL_PROP_GPU_TEXTURE_CREATE_NAME_STRING: bytes = "SDL.gpu.texture.create.name".encode()

SDL_CreateGPUBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateGPUBuffer", SDL_POINTER[SDL_GPUBuffer], [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUBufferCreateInfo]], SDL_BINARY]

SDL_PROP_GPU_BUFFER_CREATE_NAME_STRING: bytes = "SDL.gpu.buffer.create.name".encode()

SDL_CreateGPUTransferBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateGPUTransferBuffer", SDL_POINTER[SDL_GPUTransferBuffer], [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUTransferBufferCreateInfo]], SDL_BINARY]

SDL_PROP_GPU_TRANSFERBUFFER_CREATE_NAME_STRING: bytes = "SDL.gpu.transferbuffer.create.name".encode()

SDL_SetGPUBufferName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGPUBufferName", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUBuffer], ctypes.c_char_p], SDL_BINARY]
SDL_SetGPUTextureName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGPUTextureName", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUTexture], ctypes.c_char_p], SDL_BINARY]

SDL_InsertGPUDebugLabel: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_InsertGPUDebugLabel", None, [SDL_POINTER[SDL_GPUCommandBuffer], ctypes.c_char_p], SDL_BINARY]
SDL_PushGPUDebugGroup: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PushGPUDebugGroup", None, [SDL_POINTER[SDL_GPUCommandBuffer], ctypes.c_char_p], SDL_BINARY]
SDL_PopGPUDebugGroup: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PopGPUDebugGroup", None, [SDL_POINTER[SDL_GPUCommandBuffer]], SDL_BINARY]

SDL_ReleaseGPUTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReleaseGPUTexture", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUTexture]], SDL_BINARY]
SDL_ReleaseGPUSampler: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReleaseGPUSampler", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUSampler]], SDL_BINARY]
SDL_ReleaseGPUBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReleaseGPUBuffer", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUBuffer]], SDL_BINARY]
SDL_ReleaseGPUTransferBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReleaseGPUTransferBuffer", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUTransferBuffer]], SDL_BINARY]
SDL_ReleaseGPUComputePipeline: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReleaseGPUComputePipeline", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUComputePipeline]], SDL_BINARY]
SDL_ReleaseGPUShader: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReleaseGPUShader", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUShader]], SDL_BINARY]
SDL_ReleaseGPUGraphicsPipeline: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReleaseGPUGraphicsPipeline", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUGraphicsPipeline]], SDL_BINARY]

SDL_AcquireGPUCommandBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AcquireGPUCommandBuffer", SDL_POINTER[SDL_GPUCommandBuffer], [SDL_POINTER[SDL_GPUDevice]], SDL_BINARY]

SDL_PushGPUVertexUniformData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PushGPUVertexUniformData", None, [SDL_POINTER[SDL_GPUCommandBuffer], ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32], SDL_BINARY]
SDL_PushGPUFragmentUniformData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PushGPUFragmentUniformData", None, [SDL_POINTER[SDL_GPUCommandBuffer], ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32], SDL_BINARY]
SDL_PushGPUComputeUniformData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PushGPUComputeUniformData", None, [SDL_POINTER[SDL_GPUCommandBuffer], ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32], SDL_BINARY]

SDL_BeginGPURenderPass: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BeginGPURenderPass", SDL_POINTER[SDL_GPURenderPass], [SDL_POINTER[SDL_GPUCommandBuffer], SDL_POINTER[SDL_GPUColorTargetInfo], ctypes.c_uint32, SDL_POINTER[SDL_GPUDepthStencilTargetInfo]], SDL_BINARY]
SDL_BindGPUGraphicsPipeline: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUGraphicsPipeline", None, [SDL_POINTER[SDL_GPURenderPass], SDL_POINTER[SDL_GPUGraphicsPipeline]], SDL_BINARY]
SDL_SetGPUViewport: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGPUViewport", None, [SDL_POINTER[SDL_GPURenderPass], SDL_POINTER[SDL_GPUViewport]], SDL_BINARY]
SDL_SetGPUScissor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGPUScissor", None, [SDL_POINTER[SDL_GPURenderPass], SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_SetGPUBlendConstants: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGPUBlendConstants", None, [SDL_POINTER[SDL_GPURenderPass], SDL_FColor], SDL_BINARY]
SDL_SetGPUStencilReference: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGPUStencilReference", None, [SDL_POINTER[SDL_GPURenderPass], ctypes.c_uint8], SDL_BINARY]
SDL_BindGPUVertexBuffers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUVertexBuffers", None, [SDL_POINTER[SDL_GPURenderPass], ctypes.c_uint32, SDL_POINTER[SDL_GPUBufferBinding], ctypes.c_uint32], SDL_BINARY]
SDL_BindGPUIndexBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUIndexBuffer", None, [SDL_POINTER[SDL_GPURenderPass], SDL_POINTER[SDL_GPUBufferBinding], SDL_GPUIndexElementSize], SDL_BINARY]
SDL_BindGPUVertexSamplers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUVertexSamplers", None, [SDL_POINTER[SDL_GPURenderPass], ctypes.c_uint32, SDL_POINTER[SDL_GPUTextureSamplerBinding], ctypes.c_uint32], SDL_BINARY]
SDL_BindGPUVertexStorageTextures: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUVertexStorageTextures", None, [SDL_POINTER[SDL_GPURenderPass], ctypes.c_uint32, SDL_POINTER[SDL_POINTER[SDL_GPUTexture]], ctypes.c_uint32], SDL_BINARY]
SDL_BindGPUVertexStorageBuffers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUVertexStorageBuffers", None, [SDL_POINTER[SDL_GPURenderPass], ctypes.c_uint32, SDL_POINTER[SDL_POINTER[SDL_GPUBuffer]], ctypes.c_uint32], SDL_BINARY]
SDL_BindGPUFragmentSamplers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUFragmentSamplers", None, [SDL_POINTER[SDL_GPURenderPass], ctypes.c_uint32, SDL_POINTER[SDL_GPUTextureSamplerBinding], ctypes.c_uint32], SDL_BINARY]
SDL_BindGPUFragmentStorageTextures: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUFragmentStorageTextures", None, [SDL_POINTER[SDL_GPURenderPass], ctypes.c_uint32, SDL_POINTER[SDL_POINTER[SDL_GPUTexture]], ctypes.c_uint32], SDL_BINARY]
SDL_BindGPUFragmentStorageBuffers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUFragmentStorageBuffers", None, [SDL_POINTER[SDL_GPURenderPass], ctypes.c_uint32, SDL_POINTER[SDL_POINTER[SDL_GPUBuffer]], ctypes.c_uint32], SDL_BINARY]
SDL_DrawGPUIndexedPrimitives: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DrawGPUIndexedPrimitives", None, [SDL_POINTER[SDL_GPURenderPass], ctypes.c_uint32, ctypes.c_uint32, ctypes.c_uint32, ctypes.c_int32, ctypes.c_uint32], SDL_BINARY]
SDL_DrawGPUPrimitives: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DrawGPUPrimitives", None, [SDL_POINTER[SDL_GPURenderPass], ctypes.c_uint32, ctypes.c_uint32, ctypes.c_uint32, ctypes.c_uint32], SDL_BINARY]
SDL_DrawGPUPrimitivesIndirect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DrawGPUPrimitivesIndirect", None, [SDL_POINTER[SDL_GPURenderPass], SDL_POINTER[SDL_GPUBuffer], ctypes.c_uint32, ctypes.c_uint32], SDL_BINARY]
SDL_DrawGPUIndexedPrimitivesIndirect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DrawGPUIndexedPrimitivesIndirect", None, [SDL_POINTER[SDL_GPURenderPass], SDL_POINTER[SDL_GPUBuffer], ctypes.c_uint32, ctypes.c_uint32], SDL_BINARY]
SDL_EndGPURenderPass: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EndGPURenderPass", None, [SDL_POINTER[SDL_GPURenderPass]], SDL_BINARY]

SDL_BeginGPUComputePass: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BeginGPUComputePass", SDL_POINTER[SDL_GPUComputePass], [SDL_POINTER[SDL_GPUCommandBuffer], SDL_POINTER[SDL_GPUStorageTextureReadWriteBinding], ctypes.c_uint32, SDL_POINTER[SDL_GPUStorageBufferReadWriteBinding], ctypes.c_uint32], SDL_BINARY]
SDL_BindGPUComputePipeline: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUComputePipeline", None, [SDL_POINTER[SDL_GPUComputePass], SDL_POINTER[SDL_GPUComputePipeline]], SDL_BINARY]
SDL_BindGPUComputeSamplers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUComputeSamplers", None, [SDL_POINTER[SDL_GPUComputePass], ctypes.c_uint32, SDL_POINTER[SDL_GPUTextureSamplerBinding], ctypes.c_uint32], SDL_BINARY]
SDL_BindGPUComputeStorageTextures: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUComputeStorageTextures", None, [SDL_POINTER[SDL_GPUComputePass], ctypes.c_uint32, SDL_POINTER[SDL_POINTER[SDL_GPUTexture]], ctypes.c_uint32], SDL_BINARY]
SDL_BindGPUComputeStorageBuffers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindGPUComputeStorageBuffers", None, [SDL_POINTER[SDL_GPUComputePass], ctypes.c_uint32, SDL_POINTER[SDL_POINTER[SDL_GPUBuffer]], ctypes.c_uint32], SDL_BINARY]
SDL_DispatchGPUCompute: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DispatchGPUCompute", None, [SDL_POINTER[SDL_GPUComputePass], ctypes.c_uint32, ctypes.c_uint32, ctypes.c_uint32], SDL_BINARY]
SDL_DispatchGPUComputeIndirect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DispatchGPUComputeIndirect", None, [SDL_POINTER[SDL_GPUComputePass], SDL_POINTER[SDL_GPUBuffer], ctypes.c_uint32], SDL_BINARY]
SDL_EndGPUComputePass: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EndGPUComputePass", None, [SDL_POINTER[SDL_GPUComputePass]], SDL_BINARY]

SDL_MapGPUTransferBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_MapGPUTransferBuffer", ctypes.c_void_p, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUTransferBuffer], ctypes.c_bool], SDL_BINARY]
SDL_UnmapGPUTransferBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnmapGPUTransferBuffer", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUTransferBuffer]], SDL_BINARY]

SDL_BeginGPUCopyPass: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BeginGPUCopyPass", SDL_POINTER[SDL_GPUCopyPass], [SDL_POINTER[SDL_GPUCommandBuffer]], SDL_BINARY]
SDL_UploadToGPUTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UploadToGPUTexture", None, [SDL_POINTER[SDL_GPUCopyPass], SDL_POINTER[SDL_GPUTextureTransferInfo], SDL_POINTER[SDL_GPUTextureRegion], ctypes.c_bool], SDL_BINARY]
SDL_UploadToGPUBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UploadToGPUBuffer", None, [SDL_POINTER[SDL_GPUCopyPass], SDL_POINTER[SDL_GPUTransferBufferLocation], SDL_POINTER[SDL_GPUBufferRegion], ctypes.c_bool], SDL_BINARY]
SDL_CopyGPUTextureToTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CopyGPUTextureToTexture", None, [SDL_POINTER[SDL_GPUCopyPass], SDL_POINTER[SDL_GPUTextureLocation], SDL_POINTER[SDL_GPUTextureLocation], ctypes.c_uint32, ctypes.c_uint32, ctypes.c_uint32, ctypes.c_bool], SDL_BINARY]
SDL_CopyGPUBufferToBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CopyGPUBufferToBuffer", None, [SDL_POINTER[SDL_GPUCopyPass], SDL_POINTER[SDL_GPUBufferLocation], SDL_POINTER[SDL_GPUBufferLocation], ctypes.c_uint32, ctypes.c_bool], SDL_BINARY]
SDL_DownloadFromGPUTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DownloadFromGPUTexture", None, [SDL_POINTER[SDL_GPUCopyPass], SDL_POINTER[SDL_GPUTextureRegion], SDL_POINTER[SDL_GPUTextureTransferInfo]], SDL_BINARY]
SDL_DownloadFromGPUBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DownloadFromGPUBuffer", None, [SDL_POINTER[SDL_GPUCopyPass], SDL_POINTER[SDL_GPUBufferRegion], SDL_POINTER[SDL_GPUTransferBufferLocation]], SDL_BINARY]
SDL_EndGPUCopyPass: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EndGPUCopyPass", None, [SDL_POINTER[SDL_GPUCopyPass]], SDL_BINARY]

SDL_GenerateMipmapsForGPUTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GenerateMipmapsForGPUTexture", None, [SDL_POINTER[SDL_GPUCommandBuffer], SDL_POINTER[SDL_GPUTexture]], SDL_BINARY]
SDL_BlitGPUTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BlitGPUTexture", None, [SDL_POINTER[SDL_GPUCommandBuffer], SDL_POINTER[SDL_GPUBlitInfo]], SDL_BINARY]

SDL_WindowSupportsGPUSwapchainComposition: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WindowSupportsGPUSwapchainComposition", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_Window], SDL_GPUSwapchainComposition], SDL_BINARY]
SDL_WindowSupportsGPUPresentMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WindowSupportsGPUPresentMode", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_Window], SDL_GPUPresentMode], SDL_BINARY]
SDL_ClaimWindowForGPUDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ClaimWindowForGPUDevice", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_ReleaseWindowFromGPUDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReleaseWindowFromGPUDevice", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_SetGPUSwapchainParameters: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGPUSwapchainParameters", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_Window], SDL_GPUSwapchainComposition, SDL_GPUPresentMode], SDL_BINARY]
SDL_SetGPUAllowedFramesInFlight: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGPUAllowedFramesInFlight", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice], ctypes.c_uint32], SDL_BINARY]
SDL_GetGPUSwapchainTextureFormat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGPUSwapchainTextureFormat", SDL_GPUTextureFormat, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_AcquireGPUSwapchainTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AcquireGPUSwapchainTexture", ctypes.c_bool, [SDL_POINTER[SDL_GPUCommandBuffer], SDL_POINTER[SDL_Window], SDL_POINTER[SDL_POINTER[SDL_GPUTexture]], SDL_POINTER[ctypes.c_uint32], SDL_POINTER[ctypes.c_uint32]], SDL_BINARY]
SDL_WaitForGPUSwapchain: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitForGPUSwapchain", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_WaitAndAcquireGPUSwapchainTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitAndAcquireGPUSwapchainTexture", ctypes.c_bool, [SDL_POINTER[SDL_GPUCommandBuffer], SDL_POINTER[SDL_Window], SDL_POINTER[SDL_POINTER[SDL_GPUTexture]], SDL_POINTER[ctypes.c_uint32], SDL_POINTER[ctypes.c_uint32]], SDL_BINARY]

SDL_SubmitGPUCommandBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SubmitGPUCommandBuffer", ctypes.c_bool, [SDL_POINTER[SDL_GPUCommandBuffer]], SDL_BINARY]
SDL_SubmitGPUCommandBufferAndAcquireFence: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SubmitGPUCommandBufferAndAcquireFence", SDL_POINTER[SDL_GPUFence], [SDL_POINTER[SDL_GPUCommandBuffer]], SDL_BINARY]
SDL_CancelGPUCommandBuffer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CancelGPUCommandBuffer", ctypes.c_bool, [SDL_POINTER[SDL_GPUCommandBuffer]], SDL_BINARY]

SDL_WaitForGPUIdle: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitForGPUIdle", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice]], SDL_BINARY]
SDL_WaitForGPUFences: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitForGPUFences", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice], ctypes.c_bool, SDL_POINTER[SDL_POINTER[SDL_GPUFence]], ctypes.c_uint32], SDL_BINARY]

SDL_QueryGPUFence: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_QueryGPUFence", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUFence]], SDL_BINARY]
SDL_ReleaseGPUFence: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReleaseGPUFence", None, [SDL_POINTER[SDL_GPUDevice], SDL_POINTER[SDL_GPUFence]], SDL_BINARY]

SDL_GPUTextureFormatTexelBlockSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GPUTextureFormatTexelBlockSize", ctypes.c_uint32, [SDL_GPUTextureFormat], SDL_BINARY]
SDL_GPUTextureSupportsFormat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GPUTextureSupportsFormat", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice], SDL_GPUTextureFormat, SDL_GPUTextureType, SDL_GPUTextureUsageFlags], SDL_BINARY]
SDL_GPUTextureSupportsSampleCount: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GPUTextureSupportsSampleCount", ctypes.c_bool, [SDL_POINTER[SDL_GPUDevice], SDL_GPUTextureFormat, SDL_GPUSampleCount], SDL_BINARY]
SDL_CalculateGPUTextureFormatSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CalculateGPUTextureFormatSize", ctypes.c_uint32, [SDL_GPUTextureFormat, ctypes.c_uint32, ctypes.c_uint32, ctypes.c_uint32], SDL_BINARY]