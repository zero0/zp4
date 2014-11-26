#include "Main.h"
#include <png.h>

#pragma comment( lib, "zlib.lib" )
#pragma comment( lib, "libpng16.lib" )

zp_bool PNGTextureReader::getTextureBytes( const zpDataBuffer& fileData, ImageData& imageData )
{
	TextureFormat desiredFormat = imageData.format;

	zp_int r;
	zp_bool ok;

	png_image png;
	zp_zero_memory( &png );
	png.version = PNG_IMAGE_VERSION;

	r = png_image_begin_read_from_memory( &png, fileData.getData(), fileData.size() );

	// set width and height of image
	imageData.width = png.width;
	imageData.height = png.height;

	// remove color map flag if set
	png.format = (png.format) & ~PNG_FORMAT_FLAG_COLORMAP;

	// if desired format set, convert png to that type
	if( desiredFormat != TEXTURE_FORMAT_NONE )
	{
		switch( desiredFormat )
		{
		case TEXTURE_FORMAT_R:
			imageData.format = TEXTURE_FORMAT_R;
			png.format = PNG_FORMAT_GRAY;
			break;
		case TEXTURE_FORMAT_RGB:
			imageData.format = TEXTURE_FORMAT_RGB;
			png.format = PNG_FORMAT_RGB;
			break;
		case TEXTURE_FORMAT_RGBA:
			imageData.format = TEXTURE_FORMAT_RGBA;
			png.format = PNG_FORMAT_RGBA;
			break;
		}
	}
	// otherwise, output the format
	else
	{
		switch( png.format )
		{
		case PNG_FORMAT_GRAY:
			imageData.format = TEXTURE_FORMAT_R;
			break;
		case PNG_FORMAT_RGB:
			imageData.format = TEXTURE_FORMAT_RGB;
			break;
		case PNG_FORMAT_RGBA:
			imageData.format = TEXTURE_FORMAT_RGBA;
			break;
		default:
			imageData.format = TEXTURE_FORMAT_NONE;
			break;
		}
	}

	zp_int stride = PNG_IMAGE_ROW_STRIDE( png );
	zp_uint size = PNG_IMAGE_SIZE( png );

	imageData.imageBytes.reset();
	imageData.imageBytes.reserve( size );

	r = png_image_finish_read( &png, ZP_NULL, imageData.imageBytes.getData(), stride, ZP_NULL );

	png_image_free( &png );

	ok = true;

	return ok;
}
