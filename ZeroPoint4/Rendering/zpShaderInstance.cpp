#include "zpRendering.h"

zpResourceInstance<zpShaderResource>::zpResourceInstance() 
	: m_resource( ZP_NULL )
{}
zpResourceInstance<zpShaderResource>::zpResourceInstance( zpShaderResource* resource )
	: m_resource( resource )
{}
zpResourceInstance<zpShaderResource>::~zpResourceInstance() {
	m_resource = ZP_NULL;
}

void zpResourceInstance<zpShaderResource>::operator=( const zpResourceInstance<zpShaderResource>& instance ) {
	m_resource = instance.m_resource;
}
void zpResourceInstance<zpShaderResource>::operator=( zpResourceInstance<zpShaderResource>&& instance ) {
	m_resource = instance.m_resource;

	instance.m_resource = ZP_NULL;
}

zpResourceInstance<zpShaderResource>::operator zp_bool() const {
	return m_resource && m_resource->isLoaded();
}

zpShaderResource* zpResourceInstance<zpShaderResource>::getShaderResource() const {
	return m_resource;
}
