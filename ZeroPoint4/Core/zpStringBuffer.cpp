#include "zpCore.h"
#include <stdio.h>

#define ZP_STRING_BUFFER_DEFAULT_SIZE	16

zpStringBuffer::zpStringBuffer()
	: m_buffer( ZP_NULL )
	, m_length( 0 )
	, m_capacity( 0 )
{}
zpStringBuffer::zpStringBuffer( const zpString& str )
	: m_buffer( ZP_NULL )
	, m_length( 0 )
	, m_capacity( 0 )
{
	append( str );
}
zpStringBuffer::zpStringBuffer( const zpStringBuffer& buff )
	: m_buffer( ZP_NULL )
	, m_length( 0 )
	, m_capacity( 0 )
{
	append( buff.m_buffer, buff.m_length );
}
zpStringBuffer::zpStringBuffer( zpStringBuffer&& buff )
	: m_buffer( buff.m_buffer )
	, m_length( buff.m_length )
	, m_capacity( buff.m_capacity )
{
	buff.m_buffer = ZP_NULL;
}
zpStringBuffer::~zpStringBuffer()
{
	ZP_SAFE_FREE( m_buffer );
}

void zpStringBuffer::operator=( const zpStringBuffer& buff )
{
	ZP_SAFE_DELETE( m_buffer );

	m_capacity = 0;
	m_length = 0;
	append( buff.m_buffer, buff.m_length );
}
void zpStringBuffer::operator=( zpStringBuffer&& buff )
{
	ZP_SAFE_DELETE( m_buffer );

	m_buffer = buff.m_buffer;
	m_capacity = buff.m_capacity;
	m_length = buff.m_length;

	buff.m_buffer = ZP_NULL;
}

zp_char& zpStringBuffer::operator[]( zp_uint index )
{
	return m_buffer[ index ];
}
zp_char zpStringBuffer::operator[]( zp_uint index ) const
{
	return m_buffer[ index ];
}

zpStringBuffer& zpStringBuffer::operator<<( zp_char value )
{
	append( (zp_char)value );
	return (*this);
}
zpStringBuffer& zpStringBuffer::operator<<( const zp_char* value )
{
	append( (const zp_char*)value );
	return (*this);
}
zpStringBuffer& zpStringBuffer::operator<<( const zpString& value )
{
	append( value );
	return (*this);
}

zpStringBuffer& zpStringBuffer::operator<<( zp_sbyte value )
{
	append( (zp_sbyte)value );
	return (*this);
}
zpStringBuffer& zpStringBuffer::operator<<( zp_short value )
{
	append( (zp_short)value );
	return (*this);
}
zpStringBuffer& zpStringBuffer::operator<<( zp_int value )
{
	append( (zp_int)value );
	return (*this);
}
zpStringBuffer& zpStringBuffer::operator<<( zp_long value )
{
	append( (zp_long)value );
	return (*this);
}

zpStringBuffer& zpStringBuffer::operator<<( zp_byte value )
{
	append( (zp_byte)value );
	return (*this);
}
zpStringBuffer& zpStringBuffer::operator<<( zp_ushort value )
{
	append( (zp_ushort)value );
	return (*this);
}
zpStringBuffer& zpStringBuffer::operator<<( zp_uint value )
{
	append( (zp_uint)value );
	return (*this);
}
zpStringBuffer& zpStringBuffer::operator<<( zp_ulong value )
{
	append( (zp_ulong)value );
	return (*this);
}

zpStringBuffer& zpStringBuffer::operator<<( zp_float value )
{
	append( (zp_float)value );
	return (*this);
}
zpStringBuffer& zpStringBuffer::operator<<( zp_double value )
{
	append( (zp_double)value );
	return (*this);
}
	
zp_bool zpStringBuffer::isEmpty() const
{
	return m_length == 0;
}
zp_uint zpStringBuffer::length() const
{
	return m_length;
}
zp_uint zpStringBuffer::capacity() const
{
	return m_capacity;
}
zp_uint zpStringBuffer::size() const
{
	return m_length;
}

void zpStringBuffer::append( zp_char value )
{
	ensureCapacity( m_length + 2 );
	m_buffer[ m_length ] = value;
	m_length++;
	m_buffer[ m_length ] = '\0';
}
void zpStringBuffer::append( const zp_char* value, zp_int length )
{
	if( value == ZP_NULL ) return;

	if( length < 0 )
	{
		length = zp_strlen( value );
	}

	if( length == 0 )
	{
		return;
	}
	else if( length == 1 )
	{
		append( value[0] );
		return;
	}
	else
	{
		ensureCapacity( m_length + length + 2 );

		zp_char* c = m_buffer + m_length;
		for( zp_int i = 0; i < length; ++i, ++c )
		{
			*c = value[ i ];
		}
		
		m_length += length;
		m_buffer[ m_length ] = '\0';
	}
}
void zpStringBuffer::append( const zpString& value )
{
	append( value.str(), value.length() );
}

void zpStringBuffer::append( zp_sbyte value )
{
	zp_char buff[8];
	sprintf_s( buff, "%d", value );
	append( buff );
}
void zpStringBuffer::append( zp_short value )
{
	zp_char buff[16];
	sprintf_s( buff, "%d", value );
	append( buff );
}
void zpStringBuffer::append( zp_int value )
{
	zp_char buff[32];
	sprintf_s( buff, "%d", value );
	append( buff );
}
void zpStringBuffer::append( zp_long value )
{
	zp_char buff[64];
	sprintf_s( buff, "%d", value );
	append( buff );
}

void zpStringBuffer::append( zp_byte value )
{
	zp_char buff[16];
	sprintf_s( buff, "%u", value );
	append( buff );
}
void zpStringBuffer::append( zp_ushort value )
{
	zp_char buff[16];
	sprintf_s( buff, "%u", value );
	append( buff );
}
void zpStringBuffer::append( zp_uint value )
{
	zp_char buff[32];
	sprintf_s( buff, "%u", value );
	append( buff );
}
void zpStringBuffer::append( zp_ulong value )
{
	zp_char buff[64];
	sprintf_s( buff, "%u", value );
	append( buff );
}

void zpStringBuffer::append( zp_float value )
{
	zp_char buff[64];
	sprintf_s( buff, "%f", value );
	append( buff );
}
void zpStringBuffer::append( zp_double value )
{
	zp_char buff[64];
	sprintf_s( buff, "%f", value );
	append( buff );
}

void zpStringBuffer::prepend( zp_char value ) {}
void zpStringBuffer::prepend( const zp_char* value ) {}

void zpStringBuffer::prepend( zp_sbyte value ) {}
void zpStringBuffer::prepend( zp_short value ) {}
void zpStringBuffer::prepend( zp_int value ) {}
void zpStringBuffer::prepend( zp_long value ) {}

void zpStringBuffer::prepend( zp_byte value ) {}
void zpStringBuffer::prepend( zp_ushort value ) {}
void zpStringBuffer::prepend( zp_uint value ) {}
void zpStringBuffer::prepend( zp_ulong value ) {}

void zpStringBuffer::prepend( zp_float value ) {}
void zpStringBuffer::prepend( zp_double value ) {}

void zpStringBuffer::erase( zp_uint start, zp_uint end ) {}
void zpStringBuffer::clear()
{
	m_length = 0;
	if( m_buffer )
	{
		m_buffer[ 0 ] = '\0';
	}
}

zp_int zpStringBuffer::indexOf( zp_char ch, zp_uint fromIndex ) const
{
	if( fromIndex > m_length ) return npos;

	zp_int pos = npos;
	for( zp_uint i = fromIndex; i < m_length; ++i )
	{
		if( m_buffer[i] == ch )
		{
			pos = i;
			break;
		}
	}

	return pos;
}
zp_int zpStringBuffer::indexOf( const zpString& string, zp_uint fromIndex ) const
{
	if( string.length() > m_length ) return npos;

	zp_char first = string[ 0 ];
	zp_uint count = m_length - string.length();
	zp_uint j;
	zp_uint end;
	zp_uint k;

	for( zp_uint i = fromIndex; i <= count; ++i )
	{
		// find the first character
		if( m_buffer[ i ] != first )
		{
			while( ++i <= count && m_buffer[ i ] != first );
		}

		// look for the rest of the characters
		if( i <= count )
		{
			j = i + 1;
			end = j + string.length() - 1;
			for( k = 1; j < end && m_buffer[ j ] == string[ k ]; ++j, ++k );

			// if whole string found, return index
			if( j == end )
			{
				return i;
			}
		}
	}

	return npos;
}

zpString zpStringBuffer::toString() const
{
	return zpString( m_buffer, m_length );
}
const zp_char* zpStringBuffer::str() const
{
	return m_buffer;
}

void zpStringBuffer::reserve( zp_uint size )
{
	ensureCapacity( size + 1 );
}

void zpStringBuffer::ensureCapacity( zp_uint size )
{
	if( size > m_capacity )
	{
		if( m_capacity == 0 )
		{
			m_capacity = ZP_STRING_BUFFER_DEFAULT_SIZE;
		}

		while( m_capacity < size ) m_capacity *= 2;
		
		zp_char* buff = new zp_char[ m_capacity ];
		if( m_buffer && m_length > 0 )
		{
			zp_strcpy( buff, 1 + m_length * sizeof( zp_char ), m_buffer );
		}
		ZP_SAFE_DELETE_ARRAY( m_buffer );
		
		m_buffer = buff;
	}
}