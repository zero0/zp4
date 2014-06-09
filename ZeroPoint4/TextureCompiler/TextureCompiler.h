#include "Core/zpCore.h"

enum TextureFormat
{
	TEXTURE_FORMAT_R8,
	TEXTURE_FORMAT_R16,
	TEXTURE_FORMAT_R32,
	TEXTURE_FORMAT_RGB8,
	TEXTURE_FORMAT_RGBA8,

	TextureFormat_Count,

	TEXTURE_FORMAT_NONE,
};

enum TextureCompression
{
	TEXTURE_COMPRESSION_TRUECOLOR,
	TEXTURE_COMPRESSION_DXT1,
	TEXTURE_COMPRESSION_DXT3,
	TEXTURE_COMPRESSION_DXT5,
	TEXTURE_COMPRESSION_PVTRC,
	TEXTURE_COMPRESSION_PVTRC2,
	TEXTURE_COMPRESSION_ETC,

	TextureCompression_Count,

	TEXTURE_COMPRESSION_NONE,
};

const zp_char* s_textureFormat[ TextureFormat_Count ] =
{
	"R8",
	"R16",
	"R32",
	"RGB8",
	"RGBA8",
};
zp_uint s_textureFormatStride[ TextureFormat_Count ] =
{
	1,
	2,
	4,
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
	8,
	8,
};

const zp_char* s_textureCompression[] =
{
	"TrueColor",
	"DXT1",
	"DXT3",
	"DXT5",
	"PVRTC",
	"PVRTC2",
	"ETC",
};

struct ImageData
{
	zp_uint width;
	zp_uint height;
	TextureFormat format;
	zpDataBuffer imageBytes;
};

ZP_ABSTRACT_CLASS BaseTextureCompiler
{
public:
	BaseTextureCompiler();
	virtual ~BaseTextureCompiler();

	zp_bool initialize( const zpArrayList< zpString >& args );
	zp_bool compile();
	void shutdown();

protected:
	virtual zp_bool getTextureBytes( ImageData& imageData ) = 0;

	void compileDXT1( const ImageData& inputImage, ImageData& compiledImage );

	zpString m_compression;

	zpString m_inputFile;
	zpString m_outputFile;

	TextureFormat m_desiredFormat;
	TextureCompression m_desiredCompression;

	ImageData m_rawImage;
	ImageData m_compressedImage;

};

class PNGTextureCompiler : public BaseTextureCompiler
{
public:
	PNGTextureCompiler();
	virtual ~PNGTextureCompiler();

protected:
	zp_bool getTextureBytes( ImageData& imageData );
};
