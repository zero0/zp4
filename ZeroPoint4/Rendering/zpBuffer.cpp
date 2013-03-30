#include "zpRendering.h"

#if ZP_RENDERING_TYPE == ZP_DX11
#include "RenderingDX/zpDX11.h"
#elif ZP_RENDERING_TYPE == ZP_GL2
#include "RenderingOpenGL/zpOpenGL.h"
#else
#error( "No rendering engine selected!" )
#endif

zpBuffer::zpBuffer( zpBufferImpl* buffer )
	: m_buffer( buffer )
{}
zpBuffer::~zpBuffer()
{
	ZP_SAFE_DELETE( m_buffer );
}
zp_uint zpBuffer::getSize() const
{
	return m_buffer->getSize();
}
zp_uint zpBuffer::getStride() const
{
	return m_buffer->getStride();
}
zpBufferType zpBuffer::getBufferType() const
{
	return m_buffer->getBufferType();
}
zpDisplayFormat zpBuffer::getFormat() const
{
	return m_buffer->getFormat();
}
zpBufferBindType zpBuffer::getBufferBindType() const
{
	return m_buffer->getBufferBindType();
}