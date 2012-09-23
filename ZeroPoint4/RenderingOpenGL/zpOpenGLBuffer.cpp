#include "zpOpenGL.h"

zpOpenGLBuffer::zpOpenGLBuffer() :
	m_count( 0 ),
	m_stride( 0 ),
	m_type( ZP_BUFFER_TYPE_VERTEX ),
	m_format( ZP_DISPLAY_FORMAT_UNKNOWN ),
	m_bind( ZP_BUFFER_BIND_DEFAULT ),
	m_buffer( 0 )
{}
zpOpenGLBuffer::~zpOpenGLBuffer() {
	destroy();
}

void zpOpenGLBuffer::create( zpBufferType type, zpBufferBindType bind, zp_uint count, zp_uint stride, void* data ) {
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
void zpOpenGLBuffer::destroy() {
	glDeleteBuffers( 1, &m_buffer );
}

void zpOpenGLBuffer::map( void** data, zpMapType mapType, zp_uint subResource ) {
	glBindBuffer( m_target, m_buffer );
	*data = glMapBuffer( m_target, __zpToGL( mapType ) );
}
void zpOpenGLBuffer::unmap( zp_uint subResource ) {
	glUnmapBuffer( m_target );
	glBindBuffer( m_target, 0 );
}

void zpOpenGLBuffer::update( zp_uint count, void* data ) {
	glBindBuffer( m_target, m_buffer );
	glBufferSubData( m_target, 0, count * m_stride, data );
	glBindBuffer( m_target, 0 );
}

zp_uint zpOpenGLBuffer::getCount() const {
	return m_count;
}
zp_uint zpOpenGLBuffer::getStride() const {
	return m_stride;
}
zpBufferType zpOpenGLBuffer::getBufferType() const {
	return m_type;
}
zpDisplayFormat zpOpenGLBuffer::getFormat() const {
	return m_format;
}
zpBufferBindType zpOpenGLBuffer::getBufferBindType() const {
	return m_bind;
}

zp_uint zpOpenGLBuffer::getBuffer() const {
	return m_buffer;
}
zp_uint zpOpenGLBuffer::getTarget() const {
	return m_target;
}