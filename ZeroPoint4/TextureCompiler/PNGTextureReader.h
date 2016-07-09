#ifndef PNG_TEXTURE_READER_H
#define PNG_TEXTURE_READER_H

class PNGTextureReader : public TextureReader
{
public:
    virtual ~PNGTextureReader() {}

    zp_bool getTextureBytes( const zpDataBuffer& data, ImageData& imageData );
};

#endif
