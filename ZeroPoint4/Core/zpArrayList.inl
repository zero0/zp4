
template<typename T>
zpArrayList<T>::zpArrayList()
    : m_array( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_isFixed( false )
{}
template<typename T>
zpArrayList<T>::zpArrayList( const zpArrayList& arr )
    : m_array( ZP_NULL )
    , m_size( 0 )
    , m_capacity ( 0 )
    , m_isFixed( false )
{
    ensureCapacity( arr.m_size );
    for( zp_size_t i = 0; i < arr.m_size; ++i )
    {
        pushBack( arr[ i ] );
    }
}
template<typename T>
zpArrayList<T>::zpArrayList( T* fixedArray, zp_size_t count )
    : m_array( fixedArray )
    , m_size( 0 )
    , m_capacity( count )
    , m_isFixed( true )
{}
template<typename T>
zpArrayList<T>::zpArrayList( zpArrayList&& arr )
    : m_array( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_isFixed( false )
{
    if( arr.m_isFixed )
    {
        ensureCapacity( arr.m_size );
        for( zp_size_t i = 0; i < arr.m_size; ++i )
        {
            pushBack( arr[ i ] );
        }
    }
    else
    {
        m_array = arr.m_array;
        m_size = arr.m_size;
        m_capacity = arr.m_capacity;

        arr.m_array = ZP_NULL;
        arr.m_size = 0;
    }
}
template<typename T>
zpArrayList<T>::~zpArrayList()
{
    destroy();
}

template<typename T>
void zpArrayList<T>::operator=( const zpArrayList& arr )
{
    clear();
    ensureCapacity( arr.m_size );
    for( zp_size_t i = 0; i < arr.m_size; ++i )
    {
        pushBack( arr[ i ] );
    }
}
template<typename T>
void zpArrayList<T>::operator=( zpArrayList&& arr )
{
    clear();
    if( arr.m_isFixed )
    {
        ensureCapacity( arr.m_size );
        for( zp_size_t i = 0; i < arr.m_size; ++i )
        {
            pushBack( arr[ i ] );
        }
    }
    else
    {
        m_array = arr.m_array;
        m_size = arr.m_size;
        m_capacity = arr.m_capacity;

        arr.m_array = ZP_NULL;
        arr.m_size = 0;
    }
}

template<typename T>
T& zpArrayList<T>::operator[]( zp_size_t index )
{
    ZP_ASSERT( index < m_size, "zpArrayList: Index %d out of bounds %d", index, m_size );
    return m_array[ index ];
}
template<typename T>
const T& zpArrayList<T>::operator[]( zp_size_t index ) const
{
    ZP_ASSERT( index < m_size, "zpArrayList: Index %d out of bounds %d", index, m_size );
    return m_array[ index ];
}

template<typename T>
const T& zpArrayList<T>::at( zp_size_t index ) const
{
    ZP_ASSERT( index < m_size, "zpArrayList: Index %d out of bounds %d", index, m_size );
    return m_array[ index ];
}

template<typename T>
zp_size_t zpArrayList<T>::size() const
{
    return m_size;
}
template<typename T>
zp_bool zpArrayList<T>::isEmpty() const
{
    return m_size == 0;
}
template<typename T>
zp_bool zpArrayList<T>::isFixed() const
{
    return m_isFixed;
}

template<typename T>
void zpArrayList<T>::pushBack( const T& val )
{
    ensureCapacity( m_size + 1 );
    m_array[ m_size ] = val;
    ++m_size;
}
template<typename T>
T& zpArrayList<T>::pushBackEmpty()
{
    ensureCapacity( m_size + 1 );
    m_array[ m_size ] = (T&&)T();
    ++m_size;
    return back();
}

template<typename T>
void zpArrayList<T>::pushFront( const T& val )
{
    ensureCapacity( m_size + 1 );

    for( zp_size_t i = m_size + 1; i >= 1; --i )
    {
        m_array[ i ] = zp_move( m_array[ i - 1 ] );
    }

    m_array[ 0 ] = val;

    ++m_size;
}
template<typename T>
T& zpArrayList<T>::pushFrontEmpty()
{
    pushFront( T() );
    return front();
}

template<typename T>
void zpArrayList<T>::popBack()
{
    ZP_ASSERT( m_size > 0, "zpArrayList: popBack empty array" );
    
    --m_size;
    ( m_array + m_size )->~T();
}
template<typename T>
void zpArrayList<T>::popFront()
{
    ZP_ASSERT( m_size > 0, "zpArrayList: popFront empty array" );

    m_array->~T();
    for( zp_size_t i = 1; i < m_size; ++i )
    {
        m_array[ i - 1 ] = zp_move( m_array[ i ] );
    }
    --m_size;
}

template<typename T>
void zpArrayList<T>::erase( zp_size_t index )
{
    ZP_ASSERT( index < m_size, "zpArrayList: Index out of bound of Array. Index: %d Size: %d", index, m_size );

    ( m_array + index )->~T();
    m_array[ index ] = zp_move( m_array[ m_size - 1 ] );
    --m_size;
}
template<typename T>
zp_size_t zpArrayList<T>::eraseAll( const T& val )
{
    zp_size_t count = 0;
    for( zp_size_t i = 0; i < m_size; ++i )
    {
        if( m_array[ i ] == val )
        {
            erase( i );
            ++count;
            --i;
        }
    }
    return count;
}

template<typename T>
void zpArrayList<T>::clear()
{
    for( zp_size_t i = 0; i < m_size; ++i )
    {
        ( m_array + i )->~T();
    }
    m_size = 0;
}

template<typename T>
void zpArrayList<T>::reset()
{
    m_size = 0;
}

template<typename T>
void zpArrayList<T>::reserve( zp_size_t size )
{
    ensureCapacity( size );
}

template<typename T>
void zpArrayList<T>::resize( zp_size_t size )
{
    ensureCapacity( size );
    m_size = size;
}

template<typename T>
void zpArrayList<T>::destroy()
{
    clear();

    if( !m_isFixed )
    {
        ZP_SAFE_DELETE_ARRAY( m_array );
        m_capacity = 0;
    }
}

template<typename T>
zp_size_t zpArrayList<T>::indexOf( const T& val ) const
{
    for( zp_size_t i = 0; i < m_size; ++i )
    {
        if( m_array[ i ] == val ) return i;
    }
    return npos;
}
template<typename T>
zp_size_t zpArrayList<T>::lastIndexOf( const T& val ) const
{
    for( zp_size_t i = (zp_int)m_size; i-- > 0; )
    {
        if( m_array[ i ] == val ) return i;
    }
    return npos;
}

template<typename T>
void zpArrayList<T>::ensureCapacity( zp_size_t size )
{
    // if the array is already large enough, return
    if( size <= m_capacity ) return;

    ZP_ASSERT( !m_isFixed, "zpArrayList: Unable to resize fixed array" );

    // calculate the new capacity
    // - capacity of 0 means it's coming from the constructor, set it to size
    // - otherwise, grow capacity
    if( m_capacity == 0 )
    {
        m_capacity = size;
    }
    else
    {
        while( m_capacity < size ) m_capacity *= 2;
    }
    
    T* newArray = new T[ m_capacity ];
    if( m_array )
    {
        for( zp_size_t i = 0; i < m_size; ++i )
        {
            newArray[ i ] = zp_move( m_array[ i ] );
        }

        ZP_SAFE_DELETE_ARRAY( m_array );
    }
    m_array = newArray;
}

template<typename T>
T& zpArrayList<T>::front()
{
    ZP_ASSERT( m_size > 0, "zpArrayList: Accessing empty array" );
    return m_array[ 0 ];
}
template<typename T>
T& zpArrayList<T>::back()
{
    ZP_ASSERT( m_size > 0, "zpArrayList: Accessing empty array" );
    return m_array[ m_size - 1 ];
}

template<typename T>
const T& zpArrayList<T>::front() const
{
    ZP_ASSERT( m_size > 0, "zpArrayList: Accessing empty array" );
    return m_array[ 0 ];
}
template<typename T>
const T& zpArrayList<T>::back() const
{
    ZP_ASSERT( m_size > 0, "zpArrayList: Accessing empty array" );
    return m_array[ m_size - 1 ];
}

template<typename T>
T* zpArrayList<T>::begin()
{
    return m_array;
}
template<typename T>
T* zpArrayList<T>::end()
{
    return m_array + m_size;
}

template<typename T>
const T* zpArrayList<T>::begin() const
{
    return m_array;
}
template<typename T>
const T* zpArrayList<T>::end() const
{
    return m_array + m_size;
}

template<typename T> template<typename Func>
zp_bool zpArrayList<T>::findIf( Func func, const T** found ) const
{
    for( zp_size_t i = 0; i < m_size; ++i )
    {
        if( func( m_array[ i ] ) )
        {
            *found = m_array + i;
            return true;
        }
    }
    return false;
}
template<typename T> template<typename Func>
zp_bool zpArrayList<T>::findIf( Func func, T** found )
{
    for( zp_size_t i = 0; i < m_size; ++i )
    {
        if( func( m_array[ i ] ) )
        {
            *found = m_array + i;
            return true;
        }
    }
    return false;
}

template<typename T> template<typename Func>
zp_bool zpArrayList<T>::findIndexIf( Func func, zp_size_t& index ) const
{
    for( zp_size_t i = 0; i < m_size; ++i )
    {
        if( func( m_array[ i ] ) )
        {
            index = i;
            return true;
        }
    }
    return false;
}

template<typename T> template<typename Func>
void zpArrayList<T>::foreach( Func func ) const
{
    for( zp_uint i = 0; i < m_size; ++i )
    {
        func( m_array[ i ] );
    }
}
template<typename T> template<typename Func>
void zpArrayList<T>::foreachIf( Func func ) const
{
    for( zp_uint i = 0; i < m_size; ++i )
    {
        if( func( m_array[ i ] ) ) break;
    }
}

template<typename T> template<typename Func>
void zpArrayList<T>::foreachIndexed( Func func ) const
{
    for( zp_uint i = 0; i < m_size; ++i )
    {
        func( i, m_array[ i ] );
    }
}
template<typename T> template<typename Func>
void zpArrayList<T>::foreachIndexedIf( Func func ) const
{
    for( zp_uint i = 0; i < m_size; ++i )
    {
        if( func( i, m_array[ i ] ) ) break;
    }
}

template<typename T> template<typename Func>
void zpArrayList<T>::eraseIf( Func func )
{
    for( zp_uint i = 0; i < m_size; ++i )
    {
        if( func( m_array[ i ] ) )
        {
            erase( i );
        }
    }
}

template<typename T> template<typename Func>
void zpArrayList<T>::map( Func func )
{
    for( zp_uint i = 0; i < m_size; ++i )
    {
        m_array[ i ] = func( m_array[ i ] );
    }
}

template<typename T> template<typename Func>
void zpArrayList<T>::sort( Func func )
{
    if( m_size > 1 )
    {
        zp_qsort( m_array, m_size, func );
    }
}


template<typename T, zp_size_t Size>
zpFixedArrayList<T, Size>::zpFixedArrayList()
    : zpArrayList<T>( m_fixedArray, Size )
{}

