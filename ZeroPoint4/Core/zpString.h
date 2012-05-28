#pragma once
#ifndef ZP_STRING_H
#define ZP_STRING_H

#define ZP_STRING_USE_LONG	0

class zpString {
public:
	static const zp_uint npos = -1;

	zpString();
	explicit zpString( const zp_char* string );
	zpString( const zp_char* string, zp_uint length, zp_uint offset = 0 );
	zpString( const zpString& string );
	zpString( const zpString& string, zp_uint length, zp_uint offset = 0 );
	zpString( zpString&& string );
	~zpString();

	zpString& operator=( const zpString& string );
	zpString& operator=( zpString&& string );

	//operator const zp_char*() const;
	const zp_char* getChars() const;

	zp_char operator[]( zp_uint index ) const;
	zp_char charAt( zp_uint index ) const;

	zp_bool startsWith( const zpString& string ) const;
	zp_bool endsWith( const zpString& string ) const;

	zp_uint indexOf( zp_char ch, zp_uint fromIndex = 0 ) const;
	zp_uint indexOf( const zpString& string, zp_uint fromIndex = 0 ) const;

	zp_bool isEmpty() const;
	zp_uint length() const;
	zp_uint capacity() const;

	zp_bool equals( const zpString& string ) const;
	zp_bool equalsIgnoreCase( const zpString& string ) const;

	zp_int compareTo( const zpString& string ) const;
	zp_int compareToIgnoreCase( const zpString& string ) const;

	void setCharAt( zp_uint index, zp_char ch );

	zpString substring( zp_uint startIndex ) const;
	zpString substring( zp_uint startIndex, zp_int endIndex ) const;

private:
	void initialize( const zp_char* string, zp_uint length, zp_uint offset );

#if ZP_STRING_USE_LONG
	union {
		zp_lptr m_string;
		zp_char* m_ptr;
		zp_char m_chars[8];
	};
#else
	union {
		zp_char* m_string;
		zp_char m_chars[4];
	};
#endif

	zp_uint m_capacity;
	zp_uint m_length;
};

zp_bool operator==( const zpString& string1, const zpString& string2 );

zp_bool operator!=( const zpString& string1, const zpString& string2 );
#endif