#pragma once
#ifndef ZP_RENDERING_H
#define ZP_RENDERING_H

#include "Core\zpCore.h"

#if ZP_WIN_32 || ZP_WIN_64
#define ZP_DIRECTX_SUPPORT		1
#endif

enum zpRenderingEngineType {
	ZP_RENDERING_ENGINE_NONE =			0,

	ZP_RENDERING_ENGINE_OPENGL =		0x1000,
	ZP_RENDERING_ENGINE_OPENGL_4,
	ZP_RENDERING_ENGINE_OPENGL_3,
	ZP_RENDERING_ENGINE_OPENGL_2,
	ZP_RENDERING_ENGINE_OPENGL_1_1,
	ZP_RENDERING_ENGINE_OPENGL_1,
	
	ZP_RENDERING_ENGINE_DX =			0x2000,
	ZP_RENDERING_ENGINE_DX11,
	ZP_RENDERING_ENGINE_DX10_1,
	ZP_RENDERING_ENGINE_DX10,
	ZP_RENDERING_ENGINE_DX9_1,
	ZP_RENDERING_ENGINE_DX9,
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

#include "zpRenderingEngine.h"
#include "zpRenderingFactory.h"

#include "zpBuffer.h"
#include "zpCamera.h"

#include "zpDX11RenderingEngine.h"
#include "zpRenderingManager.h"

#endif