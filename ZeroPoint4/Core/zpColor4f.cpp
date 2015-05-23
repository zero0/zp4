#include "zpCore.h"

zpColor4f::zpColor4f()
	: m_red( 0 )
	, m_green( 0 )
	, m_blue( 0 )
	, m_alpha( 1 )
{}
zpColor4f::zpColor4f( zp_float* rgba )
	: m_red( rgba[0] )
	, m_green( rgba[1] )
	, m_blue( rgba[2] )
	, m_alpha( rgba[3] )
{}
zpColor4f::zpColor4f( zp_float r, zp_float g, zp_float b, zp_float a )
	: m_red( r )
	, m_green( g )
	, m_blue( b )
	, m_alpha( a )
{}
zpColor4f::zpColor4f( const zpColor4f& color )
	: m_red( color.m_red )
	, m_green( color.m_green )
	, m_blue( color.m_blue )
	, m_alpha( color.m_alpha )
{}
zpColor4f::zpColor4f( zpColor4f&& color )
	: m_red( color.m_red )
	, m_green( color.m_green )
	, m_blue( color.m_blue )
	, m_alpha( color.m_alpha )
{}
zpColor4f::~zpColor4f() {}

void zpColor4f::operator=( const zpColor4f& color )
{
	m_red = color.m_red;
	m_green = color.m_green;
	m_blue = color.m_blue;
	m_alpha = color.m_alpha;
}
void zpColor4f::operator=( zpColor4f&& color )
{
	m_red = color.m_red;
	m_green = color.m_green;
	m_blue = color.m_blue;
	m_alpha = color.m_alpha;
}

const zp_float* zpColor4f::asFloat4() const
{
	return m_rgba;
}

zpColor4f zpColor4f::operator+( const zpColor4f& color ) const
{
	return zpColor4f( m_red + color.m_red, m_green + color.m_green, m_blue + color.m_blue, m_alpha + color.m_alpha );
}
zpColor4f zpColor4f::operator-( const zpColor4f& color ) const
{
	return zpColor4f( m_red - color.m_red, m_green - color.m_green, m_blue - color.m_blue, m_alpha - color.m_alpha );
}
zpColor4f zpColor4f::operator*( const zpColor4f& color ) const
{
	return zpColor4f( m_red * color.m_red, m_green * color.m_green, m_blue * color.m_blue, m_alpha * color.m_alpha );
}
zpColor4f zpColor4f::operator*( zp_float scale ) const
{
	return zpColor4f( m_red * scale, m_green * scale, m_blue * scale, m_alpha * scale );
}
zpColor4f zpColor4f::operator/( zp_float scale ) const
{
	const zp_float invScale = 1.f / scale;
	return zpColor4f( m_red * invScale, m_green * invScale, m_blue * invScale, m_alpha * invScale );
}

zp_float zpColor4f::getRed() const
{
	return m_red;
}
zp_float zpColor4f::getBlue() const
{
	return m_blue;
}
zp_float zpColor4f::getGreen() const
{
	return m_green;
}
zp_float zpColor4f::getAlpha() const
{
	return m_alpha;
}

void zpColor4f::setRed( zp_float r )
{
	m_red = r;
}
void zpColor4f::setGreen( zp_float g )
{
	m_green = g;
}
void zpColor4f::setBlue( zp_float b )
{
	m_blue = b;
}
void zpColor4f::setAlpha( zp_float a )
{
	m_alpha = a;
}
void zpColor4f::set( zp_float r, zp_float g, zp_float b )
{
	m_red = r;
	m_green = g;
	m_blue = b;
}
void zpColor4f::set( zp_float r, zp_float g, zp_float b, zp_float a )
{
	m_red = r;
	m_green = g;
	m_blue = b;
	m_alpha = a;
}

zpColor4f zpColor4f::lerp( const zpColor4f& color, zp_float alpha ) const
{
	zp_saturate( alpha, alpha );
	return (*this) + ( color - (*this) ) * alpha;
}
void zpColor4f::clamp()
{
	zp_saturate( m_red, m_red );
	zp_saturate( m_green, m_green );
	zp_saturate( m_blue, m_blue );
	zp_saturate( m_alpha, m_alpha );
}

void zpColor4f::store3( zp_float* rgb ) const
{
	rgb[0] = m_red;
	rgb[1] = m_green;
	rgb[2] = m_blue;
}
void zpColor4f::store4( zp_float* rgba ) const
{
	rgba[0] = m_red;
	rgba[1] = m_green;
	rgba[2] = m_blue;
	rgba[3] = m_alpha;
}

void zpColor4f::load3( const zp_float* rgb )
{
	m_red =   rgb[0];
	m_green = rgb[1];
	m_blue =  rgb[2];
}
void zpColor4f::load4( const zp_float* rgba )
{
	m_red =   rgba[0];
	m_green = rgba[1];
	m_blue =  rgba[2];
	m_alpha = rgba[3];
}

zp_bool zpColor4f::operator==( const zpColor4f& color ) const
{
	return m_red == color.m_red && m_green == color.m_green && m_blue == color.m_blue && m_alpha == color.m_alpha;
}
zp_bool zpColor4f::operator!=( const zpColor4f& color ) const
{
	return !( m_red == color.m_red && m_green == color.m_green && m_blue == color.m_blue && m_alpha == color.m_alpha );
}

void zpColor4f::Lerp( zpColor4f& s, const zpColor4f& a, const zpColor4f& b, const zpScalar& alpha )
{
	s = a.lerp( b, zpMath::AsFloat( alpha ) );
}

zpPackedColor::zpPackedColor()
	: m_argb( 0 )
{}
zpPackedColor::zpPackedColor( zpPackedColorType argb )
	: m_argb( argb )
{}
zpPackedColor::zpPackedColor( const zpColor4f& color )
	: m_argb( 0 )
{
	fromColor4f( color );
}
zpPackedColor::zpPackedColor( const zpPackedColor& color )
	: m_argb( color.m_argb )
{}
zpPackedColor::zpPackedColor( zpPackedColorPartType r, zpPackedColorPartType g, zpPackedColorPartType b )
	: m_argb( ( 0xFF << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b ) )
{}
zpPackedColor::zpPackedColor( zpPackedColorPartType r, zpPackedColorPartType g, zpPackedColorPartType b, zpPackedColorPartType a )
	: m_argb( ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b ) )
{}
zpPackedColor::~zpPackedColor()
{}

zpPackedColorPartType zpPackedColor::getRed() const
{
	return (zpPackedColorPartType)( 0xFF & ( m_argb >> 16 ) );
}
zpPackedColorPartType zpPackedColor::getBlue() const
{
	return (zpPackedColorPartType)( 0xFF & ( m_argb ) );
}
zpPackedColorPartType zpPackedColor::getGreen() const
{
	return (zpPackedColorPartType)( 0xFF & ( m_argb >> 8 ) );
}
zpPackedColorPartType zpPackedColor::getAlpha() const
{
	return (zpPackedColorPartType)( 0xFF & ( m_argb >> 24 ) );
}

void zpPackedColor::setRed( zpPackedColorPartType r )
{
	m_argb &= 0xFF00FFFF;
	m_argb |= ( r << 16 );
}
void zpPackedColor::setGreen( zpPackedColorPartType g )
{
	m_argb &= 0xFFFF00FF;
	m_argb |= ( g << 8 );
}
void zpPackedColor::setBlue( zpPackedColorPartType b )
{
	m_argb &= 0xFFFFFF00;
	m_argb |= ( b );
}
void zpPackedColor::setAlpha( zpPackedColorPartType a )
{
	m_argb &= 0x00FFFFFF;
	m_argb |= ( a << 24 );
}
void zpPackedColor::set( zpPackedColorPartType r, zpPackedColorPartType g, zpPackedColorPartType b )
{
	m_argb &= 0xFF000000;
	m_argb |= ( r << 16 ) | ( g << 8 ) | ( b );
}
void zpPackedColor::set( zpPackedColorPartType r, zpPackedColorPartType g, zpPackedColorPartType b, zpPackedColorPartType a )
{
	m_argb = ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b );
}

zpPackedColorType zpPackedColor::getPackedColor() const
{
	return m_argb;
}
void zpPackedColor::toColor4f( zpColor4f& outColor ) const
{
	outColor.set(
		(zp_float)getRed() / 255.0f,
		(zp_float)getGreen() / 255.0f,
		(zp_float)getBlue() / 255.0f,
		(zp_float)getAlpha() / 255.0f
	);
}
void zpPackedColor::fromColor4f( const zpColor4f& color )
{
	set(
		(zpPackedColorPartType)( color.getRed() * 255.0f ),
		(zpPackedColorPartType)( color.getGreen() * 255.0f ),
		(zpPackedColorPartType)( color.getBlue() * 255.0f ),
		(zpPackedColorPartType)( color.getAlpha() * 255.0f )
	);
}
