#pragma once
#ifndef ZP_RENDERING_H
#define ZP_RENDERING_H

#include "Core\zpCore.h"
#include "Content\zpContent.h"

#if ZP_WIN_32 || ZP_WIN_64
#define ZP_DIRECTX_SUPPORT		1
#endif

#define ZP_RENDER_TARGET_MAX_COUNT		8

enum zpRenderingEngineType {
	ZP_RENDERING_ENGINE_NONE =			0,

	ZP_RENDERING_ENGINE_OPENGL =		0x1000,
	ZP_RENDERING_ENGINE_OPENGL_1,
	ZP_RENDERING_ENGINE_OPENGL_1_1,
	ZP_RENDERING_ENGINE_OPENGL_2,
	ZP_RENDERING_ENGINE_OPENGL_3,
	ZP_RENDERING_ENGINE_OPENGL_4,
	
	ZP_RENDERING_ENGINE_DX =			0x2000,
	ZP_RENDERING_ENGINE_DX9,
	ZP_RENDERING_ENGINE_DX9_1,
	ZP_RENDERING_ENGINE_DX10,
	ZP_RENDERING_ENGINE_DX10_1,
	ZP_RENDERING_ENGINE_DX11,
};

enum zpDisplayFormat {
	ZP_DISPLAY_FORMAT_UNKNOWN	=		0,

	// R Component
	ZP_DISPLAY_FORMAT_R8_UINT,
	ZP_DISPLAY_FORMAT_R8_SINT,

	ZP_DISPLAY_FORMAT_R16_UINT,
	ZP_DISPLAY_FORMAT_R16_SINT,
	ZP_DISPLAY_FORMAT_R16_FLOAT,

	ZP_DISPLAY_FORMAT_R32_UINT,
	ZP_DISPLAY_FORMAT_R32_SINT,
	ZP_DISPLAY_FORMAT_R32_FLOAT,

	// RG Components
	ZP_DISPLAY_FORMAT_RG8_UINT,
	ZP_DISPLAY_FORMAT_RG8_SINT,

	ZP_DISPLAY_FORMAT_RG16_UINT,
	ZP_DISPLAY_FORMAT_RG16_SINT,
	ZP_DISPLAY_FORMAT_RG16_FLOAT,

	ZP_DISPLAY_FORMAT_RG32_UINT,
	ZP_DISPLAY_FORMAT_RG32_SINT,
	ZP_DISPLAY_FORMAT_RG32_FLOAT,

	// RGB Components
	ZP_DISPLAY_FORMAT_RGB32_UINT,
	ZP_DISPLAY_FORMAT_RGB32_SINT,
	ZP_DISPLAY_FORMAT_RGB32_FLOAT,

	// RGBA Components
	ZP_DISPLAY_FORMAT_RGBA8_UINT,
	ZP_DISPLAY_FORMAT_RGBA8_SINT,
	ZP_DISPLAY_FORMAT_RGBA8_UNORM,
	ZP_DISPLAY_FORMAT_RGBA8_SNORM,

	ZP_DISPLAY_FORMAT_RGBA16_UINT,
	ZP_DISPLAY_FORMAT_RGBA16_SINT,
	ZP_DISPLAY_FORMAT_RGBA16_UNORM,
	ZP_DISPLAY_FORMAT_RGBA16_SNORM,
	ZP_DISPLAY_FORMAT_RGBA16_FLOAT,

	ZP_DISPLAY_FORMAT_RGBA32_UINT,
	ZP_DISPLAY_FORMAT_RGBA32_SINT,
	ZP_DISPLAY_FORMAT_RGBA32_FLOAT,

	// Depth Buffer
	ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT,
	ZP_DISPLAY_FORMAT_D32_FLOAT,
};

enum zpScreenMode {
	ZP_SCREEN_MODE_FULLSCREEN,
	ZP_SCREEN_MODE_WINDOWED,
	ZP_SCREEN_MODE_FULLSCREEN_WINDOWED,
};

struct zpDisplayMode {
	zp_uint width;
	zp_uint height;
	zp_uint refreshRate;
	zpDisplayFormat displayFormat;
};

enum zpBufferType {
	ZP_BUFFER_TYPE_VERTEX,
	ZP_BUFFER_TYPE_INDEX,
	ZP_BUFFER_TYPE_CONSTANT,
	ZP_BUFFER_TYPE_SHADER_RESOURCE,
	ZP_BUFFER_TYPE_STREAM_OUT,
	ZP_BUFFER_TYPE_RENDER_TARGET,
	ZP_BUFFER_TYPE_DEPTH_STENCIL,
	ZP_BUFFER_TYPE_UNORDERED_ACCESS,
};

enum zpBufferBindType {
	ZP_BUFFER_BIND_DEFAULT,
	ZP_BUFFER_BIND_DYNAMIC,
	ZP_BUFFER_BIND_IMMUTABLE,
};

enum zpMapType {
	ZP_MAP_TYPE_READ,
	ZP_MAP_TYPE_WRITE,
	ZP_MAP_TYPE_READ_WRITE,
	ZP_MAP_TYPE_WRITE_DISCARD,
};

enum zpTopology {
	ZP_TOPOLOGY_UNKNOWN,

	ZP_TOPOLOGY_POINT_LIST,

	ZP_TOPOLOGY_LINE_LIST,
	ZP_TOPOLOGY_LINE_STRIP,

	ZP_TOPOLOGY_TRIANGLE_LIST,
	ZP_TOPOLOGY_TRIANGLE_STRIP,
};

enum zpResourceBindSlot {
	ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER =	0x01,
	ZP_RESOURCE_BIND_SLOT_GEOMETRY_SHADER =	0x02,
	ZP_RESOURCE_BIND_SLOT_COMPUTE_SHADER =	0x04,
	ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER =	0x08,
};

enum zpMeshTextureBindSlot {
	ZP_MESH_TEXTURE_BIND_SLOT_DIFFUSE =		0,
	ZP_MESH_TEXTURE_BIND_SLOT_NORMAL,
	ZP_MESH_TEXTURE_BIND_SLOT_SPECULAR,
	ZP_MESH_TEXTURE_BIND_SLOT_OPACITY,
	ZP_MESH_TEXTURE_BIND_SLOT_OTHER,
};

enum zpTextureWrap {
	ZP_TEXTURE_WRAP_REPEAT =		1,
	ZP_TEXTURE_WRAP_MIRROR,
	ZP_TEXTURE_WRAP_CLAMP,
	ZP_TEXTURE_WRAP_BORDER,
};

enum zpTextureFilter {
	ZP_TEXTURE_FILTER_POINT =					0x01,
	ZP_TEXTURE_FILTER_LINEAR =					0x02,
	ZP_TEXTURE_FILTER_ANISOTROPIC =				0x04,

	ZP_TEXTURE_FILTER_COMPARISON_POINT =		0x11,
	ZP_TEXTURE_FILTER_COMPARISON_LINEAR =		0x12,
	ZP_TEXTURE_FILTER_COMPARISON_ANISOTROPIC =	0x14,
};

enum zpComparisonFunc {
	ZP_COMPARISON_FUNC_NONE =		1,
	ZP_COMPARISON_FUNC_LESS,
	ZP_COMPARISON_FUNC_LESS_EQUAL,
	ZP_COMPARISON_FUNC_EQUAL,
	ZP_COMPARISON_FUNC_NOT_EQUAL,
	ZP_COMPARISON_FUNC_GREATER_EQUAL,
	ZP_COMPARISON_FUNC_GREATER,
	ZP_COMPARISON_FUNC_ALWAYS,
};

enum zpCullMode {
	ZP_CULL_MODE_NONE =		1,
	ZP_CULL_MODE_FRONT,
	ZP_CULL_MODE_BACK,
};

enum zpFillMode {
	ZP_FILL_MODE_SOLID =	1,
	ZP_FILL_MODE_WIREFRAME,
};

enum zpFrontFace {
	ZP_FRONT_FACE_CW =		1,
	ZP_FRONT_FACE_CCW,
};

struct zpSamplerStateDesc {
	struct {
		zpComparisonFunc cmpFunc : 8;
		zpTextureFilter minFilter : 8;
		zpTextureFilter magFilter : 8;
		zpTextureFilter mipFilter : 8;
	};

	struct {
		zpTextureWrap texWrapU : 8;
		zpTextureWrap texWrapV : 8;
		zpTextureWrap texWrapW : 8;
		zp_byte maxAnisotrpy : 8;
	};

	zp_float lodMin;
	zp_float lodMax;
	zp_float lodBias;

	zpColor4f borderColor;

	zpSamplerStateDesc();
};

struct zpRasterStateDesc {
	zpFillMode fillMode;
	zpCullMode cullMode;
	zpFrontFace frontFace;
	zp_int depthBias;
	zp_float depthBiasClamp;
	zp_float slopeScaledDepthBias;
	zp_bool depthClipEnable;
	zp_bool scissorEnable;
	zp_bool multisampleEnable;
	zp_bool antialiasedLineEnable;

	zpRasterStateDesc();
};


class zpViewport;
ZP_PURE_INTERFACE zpTexture;

ZP_PURE_INTERFACE zpShaderResource;
template<> class zpResourceInstance<zpShaderResource>;

ZP_PURE_INTERFACE zpTextureResource;
template<> class zpResourceInstance<zpTextureResource>;

class zpRenderingResourceCreator;

ZP_PURE_INTERFACE zpBuffer;
ZP_PURE_INTERFACE zpRenderTarget;
ZP_PURE_INTERFACE zpDepthStencilBuffer;
ZP_PURE_INTERFACE zpVertexLayout;
ZP_PURE_INTERFACE zpSamplerState;
ZP_PURE_INTERFACE zpRasterState;

ZP_ABSTRACT_CLASS zpStaticMeshResource;
class zpOBJStaticMeshResource;
template<> class zpResourceInstance<zpStaticMeshResource>;

ZP_PURE_INTERFACE zpRenderingContext;
ZP_PURE_INTERFACE zpRenderingEngine;
class zpRenderingFactory;

class zpCamera;
class zpRenderingManager;

class zpCameraComponent;
class zpStaticMeshRenderingComponent;

#include "zpVertex.h"
#include "zpViewport.h"
#include "zpTexture.h"

#include "zpShaderResource.h"
#include "zpShaderInstance.h"

#include "zpTextureResource.h"
#include "zpTextureInstance.h"

#include "zpRenderingResourceCreator.h"

#include "zpBuffer.h"
#include "zpRenderTarget.h"
#include "zpDepthStencilBuffer.h"
#include "zpVertexLayout.h"
#include "zpSamplerState.h"
#include "zpRasterState.h"

#include "zpStaticMeshResource.h"
#include "zpOBJStaticMeshResource.h"
#include "zpStaticMeshInstance.h"

#include "zpRenderingContext.h"
#include "zpRenderingEngine.h"
#include "zpRenderingFactory.h"

#include "zpCamera.h"
#include "zpRenderingManager.h"

#include "zpCameraComponent.h"
#include "zpStaticMeshRenderingComponent.h"

#endif