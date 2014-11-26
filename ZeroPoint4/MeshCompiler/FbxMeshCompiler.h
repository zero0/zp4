#ifndef FBX_MESH_COMPILER_H
#define FBX_MESH_COMPILER_H

class BaseMeshCompiler;

struct zpFbxMaterialData
{
	zpArrayList< zpString > materialNames;
	zpArrayList< zp_int > polygonIndexToMaterialName;
};

struct zpFbxBone
{
	zpString name;
	zpArrayList< zp_int > controlPointIndices;
	zpArrayList< zp_float > controlPointWeights;
	zpMatrix4f bindPose;
};

struct zpFbxSkeleton
{
	zpArrayList< zpFbxBone > bones;
};

struct zpFbxMeshDataPart
{
	zpFbxMaterialData materialData;

	zpArrayList< zpVector4f > verts;
	zpArrayList< zp_int > vertIndecies;

	zpArrayList< zpVector2f > uvs;
	zpArrayList< zp_int > uvIndecies;

	zpArrayList< zpVector4f > normals;
	zpArrayList< zp_int > normIndecies;

	zpArrayList< zpVector4f > binormals;

	zpArrayList< zpVector4f > tangents;

	zpArrayList< zpColor4f > colors;


};

struct zpFbxAnimationFrame
{
	zp_float time;
	zpMatrix4f transform;
};

struct zpFbxAnimation
{
	zp_float startTime;
	zp_float endTime;
	zp_float fps;
	zpArrayList< zpFbxAnimationFrame > frames;
};

struct zpFbxMeshData
{
	zpArrayList< zpFbxMeshDataPart > parts;
	zpFbxSkeleton skeleton;
	zpFbxAnimation animation;
};

struct zpVertexNormalTexture
{
	zpVector4f v;
	zpVector4f n;
	zpVector2f t;
};


class FbxMeshCompiler : public BaseMeshCompiler
{
public:
	virtual ~FbxMeshCompiler();

	zp_bool importCallback( zp_float percent, const zp_char* status );

protected:
	zp_bool compileMesh();

private:
	zpFbxMeshData m_fbxData;
};

#endif
