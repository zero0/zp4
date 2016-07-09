#include "zpRendering.h"
#include "zpRenderingImpl.inl"

zpBlendState::zpBlendState()
    : m_blendState( ZP_NULL )
{}
zpBlendState::~zpBlendState()
{
    //ZP_SAFE_DELETE( m_blendState );
}

zpBlendStateImpl* zpBlendState::getBlendStateImpl() const
{
    return m_blendState;
}
