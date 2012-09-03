#include "zpRendering.h"
#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"

zpDX11RasterState::zpDX11RasterState( ID3D11RasterizerState* state ) :
	m_raster( state )
{}
zpDX11RasterState::~zpDX11RasterState() {
	ZP_SAFE_RELEASE( m_raster );
}