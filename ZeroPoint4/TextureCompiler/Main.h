#pragma once
#ifndef TEXTURE_COMPILER_MAIN_H
#define TEXTURE_COMPILER_MAIN_H

#include "Core\zpCore.h"

enum TextureType
{
	TEXTURE_TYPE_PNG,

	TextureType_Count,
};

enum TextureFormat
{
	TEXTURE_FORMAT_R,
	TEXTURE_FORMAT_RG,
	TEXTURE_FORMAT_RGB,
	TEXTURE_FORMAT_RGBA,

	TextureFormat_Count,

	TEXTURE_FORMAT_NONE,
};

enum TextureCompression
{
	TEXTURE_COMPRESSION_TRUECOLOR,
	TEXTURE_COMPRESSION_BC1,
	TEXTURE_COMPRESSION_BC2,
	TEXTURE_COMPRESSION_BC3,
	TEXTURE_COMPRESSION_BC4,
	TEXTURE_COMPRESSION_BC5,
	TEXTURE_COMPRESSION_PVRTC,
	TEXTURE_COMPRESSION_PVRTC2,
	TEXTURE_COMPRESSION_ETC1,
	TEXTURE_COMPRESSION_ETC2,
	TEXTURE_COMPRESSION_ETC2A,

	TextureCompression_Count,

	TEXTURE_COMPRESSION_DXT,
	TEXTURE_COMPRESSION_PVR,
	TEXTURE_COMPRESSION_ETC,

	TEXTURE_COMPRESSION_NONE,
};

struct ImageData
{
	zp_uint width;
	zp_uint height;
	zp_uint stride;

	TextureCompression compression;
	TextureFormat format;

	zpDataBuffer imageBytes;
};

#include "TextureCompiler.h"

#include "PNGTextureReader.h"

#include "DXTTextureCompressor.h"

#endif
