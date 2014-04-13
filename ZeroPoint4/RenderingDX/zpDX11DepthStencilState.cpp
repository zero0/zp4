#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"

zpDepthStencilStateImpl::zpDepthStencilStateImpl()
	: m_depthStencil( ZP_NULL )
{}
zpDepthStencilStateImpl::~zpDepthStencilStateImpl()
{
	ZP_SAFE_RELEASE( m_depthStencil );
}

#if 0
zpDX11DepthStencilState::zpDX11DepthStencilState( ID3D11DepthStencilState* state ) :
m_DepthStencil( state )
{}
zpDX11DepthStencilState::~zpDX11DepthStencilState() {
	ZP_SAFE_RELEASE( m_DepthStencil );
}
#endif
