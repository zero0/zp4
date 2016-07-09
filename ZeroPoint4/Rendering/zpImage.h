#pragma once
#ifndef ZP_IMAGE_H
#define ZP_IMAGE_H

#define DDS_CAPS        0x00000001
#define DDS_HEIGHT      0x00000002
#define DDS_WIDTH       0x00000004
#define DDS_PITCH       0x00000008
#define DDS_PIXELFORMAT 0x00001000
#define DDS_MIPMAPCOUNT 0x00020000
#define DDS_LINEARSIZE  0x00080000
#define DDS_DEPTH       0x00800000

#define DDSCAPS_COMPLEX  0x00000008 
#define DDSCAPS_TEXTURE  0x00001000 
#define DDSCAPS_MIPMAP   0x00400000 

#define DDSCAPS2_CUBEMAP 0x00000200 
#define DDSCAPS2_VOLUME  0x00200000 

#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000
#define DDSCAPS2_CUBEMAP_ALL_FACES (DDSCAPS2_CUBEMAP_POSITIVEX | DDSCAPS2_CUBEMAP_NEGATIVEX | DDSCAPS2_CUBEMAP_POSITIVEY | DDSCAPS2_CUBEMAP_NEGATIVEY | DDSCAPS2_CUBEMAP_POSITIVEZ | DDSCAPS2_CUBEMAP_NEGATIVEZ)
#define DDSCAPS2_VOLUME    0x00200000

#define DDS_MAGIC    ZP_MAKE_UINT( 'D', 'D', 'S', ' ' )
#define DDS_DXT1_ID    ZP_MAKE_UINT( 'D', 'X', 'T', '1' )
#define DDS_DXT2_ID    ZP_MAKE_UINT( 'D', 'X', 'T', '2' )
#define DDS_DXT3_ID    ZP_MAKE_UINT( 'D', 'X', 'T', '3' )

enum zpCompressionType
{
    ZP_COMPRESSION_NONE,
    ZP_COMPRESSION_BC1,        /* DXT1 */
    ZP_COMPRESSION_BC2,        /* DXT3 */
    ZP_COMPRESSION_BC3,        /* DXT5 */
    ZP_COMPRESSION_BC3N,    /* DXT5n */
    ZP_COMPRESSION_BC4,        /* ATI1 */
    ZP_COMPRESSION_BC5,        /* ATI2 */
    ZP_COMPRESSION_AEXP,    /* DXT5 */
    ZP_COMPRESSION_YCOCG,    /* DXT5 */
    ZP_COMPRESSION_YCOCGS,    /* DXT5 */

    zpCompressionType_Count,
};

struct zpDDSHeader
{
    zp_uint dwMagic;
    zp_uint dwSize;
    zp_uint dwFlags;
    zp_uint dwHeight;
    zp_uint dwWidth;
    zp_uint dwPitchOrLinearSize;
    zp_uint dwDepth; 
    zp_uint dwMipMapCount;
    zp_uint dwReserved[11];

    struct
    {
        zp_uint dwSize;
        zp_uint dwFlags;
        zp_uint dwFourCC;
        zp_uint dwRGBBitCount;
        zp_uint dwRBitMask;
        zp_uint dwGBitMask;
        zp_uint dwBBitMask;
        zp_uint dwRGBAlphaBitMask; 
    } ddpfPixelFormat;

    struct
    {
        zp_uint dwCaps;
        zp_uint dwCaps2;
        zp_uint dwCaps3;
        zp_uint dwCaps4;
    } ddsCaps;

    zp_uint dwReserved2;
};

struct zpDDSHeaderDX10
{
    zpDisplayFormat dxgiFormat;
    zp_uint resourceDimension;
    zp_uint miscFlag;
    zp_uint arraySize;
    zp_uint reserved;
};

struct zpImageMipLevel
{
    const zp_byte* data;
    zp_uint rowBytes;
    zp_uint numBytes;
};

struct zpImageData
{
    zp_uint width;
    zp_uint height;
    zp_uint depth;
    zp_uint mipMaps;
    zp_uint arraySize;
    zp_uint pitch;
    zpCompressionType compressionType;
    zpDisplayFormat format;
    zpImageMipLevel mipLevels[10];
    zpDataBuffer pixels;
};

namespace zpImage
{
    zp_bool loadDDSFromFile( const zp_char* filename, zp_uint flags, zpImageData& outImageData );
};

#endif