#include "zpCore.h"
#include <string.h>

#define IS_STRING_PACKED	( m_length <= sizeof( m_string ) )
#define NOT_STRING_PACKED	( !( m_length <= sizeof( m_string ) ) )

#if ZP_STRING_USE_LONG
#define CHAR_AT_PACKED( a )	(zp_char)( (zp_lptr)m_string >> ( sizeof( zp_char ) * (a) * 8 ) )
#define PACK_CHAR( ch, i )	( (zp_lptr)(ch) << ( sizeof( zp_char ) * (i) * 8 ) )

ZP_FORCE_INLINE zp_lptr __pack_string( const zp_char* string, zp_uint length ) {
	zp_lptr packedString = 0;
#else 
#define CHAR_AT_PACKED( a )	(zp_char)( (zp_ptr)m_string >> ( sizeof( zp_char ) * (a) * 8 ) )
#define PACK_CHAR( ch, i )	( (zp_ptr)(ch) << ( sizeof( zp_char ) * (i) * 8 ) )

ZP_FORCE_INLINE zp_ptr __pack_string( const zp_char* string, zp_uint length ) {
	zp_ptr packedString = 0;
#endif
	
	for( zp_uint i = 0; i < length; ++i ) {
		packedString |= PACK_CHAR( string[i], i );
	}
	return packedString;
}

ZP_FORCE_INLINE zp_char __to_lower( zp_char ch ) {
	return ( 'A' <= ch && ch <= 'Z' ) ? ch - 'A' - 'a' : ch;
}

ZP_FORCE_INLINE zp_char __to_upper( zp_char ch ) {
	return ( 'a' <= ch && ch <= 'z' ) ? ch - 'a' - 'A' : ch;
}

zpString::zpString() : 
#if ZP_STRING_USE_LONG
	m_string( 0 ), 
#else 
	m_string( ZP_NULL ), 
#endif
	m_capacity( sizeof( m_string ) ),
	m_length( 0 )
{}
zpString::zpString( const zp_char* string ) : 
#if ZP_STRING_USE_LONG
	m_string( 0 ), 
#else 
	m_string( ZP_NULL ), 
#endif 
	m_capacity( string ? strlen( string ) + 1 : 0 ), 
	m_length( m_capacity - 1 )
{
	if( m_length > 0 ) {
		// if the length of the string is less than the size of a pointer
		//  pack it into the pointer instead of creating new array
		if( IS_STRING_PACKED ) {
			m_capacity = sizeof( m_string );
#if ZP_STRING_USE_LONG
			m_string = __pack_string( string, m_length );
#else
			m_string = (zp_char*)__pack_string( string, m_length );
#endif
		} else {
#if ZP_STRING_USE_LONG
			m_ptr = new zp_char[ m_capacity ];
			strcpy_s( m_ptr, m_capacity, string );
#else
			m_string = new zp_char[ m_capacity ];
			strcpy_s( m_string, m_capacity, string );
#endif
		}
	}
}
zpString::zpString( const zp_char* string, zp_uint length, zp_uint offset ) : 
#if ZP_STRING_USE_LONG
	m_string( 0 ), 
#else 
	m_string( ZP_NULL ), 
#endif
	m_capacity( length + 1 ), 
	m_length( length )
{
	if( m_length > 0 ) {
		// if the length of the string is less than the size of a pointer
		//  pack it into the pointer instead of creating new array
		if( IS_STRING_PACKED ) {
			m_capacity = sizeof( m_string );
#if ZP_STRING_USE_LONG
			m_string = __pack_string( string + offset, m_length );
#else
			m_string = (zp_char*)__pack_string( string + offset, m_length );
#endif
		} else {
#if ZP_STRING_USE_LONG
			m_ptr = new zp_char[ m_capacity ];
			strcpy_s( m_ptr, m_capacity, string + offset );
#else
			m_string = new zp_char[ m_capacity ];
			memcpy_s( m_string, m_capacity, string + offset, m_length );
			m_string[ m_length ] = '\0';
#endif
		}
	}
}
zpString::zpString( const zpString& string ) : 
#if ZP_STRING_USE_LONG
	m_string( 0 ), 
#else 
	m_string( ZP_NULL ), 
#endif
	m_capacity( string.m_capacity ), 
	m_length( string.m_length )
{
	
	if( m_length > 0 ) {
		// if the length of the string is less than the size of a pointer
		//  pack it into the pointer instead of creating new array
		if( IS_STRING_PACKED ) {
#if ZP_STRING_USE_LONG
			m_string = string.m_string;
#else
			m_string = string.m_string;
#endif
		} else {
#if ZP_STRING_USE_LONG
			m_ptr = new zp_char[ m_capacity ];
			strcpy_s( m_ptr, m_capacity, string.m_ptr );
#else
			m_string = new zp_char[ m_capacity ];
			strcpy_s( m_string, m_capacity, string.m_string );
#endif
		}
	}
	
}
zpString::zpString( const zpString& string, zp_uint length, zp_uint offset ) :
#if ZP_STRING_USE_LONG
	m_string( 0 ), 
#else 
	m_string( ZP_NULL ), 
#endif
	m_capacity( length + 1 ), 
	m_length( length )
{
	if( string.m_length > 0 ) {
		// if the length of the string is less than the size of a pointer
		//  pack it into the pointer instead of creating new array
		if( string.m_length <= sizeof( m_string ) ) {
			m_capacity = sizeof( m_string );
#if ZP_STRING_USE_LONG
			zp_lptr shift = string.m_length - length - offset;
			m_string = string.m_string << shift * sizeof( zp_char ) * 8;
			m_string = ((zp_lptr)m_string >> ( offset + shift ) * sizeof( zp_char ) * 8);
#else
			zp_ptr shift = string.m_length - length - offset;
			m_string = (zp_char*)((zp_ptr)string.m_string << shift * sizeof( zp_char ) * 8);
			m_string = (zp_char*)((zp_ptr)m_string >> ( offset + shift ) * sizeof( zp_char ) * 8);
#endif
		} else if( IS_STRING_PACKED ) {
			
		} else {
#if ZP_STRING_USE_LONG
			m_ptr = new zp_char[ m_capacity ];
			strcpy_s( m_ptr, m_capacity, string.m_ptr + offset );
#else
			m_string = new zp_char[ m_capacity ];
			memcpy_s( m_string, m_capacity, string.m_string + offset, m_length );
			m_string[ m_length ] = '\0';
#endif
		}
	}
}
zpString::zpString( zpString&& string ) : 
	m_string( string.m_string ), 
	m_capacity( string.m_capacity ), 
	m_length( string.m_length )
{
#if ZP_STRING_USE_LONG
	string.m_string = 0;
#else
	string.m_string = ZP_NULL;
#endif
	string.m_length = 0;
	string.m_capacity = 0;
}
zpString::~zpString() {
	if( NOT_STRING_PACKED ) {
#if ZP_STRING_USE_LONG
		if( m_ptr ) {
			delete[] m_ptr;
		}
#else
		ZP_SAFE_DELETE_ARRAY( m_string );
#endif
	}
#if ZP_STRING_USE_LONG
	m_string = 0;
#else
	m_string = ZP_NULL;
#endif
	m_length = 0;
	m_capacity = 0;
}

zpString& zpString::operator=( const zpString& string ) {
	if( this != &string ) {
		if( NOT_STRING_PACKED ) {
#if ZP_STRING_USE_LONG
			if( m_ptr ) {
				delete[] m_ptr;
			}
#else
			ZP_SAFE_DELETE_ARRAY( m_string );
#endif
		}

		m_length = string.m_length;
		m_capacity = string.m_capacity;

		if( m_length > 0 ) {
			// if the length of the string is less than the size of a pointer
			//  pack it into the pointer instead of creating new array
			if( IS_STRING_PACKED ) {
#if ZP_STRING_USE_LONG
				m_string = string.m_string;
#else
				m_string = string.m_string;
#endif
			} else {
#if ZP_STRING_USE_LONG
				m_ptr = new zp_char[ m_capacity ];
				strcpy_s( m_ptr, m_capacity, string.m_ptr );
#else
				m_string = new zp_char[ m_capacity ];
				strcpy_s( m_string, m_capacity, string.m_string );
#endif
			}
		}
	}
	return *this;
}
zpString& zpString::operator=( zpString&& string ) {
	if( this != &string ) {
		if( NOT_STRING_PACKED ) {
#if ZP_STRING_USE_LONG
			//ZP_SAFE_DELETE_ARRAY( (zp_char*)m_string );
			if( m_ptr ) {
				delete[] m_ptr;
			}
#else
			ZP_SAFE_DELETE_ARRAY( m_string );
#endif
		}

		m_string = string.m_string;
		m_length = string.m_length;
		m_capacity = string.m_capacity;

#if ZP_STRING_USE_LONG
		string.m_string = 0;
#else
		string.m_string = ZP_NULL;
#endif
		string.m_length = 0;
		string.m_capacity = 0;
	}
	return *this;
}
/*
zpString::operator const zp_char*() const {
	return m_string;
}
*/
const zp_char* zpString::getChars() const {
	if( IS_STRING_PACKED ) {
#if ZP_STRING_USE_LONG
		return m_chars;
#else
		return m_chars;
#endif
	} else {
#if ZP_STRING_USE_LONG
		return m_ptr;
#else
		return m_string;
#endif
	}
}
zp_char zpString::operator[]( zp_uint index ) const {
	ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );
	return IS_STRING_PACKED ? CHAR_AT_PACKED( index ) : 
#if ZP_STRING_USE_LONG
		m_ptr[ index ]
#else
		m_string[ index ]
#endif
	;
}
zp_char zpString::charAt( zp_uint index ) const {
	ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );
	return IS_STRING_PACKED ? CHAR_AT_PACKED( index ) : 
#if ZP_STRING_USE_LONG
		m_ptr[ index ]
#else
		m_string[ index ]
#endif
	;
}

zp_bool zpString::startsWith( const zpString& string ) const {
	if( string.m_length > m_length ) return false;
	if( string.m_length == m_length ) return equals( (zpString&&)string );

	for( zp_uint i = 0; i < string.m_length; ++i ) {
		if( charAt( i ) != string[ i ] ) return false;
	}

	return true;
}
zp_bool zpString::endsWith( const zpString& string ) const {
	if( string.m_length > m_length ) return false;
	if( string.m_length == m_length ) return equals( (zpString&&)string );
	
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
	if( IS_STRING_PACKED && string.m_length <= sizeof( m_string ) ) {
		return (zp_int)( m_string - string.m_string );
	} else {
		zp_uint count = ZP_MIN( m_length, string.m_length );
		zp_char a, b;

		for( zp_uint i = 0; i < count; ++i ) {
			a = charAt( i );
			b = string[ i ];

			if( a != b ) return a - b;
		}

		return m_length - string.m_length;
	}
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

void zpString::setCharAt( zp_uint index, zp_char ch ) {
	ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );
	if( IS_STRING_PACKED ) {
#if ZP_STRING_USE_LONG
		zp_lptr clear = PACK_CHAR( 0x0ff, index );
		m_string &= ~clear;
		m_string |= PACK_CHAR( ch, index );
#else
		zp_ptr p = (zp_ptr)m_string;

		zp_ptr clear = PACK_CHAR( 0x0ff, index );
		p &= ~clear;
		p |= PACK_CHAR( ch, index );

		m_string = (zp_char*)p;
#endif
	} else {
#if ZP_STRING_USE_LONG
		m_ptr[ index ] = ch
#else
		m_string[ index ] = ch
#endif
			;
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

zp_bool operator==( const zpString& string1, const zpString& string2 ) {
	return string1.equals( string2 );
}

zp_bool operator!=( const zpString& string1, const zpString& string2 ) {
	return !string1.equals( string2 );
}