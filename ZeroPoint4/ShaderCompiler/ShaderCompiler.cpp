#include "ShaderCompiler.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>

#define SHADER_DX9			"DX9"
#define SHADER_DX10			"DX10"
#define SHADER_DX11			"DX11"
#define SHADER_GL2			"GL2"

#define SHADER_COMPILE_VS	"COMPILE_VS"
#define SHADER_COMPILE_PS	"COMPILE_PS"
#define SHADER_COMPILE_GS	"COMPILE_GS"
#define SHADER_COMPILE_CS	"COMPILE_CS"

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
		} );
	}

	// use last two args as input and output files
	m_inputFile = args[ args.size() - 2 ];
	m_outputFile = args[ args.size() - 1 ];

	zpFile::convertToFilePath( m_inputFile );
	zpFile::convertToFilePath( m_outputFile );

	zpJsonParser parser;
	zp_bool success = false;

	// parse input file and read in shader file text
	if( parser.parseFile( m_inputFile, m_shaderDesc ) )
	{
		const zp_char* filenameStr = m_shaderDesc[ "File" ].asCString();

		zp_int lastSlash = m_inputFile.lastIndexOf( zpFile::sep );
		
		zpString filename = m_inputFile.substring( 0, lastSlash + 1 );
		filename.append( filenameStr );

		zpFile shaderFile( filename );
		if( shaderFile.open( ZP_FILE_MODE_ASCII_READ ) )
		{
			shaderFile.readFile( m_shaderText );
			shaderFile.close();

			success = true;

			prepareShaderMacros( m_shaderDesc[ "Macros" ], m_globalMacros );
		}
		else
		{
			zpLog::error() << "Failed to read file '" << filename << "'" << zpLog::endl;
		}
	}
	else
	{
		zpLog::error() << "Failed to parse file '" << m_inputFile << "'" << zpLog::endl;
	}

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

void BaseShaderCompiler::prepareShaderMacros( const zpJson& desc, ShaderMacros& macros ) const
{
	if( desc.isObject() && !desc.isEmpty() )
	{
		desc.foreachObject( [ &macros ]( const zpString& key, const zpJson& value )
		{
			ShaderMacro& macro = macros.pushBackEmpty();
			macro.first() = key.str();
			macro.second() = value.asCString();
		} );
	}
}

void BaseShaderCompiler::compileShaderVS()
{
	const zpJson& vs = m_shaderDesc[ "VS" ];
	if( vs.isObject() && !vs.isEmpty() )
	{
		// copy global macros
		ShaderMacros localMacros = m_globalMacros;
		prepareShaderMacros( vs[ "Macros" ], localMacros );

		// add shader specific flags
		ShaderMacro& macro = localMacros.pushBackEmpty();
		macro.first() = SHADER_COMPILE_VS;
		macro.second() = "1";

		// compile platform shader
		zpDataBuffer data;
		if( compileShaderVSPlatform( vs, localMacros, data ) )
		{
			zpJson& cvs = m_compiledShaderDesc[ "VS" ];
			cvs[ "Shader" ] = zpJson( data );
			cvs[ "Format" ] = vs[ "Format" ];
		}
	}
}

void BaseShaderCompiler::compileShaderPS()
{
	const zpJson& ps = m_shaderDesc[ "PS" ];
	if( ps.isObject() && !ps.isEmpty() )
	{
		// copy global macros
		ShaderMacros localMacros = m_globalMacros;
		prepareShaderMacros( ps[ "Macros" ], localMacros );

		// add shader specific flags
		ShaderMacro& macro = localMacros.pushBackEmpty();
		macro.first() = SHADER_COMPILE_PS;
		macro.second() = "1";

		// compile platform shader
		zpDataBuffer data;
		if( compileShaderPSPlatform( ps, localMacros, data ) )
		{
			zpJson& ps = m_compiledShaderDesc[ "PS" ];
			ps[ "Shader" ] = zpJson( data );
		}
	}
}


DXShaderCompiler::DXShaderCompiler( DXShaderCompilerPlatform platform )
	: m_platform( platform )
{}
DXShaderCompiler::~DXShaderCompiler()
{}

zp_bool DXShaderCompiler::compileShaderVSPlatform( const zpJson& vs, const ShaderMacros& vsMacros, zpDataBuffer& data ) const
{
	const zp_char* profile;
	switch( m_platform )
	{
	case DX_SHADER_COMPILER_PLATFORM_DX9:
		profile = "vs_3_0";
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX10:
		profile = "vs_4_0";
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX11:
		profile = "vs_5_0";
		break;
	}

	return compileShaderPlatform( vs, profile, vsMacros, data );
}
zp_bool DXShaderCompiler::compileShaderPSPlatform( const zpJson& ps, const ShaderMacros& vsMacros, zpDataBuffer& data ) const
{
	const zp_char* profile;
	switch( m_platform )
	{
	case DX_SHADER_COMPILER_PLATFORM_DX9:
		profile = "ps_3_0";
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX10:
		profile = "ps_4_0";
		break;
	case DX_SHADER_COMPILER_PLATFORM_DX11:
		profile = "ps_5_0";
		break;
	}

	return compileShaderPlatform( ps, profile, vsMacros, data );
}

zp_bool DXShaderCompiler::compileShaderPlatform( const zpJson& shader, const zp_char* profile, const ShaderMacros& shaderMacros, zpDataBuffer& data ) const
{
	// get the main function name
	const zp_char* mainFunc = shader[ "Main" ].asCString();

	// get the shader text and size
	const zp_char* shaderText = m_shaderText.str();
	zp_uint shaderTextSize = m_shaderText.length();

	// convert macros to DX macros, adding an empty one to for the end
	zpArrayList< D3D10_SHADER_MACRO > macros;
	if( !shaderMacros.isEmpty() )
	{
		macros.reserve( macros.size() + 1 );
		shaderMacros.foreach( [ &macros ]( const ShaderMacro& m )
		{
			D3D10_SHADER_MACRO& shaderMacro = macros.pushBackEmpty();
			shaderMacro.Name = m.first();
			shaderMacro.Definition = m.second();
		} );
		macros.pushBackEmpty();
	}

	// process flags
	zp_uint flags = 0;
	switch( m_optimizationLevel )
	{
	case -1: flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION; break;
	case 0:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0; break;
	case 1:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL1; break;
	case 2:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL2; break;
	case 3:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3; break;
	case 4:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_WARNINGS_ARE_ERRORS; break;
	case 5:  flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS; break;
	default: flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0; break;
	}

	// compile shader
	HRESULT hr;
	ID3D10Blob* shaderBlob = ZP_NULL;
	ID3D10Blob* errors = ZP_NULL;
	hr = D3DX11CompileFromMemory( shaderText, shaderTextSize, m_inputFile.str(), macros.begin(), ZP_NULL, mainFunc, profile, flags, 0, ZP_NULL, &shaderBlob, &errors, ZP_NULL );

	// print any errors
	if( errors )
	{
		const zp_char* errorText = (const zp_char*)errors->GetBufferPointer();
		zpLog::warning() << errorText << zpLog::endl;
	}

	// write the shader data into the out buffer
	if( shaderBlob )
	{
		data.reset();
		data.writeBulk( (const zp_byte*)shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize() );
	}

	// release blobs
	ZP_SAFE_RELEASE( shaderBlob );
	ZP_SAFE_RELEASE( errors );

	// return if compiled successfully
	return SUCCEEDED( hr );
}

zp_int main( zp_int argCount, const zp_char* args[] )
{
	zpArrayList< zpString > arguments;
	if( argCount > 1 )
	{
		zp_int c = argCount - 1;
		arguments.reserve( c );
		for( zp_int i = 0; i < c; ++i )
		{
			arguments.pushBackEmpty() = args[ i + 1 ];
		}
	}

	if( arguments.size() < 3 )
	{
		zpLog::message()
			<< "Usage: ShaderCompiler.exe "
			<< zpLog::gray << "DX9/DX10/DX11/GL2 "
			<< zpLog::dark_gray << "[-O#] "
			<< zpLog::gray << "path/to/inputfile.shader path/to/outputfile.shaderb"
			<< zpLog::endl
			<< "    -O# - Optimization Level  [dD]012345"
			;
	}
	else
	{
		BaseShaderCompiler* compiler = ZP_NULL;

		const zpString& type = arguments[ 0 ];

		if( type == SHADER_DX9 )
		{
			compiler = new DXShaderCompiler( DXShaderCompiler::DX_SHADER_COMPILER_PLATFORM_DX9 );
		}
		else if( type == SHADER_DX10 )
		{
			compiler = new DXShaderCompiler( DXShaderCompiler::DX_SHADER_COMPILER_PLATFORM_DX10 );
		}
		else if( type == SHADER_DX11 )
		{
			compiler = new DXShaderCompiler( DXShaderCompiler::DX_SHADER_COMPILER_PLATFORM_DX11 );
		}
		else if( type == SHADER_GL2 )
		{
			
		}

		if( compiler != ZP_NULL )
		{
			if( compiler->initialize( arguments ) )
			{
				compiler->compile();
				compiler->shutdown();
			}
			else
			{
				zpLog::error() << "Failed to initialize compiler." << zpLog::endl;
			}
		}
		else
		{
			zpLog::error() << "Shader type '" << type << "' undefined." << zpLog::endl;
		}

		ZP_SAFE_DELETE( compiler );
	}

	return 0;
}