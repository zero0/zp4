#pragma once
#ifndef ZP_MATRIX4F_H
#define ZP_MATRIX4F_H

ZP_ALIGN16 class zpMatrix4f {
public:
	/*
	ZP_FORCE_INLINE void* operator new( zp_uint size );
	ZP_FORCE_INLINE void* operator new[]( zp_uint size );
	ZP_FORCE_INLINE void operator delete( void* ptr );
	*/
	ZP_FORCE_INLINE zpMatrix4f();
	ZP_FORCE_INLINE zpMatrix4f( const zp_float* matrix );
	ZP_FORCE_INLINE zpMatrix4f( const zp_vec4* matrix );
	ZP_FORCE_INLINE zpMatrix4f( const zpMatrix4f& matrix );
	ZP_FORCE_INLINE zpMatrix4f( zpMatrix4f&& matrix );
	ZP_FORCE_INLINE ~zpMatrix4f();

	ZP_FORCE_INLINE void operator=( const zpMatrix4f& matrix );
	ZP_FORCE_INLINE void operator=( zpMatrix4f&& matrix );
	ZP_FORCE_INLINE void operator=( const zp_float* matrix );

	ZP_FORCE_INLINE zp_float& operator()( zp_uint row, zp_uint col );
	ZP_FORCE_INLINE zp_float operator()( zp_uint row, zp_uint col ) const;

	ZP_FORCE_INLINE operator zp_float*();
	ZP_FORCE_INLINE operator const zp_float*() const;

	ZP_FORCE_INLINE void operator*=( const zpMatrix4f& matrix );

	ZP_FORCE_INLINE zpMatrix4f operator*( const zpMatrix4f& matrix ) const;

	ZP_FORCE_INLINE void getPosition( zpVector4f* position ) const;
	//ZP_FORCE_INLINE void getRotation( zpVector4f* rotation ) const;
	//ZP_FORCE_INLINE void getScale( zpVector4f* rotation ) const;

	ZP_FORCE_INLINE void setPosition( const zpVector4f& position );

	ZP_FORCE_INLINE void translate( const zpVector4f& position );
	ZP_FORCE_INLINE void rotate( const zpVector4f& axis, zp_float angle );
	ZP_FORCE_INLINE void rotateX( zp_real angle );
	ZP_FORCE_INLINE void rotateY( zp_real angle );
	ZP_FORCE_INLINE void rotateZ( zp_real angle );
	ZP_FORCE_INLINE void scale( zp_float uniformScale );
	ZP_FORCE_INLINE void scale( const zpVector4f& scale );

	ZP_FORCE_INLINE zp_bool isIdentity() const;

	ZP_FORCE_INLINE void mul( const zpVector4f& vector, zpVector4f* outVector ) const;

	ZP_FORCE_INLINE void lookAt( const zpVector4f& eye, const zpVector4f& at, const zpVector4f& up );
	ZP_FORCE_INLINE void lookTo( const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up );
	ZP_FORCE_INLINE void perspective( zp_float fovy, zp_float aspect, zp_float nearDistance, zp_float farDistance );
	ZP_FORCE_INLINE void ortho( zp_float width, zp_float height, zp_float nearDistance, zp_float farDistance );
	ZP_FORCE_INLINE void orthoOffset( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance );
	ZP_FORCE_INLINE void frustum( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance );

	ZP_FORCE_INLINE void transpose();

	ZP_FORCE_INLINE zp_bool operator==( const zpMatrix4f& matrix ) const;
	ZP_FORCE_INLINE zp_bool operator!=( const zpMatrix4f& matrix ) const;

	static const zpMatrix4f identity;

private:
	union {
		zp_float m_data[16];
		zp_float m_matrix[4][4];

		struct {
			zp_float m_11, m_12, m_13, m_14;
			zp_float m_21, m_22, m_23, m_24;
			zp_float m_31, m_32, m_33, m_34;
			zp_float m_41, m_42, m_43, m_44;
		};

		struct {
			zp_vec4 m_m1;
			zp_vec4 m_m2;
			zp_vec4 m_m3;
			zp_vec4 m_m4;
		};
	};
};

#endif