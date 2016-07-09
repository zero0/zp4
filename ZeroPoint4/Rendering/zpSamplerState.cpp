#include "zpRendering.h"

#if ZP_RENDERING_TYPE == ZP_DX11
#include "RenderingDX/zpDX11.h"
#elif ZP_RENDERING_TYPE == ZP_GL2
#include "RenderingOpenGL/zpOpenGL.h"
#else
#error( "No rendering engine selected!" )
#endif

zpSamplerState::zpSamplerState()
    : m_samplerState( ZP_NULL )
{}
zpSamplerState::~zpSamplerState()
{
    //ZP_SAFE_DELETE( m_samplerState );
}

zpSamplerStateImpl* zpSamplerState::getSamplerStateImpl() const
{
    return m_samplerState;
}
