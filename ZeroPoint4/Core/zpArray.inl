
template<typename T, zp_uint Size>
const zp_uint zpArray<T, Size>::npos = (zp_uint)-1;

template<typename T, zp_uint Size>
zpArray<T, Size>::zpArray() {
	zp_memset( m_array, 0, Size * sizeof( T ) );
}
template<typename T, zp_uint Size>
zpArray<T, Size>::zpArray( T (&arr)[ Size ] ) {
	zp_memcpy( m_array, Size * sizeof( T ), arr, size );
}
template<typename T, zp_uint Size>
zpArray<T, Size>::zpArray( T* data, zp_uint size ) {
	zp_memcpy( m_array, Size * sizeof( T ), data, size );
}
template<typename T, zp_uint Size>
zpArray<T, Size>::zpArray( const zpArray& arr ) {
	zp_memcpy( m_array, Size * sizeof( T ), arr.m_array, Size * sizeof( T ) );
}
template<typename T, zp_uint Size>
zpArray<T, Size>::zpArray( zpArray&& arr ) {
	zp_memcpy( m_array, Size * sizeof( T ), arr.m_array, Size * sizeof( T ) );
	arr.clear();
}
template<typename T, zp_uint Size>
zpArray<T, Size>::~zpArray() {
	clear();
}

template<typename T, zp_uint Size>
void zpArray<T, Size>::operator=( T (&arr)[ Size ] ) {
	clear();
	zp_memcpy( m_array, Size * sizeof( T ), arr, Size * sizeof( T ) );
}
template<typename T, zp_uint Size>
void zpArray<T, Size>::operator=( const zpArray& arr ) {
	clear();
	zp_memcpy( m_array, Size * sizeof( T ), arr.m_array, Size * sizeof( T ) );
}
template<typename T, zp_uint Size>
void zpArray<T, Size>::operator=( zpArray&& arr ) {
	clear();
}

template<typename T, zp_uint Size>
T& zpArray<T, Size>::operator[]( zp_uint index ) {
	return m_array[ index ];
}
template<typename T, zp_uint Size>
const T& zpArray<T, Size>::operator[]( zp_uint index ) const {
	return m_array[ index ];
}

template<typename T, zp_uint Size>
T* zpArray<T, Size>::data() {
	return m_array;
}
template<typename T, zp_uint Size>
const T* zpArray<T, Size>::data() const {
	return m_array;
}

template<typename T, zp_uint Size>
const T& zpArray<T, Size>::at( zp_uint index ) const {
	return m_array[ index ];
}

template<typename T, zp_uint Size>
zp_uint zpArray<T, Size>::size() const {
	return Size;
}

template<typename T, zp_uint Size>
void zpArray<T, Size>::remove( zp_uint index, T* outVal = ZP_NULL ) {
	if( outVal ) *outVal = (T&&)m_array[ index ];
	zp_memset( &m_array[ index ], 0, sizeof( T ) );
}
template<typename T, zp_uint Size>
void zpArray<T, Size>::erase( zp_uint index ) {
	if( &m_array[ index ] ) {
		(&m_array[ index ])->~T();
		//m_array[ index ] = T();
		zp_memset( &m_array[ index ], 0, sizeof( T ) );
	}
}

template<typename T, zp_uint Size>
void zpArray<T, Size>::clear() {
	if( !m_array ) return;
	for( zp_uint i = Size; i --> 0; ) {
		erase( i );
	}
}

template<typename T, zp_uint Size>
zp_uint zpArray<T, Size>::indexOf( const T& val ) const {
	for( zp_uint i = 0; i < Size; ++i ) {
		if( m_array[ i ] == val ) return i;
	}
	return npos;
}
template<typename T, zp_uint Size>
zp_uint zpArray<T, Size>::lastIndexOf( const T& val ) const {
	for( zp_uint i = Size; i --> 0; ) {
		if( m_array[ i ] == val ) return i;
	}
	return npos;
}

template<typename T, zp_uint Size>
void zpArray<T, Size>::fill( const T* vals, zp_uint size, zp_uint startIndex ) {
	if( startIndex > Size ) return;
	zp_uint count = Size - startIndex;
	count = ZP_MIN( count, size );

	for( zp_uint i = startIndex, j = 0; i < count; ++i, ++j ) {
		m_array[ i ] = vals[ j ];
	}
}

template<typename T, zp_uint Size> template<typename Func>
void zpArray<T, Size>::foreach( Func func ) const {
	for( zp_uint i = 0; i < Size; ++i ) {
		func( m_array[ i ] );
	}
}
template<typename T, zp_uint Size> template<typename Func>
void zpArray<T, Size>::foreachIf( Func func ) const {
	for( zp_uint i = 0; i < Size; ++i ) {
		if( func( m_array[ i ] ) ) break;
	}
}
template<typename T, zp_uint Size> template<typename Func>
void zpArray<T, Size>::foreachInRange( zp_uint start, zp_int count, Func func ) const {
	zp_uint end = ZP_MIN( start + count, Size );
	for( zp_uint i = start; i < end; ++i ) {
		func( m_array[ i ] );
	}
}

template<typename T, zp_uint Size> template<typename Func>
void zpArray<T, Size>::foreachIndexed( Func func ) const {
	for( zp_uint i = 0; i < Size; ++i ) {
		func( i, m_array[ i ] );
	}
}
template<typename T, zp_uint Size> template<typename Func>
void zpArray<T, Size>::foreachIndexedIf( Func func ) const {
	for( zp_uint i = 0; i < Size; ++i ) {
		if( func( i, m_array[ i ] ) ) break;
	}
}

template<typename T, zp_uint Size> template<typename Func>
void zpArray<T, Size>::map( Func func ) const {
	for( zp_uint i = 0; i < Size; ++i ) {
		m_array[ i ] = func( m_array[ i ] );
	}
}