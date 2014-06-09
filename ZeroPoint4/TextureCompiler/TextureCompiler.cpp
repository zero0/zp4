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
	union
	{
		struct
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
		zp_long block;
	};
};

zp_int FloatToInt( zp_float a, zp_int l )
{
	zp_int i = (zp_int)( a + 0.5f );
	zp_clamp( i, i, 0, l );
	return i;
}

zpVector4f MakeColorRGB( const zp_byte* rgb )
{
	zpVector4f c( (zp_float)rgb[0] / 255.f, (zp_float)rgb[1] / 255.f, (zp_float)rgb[2] / 255.f, 1 );
	return c;
}
zp_ushort Vector4To565( const zpVector4f& c )
{
	zp_int cr = FloatToInt( 31.0f * c.getX().getFloat(), 31 );
	zp_int cg = FloatToInt( 63.0f * c.getY().getFloat(), 63 );
	zp_int cb = FloatToInt( 31.0f * c.getZ().getFloat(), 31 );

	return ( cr << 11 ) | ( cg << 5 ) | cb;
}

zp_float DistanceColor( const zpVector4f& a, const zpVector4f& b )
{
	zpVector4f s;
	zpScalar d;

	zpMath::Sub( s, a, b );
	zpMath::Dot3( d, s, s );
	return d.getFloat();
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
			zpVector4f color0, color1, color2, color3;
			zpVector4f pixels[ COMPRESSED_BLOCK_SIZE ];

			color0 = zpVector4f( 0, 0, 0, 1 );
			color1 = zpVector4f( 1, 1, 1, 1 );

			// find min and max
			for( zp_uint py = 0; py < COMPRESSED_BLOCK_STRIDE; ++py )
			{
				for( zp_uint px = 0; px < COMPRESSED_BLOCK_STRIDE; ++px )
				{
					zpVector4f& c =  pixels[ px + ( py * COMPRESSED_BLOCK_STRIDE ) ];

					if( x + px > inputImage.width || y + py > inputImage.height )
					{
						c = zpVector4f( 0, 0, 0, 1 );
					}
					else
					{
						c = MakeColorRGB( &image[ stride * ( ( x + px ) + ( ( y + py ) * inputImage.width ) ) ] );

						if( Vector4To565( c ) > Vector4To565( color0 ) )
						{
							color0 = c;
						}
						if( Vector4To565( c ) < Vector4To565( color1 ) )
						{
							color1 = c;
						}
					}
				}
			}

			zpMath::Lerp( color2, color0, color1, zpScalar( 1.f / 3.f ) );
			zpMath::Lerp( color3, color0, color1, zpScalar( 1.f / 3.f ) );

			BC1Block block;
			block.color_0 = Vector4To565( color0 );
			block.color_1 = Vector4To565( color1 );
			block.indices = 0;

			zpVector4f midColor[3];
			zpMath::Lerp( midColor[0], color1, color2, zpScalar( 0.5f ) );
			zpMath::Lerp( midColor[1], color2, color3, zpScalar( 0.5f ) );
			zpMath::Lerp( midColor[2], color3, color0, zpScalar( 0.5f ) );

			zp_ushort mid[3] =
			{
				Vector4To565( midColor[0] ),
				Vector4To565( midColor[1] ),
				Vector4To565( midColor[2] )
			};

			for( zp_uint y = 0; y < COMPRESSED_BLOCK_STRIDE; ++y )
			{
				block.data[y] = 0;
				for( zp_int x = COMPRESSED_BLOCK_STRIDE - 1; x >= 0; --x )
				{
					const zpVector4f& c = pixels[ x + ( y * COMPRESSED_BLOCK_STRIDE ) ];
					zp_ushort p = Vector4To565( c );

					zp_byte index = 0;
					if( p <= mid[0] )
					{
						index = 1;
					}
					else if( p <= mid[1] )
					{
						index = 2;
					}
					else if( p <= mid[2] )
					{
						index = 3;
					}
					else
					{
						index = 0;
					}

					block.data[y] = ( block.data[y] << 2 ) | ( 0x03 & index );
				}
			}
#if 0
			// select diagonal
			zpVector4f center;
			zpMath::Lerp( center, color0, color1, zpScalar( 0.5f ) );
			zpVector4f cov( 0, 0 );
			
			for( zp_uint i = 0; i < COMPRESSED_BLOCK_SIZE; ++i )
			{
				zpVector4f t;
				zpMath::Sub( t, pixels[i], center );

				zpMath::Madd( cov, cov, t.xy(), t.getZ() );
			}
			
			zp_float x0 = color0.getX().getFloat();
			zp_float y0 = color0.getY().getFloat();
			zp_float x1 = color1.getX().getFloat();
			zp_float y1 = color1.getY().getFloat();
			
			if (zpMath::Cmp0( cov.getX() ) < 0) {
				zp_move_swap(x0, x1);
			}
			if (zpMath::Cmp0( cov.getY() ) < 0) {
				zp_move_swap(y0, y1);
			}
			
			color0 = zpVector4f( x0, y0, color0.getZ().getFloat() );
			color1 = zpVector4f( x1, y1, color1.getZ().getFloat() );

			// insert bbox
			zpVector4f th( ( 8.f / 255.f ) / 16.f );
			zpVector4f inset;
			zpMath::Sub( inset, color0, color1 );
			zpMath::Div( inset, inset, zpScalar( 16.f ) );
			zpMath::Sub( inset, inset, th );

			zpMath::Sub( color0, color0, inset );
			zpMath::Add( color1, color1, inset );

			zpMath::Min( color0, color0, zpVector4f( 1, 1, 1, 1 ) );
			zpMath::Max( color0, color0, zpVector4f( 0, 0, 0, 0 ) );

			zpMath::Min( color1, color1, zpVector4f( 1, 1, 1, 1 ) );
			zpMath::Max( color1, color1, zpVector4f( 0, 0, 0, 0 ) );

			// calculate indices
			zp_uint index;
			zpMath::Lerp( color2, color0, color1, zpScalar( 1.f / 3.f ) );
			zpMath::Lerp( color3, color0, color1, zpScalar( 1.f / 3.f ) );

			zpVector4f palette[4] =
			{
				color0,
				color1,
				color2,
				color3
			};

			BC1Block block;
			block.color_0 = Vector4To565( color0 );
			block.color_1 = Vector4To565( color1 );
			block.indices = 0;

			for( zp_uint i = 0; i < COMPRESSED_BLOCK_SIZE; ++i )
			{
				zp_float d0 = DistanceColor( palette[0], pixels[i] );
				zp_float d1 = DistanceColor( palette[1], pixels[i] );
				zp_float d2 = DistanceColor( palette[2], pixels[i] );
				zp_float d3 = DistanceColor( palette[3], pixels[i] );

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
#endif
			compiledImage.imageBytes.write( block.color_0 );
			compiledImage.imageBytes.write( block.color_1 );
			compiledImage.imageBytes.write( block.dcba );
			compiledImage.imageBytes.write( block.hgfe );
			compiledImage.imageBytes.write( block.lkji );
			compiledImage.imageBytes.write( block.ponm );
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

#if 0
	zpStringBuffer sb;
	zpDataBuffer db;

	zp_base64_encode( m_compressedImage.imageBytes.getData(), m_compressedImage.imageBytes.size(), sb );
	zp_base64_decode( sb.str(), sb.length(), db );

	ZP_ASSERT( db.size() == m_compressedImage.imageBytes.size(), "" );
	ZP_ASSERT( zp_memcmp( db.getData(), m_compressedImage.imageBytes.getData(), m_compressedImage.imageBytes.size() ) == 0, "" );
#endif

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
