#include "Main.h"

BaseMeshCompiler::BaseMeshCompiler()
{
}
BaseMeshCompiler::~BaseMeshCompiler()
{
}

zp_bool BaseMeshCompiler::initialize( const zpArrayList< zpString >& args )
{
	m_inputFile = args[ args.size() - 2 ];
	m_outputFile = args[ args.size() - 1 ];

	return true;
}
zp_bool BaseMeshCompiler::compile()
{
	zp_bool ok;

	ok = compileMesh();

	return ok;
}
void BaseMeshCompiler::shutdown()
{
	zpJson data;
	data[ "Format" ] = zpJson( m_data.format );
	data[ "Vertex" ] = zpJson( m_data.vertex );
	data[ "Index" ] = zpJson( m_data.index );
	
	zpJson& parts = data[ "Parts" ];
	for( zp_uint i = 0; i < m_data.parts.size(); ++i )
	{
		MeshDataPart& mp = m_data.parts[ i ];
		zpJson& p = parts[ i ];
		p[ "Material" ] = zpJson( mp.material );
		p[ "Start" ] = zpJson( mp.startIndex );
		p[ "Count" ] = zpJson( mp.count );
	}

	if( !zpBison::compileToFile( m_outputFile, data ) )
	{
		zpLog::error() << "Unable to compile output file '" << m_outputFile << "'" << zpLog::endl;
	}
}

/************************************************************************/
/* 
{
	"Format" : "VNU",
	"Vertex" : <>,
	"Index" : <>,
	"Parts" :
	[
		{
			"Material" : "",
			"StartIndex" : 0,
			"Count" : 10,
		},
		...
	]
}
*/
/************************************************************************/
