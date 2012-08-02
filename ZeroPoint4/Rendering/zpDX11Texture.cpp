#include "zpRendering.h"
#include "zpDX11.h"
#include <D3DX11.h>

zpDX11Texture::zpDX11Texture() :
	m_referenceCount( 1 ),
	m_width( 0 ),
	m_height( 0 ),
	m_type( ZP_TEXTURE_TYPE_2D ),
	m_texture( ZP_NULL ),
	m_textureResourceView( ZP_NULL )
{}
zpDX11Texture::~zpDX11Texture() {}

zpTextureType zpDX11Texture::getTextureType() const {
	return m_type;
}

zp_uint zpDX11Texture::getWidth() const {
	return m_width;
}
zp_uint zpDX11Texture::getHeight() const {
	return m_height;
}

ID3D11Resource* zpDX11Texture::getTexture() const {
	return m_texture;
}
ID3D11ShaderResourceView* zpDX11Texture::getResourceView() const {
	return m_textureResourceView;
}

void zpDX11Texture::addReference() const {
	++m_referenceCount;
}
zp_bool zpDX11Texture::removeReference() const {
	if( m_referenceCount == 0 ) return true;
	--m_referenceCount;
	return m_referenceCount == 0;
}

zp_uint zpDX11Texture::getReferenceCount() const {
	return m_referenceCount;
}

void zpDX11Texture::markForAutoDelete( zp_bool marked ) const {}
zp_bool zpDX11Texture::isMarkedForAutoDelete() const {
	return false;
}