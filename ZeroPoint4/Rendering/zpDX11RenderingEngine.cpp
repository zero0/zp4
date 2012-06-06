#include "zpRendering.h"
#include <D3D11.h>

zpDX11RenderingEngine::zpDX11RenderingEngine() : m_d3dDevice( ZP_NULL ) {}
zpDX11RenderingEngine::~zpDX11RenderingEngine() {
	ZP_SAFE_RELEASE( m_d3dDevice );
}

void zpDX11RenderingEngine::clear() {}
void zpDX11RenderingEngine::present() {}

