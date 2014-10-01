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

protected:
	zpString m_inputFile;
	zpString m_outputFile;

	MeshData m_data;
};

#endif
