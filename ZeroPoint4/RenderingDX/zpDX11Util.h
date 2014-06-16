#pragma once
#ifndef ZP_DX11_UTIL
#define ZP_DX11_UTIL

// D3D11_USAGE <-> zpBufferBindType
ZP_FORCE_INLINE D3D11_USAGE __zpToDX( zpBufferBindType t ) {
	switch( t ) {
	case ZP_BUFFER_BIND_DEFAULT:	return D3D11_USAGE_DEFAULT;
	case ZP_BUFFER_BIND_DYNAMIC:	return D3D11_USAGE_DYNAMIC;
	case ZP_BUFFER_BIND_IMMUTABLE:	return D3D11_USAGE_IMMUTABLE;
	default:						return D3D11_USAGE_DEFAULT;
	}
}

ZP_FORCE_INLINE zpBufferBindType __dxToZP( D3D11_USAGE t ) {
	switch( t ) {
	case D3D11_USAGE_DEFAULT:	return ZP_BUFFER_BIND_DEFAULT;
	case D3D11_USAGE_DYNAMIC:	return ZP_BUFFER_BIND_DYNAMIC;
	case D3D11_USAGE_IMMUTABLE:	return ZP_BUFFER_BIND_IMMUTABLE;
	default:					return ZP_BUFFER_BIND_DEFAULT;
	}
}

// D3D11_BIND_FLAG <-> zpBufferType
ZP_FORCE_INLINE D3D11_BIND_FLAG __zpToDX( zpBufferType t ) {
	switch( t ) {
	case ZP_BUFFER_TYPE_VERTEX:	return D3D11_BIND_VERTEX_BUFFER;
	case ZP_BUFFER_TYPE_INDEX:	return D3D11_BIND_INDEX_BUFFER;
	case ZP_BUFFER_TYPE_CONSTANT: return D3D11_BIND_CONSTANT_BUFFER;
	default:					return D3D11_BIND_VERTEX_BUFFER;
	}
}

ZP_FORCE_INLINE zpBufferType __dxToZP( D3D11_BIND_FLAG t ) {
	switch( t ) {
	case D3D11_BIND_VERTEX_BUFFER:	return ZP_BUFFER_TYPE_VERTEX;
	case D3D11_BIND_INDEX_BUFFER:	return ZP_BUFFER_TYPE_INDEX;
	case D3D11_BIND_CONSTANT_BUFFER: return ZP_BUFFER_TYPE_CONSTANT;
	default:						return ZP_BUFFER_TYPE_VERTEX;
	}
}

// D3D11_MAP <-> zpMapType
ZP_FORCE_INLINE D3D11_MAP __zpToDX( zpMapType t ) {
	switch( t ) {
	case ZP_MAP_TYPE_READ:			return D3D11_MAP_READ;
	case ZP_MAP_TYPE_WRITE:			return D3D11_MAP_WRITE;
	case ZP_MAP_TYPE_READ_WRITE:	return D3D11_MAP_READ_WRITE;
	case ZP_MAP_TYPE_WRITE_DISCARD:	return D3D11_MAP_WRITE_DISCARD;
	case ZP_MAP_TYPE_WRITE_NO_OVERWRITE:	return D3D11_MAP_WRITE_NO_OVERWRITE;
	default:						return D3D11_MAP_WRITE_DISCARD;
	}
}

ZP_FORCE_INLINE zpMapType __dxToZP( D3D11_MAP t ) {
	switch( t ) {
	case D3D11_MAP_READ:			return ZP_MAP_TYPE_READ;
	case D3D11_MAP_WRITE:			return ZP_MAP_TYPE_WRITE;
	case D3D11_MAP_READ_WRITE:		return ZP_MAP_TYPE_READ_WRITE;
	case D3D11_MAP_WRITE_DISCARD:	return ZP_MAP_TYPE_WRITE_DISCARD;
	case D3D11_MAP_WRITE_NO_OVERWRITE: return ZP_MAP_TYPE_WRITE_NO_OVERWRITE;
	default:						return ZP_MAP_TYPE_WRITE_DISCARD;
	}
}

// DXGI_FORMAT <-> zpDisplayFormat
ZP_FORCE_INLINE DXGI_FORMAT __zpToDX( zpDisplayFormat t ) {
	switch( t ) {
	
		// R Component
	case ZP_DISPLAY_FORMAT_R8_UINT: return DXGI_FORMAT_R8_UINT;
	case ZP_DISPLAY_FORMAT_R8_SINT: return DXGI_FORMAT_R8_SINT;

	case ZP_DISPLAY_FORMAT_R16_UINT: return DXGI_FORMAT_R16_UINT;
	case ZP_DISPLAY_FORMAT_R16_SINT: return DXGI_FORMAT_R16_SINT;
	case ZP_DISPLAY_FORMAT_R16_FLOAT: return DXGI_FORMAT_R16_FLOAT;

	case ZP_DISPLAY_FORMAT_R32_UINT: return DXGI_FORMAT_R32_UINT;
	case ZP_DISPLAY_FORMAT_R32_SINT: return DXGI_FORMAT_R32_SINT;
	case ZP_DISPLAY_FORMAT_R32_FLOAT: return DXGI_FORMAT_R32_FLOAT;

		// RG Component
	case ZP_DISPLAY_FORMAT_RG8_UINT: return DXGI_FORMAT_R8G8_UINT;
	case ZP_DISPLAY_FORMAT_RG8_SINT: return DXGI_FORMAT_R8G8_SINT;

	case ZP_DISPLAY_FORMAT_RG16_UINT: return DXGI_FORMAT_R16G16_UINT;
	case ZP_DISPLAY_FORMAT_RG16_SINT: return DXGI_FORMAT_R16G16_SINT;
	case ZP_DISPLAY_FORMAT_RG16_FLOAT: return DXGI_FORMAT_R16G16_FLOAT;

	case ZP_DISPLAY_FORMAT_RG32_UINT: return DXGI_FORMAT_R32G32_UINT;
	case ZP_DISPLAY_FORMAT_RG32_SINT: return DXGI_FORMAT_R32G32_SINT;
	case ZP_DISPLAY_FORMAT_RG32_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;

		// RGB Component
	case ZP_DISPLAY_FORMAT_RGB32_UINT: return DXGI_FORMAT_R32G32B32_UINT;
	case ZP_DISPLAY_FORMAT_RGB32_SINT: return DXGI_FORMAT_R32G32B32_SINT;
	case ZP_DISPLAY_FORMAT_RGB32_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;

		// RGBA Components
	case ZP_DISPLAY_FORMAT_RGBA8_UINT: return DXGI_FORMAT_R8G8B8A8_UINT;
	case ZP_DISPLAY_FORMAT_RGBA8_SINT: return DXGI_FORMAT_R8G8B8A8_SINT;
	case ZP_DISPLAY_FORMAT_RGBA8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
	case ZP_DISPLAY_FORMAT_RGBA8_SNORM: return DXGI_FORMAT_R8G8B8A8_SNORM;

	case ZP_DISPLAY_FORMAT_RGBA16_UINT: return DXGI_FORMAT_R16G16B16A16_UINT;
	case ZP_DISPLAY_FORMAT_RGBA16_SINT: return DXGI_FORMAT_R16G16B16A16_SINT;
	case ZP_DISPLAY_FORMAT_RGBA16_UNORM: return DXGI_FORMAT_R16G16B16A16_UNORM;
	case ZP_DISPLAY_FORMAT_RGBA16_SNORM: return DXGI_FORMAT_R16G16B16A16_SNORM;
	case ZP_DISPLAY_FORMAT_RGBA16_FLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;

	case ZP_DISPLAY_FORMAT_RGBA32_UINT: return DXGI_FORMAT_R32G32B32A32_UINT;
	case ZP_DISPLAY_FORMAT_RGBA32_SINT: return DXGI_FORMAT_R32G32B32A32_SINT;
	case ZP_DISPLAY_FORMAT_RGBA32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;

		// Depth Buffer
	case ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT: return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case ZP_DISPLAY_FORMAT_D32_FLOAT: return DXGI_FORMAT_D32_FLOAT;

		// Compressed
	case ZP_DISPLAY_FORMAT_BC1: return DXGI_FORMAT_BC1_UNORM;
	case ZP_DISPLAY_FORMAT_BC2: return DXGI_FORMAT_BC2_UNORM;
	case ZP_DISPLAY_FORMAT_BC3: return DXGI_FORMAT_BC3_UNORM;

	default: return DXGI_FORMAT_UNKNOWN;
	}
}

ZP_FORCE_INLINE zpDisplayFormat __dxToZP( DXGI_FORMAT t ) {
	switch( t ) {
	
		// R Component
	case DXGI_FORMAT_R8_UINT: return ZP_DISPLAY_FORMAT_R8_UINT;
	case DXGI_FORMAT_R8_SINT: return ZP_DISPLAY_FORMAT_R8_SINT;

	case DXGI_FORMAT_R16_UINT: return ZP_DISPLAY_FORMAT_R16_UINT;
	case DXGI_FORMAT_R16_SINT: return ZP_DISPLAY_FORMAT_R16_SINT;
	case DXGI_FORMAT_R16_FLOAT: return ZP_DISPLAY_FORMAT_R16_FLOAT;

	case DXGI_FORMAT_R32_UINT: return ZP_DISPLAY_FORMAT_R32_UINT;
	case DXGI_FORMAT_R32_SINT: return ZP_DISPLAY_FORMAT_R32_SINT;
	case DXGI_FORMAT_R32_FLOAT: return ZP_DISPLAY_FORMAT_R32_FLOAT;

		// RG Component
	case DXGI_FORMAT_R8G8_UINT: return ZP_DISPLAY_FORMAT_RG8_UINT;
	case DXGI_FORMAT_R8G8_SINT: return ZP_DISPLAY_FORMAT_RG8_SINT;

	case DXGI_FORMAT_R16G16_UINT: return ZP_DISPLAY_FORMAT_RG16_UINT;
	case DXGI_FORMAT_R16G16_SINT: return ZP_DISPLAY_FORMAT_RG16_SINT;
	case DXGI_FORMAT_R16G16_FLOAT: return ZP_DISPLAY_FORMAT_RG16_FLOAT;

	case DXGI_FORMAT_R32G32_UINT: return ZP_DISPLAY_FORMAT_RG32_UINT;
	case DXGI_FORMAT_R32G32_SINT: return ZP_DISPLAY_FORMAT_RG32_SINT;
	case DXGI_FORMAT_R32G32_FLOAT: return ZP_DISPLAY_FORMAT_RG32_FLOAT;

		// RGB Component
	case DXGI_FORMAT_R32G32B32_UINT: return ZP_DISPLAY_FORMAT_RGB32_UINT;
	case DXGI_FORMAT_R32G32B32_SINT: return ZP_DISPLAY_FORMAT_RGB32_SINT;
	case DXGI_FORMAT_R32G32B32_FLOAT: return ZP_DISPLAY_FORMAT_RGB32_FLOAT;

		// RGBA Components
	case DXGI_FORMAT_R8G8B8A8_UINT: return ZP_DISPLAY_FORMAT_RGBA8_UINT;
	case DXGI_FORMAT_R8G8B8A8_SINT: return ZP_DISPLAY_FORMAT_RGBA8_SINT;
	case DXGI_FORMAT_R8G8B8A8_UNORM: return ZP_DISPLAY_FORMAT_RGBA8_UNORM;
	case DXGI_FORMAT_R8G8B8A8_SNORM: return ZP_DISPLAY_FORMAT_RGBA8_SNORM;

	case DXGI_FORMAT_R16G16B16A16_UINT: return ZP_DISPLAY_FORMAT_RGBA16_UINT;
	case DXGI_FORMAT_R16G16B16A16_SINT: return ZP_DISPLAY_FORMAT_RGBA16_SINT;
	case DXGI_FORMAT_R16G16B16A16_UNORM: return ZP_DISPLAY_FORMAT_RGBA16_UNORM;
	case DXGI_FORMAT_R16G16B16A16_SNORM: return ZP_DISPLAY_FORMAT_RGBA16_SNORM;
	case DXGI_FORMAT_R16G16B16A16_FLOAT: return ZP_DISPLAY_FORMAT_RGBA16_FLOAT;

	case DXGI_FORMAT_R32G32B32A32_UINT: return ZP_DISPLAY_FORMAT_RGBA32_UINT;
	case DXGI_FORMAT_R32G32B32A32_SINT: return ZP_DISPLAY_FORMAT_RGBA32_SINT;
	case DXGI_FORMAT_R32G32B32A32_FLOAT: return ZP_DISPLAY_FORMAT_RGBA32_FLOAT;

		// Depth Buffer
	case DXGI_FORMAT_D24_UNORM_S8_UINT: return ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT;
	case DXGI_FORMAT_D32_FLOAT: return ZP_DISPLAY_FORMAT_D32_FLOAT;

		// Compressed
	case DXGI_FORMAT_BC1_UNORM: return ZP_DISPLAY_FORMAT_BC1;
	case DXGI_FORMAT_BC3_UNORM: return ZP_DISPLAY_FORMAT_BC2;
	case DXGI_FORMAT_BC5_UNORM: return ZP_DISPLAY_FORMAT_BC3;

	default: return ZP_DISPLAY_FORMAT_UNKNOWN;
	}
}


// zpTopology <-> D3D11_PRIMITIVE_TOPOLOGY
ZP_FORCE_INLINE D3D11_PRIMITIVE_TOPOLOGY __zpToDX( zpTopology t ) {
	switch( t ) {
	case ZP_TOPOLOGY_POINT_LIST: return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	case ZP_TOPOLOGY_LINE_LIST: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case ZP_TOPOLOGY_LINE_STRIP: return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

	case ZP_TOPOLOGY_TRIANGLE_LIST: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case ZP_TOPOLOGY_TRIANGLE_STRIP: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	default: return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}
}

ZP_FORCE_INLINE zpTopology __dxToZP( D3D11_PRIMITIVE_TOPOLOGY t ) {
	switch( t ) {
	case D3D11_PRIMITIVE_TOPOLOGY_POINTLIST: return ZP_TOPOLOGY_POINT_LIST;

	case D3D11_PRIMITIVE_TOPOLOGY_LINELIST: return ZP_TOPOLOGY_LINE_LIST;
	case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP: return ZP_TOPOLOGY_LINE_STRIP;

	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST: return ZP_TOPOLOGY_TRIANGLE_LIST;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP: return ZP_TOPOLOGY_TRIANGLE_STRIP;

	default: return ZP_TOPOLOGY_UNKNOWN;
	}
}

// zpComparisonFunc <-> D3D11_COMPAIRSON_FUNC
ZP_FORCE_INLINE D3D11_COMPARISON_FUNC __zpToDX( zpComparisonFunc t ) {
	switch( t ) {
	case ZP_COMPARISON_FUNC_NEVER:			return D3D11_COMPARISON_NEVER;
	case ZP_COMPARISON_FUNC_LESS:			return D3D11_COMPARISON_LESS;
	case ZP_COMPARISON_FUNC_EQUAL:			return D3D11_COMPARISON_EQUAL;
	case ZP_COMPARISON_FUNC_LESS_EQUAL:		return D3D11_COMPARISON_LESS_EQUAL;
	case ZP_COMPARISON_FUNC_GREATER:		return D3D11_COMPARISON_GREATER;
	case ZP_COMPARISON_FUNC_NOT_EQUAL:		return D3D11_COMPARISON_NOT_EQUAL;
	case ZP_COMPARISON_FUNC_GREATER_EQUAL:	return D3D11_COMPARISON_GREATER_EQUAL;
	case ZP_COMPARISON_FUNC_ALWAYS:			return D3D11_COMPARISON_ALWAYS;
	default: return D3D11_COMPARISON_NEVER;
	}
}

// zpTextureWrap <-> D3D11_TEXTURE_ADDRESS_MODE
ZP_FORCE_INLINE D3D11_TEXTURE_ADDRESS_MODE __zpToDX( zpTextureWrap t ) {
	switch( t ) {
	case ZP_TEXTURE_WRAP_REPEAT:		return D3D11_TEXTURE_ADDRESS_WRAP;
	case ZP_TEXTURE_WRAP_MIRROR:		return D3D11_TEXTURE_ADDRESS_MIRROR;
	case ZP_TEXTURE_WRAP_CLAMP:			return D3D11_TEXTURE_ADDRESS_CLAMP;
	case ZP_TEXTURE_WRAP_BORDER:		return D3D11_TEXTURE_ADDRESS_BORDER;
	case ZP_TEXTURE_WRAP_MIRROR_ONCE:	return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	default: return D3D11_TEXTURE_ADDRESS_WRAP;
	}
}

ZP_FORCE_INLINE D3D11_CULL_MODE __zpToDX( zpCullMode t ) {
	switch( t ) {
	case ZP_CULL_MODE_NONE:		return D3D11_CULL_NONE;
	case ZP_CULL_MODE_FRONT:	return D3D11_CULL_FRONT;
	case ZP_CULL_MODE_BACK:		return D3D11_CULL_BACK;
	default: return D3D11_CULL_FRONT;
	}
}

ZP_FORCE_INLINE D3D11_FILL_MODE __zpToDX( zpFillMode t ) {
	switch( t ) {
	case ZP_FILL_MODE_SOLID:		return D3D11_FILL_SOLID;
	case ZP_FILL_MODE_WIREFRAME:	return D3D11_FILL_WIREFRAME;
	default: return D3D11_FILL_SOLID;
	}
}

ZP_FORCE_INLINE zp_uint __zpToDX( zpCpuAccess t ) {
	switch( t ) {
	case ZP_CPU_ACCESS_NONE:		return 0;
	case ZP_CPU_ACCESS_READ:		return D3D11_CPU_ACCESS_READ;
	case ZP_CPU_ACCESS_WRITE:		return D3D11_CPU_ACCESS_WRITE;
	case ZP_CPU_ACCESS_READ_WRITE:	return D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	default:						return 0;
	}
}

// zpTextureDimension <-> D3D11_RESOURCE_DIMENSION
ZP_FORCE_INLINE D3D11_RESOURCE_DIMENSION __zpToDX( zpTextureDimension t )
{
	switch( t )
	{
	case ZP_TEXTURE_DIMENSION_1D:	return D3D11_RESOURCE_DIMENSION_TEXTURE1D;
	case ZP_TEXTURE_DIMENSION_2D:	return D3D11_RESOURCE_DIMENSION_TEXTURE2D;
	case ZP_TEXTURE_DIMENSION_3D:	return D3D11_RESOURCE_DIMENSION_TEXTURE3D;
	default:						return D3D11_RESOURCE_DIMENSION_UNKNOWN;
	}
}

ZP_FORCE_INLINE zpTextureDimension __dxToZP( D3D11_RESOURCE_DIMENSION t )
{
	switch( t )
	{
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:	return ZP_TEXTURE_DIMENSION_1D;
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:	return ZP_TEXTURE_DIMENSION_2D;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:	return ZP_TEXTURE_DIMENSION_3D;
		case D3D11_RESOURCE_DIMENSION_UNKNOWN:		return ZP_TEXTURE_DIMENSION_UNKNOWN;
		default:									return ZP_TEXTURE_DIMENSION_UNKNOWN;
	}
}

// zpStencilOp <-> D3D11_STENCILOP
ZP_FORCE_INLINE D3D11_STENCIL_OP __zpToDX( zpStencilOp t )
{
	switch( t )
	{
		case ZP_STENCIL_OP_KEEP:		return D3D11_STENCIL_OP_KEEP;
		case ZP_STENCIL_OP_ZERO:		return D3D11_STENCIL_OP_ZERO;
		case ZP_STENCIL_OP_REPLACE:		return D3D11_STENCIL_OP_REPLACE;
		case ZP_STENCIL_OP_INCR_SAT:	return D3D11_STENCIL_OP_INCR_SAT;
		case ZP_STENCIL_OP_DECR_SAT:	return D3D11_STENCIL_OP_DECR_SAT;
		case ZP_STENCIL_OP_INVERT:		return D3D11_STENCIL_OP_INVERT;
		case ZP_STENCIL_OP_INCR:		return D3D11_STENCIL_OP_INCR;
		case ZP_STENCIL_OP_DECR:		return D3D11_STENCIL_OP_DECR;
		default:						return D3D11_STENCIL_OP_KEEP;
	}
}

ZP_FORCE_INLINE D3D11_BLEND __zpToDX( zpBlend t )
{
	switch( t )
	{
		case ZP_BLEND_ZERO:					return D3D11_BLEND_ZERO;
		case ZP_BLEND_ONE:					return D3D11_BLEND_ONE;
		case ZP_BLEND_SRC_COLOR:			return D3D11_BLEND_SRC_COLOR;
		case ZP_BLEND_INV_SRC_COLOR:		return D3D11_BLEND_INV_SRC_COLOR;
		case ZP_BLEND_SRC_ALPHA:			return D3D11_BLEND_SRC_ALPHA;
		case ZP_BLEND_INV_SRC_ALPHA:		return D3D11_BLEND_INV_SRC_ALPHA;
		case ZP_BLEND_DEST_ALPHA:			return D3D11_BLEND_DEST_ALPHA;
		case ZP_BLEND_INV_DEST_ALPHA:		return D3D11_BLEND_INV_DEST_ALPHA;
		case ZP_BLEND_DEST_COLOR:			return D3D11_BLEND_DEST_COLOR;
		case ZP_BLEND_INV_DEST_COLOR:		return D3D11_BLEND_INV_DEST_COLOR;
		case ZP_BLEND_SRC_ALPHA_SAT:		return D3D11_BLEND_SRC_ALPHA_SAT;
		case ZP_BLEND_BLEND_FACTOR:			return D3D11_BLEND_BLEND_FACTOR;
		case ZP_BLEND_INV_BLEND_FACTOR:		return D3D11_BLEND_INV_BLEND_FACTOR;
		case ZP_BLEND_SRC1_COLOR:			return D3D11_BLEND_SRC1_COLOR;
		case ZP_BLEND_INV_SRC1_COLOR:		return D3D11_BLEND_INV_SRC1_COLOR;
		case ZP_BLEND_SRC1_ALPHA:			return D3D11_BLEND_SRC1_ALPHA;
		case ZP_BLEND_INV_SRC1_ALPHA:		return D3D11_BLEND_INV_SRC1_ALPHA;
		default:							return D3D11_BLEND_ZERO;
	}
}

ZP_FORCE_INLINE D3D11_BLEND_OP __zpToDX( zpBlendOp t )
{
	switch( t )
	{
		case ZP_BLEND_OP_ADD:				return D3D11_BLEND_OP_ADD;
		case ZP_BLEND_OP_SUBTRACT:			return D3D11_BLEND_OP_SUBTRACT;
		case ZP_BLEND_OP_REV_SUBTRACT:		return D3D11_BLEND_OP_REV_SUBTRACT;
		case ZP_BLEND_OP_MIN:				return D3D11_BLEND_OP_MIN;
		case ZP_BLEND_OP_MAX:				return D3D11_BLEND_OP_MAX;
		default:							return D3D11_BLEND_OP_ADD;
	}
}

#endif
