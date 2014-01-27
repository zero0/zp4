#ifndef FBX_MESH_COMPILER_H
#define FBX_MESH_COMPILER_H

class BaseMeshCompiler;

struct FbxMeshDataPart
{
	zpString material;
	zp_int indexOffset;
	zp_int indexCount;
	zp_int vertexOffset;
	zp_int vertexCount;
	zpBoundingAABB boundingBox;
};

struct FbxMeshData
{
	zpArrayList< zpVector4f > verts;
	zpArrayList< zpVector4f > normals;
	zpArrayList< zpVector4f > tangents;
	zpArrayList< zpVector4f > binormals;
	zpArrayList< zpVector2f > uvs;
	zpArrayList< zpColor4f > colors;
	zpArrayList< zp_int > indecies;

	zpArrayList< FbxMeshDataPart > parts;
};

class FbxMessCompiler : public BaseMeshCompiler
{
public:
	virtual ~FbxMessCompiler();

protected:
	zp_bool compileMesh();

private:
	FbxMeshData m_fbxData;
};

#endif
