#include "Main.h"

BaseShaderCompiler::BaseShaderCompiler()
	: m_optimizationLevel( -1 )
{}
BaseShaderCompiler::~BaseShaderCompiler()
{}

zp_bool BaseShaderCompiler::initialize( const zpArrayList< zpString >& args )
{
	// process args for optional params
	if( args.size() > 3 )
	{
		args.foreach( [ this ]( const zpString& arg )
		{
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
			else if( arg.startsWith( "-M" ) )
			{
				zpString macro( arg.substring( 2 ) );
				zpFixedArrayList< zpString, 2 > parts;
				macro.split( '=', parts );

				m_macros.put( parts[ 0 ], parts[ 1 ] );
			}
		} );
	}

	// use last two args as input and output files
	m_inputFile = args[ args.size() - 2 ];
	m_outputFile = args[ args.size() - 1 ];

	zpFile::convertToFilePath( m_inputFile );
	zpFile::convertToFilePath( m_outputFile );

	zp_bool success = false;

	zpFile input( m_inputFile );
	if( input.open( ZP_FILE_MODE_ASCII_READ ) )
	{
		success = true;
		input.readFile( m_shaderText );
		input.close();

		parsePragmas();
	}

	//zpJsonParser parser;
	//
	//// parse input file and read in shader file text
	//if( parser.parseFile( m_inputFile, m_shaderDesc ) )
	//{
	//	const zp_char* filenameStr = m_shaderDesc[ "File" ].asCString();
	//
	//	zp_int lastSlash = m_inputFile.lastIndexOf( zpFile::sep );
	//	
	//	zpString filename = m_inputFile.substring( 0, lastSlash + 1 );
	//	filename.append( filenameStr );
	//
	//	zpFile shaderFile( filename );
	//	if( shaderFile.open( ZP_FILE_MODE_ASCII_READ ) )
	//	{
	//		shaderFile.readFile( m_shaderText );
	//		shaderFile.close();
	//
	//		success = true;
	//
	//		prepareShaderMacros( m_shaderDesc[ "Macros" ], m_globalMacros );
	//	}
	//	else
	//	{
	//		zpLog::error() << "Failed to read file '" << filename << "'" << zpLog::endl;
	//	}
	//}
	//else
	//{
	//	zpLog::error() << "Failed to parse file '" << m_inputFile << "'" << zpLog::endl;
	//}

	return success;
}

void BaseShaderCompiler::compile()
{
	compileShaderPS();
	compileShaderVS();
}

void BaseShaderCompiler::shutdown()
{
	if( !m_compiledShaderDesc.isEmpty() )
	{
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

	zpDataBuffer data;
	compileShaderVSPlatform( data );

	m_macros.erase( key );

	zpJson& vs = m_compiledShaderDesc[ "VS" ];
	vs[ "Shader" ] = zpJson( data );
	vs[ "Format" ] = zpJson( m_pragmas[ zpString( PRAGMA_FORMAT ) ] );

	//const zpJson& vs = m_shaderDesc[ "VS" ];
	//if( vs.isObject() && !vs.isEmpty() )
	//{
	//	// copy global macros
	//	ShaderMacros localMacros = m_globalMacros;
	//	prepareShaderMacros( vs[ "Macros" ], localMacros );
	//
	//	// add shader specific flags
	//	ShaderMacro& macro = localMacros.pushBackEmpty();
	//	macro.first() = SHADER_COMPILE_VS;
	//	macro.second() = "1";
	//
	//	// compile platform shader
	//	zpDataBuffer data;
	//	if( compileShaderVSPlatform( vs, localMacros, data ) )
	//	{
	//		zpJson& cvs = m_compiledShaderDesc[ "VS" ];
	//		cvs[ "Shader" ] = zpJson( data );
	//		cvs[ "Format" ] = vs[ "Format" ];
	//	}
	//}
}

void BaseShaderCompiler::compileShaderPS()
{
	zpString key( SHADER_COMPILE_PS );
	m_macros.put( key, zpString( "1" ) );

	zpDataBuffer data;
	compileShaderPSPlatform( data );

	m_macros.erase( key );

	m_compiledShaderDesc[ "PS" ][ "Shader" ] = zpJson( data );
	//const zpJson& ps = m_shaderDesc[ "PS" ];
	//if( ps.isObject() && !ps.isEmpty() )
	//{
	//	// copy global macros
	//	ShaderMacros localMacros = m_globalMacros;
	//	prepareShaderMacros( ps[ "Macros" ], localMacros );
	//
	//	// add shader specific flags
	//	ShaderMacro& macro = localMacros.pushBackEmpty();
	//	macro.first() = SHADER_COMPILE_PS;
	//	macro.second() = "1";
	//
	//	// compile platform shader
	//	zpDataBuffer data;
	//	if( compileShaderPSPlatform( ps, localMacros, data ) )
	//	{
	//		zpJson& ps = m_compiledShaderDesc[ "PS" ];
	//		ps[ "Shader" ] = zpJson( data );
	//	}
	//}
}

void BaseShaderCompiler::parsePragmas()
{
	zpString allText( m_shaderText.str(), m_shaderText.size() );
	zpArrayList< zpString > lines;
	zpArrayList< zpString > parts;

	allText.split( '\n', lines );

	lines.foreach( [ this, &parts ]( const zpString& line )
	{
		if( line.startsWith( "#pragma " ) )
		{
			line.split( ' ', parts );
			if( parts.size() > 2 )
			{
				m_pragmas.put( parts[ 1 ], parts[ 2 ] );
			}
		}
	} );
}
