#pragma once
#ifndef ZP_CONTENT_MANAGER_H
#define ZP_CONTENT_MANAGER_H

template<typename Resource>
class zpResource
{
public:
	const Resource* getResource() const { return &m_resource; }

	zp_bool load( const zp_char* filename );
	void unload();

	void addRef();
	void releaseRef();

private:
	zp_bool m_isInUse;
	zp_bool m_isLoaded;
	zp_uint m_refCount;
	zpString m_filename;
	Resource m_resource;
};


template<typename Resource>
class zpResourceInstance
{
public:
	const Resource* getResource() const { return 0; }
};


class zpTextResource : public zpResource< zpBison >
{

};

class zpTextResourceInstance : public zpResourceInstance< zpTextResource >
{

};

template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
class zpContentManager
{
	ZP_NON_COPYABLE( zpContentManager );

public:
	zpContentManager();
	~zpContentManager();

	

private:
	zpFixedArrayList< Resource, ResourceCount > m_resources;
};

class zpTextContentManager : public zpContentManager< zpTextResource, zpTextResourceInstance, zpTextContentManager, 16 >
{

};

void aa()
{
	zpTextContentManager fff;
}
#endif