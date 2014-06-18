#include "Main.h"

#define COMPRESSED_BLOCK_STRIDE		4
#define COMPRESSED_BLOCK_SIZE		( COMPRESSED_BLOCK_STRIDE * COMPRESSED_BLOCK_STRIDE )

struct BC1Block
{
	zp_ushort color_0;
	zp_ushort color_1;
	zp_byte data[4];
};

struct BC3Block
{
	zp_byte alpha_0;
	zp_byte alpha_1;
	zp_ulong alpha_indeces;

	zp_ushort color_0;
	zp_ushort color_1;
	zp_byte data[4];
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
zpVector4f MakeColorRGBA( const zp_byte* rgba )
{
	zpVector4f c( (zp_float)rgba[0] / 255.f, (zp_float)rgba[1] / 255.f, (zp_float)rgba[2] / 255.f, (zp_float)rgba[3] / 255.f );
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

zp_byte ScalarTo8( const zpScalar& a )
{
	zp_float d;
	zp_clamp( d, a.getFloat(), 0.f, 1.f );

	return (zp_byte)zp_floor_to_int( d * 255.f );
}

zp_float DistanceAlpha( const zpScalar& a, const zpScalar& b )
{
	zpScalar d;

	zpMath::Sub( d, a, b );
	zpMath::Mul( d, d, d );

	return d.getFloat();
}

void DXTTextureCompressor::compress( const ImageData& inputImage, ImageData& compiledImage )
{
	switch( inputImage.compression )
	{
	case TEXTURE_COMPRESSION_BC1:
		compressBC1( inputImage, compiledImage );
		break;
	case TEXTURE_COMPRESSION_BC3:
		compressBC3( inputImage, compiledImage );
		break;
	default:
		ZP_ASSERT( false, "DXTTextureCompressor unknown format" );
		break;
	}
}

void DXTTextureCompressor::compressBC1( const ImageData& inputImage, ImageData& compiledImage )
{
	compiledImage.width = inputImage.width;
	compiledImage.height = inputImage.height;

	zp_uint wmod = compiledImage.width % COMPRESSED_BLOCK_STRIDE;
	zp_uint hmod = compiledImage.height % COMPRESSED_BLOCK_STRIDE;

	if( wmod != 0 ) compiledImage.width += COMPRESSED_BLOCK_STRIDE - wmod;
	if( hmod != 0 ) compiledImage.height += COMPRESSED_BLOCK_STRIDE - hmod;

	zp_bool hasAlpha = inputImage.format == TEXTURE_FORMAT_RGBA;
	compiledImage.format = inputImage.format;
	compiledImage.compression = TEXTURE_COMPRESSION_BC1;

	zp_uint stride = inputImage.stride;
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
			zp_bool isTransparent[ COMPRESSED_BLOCK_SIZE ];
			zp_bool hasTransparentPixel = false;

			color0 = zpVector4f( 1, 1, 1, 1 );
			color1 = zpVector4f( 0, 0, 0, 1 );

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
						if( hasAlpha )
						{
							c = MakeColorRGBA( &image[ stride * ( ( x + px ) + ( ( y + py ) * inputImage.width ) ) ] );
							
							zp_bool transparent = c.getW().getFloat() < 1.f;
							isTransparent[ px + ( py * COMPRESSED_BLOCK_STRIDE ) ] = transparent;
							hasTransparentPixel |= transparent;
						}
						else
						{
							c = MakeColorRGB( &image[ stride * ( ( x + px ) + ( ( y + py ) * inputImage.width ) ) ] );
						}

						// find min color
						if( Vector4To565( c ) < Vector4To565( color0 ) )
						{
							color0 = c;
						}
						// find max color
						if( Vector4To565( c ) > Vector4To565( color1 ) )
						{
							color1 = c;
						}
					}
				}
			}

			if( hasTransparentPixel )
			{
				zpMath::Lerp( color2, color0, color1, zpScalar( 1.f / 2.f ) );
				color3 = zpVector4f( 0, 0, 0, 0 );
			}
			else
			{
				zpMath::Lerp( color2, color0, color1, zpScalar( 1.f / 3.f ) );
				zpMath::Lerp( color3, color0, color1, zpScalar( 2.f / 3.f ) );
			}

			BC1Block block;
			block.color_0 = Vector4To565( color0 );
			block.color_1 = Vector4To565( color1 );
			block.data[0] = 0;
			block.data[1] = 0;
			block.data[2] = 0;
			block.data[3] = 0;

			if( !hasTransparentPixel )
			{
				zp_move_swap( block.color_0, block.color_1 );
				zp_move_swap( color0, color1 );
			}

			zpVector4f allColors[4] =
			{
				color0,
				color1,
				color2,
				color3,
			};

			for( zp_uint y = 0; y < COMPRESSED_BLOCK_STRIDE; ++y )
			{
				block.data[y] = 0;
				//for( zp_int x = COMPRESSED_BLOCK_STRIDE - 1; x >= 0; --x )
				for( zp_int x = 0; x <  COMPRESSED_BLOCK_STRIDE; ++x )
				{
					const zpVector4f& c = pixels[ x + ( y * COMPRESSED_BLOCK_STRIDE ) ];

					zp_byte index = 0;
					zp_float dist = ZP_FLT_MAX;

					if( hasTransparentPixel )
					{
						if( isTransparent[ x + ( y * COMPRESSED_BLOCK_STRIDE ) ] )
						{
							index = 3;
						}
						else
						{
							// find the distance to all colors
							for( zp_uint i = 0; i < 3; ++i )
							{
								zp_float d = DistanceColor( c, allColors[ i ] );

								// if the difference is 0, break
								if( zp_approximate( d, 0.f ) )
								{
									index = i;
									break;
								}

								// otherwise, use the smallest distance index
								if( d < dist )
								{
									index = i;
									dist = d;
								}
							}
						}
					}
					else
					{
						// find the distance to all colors
						for( zp_uint i = 0; i < 4; ++i )
						{
							zp_float d = DistanceColor( c, allColors[ i ] );

							// if the difference is 0, break
							if( zp_approximate( d, 0.f ) )
							{
								index = i;
								break;
							}

							// otherwise, use the smallest distance index
							if( d < dist )
							{
								index = i;
								dist = d;
							}
						}
					}

					block.data[y] = ( block.data[y] ) | ( ( 0x03 & index ) << x * 2 );
				}
			}

			// write block to compiled image data
			compiledImage.imageBytes.write( block.color_0 );
			compiledImage.imageBytes.write( block.color_1 );
			compiledImage.imageBytes.write( block.data[0] );
			compiledImage.imageBytes.write( block.data[1] );
			compiledImage.imageBytes.write( block.data[2] );
			compiledImage.imageBytes.write( block.data[3] );
		}
	}
}

void DXTTextureCompressor::compressBC2( const ImageData& inputImage, ImageData& compiledImage )
{

}

void DXTTextureCompressor::compressBC3( const ImageData& inputImage, ImageData& compiledImage )
{
	compiledImage.width = inputImage.width;
	compiledImage.height = inputImage.height;

	zp_uint wmod = compiledImage.width % COMPRESSED_BLOCK_STRIDE;
	zp_uint hmod = compiledImage.height % COMPRESSED_BLOCK_STRIDE;

	if( wmod != 0 ) compiledImage.width += COMPRESSED_BLOCK_STRIDE - wmod;
	if( hmod != 0 ) compiledImage.height += COMPRESSED_BLOCK_STRIDE - hmod;

	zp_bool hasAlpha = inputImage.format == TEXTURE_FORMAT_RGBA;
	compiledImage.format = inputImage.format;
	compiledImage.compression = TEXTURE_COMPRESSION_BC3;

	zp_uint stride = inputImage.stride;
	const zp_byte* image = inputImage.imageBytes.getData();

	zp_uint cw = ( inputImage.width + 3 ) / 4;
	zp_uint ch = ( inputImage.height + 3 ) / 4;
	zp_uint compressedSize = ( cw * ch ) * sizeof( zp_ulong ) * 2;
	compiledImage.imageBytes.reserve( compressedSize );

	for( zp_uint y = 0; y < compiledImage.height; y += COMPRESSED_BLOCK_STRIDE )
	{
		for( zp_uint x = 0; x < compiledImage.width; x += COMPRESSED_BLOCK_STRIDE )
		{
			zpVector4f color0, color1, color2, color3;
			zpScalar alpha0, alpha1, alpha2, alpha3, alpha4, alpha5, alpha6, alpha7;

			zpVector4f pixels[ COMPRESSED_BLOCK_SIZE ];

			color0 = zpVector4f( 1, 1, 1, 1 );
			color1 = zpVector4f( 0, 0, 0, 1 );

			alpha0 = zpScalar( 1 );
			alpha1 = zpScalar( 0 );

			// find min and max color
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
						if( hasAlpha )
						{
							c = MakeColorRGBA( &image[ stride * ( ( x + px ) + ( ( y + py ) * inputImage.width ) ) ] );
						}
						else
						{
							c = MakeColorRGB( &image[ stride * ( ( x + px ) + ( ( y + py ) * inputImage.width ) ) ] );
						}

						// find min color
						if( Vector4To565( c ) < Vector4To565( color0 ) )
						{
							color0 = c;
						}
						// find max color
						if( Vector4To565( c ) > Vector4To565( color1 ) )
						{
							color1 = c;
						}

						// find min alpha
						if( c.getW().getFloat() < alpha0.getFloat() )
						{
							alpha0 = c.getW();
						}
						// find max alpha
						if( c.getW().getFloat() > alpha1.getFloat() )
						{
							alpha1 = c.getW();
						}
					}
				}
			}

			zpMath::Lerp( color2, color0, color1, zpScalar( 1.f / 3.f ) );
			zpMath::Lerp( color3, color0, color1, zpScalar( 2.f / 3.f ) );

			BC3Block block;
			block.alpha_0 = ScalarTo8( alpha0 );
			block.alpha_1 = ScalarTo8( alpha1 );
			block.alpha_indeces = 0;

			block.color_0 = Vector4To565( color0 );
			block.color_1 = Vector4To565( color1 );
			block.data[0] = 0;
			block.data[1] = 0;
			block.data[2] = 0;
			block.data[3] = 0;

			//zp_move_swap( block.color_0, block.color_1 );

			zpVector4f allColors[4] =
			{
				color0,
				color1,
				color2,
				color3,
			};

			// compress colors
			for( zp_uint y = 0; y < COMPRESSED_BLOCK_STRIDE; ++y )
			{
				block.data[y] = 0;
				//for( zp_int x = COMPRESSED_BLOCK_STRIDE - 1; x >= 0; --x )
				for( zp_int x = 0; x < COMPRESSED_BLOCK_STRIDE; ++x )
				{
					const zpVector4f& c = pixels[ x + ( y * COMPRESSED_BLOCK_STRIDE ) ];

					zp_byte index = 0;
					zp_float dist = ZP_FLT_MAX;

					// find the distance to all colors
					for( zp_uint i = 0; i < 4; ++i )
					{
						zp_float d = DistanceColor( c, allColors[ i ] );

						// if the difference is 0, break
						if( zp_approximate( d, 0.f ) )
						{
							index = i;
							break;
						}

						// otherwise, use the smallest distance index
						if( d < dist )
						{
							index = i;
							dist = d;
						}
					}

					block.data[y] = ( block.data[y] ) | ( ( 0x03 & index ) << 2 * x );
				}
			}

			// if zero or full alpha is in the block, use the 1/4 lerp
			if( block.alpha_0 == 0 || block.alpha_1 == 255 )
			{
				zp_move_swap( block.alpha_0, block.alpha_1 );
			
				zpMath::Lerp( alpha2, alpha0, alpha1, zpScalar( 1.f / 5.f ) );
				zpMath::Lerp( alpha3, alpha0, alpha1, zpScalar( 2.f / 5.f ) );
				zpMath::Lerp( alpha4, alpha0, alpha1, zpScalar( 3.f / 5.f ) );
				zpMath::Lerp( alpha5, alpha0, alpha1, zpScalar( 4.f / 5.f ) );
				alpha6 = alpha0;
				alpha7 = alpha1;
			}
			// otherwise, use 1/7 lerp
			else
			{
			
				zpMath::Lerp( alpha2, alpha0, alpha1, zpScalar( 1.f / 7.f ) );
				zpMath::Lerp( alpha3, alpha0, alpha1, zpScalar( 2.f / 7.f ) );
				zpMath::Lerp( alpha4, alpha0, alpha1, zpScalar( 3.f / 7.f ) );
				zpMath::Lerp( alpha5, alpha0, alpha1, zpScalar( 4.f / 7.f ) );
				zpMath::Lerp( alpha6, alpha0, alpha1, zpScalar( 5.f / 7.f ) );
				zpMath::Lerp( alpha7, alpha0, alpha1, zpScalar( 6.f / 7.f ) );
			}

			zpScalar allAlphas[8] =
			{
				alpha0,
				alpha1,
				alpha2,
				alpha3,
				alpha4,
				alpha5,
				alpha6,
				alpha7,
			};

			// compress alphas
			for( zp_int j = 0; j < COMPRESSED_BLOCK_SIZE; ++j )
			{
				const zpScalar c = pixels[ j ].getW();

				zp_byte index = 0;
				zp_float dist = ZP_FLT_MAX;

				// find the distance to all colors
				for( zp_int i = 0; i < 8; ++i )
				{
					zp_float d = DistanceAlpha( c, allAlphas[ i ] );

					//// if the difference is 0, break
					//if( zp_approximate( d, 0.f ) )
					//{
					//	index = i;
					//	break;
					//}

					// otherwise, use the smallest distance index
					if( d < dist )
					{
						index = i;
						dist = d;
					}
				}

				block.alpha_indeces = ( block.alpha_indeces ) | ( ( 0x07L & (zp_ulong)index ) << 3 * j );
			}

			// write block to compiled image data
			zp_ulong alpha_data = ( (zp_ulong)block.alpha_1 ) | ( (zp_ulong)block.alpha_0 << 8 ) | ( 0x0000FFFFFFL & block.alpha_indeces ) << 16;

			compiledImage.imageBytes.write( alpha_data );
			compiledImage.imageBytes.write( block.color_0 );
			compiledImage.imageBytes.write( block.color_1 );
			compiledImage.imageBytes.write( block.data[0] );
			compiledImage.imageBytes.write( block.data[1] );
			compiledImage.imageBytes.write( block.data[2] );
			compiledImage.imageBytes.write( block.data[3] );
		}
	}
}

void DXTTextureCompressor::compressBC4( const ImageData& inputImage, ImageData& compiledImage )
{

}

void DXTTextureCompressor::compressBC5( const ImageData& inputImage, ImageData& compiledImage )
{

}
