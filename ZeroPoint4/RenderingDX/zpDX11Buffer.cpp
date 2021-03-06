#include "zpDX11.h"
#include <D3D11.h>
#include "zpDX11Util.h"

zpBufferImpl::zpBufferImpl()
    : m_size( 0 )
    , m_stride( 0 )
    , m_type( ZP_BUFFER_TYPE_UNKNOWN )
    , m_format( ZP_DISPLAY_FORMAT_UNKNOWN )
    , m_bind( ZP_BUFFER_BIND_DEFAULT )
    , m_buffer( ZP_NULL )
{}
zpBufferImpl::~zpBufferImpl()
{
    ZP_SAFE_RELEASE( m_buffer );
}

zp_uint zpBufferImpl::getSize() const
{
    return m_size;
}
zp_uint zpBufferImpl::getStride() const
{
    return m_stride;
}
zpBufferType zpBufferImpl::getBufferType() const
{
    return m_type;
}
zpDisplayFormat zpBufferImpl::getFormat() const
{
    return m_format;
}
zpBufferBindType zpBufferImpl::getBufferBindType() const
{
    return m_bind;
}

#if 0
zpDX11Buffer::zpDX11Buffer() :
    m_count( 0 ),
    m_stride( 0 ),
    m_type( ZP_BUFFER_TYPE_VERTEX ),
    m_format( ZP_DISPLAY_FORMAT_UNKNOWN ),
    m_bind( ZP_BUFFER_BIND_DEFAULT ),
    m_buffer( ZP_NULL )
{}
zpDX11Buffer::~zpDX11Buffer() {
    destroy();
}

void zpDX11Buffer::create( zpBufferType type, zpBufferBindType bind, zp_uint count, zp_uint stride, void* data ) {
    if( m_buffer ) return;

    m_type = type;
    m_bind = bind;
    m_count = count;
    m_stride = stride;

    if( m_type == ZP_BUFFER_TYPE_INDEX ) {
        switch( m_stride ) {
        case 2: m_format = ZP_DISPLAY_FORMAT_R8_UINT; break;
        case 4: m_format = ZP_DISPLAY_FORMAT_R16_UINT; break;
        }
    }

    HRESULT hr;

    D3D11_BUFFER_DESC desc;
    zp_zero_memory( &desc );
    desc.Usage = __zpToDX( bind );
    desc.ByteWidth = m_stride * m_count;
    desc.BindFlags = __zpToDX( type );
    desc.CPUAccessFlags = bind == ZP_BUFFER_BIND_DYNAMIC ? D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE : 0;
    desc.MiscFlags = 0;

    ID3D11Device* device = ( (zpDX11RenderingEngine*)zpRenderingFactory::getRenderingEngine() )->getDevice();
    if( data ) {
        D3D11_SUBRESOURCE_DATA sub;
        zp_zero_memory( &sub );
        sub.pSysMem = data;
        
        hr = device->CreateBuffer( &desc, &sub, &m_buffer );
    } else {
        hr = device->CreateBuffer( &desc, ZP_NULL, &m_buffer );
    }

    if( FAILED( hr ) ) zpLog::error() << "Failed to create buffer" << zpLog::endl;
}
void zpDX11Buffer::destroy() {
    ZP_SAFE_RELEASE( m_buffer );
}

void zpDX11Buffer::map( void** data, zpMapType mapType, zp_uint subResource ) {
    zpRenderingFactory::getRenderingEngine()->getImmediateRenderingContext()->map( this, mapType, 0, data );
}
void zpDX11Buffer::unmap( zp_uint subResource ) {
    zpRenderingFactory::getRenderingEngine()->getImmediateRenderingContext()->unmap( this, subResource );
}

void zpDX11Buffer::update( zp_uint count, void* data ) {
    void* d;
    zpRenderingContext* context = zpRenderingFactory::getRenderingEngine()->getImmediateRenderingContext();
    m_count = count;

    if( m_type == ZP_BUFFER_TYPE_CONSTANT ) {
        context->updateBuffer( this, data );
    } else {
        context->map( this, ZP_MAP_TYPE_WRITE_DISCARD, 0, &d );
        zp_memcpy( d, count * m_stride, data, count * m_stride );
        context->unmap( this, 0 );
    }
}

zp_uint zpDX11Buffer::getCount() const {
    return m_count;
}
zp_uint zpDX11Buffer::getStride() const {
    return m_stride;
}
zpBufferType zpDX11Buffer::getBufferType() const {
    return m_type;
}
zpDisplayFormat zpDX11Buffer::getFormat() const {
    return m_format;
}
zpBufferBindType zpDX11Buffer::getBufferBindType() const {
    return m_bind;
}

ID3D11Buffer* zpDX11Buffer::getBuffer() const {
    return m_buffer;
}

#endif
