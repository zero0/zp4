#pragma once
#ifndef ZP_RENDERING_H
#define ZP_RENDERING_H

#include "Core\zpCore.h"
#include "Content\zpContent.h"

#if ZP_WIN_32 || ZP_WIN_64
#define ZP_DIRECTX_SUPPORT				1
#endif

#define ZP_RENDER_TARGET_MAX_COUNT		8

#define ZP_DX11							0xd011
#define ZP_DX11_INC						"DX11"

#define ZP_GL2							0x9l20
#define ZP_GL2_INC						"GL2"

#define ZP_RENDERING_TYPE				ZP_DX11
#define ZP_RENDERING_INCLUDE			ZP_DX11_INC

enum
{
	ZP_RENDERING_GLOBAL_BUFFER_WORLD,
	ZP_RENDERING_GLOBAL_BUFFER_CAMERA,
	ZP_RENDERING_GLOBAL_BUFFER_LIGHT,

	ZP_RENDERING_GLOBAL_BUFFER_Count
};

enum
{
	ZP_RENDERING_MAX_COMMNADS =					1024,
	ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS	=	2,
	ZP_RENDERING_MAX_RENDERING_CONTEXTS =		8,
	ZP_RENDERING_MAX_RASTER_STATES =			16,
	ZP_RENDERING_MAX_SAMPLER_STATES =			32,

	ZP_RENDERING_IMMEDIATE_SCRATCH_VERTEX_BUFFER_SIZE = ZP_MEMORY_MB( 0.5f ),
	ZP_RENDERING_IMMEDIATE_SCRATCH_INDEX_BUFFER_SIZE =	ZP_MEMORY_MB( 0.25f ),

	ZP_RENDERING_IMMEDIATE_VERTEX_BUFFER_SIZE = ZP_MEMORY_MB( 2 ),
	ZP_RENDERING_IMMEDIATE_INDEX_BUFFER_SIZE =	ZP_MEMORY_MB( 1 ),
};

#if ZP_RENDERING_TYPE == ZP_DX11
#include "RenderingDX\zpDX11Lib.inc"
#elif ZP_RENDERING_TYPE == ZP_GL2
#include "RenderingOpenGL\zpOpenGLLib.inc"
#else
#error( "No rendering engine selected!" )
#endif

enum zpRenderingLayer : zp_byte
{
	ZP_RENDERING_LAYER_NONE,

	ZP_RENDERING_LAYER_OPAQUE,
	ZP_RENDERING_LAYER_TRANSPARENT,

	ZP_RENDERING_LAYER_UI_OPAQUE,
	ZP_RENDERING_LAYER_UI_TRANSPARENT,

	zpRenderingLayer_Count,
};

enum zpRenderingEngineType
{
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

enum zpDisplayFormat : zp_uint
{
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

	// Compressed
	ZP_DISPLAY_FORMAT_DXT1,
	ZP_DISPLAY_FORMAT_DXT3,
	ZP_DISPLAY_FORMAT_DXT5,
	ZP_DISPLAY_FORMAT_ATI1N,
	ZP_DISPLAY_FORMAT_ATI2N,

	zpDisplayFormat_Count,
	zpDisplayFormat_Force32 = ZP_FORECE_32BIT,

};

enum zpVertexFormat
{
	ZP_VERTEX_FORMAT_VERTEX_COLOR,			
	ZP_VERTEX_FORMAT_VERTEX_UV,	
	ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV,		
	ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2,

	zpVertexFormat_Count,
};
enum zpVertexFormatDesc : zp_uint
{
	ZP_VERTEX_FORMAT_DESC_VERTEX_COLOR =		ZP_MAKE_UINT( 'V', 'C', '\0', '\0' ),
	ZP_VERTEX_FORMAT_DESC_VERTEX_UV =			ZP_MAKE_UINT( 'V', 'U', '\0', '\0' ),
	ZP_VERTEX_FORMAT_DESC_VERTEX_NORMAL_UV =	ZP_MAKE_UINT( 'V', 'N', 'U', '\0' ),
	ZP_VERTEX_FORMAT_DESC_VERTEX_NORMAL_UV2 =	ZP_MAKE_UINT( 'V', 'N', 'W', '\0' ),
};

enum zpScreenMode
{
	ZP_SCREEN_MODE_FULLSCREEN,
	ZP_SCREEN_MODE_WINDOWED,
	ZP_SCREEN_MODE_FULLSCREEN_WINDOWED,
};

struct zpDisplayMode
{
	zp_uint width;
	zp_uint height;
	zp_uint refreshRate;
	zpDisplayFormat displayFormat;
};

enum zpBufferType
{
	ZP_BUFFER_TYPE_UNKNOWN =	0,
	ZP_BUFFER_TYPE_VERTEX,
	ZP_BUFFER_TYPE_INDEX,
	ZP_BUFFER_TYPE_CONSTANT,
	ZP_BUFFER_TYPE_SHADER_RESOURCE,
	ZP_BUFFER_TYPE_STREAM_OUT,
	ZP_BUFFER_TYPE_RENDER_TARGET,
	ZP_BUFFER_TYPE_DEPTH_STENCIL,
	ZP_BUFFER_TYPE_UNORDERED_ACCESS,
};

enum zpBufferBindType
{
	ZP_BUFFER_BIND_DEFAULT,
	ZP_BUFFER_BIND_DYNAMIC,
	ZP_BUFFER_BIND_IMMUTABLE,
};

enum zpMapType
{
	ZP_MAP_TYPE_READ,
	ZP_MAP_TYPE_WRITE,
	ZP_MAP_TYPE_READ_WRITE,
	ZP_MAP_TYPE_WRITE_DISCARD,
};

enum zpTopology
{
	ZP_TOPOLOGY_UNKNOWN =		0,

	ZP_TOPOLOGY_POINT_LIST,

	ZP_TOPOLOGY_LINE_LIST,
	ZP_TOPOLOGY_LINE_STRIP,

	ZP_TOPOLOGY_TRIANGLE_LIST,
	ZP_TOPOLOGY_TRIANGLE_STRIP,
};

enum zpResourceBindSlot
{
	ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER =	0x01,
	ZP_RESOURCE_BIND_SLOT_GEOMETRY_SHADER =	0x02,
	ZP_RESOURCE_BIND_SLOT_COMPUTE_SHADER =	0x04,
	ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER =	0x08,
};

enum zpMaterialTextureSlot
{
	ZP_MATERIAL_TEXTURE_SLOT_DIFFUSE =		0,
	ZP_MATERIAL_TEXTURE_SLOT_NORMAL,
	ZP_MATERIAL_TEXTURE_SLOT_SPECULAR,
	ZP_MATERIAL_TEXTURE_SLOT_OPACITY,
	ZP_MATERIAL_TEXTURE_SLOT_OTHER,

	zpMaterialTextureSlot_Count
};

enum zpTextureWrap
{
	ZP_TEXTURE_WRAP_REPEAT =		1,
	ZP_TEXTURE_WRAP_MIRROR,
	ZP_TEXTURE_WRAP_CLAMP,
	ZP_TEXTURE_WRAP_BORDER,
};

enum zpTextureFilter
{
	ZP_TEXTURE_FILTER_POINT =					0x01,
	ZP_TEXTURE_FILTER_LINEAR =					0x02,
	ZP_TEXTURE_FILTER_ANISOTROPIC =				0x04,

	ZP_TEXTURE_FILTER_COMPARISON_POINT =		0x11,
	ZP_TEXTURE_FILTER_COMPARISON_LINEAR =		0x12,
	ZP_TEXTURE_FILTER_COMPARISON_ANISOTROPIC =	0x14,
};

enum zpComparisonFunc
{
	ZP_COMPARISON_FUNC_NONE =		0,
	ZP_COMPARISON_FUNC_LESS,
	ZP_COMPARISON_FUNC_LESS_EQUAL,
	ZP_COMPARISON_FUNC_EQUAL,
	ZP_COMPARISON_FUNC_NOT_EQUAL,
	ZP_COMPARISON_FUNC_GREATER_EQUAL,
	ZP_COMPARISON_FUNC_GREATER,
	ZP_COMPARISON_FUNC_ALWAYS,
};

enum zpCullMode
{
	ZP_CULL_MODE_NONE =		0,
	ZP_CULL_MODE_FRONT,
	ZP_CULL_MODE_BACK,
};

enum zpFillMode
{
	ZP_FILL_MODE_SOLID =	0,
	ZP_FILL_MODE_WIREFRAME,
};

enum zpFrontFace
{
	ZP_FRONT_FACE_CW =		0,
	ZP_FRONT_FACE_CCW,
};

enum zpCpuAccess
{
	ZP_CPU_ACCESS_NONE =	0,
	ZP_CPU_ACCESS_READ,
	ZP_CPU_ACCESS_WRITE,
	ZP_CPU_ACCESS_READ_WRITE,
};

struct zpSamplerStateDesc
{
	struct
	{
		zpComparisonFunc cmpFunc	: 8;
		zpTextureFilter minFilter	: 8;
		zpTextureFilter magFilter	: 8;
		zpTextureFilter mipFilter	: 8;
		zpTextureWrap texWrapU		: 8;
		zpTextureWrap texWrapV		: 8;
		zpTextureWrap texWrapW		: 8;
		zp_byte maxAnisotrpy		: 8;
	};

	zp_float lodMin;
	zp_float lodMax;
	zp_float lodBias;

	zpColor4f borderColor;

	zpSamplerStateDesc();
};

struct zpRasterStateDesc
{
	struct
	{
		zpCullMode cullMode				: 2;
		zpFillMode fillMode				: 1;
		zpFrontFace frontFace			: 1;
		zp_bool depthClipEnable			: 1;
		zp_bool scissorEnable			: 1;
		zp_bool multisampleEnable		: 1;
		zp_bool antialiasedLineEnable	: 1;
	};

	zp_int depthBias;
	zp_float depthBiasClamp;
	zp_float slopeScaledDepthBias;

	zpRasterStateDesc();
};

struct zpViewport
{
	zp_float width;
	zp_float height;
	zp_float minDepth;
	zp_float maxDepth;
	zp_float topX;
	zp_float topY;

	zpViewport();
};

class zpRenderingResourceCreator;

class zpBuffer;
class zpBufferImpl;
class zpDepthStencilBuffer;
ZP_PURE_INTERFACE zpVertexLayout;
class zpSamplerState;
class zpRasterState;

class zpShader;
class zpTexture;
class zpMaterialResource;

class zpFontResource;
template<> class zpResourceInstance<zpFontResource>;

ZP_ABSTRACT_CLASS zpStaticMeshResource;
class zpOBJStaticMeshResource;
template<> class zpResourceInstance<zpStaticMeshResource>;


enum zpRenderingCommandType
{
	ZP_RENDERING_COMMNAD_NOOP,

	ZP_RENDERING_COMMNAD_CLEAR_RT,
	ZP_RENDERING_COMMNAD_CLEAR_DEPTH_STENCIL,
	ZP_RENDERING_COMMNAD_CLEAR_STATE,

	ZP_RENDERING_COMMNAD_SET_RT,
	ZP_RENDERING_COMMNAD_SET_VIEWPORT,
	ZP_RENDERING_COMMNAD_SET_SCISSOR_RECT,
	ZP_RENDERING_COMMNAD_SET_RASTER_STATE,
	ZP_RENDERING_COMMNAD_SET_SAMPLER_STATE,

	ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE,
	ZP_RENDERING_COMMNAD_DRAW_BUFFERED,
	ZP_RENDERING_COMMNAD_DRAW_INSTANCED,

	zpRenderingCommandType_Count,
};

class zpRenderingContext;
class zpRenderingEngine;
class zpRenderingFactory;

class zpCamera;
class zpRenderingManager;
class zpUIManager;

template<typename V, typename I> class zpImmediateBuffer;

ZP_ABSTRACT_CLASS zpRenderingComponent;
class zpCameraComponent;
class zpLightComponent;
class zpStaticMeshRenderingComponent;
class zpTextRenderingComponent;
class zpUIRenderingComponent;

class zpDeferredRenderingComponent;

class zpMaterialResourceInstance;

struct zpRenderingCommand
{
	zpRenderingCommandType type;
	zpRenderingLayer layer;
	zp_uint sortKey;

	union
	{
		struct
		{
			zpColor4f clearColor;
			zpTexture* clearRenderTarget;
		};

		struct 
		{
			zp_float clearDepth;
			zp_uint clearStencil;
			zpDepthStencilBuffer* clearDepthStencilBuffer;
		};

		struct
		{
			zpTexture* renderTargets[ ZP_RENDER_TARGET_MAX_COUNT ];
			zpDepthStencilBuffer* depthStencilBuffer;
		};

		struct
		{
			zpViewport viewport;
		};

		struct
		{
			zpRecti scissor;
		};

		struct
		{
			zpRasterState* rasterState;
		};

		struct 
		{
			zp_uint samplerStateBind;
			zp_uint samplerIndex;
			zpSamplerState* samplerState;
		};

		struct
		{
			zpTopology topology;
			zpBufferImpl* vertexBuffer;
			zpBufferImpl* indexBuffer;
			zpMaterialResourceInstance* material;
			zpVertexFormat vertexFormat;
			zp_uint vertexStride;
			zp_uint vertexCount;
			zp_uint indexCount;
			zp_uint vertexOffset;
			zp_uint indexOffset;
			zpBoundingAABB boundingBox;
		};
	};

	zp_uint generateSortKey() const;
};

#include "zpImage.h"
#include "zpBufferData.h"

#include "zpVertex.h"
#include "zpViewport.h"
#include "zpTexture.h"

#include "zpShader.h"

#include "zpUIResource.h"

#include "zpRenderingResourceCreator.h"

#include "zpBuffer.h"
#include "zpDepthStencilBuffer.h"
#include "zpVertexLayout.h"
#include "zpSamplerState.h"
#include "zpRasterState.h"

#include "zpMaterial.h"
#include "zpFontResource.h"
#include "zpStaticMeshResource.h"
#include "zpOBJStaticMeshResource.h"

#include "zpRenderingContext.h"
#include "zpRenderingEngine.h"
#include "zpRenderingFactory.h"
#include "zpRenderingPipeline.h"

#include "zpCamera.h"
#include "zpRenderingManager.h"
#include "zpUIManager.h"

#include "zpImmediateBuffer.h"

#include "zpRenderingComponent.h"
#include "zpCameraComponent.h"
#include "zpLightComponent.h"
#include "zpStaticMeshRenderingComponent.h"
#include "zpTextRenderingComponent.h"
#include "zpUIRenderingComponent.h"

#include "zpDeferredRenderingComponent.h"
//
//typedef zpPooledObject<zpUIRenderingComponent, 32> zpUIRenderingComponentPool;
//typedef zpPooledObject<zpCameraComponent, 32> zpCameraComponentPool;
//typedef zpPooledObject<zpLightComponent, 128> zpLightComponentPool;
//typedef zpPooledObject<zpStaticMeshRenderingComponent, 128> zpStaticMeshRenderingComponentPool;
//
void zpRenderingRegisterSerializables();

#endif