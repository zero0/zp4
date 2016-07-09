#include "BisonCompiler.h"

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
            << "Usage: BisonCompiler.exe " << zpLog::blue << "path/to/inputfile.json " << zpLog::green << "path/to/outputfile.bison" << zpLog::endl
            ;
    }
    else
    {
        zp_bool ok;

        const zpString& inputFilename = arguments[ 0 ];
        const zpString& outputFilename = arguments[ 1 ];

        zpJson json;
        zpJsonParser jsonParser;

        ok = jsonParser.parseFile( inputFilename, json );
        if( ok )
        {
            ok = zpBison::compileToFile( outputFilename, json );
            if( ok )
            {
                zpLog::message()
                    << "Successfully compiled '" << outputFilename << "'" << zpLog::endl
                    ;
            }
            else
            {
                zpLog::error()
                    << "Failed to compile Bison file '" << outputFilename << "'" << zpLog::endl
                    ;
            }
        }
        else
        {
            zpLog::error()
                << "Failed to parse JSON file '" << inputFilename << "'" << zpLog::endl
                ;
        }
    }

    return 0;
}