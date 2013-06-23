template<typename T, zp_uint Count>
zpContentPool<T, Count>::zpContentPool()
{}
template<typename T, zp_uint Count>
zpContentPool<T, Count>::~zpContentPool()
{}

template<typename T, zp_uint Count>
T* zpContentPool<T, Count>::create( const zp_char* filename )
{
	for( zp_uint i = 0; i < Count; ++i )
	{
		if( !isUsed( i ) )
		{
			T* ptr = ( ( (T*)m_pool ) + i );
			markUsed( i, true );
			new (ptr) T( filename );
			return ptr;
		}
	}
	return ZP_NULL;
}
template<typename T, zp_uint Count>
void zpContentPool<T, Count>::destroy( T* obj )
{
	for( zp_uint i = 0; i < Count; ++i )
	{
		T* ptr = ( ( (T*)m_pool ) + i );
		if( ptr == obj )
		{
			markUsed( i, false );
			ptr->~T();
			zp_zero_memory( ptr );
			return;
		}
	}
}

template<typename T, zp_uint Count>
zp_uint zpContentPool<T, Count>::size() const
{
	return Count;
}

template<typename T, zp_uint Count>
T* zpContentPool<T, Count>::at( zp_uint index )
{
	return ( ( (T*)m_pool ) + index );
}

template<typename T, zp_uint Count>
void zpContentPool<T, Count>::markUsed( zp_uint index, zp_bool used )
{
	zp_uint i = ( index / 8 );
	m_isUsed[ i ].setMarked( index % 8, used );
}
template<typename T, zp_uint Count>
zp_bool zpContentPool<T, Count>::isUsed( zp_uint index ) const
{
	zp_uint i = ( index / 8 );
	return m_isUsed[ i ].isMarked( index % 8 );
}
