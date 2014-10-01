#ifndef FBX_MESH_COMPILER_H
#define FBX_MESH_COMPILER_H

class BaseMeshCompiler;

struct FbxMaterialData
{
	zpArrayList< zpString > materialNames;
	zpArrayList< zp_int > polygonIndexToMaterialName;
};

struct FbxMeshDataPart
{
	FbxMaterialData materialData;
	zpArrayList< zpVector4f > verts;
	zpArrayList< zp_int > vertIndecies;
	zpArrayList< zpVector4f > normals;
	zpArrayList< zpVector4f > tangents;
	zpArrayList< zpVector4f > binormals;
	zpArrayList< zpVector2f > uvs;
	zpArrayList< zpColor4f > colors;
};

struct FbxMeshData
{
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
