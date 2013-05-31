#pragma once
#ifndef ZP_STRING_H
#define ZP_STRING_H

class zpString {
public:
	enum
	{
		npos = -1,
	};

	zpString();
	explicit zpString( const zp_char* string );
	zpString( const zp_char* string, zp_uint length );
	zpString( const zpString& string );
	zpString( const zpString& string, zp_uint length );
	zpString( zpString&& string );
	~zpString();

	void operator=( const zp_char* string );
	void operator=( const zpString& string );
	void operator=( zpString&& string );

	operator zp_hash() const;
	const zp_char* getChars() const;

	zp_char operator[]( zp_uint index ) const;
	zp_char& operator[]( zp_uint index );
	void setCharAt( zp_uint index, zp_char ch );
	zp_char charAt( zp_uint index ) const;
	void set( const zp_char* str, zp_uint length );

	zp_bool startsWith( const zp_char* string ) const;
	zp_bool startsWith( const zpString& string ) const;

	zp_bool endsWith( const zp_char* string ) const;
	zp_bool endsWith( const zpString& string ) const;

	zp_int indexOf( zp_char ch, zp_uint fromIndex = 0 ) const;
	zp_int indexOf( const zp_char* string, zp_uint fromIndex = 0 ) const;
	zp_int indexOf( const zpString& string, zp_uint fromIndex = 0 ) const;

	zp_int indexOfIgnoreCase( zp_char ch, zp_uint fromIndex = 0 ) const;
	zp_int indexOfIgnoreCase( const zp_char* string, zp_uint fromIndex = 0 ) const;
	zp_int indexOfIgnoreCase( const zpString& string, zp_uint fromIndex = 0 ) const;

	zp_int lastIndexOf( zp_char ch, zp_int fromIndex = 0 ) const;
	zp_int lastIndexOf( const zp_char* string, zp_int fromIndex = 0 ) const;
	zp_int lastIndexOf( const zpString& string, zp_int fromIndex = 0 ) const;

	zp_int findFirstOf( const zpString& string, zp_uint fromIndex = 0 ) const;

	zp_bool isEmpty() const;
	zp_uint length() const;

	zp_bool equals( const zpString& string ) const;
	zp_bool equalsIgnoreCase( const zpString& string ) const;

	zp_int compareTo( const zpString& string ) const;
	zp_int compareToIgnoreCase( const zpString& string ) const;
	
	void substring( zpString& out, zp_uint startIndex ) const;
	void substring( zpString& out, zp_uint startIndex, zp_int endIndex ) const;

	void erase( zp_int startIndex, zp_uint count );
	void append( zp_char ch );
	void append( const zp_char* str, zp_int length = -1 );
	void append( const zpString& string );

	static void toLower( zpString& string );
	static void toUpper( zpString& string );
	static void join( zpString& string, const zpArrayList< zpString >& parts, const zp_char* delim );

	void toCamelCase( zpString& outString, zp_bool capitalFirstLetter = true ) const;
	
	void ltrim( zpString& out ) const;
	void rtrim( zpString& out ) const;
	void trim( zpString& out ) const;

	void split( zp_char delim, zpArrayList< zpString >& slip ) const;
	
	template<typename Func>
	void foreach( Func func )
	{
		const zp_char* p = getChars();
		while( *p )
		{
			func( *p );
			++p;
		}
	}
	template<typename Func>
	void map( Func func )
	{
		zp_char* p = getCharsInternal();
		while( *p )
		{
			*p = func( *p );
			++p;
		}
	}

	void reserve( zp_uint size );
	void clear();

private:
	void ensureCapacity( zp_uint size );
	zp_char* getCharsInternal();

	enum
	{
		ZP_STRING_MAX_SMALL_SIZE = 16,
	};

	union
	{
		zp_char m_chars[ ZP_STRING_MAX_SMALL_SIZE ];
		struct
		{
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