#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"

zpRasterStateImpl::zpRasterStateImpl()
	: m_raster( ZP_NULL )
{}
zpRasterStateImpl::~zpRasterStateImpl()
{
	ZP_SAFE_RELEASE( m_raster );
}

#if 0
zpDX11RasterState::zpDX11RasterState( ID3D11RasterizerState* state ) :
	m_raster( state )
{}
zpDX11RasterState::~zpDX11RasterState() {
	ZP_SAFE_RELEASE( m_raster );
}
#endif
