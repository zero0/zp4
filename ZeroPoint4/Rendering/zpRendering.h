#pragma once
#ifndef ZP_RENDERING_H
#define ZP_RENDERING_H

#include "Core\zpCore.h"
#include "Content\zpContent.h"

#if ZP_WIN_32 || ZP_WIN_64
#define ZP_DIRECTX_SUPPORT		1
#endif

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

enum zpRenderingDisplayFormat {
	ZP_RENDERING_DISPLAY_FORMAT_UNKNOWN	=		0,

	ZP_RENDERING_DISPLAY_FORMAT_RGBA8_UNORM,
	ZP_RENDERING_DISPLAY_FORMAT_RGBA8_SNORM,

	ZP_RENDERING_DISPLAY_FORMAT_R32_UINT,
	ZP_RENDERING_DISPLAY_FORMAT_R32_SINT,
	ZP_RENDERING_DISPLAY_FORMAT_R32_FLOAT,
};

enum zpRenderingScreenMode {
	ZP_RENDERING_SCREEN_MODE_FULLSCREEN,
	ZP_RENDERING_SCREEN_MODE_WINDOWED,
	ZP_RENDERING_SCREEN_MODE_FULLSCREEN_WINDOWED,
};

struct zpRenderingDisplayMode {
	zp_uint width;
	zp_uint height;
	zp_uint refreshRate;
	zpRenderingDisplayFormat displayFormat;
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

enum zpBufferBind {
	ZP_BUFFER_BIND_DEFAULT,
	ZP_BUFFER_BIND_DYNAMIC,
	ZP_BUFFER_BIND_IMMUTABLE,
};

enum zpRenderingMapType {
	ZP_RENDERING_MAP_TYPE_READ,
	ZP_RENDERING_MAP_TYPE_WRITE,
	ZP_RENDERING_MAP_TYPE_READ_WRITE,
	ZP_RENDERING_MAP_TYPE_WRITE_DISCARD,

};

ZP_PURE_INTERFACE zpShaderResource;
ZP_PURE_INTERFACE zpTextureResource;
class zpRenderingResourceCreator;
ZP_PURE_INTERFACE zpBuffer;
ZP_PURE_INTERFACE zpRenderingContext;
ZP_PURE_INTERFACE zpRenderingEngine;
class zpRenderingFactory;

class zpCamera;
class zpRenderingManager;

#include "zpShaderResource.h"
#include "zpTextureResource.h"
#include "zpRenderingResourceCreator.h"
#include "zpBuffer.h"
#include "zpRenderingContext.h"
#include "zpRenderingEngine.h"
#include "zpRenderingFactory.h"

#include "zpCamera.h"
#include "zpRenderingManager.h"

#endif