#include "zpCore.h"

#define IS_STRING_PACKED( end )    ( ( (end)->m_length ) < ZP_STRING_MAX_SMALL_SIZE)
#define NOT_STRING_PACKED( end )    ( !IS_STRING_PACKED( end ) )

zpString::zpString()
    : m_length( 0 )
    , m_hash( 0 )
{
    m_chars[ 0 ] = '\0';
}
zpString::zpString( const zp_char* string )
    : m_length( string ? zp_strlen( string ) : 0 )
    , m_hash( 0 )
{
    if( m_length > 0 )
    {
        if( IS_STRING_PACKED( this ) )
        {
            zp_strncpy( m_chars, ZP_STRING_MAX_SMALL_SIZE, string, m_length );
        }
        else
        {
            m_capacity = m_length + 1;

            m_string = new zp_char[ m_capacity ];
            zp_strcpy( m_string, m_capacity * sizeof( zp_char ), string );
        }
    }
    getCharsInternal()[ m_length ] = '\0';
}
zpString::zpString( const zp_char* string, zp_size_t length )
    : m_length( length )
    , m_hash( 0 )
{
    if( m_length > 0 )
    {
        if( IS_STRING_PACKED( this ) )
        {
            zp_strncpy( m_chars, ZP_STRING_MAX_SMALL_SIZE, string, m_length );
        }
        else
        {
            m_capacity = m_length + 1;

            m_string = new zp_char[ m_capacity ];
            zp_strncpy( m_string, m_capacity * sizeof( zp_char ), string, m_length );
        }
    }
    getCharsInternal()[ m_length ] = '\0';
}
zpString::zpString( const zpString& string )
    : m_length( string.m_length )
    , m_hash( string.m_hash )
{
    if( m_length > 0 )
    {
        if( IS_STRING_PACKED( this ) )
        {
            zp_strncpy( m_chars, ZP_STRING_MAX_SMALL_SIZE, string.str(), m_length );
        }
        else
        {
            m_capacity = m_length + 1;

            m_string = new zp_char[ m_capacity ];
            zp_strcpy( m_string, m_capacity * sizeof( zp_char ), string.str() );
        }
    }
    getCharsInternal()[ m_length ] = '\0';
}
zpString::zpString( const zpString& string, zp_size_t length )
    : m_length( length )
    , m_hash( 0 )
{
    if( m_length > 0 )
    {
        if( IS_STRING_PACKED( this ) )
        {
            zp_strncpy( m_chars, ZP_STRING_MAX_SMALL_SIZE, string.str(), m_length );
        }
        else
        {
            m_capacity = m_length + 1;

            m_string = new zp_char[ m_capacity ];
            zp_strcpy( m_string, m_capacity * sizeof( zp_char ), string.str() );
        }
    }
    getCharsInternal()[ m_length ] = '\0';
}
zpString::zpString( zpString&& string )
    : m_length( string.m_length )
    , m_hash( string.m_hash )
{
    if( IS_STRING_PACKED( this ) )
    {
        zp_strcpy( m_chars, string.m_chars );
    }
    else
    {
        m_string = string.m_string;
        m_capacity = string.m_capacity;
        string.m_string = ZP_NULL;
    }
}
zpString::~zpString()
{
    if( IS_STRING_PACKED( this ) )
    {
        zp_zero_memory_array( m_chars );
    }
    else
    {
        ZP_SAFE_DELETE_ARRAY( m_string );
    }

    m_length = 0;
    m_hash = 0;
}

void zpString::operator=( const zp_char* string )
{
    if( m_string != string && m_chars != string )
    {
        m_hash = 0;
        if( m_length > 0 )
        {
            if( IS_STRING_PACKED( this ) )
            {
                zp_zero_memory_array( m_chars );
            }
            else
            {
                ZP_SAFE_DELETE_ARRAY( m_string );
            }
        }

        m_length = string ? zp_strlen( string ) : 0;

        if( m_length > 0 )
        {
            if( IS_STRING_PACKED( this ) )
            {
                zp_strncpy( m_chars, ZP_STRING_MAX_SMALL_SIZE, string, m_length );
            }
            else
            {
                m_capacity = m_length + 1;

                m_string = new zp_char[ m_capacity ];
                zp_strcpy( m_string, m_capacity * sizeof( zp_char ), string );
            }
        }
        getCharsInternal()[ m_length ] = '\0';
    }
}
void zpString::operator=( const zpString& string ) {
    if( this != &string )
    {
        if( m_length > 0 )
        {
            if( IS_STRING_PACKED( this ) )
            {
                zp_zero_memory_array( m_chars );
            }
            else
            {
                ZP_SAFE_DELETE_ARRAY( m_string );
            }
        }

        m_length = string.m_length;
        m_hash = string.m_hash;

        if( m_length > 0 )
        {
            if( IS_STRING_PACKED( this ) )
            {
                zp_strncpy( m_chars, ZP_STRING_MAX_SMALL_SIZE, string.str(), m_length );
            }
            else
            {
                m_capacity = m_length + 1;

                m_string = new zp_char[ m_capacity ];
                zp_strcpy( m_string, m_capacity * sizeof( zp_char ), string.str() );
            }
        }
        getCharsInternal()[ m_length ] = '\0';
    }
}
void zpString::operator=( zpString&& string )
{
    if( this != &string )
    {
        if( IS_STRING_PACKED( this ) )
        {
            zp_zero_memory_array( m_chars );
        }
        else
        {
            ZP_SAFE_DELETE_ARRAY( m_string );
        }

        m_length = string.m_length;
        m_hash = string.m_hash;

        if( m_length > 0 )
        {
            if( IS_STRING_PACKED( this ) )
            {
                zp_strncpy( m_chars, ZP_STRING_MAX_SMALL_SIZE, string.str(), m_length );
            }
            else
            {
                m_string = string.m_string;
                m_capacity = string.m_capacity;
                string.m_string = ZP_NULL;
            }
        }
    }
}

zpString::operator zp_hash() const
{
    if( m_hash == 0 )
    {
        m_hash = zp_fnv1_32_string( str(), m_hash );
    }

    return m_hash;
}

const zp_char* zpString::str() const
{
    return IS_STRING_PACKED( this ) ? m_chars : m_string;
}
zp_char* zpString::getCharsInternal()
{
    return IS_STRING_PACKED( this ) ? m_chars : m_string;
}

zp_char zpString::operator[]( zp_size_t index ) const
{
    ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );
    return str()[ index ];
}
zp_char& zpString::operator[]( zp_size_t index )
{
    ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );
    return getCharsInternal()[ index ];
}
void zpString::setCharAt( zp_size_t index, zp_char ch )
{
    ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );
    getCharsInternal()[ index ] = ch;
}
zp_char zpString::charAt( zp_size_t index ) const
{
    ZP_ASSERT( index < m_length, "zpString: Index %d out of range %d", index, m_length );
    return str()[ index ];
}

zp_bool zpString::startsWith( zp_char ch ) const
{
    if( m_length == 0 ) return false;

    return str()[ 0 ] == ch;
}
zp_bool zpString::startsWith( const zp_char* string ) const
{
    zp_size_t len = zp_strlen( string );
    if( len > m_length ) return false;

    const zp_char* a = str();
    for( zp_size_t i = 0; i < len; ++i )
    {
        if( a[ i ] != string[ i ] ) return false;
    }

    return true;
}
zp_bool zpString::startsWith( const zpString& string ) const
{
    if( string.m_length > m_length ) return false;

    const zp_char* a = str();
    const zp_char* b = string.str();
    for( zp_size_t i = 0; i < string.m_length; ++i )
    {
        if( a[ i ] != b[ i ] ) return false;
    }

    return true;
}

zp_bool zpString::endsWith( zp_char ch ) const
{
    if( m_length == 0 ) return false;

    return str()[ m_length - 1 ] == ch;
}
zp_bool zpString::endsWith( const zp_char* string ) const
{
    zp_size_t len = zp_strlen( string );
    if( len > m_length ) return false;

    const zp_char* a = str();
    for( zp_size_t i = m_length - 1, j = len - 1; i != npos && j != npos; --i, --j )
    {
        if( a[ i ] != string[ j ] ) return false;
    }

    return true;
}
zp_bool zpString::endsWith( const zpString& string ) const
{
    if( string.m_length > m_length ) return false;
    
    const zp_char* a = str();
    const zp_char* b = string.str();
    for( zp_size_t i = m_length - 1, j = string.m_length - 1; i != npos && j != npos; --i, --j )
    {
        if( a[ i ] != b[ j ] ) return false;
    }

    return true;
}

zp_size_t zpString::indexOf( zp_char ch, zp_size_t fromIndex ) const
{
    if( fromIndex > m_length ) return npos;

    const zp_char* a = str();
    for( zp_size_t i = fromIndex; i < m_length; ++i )
    {
        if( a[ i ] == ch ) return i;
    }

    return npos;
}
zp_size_t zpString::indexOf( const zp_char* string, zp_size_t fromIndex ) const
{
    zp_size_t len = zp_strlen( string );
    if( len > m_length ) return npos;

    const zp_char* a = str();

    zp_char first = string[ 0 ];
    zp_size_t count = m_length - len;
    zp_size_t j;
    zp_size_t end;
    zp_size_t k;

    for( zp_size_t i = fromIndex; i <= count; ++i )
    {
        // find the first character
        if( a[ i ] != first )
        {
            for( ; i <= count && a[ i ] != first; ++i );
        }

        // look for the rest of the characters
        if( i <= count ) {
            j = i + 1;
            end = j + len - 1;
            for( k = 1; j < end && a[ j ] == string[ k ]; ++j, ++k );

            // if whole string found, return index
            if( j == end ) return i;
        }
    }

    return npos;
}
zp_size_t zpString::indexOf( const zpString& string, zp_size_t fromIndex ) const
{
    if( string.m_length > m_length ) return npos;

    const zp_char* a = str();
    const zp_char* b = string.str();

    zp_char first = b[ 0 ];
    zp_size_t count = m_length - string.m_length;
    zp_size_t j;
    zp_size_t end;
    zp_size_t k;

    for( zp_size_t i = fromIndex; i <= count; ++i )
    {
        // find the first character
        if( a[ i ] != first )
        {
            for( ; i <= count && a[ i ] != first; ++i );
        }

        // look for the rest of the characters
        if( i <= count ) {
            j = i + 1;
            end = j + string.m_length - 1;
            for( k = 1; j < end && a[ i ] == b[ k ]; ++j, ++k );

            // if whole string found, return index
            if( j == end ) return i;
        }
    }

    return npos;
}

zp_size_t zpString::indexOfIgnoreCase( zp_char ch, zp_size_t fromIndex ) const
{
    if( fromIndex > m_length ) return npos;

    const zp_char* a = str();
    zp_char l = zp_to_lower( ch );

    for( zp_size_t i = fromIndex; i < m_length; ++i )
    {
        if( zp_to_lower( a[ i ] ) == l ) return i;
    }

    return npos;
}
zp_size_t zpString::indexOfIgnoreCase( const zp_char* string, zp_size_t fromIndex ) const
{
    zp_size_t len = zp_strlen( string );
    if( len > m_length ) return npos;

    const zp_char* a = str();

    zp_char first = zp_to_lower( string[ 0 ] );
    zp_size_t count = m_length - len;
    zp_size_t j;
    zp_size_t end;
    zp_size_t k;

    for( zp_size_t i = fromIndex; i <= count; ++i )
    {
        // find the first character
        if( zp_to_lower( a[ i ] ) != first )
        {
            for( ; i <= count && zp_to_lower( a[ i ] ) != first; ++i );
        }

        // look for the rest of the characters
        if( i <= count )
        {
            j = i + 1;
            end = j + len - 1;
            for( k = 1; j < end && zp_to_lower( a[ i ] ) == zp_to_lower( string[ k ] ); ++j, ++k );

            // if whole string found, return index
            if( j == end ) return i;
        }
    }

    return npos;
}
zp_size_t zpString::indexOfIgnoreCase( const zpString& string, zp_size_t fromIndex ) const
{
    if( string.m_length > m_length ) return npos;

    const zp_char* a = str();
    const zp_char* b = string.str();

    zp_char first = zp_to_lower( b[ 0 ] );
    zp_size_t count = m_length - string.m_length;
    zp_size_t j;
    zp_size_t end;
    zp_size_t k;

    for( zp_size_t i = fromIndex; i <= count; ++i )
    {
        // find the first character
        if( zp_to_lower( a[ i ] ) != first )
        {
            for( ; i <= count && zp_to_lower( a[ i ] ) != first; ++i );
        }

        // look for the rest of the characters
        if( i <= count )
        {
            j = i + 1;
            end = j + string.m_length - 1;
            for( k = 1; j < end && zp_to_lower( a[ i ] ) == zp_to_lower( b[ k ] ); ++j, ++k );

            // if whole string found, return index
            if( j == end ) return i;
        }
    }

    return npos;
}

zp_size_t zpString::lastIndexOf( zp_char ch, zp_size_t fromIndex ) const
{
    if( fromIndex > m_length ) return npos;
    if( fromIndex <= 0 ) fromIndex = m_length + fromIndex;

    const zp_char* a = str();
    for( zp_size_t i = fromIndex - 1; i > 0; --i )
    {
        if( a[ i ] == ch ) return i;
    }

    return npos;
}
zp_size_t zpString::lastIndexOf( const zpString& string, zp_size_t fromIndex ) const
{
    if( fromIndex > m_length ) return npos;
    if( fromIndex < 0 ) fromIndex = m_length + fromIndex;
    ZP_ASSERT( false, "zpString::lastIndexOf not implimented" );

    return npos;
}

zp_size_t zpString::findFirstOf( const zpString& string, zp_size_t fromIndex ) const
{
    if( fromIndex > m_length ) return npos;
    if( fromIndex < 0 ) fromIndex = m_length + fromIndex;

    const zp_char* a = str();
    const zp_char* b = string.str();

    zp_char ch;
    for( zp_size_t i = fromIndex; i < m_length; ++i )
    {
        ch = a[ i ];
        for( zp_size_t j = 0; j < string.length(); ++j )
        {
            if( ch == b[ j ] ) return i;
        }
    }

    return npos;
}

zp_bool zpString::isEmpty() const
{
    return m_length == 0;
}
zp_size_t zpString::length() const
{
    return m_length;
}

zp_bool zpString::equals( const zpString& string ) const
{
    if( m_length != string.m_length ) return false;

    return compareTo( string ) == 0;
}
zp_bool zpString::equalsIgnoreCase( const zpString& string ) const
{
    if( m_length != string.m_length ) return false;

    return compareToIgnoreCase( string ) == 0;
}

zp_int zpString::compareTo( const zpString& string ) const
{
    zp_size_t count = ZP_MIN( m_length, string.m_length );
    zp_char sa, sb;

    const zp_char* a = str();
    const zp_char* b = string.str();

    for( zp_size_t i = 0; i < count; ++i )
    {
        sa = a[ i ];
        sb = b[ i ];

        if( sa != sb ) return sa - sb;
    }

    return m_length < string.m_length ? -1 : m_length > string.m_length ? 1 : 0;
}
zp_int zpString::compareToIgnoreCase( const zpString& string ) const
{
    zp_size_t count = ZP_MIN( m_length, string.m_length );
    zp_char sa, sb;
    
    const zp_char* a = str();
    const zp_char* b = string.str();

    for( zp_size_t i = 0; i < count; ++i )
    {
        sa = zp_to_lower( a[ i ] );
        sb = zp_to_lower( b[ i ] );

        if( sa != sb ) return sa - sb;
    }

    return m_length < string.m_length ? -1 : m_length > string.m_length ? 1 : 0;
}

zpString zpString::substring( zp_size_t startIndex ) const
{
    if( startIndex > m_length ) return zpString();

    zp_size_t count = m_length - startIndex;

    return zpString( str() + startIndex, count );
}
zpString zpString::substring( zp_size_t startIndex, zp_size_t endIndex ) const
{
    if( endIndex < 0 ) endIndex = m_length + endIndex;
    if( endIndex <= startIndex ) return zpString();

    zp_size_t count = endIndex - startIndex;

    return zpString( str() + startIndex, count );
}

void zpString::erase( zp_size_t startIndex, zp_size_t count )
{
    ZP_ASSERT( startIndex < (zp_int)m_length, "zpString: Index %d out of range %d", startIndex, m_length );
    if( startIndex < 0 ) startIndex = m_length + startIndex;
    if( count == 0 ) return;

    if( IS_STRING_PACKED( this ) )
    {
        zp_char* c = m_chars;
        zp_size_t start = startIndex;
        zp_size_t end = startIndex + count;

        for( zp_size_t j = 0; j < count && start < m_length; ++j, ++start, ++end )
        {
            c[ start ] = end > m_length ? '\0' : c[ end ];
        }

        m_length -= count;
    }
    else
    {
        if( ( m_length - count ) < ZP_STRING_MAX_SMALL_SIZE )
        {
            zp_char* c = m_string;
            zp_size_t start = startIndex;
            zp_size_t end = startIndex + count;

            for( zp_size_t j = 0; j < count && start < m_length; ++j, ++start, ++end )
            {
                c[ start ] = end > m_length ? '\0' : c[ end ];
            }

            m_length -= count;
            zp_strcpy( m_chars, ZP_STRING_MAX_SMALL_SIZE * sizeof( zp_char ), c );
            ZP_SAFE_DELETE_ARRAY( c );
        }
        else
        {
            zp_char* c = m_string;
            zp_size_t start = startIndex;
            zp_size_t end = startIndex + count;

            for( zp_size_t j = 0; j < count && start < m_length; ++j, ++start, ++end )
            {
                c[ start ] = end > m_length ? '\0' : c[ end ];
            }

            m_length -= count;
        }
    }
}
void zpString::append( zp_char ch )
{
    ensureCapacity( m_length + 1 );

    zp_char* c = ( m_length + 1 ) < ZP_STRING_MAX_SMALL_SIZE ? m_chars : m_string; //getCharsInternal();
    c[ m_length ] = ch;
    ++m_length;
    c[ m_length ] = '\0';
}
void zpString::append( const zp_char* str, zp_size_t length )
{
    if( length < 0 )
    {
        length = zp_strlen( str );
    }
    if( length > 0 )
    {
        ensureCapacity( m_length + length );

        zp_char* a = ( m_length + length ) < ZP_STRING_MAX_SMALL_SIZE ? m_chars : m_string; //getCharsInternal();
        const zp_char* b = str;
        for( zp_size_t i = 0; i < length; ++i, ++m_length )
        {
            a[ m_length ] = b[ i ];
        }

        a[ m_length ] = '\0';
    }
}
void zpString::append( const zpString& string )
{
    append( string.str(), string.length() );
}

void zpString::toLower( zpString& string )
{
    string.map( zp_to_lower );
}
void zpString::toUpper( zpString& string )
{
    string.map( zp_to_upper );
}
void zpString::join( zpString& string, const zpArrayList< zpString >& parts, const zp_char* delim )
{
    if( parts.isEmpty() )
    {
        string = "";
    }
    else
    {
        zpStringBuffer buff;
        buff.append( parts[ 0 ] );
        for( zp_size_t i = 1; i < parts.size(); ++i )
        {
            buff.append( delim );
            buff.append( parts[ i ] );
        }

        string = buff.str();
    }
}

zpString zpString::toCamelCase( zp_bool capitalFirstLetter ) const
{
    zpStringBuffer buff;
    buff.reserve( m_length );

    if( m_length > 0 )
    {
        const zp_char* s = str();
        zp_char c;
        zp_bool shouldCapital = capitalFirstLetter;
        
        for( zp_size_t i = 0; i < m_length; ++i )
        {
            c = s[ i ];
            if( c == '_' || zp_is_whitespace( c ) )
            {
                shouldCapital = true;
            }
            else
            {
                buff.append( shouldCapital ? zp_to_upper( c ) : zp_to_lower( c ) );
                shouldCapital = false;
            }
        }
    }

    return buff.toString();
}

zpString zpString::ltrim() const
{
    const zp_char* start = str();
    
    while( *start && zp_is_whitespace( *start++ ) );

    return zpString( start );
}
zpString zpString::rtrim() const
{
    const zp_char* start = str();
    const zp_char* end = start + m_length;

    while( *end && zp_is_whitespace( *end-- ) );
    end++;

    return zpString( start, m_length - ( end - start ) );
}
zpString zpString::trim() const
{
    const zp_char* start = str();
    const zp_char* end = start + m_length;

    while( *start && zp_is_whitespace( *start++ ) );
    while( *end && zp_is_whitespace( *end-- ) );

    return zpString( start, m_length - ( end - start ) );
}
void zpString::split( zp_char delim, zpArrayList< zpString >& parts ) const
{
    zp_size_t from = 0;
    zp_size_t to = 0;

    parts.clear();
    while( to != npos )
    {
        to = indexOf( delim, from );
        if( from < m_length && from != to )
        {
            if( to == npos )
            {
                parts.pushBackEmpty() = zp_move( substring( from ) );
            }
            else
            {
                parts.pushBackEmpty() = zp_move( substring( from, to ) );
            }
        }
        from = to + 1;
    }
}
void zpString::split( zp_char delim, zpArrayList< zpString >& parts, zp_int maxSplits ) const
{
    zp_size_t from = 0;
    zp_size_t to = 0;

    parts.clear();
    while( to != npos )
    {
        to = maxSplits == 0 ? npos : indexOf( delim, from );
        if( from < m_length && from != to )
        {
            if( to == npos )
            {
                parts.pushBackEmpty() = zp_move( substring( from ) );
            }
            else
            {
                parts.pushBackEmpty() = zp_move( substring( from, to ) );
                --maxSplits;
            }
        }
        from = to + 1;
    }
}

void zpString::ensureCapacity( zp_size_t size )
{
    if( IS_STRING_PACKED( this ) )
    {
        if( size >= ZP_STRING_MAX_SMALL_SIZE )
        {
            zp_size_t cap = ZP_STRING_MAX_SMALL_SIZE;
            while( cap <= size ) cap *= 2;

            zp_char* buff = new zp_char[ cap ];
            zp_strcpy( buff, cap * sizeof( zp_char ), m_chars );
            zp_zero_memory_array( m_chars );

            m_string = buff;
            m_capacity = cap;
        }
    }
    else
    {
        if( size > m_capacity )
        {
            while( m_capacity <= size ) m_capacity *= 2;

            zp_char* buff = new zp_char[ m_capacity ];
            zp_strcpy( buff, m_capacity * sizeof( zp_char ), m_string );
            ZP_SAFE_DELETE_ARRAY( m_string );

            m_string = buff;
        }
    }
}

void zpString::reserve( zp_size_t size )
{
    ensureCapacity( size );
}
void zpString::clear()
{
    (*this) = "";
}

zp_bool operator==( const zpString& string1, const zpString& string2 )
{
    return string1.equals( string2 );
}
zp_bool operator==( const zpString& string1, const zp_char* string2 )
{
    return zp_strcmp( string1.str(), string2 ) == 0;
}
zp_bool operator==( const zp_char* string1, const zpString& string2 )
{
    return zp_strcmp( string1, string2.str() ) == 0;
}

zp_bool operator!=( const zpString& string1, const zpString& string2 )
{
    return !( string1 == string2 );
}
zp_bool operator!=( const zpString& string1, const zp_char* string2 )
{
    return !( string1 == string2 );
}
zp_bool operator!=( const zp_char* string1, const zpString& string2 )
{
    return !( string1 == string2 );
}
