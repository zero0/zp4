#include "zpOpenGL.h"

zpBufferImpl::zpBufferImpl()
	: m_count( 0 )
	, m_stride( 0 )
	, m_type( ZP_BUFFER_TYPE_VERTEX )
	, m_format( ZP_DISPLAY_FORMAT_UNKNOWN )
	, m_bind( ZP_BUFFER_BIND_DEFAULT )
	, m_buffer( 0 )
	, m_target( 0 
{}
zpBufferImpl::~zpBufferImpl()
{
	destroy();
}

void zpBufferImpl::create( zpBufferType type, zpBufferBindType bind, zp_uint count, zp_uint stride, void* data )
{
	m_type = type;
	m_bind = bind;
	m_count = count;
	m_stride = stride;
	m_target = __zpToGL( type );

	glGenBuffers( 1, &m_buffer );

	glBindBuffer( m_target, m_buffer );
	glBufferData( m_target, count * stride, data, __zpToGL( bind ) );
	glBindBuffer( m_target, 0 );
}
void zpBufferImpl::destroy()
{
	glDeleteBuffers( 1, &m_buffer );
}

void zpBufferImpl::map( void** data, zpMapType mapType, zp_uint subResource )
{
	glBindBuffer( m_target, m_buffer );
	*data = glMapBuffer( m_target, __zpToGL( mapType ) );
}
void zpBufferImpl::unmap( zp_uint subResource )
{
	glUnmapBuffer( m_target );
	glBindBuffer( m_target, 0 );
}

void zpBufferImpl::update( zp_uint count, void* data )
{
	glBindBuffer( m_target, m_buffer );
	glBufferSubData( m_target, 0, count * m_stride, data );
	glBindBuffer( m_target, 0 );
}

zp_uint zpBufferImpl::getCount() const
{
	return m_count;
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
