#pragma once
#ifndef ZP_TEXTURE_INSTANCE_H
#define ZP_TEXTURE_INSTANCE_H

template<>
class zpResourceInstance<zpTextureResource> {
public:
	zpResourceInstance();
	zpResourceInstance( zpTextureResource* resource );
	~zpResourceInstance();

	void operator=( const zpResourceInstance<zpTextureResource>& instance );
	void operator=( zpResourceInstance<zpTextureResource>&& instance );

	operator zp_bool() const;

	zpTextureResource* getTextureResource() const;

private:
	zpTextureResource* m_resource;
};

#endif