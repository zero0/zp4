#include "Main.h"

zp_bool TGATextureReader::getTextureBytes( const zpDataBuffer& data, ImageData& imageData )
{
	zp_bool ok = false;

	TGAHeader header;

	// Uncompressed TGA Header
	zp_byte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
	// Compressed TGA Header
	zp_byte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

	data.readAt( header, 0 );

	TGA tga;

	data.readAt( tga, sizeof( TGAHeader ) );

	// calc width and height
	imageData.width = tga.header[1] * 256 + tga.header[0];
	imageData.height = tga.header[3] * 256 + tga.header[2];
	tga.Bpp = tga.header[4];
	tga.Width = imageData.width;
	tga.Height = imageData.height;

	if( tga.Width == 0 || tga.Height == 0 ) return false;

	// calc format
	switch( tga.Bpp )
	{
	case 24:
		imageData.format = TEXTURE_FORMAT_RGB;
		break;
	case 32:
		imageData.format = TEXTURE_FORMAT_RGBA;
		break;
	default:
		return false;
	}

	// gets stride and image size
	tga.bytesPerPixel = tga.Bpp / 8;
	tga.imageSize = tga.bytesPerPixel * tga.Width * tga.Height;

	// read uncompressed texture
	if( zp_memcmp( uTGAcompare, &header, sizeof( TGAHeader ) ) == 0 )
	{
		ok = getUncompressedTextureBytes( tga, data, imageData );
	}
	// read compressed texture
	else if( zp_memcmp( cTGAcompare, &header, sizeof( TGAHeader ) ) == 0 )
	{
		ok = getCompressedTextureBytes( tga, data, imageData );
	}

	// if reading was ok, flip vertically if needed and swap from BGR to RGB
	if( ok )
	{
		zp_byte* d = imageData.imageBytes.getData();

		// swap BGR to RGB
		for( zp_uint i = 0; i < tga.imageSize; i += tga.bytesPerPixel )
		{
			d[ i ] ^= d[ i + 2 ] ^= d[ i ] ^= d[ i + 2 ];
		}
	}

	return ok;
}

zp_bool TGATextureReader::getCompressedTextureBytes( const TGA& tga,const zpDataBuffer& data, ImageData& imageData )
{
	zp_uint pixelCount = tga.Height * tga.Width;
	zp_uint currentPixel = 0;
	zp_uint readByte = 0;

	const zp_byte* imageBytes = data.getData() + sizeof( TGAHeader ) + sizeof( tga.header );

	imageData.imageBytes.reset();
	imageData.imageBytes.reserve( tga.imageSize );

	zp_byte buff[ 4 ];

	do
	{
		zp_byte chunkheader = imageBytes[ readByte++ ];

		// write a few pixels
		if( chunkheader < 128 )
		{
			chunkheader++;

			for( zp_uint c = 0; c < chunkheader; ++c )
			{
				imageData.imageBytes.writeBulk( imageBytes + readByte, tga.bytesPerPixel );
				readByte += tga.bytesPerPixel;
			}

		}
		// write repeating pixels
		else
		{
			chunkheader -= 127;

			zp_memcpy( buff, sizeof( buff ), imageBytes + readByte, tga.bytesPerPixel );
			readByte += tga.bytesPerPixel;

			for( zp_uint c = 0; c < chunkheader; ++c )
			{
				imageData.imageBytes.writeBulk( buff, tga.bytesPerPixel );
			}
		}

		currentPixel += chunkheader;

	}
	while( currentPixel < pixelCount );

	return true;
}

zp_bool TGATextureReader::getUncompressedTextureBytes( const TGA& tga,const zpDataBuffer& data, ImageData& imageData )
{
	imageData.imageBytes.reset();
	imageData.imageBytes.reserve( tga.imageSize );

	// write tga bytes to output image data
	const zp_byte* imageBytes = data.getData() + sizeof( TGAHeader ) + sizeof( tga.header );
	imageData.imageBytes.writeBulk( imageBytes, tga.imageSize );

	zp_byte* d = imageData.imageBytes.getData();

	// tga stores texture like GL, flip if incoming compression is DXT
	if( imageData.compression != TEXTURE_COMPRESSION_DXT )
	{
		zp_uint stride = tga.bytesPerPixel * tga.Width;
		zp_byte* buff = new zp_byte[ stride ];

		// flip image upside down
		for( zp_uint h = 0, hmax = tga.Height / 2; h < hmax; ++h )
		{
			zp_byte* a = d + h * stride;
			zp_byte* b = d + ( tga.Height - h - 1 ) * stride;

			zp_memcpy( buff, stride, a, stride );
			zp_memcpy( a, stride, b, stride );
			zp_memcpy( b, stride, buff, stride );
		}

		ZP_SAFE_DELETE_ARRAY( buff );
	}

	return true;
}
