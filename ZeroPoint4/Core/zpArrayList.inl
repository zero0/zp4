
template<typename T>
const zp_uint zpArrayList<T>::npos = (zp_uint)-1;

template<typename T>
zpArrayList<T>::zpArrayList() : m_array( ZP_NULL ), m_size( 0 ), m_capacity( 0 ) {
	ensureCapacity( ZP_ARRAY_DEFAULT_SIZE );
	ZP_ON_DEBUG( zp_memset( m_array, 0, m_capacity * sizeof( T ) ) );
}
template<typename T>
zpArrayList<T>::zpArrayList( zp_uint size ) : m_array( ZP_NULL ), m_size( 0 ), m_capacity( 0 ) {
	ensureCapacity( size );
	ZP_ON_DEBUG( zp_memset( m_array, 0, m_capacity * sizeof( T ) ) );
}
template<typename T>
zpArrayList<T>::zpArrayList( T* arr, zp_uint size, zp_uint capacity ) : m_array( ZP_NULL ), m_size( 0 ), m_capacity ( 0 ) {
	ensureCapacity( ZP_MAX( size, capacity ) );
	pushBack( arr, size );
	ZP_ON_DEBUG( zp_memset( m_array + m_size, 0, ( m_capacity - m_size ) * sizeof( T ) ) );
}
template<typename T>
zpArrayList<T>::zpArrayList( const zpArrayList& arr ) : m_array( ZP_NULL ), m_size( 0 ), m_capacity ( 0 ) {
	ensureCapacity( arr.m_capacity );
	pushBack( arr.m_array, arr.m_size );
	ZP_ON_DEBUG( zp_memset( m_array + m_size, 0, ( m_capacity - m_size ) * sizeof( T ) ) );
}
template<typename T>
zpArrayList<T>::zpArrayList( zpArrayList&& arr ) : m_array( arr.m_array ), m_size( arr.m_size ), m_capacity( arr.m_capacity ) {
	arr.m_array = ZP_NULL;
}
template<typename T>
zpArrayList<T>::~zpArrayList() {
	ZP_SAFE_FREE( m_array );
}

template<typename T>
void zpArrayList<T>::operator=( const zpArrayList& arr ) {
	ensureCapacity( arr.m_capacity );
	pushBack( arr.m_array, arr.m_size );
}
template<typename T>
void zpArrayList<T>::operator=( zpArrayList&& arr ) {
	m_array = arr.m_array;
	m_size = arr.m_size;
	m_capacity = arr.m_capacity;

	arr.m_array = ZP_NULL;
}

template<typename T>
T& zpArrayList<T>::operator[]( zp_uint index ) {
	return m_array[ index ];
}
template<typename T>
const T& zpArrayList<T>::operator[]( zp_uint index ) const {
	return m_array[ index ];
}

template<typename T>
const T& zpArrayList<T>::at( zp_uint index ) const {
	return m_array[ indext ];
}

template<typename T>
zp_uint zpArrayList<T>::size() const {
	return m_size;
}
template<typename T>
zp_uint zpArrayList<T>::capacity() const {
	return m_capacity;
}
template<typename T>
zp_bool zpArrayList<T>::isEmpty() const {
	return m_size == 0;
}
template<typename T>
zp_bool zpArrayList<T>::isFull() const {
	return m_size == m_capacity;
}

template<typename T>
void zpArrayList<T>::pushBack( const T& val ) {
	ensureCapacity( m_size + 1 );
	m_array[ m_size ] = val;
	++m_size;
}
template<typename T>
void zpArrayList<T>::pushBack( const T* arr, zp_uint count ) {
	zp_uint newSize = m_size + count;
	ensureCapacity( newSize );

	zp_memcpy( m_array + m_size, ( m_capacity - m_size ) * sizeof( T ), arr, count * sizeof( T ) );

	m_size = newSize;
}
template<typename T> template<zp_uint Count>
void zpArrayList<T>::pushBack( const T (&arr)[Count] ) {
	pushBack( arr, Count );
}

template<typename T>
void zpArrayList<T>::pushFront( const T& val ) {
	ensureCapacity( m_size + 1 );

	zp_memmove( m_array + 1, ( m_capacity - 1 ) * sizeof( T ), m_array, ( m_capacity - 1 ) * sizeof( T ) );
	m_array[ 0 ] = val;

	++m_size;
}
template<typename T>
void zpArrayList<T>::pushFront( const T* arr, zp_uint count ) {
	zp_uint newSize = m_size + count;
	ensureCapacity( newSize );

	zp_memmove( m_array + count, ( m_capacity - count ) * sizeof( T ), m_array, ( m_capacity - count ) * sizeof( T ) );
	zp_memcpy( m_array, m_capacity * sizeof( T ), arr, count * sizeof( T ) );

	m_size = newSize;
}
template<typename T> template<zp_uint Count>
void zpArrayList<T>::pushFront( const T (&arr)[Count] ) {
	pushFront( arr, Count );
}

template<typename T>
void zpArrayList<T>::popBack( zp_uint numToPop = 1 ) {
	zp_uint newSize = m_size - numToPop;
	for( zp_uint s = m_size; s --> newSize; ) {
		(&m_array[ s ])->~T();
	}
	m_size = newSize;
}
template<typename T>
void zpArrayList<T>::popFront( zp_uint numToPop = 1 ) {
	for( zp_uint s = 0; s < numToPop; ++s ) {
		(&m_array[ s ])->~T();
	}
	zp_memmove( m_array, m_capacity * sizeof( T ), m_array + numToPop, ( m_capacity - numToPop ) * sizeof( T ) );
	m_size = m_size - numToPop;
}

template<typename T>
void zpArrayList<T>::insert( zp_uint index, const T& val ) {
	ZP_ASSERT( index < m_size, "Index out of bound of Array. Index: %d Size: %d", index, m_size );
	ensureCapacity( m_size + 1 );

	zp_memmove( m_array + index + 1, ( m_capacity - 1 ) * sizeof( T ), m_array + index, ( m_capacity - 1 ) * sizeof( T ) );
	m_array[ index ] = val;
	++m_size;
}

template<typename T>
void zpArrayList<T>::remove( zp_uint index, T* outVal ) {
	ZP_ASSERT( index < m_size, "zpArrayList: Index out of bound of Array. Index: %d Size: %d", index, m_size );
	
	if( outVal ) *outVal = m_array[ index ];
	zp_memmove( m_array + index, ( m_capacity - 1 ) * sizeof( T ), m_array + index + 1, ( m_capacity - 1 ) * sizeof( T ) );
	--m_size;
}
template<typename T>
zp_bool zpArrayList<T>::removeFirst( const T& val, T* outVal ) {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( m_array[ i ] == val ) {
			remove( i, outVal );
			return true;
		}
	}
	return false;
}
template<typename T>
zp_bool zpArrayList<T>::removeLast( const T& val, T* outVal ) {
	for( zp_uint i = m_size; i --> 0; ) {
		if( m_array[ i ] == val ) {
			remove( i, outVal );
			return true;
		}
	}
	return false;
}
template<typename T>
zp_uint zpArrayList<T>::removeAll( const T& val ) {
	zp_uint removed = 0;
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( m_array[ i ] == val ) {
			remove( i, ZP_NULL );
			removed++;
			i--;
		}
	}
	return removed;
}

template<typename T>
void zpArrayList<T>::erase( zp_uint index ) {
	ZP_ASSERT( index < m_size, "zpArrayList: Index out of bound of Array. Index: %d Size: %d", index, m_size );

	(&m_array[ index ])->~T();
	zp_memmove( m_array + index, ( m_capacity - 1 ) * sizeof( T ), m_array + index + 1, ( m_capacity - 1 ) * sizeof( T ) );
	--m_size;
}
template<typename T>
zp_bool zpArrayList<T>::eraseFirst( const T& val ) {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( m_array[ i ] == val ) {
			erase( i );
			return true;
		}
	}
	return false;
}
template<typename T>
zp_bool zpArrayList<T>::eraseLast( const T& val ) {
	for( zp_uint i = m_size; i --> 0; ) {
		if( m_array[ i ] == val ) {
			erase( i );
			return true;
		}
	}
	return false;
}
template<typename T>
zp_uint zpArrayList<T>::eraseAll( const T& val ) {
	zp_uint count = 0;
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( m_array[ i ] == val ) {
			erase( i );
			++count;
			--i;
		}
	}
	return count;
}

template<typename T>
void zpArrayList<T>::clear() {
	for( zp_uint i = 0; i < m_size; ++i ) {
		(&m_array[ i ])->~T();
	}
	ZP_ON_DEBUG( zp_memset( m_array, 0, m_size * sizeof( T ) ) );
	m_size = 0;
}

template<typename T>
zp_uint zpArrayList<T>::indexOf( const T& val ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( m_array[ i ] == val ) return i;
	}
	return npos;
}
template<typename T>
zp_uint zpArrayList<T>::lastIndexOf( const T& val ) const {
	for( zp_uint i = m_size; i --> 0; ) {
		if( m_array[ i ] == val ) return i;
	}
	return npos;
}

template<typename T>
void zpArrayList<T>::ensureCapacity( zp_uint size ) {
	// if the array is already large enough, return
	if( size <= m_capacity ) return;
	
	// calculate the new capacity
	// - capacity of 0 means it's coming from the constructor, set it to size
	// - otherwise, grow capacity
	if( m_capacity == 0 ) {
		m_capacity = size;
	} else {
		while( m_capacity < size ) m_capacity *= 2;
	}
	//m_array = (T*)zp_realloc( m_array, m_capacity * sizeof( T ) );
	
	T* newArray = (T*)zp_calloc( m_capacity, sizeof( T ) );
	if( m_array ) {
		zp_memcpy( newArray, m_capacity * sizeof( T ), m_array, m_size * sizeof( T ) );
		zp_free( m_array );
	}
	m_array = newArray;
	
}
template<typename T>
void zpArrayList<T>::shrinkToFit( zp_uint padding ) {/*
	zp_uint newCapacity = m_size + padding;
	if( newCapacity > 0 && newCapacity < m_capacity ) {
		zp_uint sizeInBytes = newCapacity * sizeof( T );
		
		T* newArray = (T*)malloc( sizeInBytes );
		zp_memcpy( newArray, sizeInBytes, m_array, sizeInBytes );
		free( m_array );
		
		m_array = newArray;
		m_capacity = newCapacity;
	}
*/}

template<typename T>
T& zpArrayList<T>::front() {
	return m_array[ 0 ];
}
template<typename T>
T& zpArrayList<T>::back() {
	return m_array[ m_size == 0 ? 0 : m_size - 1 ];
}

template<typename T>
const T& zpArrayList<T>::front() const {
	return m_array[ 0 ];
}
template<typename T>
const T& zpArrayList<T>::back() const {
	return m_array[ m_size == 0 ? 0 : m_size - 1 ];
}

template<typename T>
T* zpArrayList<T>::begin() {
	return m_array;
}
template<typename T>
T* zpArrayList<T>::end() {
	return &m_array[ m_size ];
}

template<typename T>
const T* zpArrayList<T>::begin() const {
	return m_array;
}
template<typename T>
const T* zpArrayList<T>::end() const {
	return &m_array[ m_size ];
}

template<typename T> template<typename Func>
zp_bool zpArrayList<T>::findIf( Func func, T** found ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( func( m_array[ i ] ) ) {
			*found = &m_array[ i ];
			return true;
		}
	}
	return false;
}
template<typename T> template<typename Func>
zp_bool zpArrayList<T>::findIf( Func func, T** found ) {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( func( m_array[ i ] ) ) {
			*found = &m_array[ i ];
			return true;
		}
	}
	return false;
}

template<typename T> template<typename Func>
void zpArrayList<T>::foreach( Func func ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		func( m_array[ i ] );
	}
}
template<typename T> template<typename Func>
void zpArrayList<T>::foreachIf( Func func ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( func( m_array[ i ] ) ) break;
	}
}

template<typename T> template<typename Func>
void zpArrayList<T>::foreachIndexed( Func func ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		func( i, m_array[ i ] );
	}
}
template<typename T> template<typename Func>
void zpArrayList<T>::foreachIndexedIf( Func func ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( func( i, m_array[ i ] ) ) break;
	}
}

template<typename T> template<typename Func>
void zpArrayList<T>::removeFirstIf( Func func, T* outVal ) {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( func( m_array[ i ] ) ) {
			remove( i, outVal );
			return;
		}
	}
}
template<typename T> template<typename Func>
void zpArrayList<T>::removeLastIf( Func func, T* outVal ) {
	for( zp_uint i = m_size; i --> 0; ) {
		if( func( m_array[ i ] ) ) {
			remove( i, outVal );
			return;
		}
	}
}
template<typename T> template<typename Func>
void zpArrayList<T>::removeAllIf( Func func, T* outVal ) {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( func( m_array[ i ] ) ) {
			remove( i, outVal );
		}
	}
}

template<typename T> template<typename Func>
void zpArrayList<T>::map( Func func ) {
	for( zp_uint i = 0; i < m_size; ++i ) {
		m_array[ i ] = func( m_array[ i ] );
	}
}
