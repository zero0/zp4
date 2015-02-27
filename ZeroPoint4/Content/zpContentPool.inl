template<typename T, zp_uint Count>
zpContentPool<T, Count>::zpContentPool()
	: m_memory( ZP_NULL )
{}
template<typename T, zp_uint Count>
zpContentPool<T, Count>::~zpContentPool()
{}

template<typename T, zp_uint Count>
void zpContentPool<T, Count>::setup( zpMemorySystem* memory )
{
	for( zp_uint i = 0, imax = Count; i < imax; ++i )
	{
		T* t = (T*)memory->allocate( sizeof( T ) );
		m_free.pushBack( t );
	}

	m_memory = memory;
}

template<typename T, zp_uint Count>
void zpContentPool<T, Count>::teardown()
{
	ZP_ASSERT( m_used.isEmpty(), "Not all objects destroyed %d", m_used.size() );

	T** b = m_free.begin();
	T** e = m_free.end();
	for( ; b != e; ++b )
	{
		T* t = *b;
		m_memory->deallocate( t );
	}

	m_free.clear();
	m_used.clear();
}

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

template<typename T, zp_uint Count>
T* zpContentPool<T, Count>::getUsed( zp_uint index )
{
	return m_used[ index ];
}
template<typename T, zp_uint Count>
zp_uint zpContentPool<T, Count>::getUsedCount() const
{
	return m_used.size();
}
