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
	zpString parent;
	zpString name;
	zpArrayList< zp_int > controlPointIndices;
	zpArrayList< zp_float > controlPointWeights;
	zpMatrix4f bindPose;
};

struct zpFbxSkeleton
{
	zpString rootBone;
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

struct zpFbxAnimationKeyFrame
{
	zpVector4f position;
	zpQuaternion4f rotataion;
	zpVector4f scale;
};

struct zpFbxAnimationKeyFrames
{
	zpString boneName;
	zpArrayList< zpFbxAnimationKeyFrame > frames;
};

struct zpFbxAnimation
{
	zp_float frameRate;
	zpString name;
	zpArrayList< zpFbxAnimationKeyFrames > boneFrames;
};

struct zpFbxMeshData
{
	zpArrayList< zpFbxMeshDataPart > parts;
	zpFbxSkeleton skeleton;
	zpArrayList< zpFbxAnimation > animations;
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
