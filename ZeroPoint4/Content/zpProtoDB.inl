
template< typename T >
zpProtoDBHandle<T>::zpProtoDBHandle()
	: m_protoId()
	, m_protoDB( ZP_NULL )
	, m_index( ZP_PROTODB_INVALID_HANDLE )
	, m_data( ZP_NULL )
{}
template< typename T >
zpProtoDBHandle<T>::~zpProtoDBHandle()
{
	invalidate();
}

template< typename T >
zp_bool zpProtoDBHandle<T>::isValid() const
{
	return m_index != ZP_PROTODB_INVALID_HANDLE;
}
template< typename T >
const T* zpProtoDBHandle<T>::getData() const
{
	if( m_index == ZP_PROTODB_INVALID_HANDLE )
	{
		m_index = m_protoDB->m_entries.indexOf( m_protoId );
		m_data = m_protoDB->m_database[ m_index ];
	}

	return m_data;
}

template< typename T >
const zpString& zpProtoDBHandle<T>::getProtoId() const
{
	return m_protoId;
}

template< typename T >
void zpProtoDBHandle<T>::invalidate() const
{
	m_index = ZP_PROTODB_INVALID_HANDLE;
	m_data = ZP_NULL;
}

template< typename T >
zpProtoDB<T>::zpProtoDB()
{}
template< typename T >
zpProtoDB<T>::~zpProtoDB()
{}

template< typename T >
void zpProtoDB<T>::initialize()
{
	// setup handles
	m_handles.resize( 32 );

	zpProtoDBHandle< T >* b = m_handles.begin();
	zpProtoDBHandle< T >* e = m_handles.begin();
	for( ; b != e; ++b )
	{
		m_freeHandles.pushBack( b );
	}
}
template< typename T >
void zpProtoDB<T>::setup( const zpBison::Value& protoDb )
{
	// invalidate existing handles
	invalidateHandles();

	// setup database
	zp_uint count = protoDb.size();
	m_entries.reserve( count );
	m_database.reserve( count );
	protoDb.foreachObject( [ this ]( const zpBison::Value& k, const zpBison::Value& v ) {
		m_entries.pushBackEmpty() = k.asCString();
		m_database.pushBackEmpty().load( v );
	} );
}
template< typename T >
void zpProtoDB<T>::shutdown()
{
	m_entries.clear();
	m_database.clear();
}
template< typename T >
void zpProtoDB<T>::destroy()
{
	ZP_ASSERT( m_usedHandles.isEmpty(), "Still ProtoDB Handles in use" );

	m_freeHandles.clear();
	m_usedHandles.clear();
	m_handles.clear();
}

template< typename T >
const zpProtoDBHandle< T >* zpProtoDB<T>::getHandle( const zp_char* protoId )
{
	ZP_ASSERT( !m_freeHandles.isEmpty(), "Ran out of ProtoDB Handles" );

	zpProtoDBHandle< T >* handle = m_freeHandles.back();
	m_freeHandles.popBack();
	m_usedHandles.pushBack( handle );

	handle->invalidate();
	handle->m_protoDB = this;
	handle->m_protoId = protoId;

	return handle;
}
template< typename T >
void zpProtoDB<T>::releaseHandle( const zpProtoDBHandle< T >*& handle )
{
	handle->invalidate();
	m_usedHandles.eraseAll( handle );
	m_freeHandles.pushBack( handle );

	handle = ZP_NULL;
}

template< typename T >
void zpProtoDB<T>::invalidateHandles()
{
	zpProtoDBHandle< T >** b = m_usedHandles.begin();
	zpProtoDBHandle< T >** e = m_usedHandles.begin();
	for( ; b != e; ++b )
	{
		(*b)->invalidate();
	}
}
