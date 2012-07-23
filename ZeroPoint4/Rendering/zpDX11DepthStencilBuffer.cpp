#include "zpRendering.h"
#include "zpDX11.h"
#include <D3DX11.h>

zpDX11DepthStencilBuffer::zpDX11DepthStencilBuffer() :
	m_depthStencilView( ZP_NULL ),
	m_depthTexture( ZP_NULL ),
	m_referenceCount( 1 ),
	m_width( 0 ),
	m_height( 0 )
{}
zpDX11DepthStencilBuffer::zpDX11DepthStencilBuffer( ID3D11Texture2D* texture, ID3D11DepthStencilView* view, zp_uint width, zp_uint height ) :
	m_depthStencilView( view ),
	m_depthTexture( texture ),
	m_referenceCount( 1 ),
	m_width( width ),
	m_height( height )
{}
zpDX11DepthStencilBuffer::~zpDX11DepthStencilBuffer() {
	m_depthStencilView->Release();
	m_depthTexture->Release();
}

zp_uint zpDX11DepthStencilBuffer::getWidth() const {
	return m_width;
}
zp_uint zpDX11DepthStencilBuffer::getHeight() const {
	return m_height;
}

zpRenderingDisplayFormat zpDX11DepthStencilBuffer::getDisplayFormat() const {
	return ZP_RENDERING_DISPLAY_FORMAT_UNKNOWN;
}

void zpDX11DepthStencilBuffer::addReference() const {
	++m_referenceCount;
	m_depthTexture->AddRef();
	m_depthStencilView->AddRef();
}
zp_bool zpDX11DepthStencilBuffer::removeReference() const {
	--m_referenceCount;
	m_depthTexture->Release();
	m_depthStencilView->Release();
	return m_referenceCount == 0;
}

zp_uint zpDX11DepthStencilBuffer::getReferenceCount() const {
	return m_referenceCount;
}

void zpDX11DepthStencilBuffer::markForAutoDelete( zp_bool marked ) const {}
zp_bool zpDX11DepthStencilBuffer::isMarkedForAutoDelete() const {
	return false;
}

ID3D11DepthStencilView* zpDX11DepthStencilBuffer::getDepthStencilView() {
	return m_depthStencilView;
}
ID3D11Texture2D* zpDX11DepthStencilBuffer::getDepthTexture() {
	return m_depthTexture;
}