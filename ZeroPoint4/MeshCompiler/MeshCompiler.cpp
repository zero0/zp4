#include "Main.h"

#define MESH_COMPILER_OUTPUT_JSON	1

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

#if MESH_COMPILER_OUTPUT_JSON
	zpString jsonFileName( m_outputFile );
	jsonFileName.append( ".txt" );

	zpJsonWriter writer;
	const zp_char* jsonText = writer.styleWrite( data );

	zpFile jsonFile( jsonFileName );
	if( jsonFile.open( ZP_FILE_MODE_ASCII_TRUNCATE_WRITE ) )
	{
		zpStringBuffer buff;
		buff.append( jsonText );

		jsonFile.writeBuffer( buff );
		jsonFile.close();
	}
#endif

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
		zpJson& boneNames = data[ "BoneNames" ];
		zpJson& bones = data[ "Bones" ];

		ZP_ALIGN16 zp_float matrix[16];
		zpDataBuffer dataBuffer;

		// store bone names
		for( zp_int i = 0, imax = m_skeleton.boneNames.size(); i < imax; ++i )
		{
			boneNames[ i ] = zpJson( m_skeleton.boneNames[ i ] );
		}

		// store bone data
		for( zp_int i = 0, imax = m_skeleton.bones.size(); i < imax; ++i )
		{
			const MeshSkeletonBone& meshBone = m_skeleton.bones[ i ];
			zpJson& b = bones[ meshBone.name ];

			zpMath::MatrixStoreOpenGL( meshBone.bindPose, matrix );

			dataBuffer.reset();
			dataBuffer.writeBulk( matrix, 16 );

			b[ "Parent" ] = zpJson( meshBone.parent );
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

#if MESH_COMPILER_OUTPUT_JSON
		zpString jsonFileName( skeletonOutputFile );
		jsonFileName.append( ".txt" );

		zpJsonWriter writer;
		const zp_char* jsonText = writer.styleWrite( data );

		zpFile jsonFile( jsonFileName );
		if( jsonFile.open( ZP_FILE_MODE_ASCII_TRUNCATE_WRITE ) )
		{
			zpStringBuffer buff;
			buff.append( jsonText );

			jsonFile.writeBuffer( buff );
			jsonFile.close();
		}
#endif

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
	if( !m_animation.clips.isEmpty() )
	{
		zp_int idx = m_outputFile.lastIndexOf( '.' ) + 1;
		zpString animationOutputFile = m_outputFile.substring( 0, idx );
		animationOutputFile.append( "animb" );

		ZP_ALIGN16 zp_float matrix[16];
		zpDataBuffer dataBuffer;

		zpJson data;

		for( zp_uint i = 0; i < m_animation.clips.size(); ++i )
		{
			MeshAnimationClip& clip = m_animation.clips[ i ];
			zpJson& jsonClip = data[ clip.name ];
			
			jsonClip[ "FrameRate" ] = zpJson( clip.frameRate );

			zpJson& keyFrames = jsonClip[ "KeyFrames" ];
			for( zp_uint b = 0; b < clip.boneFrames.size(); ++b )
			{
				MeshBoneAnimation& boneAnim = clip.boneFrames[ b ];

				dataBuffer.reset();
				dataBuffer.reserve( boneAnim.keyFrames.size() * sizeof( matrix ) );

				for( zp_uint k = 0; k < boneAnim.keyFrames.size(); ++k )
				{
					zpMath::MatrixStoreOpenGL( boneAnim.keyFrames[ k ], matrix );
					dataBuffer.writeBulk( matrix, 16 );
				}

				keyFrames[ boneAnim.boneName ] = zpJson( dataBuffer );
			}
		}

#if MESH_COMPILER_OUTPUT_JSON
		zpString jsonFileName( animationOutputFile );
		jsonFileName.append( ".txt" );

		zpJsonWriter writer;
		const zp_char* jsonText = writer.styleWrite( data );

		zpFile jsonFile( jsonFileName );
		if( jsonFile.open( ZP_FILE_MODE_ASCII_TRUNCATE_WRITE ) )
		{
			zpStringBuffer buff;
			buff.append( jsonText );

			jsonFile.writeBuffer( buff );
			jsonFile.close();
		}
#endif

		if( !zpBison::compileToFile( animationOutputFile, data ) )
		{
			zpLog::error() << "Unable to compile output file '" << animationOutputFile << "'" << zpLog::endl;
		}
		else
		{
			zpLog::message() << "Successfully compiled '" << animationOutputFile << "'" << zpLog::endl;
		}
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
