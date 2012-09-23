#include "zpDX11.h"
#include <D3DX11.h>
#include "zpDX11Util.h"

zpDX11RenderTarget::zpDX11RenderTarget() :
	m_referenceCount( 1 ),
	m_width( 0 ),
	m_height( 0 ),
	m_renderTargets( 0 ),
	m_textures( 0 )
{}
zpDX11RenderTarget::zpDX11RenderTarget( ID3D11Texture2D** textures, ID3D11RenderTargetView** targets, zp_uint count, zp_uint width, zp_uint height ) :
	m_referenceCount( 1 ),
	m_width( width ),
	m_height( height ),
	m_renderTargets( count ),
	m_textures( count )
{
	m_textures.fill( textures, count );
	m_renderTargets.fill( targets, count );
	
	m_textures.foreach( []( ID3D11Texture2D* tex ) {
		tex->AddRef();
	} );
	m_renderTargets.foreach( []( ID3D11RenderTargetView* rtv ) {
		rtv->AddRef();
	} );
}
zpDX11RenderTarget::~zpDX11RenderTarget() {
	m_textures.foreach( []( ID3D11Texture2D* tex ) {
		tex->Release();
	} );
	m_renderTargets.foreach( []( ID3D11RenderTargetView* rtv ) {
		rtv->Release();
	} );

	m_textures.clear();
	m_renderTargets.clear();
}

zp_uint zpDX11RenderTarget::getWidth() const {
	return m_width;
}
zp_uint zpDX11RenderTarget::getHeight() const {
	return m_height;
}

zp_uint zpDX11RenderTarget::getDisplayFormat( zpDisplayFormat* formats ) const {
	D3D11_TEXTURE2D_DESC desc;
	m_textures.foreachIndexed( [ &formats, &desc ]( zp_uint index, ID3D11Texture2D* tex ) {
		tex->GetDesc( &desc );
		formats[ index ] = __dxToZP( desc.Format );
	} );
	return m_renderTargets.size();
}

zp_uint zpDX11RenderTarget::getNumberOfTargets() const {
	return m_renderTargets.size();
}

void zpDX11RenderTarget::addReference() const {
	++m_referenceCount;
	m_textures.foreach( []( ID3D11Texture2D* tex ) {
		tex->AddRef();
	} );
	m_renderTargets.foreach( []( ID3D11RenderTargetView* rtv ) {
		rtv->AddRef();
	} );
}
zp_bool zpDX11RenderTarget::removeReference() const {
	--m_referenceCount;
	m_textures.foreach( []( ID3D11Texture2D* tex ) {
		tex->Release();
	} );
	m_renderTargets.foreach( []( ID3D11RenderTargetView* rtv ) {
		rtv->Release();
	} );
	return m_referenceCount == 0;
}

zp_uint zpDX11RenderTarget::getReferenceCount() const {
	return m_referenceCount;
}

void zpDX11RenderTarget::markForAutoDelete( zp_bool marked ) const {}
zp_bool zpDX11RenderTarget::isMarkedForAutoDelete() const {
	return false;
}

ID3D11RenderTargetView** zpDX11RenderTarget::getRenderTargets() {
	return m_renderTargets.data();
}
ID3D11Texture2D** zpDX11RenderTarget::getTextures() {
	return m_textures.data();
}