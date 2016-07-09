#pragma once
#ifndef ZP_DX11_BLEND_STATE_H
#define ZP_DX11_BLEND_STATE_H

struct ID3D11BlendState;

class zpBlendStateImpl
{
public:
    zpBlendStateImpl();
    ~zpBlendStateImpl();

private:
    ID3D11BlendState* m_blend;;
    zp_hash m_descHash;
    zpBlendStateDesc m_desc;

    friend class zpRenderingEngineImpl;
    friend class zpRenderingContextImpl;
};

#endif
