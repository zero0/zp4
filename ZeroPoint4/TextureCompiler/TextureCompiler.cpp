#include "TextureCompiler.h"
#include <png.h>

#if ZP_DEBUG
#pragma comment( lib, "zlibd.lib" )
#pragma comment( lib, "libpng16d.lib" )
#else
#pragma comment( lib, "zlib.lib" )
#pragma comment( lib, "libpng16.lib" )
#endif

#define COMPRESSED_BLOCK_STRIDE		4
#define COMPRESSED_BLOCK_SIZE		( COMPRESSED_BLOCK_STRIDE * COMPRESSED_BLOCK_STRIDE )

BaseTextureCompiler::BaseTextureCompiler()
{}
BaseTextureCompiler::~BaseTextureCompiler()
{}

zp_bool BaseTextureCompiler::initialize( const zpArrayList< zpString >& args )
{
	m_compression = args[ 0 ];
	m_desiredCompression = TEXTURE_COMPRESSION_DXT1;

	m_desiredFormat = TEXTURE_FORMAT_NONE;
	if( args.size() == 4 )
	{
		const zp_char* desiredformat = args[ 1 ].str();
		for( zp_uint i = 0; i < TextureFormat_Count; ++i )
		{
			if( zp_strcmp( s_textureFormat[ i ], desiredformat ) == 0 )
			{
				m_desiredFormat = (TextureFormat)i;
				break;
			}
		}
	}

	m_inputFile = args[ args.size() - 2 ];
	m_outputFile = args[ args.size() - 1 ];

	return true;
}
zp_bool BaseTextureCompiler::compile()
{
	zp_bool ok = getTextureBytes( m_rawImage );
	if( ok )
	{
		switch( m_desiredCompression )
		{
		case TEXTURE_COMPRESSION_DXT1:
			compileDXT1( m_rawImage, m_compressedImage );
			break;
		}
	}

	return ok;
}

struct BC1Block
{
	zp_ushort color_0;
	zp_ushort color_1;

	union
	{
		struct
		{
			zp_byte dcba;
			zp_byte hgfe;
			zp_byte lkji;
			zp_byte ponm;
		};
		zp_byte data[4];
		zp_uint indices;
	};
	
};

struct ColorRGBA
{
	union
	{
		struct
		{
			zp_byte r, g, b, a;
		};
		zp_uint data;
	};

	zp_bool operator<( const ColorRGBA& c ) const
	{
		return r < c.r && g < c.g && b < c.b && a < c.a;
	}
};
struct ColorARGB
{
	union
	{
		struct
		{
			zp_byte a, r, g, b;
		};
		zp_uint data;
	};

	zp_bool operator<( const ColorRGBA& c ) const
	{
		return r < c.r && g < c.g && b < c.b && a < c.a;
	}
};

zp_int FloatToInt( zp_float a, zp_int l )
{
	zp_int i = (zp_int)( a + 0.5f );
	zp_clamp( i, i, 0, l );
	return i;
}

zp_ushort ColorTo565( zp_byte r, zp_byte g, zp_byte b )
{
	zp_int cr = FloatToInt( 31.0f * ( r / 255.f ), 31 );
	zp_int cg = FloatToInt( 63.0f * ( g / 255.f ), 63 );
	zp_int cb = FloatToInt( 31.0f * ( b / 255.f ), 31 );

	return ( cr << 11 ) | ( cg << 5 ) | cb;
}

template< typename ColorT >
ColorT LerpColor( const ColorT& a, const ColorT& b, zp_float alpha )
{
	ColorT r;

	zp_lerp( r.r, a.r, b.r, alpha );
	zp_lerp( r.g, a.g, b.g, alpha );
	zp_lerp( r.b, a.b, b.b, alpha );
	zp_lerp( r.a, a.a, b.a, alpha );

	return r;
};

template< typename ColorT >
zp_ushort ColorTo565( const ColorT& c )
{
	return ColorTo565( c.r, c.g, c.b );
}

template< typename ColorT >
void MakeColorRGB( ColorT& c, const zp_byte* rgb )
{
	c.r = rgb[0];
	c.g = rgb[1];
	c.b = rgb[2];
	c.a = zp_limit_max< zp_byte >();
}
template< typename ColorT >
void MakeColorRGB( ColorT& c, zp_byte r, zp_byte g, zp_byte b )
{
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = zp_limit_max< zp_byte >();
}
template< typename ColorT >
void MakeColorRGBA( ColorT& c, const zp_byte* rgba )
{
	c.r = rgba[0];
	c.g = rgba[1];
	c.b = rgba[2];
	c.a = rgba[3];
}
template< typename ColorT >
void MakeColorRGBA( ColorT& c, zp_byte r, zp_byte g, zp_byte b, zp_byte a )
{
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
}

template< typename ColorT >
void MinColor( ColorT& c, const ColorT& a, const ColorT& b )
{
	c.r = ZP_MIN( a.r, b.r );
	c.g = ZP_MIN( a.g, b.g );
	c.b = ZP_MIN( a.b, b.b );
	c.a = ZP_MIN( a.a, b.a );
}
template< typename ColorT >
void MaxColor( ColorT& c, const ColorT& a, const ColorT& b )
{
	c.r = ZP_MAX( a.r, b.r );
	c.g = ZP_MAX( a.g, b.g );
	c.b = ZP_MAX( a.b, b.b );
	c.a = ZP_MAX( a.a, b.a );
}

template< typename ColorT >
ColorT SubColor( const ColorT& a, const ColorT& b )
{
	ColorT c;
	c.r = ( a.r - b.r );
	c.g = ( a.g - b.g );
	c.b = ( a.b - b.b );
	c.a = zp_limit_max< zp_byte >();
	return c;
}
template< typename ColorT >
zp_uint DotColor( const ColorT& a, const ColorT& b )
{
	zp_uint d = ( a.r * b.r ) + ( a.g * b.g ) + ( a.b * b.b );
	return d;
}
template< typename ColorT >
zp_uint DistanceColor( const ColorT& a, const ColorT& b )
{
	ColorT s = SubColor( a, b );
	zp_uint d = DotColor( s, s );
	return d;
}

void BaseTextureCompiler::compileDXT1( const ImageData& inputImage, ImageData& compiledImage )
{
	compiledImage.width = inputImage.width;
	compiledImage.height = inputImage.height;

	zp_uint wmod = compiledImage.width % COMPRESSED_BLOCK_STRIDE;
	zp_uint hmod = compiledImage.height % COMPRESSED_BLOCK_STRIDE;

	if( wmod != 0 ) compiledImage.width += COMPRESSED_BLOCK_STRIDE - wmod;
	if( hmod != 0 ) compiledImage.height += COMPRESSED_BLOCK_STRIDE - hmod;

	compiledImage.format = inputImage.format;

	zp_uint stride = s_textureFormatStride[ inputImage.format ];
	const zp_byte* image = inputImage.imageBytes.getData();

	zp_uint cw = ( inputImage.width + 3 ) / 4;
	zp_uint ch = ( inputImage.height + 3 ) / 4;
	zp_uint compressedSize = ( cw * ch ) * sizeof( BC1Block );
	compiledImage.imageBytes.reserve( compressedSize );

	for( zp_uint y = 0; y < compiledImage.height; y += COMPRESSED_BLOCK_STRIDE )
	{
		for( zp_uint x = 0; x < compiledImage.width; x += COMPRESSED_BLOCK_STRIDE )
		{
			ColorRGBA color0, color1, color2, color3;
			ColorRGBA pixels[ COMPRESSED_BLOCK_SIZE ];

			color0.data = zp_limit_min< zp_uint >();
			color1.data = zp_limit_max< zp_uint >();

			for( zp_uint py = 0; py < COMPRESSED_BLOCK_STRIDE; ++py )
			{
				for( zp_uint px = 0; px < COMPRESSED_BLOCK_STRIDE; ++px )
				{
					ColorRGBA& c =  pixels[ px + ( py * COMPRESSED_BLOCK_STRIDE ) ];

					if( x + px > inputImage.width || y + py > inputImage.height )
					{
						c.r = 0;
						c.g = 0;
						c.b = 0;
						c.a = zp_limit_max< zp_byte >();
					}
					else
					{
						MakeColorRGB( c, &image[ stride * ( ( x + px ) + ( ( y + py ) * inputImage.width ) ) ] );
						
						MaxColor( color0, color0, c );
						MinColor( color1, color1, c );
					}
				}
			}

			zp_uint index;
			color2 = LerpColor( color0, color1,  1.f / 3.f );
			color3 = LerpColor( color0, color1,  2.f / 3.f );

			ColorRGBA palette[4] =
			{
				color0,
				color1,
				color2,
				color3
			};

			BC1Block block;
			block.color_0 = ColorTo565( color0 );
			block.color_1 = ColorTo565( color1 );
			block.indices = 0;

			for( zp_uint i = 0; i < COMPRESSED_BLOCK_SIZE; ++i )
			{
				zp_uint d0 = DistanceColor( palette[0], pixels[i] );
				zp_uint d1 = DistanceColor( palette[1], pixels[i] );
				zp_uint d2 = DistanceColor( palette[2], pixels[i] );
				zp_uint d3 = DistanceColor( palette[3], pixels[i] );

				zp_uint b0 = d0 > d3;
				zp_uint b1 = d1 > d2;
				zp_uint b2 = d0 > d2;
				zp_uint b3 = d1 > d3;
				zp_uint b4 = d2 > d3;

				zp_uint x0 = b1 & b2;
				zp_uint x1 = b0 & b3;
				zp_uint x2 = b0 & b4;

				block.indices |= (x2 | ((x0 | x1) << 1)) << (2 * i);
			}

			compiledImage.imageBytes.write( block );
		}
	}


}

void BaseTextureCompiler::shutdown()
{
	zpJson image;
	image[ "RawWidth" ] = zpJson( m_rawImage.width );
	image[ "RawHeight" ] = zpJson( m_rawImage.height );

	image[ "Width" ] = zpJson( m_compressedImage.width );
	image[ "Height" ] = zpJson( m_compressedImage.height );

	image[ "Format" ] = zpJson( s_textureFormat[ m_compressedImage.format ] );
	image[ "Stride" ] = zpJson( s_textureCompressionStride[ m_desiredCompression ] );

	image[ "Data" ] = zpJson( m_compressedImage.imageBytes );

	if( !zpBison::compileToFile( m_outputFile, image ) )
	{
		zpLog::error() << "Unable to compile output file '" << m_outputFile << "'" << zpLog::endl;
	}
	else
	{
		zpLog::message() << "Successfully compiled '" << m_outputFile << "'" << zpLog::endl;
	}
}



PNGTextureCompiler::PNGTextureCompiler()
{}
PNGTextureCompiler::~PNGTextureCompiler()
{}

zp_bool PNGTextureCompiler::getTextureBytes( ImageData& imageData )
{
	zp_bool ok = false;
	zpFile file( m_inputFile );
	if( file.open( ZP_FILE_MODE_BINARY_READ ) )
	{
		zp_int r;
		zpDataBuffer fileData;
		file.readFileBinary( fileData );

		file.close();

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
		if( m_desiredFormat != TEXTURE_FORMAT_NONE )
		{
			switch( m_desiredFormat )
			{
			case TEXTURE_FORMAT_R8:
				imageData.format = TEXTURE_FORMAT_R8;
				png.format = PNG_FORMAT_GRAY;
			case TEXTURE_FORMAT_RGB8:
				imageData.format = TEXTURE_FORMAT_RGB8;
				png.format = PNG_FORMAT_RGB;
			case TEXTURE_FORMAT_RGBA8:
				imageData.format = TEXTURE_FORMAT_RGBA8;
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
				imageData.format = TEXTURE_FORMAT_R8;
				break;
			case PNG_FORMAT_RGB:
				imageData.format = TEXTURE_FORMAT_RGB8;
				break;
			case PNG_FORMAT_RGBA:
				imageData.format = TEXTURE_FORMAT_RGBA8;
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
	}

	return ok;
}



zp_int main( zp_int argCount, const zp_char* args[] )
{
	zpArrayList< zpString > arguments;
	if( argCount > 1 )
	{
		zp_int c = argCount - 1;
		arguments.reserve( c );
		for( zp_int i = 0; i < c; ++i )
		{
			arguments.pushBackEmpty() = args[ i + 1 ];
		}
	}

	if( arguments.size() < 2 )
	{
		zpLog::message()
			<< "Usage: TextureCompiler.exe "
			<< zpLog::gray << "DX9/DX10/DX11/GL2 "
			<< zpLog::gray << "[R8|R16|R32|RGB8|RGBA8] "
			<< zpLog::gray << "path/to/inputfile.png path/to/outputfile.textureb"
			<< zpLog::endl
			;
	}
	else
	{
		BaseTextureCompiler* compiler = ZP_NULL;

		const zpString& input = arguments[  arguments.size() - 2 ];

		if( input.endsWith( ".png" ) )
		{
			compiler = new PNGTextureCompiler;
		}

		if( compiler != ZP_NULL )
		{
			if( compiler->initialize( arguments ) )
			{
				compiler->compile();
				compiler->shutdown();
			}
			else
			{
				zpLog::error() << "Failed to initialize compiler." << zpLog::endl;
			}
		}
		else
		{
			zpLog::error() << "Texture type '" << input << "' undefined." << zpLog::endl;
		}

		ZP_SAFE_DELETE( compiler );
	}

	return 0;
}
