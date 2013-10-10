#pragma once
#ifndef ZP_DX11_SAMPLER_STATE_H
#define ZP_DX11_SAMPLER_STATE_H

struct ID3D11SamplerState;

class zpSamplerStateImpl
{
public:
	zpSamplerStateImpl();
	~zpSamplerStateImpl();

private:
	ID3D11SamplerState* m_sampler;
	zp_hash m_descHash;
	zpSamplerStateDesc m_desc;

	friend class zpRenderingEngineImpl;
	friend class zpRenderingContextImpl;
};

#if 0
class zpDX11SamplerState : public zpSamplerState {
public:
	virtual ~zpDX11SamplerState();

private:
	zpDX11SamplerState( ID3D11SamplerState* state = ZP_NULL );

	ID3D11SamplerState* m_sampler;

	friend class zpDX11RenderingEngine;
	friend class zpDX11RenderingContext;
};
#endif

#endif