#pragma once
#ifndef ZP_RESOURCE_H
#define ZP_RESOURCE_H

template<typename Resource>
class zpResource
{
public:
	zpResource();
	~zpResource();

	const Resource* getData() const;

	const zpString& getFilename() const;

	void addRef();
	void releaseRef();
	zp_uint getRefCount() const;

	zp_bool isLoaded() const;

protected:
	zp_bool m_isLoaded;
	zp_uint m_refCount;
	zpString m_filename;

	Resource* m_resource;
};

#include "zpResource.inl"

#endif