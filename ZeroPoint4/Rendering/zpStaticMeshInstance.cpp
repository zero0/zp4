#include "zpRendering.h"

zpResourceInstance<zpStaticMeshResource>::zpResourceInstance()
	: m_resource( ZP_NULL )
{}
zpResourceInstance<zpStaticMeshResource>::zpResourceInstance( zpStaticMeshResource* resource )
	: m_resource( resource )
{}
zpResourceInstance<zpStaticMeshResource>::~zpResourceInstance() {
	m_resource = ZP_NULL;
}

void zpResourceInstance<zpStaticMeshResource>::operator=( const zpResourceInstance<zpStaticMeshResource>& instance ) {
	m_resource = instance.m_resource;
}
void zpResourceInstance<zpStaticMeshResource>::operator=( zpResourceInstance<zpStaticMeshResource>&& instance ) {
	m_resource = instance.m_resource;

	instance.m_resource = ZP_NULL;
}

zpResourceInstance<zpStaticMeshResource>::operator zp_bool() const {
	return m_resource && m_resource->isLoaded();
}

zpStaticMeshResource* zpResourceInstance<zpStaticMeshResource>::getStaticMeshResource() const {
	return m_resource;
}
