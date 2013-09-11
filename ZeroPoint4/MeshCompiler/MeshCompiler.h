#ifndef MESH_COMPILER_H
#define MESH_COMPILER_H

struct MeshDataPart
{
	zpString material;
	zp_int startIndex;
	zp_int count;
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

protected:
	virtual zp_bool compileMesh() = 0;

protected:
	zpString m_inputFile;
	zpString m_outputFile;

	MeshData m_data;
};

#endif
