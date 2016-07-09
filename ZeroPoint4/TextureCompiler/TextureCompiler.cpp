#include "Main.h"

const zp_char* s_textureFormat[ TextureFormat_Count ] =
{
    "R",
    "RG",
    "RGB",
    "RGBA",
};
zp_uint s_textureFormatStride[ TextureFormat_Count ] =
{
    1,
    2,
    3,
    4,
};

zp_uint s_textureCompressionStride[ TextureCompression_Count ] =
{
    4,
    8,
    8,
    16,
    8,
    16,
    8,
    8,
    8,
    8,
    8,
};

const zp_char* s_textureCompression[ TextureCompression_Count ] =
{
    "TrueColor",
    "BC1",
    "BC2",
    "BC3",
    "BC4",
    "BC5",
    "PVRTC",
    "PVRTC2",
    "ETC1",
    "ETC2",
    "ETC2A",
};

TextureCompiler::TextureCompiler()
{}
TextureCompiler::~TextureCompiler()
{}

zp_bool TextureCompiler::initialize( const zpArrayList< zpString >& args )
{
    m_compression = args[ 0 ];
    if( m_compression.startsWith( "DX" ) )
    {
        m_desiredCompressionType = TEXTURE_COMPRESSION_DXT;
    }
    else if( m_compression.startsWith( "GL" ) )
    {
        m_desiredCompressionType = TEXTURE_COMPRESSION_PVR;
    }

    m_desiredFormat = TEXTURE_FORMAT_NONE;
    m_desiredCompression = TEXTURE_COMPRESSION_NONE;

    zp_bool isCubeMap = false;

    for( zp_uint i = 0, imax = args.size(); i < imax; ++i )
    {
        const zpString& arg = args[ i ];

        if( arg.startsWith( "-F" ) )
        {
            const zp_char* desiredFormat = arg.str() + 2;
            for( zp_uint i = 0; i < TextureFormat_Count; ++i )
            {
                if( zp_strcmp( s_textureFormat[ i ], desiredFormat ) == 0 )
                {
                    m_desiredFormat = (TextureFormat)i;
                    break;
                }
            }
        }
        else if( arg.startsWith( "-C" ) )
        {
            const zp_char* desiredCompression = arg.str() + 2;
            for( zp_uint i = 0; i < TextureCompression_Count; ++i )
            {
                if( zp_strcmp( s_textureCompression[ i ], desiredCompression ) == 0 )
                {
                    m_desiredCompression = (TextureCompression)i;
                    break;
                }
            }
        }
        else if( arg.startsWith( "-3" ) )
        {
            if( !isCubeMap )
            {
                m_inputFiles.reserve( TextureCubeFaceOrder_Count );
                m_inputFiles.resize( TextureCubeFaceOrder_Count );
            }

            isCubeMap = true;
            zp_char face = arg.charAt( 2 );
            const zp_char* cubeMapFile = arg.str() + 3;

            TextureCubeFaceOrder face;
            switch( face )
            {
                // positive
            case 'X':
                face = TEXTURE_CUBE_FACE_P_X;
                break;
                // negative
            case 'x':
                face = TEXTURE_CUBE_FACE_N_X;
                break;

                // positive
            case 'Y':
                face = TEXTURE_CUBE_FACE_P_Y;
                break;
                // negative
            case 'y':
                face = TEXTURE_CUBE_FACE_N_Y;
                break;

                // positive
            case 'Z':
                face = TEXTURE_CUBE_FACE_P_Z;
                break;
                // negative
            case 'z':
                face = TEXTURE_CUBE_FACE_N_Z;
                break;
            }

            m_inputFiles[ face ] = cubeMapFile;
        }
    }

    if( !isCubeMap )
    {
        m_inputFiles.pushBackEmpty() = args[ args.size() - 2 ];
    }
    m_outputFile = args[ args.size() - 1 ];

    return true;
}
zp_bool TextureCompiler::compile()
{
    zp_bool ok = false;

    TextureReader* reader = ZP_NULL;
    TextureCompressor* compressor = ZP_NULL;

    // open texture file
    zpFile textureFile( m_inputFile );
    if( textureFile.open( ZP_FILE_MODE_BINARY_READ ) )
    {
        // read all bytes of file
        zpDataBuffer fileData;
        textureFile.readFileBinary( fileData );
        textureFile.close();

        // get png reader
        if( m_inputFile.endsWith( ".png" ) )
        {
            reader = new PNGTextureReader;
        }
        // get tga reader
        else if( m_inputFile.endsWith( ".tga" ) )
        {
            reader = new TGATextureReader;
        }

        ZP_ASSERT( reader != ZP_NULL, "Unknown image format for image %s", m_inputFile.str() );

        // set the desired format of the image
        m_rawImage.format = m_desiredFormat;
        m_rawImage.compression = m_desiredCompressionType;

        // read texture bytes from format to raw
        zp_bool ok = reader->getTextureBytes( fileData, m_rawImage );
        if( ok )
        {
            // use desired compressor
            switch( m_desiredCompressionType )
            {
            case TEXTURE_COMPRESSION_DXT:
                compressor = new DXTTextureCompressor;
                break;
            }

            ZP_ASSERT( compressor, "Unknown compression format for image %s", m_inputFile.str() );

            // set stride
            m_rawImage.stride = s_textureFormatStride[ m_rawImage.format ];
            m_rawImage.compression = m_desiredCompression;

            // compress image
            compressor->compress( m_rawImage, m_compressedImage );

            // set stride from compressed image
            m_compressedImage.stride = s_textureCompressionStride[ m_compressedImage.compression ];
        }
    }

    // delete reader and compressor
    ZP_SAFE_DELETE( reader );
    ZP_SAFE_DELETE( compressor );

    return ok;
}


void TextureCompiler::shutdown()
{
    zpJson image;
    image[ "RawWidth" ] = zpJson( m_rawImage.width );
    image[ "RawHeight" ] = zpJson( m_rawImage.height );

    image[ "Width" ] = zpJson( m_compressedImage.width );
    image[ "Height" ] = zpJson( m_compressedImage.height );

    image[ "Format" ] = zpJson( s_textureFormat[ m_compressedImage.format ] );
    image[ "Stride" ] = zpJson( m_compressedImage.stride );

    image[ "Compression" ] = zpJson( s_textureCompression[ m_compressedImage.compression ] );

    image[ "Data" ] = zpJson( m_compressedImage.imageBytes );

#if 0
    zpStringBuffer sb;
    zpDataBuffer db;

    zp_base64_encode( m_compressedImage.imageBytes.getData(), m_compressedImage.imageBytes.size(), sb );
    zp_base64_decode( sb.str(), sb.length(), db );

    ZP_ASSERT( db.size() == m_compressedImage.imageBytes.size(), "" );
    ZP_ASSERT( zp_memcmp( db.getData(), m_compressedImage.imageBytes.getData(), m_compressedImage.imageBytes.size() ) == 0, "" );
#endif

    if( !zpBison::compileToFile( m_outputFile, image ) )
    {
        zpLog::error() << "Unable to compile output file '" << m_outputFile << "'" << zpLog::endl;
    }
    else
    {
        zpLog::message() << "Successfully compiled '" << m_outputFile << "'" << zpLog::endl;
    }
}
