#include "Main.h"

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
			<< zpLog::dark_gray << "[-O#|-Mkey=value|-Ipath/to/sys/include] "
			<< zpLog::gray << "path/to/inputfile.shader path/to/outputfile.shaderb"
			<< zpLog::endl
			<< "    -O# - Optimization Level  [dD]012345" << zpLog::endl
			<< "    -Ipath/to/sys/include - System Include Dir" << zpLog::endl
			<< "    -Mkey=value - key = value Macro"
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
			compiler->initialize( arguments );
			if( compiler->readInputFile() )
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
