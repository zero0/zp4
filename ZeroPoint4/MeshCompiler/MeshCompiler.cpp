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
	compileMeshToFile();

	compileSkeletonToFile();

	compileAnimationToFile();
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

void BaseMeshCompiler::compileMeshToFile()
{
	zpJson data;
	data[ "Format" ] = zpJson( m_mesh.format );
	data[ "Vertex" ] = zpJson( m_mesh.vertex );
	data[ "Index" ] = zpJson( m_mesh.index );

	zpDataBuffer boundingData;
	boundingData.reserve( sizeof( zpBoundingAABB ) );

	zpJson& parts = data[ "Parts" ];
	for( zp_uint i = 0; i < m_mesh.parts.size(); ++i )
	{
		MeshDataPart& mp = m_mesh.parts[ i ];

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
void BaseMeshCompiler::compileSkeletonToFile()
{
	if( !m_skeleton.bones.isEmpty() )
	{
		zp_int idx = m_outputFile.lastIndexOf( '.' ) + 1;
		zpString skeletonOutputFile = m_outputFile.substring( 0, idx );
		skeletonOutputFile.append( "skeletonb" );
	
		zpJson data;
		zpJson& bones = data[ "Bones" ];

		ZP_ALIGN16 zp_float matrix[16];
		zpDataBuffer dataBuffer;

		for( zp_int i = 0, imax = m_skeleton.bones.size(); i < imax; ++i )
		{
			const MeshSkeletonBone& meshBone = m_skeleton.bones[ i ];
			zpJson& b = bones[ meshBone.name ];

			zpMath::MatrixStoreOpenGL( meshBone.bindPose, matrix );

			dataBuffer.reset();
			dataBuffer.writeBulk( matrix, 16 );

			b[ "BindPose" ] = zpJson( dataBuffer );

			if( !meshBone.controlPointIndicies.isEmpty() )
			{
				dataBuffer.reset();
				dataBuffer.writeBulk( meshBone.controlPointIndicies.begin(), meshBone.controlPointIndicies.size() );

				b[ "Indices" ] = zpJson( dataBuffer );
			}

			if( !meshBone.controlPointWeights.isEmpty() )
			{
				dataBuffer.reset();
				dataBuffer.writeBulk( meshBone.controlPointWeights.begin(), meshBone.controlPointWeights.size() );

				b[ "Weights" ] = zpJson( dataBuffer );
			}
		}

		//zpJsonWriter writer;
		//const zp_char* jsonText = writer.styleWrite( data );

		if( !zpBison::compileToFile( skeletonOutputFile, data ) )
		{
			zpLog::error() << "Unable to compile output file '" << skeletonOutputFile << "'" << zpLog::endl;
		}
		else
		{
			zpLog::message() << "Successfully compiled '" << skeletonOutputFile << "'" << zpLog::endl;
		}
	}
}
void BaseMeshCompiler::compileAnimationToFile()
{

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
