#include "zpRendering.h"
#include "zpDX11.h"
#include <D3D11.h>

ZP_FORCE_INLINE D3D11_MAP __zpMapTypeToD3DMap( zpRenderingMapType type ) {
	switch( type ) {
		case ZP_RENDERING_MAP_TYPE_READ: return D3D11_MAP_READ;
		case ZP_RENDERING_MAP_TYPE_WRITE: return D3D11_MAP_WRITE;
		case ZP_RENDERING_MAP_TYPE_READ_WRITE: return D3D11_MAP_READ_WRITE;
		case ZP_RENDERING_MAP_TYPE_WRITE_DISCARD: return D3D11_MAP_WRITE_DISCARD;
		default: return D3D11_MAP_WRITE_DISCARD;
	}
}

zpDX11RenderingContext::zpDX11RenderingContext() :
	m_context( ZP_NULL ),
	m_renderTarget( ZP_NULL ),
	m_depthStencilBuffer( ZP_NULL ),
	m_referenceCount( 1 ),
	m_name()
{}
zpDX11RenderingContext::zpDX11RenderingContext( ID3D11DeviceContext* context, const zpString& name ) :
	m_context( context ),
	m_renderTarget( ZP_NULL ),
	m_depthStencilBuffer( ZP_NULL ),
	m_referenceCount( 1 ),
	m_name( name )
{}
zpDX11RenderingContext::~zpDX11RenderingContext() {
}

const zpString& zpDX11RenderingContext::getName() const {
	return m_name;
}

void zpDX11RenderingContext::setRenderTarget( zpRenderTarget* target ) {
	if( m_renderTarget ) m_renderTarget->removeReference();
	if( target ) target->addReference();

	m_renderTarget = target;
}
zpRenderTarget* zpDX11RenderingContext::getRenderTarget() const {
	return m_renderTarget;
}

void zpDX11RenderingContext::setDepthStencilBuffer( zpDepthStencilBuffer* depthBuffer ) {
	m_depthStencilBuffer = depthBuffer;
}
zpDepthStencilBuffer* zpDX11RenderingContext::getDepthStencilBuffer() const {
	return m_depthStencilBuffer;
}

void zpDX11RenderingContext::clearRenderTarget( const zpColor4f* colors, zp_uint count ) {
	zp_uint numTargets = m_renderTarget->getNumberOfTargets();
	zp_uint num = ZP_MIN( count, numTargets );
	zp_uint i;

	ID3D11RenderTargetView** views = ( (zpDX11RenderTarget*)m_renderTarget )->getRenderTargets();
	
	// clear the targets up to the minimum number of either colors or targets
	for( i = 0; i < num; ++i ) {
		m_context->ClearRenderTargetView( views[ i ], colors[ i ] );
	}
	// clear the rest of the targets using the last color provided
	for( ; i < numTargets; ++i ) {
		m_context->ClearRenderTargetView( views[ i ], colors[ count - 1 ] );
	}
}
void zpDX11RenderingContext::clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil ) {

}

void zpDX11RenderingContext::map( zpBuffer* buffer, zpRenderingMapType mapType, zp_uint subResource, void** data ) {
	zpDX11Buffer* buff = (zpDX11Buffer*)buffer;
	
	D3D11_MAPPED_SUBRESOURCE r;
	m_context->Map( buff->getBuffer(), subResource, __zpMapTypeToD3DMap( mapType ), 0, &r );
	data = &r.pData;
}
void zpDX11RenderingContext::unmap( zpBuffer* buffer, zp_uint subResource ) {
	zpDX11Buffer* buff = (zpDX11Buffer*)buffer;
	m_context->Unmap( buff->getBuffer(), subResource );
}

void zpDX11RenderingContext::addReference() const {
	++m_referenceCount;
	m_context->AddRef();
}
zp_bool zpDX11RenderingContext::removeReference() const {
	--m_referenceCount;
	m_context->Release();
	return m_referenceCount == 0;
}

zp_uint zpDX11RenderingContext::getReferenceCount() const {
	return m_referenceCount;
}

void zpDX11RenderingContext::markForAutoDelete( zp_bool marked ) const {}
zp_bool zpDX11RenderingContext::isMarkedForAutoDelete() const {
	return false;
}
ID3D11DeviceContext* zpDX11RenderingContext::getContext() const {
	return m_context;
}