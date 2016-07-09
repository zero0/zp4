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

    const zp_float* asFloat4() const;

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

    void store3( zp_float* rgb ) const;
    void store4( zp_float* rgba ) const;

    void load3( const zp_float* rgb );
    void load4( const zp_float* rgba );

    zp_bool operator==( const zpColor4f& color ) const;
    zp_bool operator!=( const zpColor4f& color ) const;

    static void Lerp( zpColor4f& s, const zpColor4f& a, const zpColor4f& b, const zpScalar& alpha );

private:
    union {
        struct {
            zp_float m_red, m_green, m_blue, m_alpha;
        };
        zp_float m_rgba[4];
    };
};

typedef zp_dword zpPackedColorType;
typedef zp_byte  zpPackedColorPartType;

class zpPackedColor
{
public:
    zpPackedColor();
    explicit zpPackedColor( zpPackedColorType argb );
    explicit zpPackedColor( const zpColor4f& color );
    zpPackedColor( zpPackedColorPartType r, zpPackedColorPartType g, zpPackedColorPartType b );
    zpPackedColor( zpPackedColorPartType r, zpPackedColorPartType g, zpPackedColorPartType b, zpPackedColorPartType a );
    zpPackedColor( const zpPackedColor& color );
    ~zpPackedColor();

    zpPackedColorPartType getRed() const;
    zpPackedColorPartType getBlue() const;
    zpPackedColorPartType getGreen() const;
    zpPackedColorPartType getAlpha() const;

    void setRed( zpPackedColorPartType r );
    void setGreen( zpPackedColorPartType g );
    void setBlue( zpPackedColorPartType b );
    void setAlpha( zpPackedColorPartType a );
    void set( zpPackedColorPartType r, zpPackedColorPartType g, zpPackedColorPartType b );
    void set( zpPackedColorPartType r, zpPackedColorPartType g, zpPackedColorPartType b, zpPackedColorPartType a );

    zpPackedColorType getPackedColor() const;
    void toColor4f( zpColor4f& outColor ) const;
    void fromColor4f( const zpColor4f& color );

private:
    zpPackedColorType m_argb;
};

#endif