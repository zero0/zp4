
#include <memory.h>
#include <string.h>

template<typename T>
zpArray<T>::zpArray() : m_array( ZP_NULL ), m_size( 0 ), m_capacity( 0 ) {
	ensureCapacity( ZP_ARRAY_DEFAULT_SIZE );
}
template<typename T>
zpArray<T>::zpArray( zp_uint size ) : m_array( ZP_NULL ), m_size( 0 ), m_capacity( 0 ) {
	ensureCapacity( size );
}
template<typename T>
zpArray<T>::zpArray( T* arr, zp_uint size, zp_uint capacity ) : m_array( ZP_NULL ), m_size( 0 ), m_capacity ( 0 ) {
	ensureCapacity( ZP_MAX( size, capacity ) );
	pushBack( arr, size );
}
template<typename T>
zpArray<T>::zpArray( const zpArray& arr ) : m_array( ZP_NULL ), m_size( 0 ), m_capacity ( 0 ) {
	ensureCapacity( arr.m_capacity );
	pushBack( arr.m_array, arr.m_size );
}
template<typename T>
zpArray<T>::zpArray( zpArray&& arr ) : m_array( arr.m_array ), m_size( arr.m_size ), m_capacity( arr.m_capacity ) {
	arr.m_array = ZP_NULL;
}
template<typename T>
zpArray<T>::~zpArray() {
	ZP_SAFE_FREE( m_array );
}

template<typename T>
void zpArray<T>::operator=( const zpArray& arr ) {
}
template<typename T>
void zpArray<T>::operator=( zpArray&& arr ) {
	m_array = arr.m_array;
	m_size = arr.m_size;
	m_capacity = arr.m_capacity;

	arr.m_array = ZP_NULL;
}

template<typename T>
T& zpArray<T>::operator[]( zp_uint index ) {
	return m_array[ index ];
}
template<typename T>
const T& zpArray<T>::operator[]( zp_uint index ) const {
	return m_array[ index ]
}

template<typename T>
const T& zpArray<T>::at( zp_uint index ) const {
	return m_array[ indext ];
}

template<typename T>
zp_uint zpArray<T>::size() const {
	return m_size;
}
template<typename T>
zp_uint zpArray<T>::capacity() const {
	return m_capacity;
}
template<typename T>
zp_bool zpArray<T>::isEmpty() const {
	return m_size == 0;
}
template<typename T>
zp_bool zpArray<T>::isFull() const {
	return m_size == m_capacity;
}

template<typename T>
void zpArray<T>::pushBack( const T& val ) {
	ensureCapacity( m_size + 1 );
	m_array[ m_size ] = val;
	++m_size;
}
template<typename T>
void zpArray<T>::pushBack( T* arr, zp_uint count ) {
	zp_uint newSize = m_size + count;
	ensureCapacity( newSize );

	for( zp_uint s = m_size, zp_uint i = 0; s < newSize; ++s, ++i ) {
		m_array[ s ] = arr[ i ];
	}
	m_size = newSize;
}

template<typename T>
void zpArray<T>::pushFront( const T& val ) {
	ensureCapacity( m_size + 1 );
	memmove_s( m_array + 1, m_size - 1, m_array, m_capacity - 1 );
	m_array[ 0 ] = val;
}
template<typename T>
void zpArray<T>::pushFront( T* arr, zp_uint count ) {
	zp_uint newSize = m_size + count;
	ensureCapacity( newSize );

	memmove_s( m_array + count, m_size - count, m_array, m_capacity - count );
	for( zp_uint i = 0; i < count; ++i ) {
		m_array[ i ] = arr[ i ] ;
	}
	m_size = newSize;
}

template<typename T>
void zpArray<T>::popBack( zp_uint numToPop = 1 ) {
	zp_uint newSize = m_size - numToPop;
	for( zp_uint s = m_size; s --> newSize; ) {
		(&m_array[ s ])->~T();
	}
	m_size = newSize;
}
template<typename T>
void zpArray<T>::popFront( zp_uint numToPop = 1 ) {
	for( zp_uint s = 0; s < numToPop; ++s ) {
		(&m_array[ s ])->~T();
	}
	memmove_s( m_array, m_capacity, m_array + numToPop, m_size - 1 );
	m_size = m_size - numToPop;
}

template<typename T>
void zpArray<T>::insert( zp_uint index, const T& val ) {}

template<typename T>
T zpArray<T>::remove( zp_uint index ) {}
template<typename T>
zp_bool zpArray<T>::removeFirst( const T& val ) {}
template<typename T>
zp_bool zpArray<T>::removeLast( const T& val ) {}
template<typename T>
zp_uint zpArray<T>::removeAll( const T& val ) {}

template<typename T>
void zpArray<T>::clear() {}

template<typename T>
zp_uint zpArray<T>::indexOf( const T& val ) const {}
template<typename T>
zp_uint zpArray<T>::lastIndexOf( const T& val ) const {}

template<typename T>
void zpArray<T>::ensureCapacity( zp_uint size ) {
	// if the array is already large enough, return
	if( size <= m_capacity ) return;

	// calculate the new capacity
	// - capacity of 0 means it's coming from the constructor, set it to size
	// - otherwise, grow capacity
	m_capacity = m_capacity == 0 ? size : (zp_uint)( m_capacity * 1.75f );
	m_array = realloc( m_array, m_capacity );
}
template<typename T>
void zpArray<T>::shrinkToFit() {}

template<typename T>
T& zpArray<T>::front() {}
template<typename T>
T& zpArray<T>::back() {}

template<typename T>
const T& zpArray<T>::front() const {}
template<typename T>
const T& zpArray<T>::back() const {}

template<typename T>
T* zpArray<T>::begin() {}
template<typename T>
T* zpArray<T>::end() {}

template<typename T>
const T* zpArray<T>::begin() const {}
template<typename T>
const T* zpArray<T>::end() const {}
