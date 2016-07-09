#include "zpRendering.h"
#include "zpRenderingImpl.inl"

zpDepthStencilState::zpDepthStencilState()
    : m_depthStencilState( ZP_NULL )
{}
zpDepthStencilState::~zpDepthStencilState()
{
    //ZP_SAFE_DELETE( m_DepthStencilState );
}

zpDepthStencilStateImpl* zpDepthStencilState::getDepthStencilStateImpl() const
{
    return m_depthStencilState;
}
