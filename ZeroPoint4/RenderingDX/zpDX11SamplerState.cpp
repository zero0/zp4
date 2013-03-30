#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"

zpSamplerStateImpl::zpSamplerStateImpl()
	: m_sampler( ZP_NULL )
{}
zpSamplerStateImpl::~zpSamplerStateImpl()
{
	ZP_SAFE_RELEASE( m_sampler );
}

#if 0
zpDX11SamplerState::zpDX11SamplerState( ID3D11SamplerState* state ) :
	m_sampler( state )
{}
zpDX11SamplerState::~zpDX11SamplerState() {
	ZP_SAFE_RELEASE( m_sampler );
}
#endif