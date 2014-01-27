#include "Main.h"

zp_int main( zp_int argCount, const zp_char* args[] )
{
	zpArrayList< zpString > arguments;
	if( argCount > 1 )
	{
		arguments.reserve( argCount - 1 );
		for( zp_int i = 1; i < argCount; ++i )
		{
			arguments.pushBackEmpty() = args[ i ];
		}
	}

	if( arguments.size() < 2 )
	{
		zpLog::message()
			<< "Usage: MeshCompiler.exe " << zpLog::blue << "path/to/inputfile " << zpLog::green << "path/to/outputfile" << zpLog::endl
			;
	}
	else
	{
		const zpString& inputFilename = arguments[ arguments.size() - 2 ];

		BaseMeshCompiler* compiler = ZP_NULL;
		if( inputFilename.endsWith( ".obj" ) )
		{
			compiler = new ObjMessCompiler;
		}
		else if( inputFilename.endsWith( ".fbx" ) )
		{
			compiler = new FbxMessCompiler;
		}

		if( compiler )
		{
			compiler->initialize( arguments );
			compiler->compile();
			compiler->shutdown();
		}
		else
		{
			zpLog::error() << "Unable to compile '" << inputFilename << "'. No compiler found for file type." << zpLog::endl;
		}

		ZP_SAFE_DELETE( compiler );
	}

	return 0;
}
