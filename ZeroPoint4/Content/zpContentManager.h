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
	zpResourceInstance() : m_resource( ZP_NULL ) {}
	virtual ~zpResourceInstance()
	{
		release();
	}

	zpResourceInstance( const zpResourceInstance< Resource >& other )
	{
		release();

		m_resource = other.m_resource;
		if( m_resource )
		{
			m_resource->addRef();
		}
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
		if( m_resource )
		{
			m_resource->addRef();
		}
	}
	void operator=( zpResourceInstance< Resource >&& other )
	{
		release();

		m_resource = other.m_resource;
		other.m_resource = ZP_NULL;
	}

	const Resource* getResource() const { return m_resource; }

	zp_bool isVaild() const { return m_resource != ZP_NULL; }

	void release()
	{
		if( m_resource )
		{
			m_resource->releaseRef();
		}
		m_resource = ZP_NULL;
	}

private:
	virtual void initialized() {};

	Resource* m_resource;

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};



template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
class zpContentManager
{
	ZP_NON_COPYABLE( zpContentManager );

public:
	zpContentManager();
	~zpContentManager();

	zp_bool getResource( const zpString& filename, ResourceInstance& outInstance );
	zp_bool getResource( const zp_char* filename, ResourceInstance& outInstance );

	zp_bool reloadResource( const zp_char* filename );
	void reloadAllResources();

	void garbageCollect();

	void setApplication( zpApplication* app ) { m_application = app; }
	zpApplication* getApplication() { return m_application; }

private:
	zpApplication* m_application;
	zpFixedArrayList< Resource, ResourceCount > m_resources;
};

#include "zpContentManager.inl"



#endif