#include "zpRendering.h"
#include "zpRenderingImpl.inl"

zpRasterState::zpRasterState()
    : m_rasterState( ZP_NULL )
{}
zpRasterState::~zpRasterState()
{
    //ZP_SAFE_DELETE( m_rasterState );
}

zpRasterStateImpl* zpRasterState::getRasterStateImpl() const
{
    return m_rasterState;
}
