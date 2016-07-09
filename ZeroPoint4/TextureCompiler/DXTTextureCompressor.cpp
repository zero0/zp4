#include "Main.h"

#define COMPRESSED_BLOCK_STRIDE        4
#define COMPRESSED_BLOCK_SIZE        ( COMPRESSED_BLOCK_STRIDE * COMPRESSED_BLOCK_STRIDE )

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
    i = zp_clamp( i, 0, l );
    return i;
}

zpVector4f MakeColorRGB( const zp_byte* rgb )
{
    zpVector4f c = zpMath::Vector4( (zp_float)rgb[0] / 255.f, (zp_float)rgb[1] / 255.f, (zp_float)rgb[2] / 255.f, 1 );
    return c;
}
zpVector4f MakeColorRGBA( const zp_byte* rgba )
{
    zpVector4f c = zpMath::Vector4( (zp_float)rgba[0] / 255.f, (zp_float)rgba[1] / 255.f, (zp_float)rgba[2] / 255.f, (zp_float)rgba[3] / 255.f );
    return c;
}
zp_ushort Vector4To565( zpVector4fParamF c )
{
    ZP_ALIGN16 zp_float v[4];

    zpVector4f s = zpMath::Vector4Mul( c, zpMath::Vector4( 31.f, 63.f, 31.f, 0.f ) );
    zpMath::Vector4Store4( s, v );

    zp_int cr = FloatToInt( v[0], 31 );
    zp_int cg = FloatToInt( v[1], 63 );
    zp_int cb = FloatToInt( v[2], 31 );

    return ( cr << 11 ) | ( cg << 5 ) | cb;
}

zp_float DistanceColor( zpVector4fParamF a, zpVector4fParamF b )
{
    zpVector4f s;
    zpScalar d;
    
    s = zpMath::Vector4Sub( a, b );
    d = zpMath::Vector4Dot3( s, s );
    return zpMath::AsFloat( d );
}

zp_byte ScalarTo8( zpScalarParamF a )
{
    zp_float d = zpMath::AsFloat( a );
    d = zp_clamp( d, 0.f, 1.f );

    return (zp_byte)zp_floor_to_int( d * 255.f );
}

zp_float DistanceAlpha( zpScalarParamF a, zpScalarParamF b )
{
    zpScalar d;

    d = zpMath::ScalarSub( a, b );
    d = zpMath::ScalarMul( d, d );

    return zpMath::AsFloat( d );
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

            color0 = zpMath::Vector4( 1, 1, 1, 1 );
            color1 = zpMath::Vector4( 0, 0, 0, 1 );

            // find min and max
            for( zp_uint py = 0; py < COMPRESSED_BLOCK_STRIDE; ++py )
            {
                for( zp_uint px = 0; px < COMPRESSED_BLOCK_STRIDE; ++px )
                {
                    zpVector4f& c =  pixels[ px + ( py * COMPRESSED_BLOCK_STRIDE ) ];

                    if( x + px > inputImage.width || y + py > inputImage.height )
                    {
                        c = zpMath::Vector4( 0, 0, 0, 1 );
                    }
                    else
                    {
                        if( hasAlpha )
                        {
                            c = MakeColorRGBA( &image[ stride * ( ( x + px ) + ( ( y + py ) * inputImage.width ) ) ] );
                            
                            zp_bool transparent = zpMath::ScalarCmp( zpMath::Vector4GetW( c ), zpMath::Scalar( 1.f ) ) < 0;
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
                color2 = zpMath::Vector4Lerp( color0, color1, zpMath::Scalar( 1.f / 2.f ) );
                color3 = zpMath::Vector4( 0, 0, 0, 0 );
            }
            else
            {
                color2 = zpMath::Vector4Lerp( color0, color1, zpMath::Scalar( 1.f / 3.f ) );
                color3 = zpMath::Vector4Lerp( color0, color1, zpMath::Scalar( 2.f / 3.f ) );
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

                    block.data[y] = ( block.data[y] ) | ( ( 0x03 & index ) << ( x * 2 ) );
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

            color0 = zpMath::Vector4( 1, 1, 1, 1 );
            color1 = zpMath::Vector4( 0, 0, 0, 1 );

            alpha0 = zpMath::Scalar( 1 );
            alpha1 = zpMath::Scalar( 0 );

            // find min and max color
            for( zp_uint py = 0; py < COMPRESSED_BLOCK_STRIDE; ++py )
            {
                for( zp_uint px = 0; px < COMPRESSED_BLOCK_STRIDE; ++px )
                {
                    zpVector4f& c =  pixels[ px + ( py * COMPRESSED_BLOCK_STRIDE ) ];

                    if( x + px > inputImage.width || y + py > inputImage.height )
                    {
                        c = zpMath::Vector4( 0, 0, 0, 1 );
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
                        if( zpMath::ScalarCmp( zpMath::Vector4GetW( c ), alpha0 ) < 0 )
                        {
                            alpha0 = zpMath::Vector4GetW( c );
                        }
                        // find max alpha
                        if( zpMath::ScalarCmp( zpMath::Vector4GetW( c ), alpha0 ) > 0 )
                        {
                            alpha1 = zpMath::Vector4GetW( c );
                        }
                    }
                }
            }

            color2 = zpMath::Vector4Lerp( color0, color1, zpMath::Scalar( 1.f / 3.f ) );
            color3 = zpMath::Vector4Lerp( color0, color1, zpMath::Scalar( 2.f / 3.f ) );

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
            
                alpha2 = zpMath::ScalarLerp( alpha0, alpha1, zpMath::Scalar( 1.f / 5.f ) );
                alpha3 = zpMath::ScalarLerp( alpha0, alpha1, zpMath::Scalar( 2.f / 5.f ) );
                alpha4 = zpMath::ScalarLerp( alpha0, alpha1, zpMath::Scalar( 3.f / 5.f ) );
                alpha5 = zpMath::ScalarLerp( alpha0, alpha1, zpMath::Scalar( 4.f / 5.f ) );
                alpha6 = alpha0;
                alpha7 = alpha1;
            }
            // otherwise, use 1/7 lerp
            else
            {
            
                alpha2 = zpMath::ScalarLerp( alpha0, alpha1, zpMath::Scalar( 1.f / 7.f ) );
                alpha3 = zpMath::ScalarLerp( alpha0, alpha1, zpMath::Scalar( 2.f / 7.f ) );
                alpha4 = zpMath::ScalarLerp( alpha0, alpha1, zpMath::Scalar( 3.f / 7.f ) );
                alpha5 = zpMath::ScalarLerp( alpha0, alpha1, zpMath::Scalar( 4.f / 7.f ) );
                alpha6 = zpMath::ScalarLerp( alpha0, alpha1, zpMath::Scalar( 5.f / 7.f ) );
                alpha7 = zpMath::ScalarLerp( alpha0, alpha1, zpMath::Scalar( 6.f / 7.f ) );
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
                zpScalar c = zpMath::Vector4GetW( pixels[ j ] );

                zp_byte index = 0;
                zp_float dist = ZP_FLT_MAX;

                // find the distance to all colors
                for( zp_int i = 0; i < 8; ++i )
                {
                    zp_float d = DistanceAlpha( c, allAlphas[ i ] );

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

                block.alpha_indeces = ( block.alpha_indeces ) | ( ( 0x07L & (zp_ulong)index ) << ( 3 * j ) );
            }

            // write block to compiled image data
            zp_ulong alpha_data = ( (zp_ulong)block.alpha_1 ) | ( (zp_ulong)block.alpha_0 << 8 ) | ( block.alpha_indeces << 16 );

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
