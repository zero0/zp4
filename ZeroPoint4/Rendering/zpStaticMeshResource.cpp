#include "zpRendering.h"

zpStaticMeshPart::zpStaticMeshPart()
	: topology( ZP_TOPOLOGY_TRIANGLE_LIST )
	, numVertices( 0 )
	, vertexBuffer( ZP_NULL )
	, indexBuffer( ZP_NULL )
{}
zpStaticMeshPart::~zpStaticMeshPart() {
	if( vertexBuffer ) vertexBuffer->destroy();
	if( indexBuffer ) indexBuffer->destroy();
}

zpStaticMeshResource::zpStaticMeshResource()
	: m_meshParts( 1 )
{}
zpStaticMeshResource::~zpStaticMeshResource() {
	unload();
}

const zpStaticMeshPart& zpStaticMeshResource::getMeshPart( zp_uint index ) const {
	return m_meshParts[ index ];
}

zp_uint zpStaticMeshResource::getNumMeshParts() const {
	return m_meshParts.size();
}

void zpStaticMeshResource::unload() {
	m_meshParts.clear();
}