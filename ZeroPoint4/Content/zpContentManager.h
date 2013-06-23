#pragma once
#ifndef ZP_CONTENT_MANAGER_H
#define ZP_CONTENT_MANAGER_H

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
class zpContentManager;

template<typename Resource>
class zpResourceInstance
{
public:
	zpResourceInstance() : m_resource( ZP_NULL ) {}
	~zpResourceInstance() { m_resource = ZP_NULL; }

	const Resource* getResource() const { return m_resource; }

private:
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

	zp_bool getResource( const zp_char* filename, ResourceInstance& outInstance );
	zp_bool reloadResource( const zp_char* filename );
	void releaseResource( ResourceInstance& instance );

	void garbageCollect();

private:
	zpFixedArrayList< Resource, ResourceCount > m_resources;
};

#include "zpContentManager.inl"



#endif