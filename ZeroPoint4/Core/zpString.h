#pragma once
#ifndef ZP_STRING_H
#define ZP_STRING_H

#define ZP_STRING_MAX_SMALL_SIZE	16

class zpString {
public:
	static const zp_uint npos = -1;

	zpString();
	zpString( const zp_char* string );
	zpString( const zp_char* string, zp_uint length, zp_uint offset = 0 );
	zpString( const zpString& string );
	zpString( const zpString& string, zp_uint length, zp_uint offset = 0 );
	zpString( zpString&& string );
	~zpString();

	zpString& operator=( const zp_char* string );
	zpString& operator=( const zpString& string );
	zpString& operator=( zpString&& string );

	operator zp_hash() const;
	const zp_char* getChars() const;
	const char* c_str() const;

	zp_char operator[]( zp_uint index ) const;
	zp_char charAt( zp_uint index ) const;

	zp_bool startsWith( const zpString& string ) const;
	zp_bool endsWith( const zpString& string ) const;

	zp_uint indexOf( zp_char ch, zp_uint fromIndex = 0 ) const;
	zp_uint indexOf( const zpString& string, zp_uint fromIndex = 0 ) const;

	zp_uint indexOfIgnoreCase( zp_char ch, zp_uint fromIndex = 0 ) const;
	zp_uint indexOfIgnoreCase( const zpString& string, zp_uint fromIndex = 0 ) const;

	zp_uint lastIndexOf( zp_char ch, zp_uint fromIndex = 0 ) const;
	zp_uint lastIndexOf( const zpString& string, zp_uint fromIndex = 0 ) const;

	zp_uint findFirstOf( const zpString& string, zp_uint fromIndex = 0 ) const;

	zp_bool isEmpty() const;
	zp_uint length() const;
	zp_uint capacity() const;
	zp_uint size() const;

	zp_bool equals( const zpString& string ) const;
	zp_bool equalsIgnoreCase( const zpString& string ) const;

	zp_int compareTo( const zpString& string ) const;
	zp_int compareToIgnoreCase( const zpString& string ) const;

	zp_char& operator[]( zp_uint index );
	void setCharAt( zp_uint index, zp_char ch );

	zpString substring( zp_uint startIndex ) const;
	zpString substring( zp_uint startIndex, zp_int endIndex ) const;

	zpString toLower() const;
	zpString toUpper() const;

	zpString& toLower();
	zpString& toUpper();

	zpString toCamelCase( zp_bool capitalFirstLetter = true ) const;

	zp_int scan( const zp_char* format, ... ) const;

	zpString ltrim() const;
	zpString rtrim() const;
	zpString trim() const;

	zpString& ltrim();
	zpString& rtrim();
	zpString& trim();
	
	template<typename Func>
	void foreach( Func func ) {
		const zp_char* p = getChars();
		while( *p ) {
			func( *p );
			++p;
		}
	}
	template<typename Func>
	void map( Func func ) {
		zp_char* p = m_length < ZP_STRING_MAX_SMALL_SIZE ? m_chars : m_string;
		while( *p ) {
			*p = func( *p );
			++p;
		}
	}

private:
	void ensureCapacity( zp_uint size );

	union {
		zp_char m_chars[ ZP_STRING_MAX_SMALL_SIZE ];
		struct {
			zp_char* m_string;
			zp_uint m_capacity;
		};
	};

	zp_uint m_length;
	mutable zp_hash m_hash;
};

zp_bool operator==( const zpString& string1, const zpString& string2 );
zp_bool operator==( const zpString& string1, const zp_char* string2 );
zp_bool operator==( const zp_char* string1, const zpString& string2 );

zp_bool operator!=( const zpString& string1, const zpString& string2 );
zp_bool operator!=( const zpString& string1, const zp_char* string2 );
zp_bool operator!=( const zp_char* string1, const zpString& string2 );

#endif