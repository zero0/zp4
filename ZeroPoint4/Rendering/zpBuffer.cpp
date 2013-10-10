#include "zpRendering.h"
#include "zpRenderingImpl.inl"

zpBuffer::zpBuffer()
	: m_buffer( ZP_NULL )
{}
zpBuffer::~zpBuffer()
{
	if( m_buffer )
	{
		m_buffer->release();
	}
	m_buffer = ZP_NULL;
}
void zpBuffer::operator=( const zpBuffer& buffer )
{
	if( m_buffer )
	{
		m_buffer->release();
	}

	m_buffer = buffer.m_buffer;

	if( m_buffer )
	{
		m_buffer->addRef();
	}
}
void zpBuffer::operator=( zpBuffer&& buffer )
{
	if( m_buffer )
	{
		m_buffer->release();
	}

	m_buffer = buffer.m_buffer;

	buffer.m_buffer = ZP_NULL;
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

zpBufferImpl* zpBuffer::getBufferImpl() const
{
	return m_buffer;
}

void zpBuffer::release()
{
	m_buffer->release();
}
