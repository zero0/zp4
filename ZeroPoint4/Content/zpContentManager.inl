
template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::zpContentManager()
{
	while( m_resources.size() < ResourceCount )
	{
		m_resources.pushBackEmpty();
	}
}
template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::~zpContentManager()
{
	for( zp_uint i = 0; i < ResourceCount; ++i )
	{
		Resource* r = &m_resources[ i ];
		ZP_ASSERT( r->getRefCount() == 0, "Resource %s still loaded", r->getFilename().getChars() );
	}
	m_resources.clear();
}

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
zp_bool zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::getResource( const zp_char* filename, ResourceInstance& outInstance )
{
	Resource* empty = ZP_NULL;
	Resource* found = ZP_NULL;
	for( zp_uint i = 0; i < ResourceCount; ++i )
	{
		Resource* r = &m_resources[ i ];
		if( r->getRefCount() == 0 )
		{
			if( empty == ZP_NULL )
			{
				empty = r;
			}
		}
		else if( r->getFilename() == filename )
		{
			found = r;
			break;
		}
	}

	// if the resource was already loaded, add a ref and return
	if( found )
	{
		outInstance.m_resource = found;
		found->addRef();

		return true;
	}

	// if the resource was not found, make sure there is an empty slot to put it in
	ZP_ASSERT( empty != ZP_NULL, "" );
	if( empty == ZP_NULL )
	{
		return false;
	}

	ImplManager *impl = (ImplManager*)this; //dynamic_cast<ImplManager*>( this );

	// if the empty slot was loaded (between garbage collects), unload it
	if( empty->isLoaded() )
	{
		impl->destroyResource( empty );
		empty->m_isLoaded = false;
	}

	// create the resource
	if( impl != ZP_NULL )
	{
		// if the resource was able to create, add ref and return
		if( impl->createResource( empty, filename ) )
		{
			outInstance.m_resource = empty;
			empty->addRef();
			empty->m_isLoaded = true;

			return true;
		}
	}

	return false;
}
template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
void zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::releaseResource( ResourceInstance& instance )
{
	if( instance.getResource() == ZP_NULL )
	{
		return;
	}

	Resource* found = ZP_NULL;
	for( zp_uint i = 0; i < ResourceCount; ++i )
	{
		if( &m_resources[ i ] == instance.getResource() )
		{
			found = &m_resources[ i ];
			break;
		}
	}

	ZP_ASSERT( found != ZP_NULL, "" );
	if( found != ZP_NULL )
	{
		ImplManager *impl = (ImplManager*)this; //dynamic_cast<ImplManager*>( this );
		if( impl != ZP_NULL )
		{
			impl->destroyResource( found );

			instance.m_resource = ZP_NULL;
			found->releaseRef();
			found->m_isLoaded = false;
		}
	}
}

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
void zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::garbageCollect()
{
	Resource* r;
	ImplManager *impl = (ImplManager*)this; //dynamic_cast<ImplManager*>( this );
	for( zp_uint i = 0; i < ResourceCount; ++i )
	{
		r = &m_resources[ i ];
		if( r->getRefCount() == 0 && r->isLoaded() )
		{
			impl->destroyResource( r );
		}
	}
}
