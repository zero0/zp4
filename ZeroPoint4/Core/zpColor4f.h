#pragma once
#ifndef ZP_COLOR4F_H
#define ZP_COLOR4F_H

class zpColor4f {
public:
	zpColor4f();
	explicit zpColor4f( zp_float* rgba );
	zpColor4f( zp_float r, zp_float g, zp_float b, zp_float a = 1.0f );
	zpColor4f( const zpColor4f& color );
	zpColor4f( zpColor4f&& color );
	~zpColor4f();

	void operator=( const zpColor4f& color );
	void operator=( zpColor4f&& color );

	operator const zp_float*() const;

	zpColor4f operator+( const zpColor4f& color ) const;
	zpColor4f operator-( const zpColor4f& color ) const;
	zpColor4f operator*( const zpColor4f& color ) const;
	zpColor4f operator*( zp_float scale ) const;
	zpColor4f operator/( zp_float scale ) const;

	zp_float getRed() const;
	zp_float getBlue() const;
	zp_float getGreen() const;
	zp_float getAlpha() const;

	void setRed( zp_float r );
	void setGreen( zp_float g );
	void setBlue( zp_float b );
	void setAlpha( zp_float a );
	void set( zp_float r, zp_float g, zp_float b );
	void set( zp_float r, zp_float g, zp_float b, zp_float a );

	zpColor4f lerp( const zpColor4f& color, zp_float alpha ) const;
	void clamp();

	zp_bool operator==( const zpColor4f& color ) const;
	zp_bool operator!=( const zpColor4f& color ) const;

private:
	union {
		struct {
			zp_float m_red, m_green, m_blue, m_alpha;
		};
		zp_float m_rgba[4];
	};
};

#endif