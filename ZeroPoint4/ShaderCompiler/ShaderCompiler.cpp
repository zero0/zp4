#include "Main.h"

BaseShaderCompiler::BaseShaderCompiler()
	: m_optimizationLevel( -1 )
{}
BaseShaderCompiler::~BaseShaderCompiler()
{}

void BaseShaderCompiler::initialize( const zpArrayList< zpString >& args )
{
	// use last two args as input and output files
	m_inputFile = args[ args.size() - 2 ];
	m_outputFile = args[ args.size() - 1 ];

	zpFile::convertToFilePath( m_inputFile );
	zpFile::convertToFilePath( m_outputFile );

	// find local include dir from input file and by default use the local as the system include path
	m_localIncludeDir = m_inputFile.substring( 0, m_inputFile.lastIndexOf( zpFile::sep ) );
	m_systemIncludeDir = m_localIncludeDir;
	m_previewFile = "txt";
	// process args for optional params
	if( args.size() > 3 )
	{
		args.foreach( [ this ]( const zpString& arg )
		{
			// set optimization level
			if( arg.startsWith( "-O" ) )
			{
				zp_char level = arg.charAt( arg.length() - 1 );
				if( zp_is_digit( level ) )
				{
					m_optimizationLevel = level - '0';
				}
				else if( level == 'd' || level == 'D' )
				{
					m_optimizationLevel = -1;
				}
			}
			// add any external macros
			else if( arg.startsWith( "-M" ) )
			{
				zpFixedArrayList< zpString, 2 > parts;
				 arg.substring( 2 ).split( '=', parts );

				m_macros.put( parts[ 0 ], parts[ 1 ] );
			}
			// set system include dir
			else if( arg.startsWith( "-I" ) )
			{
				m_systemIncludeDir = arg.substring( 2 );
				zpFile::convertToFilePath( m_systemIncludeDir );
			}
			// set preview file
			else if( arg.startsWith( "-P" ) )
			{
				m_previewFile = arg.substring( 2 );
			}
		} );
	}

	initializePlatform();
}

zp_bool BaseShaderCompiler::readInputFile()
{
	zp_bool success = false;

	// read input file
	zpFile input( m_inputFile );
	if( input.open( ZP_FILE_MODE_ASCII_READ ) )
	{
		input.readFile( m_shaderText );
		input.close();

		parsePragmas();

		success = true;
	}

	return success;
}

void BaseShaderCompiler::compile()
{
	compileShaderVS();
	compileShaderPS();
	compileShaderGS();
	compileShaderCS();
}

void BaseShaderCompiler::shutdown()
{
	if( !m_compiledShaderDesc.isEmpty() )
	{
		if( !m_previewFile.isEmpty() )
		{
			zpJsonWriter writer;
			const zp_char* previewText = writer.styleWrite( m_compiledShaderDesc );

			zpStringBuffer previewFilename;
			previewFilename.append( m_outputFile );
			previewFilename.append( '.' );
			previewFilename.append( m_previewFile );

			zpFile previewFile( previewFilename.str() );
			if( previewFile.open( ZP_FILE_MODE_ASCII_TRUNCATE_WRITE ) )
			{
				previewFile.writeFormat( previewText );
				previewFile.close();
			}
		}

		if( !zpBison::compileToFile( m_outputFile, m_compiledShaderDesc ) )
		{
			zpLog::error() << "Unable to compile output file '" << m_outputFile << "'" << zpLog::endl;
		}
		else
		{
			zpLog::message() << "Successfully compiled '" << m_outputFile << "'" << zpLog::endl;
		}
	}
	else
	{
		zpLog::warning() << "Output JSON empty. Unable to compile." << zpLog::endl;
	}
}

void BaseShaderCompiler::compileShaderVS()
{
	zpString key( SHADER_COMPILE_VS );
	m_macros.put( key, zpString( "1" ) );

	zpShaderInfo info;
	compileShaderVSPlatform( info );

	m_macros.erase( key );

	zpJson& vs = m_compiledShaderDesc[ "VS" ];
	vs[ "Shader" ] = zpJson( info.data );
	vs[ "Format" ] = zpJson( m_pragmas[ zpString( PRAGMA_FORMAT ) ] );

	parseShaderInfo( vs, info );

	parseGlobalShaderInfo( m_compiledShaderDesc, info );
}

void BaseShaderCompiler::compileShaderPS()
{
	zpString key( SHADER_COMPILE_PS );
	m_macros.put( key, zpString( "1" ) );

	zpShaderInfo info;
	compileShaderPSPlatform( info );

	m_macros.erase( key );

	zpJson& ps = m_compiledShaderDesc[ "PS" ];
	ps[ "Shader" ] = zpJson( info.data );

	parseShaderInfo( ps, info );

	parseGlobalShaderInfo( m_compiledShaderDesc, info );

}

void BaseShaderCompiler::compileShaderGS()
{}

void BaseShaderCompiler::compileShaderCS()
{}

void BaseShaderCompiler::parsePragmas()
{
	zpString allText = m_shaderText.toString();
	zpArrayList< zpString > lines;
	zpFixedArrayList< zpString, 3 > parts;

	allText.split( '\n', lines );

	lines.foreach( [ this, &parts ]( const zpString& line )
	{
		if( line.startsWith( "#pragma " ) )
		{
			line.split( ' ', parts, 2 );
			if( parts.size() == 3 )
			{
				m_pragmas.put( parts[ 1 ], parts[ 2 ] );
			}
			else if( parts.size() == 2 )
			{
				m_pragmas.put( parts[ 1 ], zpString( "" ) );
			}
		}
	} );
}

void BaseShaderCompiler::parseShaderInfo( zpJson& shaderJson, zpShaderInfo& info )
{
	if( !info.shaderInput.textures.isEmpty() )
	{
		zpJson& textures = shaderJson[ "Textures" ];
		info.shaderInput.textures.foreachIndexed( [ &textures ]( zp_uint index, zpTextureShaderInput& t ) {
			zpJson& tex = textures[ index ];
			tex[ "Name" ] = zpJson( t.name );
			tex[ "Slot" ] = zpJson( t.index );
			tex[ "Type" ] = zpJson( t.dimension );
		} );
	}

	//if( !info.shaderInput.constantBuffers.isEmpty() )
	//{
	//	zpJson& constantBuffers = shaderJson[ "ConstantBuffers" ];
	//	info.shaderInput.constantBuffers.foreachIndexed( [ &constantBuffers ]( zp_uint index, zpConstantBufferShaderInput& c ) {
	//		zpJson& cb = constantBuffers[ index ];
	//		cb[ "Name" ] = zpJson( c.name );
	//		cb[ "Size" ] = zpJson( c.size );
	//		cb[ "Slot" ] = zpJson( c.slot );
	//	} );
	//}
}

void BaseShaderCompiler::parseGlobalShaderInfo( zpJson& shaderJson, zpShaderInfo& info )
{
	if( !info.shaderInput.globalVariables.isEmpty() )
	{
		zpJson& globals = shaderJson[ "Globals" ];
		if( globals.isEmpty() )
		{
			globals[ "Size" ] = zpJson( info.shaderInput.globalVariablesSize );
			zpJson& variables = globals[ "Variables" ];
			info.shaderInput.globalVariables.foreachIndexed( [ &variables ]( zp_uint index, zpGlobalVariableInput& g ) {
				zpJson& gv = variables[ index ];
				gv[ "Name" ] = zpJson( g.name );
				gv[ "Size" ] = zpJson( g.size );
				gv[ "Type" ] = zpJson( g.type );
				gv[ "Offset" ] = zpJson( g.offset );
			} );
		}
	}
}
