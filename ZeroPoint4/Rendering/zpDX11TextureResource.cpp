#include "zpRendering.h"
#include "zpDX11.h"
#include <D3DX11.h>

zpDX11TextureResource::zpDX11TextureResource() :
	m_width( 0 ),
	m_height( 0 ),
	m_type( ZP_TEXTURE_TYPE_2D ),
	m_texture( ZP_NULL ),
	m_textureRV( ZP_NULL )
{}
zpDX11TextureResource::~zpDX11TextureResource() {
	unload();
}

zp_bool zpDX11TextureResource::load() {
	if( m_texture ) return true;

	zpDX11RenderingEngine* engine = (zpDX11RenderingEngine*)zpRenderingFactory::getRenderingEngine();
	HRESULT hr;

	// create the texture from the file
	hr = D3DX11CreateTextureFromFile( engine->getDevice(), getFilename().c_str(), ZP_NULL, ZP_NULL, &m_texture, ZP_NULL );
	if( FAILED( hr ) ) return false;

	// create the shader resource view from the file
	hr = D3DX11CreateShaderResourceViewFromFile( engine->getDevice(), getFilename().c_str(), ZP_NULL, ZP_NULL, &m_textureRV, ZP_NULL );
	if( FAILED( hr ) ) return false;
	
	// try and get the image info from the file, okay if fails
	D3DX11_IMAGE_INFO info;
	hr = D3DX11GetImageInfoFromFile( getFilename().c_str(), ZP_NULL, &info, ZP_NULL );
	if( SUCCEEDED( hr ) ) {
		m_width = info.Width;
		m_height = info.Height;
		switch( info.ResourceDimension ) {
			case D3D11_RESOURCE_DIMENSION_TEXTURE1D: m_type = ZP_TEXTURE_TYPE_1D; break;
			default:
			case D3D11_RESOURCE_DIMENSION_TEXTURE2D: m_type = ZP_TEXTURE_TYPE_2D; break;
			case D3D11_RESOURCE_DIMENSION_TEXTURE3D: m_type = ZP_TEXTURE_TYPE_3D; break;
		}
	}

	return true;
}
void zpDX11TextureResource::unload() {
	// release texture and resource view
	ZP_SAFE_RELEASE( m_texture );
	ZP_SAFE_RELEASE( m_textureRV );

	// reset info
	m_width = 0;
	m_height = 0;
	m_type = ZP_TEXTURE_TYPE_2D;
}

zpTextureType zpDX11TextureResource::getTextureType() const {
	return m_type;
}
zp_uint zpDX11TextureResource::getWidth() const {
	return m_width;
}
zp_uint zpDX11TextureResource::getHeight() const {
	return m_height;
}

ID3D11Resource* zpDX11TextureResource::getTexture() const {
	return m_texture;
}
ID3D11ShaderResourceView* zpDX11TextureResource::getTextureResourceView() const {
	return m_textureRV;
}