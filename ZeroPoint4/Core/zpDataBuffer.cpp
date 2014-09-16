#include "zpCore.h"

#define ZP_DATA_BUFFER_DEFAULT_SIZE	16

zpDataBuffer::zpDataBuffer()
	: m_data( ZP_NULL )
	, m_capacity( 0 )
	, m_size( 0 )
	, m_isFixed( false )
{}
zpDataBuffer::zpDataBuffer( zp_uint capacity )
	: m_data( ZP_NULL )
	, m_capacity( 0 )
	, m_size( 0 )
	, m_isFixed( false )
{
	ensureCapacity( capacity );
}
zpDataBuffer::zpDataBuffer( const zpDataBuffer& other )
	: m_data( ZP_NULL )
	, m_capacity( 0 )
	, m_size( 0 )
	, m_isFixed( false )
{
	readIn( other );
}
zpDataBuffer::zpDataBuffer( zp_byte* data, zp_uint capacity )
	: m_data( data )
	, m_capacity( capacity )
	, m_size( 0 )
	, m_isFixed( true )
{}

zpDataBuffer::~zpDataBuffer()
{
	destroy();
}

void zpDataBuffer::operator=( const zpDataBuffer& other )
{
	reset();
	readIn( other );
}

zp_byte* zpDataBuffer::getData()
{
	return m_data;
}
const zp_byte* zpDataBuffer::getData() const
{
	return m_data;
}

zp_uint zpDataBuffer::size() const
{
	return m_size;
}

zp_uint zpDataBuffer::capacity() const
{
	return m_capacity;
}

void zpDataBuffer::ensureCapacity( zp_uint capacity )
{
	if( capacity > m_capacity )
	{
		ZP_ASSERT( !m_isFixed, "Attempting to grow fixed size data buffer" );
		if( !m_isFixed )
		{
			if( m_capacity == 0 )
			{
				m_capacity = ZP_DATA_BUFFER_DEFAULT_SIZE;
			}
			while( m_capacity < capacity ) m_capacity *= 2;

			zp_byte* data = (zp_byte*)zp_malloc( m_capacity );
			if( m_data )
			{
				zp_memcpy( data, capacity, m_data, m_size );
			}
			ZP_SAFE_FREE( m_data );

			m_data = data;
		}
	}
}

void zpDataBuffer::reserve( zp_uint size )
{
	ensureCapacity( size );
}
void zpDataBuffer::resize( zp_uint size )
{
	m_size = size;
}

void zpDataBuffer::clear()
{
	m_size = 0;
	if( m_data )
	{
		zp_memset( m_data, 0, m_capacity );
	}
}
void zpDataBuffer::reset()
{
	m_size = 0;
}
void zpDataBuffer::destroy()
{
	m_size = 0;
	m_capacity = 0;
	if( !m_isFixed )
	{
		ZP_SAFE_FREE( m_data );
	}
}

void zpDataBuffer::readIn( const zpDataBuffer& buffer )
{
	if( buffer.size() > 0 )
	{
		writeBulk( buffer.getData(), buffer.size() );
	}
}
void zpDataBuffer::writeOut( zpDataBuffer& buffer ) const
{
	if( m_size > 0 )
	{
		buffer.readIn( *this );
	}
}