#include "zpCore.h"

zpDataBuffer::zpDataBuffer( zp_uint capacity )
	: m_data( ZP_NULL )
	, m_capacity( 0 )
	, m_offset( 0 )
	, m_isFixed( false )
{
	ensureCapacity( capacity );
}
zpDataBuffer::zpDataBuffer( void* data, zp_uint capacity )
	: m_data( data )
	, m_capacity( capacity )
	, m_offset( 0 )
	, m_isFixed( true )
{}
zpDataBuffer::~zpDataBuffer()
{
	clear();
	if( !m_isFixed )
	{
		ZP_SAFE_FREE( m_data );
	}
}

const void* zpDataBuffer::getData() const
{
	return m_data;
}

zp_uint zpDataBuffer::size() const
{
	return m_offset;
}

zp_uint zpDataBuffer::capacity() const
{
	return m_capacity;
}

void zpDataBuffer::ensureCapacity( zp_uint capacity )
{
	if( capacity && capacity > m_capacity )
	{
		ZP_ASSERT( !m_isFixed, "Attempting to grow fixed size data buffer" );

		void* data = zp_malloc( capacity );
		if( m_data )
		{
			zp_memcpy( data, capacity, m_data, m_capacity );
		}
		ZP_SAFE_FREE( m_data );

		m_data = data;
		m_capacity = capacity;
	}
}

void zpDataBuffer::clear()
{
	m_offset = 0;
	if( m_data )
	{
		zp_memset( m_data, 0, m_capacity );
	}
}
void zpDataBuffer::reset()
{
	m_offset = 0;
}
