#pragma once
#ifndef DXT_TEXTURE_COMPRESSOR_H
#define DXT_TEXTURE_COMPRESSOR_H

class DXTTextureCompressor : public TextureCompressor
{
public:
	virtual ~DXTTextureCompressor() {}

	void compress( const ImageData& inputImage, ImageData& compiledImage );

private:
	void compressBC1( const ImageData& inputImage, ImageData& compiledImage );
	void compressBC2( const ImageData& inputImage, ImageData& compiledImage );
	void compressBC3( const ImageData& inputImage, ImageData& compiledImage );
	void compressBC4( const ImageData& inputImage, ImageData& compiledImage );
	void compressBC5( const ImageData& inputImage, ImageData& compiledImage );
};

#endif
