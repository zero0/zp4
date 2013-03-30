#include "zpRendering.h"

#if ZP_RENDERING_TYPE == ZP_DX11
#include "RenderingDX/zpDX11.h"
#elif ZP_RENDERING_TYPE == ZP_GL2
#include "RenderingOpenGL/zpOpenGL.h"
#else
#error( "No rendering engine selected!" )
#endif

zpRasterState::zpRasterState( zpRasterStateImpl* rasterState, zp_hash descHash )
	: m_rasterState( rasterState )
	, m_descHash( descHash )
{}
zpRasterState::~zpRasterState()
{
	ZP_SAFE_DELETE( m_rasterState );
}

zpRasterStateImpl* zpRasterState::getRasterStateImpl() const
{
	return m_rasterState;
}
