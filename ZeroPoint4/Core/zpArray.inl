#include <string.h>

template<typename T>
const zp_uint zpArray<T>::npos = (zp_uint)-1;

template<typename T>
zpArray<T>::zpArray() : m_array( ZP_NULL ), m_size( 0 ) {}
template<typename T>
zpArray<T>::zpArray( zp_uint size ) : m_array( size == 0 ? ZP_NULL : (T*)calloc( size, sizeof( T ) ) ), m_size( size ) {}
template<typename T>
zpArray<T>::zpArray( const zpArray& arr ) : m_array( (T*)calloc( size, sizeof( T ) ) ), m_size( arr.m_size ) {
	memcpy_s( m_array, m_size * sizeof( T ), arr.m_array, m_size * sizeof( T ) );
}
template<typename T>
zpArray<T>::zpArray( zpArray&& arr ) : m_array( arr.m_array ), m_size( arr.m_size ) {
	arr.m_array = ZP_NULL;
}
template<typename T>
zpArray<T>::~zpArray() {
	clear();
	ZP_SAFE_FREE( m_array );
}

template<typename T>
void zpArray<T>::operator=( const zpArray& arr ) {
	clear();
	ZP_SAFE_FREE( m_array );
	m_size = arr.m_size;
	m_array = (T*)calloc( m_size, sizeof( T ) );
	memcpy_s( m_array, m_size * sizeof( T ), arr.m_array, m_size * sizeof( T ) );
}
template<typename T>
void zpArray<T>::operator=( zpArray&& arr ) {
	clear();
	ZP_SAFE_FREE( m_array );
	m_size = arr.m_size;
	m_array = arr.m_array;

	arr.m_array = ZP_NULL;
}

template<typename T>
T& zpArray<T>::operator[]( zp_uint index ) {
	return m_array[ index ];
}
template<typename T>
const T& zpArray<T>::operator[]( zp_uint index ) const {
	return m_array[ index ];
}

template<typename T>
zpArray<T>::operator T*() {
	return m_array;
}
template<typename T>
zpArray<T>::operator const T*() const {
	return m_array;
}

template<typename T>
const T& zpArray<T>::at( zp_uint index ) const {
	return m_array[ index ];
}

template<typename T>
zp_uint zpArray<T>::size() const {
	return m_size;
}

template<typename T>
void zpArray<T>::remove( zp_uint index, T* outVal = ZP_NULL ) {
	if( outVal ) *outVal = (T&&)m_array[ index ];
	memset( &m_array[ index ], 0, sizeof( T ) );
}
template<typename T>
void zpArray<T>::erase( zp_uint index ) {
	if( &m_array[ index ] ) {
		(&m_array[ index ])->~T();
		//m_array[ index ] = T();
		memset( &m_array[ index ], 0, sizeof( T ) );
	}
}

template<typename T>
void zpArray<T>::clear() {
	if( !m_array ) return;
	for( zp_uint i = m_size; i --> 0; ) {
		erase( i );
	}
}

template<typename T>
zp_uint zpArray<T>::indexOf( const T& val ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( m_array[ i ] == val ) return i;
	}
	return npos;
}
template<typename T>
zp_uint zpArray<T>::lastIndexOf( const T& val ) const {
	for( zp_uint i = m_size; i --> 0; ) {
		if( m_array[ i ] == val ) return i;
	}
	return npos;
}

template<typename T> template<typename Func>
void zpArray<T>::foreach( Func func ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		func( m_array[ i ] );
	}
}
template<typename T> template<typename Func>
void zpArray<T>::foreachIf( Func func ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( func( m_array[ i ] ) ) break;
	}
}

template<typename T> template<typename Func>
void zpArray<T>::foreachIndexed( Func func ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		func( i, m_array[ i ] );
	}
}
template<typename T> template<typename Func>
void zpArray<T>::foreachIndexedIf( Func func ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		if( func( i, m_array[ i ] ) ) break;
	}
}

template<typename T> template<typename Func>
void zpArray<T>::map( Func func ) const {
	for( zp_uint i = 0; i < m_size; ++i ) {
		m_array[ i ] = func( m_array[ i ] );
	}
}