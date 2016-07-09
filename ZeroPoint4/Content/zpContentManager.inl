
template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::zpContentManager()
    : m_application( ZP_NULL )
{
    m_resources.resize( ResourceCount );
}
template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::~zpContentManager()
{
    Resource* res = m_resources.begin();
    Resource* end = m_resources.end();

    for( ; res != end; ++res )
    {
        ZP_ASSERT( res->getRefCount() == 0, "Resource %s still loaded", res->getFilename().str() );
    }
    m_resources.clear();
    m_application = ZP_NULL;
}

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
zp_bool zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::getResource( const zpString& filename, ResourceInstance& outInstance )
{
    return getResource( filename.str(), outInstance );
}

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
zp_bool zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::getResource( const zp_char* filename, ResourceInstance& outInstance )
{
    Resource* empty = ZP_NULL;
    Resource* found = ZP_NULL;
    Resource* res = m_resources.begin();
    Resource* end = m_resources.end();

    for( ; res != end; ++res )
    {
        if( res->getRefCount() == 0 )
        {
            if( empty == ZP_NULL )
            {
                empty = res;
            }
        }
        else if( res->m_isLoadedFromFile && res->getFilename() == filename )
        {
            found = res;
            break;
        }
    }

    ImplManager *impl = (ImplManager*)this; //dynamic_cast<ImplManager*>( this );

    // if the resource was already loaded, add a ref and return
    if( found )
    {
        outInstance.release();
        outInstance.m_resource = found;
        found->addRef();

        //outInstance.initialized();
        impl->initializeInstance( outInstance );

        return true;
    }

    // if the resource was not found, make sure there is an empty slot to put it in
    ZP_ASSERT( empty != ZP_NULL, "" );
    if( empty == ZP_NULL )
    {
        return false;
    }

    // if the empty slot was loaded (between garbage collects), unload it
    if( empty->isLoaded() )
    {
        impl->destroyResource( empty );
        empty->m_isLoaded = false;
        empty->m_isLoadedFromFile = false;
    }

    // create the resource
    //if( impl != ZP_NULL )
    {
        // if the resource was able to create, add ref and return
        if( impl->createResource( empty, filename ) )
        {
            outInstance.release();
            outInstance.m_resource = empty;
            empty->addRef();
            empty->m_isLoaded = true;
            empty->m_isLoadedFromFile = true;
            empty->m_lastTimeLoaded = zpFile::getFileModificationTime( filename );

            //outInstance.initialized();
            impl->initializeInstance( outInstance );

            return true;
        }
    }

    return false;
}

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount> template< typename ResourceType >
zp_bool zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::getResourceWithoutLoad( ResourceInstance& outInstance, ResourceType*& resource )
{
    Resource* empty = ZP_NULL;
    Resource* res = m_resources.begin();
    Resource* end = m_resources.end();

    for( ; res != end; ++res )
    {
        if( res->getRefCount() == 0 )
        {
            empty = res;
            break;
        }
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
        empty->m_isLoadedFromFile = false;
    }

    // create the resource
    //if( impl != ZP_NULL )
    {
        // if the resource was able to create, add ref and return
        outInstance.release();
        outInstance.m_resource = empty;
        empty->addRef();
        empty->m_isLoaded = true;
        empty->m_isLoadedFromFile = false;
        resource = &empty->m_resource;
        empty->m_lastTimeLoaded = 0; // TODO: fix by actually get the time

        //outInstance.initialized();
        impl->initializeInstance( outInstance );

        return true;
    }

    return false;
}

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
zp_bool zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::reloadResource( const zp_char* filename )
{
    Resource* found = ZP_NULL;
    Resource* res = m_resources.begin();
    Resource* end = m_resources.end();

    for( ; res != end; ++res )
    {
        if( res->m_isLoadedFromFile && res->getFilename() == filename )
        {
            found = res;
            break;
        }
    }

    if( found )
    {
        ImplManager *impl = (ImplManager*)this; //dynamic_cast<ImplManager*>( this );

        if( found->isLoaded() )
        {
            impl->destroyResource( found );
            found->m_isLoaded = false;
            found->m_isLoadedFromFile = false;
        }

        if( impl->createResource( found, filename ) )
        {
            found->m_isLoaded = true;
            found->m_isLoadedFromFile = true;
            found->m_lastTimeLoaded = zpFile::getFileModificationTime( filename );
            return true;
        }
    }

    return false;
}

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
void zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::reloadAllResources()
{
    zpString filename;

    Resource* found = ZP_NULL;
    Resource* res = m_resources.begin();
    Resource* end = m_resources.end();

    ImplManager *impl = (ImplManager*)this; //dynamic_cast<ImplManager*>( this );

    for( ; res != end; ++res )
    {
        if( res->m_isLoadedFromFile && res->getRefCount() > 0 )
        {
            filename = res->getFilename();
            if( res->isLoaded() )
            {
                impl->destroyResource( res );
                res->m_isLoaded = false;
                res->m_isLoadedFromFile = false;
            }

            if( impl->createResource( res, filename.str() ) )
            {
                res->m_isLoaded = true;
                res->m_isLoadedFromFile = true;
                res->m_lastTimeLoaded = zpFile::getFileModificationTime( filename.str() );
            }
        }
    }
}

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
void zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::garbageCollect()
{
    ImplManager *impl = (ImplManager*)this; //dynamic_cast<ImplManager*>( this );
    Resource* res = m_resources.begin();
    Resource* end = m_resources.end();

    for( ; res != end; ++res )
    {
        if( res->getRefCount() == 0 && res->isLoaded() )
        {
            impl->destroyResource( res );
            res->m_isLoaded = false;
            res->m_isLoadedFromFile = false;
        }
    }
}

#if ZP_USE_HOT_RELOAD
template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
void zpContentManager<Resource, ResourceInstance, ImplManager, ResourceCount>::reloadChangedResources()
{
    zp_long mtime;
    Resource* found = ZP_NULL;
    Resource* res = m_resources.begin();
    Resource* end = m_resources.end();

    ImplManager *impl = (ImplManager*)this; //dynamic_cast<ImplManager*>( this );
    zpString filename;

    for( ; res != end; ++res )
    {
        if( res->m_isLoadedFromFile && res->getRefCount() > 0 )
        {
            mtime = zpFile::getFileModificationTime( res->getFilename().str() );
            if( res->m_lastTimeLoaded != mtime )
            {
                filename = res->getFilename();

                if( res->isLoaded() )
                {
                    impl->destroyResource( res );
                    res->m_isLoaded = false;
                    res->m_isLoadedFromFile = false;
                }

                if( impl->createResource( res, filename.str() ) )
                {
                    res->m_isLoaded = true;
                    res->m_isLoadedFromFile = true;
                    res->m_lastTimeLoaded = mtime;
                }

                zp_printfln( "Asset %s reloaded at %d", filename.str(), mtime );
            }
        }
    }
}
#endif
