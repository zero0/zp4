template<typename T, zp_uint Count>
zpPooledObject<T, Count>::zpPooledObject()
	//: m_lastUsedIndex( 0 )
	//, m_pool( (T*)zp_calloc( Count, sizeof( T ) ) )
{}
template<typename T, zp_uint Count>
zpPooledObject<T, Count>::~zpPooledObject()
{
	//ZP_SAFE_FREE( m_pool );
}

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
			markUsed( i, true );
			//new (&m_pool[ i ]) T;
			return &m_pool[ i ];
		}
	}
	return ZP_NULL;
}
template<typename T, zp_uint Count>
void zpPooledObject<T, Count>::release( T* obj )
{
	for( zp_uint i = 0; i < Count; ++i )
	{
		if( &m_pool[ i ] == obj )
		{
			markUsed( i, false );
			//(&m_pool[ i ])->~T();
			return;
		}
	}
}

template<typename T, zp_uint Count>
void zpPooledObject<T, Count>::markUsed( zp_uint index, zp_bool used )
{
	if( used )
	{
		m_isUsed[ index / 8 ].mark( index % 8 );
	}
	else
	{
		m_isUsed[ index / 8 ].unmark( index % 8 );
	}
}
template<typename T, zp_uint Count>
zp_bool zpPooledObject<T, Count>::isUsed( zp_uint index ) const
{
	return m_isUsed[ index / 8 ].isMarked( index % 8 );
}