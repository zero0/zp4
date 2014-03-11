#include "FontCompiler.h"

void compileBitmapFont( const zpString& inputFile, const zpString& outputFile )
{
	zpString lineStr, name;
	zpStringBuffer line;
	zpArrayList< zpString > parts, subParts, arrayParts;
	zpJson outputJson;

	zpFile file( inputFile );
	if( file.open( ZP_FILE_MODE_ASCII_READ ) )
	{
		while( !file.isEOF() )
		{
			file.readLine( line );
			if( line.isEmpty() ) continue;

			lineStr = line.str();
			lineStr.split( ' ', parts );
			if( parts.isEmpty() ) continue;

			const zp_char* type = parts[0].str();

			if( zp_strcmp( type, "info" ) == 0 )
			{
				for( zp_int i = 1; parts.size(); ++i )
				{
					parts[i].split( '=', subParts );

					subParts[0].toLower( name );
					const zpString& value = subParts[1];

					// string
					if( value.startsWith( '"' ) && value.endsWith( '"' ) )
					{
						zpString v;
						value.substring( v, 1, -1 );
						outputJson[ name ] = zpJson( v );
					}
					// array
					else if( value.indexOf( ',' ) != -1 )
					{
						value.split( ',', arrayParts );
						zpJson arr;
						arrayParts.foreachIndexed( [ &arr ]( zp_uint index, const zpString& value ) {
							zp_int v = 0;
							v = zp_atoi( value.str() );
							arr[ index ] = zpJson( v );
						} );
						outputJson[ name ] = arr;
					}
					// number
					else
					{
						zp_int v = 0;
						v = zp_atoi( value.str() );
						outputJson[ name ] = zpJson( v );
					}
				}
			}
			else if( zp_strcmp( type, "common" ) == 0 )
			{
				for( zp_int i = 1; parts.size(); ++i )
				{
					parts[i].split( '=', subParts );

					subParts[0].toLower( name );
					const zpString& value = subParts[1];

					zp_int v = 0;
					v = zp_atoi( value.str() );
					outputJson[ name ] = zpJson( v );
				}
			}
			else if( zp_strcmp( type, "page" ) == 0 )
			{
				zpJson& pages = outputJson[ "pages" ];

				zp_int pageId = 0;
				for( zp_int i = 1; parts.size(); ++i )
				{
					parts[i].split( '=', subParts );

					const zpString& id = subParts[0];
					const zpString& value = subParts[1];

					if( zp_strcmp( id.str(), "id" ) == 0 )
					{
						pageId = zp_atoi( value.str() );
					}
					else if( zp_strcmp( id.str(), "page" ) == 0 )
					{
						zpString v;
						value.substring( v, 1, -1 );

						pages[ pageId ] = zpJson( v );
					}
				}
			}
			//else if( zp_strcmp( type, "chars" ) == 0 )
			//{
			//	parts[1].split( '=', subParts );
			//
			//	const zpString& value = subParts[1];
			//
			//	zp_int v = 0;
			//	v = zp_atoi( value.str() );
			//	json[ name ] = zpJson( v );
			//}
			else if( zp_strcmp( type, "char" ) == 0 )
			{
				zpJson& chars = outputJson[ "chars" ];
				zpJson& c = chars[ chars.size() ];
				for( zp_int i = 1; parts.size(); ++i )
				{
					parts[i].split( '=', subParts );

					subParts[0].toLower( name );
					const zpString& value = subParts[1];

					zp_int v = 0;
					v = zp_atoi( value.str() );
					c[ name ] = zpJson( v );
				}
			}
			//else if( zp_strcmp( type, "kernings" ) == 0 )
			//{
			//
			//}
			else if( zp_strcmp( type, "kerning" ) == 0 )
			{
				zpJson& kernings = outputJson[ "kernings" ];
				zpJson& k = kernings[ kernings.size() ];
				for( zp_int i = 1; parts.size(); ++i )
				{
					parts[i].split( '=', subParts );

					subParts[0].toLower( name );
					const zpString& value = subParts[1];

					zp_int v = 0;
					v = zp_atoi( value.str() );
					k[ name ] = zpJson( v );
				}
			}
		}

		if( zpBison::compileToFile( outputFile, outputJson ) )
		{
			zpLog::message() << "Successfully compiled '" << outputFile << "'" << zpLog::endl;
		}
		else
		{
			zpLog::error() << "Unable to compile output file '" << outputFile << "'" << zpLog::endl;
		}
	}
}

void compileTrueTypeFont( const zpString& inputFile, const zpString& outputFile )
{

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
			<< "Usage: FontCompiler.exe "
			<< zpLog::gray << "path/to/inputfile.fnt path/to/outputfile.fontb"
			<< zpLog::endl
			;
	}
	else
	{
		const zpString& inputFile = arguments[ 0 ];
		const zpString& outputFile = arguments[ 1 ];

		if( inputFile.endsWith( ".fnt" ) )
		{
			compileBitmapFont( inputFile, outputFile );
		}
		else if( inputFile.endsWith( ".ttf" ) )
		{
			compileTrueTypeFont( inputFile, outputFile );
		}
	}

	return 0;
}