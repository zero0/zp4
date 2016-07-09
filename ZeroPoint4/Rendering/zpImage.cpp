#include "zpRendering.h"

static zp_uint bitsPerPixel( zpDisplayFormat fmt )
{
    switch( fmt )
    {
    case ZP_DISPLAY_FORMAT_UNKNOWN:
        return 1;

        // R Component
    case ZP_DISPLAY_FORMAT_R8_UINT:
    case ZP_DISPLAY_FORMAT_R8_SINT:
        return 8;

    case ZP_DISPLAY_FORMAT_R16_UINT:
    case ZP_DISPLAY_FORMAT_R16_SINT:
    case ZP_DISPLAY_FORMAT_R16_FLOAT:

        // RG Component
    case ZP_DISPLAY_FORMAT_RG8_UINT:
    case ZP_DISPLAY_FORMAT_RG8_SINT:
        return 16;

    case ZP_DISPLAY_FORMAT_R32_UINT:
    case ZP_DISPLAY_FORMAT_R32_SINT:
    case ZP_DISPLAY_FORMAT_R32_FLOAT:

    case ZP_DISPLAY_FORMAT_RG16_UINT:
    case ZP_DISPLAY_FORMAT_RG16_SINT:
    case ZP_DISPLAY_FORMAT_RG16_FLOAT:

        // RGBA Components
    case ZP_DISPLAY_FORMAT_RGBA8_UINT:
    case ZP_DISPLAY_FORMAT_RGBA8_SINT:
    case ZP_DISPLAY_FORMAT_RGBA8_UNORM:
    case ZP_DISPLAY_FORMAT_RGBA8_SNORM:
        return 32;

    case ZP_DISPLAY_FORMAT_RG32_UINT:
    case ZP_DISPLAY_FORMAT_RG32_SINT:
    case ZP_DISPLAY_FORMAT_RG32_FLOAT:

    case ZP_DISPLAY_FORMAT_RGBA16_UINT:
    case ZP_DISPLAY_FORMAT_RGBA16_SINT:
    case ZP_DISPLAY_FORMAT_RGBA16_UNORM:
    case ZP_DISPLAY_FORMAT_RGBA16_SNORM:
    case ZP_DISPLAY_FORMAT_RGBA16_FLOAT:
        return 64;

        // RGB Component
    case ZP_DISPLAY_FORMAT_RGB32_UINT:
    case ZP_DISPLAY_FORMAT_RGB32_SINT:
    case ZP_DISPLAY_FORMAT_RGB32_FLOAT:
        return 96;

    case ZP_DISPLAY_FORMAT_RGBA32_UINT:
    case ZP_DISPLAY_FORMAT_RGBA32_SINT:
    case ZP_DISPLAY_FORMAT_RGBA32_FLOAT:
        return 128;

    default: return 0;
    }
}

zp_bool zpImage::loadDDSFromFile( const zp_char* filename, zp_uint flags, zpImageData& outImageData )
{
    zpFile imageFile( filename );
    if( imageFile.open( ZP_FILE_MODE_BINARY_READ ) )
    {
        zpDataBuffer imageBuffer;
        imageFile.readFileBinary( imageBuffer );
        imageFile.close();

        zpDDSHeader ddsHeader;
        imageBuffer.readAt( ddsHeader, 0 );

        if( ddsHeader.dwMagic != DDS_MAGIC )
        {
            return false;
        }

        outImageData.width = ddsHeader.dwWidth;
        outImageData.height = ddsHeader.dwHeight;
        outImageData.depth = ( ddsHeader.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP ) ? 0 : ( ddsHeader.dwDepth == 0 ) ? 1 : ddsHeader.dwDepth;
        outImageData.mipMaps = ( ddsHeader.dwMipMapCount == 0 ) ? 1 : ddsHeader.dwMipMapCount;
        outImageData.arraySize = 1;


    }

    return false;
}