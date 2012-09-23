#pragma once
#ifndef ZP_STATIC_MESH_RESOURCE_H
#define ZP_STATIC_MESH_RESOURCE_H

#define ZP_STATIC_MESH_PART_NUM_TEXTURES	4

struct zpStaticMeshPart {
	zpTopology topology;
	zpBuffer* vertexBuffer;
	zpBuffer* indexBuffer;
	zpResourceInstance<zpShaderResource> shader;
	zpResourceInstance<zpTextureResource> textures[ ZP_STATIC_MESH_PART_NUM_TEXTURES ];

	zpStaticMeshPart();
	~zpStaticMeshPart();
};

ZP_ABSTRACT_CLASS zpStaticMeshResource : public zpResource {
public:
	zpStaticMeshResource();
	virtual ~zpStaticMeshResource();

	zp_uint getNumMeshParts() const;
	const zpStaticMeshPart& getMeshPart( zp_uint index ) const;
	
	void unload();

protected:
	zpArrayList<zpStaticMeshPart> m_meshParts;
};

#endif