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
	
	zpDataBuffer boundingData;
	boundingData.reserve( sizeof( zpBoundingAABB ) );

	zpJson& parts = data[ "Parts" ];
	for( zp_uint i = 0; i < m_data.parts.size(); ++i )
	{
		MeshDataPart& mp = m_data.parts[ i ];

		zpJson& p = parts[ i ];
		p[ "Material" ] = zpJson( mp.material );
		p[ "IndexOffset" ] = zpJson( mp.indexOffset );
		p[ "IndexCount" ] = zpJson( mp.indexCount );
		p[ "VertexOffset" ] = zpJson( mp.vertexOffset );
		p[ "VertexCount" ] = zpJson( mp.vertexCount );

		boundingData.reset();
		boundingData.write( mp.boundingBox );
		p[ "BoundingBox" ] = zpJson( boundingData );
	}

	if( !zpBison::compileToFile( m_outputFile, data ) )
	{
		zpLog::error() << "Unable to compile output file '" << m_outputFile << "'" << zpLog::endl;
	}
	else
	{
		zpLog::message() << "Successfully compiled '" << m_outputFile << "'" << zpLog::endl;
	}
}

void BaseMeshCompiler::formatToString( VertexFormat fmt, zpString& outFormat ) const
{
	switch( fmt )
	{
	case VF_VERTEX:
		outFormat = "V";
		break;
	case VF_VERTEX_COLOR:
		outFormat = "VC";
		break;
	case VF_VERTEX_NORMAL:
		outFormat = "VN";
		break;
	case VF_VERTEX_TEXTURE:
		outFormat = "VU";
		break;
	case VF_VERTEX_NORMAL_TEXTURE:
		outFormat = "VNU";
		break;
	default:
		ZP_ASSERT( false, "Unknown format" );
		break;
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
