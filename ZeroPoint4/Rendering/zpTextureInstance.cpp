#include "zpRendering.h"

zpResourceInstance<zpTextureResource>::zpResourceInstance()
	: m_resource( ZP_NULL )
{}
zpResourceInstance<zpTextureResource>::zpResourceInstance( zpTextureResource* resource )
	: m_resource( resource )
{}
zpResourceInstance<zpTextureResource>::~zpResourceInstance() {
	m_resource = ZP_NULL;
}

void zpResourceInstance<zpTextureResource>::operator=( const zpResourceInstance<zpTextureResource>& instance ) {
	m_resource = instance.m_resource;
}
void zpResourceInstance<zpTextureResource>::operator=( zpResourceInstance<zpTextureResource>&& instance ) {
	m_resource = instance.m_resource;

	instance.m_resource = ZP_NULL;
}

zpResourceInstance<zpTextureResource>::operator zp_bool() const {
	return m_resource && m_resource->isLoaded();
}

zpTextureResource* zpResourceInstance<zpTextureResource>::getTextureResource() const {
	return m_resource;
}
