#pragma once
#ifndef TEXUTRE_COMPILER_H
#define TEXUTRE_COMPILER_H

ZP_PURE_INTERFACE TextureReader
{
public:
	virtual ~TextureReader() {}

	virtual zp_bool getTextureBytes( const zpDataBuffer& data, ImageData& imageData ) = 0;
};

ZP_PURE_INTERFACE TextureCompressor
{
public:
	virtual ~TextureCompressor() {}

	virtual void compress( const ImageData& inputImage, ImageData& compiledImage ) = 0;
};


class TextureCompiler
{
public:
	TextureCompiler();
	~TextureCompiler();

	zp_bool initialize( const zpArrayList< zpString >& args );
	zp_bool compile();
	void shutdown();

private:
	TextureReader* m_reader;
	TextureCompressor* m_compressor;

	zpString m_compression;

	zpArrayList< zpString > m_inputFiles;
	zpString m_outputFile;

	TextureFormat m_desiredFormat;
	TextureCompression m_desiredCompressionType;
	TextureCompression m_desiredCompression;

	zpArrayList< ImageData > m_rawImages;
	zpArrayList< ImageData > m_compressedImages;
};

#endif
