#pragma once
#ifndef ZP_VECTOR2_H
#define ZP_VECTOR2_H

template<typename T>
class zpVector2 {
public:
    zpVector2();
    zpVector2( T x, T y );
    explicit zpVector2( T* xy );
    template<typename F>
    zpVector2( const zpVector2<F>& vector );
    template<typename F>
    zpVector2( zpVector2<F>&& vector );
    ~zpVector2();

    void operator=( T* xy );
    template<typename F>
    void operator=( const zpVector2<F>& vector );
    template<typename F>
    void operator=( zpVector2<F>&& vector );

    zpVector2<T> operator-() const;

    template<typename F>
    zpVector2<T> operator+( const zpVector2<F>& vector ) const;
    template<typename F>
    zpVector2<T> operator-( const zpVector2<F>& vector ) const;
    template<typename F>
    zpVector2<T> operator*( const zpVector2<F>& vector ) const;
    template<typename F>
    zpVector2<T> operator/( const zpVector2<F>& vector ) const;

    zpVector2<T> operator*( zp_float scale ) const;
    zpVector2<T> operator/( zp_float scale ) const;

    template<typename F>
    void operator+=( const zpVector2<F>& vector );
    template<typename F>
    void operator-=( const zpVector2<F>& vector );
    template<typename F>
    void operator*=( const zpVector2<F>& vector );
    template<typename F>
    void operator/=( const zpVector2<F>& vector );

    void operator*=( zp_float scale );
    void operator/=( zp_float scale );

    void setX( T x );
    T getX() const;

    void setY( T y );
    T getY() const;

    void set( T x, T y );

    void zero();
    zp_bool isZero() const;

    template<typename F>
    zp_float dot( const zpVector2<F>& vector ) const;
    template<typename F>
    zp_float cross( const zpVector2<F>& vector ) const;

    zp_float magnitudeSquared() const;
    zp_float magnitude() const;
    void normalize();

    template<typename F>
    zp_float distanceToSquared( const zpVector2<F>& vector ) const;
    template<typename F>
    zp_float distanceTo( const zpVector2<F>& vector ) const;

    template<typename F>
    zpVector2<T> reflect( const zpVector2<F>& normal ) const;
    template<typename F>
    zpVector2<T> refract( const zpVector2<F>& normal, zp_float indexOfRefractionIncoming, zp_float indexOfRefractionTransmit ) const;
    template<typename F>
    zpVector2<T> faceForward( const zpVector2<F>& normal ) const;
    template<typename F>
    zp_float angleBetween( const zpVector2<F>& normal, zp_bool inRadians = true ) const;

    template<typename F>
    zpVector2<T> lerp(  const zpVector2<F>& vector, zp_float alpha ) const;
    template<typename F>
    zpVector2<T> slerp(  const zpVector2<F>& vector, zp_float alpha ) const;

    zpVector2<T> rotateLeft() const;
    zpVector2<T> rotateRight() const;
    zpVector2<T> rotate( zp_float radians ) const;
    zpVector2<T> rotateDeg( zp_float degrees ) const;
    zp_float angle() const;

    template<typename F>
    zp_bool operator==( const zpVector2<F>& vector ) const;
    template<typename F>
    zp_bool operator!=( const zpVector2<F>& vector ) const;

    zpVector4f asVector4( T z = (T)0, T w = (T)1 ) const;

private:
    union {
        T m_xy[2];
        struct {
            T m_x, m_y;
        };
    };
};

#include "zpVector2.inl"

typedef zpVector2<zp_float> zpVector2f;
typedef zpVector2<zp_int> zpVector2i;

#endif