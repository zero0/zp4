#pragma once
#ifndef ZP_SAMPLER_STATE_H
#define ZP_SAMPLER_STATE_H

class zpSamplerStateImpl;

class zpSamplerState
{
public:
    zpSamplerState();
    ~zpSamplerState();

    zpSamplerStateImpl* getSamplerStateImpl() const;

private:
    zpSamplerStateImpl* m_samplerState;

    friend class zpRenderingEngine;
};

#endif