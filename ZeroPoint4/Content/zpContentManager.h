#pragma once
#ifndef ZP_CONTENT_MANAGER_H
#define ZP_CONTENT_MANAGER_H

class zpApplication;

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
class zpContentManager;



template<typename Resource>
class zpResourceInstance
{
public:
    zpResourceInstance()
        : m_resource( ZP_NULL )
    {}
    virtual ~zpResourceInstance()
    {
        release();
    }

    zpResourceInstance( const zpResourceInstance< Resource >& other )
        : m_resource( other.m_resource )
    {
        addReference();
    }
    zpResourceInstance( zpResourceInstance< Resource >&& other )
        : m_resource( other.m_resource )
    {
        other.m_resource = ZP_NULL;
    }

    void operator=( const zpResourceInstance< Resource >& other )
    {
        release();

        m_resource = other.m_resource;
        addReference();
    }
    void operator=( zpResourceInstance< Resource >&& other )
    {
        release();

        m_resource = other.m_resource;
        other.m_resource = ZP_NULL;
    }

    const Resource* getResource() const { return m_resource; }

    zp_bool isVaild() const { return m_resource != ZP_NULL; }

    zp_uint release()
    {
        zp_uint refCount = 0;
        if( m_resource )
        {
            m_resource->releaseRef();
            refCount = m_resource->getRefCount();
        }
        m_resource = ZP_NULL;
        return refCount;
    }

private:
    Resource* m_resource;

    void addReference()
    {
        if( m_resource )
        {
            m_resource->addRef();
        }
    }

    template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
    friend class zpContentManager;
};



template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
class zpContentManager
{
    ZP_NON_COPYABLE( zpContentManager );

public:
    zpContentManager();
    virtual ~zpContentManager();

    //zp_bool getResource(  const zp_char* filename, ResourceInstance* outInstance ) {
    //    return getResource( filename, *outInstance ); }
    //zp_bool getResource(  const zpString& filename, ResourceInstance* outInstance ) {
    //    return getResource( filename.str(), *outInstance ); }

    zp_bool getResource( const zpString& filename, ResourceInstance& outInstance );
    zp_bool getResource( const zp_char* filename, ResourceInstance& outInstance );

    template< typename ResourceType >
    zp_bool getResourceWithoutLoad( ResourceInstance& outInstance, ResourceType*& resource );

    zp_bool reloadResource( const zp_char* filename );
    void reloadAllResources();

    void garbageCollect();

    void setApplication( zpApplication* app ) { m_application = app; }
    zpApplication* getApplication() { return m_application; }

#if ZP_USE_HOT_RELOAD
    void reloadChangedResources();
#endif

private:
    zpApplication* m_application;
    zpFixedArrayList< Resource, ResourceCount > m_resources;
};

#include "zpContentManager.inl"

#endif