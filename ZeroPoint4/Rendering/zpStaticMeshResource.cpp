#include "zpRendering.h"

zpStaticMeshResource::zpStaticMeshResource() :
	m_numVertices( 0 ),
	m_vertexBuffer( ZP_NULL ),
	m_indexBuffer( ZP_NULL )
{}
zpStaticMeshResource::~zpStaticMeshResource() {
	unload();
}

zpBuffer* zpStaticMeshResource::getVertexBuffer() const {
	return m_vertexBuffer;
}
zpBuffer* zpStaticMeshResource::getIndexBuffer() const {
	return m_indexBuffer;
}

zp_uint zpStaticMeshResource::getNumVertices() const {
	return m_numVertices;
}

void zpStaticMeshResource::unload() {
	if( m_vertexBuffer ) {
		m_vertexBuffer->destroy();
		m_vertexBuffer = ZP_NULL;
	}
	if( m_indexBuffer ) {
		m_indexBuffer->destroy();
		m_indexBuffer = ZP_NULL;
	}
}