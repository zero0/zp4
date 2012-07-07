#include "zpRendering.h"
#include "zpDX11.h"
#include <D3D11.h>

ZP_FORCE_INLINE D3D11_USAGE __zpBindToUsage( zpBufferBind bind ) {
	switch( bind ) {
		case ZP_BUFFER_BIND_DEFAULT: return D3D11_USAGE_DEFAULT;
		case ZP_BUFFER_BIND_DYNAMIC: return D3D11_USAGE_DYNAMIC;
		case ZP_BUFFER_BIND_IMMUTABLE: return D3D11_USAGE_IMMUTABLE;
		default: return D3D11_USAGE_DEFAULT;
	}
}

ZP_FORCE_INLINE D3D11_BIND_FLAG __zpBufferTypeToBind( zpBufferType type ) {
	switch( type ) {
		case ZP_BUFFER_TYPE_VERTEX: return D3D11_BIND_VERTEX_BUFFER;
		case ZP_BUFFER_TYPE_INDEX: return D3D11_BIND_INDEX_BUFFER;
	}
}

zpDX11Buffer::zpDX11Buffer() :
	m_count( 0 ),
	m_stride( 0 ),
	m_type( ZP_BUFFER_TYPE_VERTEX ),
	m_bind( ZP_BUFFER_BIND_DEFAULT ),
	m_buffer( ZP_NULL ),
	m_engine( ZP_NULL )
{}
zpDX11Buffer::zpDX11Buffer( zpDX11RenderingEngine* engine ) :
	m_count( 0 ),
	m_stride( 0 ),
	m_type( ZP_BUFFER_TYPE_VERTEX ),
	m_bind( ZP_BUFFER_BIND_DEFAULT ),
	m_buffer( ZP_NULL ),
	m_engine( engine )
{}
zpDX11Buffer::~zpDX11Buffer() {
	release();
}

void zpDX11Buffer::create( zpBufferType type, zpBufferBind bind, zp_uint count, zp_uint stride, void* data ) {
	m_type = type;
	m_bind = bind;
	m_count = count;
	m_stride = stride;


	D3D11_BUFFER_DESC desc;
	zp_zero_memory( &desc );
	desc.Usage = __zpBindToUsage( bind );
	desc.ByteWidth = m_stride * m_count;
	desc.BindFlags = __zpBufferTypeToBind( type );
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	if( data ) {
		D3D11_SUBRESOURCE_DATA sub;
		zp_zero_memory( &sub );
		sub.pSysMem = data;

		m_engine->getDevice()->CreateBuffer( &desc, &sub, &m_buffer );
	} else {
		m_engine->getDevice()->CreateBuffer( &desc, ZP_NULL, &m_buffer );
	}
}
void zpDX11Buffer::release() {
	ZP_SAFE_RELEASE( m_buffer );
}

void zpDX11Buffer::map( void** data, zpRenderingMapType mapType, zp_uint subResource ) {
	m_engine->getCurrentRenderingContext()->map( this, mapType, 0, data );
}
void zpDX11Buffer::unmap( zp_uint subResource ) {
	m_engine->getCurrentRenderingContext()->unmap( this, subResource );
}

void zpDX11Buffer::update( zp_uint count, void* data ) {
	void* d;
	zpRenderingContext* context = m_engine->getCurrentRenderingContext();
	count = ZP_MIN( m_count, count );
	
	context->map( this, ZP_RENDERING_MAP_TYPE_WRITE_DISCARD, 0, &d );
	memcpy_s( d, count * m_stride, data, count * m_stride );
	context->unmap( this, 0 );
}

void zpDX11Buffer::bind() {}
void zpDX11Buffer::unbind() {}

zp_uint zpDX11Buffer::getCount() const {
	return m_count;
}

zp_uint zpDX11Buffer::getStride() const {
	return m_stride;
}

ID3D11Buffer* zpDX11Buffer::getBuffer() const {
	return m_buffer;
}