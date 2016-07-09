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
