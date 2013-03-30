#pragma once
#ifndef ZP_SAMPLER_STATE_H
#define ZP_SAMPLER_STATE_H

class zpSamplerStateImpl;

class zpSamplerState
{
	ZP_NON_COPYABLE( zpSamplerState );
public:
	~zpSamplerState();

	zpSamplerStateImpl* getSamplerStateImpl() const;

private:
	zpSamplerState();
	zpSamplerState( zpSamplerStateImpl* samplerState, zp_hash descHash );

	zpSamplerStateImpl* m_samplerState;
	zp_hash m_descHash;

	friend class zpRenderingEngine;
};

#endif