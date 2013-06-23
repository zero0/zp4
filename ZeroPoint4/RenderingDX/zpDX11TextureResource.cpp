
#include "zpDX11.h"
#include <D3DX11.h>
#if 0

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
	hr = D3DX11CreateTextureFromFile( engine->getDevice(), getFilename().str(), ZP_NULL, ZP_NULL, &texture, ZP_NULL );
	if( FAILED( hr ) ) return false;

	// create the shader resource view from the file
	hr = D3DX11CreateShaderResourceViewFromFile( engine->getDevice(), getFilename().str(), ZP_NULL, ZP_NULL, &resourceView, ZP_NULL );
	if( FAILED( hr ) ) return false;
	
	// set the texture variables directly
	m_texture.m_texture = texture;
	m_texture.m_textureResourceView = resourceView;
	m_texture.m_type = ZP_TEXTURE_TYPE_TEXTURE;
	
	// try and get the image info from the file, ok if fails
	D3DX11_IMAGE_INFO info;
	hr = D3DX11GetImageInfoFromFile( getFilename().str(), ZP_NULL, &info, ZP_NULL );
	if( SUCCEEDED( hr ) ) {
		m_texture.m_width = info.Width;
		m_texture.m_height = info.Height;
		switch( info.ResourceDimension ) {
			case D3D11_RESOURCE_DIMENSION_TEXTURE1D: m_texture.m_dimension = ZP_TEXTURE_DIMENSION_1D; break;
			default:
			case D3D11_RESOURCE_DIMENSION_TEXTURE2D: m_texture.m_dimension = ZP_TEXTURE_DIMENSION_2D; break;
			case D3D11_RESOURCE_DIMENSION_TEXTURE3D: m_texture.m_dimension = ZP_TEXTURE_DIMENSION_3D; break;
		}
	}

	return true;
}
void zpDX11TextureResource::unload() {
	// release texture so it can be recreated (if need be)
	ZP_SAFE_RELEASE( m_texture.m_texture );
	ZP_SAFE_RELEASE( m_texture.m_textureResourceView );

	m_texture.m_height = 0;
	m_texture.m_width = 0;
	m_texture.m_dimension = ZP_TEXTURE_DIMENSION_UNKNOWN;
}

zpTexture* zpDX11TextureResource::getTexture() const {
	return (zpTexture*)&m_texture;

}
#endif
