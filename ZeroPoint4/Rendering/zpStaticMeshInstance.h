#pragma once
#ifndef ZP_STATIC_MESH_INSTANCE_H
#define ZP_STATIC_MESH_INSTANCE_H

template<>
class zpResourceInstance<zpStaticMeshResource> {
public:
	zpResourceInstance();
	zpResourceInstance( zpStaticMeshResource* resource );
	~zpResourceInstance();

	void operator=( const zpResourceInstance<zpStaticMeshResource>& instance );
	void operator=( zpResourceInstance<zpStaticMeshResource>&& instance );

	operator zp_bool() const;

	zpStaticMeshResource* getStaticMeshResource() const;

private:
	zpStaticMeshResource* m_resource;
};

#endif