#include "ProtoDBCompiler.h"

zp_bool compileProtoFile( zpJsonParser& parser, const zpString& catagoryName, const zpString catagoryDir, zpJson& outProtoDB )
{

    return true;
}

zp_bool compileProtoCatagory( zpJsonParser& parser, const zpString& catagoryName, const zpString catagoryDir, zpJson& outProtoDB )
{
    zpArrayList< zpString > protoFiles;
    zpFile::listFiles( catagoryDir, protoFiles );

    return true;
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
            << "Usage: ProtoDBCompiler.exe "
            << zpLog::gray << "path/to/protodb/ path/to/proto.db"
            << zpLog::endl
            ;
    }
    else
    {
        // use last two args as input and output files
        const zpString& inputFile = arguments[ arguments.size() - 2 ];
        const zpString& outputFile = arguments[ arguments.size() - 1 ];

        zp_bool ok = false;
        zpJson protoDB;
        zpJsonParser parser;
        zpArrayList< zpString > protoCatagoryFiles;

        zpFile::listFiles( inputFile, protoCatagoryFiles );

        // if compilation ok
        if( ok )
        {
            if( zpBison::compileToFile( outputFile, protoDB ) )
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
