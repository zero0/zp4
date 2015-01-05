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
}

void BaseShaderCompiler::compileShaderPS()
{
	zpString key( SHADER_COMPILE_PS );
	m_macros.put( key, zpString( "1" ) );

	zpDataBuffer data;
	compileShaderPSPlatform( data );

	m_macros.erase( key );

	m_compiledShaderDesc[ "PS" ][ "Shader" ] = zpJson( data );
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
