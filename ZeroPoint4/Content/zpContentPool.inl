template<typename T, zp_uint Count>
zpContentPool<T, Count>::zpContentPool()
{
	zp_zero_memory_array( m_pool );
	T* t = (T*)m_pool;

	for( zp_uint i = 0; i < Count; ++i, ++t )
	{
		m_free.pushBack( t );
	}
}
template<typename T, zp_uint Count>
zpContentPool<T, Count>::~zpContentPool()
{}

template<typename T, zp_uint Count> template<typename R>
T* zpContentPool<T, Count>::create( const R& param )
{
	T* ptr = ZP_NULL;

	if( !m_free.isEmpty() )
	{
		ptr = m_free.back();
		m_free.popBack();
		m_used.pushBack( ptr );

		zp_zero_memory( ptr );
		new (ptr) T( param );
	}

	return ptr;
}
template<typename T, zp_uint Count> template<typename R0, typename R1>
T* zpContentPool<T, Count>::create( const R0& param0, const R1& param1 )
{
	T* ptr = ZP_NULL;

	if( !m_free.isEmpty() )
	{
		ptr = m_free.back();
		m_free.popBack();
		m_used.pushBack( ptr );

		zp_zero_memory( ptr );
		new (ptr) T( param0, param1 );
	}

	return ptr;
}

template<typename T, zp_uint Count>
void zpContentPool<T, Count>::destroy( T* obj )
{
	zp_int at = m_used.indexOf( obj );
	if( at >= 0 )
	{
		T* ptr =  m_used[ at ];
		ptr->~T();

		m_free.pushBack( ptr );
		m_used.erase( at );
	}
}

template<typename T, zp_uint Count>
zp_uint zpContentPool<T, Count>::size() const
{
	return Count;
}
