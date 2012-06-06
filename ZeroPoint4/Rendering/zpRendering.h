#pragma once
#ifndef ZP_RENDERING_H
#define ZP_RENDERING_H

#include "Core\zpCore.h"

#if ZP_WIN_32 || ZP_WIN_64
#define ZP_DIRECTX_SUPPORT		1
#endif

enum zpRenderingEngineType {
	ZP_RENDERING_ENGINE_OPENGL =		0x0001000,
	ZP_RENDERING_ENGINE_OPENGL_3,
	ZP_RENDERING_ENGINE_OPENGL_2,
	ZP_RENDERING_ENGINE_OPENGL_1,
	
	ZP_RENDERING_ENGINE_DX =			0x0002000,
	ZP_RENDERING_ENGINE_DX11,
	ZP_RENDERING_ENGINE_DX10_1,
	ZP_RENDERING_ENGINE_DX10,
	ZP_RENDERING_ENGINE_DX9_1,
	ZP_RENDERING_ENGINE_DX9,
};

#include "zpRenderingEngine.h"
#include "zpRenderingFactory.h"

#include "zpBuffer.h"

#include "zpDX11RenderingEngine.h"

#endif