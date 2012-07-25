#pragma once
#ifndef ZP_DX11_H
#define ZP_DX11_H

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dxguid.lib" )

#if ZP_DEBUG
#pragma comment( lib, "d3dx11d.lib" )
#else
#pragma comment( lib, "d3dx11.lib" )
#endif

class zpDX11ShaderResource;
class zpDX11TextureResource;

class zpDX11Buffer;
class zpDX11RenderTarget;
class zpDX11DepthStencilBuffer;
class zpDX11VertexLayout;

class zpDX11RenderingContext;
class zpDX11RenderingEngine;

#include "zpDX11ShaderResource.h"
#include "zpDX11TextureResource.h"

#include "zpDX11Buffer.h"
#include "zpDX11RenderTarget.h"
#include "zpDX11DepthStencilBuffer.h"
#include "zpDX11VertexLayout.h"

#include "zpDX11RenderingContext.h"
#include "zpDX11RenderingEngine.h"

#endif