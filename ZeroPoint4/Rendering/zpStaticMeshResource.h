#pragma once
#ifndef ZP_STATIC_MESH_RESOURCE_H
#define ZP_STATIC_MESH_RESOURCE_H

ZP_ABSTRACT_CLASS zpStaticMeshResource : public zpResource {
public:
	zpStaticMeshResource();
	virtual ~zpStaticMeshResource();

	zpBuffer* getVertexBuffer() const;
	zpBuffer* getIndexBuffer() const;

	zp_uint getNumVertices() const;

	void unload();

protected:
	zp_uint m_numVertices;

	zpBuffer* m_vertexBuffer;
	zpBuffer* m_indexBuffer;
};

#endif