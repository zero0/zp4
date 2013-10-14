#pragma once
#ifndef ZP_DX11_DEPTH_STENCIL_STATE_H
#define ZP_DX11_DEPTH_STENCIL_STATE_H

struct ID3D11DepthStencilState;

class zpDepthStencilStateImpl
{
public:
	zpDepthStencilStateImpl();
	~zpDepthStencilStateImpl();

private:
	ID3D11DepthStencilState* m_depthStencil;
	zp_hash m_descHash;
	zpDepthStencilStateDesc m_desc;

	friend class zpRenderingEngineImpl;
	friend class zpRenderingContextImpl;
};

#if 0
class zpDX11DepthStencilState : public zpDepthStencilState {
public:
	virtual ~zpDX11DepthStencilState();

private:
	zpDX11DepthStencilState( ID3D11DepthStencilState* state = ZP_NULL );

	ID3D11DepthStencilState* m_DepthStencil;

	friend class zpDX11RenderingEngine;
	friend class zpDX11RenderingContext;
};
#endif

#endif
