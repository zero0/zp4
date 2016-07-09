#pragma once
#ifndef ZP_STRING_H
#define ZP_STRING_H

class zpString
{
public:
    enum : zp_size_t
    {
        npos = (zp_size_t)-1,
    };

    zpString();
    explicit zpString( const zp_char* string );
    zpString( const zp_char* string, zp_size_t length );
    zpString( const zpString& string );
    zpString( const zpString& string, zp_size_t length );
    zpString( zpString&& string );
    ~zpString();

    void operator=( const zp_char* string );
    void operator=( const zpString& string );
    void operator=( zpString&& string );

    operator zp_hash() const;
    const zp_char* str() const;

    zp_char operator[]( zp_size_t index ) const;
    zp_char& operator[]( zp_size_t index );
    void setCharAt( zp_size_t index, zp_char ch );
    zp_char charAt( zp_size_t index ) const;
    void set( const zp_char* str, zp_size_t length );

    zp_bool startsWith( zp_char ch ) const;
    zp_bool startsWith( const zp_char* string ) const;
    zp_bool startsWith( const zpString& string ) const;

    zp_bool endsWith( zp_char ch ) const;
    zp_bool endsWith( const zp_char* string ) const;
    zp_bool endsWith( const zpString& string ) const;

    zp_size_t indexOf( zp_char ch, zp_size_t fromIndex = 0 ) const;
    zp_size_t indexOf( const zp_char* string, zp_size_t fromIndex = 0 ) const;
    zp_size_t indexOf( const zpString& string, zp_size_t fromIndex = 0 ) const;

    zp_size_t indexOfIgnoreCase( zp_char ch, zp_size_t fromIndex = 0 ) const;
    zp_size_t indexOfIgnoreCase( const zp_char* string, zp_size_t fromIndex = 0 ) const;
    zp_size_t indexOfIgnoreCase( const zpString& string, zp_size_t fromIndex = 0 ) const;

    zp_size_t lastIndexOf( zp_char ch, zp_size_t fromIndex = 0 ) const;
    zp_size_t lastIndexOf( const zp_char* string, zp_size_t fromIndex = 0 ) const;
    zp_size_t lastIndexOf( const zpString& string, zp_size_t fromIndex = 0 ) const;

    zp_size_t findFirstOf( const zpString& string, zp_size_t fromIndex = 0 ) const;

    zp_bool isEmpty() const;
    zp_size_t length() const;

    zp_bool equals( const zpString& string ) const;
    zp_bool equalsIgnoreCase( const zpString& string ) const;

    zp_int compareTo( const zpString& string ) const;
    zp_int compareToIgnoreCase( const zpString& string ) const;
    
    zpString substring( zp_size_t startIndex ) const;
    zpString substring( zp_size_t startIndex, zp_size_t endIndex ) const;

    void erase( zp_size_t startIndex, zp_size_t count );
    void append( zp_char ch );
    void append( const zp_char* str, zp_size_t length );
    void append( const zpString& string );

    static void toLower( zpString& string );
    static void toUpper( zpString& string );
    static void join( zpString& string, const zpArrayList< zpString >& parts, const zp_char* delim );

    zpString toCamelCase( zp_bool capitalFirstLetter = true ) const;
    
    zpString ltrim() const;
    zpString rtrim() const;
    zpString trim() const;

    void split( zp_char delim, zpArrayList< zpString >& parts ) const;
    void split( zp_char delim, zpArrayList< zpString >& parts, zp_int maxSplits ) const;
    
    template<typename Func>
    void foreach( Func func )
    {
        const zp_char* p = str();
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

    void reserve( zp_size_t size );
    void clear();

private:
    void ensureCapacity( zp_size_t size );
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
            zp_size_t m_capacity;
        };
    };

    zp_size_t m_length;
    mutable zp_hash m_hash;
};

zp_bool operator==( const zpString& string1, const zpString& string2 );
zp_bool operator==( const zpString& string1, const zp_char* string2 );
zp_bool operator==( const zp_char* string1, const zpString& string2 );

zp_bool operator!=( const zpString& string1, const zpString& string2 );
zp_bool operator!=( const zpString& string1, const zp_char* string2 );
zp_bool operator!=( const zp_char* string1, const zpString& string2 );

#endif