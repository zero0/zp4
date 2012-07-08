#pragma once
#ifndef ZP_DX11_H
#define ZP_DX11_H

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include "zpRendering.h"

class zpDX11ShaderResource;
class zpDX11TextureResource;
class zpDX11Buffer;
class zpDX11RenderingContext;
class zpDX11RenderingEngine;

#include "zpDX11ShaderResource.h"
#include "zpDX11TextureResource.h"
#include "zpDX11Buffer.h"
#include "zpDX11RenderingContext.h"
#include "zpDX11RenderingEngine.h"

#endif