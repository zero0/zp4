#include "ObjectCompiler.h"

#define RESERVED_EXTENDS	"_Extends"

void compileTransform( zpJson& inputObject )
{
	// process transform
	zpJson& transform = inputObject[ "Transform" ];
	if( transform.isObject() && !transform.isEmpty() )
	{
		zpMatrix4f outTransform;
		outTransform.setIdentity();

		zpJson& translate = transform[ "Position" ];
		zpJson& rotation = transform[ "Rotation" ];
		zpJson& scale = transform[ "Scale" ];

		if( !translate.isEmpty() )
		{
			zpVector4f pos( translate[ (zp_uint)0 ].asFloat(), translate[ (zp_uint)1 ].asFloat(), translate[ (zp_uint)2 ].asFloat(), 1.0f );
			outTransform.setRow( 3, pos );
		}
		if( !rotation.isEmpty() )
		{

		}
		if( !scale.isEmpty() )
		{
			zpMatrix4f mat;
			mat.setIdentity();

			zpVector4f rot;
			for( zp_uint i = 0; i < 3; ++i )
			{
				zpMath::Mul( rot, mat.getRow( i ), zpScalar( scale[ i ].asFloat() ) );
				mat.setRow( i, rot );
			}

			zpMath::Mul( outTransform, outTransform, mat );
		}

		transform = zpJson( &outTransform, sizeof( zpMatrix4f ) );
	}
}

zp_bool compileObject( zpJsonParser& parser, zpArrayList< zpString >& processedFiles, const zpString& srcFile, zpJson& outObject )
{
	zp_bool ok = false;
	zpJson inputObject;
	if( parser.parseFile( srcFile, inputObject ) )
	{
		ok = true;

		// copy out "extends" and remove
		zpJson extends = inputObject[ RESERVED_EXTENDS ];
		inputObject[ RESERVED_EXTENDS ] = zpJson::null;

		// process extends
		if( extends.isArray() && !extends.isEmpty() )
		{
			//extends.foreachArray( [ &ok, &parser, &processedFiles, &outObject ]( const zpJson& v )
			for( zp_uint i = 0, count = extends.size(); i < count; ++i )
			{
				const zpJson& v = extends[ i ];

				zpString extendsFile( v.asCString() );
				if( processedFiles.indexOf( extendsFile ) < 0 )
				{
					processedFiles.pushBack( extendsFile );
					ok = compileObject( parser, processedFiles, extendsFile, outObject );
					if( ok )
					{
						processedFiles.popBack();
					}
					else
					{
						zpLog::error() << "Failed to compile extends file '" << extendsFile << "'" << zpLog::endl;
						break;
					}
				}
				else
				{
					zpLog::fatal() << "Circular dependency found '" << srcFile << "' -> '" << extendsFile << "'" << zpLog::endl;
					ok = false;
					break;
				}
			};
		}

		// compile transform
		//compileTransform( inputObject );

		zpJson::mergeJson( outObject, inputObject );
	}

	return ok;
}

zp_bool compileWorld( zpJsonParser& parser, zpArrayList< zpString >& processedFiles, const zpString& srcFile, zpJson& outWorld )
{
	zp_bool ok = false;

	if( parser.parseFile( srcFile, outWorld ) )
	{
		ok = true;

		//zpJson& objects = outWorld[ "Objects" ];
		//objects.foreachArray( []( zpJson& value )
		//{
		//	compileTransform( value );
		//} );
	}

	return ok;
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

	if( arguments.size() < 2 )
	{
		zpLog::message()
			<< "Usage: ObjectCompiler.exe "
			<< zpLog::gray << "path/to/inputfile.object path/to/outputfile.objectb"
			<< zpLog::endl
			;
	}
	else
	{
		// use last two args as input and output files
		const zpString& inputFile = arguments[ arguments.size() - 2 ];
		const zpString& outputFile = arguments[ arguments.size() - 1 ];

		zp_bool ok = false;
		zpJson outputJson;
		zpJsonParser parser;
		zpArrayList< zpString > processedFiles;
		processedFiles.reserve( 5 );
		processedFiles.pushBack( inputFile );

		// compile object
		if( inputFile.endsWith( ".object" ) )
		{
			ok = compileObject( parser, processedFiles, inputFile, outputJson );
		}
		// compile world
		else if( inputFile.endsWith( ".world" ) )
		{
			ok = compileWorld( parser, processedFiles, inputFile, outputJson );
		}

		// if compilation ok
		if( ok )
		{
			if( zpBison::compileToFile( outputFile, outputJson ) )
			{
				zpLog::message() << "Successfully compiled '" << outputFile << "'" << zpLog::endl;
			}
			else
			{
				zpLog::error() << "Unable to compile output file '" << outputFile << "'" << zpLog::endl;
			}
		}
		// failed to compile
		else
		{
			zpLog::error() << "Failed to compile '" << outputFile << "'" << zpLog::endl;
		}
	}

	return 0;
}