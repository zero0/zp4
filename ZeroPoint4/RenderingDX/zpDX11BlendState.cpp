#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"

zpBlendStateImpl::zpBlendStateImpl()
	: m_blend( ZP_NULL )
{}
zpBlendStateImpl::~zpBlendStateImpl()
{
	ZP_SAFE_RELEASE( m_blend );
}

#if 0
zpDX11BlendState::zpDX11BlendState( ID3D11BlendState* state ) :
m_Blend( state )
{}
zpDX11BlendState::~zpDX11BlendState() {
	ZP_SAFE_RELEASE( m_Blend );
}
#endif
