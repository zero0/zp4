#include "zpRendering.h"

zpStaticMeshResource::zpStaticMeshResource() :
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

void zpStaticMeshResource::unload() {
	if( m_vertexBuffer ) {
		m_vertexBuffer->release();
		m_vertexBuffer = ZP_NULL;
	}
	if( m_indexBuffer ) {
		m_indexBuffer->release();
		m_indexBuffer = ZP_NULL;
	}
}