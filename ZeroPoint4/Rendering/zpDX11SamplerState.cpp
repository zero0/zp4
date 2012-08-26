#include "zpRendering.h"
#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"

zpDX11SamplerState::zpDX11SamplerState( ID3D11SamplerState* state ) :
	m_sampler( state )
{}
zpDX11SamplerState::~zpDX11SamplerState() {
	ZP_SAFE_RELEASE( m_sampler );
}