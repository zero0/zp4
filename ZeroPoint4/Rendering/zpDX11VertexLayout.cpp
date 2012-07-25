#include "zpRendering.h"
#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"

zpDX11VertexLayout::zpDX11VertexLayout() :
	m_layout( ZP_NULL )
{}
zpDX11VertexLayout::~zpDX11VertexLayout() {
	ZP_SAFE_RELEASE( m_layout );
}

ID3D11InputLayout* zpDX11VertexLayout::getInputLayout() const {
	return m_layout;
}
