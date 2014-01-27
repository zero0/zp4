#ifndef MESH_COMPILER_H
#define MESH_COMPILER_H

enum VertexFormat
{
	VF_NONE,
	VF_VERTEX,
	VF_VERTEX_COLOR,
	VF_VERTEX_NORMAL,
	VF_VERTEX_TEXUTRE,
	VF_VERTEX_NORMAL_TEXTURE,

};

struct MeshDataPart
{
	zpString material;
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
