#pragma once
#ifndef ZP_MESH_H
#define ZP_MESH_H

class zpMeshContentManager;

struct zpMeshPart
{
	zp_uint m_indexOffset;
	zp_uint m_indexCount;
	zp_uint m_vertexOffset;
	zp_uint m_vertexCount;
	zpBoundingAABB m_boundingBox;
};

struct zpMesh
{
	zpVertexFormat m_format;
	zpBuffer m_vertex;
	zpBuffer m_index;
	zpArrayList< zpMeshPart > m_parts;
};

class zpMeshResource : public zpResource< zpMesh >
{
private:
	zp_bool load( const zp_char* filename, zpRenderingPipeline* pipeline );
	void unload( zpRenderingEngine* engine );

	friend class zpMeshContentManager;
};

class zpMeshResourceInstance : public zpResourceInstance< zpMeshResource >
{
};

class zpMeshContentManager : public zpContentManager< zpMeshResource, zpMeshResourceInstance, zpMeshContentManager, 64 >
{
private:
	zp_bool createResource( zpMeshResource* res, const zp_char* filename );
	void destroyResource( zpMeshResource* res );
	void initializeInstance( zpMeshResourceInstance& instance ) {}

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
