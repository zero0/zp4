#include "zpRendering.h"
#include "zpDX11.h"
#include <D3DX11.h>

zpDX11TextureResource::zpDX11TextureResource() :
	m_texture()
{}
zpDX11TextureResource::~zpDX11TextureResource() {
	unload();
}

zp_bool zpDX11TextureResource::load() {
	if( m_texture.m_texture ) return true;

	zpDX11RenderingEngine* engine = (zpDX11RenderingEngine*)zpRenderingFactory::getRenderingEngine();
	HRESULT hr;

	ID3D11Resource* texture;
	ID3D11ShaderResourceView* resourceView;

	// create the texture from the file
	hr = D3DX11CreateTextureFromFile( engine->getDevice(), getFilename().c_str(), ZP_NULL, ZP_NULL, &texture, ZP_NULL );
	if( FAILED( hr ) ) return false;

	// create the shader resource view from the file
	hr = D3DX11CreateShaderResourceViewFromFile( engine->getDevice(), getFilename().c_str(), ZP_NULL, ZP_NULL, &resourceView, ZP_NULL );
	if( FAILED( hr ) ) return false;
	
	// set the texture variables directly
	m_texture.m_texture = texture;
	m_texture.m_textureResourceView = resourceView;
	
	// try and get the image info from the file, ok if fails
	D3DX11_IMAGE_INFO info;
	hr = D3DX11GetImageInfoFromFile( getFilename().c_str(), ZP_NULL, &info, ZP_NULL );
	if( SUCCEEDED( hr ) ) {
		m_texture.m_width = info.Width;
		m_texture.m_height = info.Height;
		switch( info.ResourceDimension ) {
			case D3D11_RESOURCE_DIMENSION_TEXTURE1D: m_texture.m_type = ZP_TEXTURE_TYPE_1D; break;
			default:
			case D3D11_RESOURCE_DIMENSION_TEXTURE2D: m_texture.m_type = ZP_TEXTURE_TYPE_2D; break;
			case D3D11_RESOURCE_DIMENSION_TEXTURE3D: m_texture.m_type = ZP_TEXTURE_TYPE_3D; break;
		}
	}
	
	return true;
}
void zpDX11TextureResource::unload() {
	// release texture so it can be recreated (if need be)
	m_texture.removeReference();
}

zpTextureType zpDX11TextureResource::getTextureType() const {
	return m_texture.getTextureType();
}
zp_uint zpDX11TextureResource::getWidth() const {
	return m_texture.getWidth();
}
zp_uint zpDX11TextureResource::getHeight() const {
	return m_texture.getHeight();
}

zpTexture* zpDX11TextureResource::getTexture() const {
	return (zpTexture*)&m_texture;
}
