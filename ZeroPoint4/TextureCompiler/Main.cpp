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

	if( arguments.size() < 2 )
	{
		zpLog::message()
			<< "Usage: TextureCompiler.exe "
			<< zpLog::gray << "DX9/DX10/DX11/GL2 "
			<< zpLog::gray << "path/to/inputfile.png path/to/outputfile.textureb"
			<< zpLog::endl
			;
	}
	else
	{
		TextureCompiler compiler;

		const zpString& input = arguments[  arguments.size() - 2 ];

		if( compiler.initialize( arguments ) )
		{
			compiler.compile();
			compiler.shutdown();
		}
		else
		{
			zpLog::error() << "Failed to initialize compiler." << zpLog::endl;
		}
	}

	return 0;
}
