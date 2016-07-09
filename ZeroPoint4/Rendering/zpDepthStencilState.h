#pragma once
#ifndef ZP_DEPTH_STENCIL_STATE_H
#define ZP_DEPTH_STENCIL_STATE_H

class zpDepthStencilStateImpl;

class zpDepthStencilState
{
public:
    zpDepthStencilState();
    ~zpDepthStencilState();

    zpDepthStencilStateImpl* getDepthStencilStateImpl() const;

private:
    zpDepthStencilStateImpl* m_depthStencilState;

    friend class zpRenderingEngine;
};

#endif