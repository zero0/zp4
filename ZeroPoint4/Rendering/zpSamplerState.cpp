#include "zpRendering.h"

#if ZP_RENDERING_TYPE == ZP_DX11
#include "RenderingDX/zpDX11.h"
#elif ZP_RENDERING_TYPE == ZP_GL2
#include "RenderingOpenGL/zpOpenGL.h"
#else
#error( "No rendering engine selected!" )
#endif

zpSamplerState::zpSamplerState( zpSamplerStateImpl* samplerState, zp_hash descHash )
	: m_samplerState( samplerState )
	, m_descHash( descHash )
{}
zpSamplerState::~zpSamplerState()
{
	ZP_SAFE_DELETE( m_samplerState );
}

zpSamplerStateImpl* zpSamplerState::getSamplerStateImpl() const
{
	return m_samplerState;
}
