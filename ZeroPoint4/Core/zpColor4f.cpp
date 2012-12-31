#include "zpCore.h"

zpColor4f::zpColor4f() : m_red( 0 ), m_green( 0 ), m_blue( 0 ), m_alpha( 1 ) {}
zpColor4f::zpColor4f( zp_float* rgba ) : m_red( rgba[0] ), m_green( rgba[1] ), m_blue( rgba[2] ), m_alpha( rgba[3] ) {}
zpColor4f::zpColor4f( zp_float r, zp_float g, zp_float b, zp_float a ) : m_red( r ), m_green( g ), m_blue( b ), m_alpha( a ) {}
zpColor4f::zpColor4f( const zpColor4f& color ) : m_red( color.m_red ), m_green( color.m_green ), m_blue( color.m_blue ), m_alpha( color.m_alpha ) {}
zpColor4f::zpColor4f( zpColor4f&& color ) : m_red( color.m_red ), m_green( color.m_green ), m_blue( color.m_blue ), m_alpha( color.m_alpha ) {}
zpColor4f::~zpColor4f() {}

void zpColor4f::operator=( const zpColor4f& color ) {
	m_red = color.m_red;
	m_green = color.m_green;
	m_blue = color.m_blue;
	m_alpha = color.m_alpha;
}
void zpColor4f::operator=( zpColor4f&& color ) {
	m_red = color.m_red;
	m_green = color.m_green;
	m_blue = color.m_blue;
	m_alpha = color.m_alpha;
}

zpColor4f::operator const zp_float*() const {
	return m_rgba;
}

zpColor4f zpColor4f::operator+( const zpColor4f& color ) const {
	return zpColor4f( m_red + color.m_red, m_green + color.m_green, m_blue + color.m_blue, m_alpha + color.m_alpha );
}
zpColor4f zpColor4f::operator-( const zpColor4f& color ) const {
	return zpColor4f( m_red - color.m_red, m_green - color.m_green, m_blue - color.m_blue, m_alpha - color.m_alpha );
}
zpColor4f zpColor4f::operator*( const zpColor4f& color ) const {
	return zpColor4f( m_red * color.m_red, m_green * color.m_green, m_blue * color.m_blue, m_alpha * color.m_alpha );
}
zpColor4f zpColor4f::operator*( zp_float scale ) const {
	return zpColor4f( m_red * scale, m_green * scale, m_blue * scale, m_alpha * scale );
}
zpColor4f zpColor4f::operator/( zp_float scale ) const {
	const zp_float invScale = 1.f / scale;
	return zpColor4f( m_red * invScale, m_green * invScale, m_blue * invScale, m_alpha * invScale );
}

zp_float zpColor4f::getRed() const {
	return m_red;
}
zp_float zpColor4f::getBlue() const {
	return m_blue;
}
zp_float zpColor4f::getGreen() const {
	return m_green;
}
zp_float zpColor4f::getAlpha() const {
	return m_alpha;
}

void zpColor4f::setRed( zp_float r ) {
	m_red = r;
}
void zpColor4f::setGreen( zp_float g ) {
	m_green = g;
}
void zpColor4f::setBlue( zp_float b ) {
	m_blue = b;
}
void zpColor4f::setAlpha( zp_float a ) {
	m_alpha = a;
}
void zpColor4f::set( zp_float r, zp_float g, zp_float b ) {
	m_red = r;
	m_green = g;
	m_blue = b;
}
void zpColor4f::set( zp_float r, zp_float g, zp_float b, zp_float a ) {
	m_red = r;
	m_green = g;
	m_blue = b;
	m_alpha = a;
}

zpColor4f zpColor4f::lerp( const zpColor4f& color, zp_float alpha ) const {
	alpha = zp_saturate( alpha );
	return (*this) + ( color - (*this) ) * alpha;
}
void zpColor4f::clamp() {
	m_red = zp_saturate( m_red );
	m_green = zp_saturate( m_green );
	m_blue = zp_saturate( m_blue );
	m_alpha = zp_saturate( m_alpha );
}

void zpColor4f::store3( zp_float* rgb ) const {
	rgb[0] = m_red;
	rgb[1] = m_green;
	rgb[2] = m_blue;
}
void zpColor4f::store4( zp_float* rgba ) const {
	rgba[0] = m_red;
	rgba[1] = m_green;
	rgba[2] = m_blue;
	rgba[3] = m_alpha;
}

zp_bool zpColor4f::operator==( const zpColor4f& color ) const {
	return m_red == color.m_red && m_green == color.m_green && m_blue == color.m_blue && m_alpha == color.m_alpha;
}
zp_bool zpColor4f::operator!=( const zpColor4f& color ) const {
	return !( m_red == color.m_red && m_green == color.m_green && m_blue == color.m_blue && m_alpha == color.m_alpha );
}