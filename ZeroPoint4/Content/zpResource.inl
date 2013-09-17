
template<typename Resource>
zpResource<Resource>::zpResource()
	: m_isLoaded( false )
	, m_refCount( 0 )
	, m_lastTimeLoaded( 0 )
	, m_filename()
	, m_resource()
{}
template<typename Resource>
zpResource<Resource>::~zpResource()
{}

template<typename Resource>
const Resource* zpResource<Resource>::getData() const
{
	return &m_resource;
}

template<typename Resource>
const zpString& zpResource<Resource>::getFilename() const
{
	return m_filename;
}

template<typename Resource>
void zpResource<Resource>::addRef()
{
	++m_refCount;
}
template<typename Resource>
void zpResource<Resource>::releaseRef()
{
	ZP_ASSERT( m_refCount != 0, "Trying to release an empty ref count" );
	--m_refCount;
}
template<typename Resource>
zp_uint zpResource<Resource>::getRefCount() const
{
	return m_refCount;
}

template<typename Resource>
zp_bool zpResource<Resource>::isLoaded() const
{
	return m_isLoaded;
}

template<typename Resource>
zp_long zpResource<Resource>::getLastTimeLoaded() const
{
	return m_lastTimeLoaded;
}
