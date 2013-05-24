template<typename T, zp_uint Count>
zpPooledObject<T, Count>::zpPooledObject()
	: m_capacity( Count * sizeof( T ) )
{}
template<typename T, zp_uint Count>
zpPooledObject<T, Count>::~zpPooledObject()
{}

template<typename T, zp_uint Count>
zpPooledObject<T, Count>* zpPooledObject<T, Count>::getInstance()
{
	static zpPooledObject<T, Count> s_instance;
	return &s_instance;
}

template<typename T, zp_uint Count>
T* zpPooledObject<T, Count>::aquire()
{
	for( zp_uint i = 0; i < Count; ++i )
	{
		if( !isUsed( i ) )
		{
			T* ptr = ( ( (T*)m_pool ) + i );
			markUsed( i, true );
			new (ptr) T;
			return ptr;
		}
	}
	return ZP_NULL;
}
template<typename T, zp_uint Count>
void zpPooledObject<T, Count>::release( T* obj )
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
void zpPooledObject<T, Count>::markUsed( zp_uint index, zp_bool used )
{
	m_isUsed[ index / 8 ].setMarked( index % 8, used );
}
template<typename T, zp_uint Count>
zp_bool zpPooledObject<T, Count>::isUsed( zp_uint index ) const
{
	return m_isUsed[ index / 8 ].isMarked( index % 8 );
}