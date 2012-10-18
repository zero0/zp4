#include "zpDX11.h"
#include <D3DX11.h>

zpDX11Texture::zpDX11Texture()
	: m_width( 0 )
	, m_height( 0 )
	, m_type( ZP_TEXTURE_TYPE_UNKNOWN )
	, m_texture( ZP_NULL )
	, m_textureResourceView( ZP_NULL )
{}
zpDX11Texture::~zpDX11Texture() {
	ZP_SAFE_RELEASE( m_texture );
	ZP_SAFE_RELEASE( m_textureResourceView );
}

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
