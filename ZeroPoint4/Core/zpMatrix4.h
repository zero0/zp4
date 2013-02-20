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
	ZP_FORCE_INLINE zpMatrix4f( const zpMatrix4f& matrix );
	ZP_FORCE_INLINE zpMatrix4f( zpMatrix4f&& matrix );
	ZP_FORCE_INLINE ~zpMatrix4f();

	ZP_FORCE_INLINE void operator=( const zpMatrix4f& matrix );
	ZP_FORCE_INLINE void operator=( zpMatrix4f&& matrix );

	ZP_FORCE_INLINE const zpVector4f& getRow( zp_uint row ) const;
	ZP_FORCE_INLINE void setRow( zp_uint row, const zpVector4f& v );

	ZP_FORCE_INLINE void setIdentity();

	//ZP_FORCE_INLINE zp_float& operator()( zp_uint row, zp_uint col );
	//ZP_FORCE_INLINE zp_float operator()( zp_uint row, zp_uint col ) const;
	//
	//ZP_FORCE_INLINE operator zp_float*();
	//ZP_FORCE_INLINE operator const zp_float*() const;
	//
	//ZP_FORCE_INLINE void getPosition( zpVector4f& position ) const;
	////ZP_FORCE_INLINE void getRotation( zpVector4f* rotation ) const;
	////ZP_FORCE_INLINE void getScale( zpVector4f* rotation ) const;
	//
	//ZP_FORCE_INLINE void setPosition( const zpVector4f& position );
	//
	//ZP_FORCE_INLINE void translate( const zpVector4f& position );
	//ZP_FORCE_INLINE void rotate( const zpVector4f& axis, zp_float angle );
	//ZP_FORCE_INLINE void rotateX( const zpScalar& angle );
	//ZP_FORCE_INLINE void rotateY( const zpScalar& angle );
	//ZP_FORCE_INLINE void rotateZ( const zpScalar& angle );
	//ZP_FORCE_INLINE void scale( const zpScalar& uniformScale );
	//ZP_FORCE_INLINE void scale( const zpVector4f& scale );
	//
	//ZP_FORCE_INLINE zp_bool isIdentity() const;
	//
	//ZP_FORCE_INLINE void mul( const zpVector4f& vector, zpVector4f& outVector ) const;
	//ZP_FORCE_INLINE void mul( const zpMatrix4f& matrix, zpMatrix4f& outMatrix ) const;
	//
	//ZP_FORCE_INLINE void lookAt( const zpVector4f& eye, const zpVector4f& at, const zpVector4f& up );
	//ZP_FORCE_INLINE void lookTo( const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up );
	//ZP_FORCE_INLINE void perspective( zp_float fovy, zp_float aspect, zp_float nearDistance, zp_float farDistance );
	//ZP_FORCE_INLINE void ortho( zp_float width, zp_float height, zp_float nearDistance, zp_float farDistance );
	//ZP_FORCE_INLINE void orthoOffset( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance );
	//ZP_FORCE_INLINE void frustum( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance );
	//
	//ZP_FORCE_INLINE void transpose();
	//
	//ZP_FORCE_INLINE zp_float determinant() const;
	//ZP_FORCE_INLINE void invert();
	//
	//ZP_FORCE_INLINE zp_bool operator==( const zpMatrix4f& matrix ) const;
	//ZP_FORCE_INLINE zp_bool operator!=( const zpMatrix4f& matrix ) const;
	//
	//static const zpMatrix4f identity;

	friend void zpMath::Mul( zpVector4f& s, const zpVector4f& a, const zpMatrix4f& b );
	friend void zpMath::Mul( zpMatrix4f& s, const zpMatrix4f& a, const zpMatrix4f& b );

private:
	union
	{
		zp_float m_data[16];
		zp_float m_matrix[4][4];

		struct
		{
			zp_float m_11, m_12, m_13, m_14;
			zp_float m_21, m_22, m_23, m_24;
			zp_float m_31, m_32, m_33, m_34;
			zp_float m_41, m_42, m_43, m_44;
		};

		struct
		{
			zpVector4f m_m1;
			zpVector4f m_m2;
			zpVector4f m_m3;
			zpVector4f m_m4;
		};

		struct
		{
			zpVector4f m_rows[4];
		};
	};
};

#endif