#include "Main.h"

zp_int main( zp_int argCount, const zp_char* args[] )
{
	zpMemorySystem* mem = zpMemorySystem::getInstance();
	mem->initialize( ZP_MEMORY_MB( 10 ) );

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
				compiler = new FbxMeshCompiler;
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
		zpConsole::getInstance()->destroy();
	}

	mem->shutdown();
	mem = ZP_NULL;

	return 0;
}
