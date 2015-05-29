#ifndef MESH_COMPILER_H
#define MESH_COMPILER_H

enum VertexFormat : zp_int
{
	VF_NONE =						0,
	VF_VERTEX =						1 << 0,
	VF_COLOR =						1 << 1,
	VF_NORMAL =						1 << 2,
	VF_TEXTURE =					1 << 3,



	VF_VERTEX_COLOR =				VF_VERTEX | VF_COLOR,
	VF_VERTEX_NORMAL =				VF_VERTEX | VF_NORMAL,
	VF_VERTEX_TEXTURE =				VF_VERTEX | VF_TEXTURE,
	VF_VERTEX_NORMAL_TEXTURE =		VF_VERTEX | VF_NORMAL | VF_TEXTURE,
};

enum MeshOutputFiles
{
	ZP_MESH_OUTPUT_MESH =		1 << 0,
	ZP_MESH_OUTPUT_SKELETON =	1 << 1,
	ZP_MESH_OUTPUT_ANIMATION =	1 << 2,
};

struct MaterialData
{

};

struct MeshDataPart
{
	zpString material;
	MaterialData materialData;
	zp_int indexOffset;
	zp_int indexCount;
	zp_int vertexOffset;
	zp_int vertexCount;
	zpBoundingAABB boundingBox;
};

struct MeshData
{
	zpString format;
	zpDataBuffer vertex;
	zpDataBuffer index;
	zpArrayList< MeshDataPart > parts;
};

struct MeshSkeletonBone
{
	zp_int parent;
	zp_int name;
	zpArrayList< zp_int > controlPointIndicies;
	zpArrayList< zp_float > controlPointWeights;
	zpMatrix4f bindPose;
};

struct MeshSkeleton
{
	zpArrayList< zpString > boneNames;
	zpArrayList< MeshSkeletonBone > bones;
};

struct MeshBoneAnimation
{
	zpString boneName;
	zpArrayList< zpMatrix4f > keyFrames;
};

struct MeshAnimationClip
{
	zp_float frameRate;
	zpString name;
	zpArrayList< MeshBoneAnimation > boneFrames;
};

struct MeshAnimation
{
	zpArrayList< MeshAnimationClip > clips;
};

class BaseMeshCompiler
{
public:
	BaseMeshCompiler();
	virtual ~BaseMeshCompiler();

	zp_bool initialize( const zpArrayList< zpString >& args );
	zp_bool compile();
	void shutdown();

	void formatToString( VertexFormat fmt, zpString& outFormat ) const;

protected:
	virtual zp_bool compileMesh() = 0;

	void compileMeshToFile();
	void compileSkeletonToFile();
	void compileAnimationToFile();

protected:
	zpString m_inputFile;
	zpString m_outputFile;

	MeshData m_mesh;
	MeshSkeleton m_skeleton;
	MeshAnimation m_animation;
};

#endif
