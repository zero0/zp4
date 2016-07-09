#pragma once
#ifndef TGA_TEXTURE_COMPRESSOR_H
#define TGA_TEXTURE_COMPRESSOR_H

class TGATextureReader : public TextureReader
{
public:
    virtual ~TGATextureReader() {}

    zp_bool getTextureBytes( const zpDataBuffer& data, ImageData& imageData );

private:
    struct TGAHeader
    {
        zp_byte header[12];
    };

    struct TGA
    {
        zp_byte header[6];
        zp_uint bytesPerPixel;
        zp_uint imageSize;
        zp_uint Height;
        zp_uint Width;
        zp_uint Bpp;
    };

    zp_bool getCompressedTextureBytes( const TGA& tga, const zpDataBuffer& data, ImageData& imageData );
    zp_bool getUncompressedTextureBytes( const TGA& tga, const zpDataBuffer& data, ImageData& imageData );


};

#endif
