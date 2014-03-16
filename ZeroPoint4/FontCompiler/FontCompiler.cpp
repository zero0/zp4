#include "FontCompiler.h"

zp_bool compileBitmapFontXML( const zpString& inputFile, const zpString& outputFile, zpJson& outputJson )
{
	zp_bool ok = false;
	zpXmlParser xmlParser;
	if( xmlParser.parseFile( inputFile, true ) )
	{
		ok = true;
		if( xmlParser.push( zpString( "font" ) ) )
		{
			if( xmlParser.push( zpString( "info" ) ) )
			{
				zpXmlNode* info = xmlParser.getCurrentNode();
				outputJson[ "face" ] =		zpJson( info->attributes.getString( "face" ) );
				outputJson[ "size" ] =		zpJson( info->attributes.getInt( "size" ) );
				outputJson[ "bold" ] =		zpJson( info->attributes.getInt( "bold" ) );
				outputJson[ "italic" ] =	zpJson( info->attributes.getInt( "italic" ) );
				outputJson[ "charset" ] =	zpJson( info->attributes.getString( "charset" ) );
				outputJson[ "unicode" ] =	zpJson( info->attributes.getInt( "unicode" ) );
				outputJson[ "stretchH" ] =	zpJson( info->attributes.getInt( "stretchH" ) );
				outputJson[ "smooth" ] =	zpJson( info->attributes.getInt( "smooth" ) );
				outputJson[ "aa" ] =		zpJson( info->attributes.getInt( "aa" ) );
				outputJson[ "outline" ] =	zpJson( info->attributes.getInt( "outline" ) );

				zpFixedArrayList< zpString, 4 > parts;
				const zpString& padding = info->attributes.getString( "padding" );
				const zpString& spacing = info->attributes.getString( "spacing" );

				padding.split( ',', parts );
				zpJson& p = outputJson[ "padding" ];
				p[(zp_uint)0] = zpJson( zp_atoi( parts[0].str() ) );
				p[(zp_uint)1] = zpJson( zp_atoi( parts[1].str() ) );
				p[(zp_uint)2] = zpJson( zp_atoi( parts[2].str() ) );
				p[(zp_uint)3] = zpJson( zp_atoi( parts[3].str() ) );

				spacing.split( ',', parts );
				zpJson& s = outputJson[ "spacing" ];
				s[(zp_uint)0] = zpJson( zp_atoi( parts[0].str() ) );
				s[(zp_uint)1] = zpJson( zp_atoi( parts[1].str() ) );

				xmlParser.pop();
			}

			zp_int pageCount = 0;
			if( xmlParser.push( zpString( "common" ) ) )
			{
				zpXmlNode* common = xmlParser.getCurrentNode();
				pageCount = common->attributes.getInt( "pages" );

				outputJson[ "lineHeight" ] =	zpJson( common->attributes.getInt( "lineHeight" ) );
				outputJson[ "base" ] =			zpJson( common->attributes.getInt( "base" ) );
				outputJson[ "scaleW" ] =		zpJson( common->attributes.getInt( "scaleW" ) );
				outputJson[ "scaleH" ] =		zpJson( common->attributes.getInt( "scaleH" ) );
				outputJson[ "packed" ] =		zpJson( common->attributes.getInt( "packed" ) );
				outputJson[ "alphaChnl" ] =		zpJson( common->attributes.getInt( "alphaChnl" ) );
				outputJson[ "redChnl" ] =		zpJson( common->attributes.getInt( "redChnl" ) );
				outputJson[ "greenChnl" ] =		zpJson( common->attributes.getInt( "greenChnl" ) );
				outputJson[ "blueChnl" ] =		zpJson( common->attributes.getInt( "blueChnl" ) );

				xmlParser.pop();
			}

			if( xmlParser.push( zpString( "pages" ) ) )
			{
				if( xmlParser.push( zpString( "page" ) ) )
				{
					zpJson& pages = outputJson[ "pages" ];
					pages.reserveArray( pageCount );
					do
					{
						zpXmlNode* page = xmlParser.getCurrentNode();
						zp_int id = page->attributes.getInt( "id" );
						const zpString& file = page->attributes.getString( "file" );

						zpString outFile;
						zp_int index = outputFile.lastIndexOf( zpFile::sep );
						outputFile.substring( outFile, 0, index + 1 );
						outFile.append( file );

						pages[ id ] = zpJson( outFile );
					}
					while( xmlParser.next() );

					xmlParser.pop();
				}
				xmlParser.pop();
			}

			if( xmlParser.push( zpString( "chars" ) ) )
			{
				zpXmlNode* cs = xmlParser.getCurrentNode();
				zp_int count = cs->attributes.getInt( "count" );

				if( xmlParser.push( zpString( "char" ) ) )
				{
					zpJson& chars = outputJson[ "chars" ];
					chars.reserveArray( count );
					do
					{
						zpXmlNode* c = xmlParser.getCurrentNode();
						zpJson& ch = chars[ chars.size() ];
						ch[ "id" ] =		zpJson( c->attributes.getInt( "id" ) );
						ch[ "x" ] =			zpJson( c->attributes.getInt( "x" ) );
						ch[ "y" ] =			zpJson( c->attributes.getInt( "y" ) );
						ch[ "width" ] =		zpJson( c->attributes.getInt( "width" ) );
						ch[ "height" ] =	zpJson( c->attributes.getInt( "height" ) );
						ch[ "xoffset" ] =	zpJson( c->attributes.getInt( "xoffset" ) );
						ch[ "yoffset" ] =	zpJson( c->attributes.getInt( "yoffset" ) );
						ch[ "xadvance" ] =	zpJson( c->attributes.getInt( "xadvance" ) );
						ch[ "page" ] =		zpJson( c->attributes.getInt( "page" ) );
						ch[ "chnl" ] =		zpJson( c->attributes.getInt( "chnl" ) );
					}
					while( xmlParser.next() );

					xmlParser.pop();
				}
				xmlParser.pop();
			}

			if( xmlParser.push( zpString( "kernings" ) ) )
			{
				zpXmlNode* kernings = xmlParser.getCurrentNode();
				zp_int count = kernings->attributes.getInt( "count" );

				if( xmlParser.push( zpString( "kerning" ) ) )
				{
					zpJson& kernings = outputJson[ "kernings" ];
					kernings.reserveArray( count );
					do
					{
						zpXmlNode* k = xmlParser.getCurrentNode();
						zpJson& kerning = kernings[ kernings.size() ];
						kerning[ "first" ] =	zpJson( k->attributes.getInt( "first" ) );
						kerning[ "second" ] =	zpJson( k->attributes.getInt( "second" ) );
						kerning[ "amount" ] =	zpJson( k->attributes.getInt( "amount" ) );
					}
					while( xmlParser.next() );

					xmlParser.pop();
				}
				xmlParser.pop();
			}

			xmlParser.pop();
		}
	}

	return ok;
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
		zp_bool ok = false;
		zpJson outputJson;
		const zpString& inputFile = arguments[ 0 ];
		const zpString& outputFile = arguments[ 1 ];

		if( inputFile.endsWith( ".fnt" ) )
		{
			ok = compileBitmapFontXML( inputFile, outputFile, outputJson );
		}
		else if( inputFile.endsWith( ".ttf" ) )
		{
			//compileTrueTypeFont( inputFile, outputFile );
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