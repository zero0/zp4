#include "zpCore.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define IS_STRING_PACKED( s )	( (s)->m_length < ZP_STRING_MAX_SMALL_SIZE )
#define NOT_STRING_PACKED( s )	( !IS_STRING_PACKED( s ) )

ZP_FORCE_INLINE zp_char __to_lower( zp_char ch ) {
	return ( 'A' <= ch && ch <= 'Z' ) ? ch -= 'A' - 'a' : ch;
}

ZP_FORCE_INLINE zp_char __to_upper( zp_char ch ) {
	return ( 'a' <= ch && ch <= 'z' ) ? ch -= 'a' - 'A' : ch;
}

zpString::zpString() : 
	m_length( 0 ),
	m_capacity( ZP_STRING_MAX_SMALL_SIZE ),
	m_string( ZP_NULL )
{
	ZP_ON_DEBUG( memset( m_chars, 0, sizeof( zp_char ) * ZP_STRING_MAX_SMALL_SIZE ); );
}
zpString::zpString( const zp_char* string ) : 
	m_length( string ? strlen( string ) : 0 ),
	m_capacity( ZP_STRING_MAX_SMALL_SIZE )
{
	if( IS_STRING_PACKED( this ) ) {
		strcpy_s( m_chars, string );
	} else {
		m_capacity = m_length + 1;
		m_string = new zp_char[ m_capacity ];
		strcpy_s( m_string, m_capacity, string );
	}
}
zpString::zpString( const zp_char* string, zp_uint length, zp_uint offset ) : 
	m_length( length ),
	m_capacity( ZP_STRING_MAX_SMALL_SIZE )
{
	if( IS_STRING_PACKED( this ) ) {
		memcpy_s( m_chars, ZP_STRING_MAX_SMALL_SIZE, string + offset , m_length );
		m_chars[ m_length ] = '\0';
	} else {
		m_string = new zp_char[ m_capacity ];
		memcpy_s( m_string, m_capacity, string + offset, m_length );
		m_string[ m_length ] = '\0';
	}
}
zpString::zpString( const zpString& string ) : 
	m_length( string.m_length ),
	m_capacity( string.m_capacity )
{
	if( IS_STRING_PACKED( this ) ) {
		strcpy_s( m_chars, string.m_chars );
	} else {
		m_string = new zp_char[ m_capacity ];
		strcpy_s( m_string, m_capacity, string.m_string );
	}
}
zpString::zpString( const zpString& string, zp_uint length, zp_uint offset ) :
	m_length( length ),
	m_capacity( length + 1 ) 
{
	if( string.m_length > 0 ) {
		if( IS_STRING_PACKED( &string ) ) {
			if( IS_STRING_PACKED( this ) ) {
				memcpy_s( m_chars, m_capacity, string.m_chars + offset, m_length );
				m_chars[ m_length ] = '\0';
			} else {
				m_string = new zp_char[ m_capacity ];
				memcpy_s( m_string, m_capacity, string.m_chars + offset, m_length );
				m_string[ m_length ] = '\0';
			}
		} else {
			if( IS_STRING_PACKED( this ) ) {
				memcpy_s( m_chars, m_capacity, string.m_string + offset, m_length );
				m_chars[ m_length ] = '\0';
			} else {
				m_string = new zp_char[ m_capacity ];
				memcpy_s( m_string, m_capacity, string.m_string + offset, m_length );
				m_string[ m_length ] = '\0';
			}
		}
	}
}
zpString::zpString( zpString&& string ) : 
	m_capacity( string.m_capacity ), 
	m_length( string.m_length )
{
	if( IS_STRING_PACKED( &string ) ) {
		strcpy_s( m_chars, string.m_chars );
	} else {
		m_string = string.m_string;
	}
	string.m_string = ZP_NULL;
}
zpString::~zpString() {
	if( IS_STRING_PACKED( this ) ) {
		memset( m_chars, 0, sizeof( zp_char ) * ZP_STRING_MAX_SMALL_SIZE );
	} else {
		ZP_SAFE_DELETE_ARRAY( m_string );
	}

	m_length = 0;
	m_capacity = 0;
}

zpString& zpString::operator=( const zp_char* string ) {
	if( m_string != string && m_chars != string ) {
		// delete old buffers
		if( IS_STRING_PACKED( this ) ) {
			memset( m_chars, 0, sizeof( zp_char ) * ZP_STRING_MAX_SMALL_SIZE );
		} else {
			ZP_SAFE_DELETE_ARRAY( m_string );
		}

		m_length = string ? strlen( string ) : 0;
		m_capacity = ZP_STRING_MAX_SMALL_SIZE;

		if( m_length > 0 ) {
			if( IS_STRING_PACKED( this ) ) {
				strcpy_s( m_chars, string );
			} else {
				m_capacity = m_length + 1;
				m_string = new zp_char[ m_capacity ];
				strcpy_s( m_string, m_capacity, string );
			}
		}
	}
	return (*this);
}
zpString& zpString::operator=( const zpString& string ) {
	if( this != &string ) {
		// delete old buffers
		if( IS_STRING_PACKED( this ) ) {
			memset( m_chars, 0, sizeof( zp_char ) * ZP_STRING_MAX_SMALL_SIZE );
		} else {
			ZP_SAFE_DELETE_ARRAY( m_string );
		}

		m_length = string.m_length;
		m_capacity = string.m_capacity;

		if( m_length > 0 ) {
			if( IS_STRING_PACKED( this ) ) {
				strcpy_s( m_chars, string.m_chars );
			} else {
				m_string = new zp_char[ m_capacity ];
				strcpy_s( m_string, m_capacity, string.m_string );
			}
		}
	}
	return (*this);
}
zpString& zpString::operator=( zpString&& string ) {
	if( this != &string ) {
		// delete old buffers
		if( IS_STRING_PACKED( this ) ) {
			memset( m_chars, 0, sizeof( zp_char ) * ZP_STRING_MAX_SMALL_SIZE );
		} else {
			ZP_SAFE_DELETE_ARRAY( m_string );
		}

		// set to other string
		m_length = string.m_length;
		m_capacity = string.m_capacity;
		if( IS_STRING_PACKED( &string ) ) {
			strcpy_s( m_chars, string.m_chars );
		} else {
			m_string = string.m_string;
		}

		// unreference moved buffer
		string.m_string = ZP_NULL;
	}
	return (*this);
}
/*
zpString::operator const zp_char*() const {
	return m_string;
}
*/
const zp_char* zpString::getChars() const {
	return IS_STRING_PACKED( this ) ? m_chars : m_string;
}
const char* zpString::c_str() const {
	return IS_STRING_PACKED( this ) ? m_chars : m_string;
}

zp_char zpString::operator[]( zp_uint index ) const {
	ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );
	return IS_STRING_PACKED( this ) ? m_chars[ index ] : m_string[ index ];
}
zp_char zpString::charAt( zp_uint index ) const {
	ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );
	return IS_STRING_PACKED( this ) ? m_chars[ index ] : m_string[ index ];
}

zp_bool zpString::startsWith( const zpString& string ) const {
	if( string.m_length > m_length ) return false;

	for( zp_uint i = 0; i < string.m_length; ++i ) {
		if( charAt( i ) != string[ i ] ) return false;
	}

	return true;
}
zp_bool zpString::endsWith( const zpString& string ) const {
	if( string.m_length > m_length ) return false;
	
	for( zp_uint i = m_length, j = string.m_length; i --> 0 && j --> 0; ) {
		if( charAt( i ) != string[ j ] ) return false;
	}

	return true;
}

zp_uint zpString::indexOf( zp_char ch, zp_uint fromIndex ) const {
	if( fromIndex > m_length ) return npos;

	zp_uint pos = npos;
	for( zp_uint i = fromIndex; i < m_length; ++i ) {
		if( charAt( i ) == ch ) {
			pos = i;
			break;
		}
	}

	return pos;
}
zp_uint zpString::indexOf( const zpString& string, zp_uint fromIndex ) const {
	if( string.m_length > m_length ) return npos;

	zp_char first = string[ 0 ];
	zp_uint count = m_length - string.m_length;
	zp_uint j;
	zp_uint end;
	zp_uint k;

	for( zp_uint i = fromIndex; i <= count; ++i ) {
		// find the first character
		if( charAt( i ) != first ) {
			while( ++i <= count && charAt( i ) != first );
		}

		// look for the rest of the characters
		if( i <= count ) {
			j = i + 1;
			end = j + string.m_length - 1;
			for( k = 1; j < end && charAt( j ) == string[ k ]; ++j, ++k );

			// if whole string found, return index
			if( j == end ) {
				return i;
			}
		}
	}

	return npos;
}

zp_bool zpString::isEmpty() const {
	return m_length == 0;
}
zp_uint zpString::length() const {
	return m_length;
}
zp_uint zpString::capacity() const {
	return m_capacity;
}

zp_bool zpString::equals( const zpString& string ) const {
	if( m_length != string.m_length ) return false;

	return compareTo( string ) == 0;
}
zp_bool zpString::equalsIgnoreCase( const zpString& string ) const {
	if( m_length != string.m_length ) return false;

	return compareToIgnoreCase( string ) == 0;
}

zp_int zpString::compareTo( const zpString& string ) const {
	zp_uint count = ZP_MIN( m_length, string.m_length );
	zp_char a, b;

	for( zp_uint i = 0; i < count; ++i ) {
		a = charAt( i );
		b = string[ i ];

		if( a != b ) return a - b;
	}

	return m_length - string.m_length;
}
zp_int zpString::compareToIgnoreCase( const zpString& string ) const {
	zp_uint count = ZP_MIN( m_length, string.m_length );
	zp_char a, b;
	
	for( zp_uint i = 0; i < count; ++i ) {
		a = __to_lower( charAt( i ) );
		b = __to_lower( string[ i ] );

		if( a != b ) return a - b;
	}

	return m_length - string.m_length;
}

zp_char& zpString::operator[]( zp_uint index ) {
	ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );
	return IS_STRING_PACKED( this ) ? m_chars[ index ] : m_string[ index ];
}
void zpString::setCharAt( zp_uint index, zp_char ch ) {
	ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );

	if( IS_STRING_PACKED( this ) ) {
		m_chars[ index ] = ch;
	} else {
		m_string[ index ] = ch;
	}
}

zpString zpString::substring( zp_uint startIndex ) const {
	if( startIndex > m_length ) return zpString();

	zp_uint count = m_length - startIndex;

	return zpString( *this, count, startIndex );
}
zpString zpString::substring( zp_uint startIndex, zp_int endIndex ) const {
	if( endIndex < 0 ) endIndex = m_length + endIndex;
	if( (zp_uint)endIndex <= startIndex ) return zpString();

	zp_uint count = endIndex - startIndex;

	return zpString( *this, count, startIndex );
}

zpString zpString::toLower() const {
	zpString lower( *this );
	zp_char* p;

	if( IS_STRING_PACKED( &lower ) ) {
		p = lower.m_chars;
	} else {
		p = lower.m_string;
	}

	while( *p ) {
		*p = __to_lower( *p );
		p++;
	}

	return lower;
}
zpString zpString::toUpper() const {
	zpString upper( *this );
	zp_char* p;

	if( IS_STRING_PACKED( &upper ) ) {
		p = upper.m_chars;
	} else {
		p = upper.m_string;
	}

	while( *p ) {
		*p = __to_upper( *p );
		p++;
	}

	return upper;
}

zp_int zpString::scan( const zp_char* format, ... ) const {
	va_list args;
	zp_int ret = 0;

	va_start( args, format );
	ret = sscanf_s( getChars(), format, args );
	va_end( args );

	return ret;
}
/*
zpString zpString::format( const zpString& format, ... ) {
	zp_char buff[ 256 ];
	va_list args;

	va_start( args, format );
	sprintf_s( buff, format.getChars(), args );
	va_end( args );

	return zpString( buff );
}
*/
zpString zpString::__format( zp_char* buff, zp_uint size, const zpString& format, ... ) {
	va_list args;

	va_start( args, format );
	sprintf_s( buff, size, format.getChars(), args );
	va_end( args );

	return zpString( buff );
}
zp_bool operator==( const zpString& string1, const zpString& string2 ) {
	return string1.equals( string2 );
}

zp_bool operator!=( const zpString& string1, const zpString& string2 ) {
	return !string1.equals( string2 );
}