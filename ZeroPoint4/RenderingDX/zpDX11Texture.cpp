#include "zpDX11.h"
#include <D3DX11.h>

zpDX11Texture::zpDX11Texture()
	: m_width( 0 )
	, m_height( 0 )
	, m_dimension( ZP_TEXTURE_DIMENSION_UNKNOWN )
	, m_type( ZP_TEXTURE_TYPE_TEXTURE )
	, m_texture( ZP_NULL )
	, m_textureResourceView( ZP_NULL )
	, m_textureRenderTarget( ZP_NULL )
{}
zpDX11Texture::~zpDX11Texture() {
	ZP_SAFE_RELEASE( m_texture );
	ZP_SAFE_RELEASE( m_textureResourceView );
	ZP_SAFE_RELEASE( m_textureResourceView );
}

zpTextureDimension zpDX11Texture::getTextureDimension() const {
	return m_dimension;
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
ID3D11RenderTargetView* zpDX11Texture::getRenderTargetView() const {
	return m_textureRenderTarget;
}