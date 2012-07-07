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
	m_context( ZP_NULL )
{}
zpDX11RenderingContext::zpDX11RenderingContext( ID3D11DeviceContext* context ) :
	m_context( context )
{}
zpDX11RenderingContext::~zpDX11RenderingContext() {
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
