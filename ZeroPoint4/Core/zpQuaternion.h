#pragma once
#ifndef ZP_QUATERNION4F_H
#define ZP_QUATERNION4F_H

typedef zp_vec4 zpQuaternion4f;

#if 0
ZP_ALIGN16 class zpQuaternion4f
{
public:
	ZP_FORCE_INLINE zpQuaternion4f();

	ZP_FORCE_INLINE explicit zpQuaternion4f( zp_float xyzw );
	ZP_FORCE_INLINE zpQuaternion4f( zp_float x, zp_float y );
	ZP_FORCE_INLINE zpQuaternion4f( zp_float x, zp_float y, zp_float z );
	ZP_FORCE_INLINE zpQuaternion4f( zp_float x, zp_float y, zp_float z, zp_float w );

	ZP_FORCE_INLINE explicit zpQuaternion4f( const zpScalar& xyzw );
	ZP_FORCE_INLINE zpQuaternion4f( const zpScalar& x, const zpScalar& y );
	ZP_FORCE_INLINE zpQuaternion4f( const zpScalar& x, const zpScalar& y, const zpScalar& z );
	ZP_FORCE_INLINE zpQuaternion4f( const zpScalar& x, const zpScalar& y, const zpScalar& z, const zpScalar& w );

	ZP_FORCE_INLINE zpQuaternion4f( const zpQuaternion4f& quat );
	ZP_FORCE_INLINE zpQuaternion4f( zpQuaternion4f&& quat );
	ZP_FORCE_INLINE ~zpQuaternion4f();

	ZP_FORCE_INLINE void operator=( const zpQuaternion4f& quat );
	ZP_FORCE_INLINE void operator=( zpQuaternion4f&& quat );

	ZP_FORCE_INLINE zpScalar getX() const;
	ZP_FORCE_INLINE zpScalar getY() const;
	ZP_FORCE_INLINE zpScalar getZ() const;
	ZP_FORCE_INLINE zpScalar getW() const;

	ZP_FORCE_INLINE void setX( const zpScalar& x );
	ZP_FORCE_INLINE void setY( const zpScalar& y );
	ZP_FORCE_INLINE void setZ( const zpScalar& z );
	ZP_FORCE_INLINE void setW( const zpScalar& w );

	ZP_FORCE_INLINE void store3( zp_float* xyz ) const;
	ZP_FORCE_INLINE void store4( zp_float* xyzw ) const;

	ZP_FORCE_INLINE void load3( const zp_float* xyz );
	ZP_FORCE_INLINE void load4( const zp_float* xyzw );

	ZP_FORCE_INLINE zp_bool operator==( const zpQuaternion4f& quat ) const;
	ZP_FORCE_INLINE zp_bool operator!=( const zpQuaternion4f& quat ) const;



	friend void zpMath::LengthSquared4( zpScalar& s, const zpQuaternion4f& a );

	friend void zpMath::Dot4( zpScalar& s, const zpQuaternion4f& a, const zpQuaternion4f& b );
	friend void zpMath::Length4( zpScalar& s, const zpQuaternion4f& a );

	friend void zpMath::Normalize4( zpQuaternion4f& s, const zpQuaternion4f& a );

	friend void zpMath::Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );
	friend void zpMath::Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );
	friend void zpMath::Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );
	friend void zpMath::Div( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );

	friend void zpMath::Add( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );
	friend void zpMath::Sub( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );
	friend void zpMath::Mul( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );
	friend void zpMath::Div( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );

	friend void zpMath::Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b );
	friend void zpMath::Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b );
	friend void zpMath::Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b );
	friend void zpMath::Div( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b );

	friend void zpMath::Add( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b );
	friend void zpMath::Sub( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b );
	friend void zpMath::Mul( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b );
	friend void zpMath::Div( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b );

	friend void zpMath::Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b );
	friend void zpMath::Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b );
	friend void zpMath::Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b );

	friend void zpMath::SetEulerAngle( zpQuaternion4f& s, const zpScalar& yaw, const zpScalar& pitch, const zpScalar& roll );
	
	friend void zpMath::GetRoll( zpScalar& s, const zpQuaternion4f& a );
	friend void zpMath::GetPitch( zpScalar& s, const zpQuaternion4f& a );
	friend void zpMath::GetYaw( zpScalar& s, const zpQuaternion4f& a );
	
	friend void zpMath::Conjugate( zpQuaternion4f& s, const zpQuaternion4f& a );
	friend void zpMath::Transform( zpVector4f& s, const zpQuaternion4f& a, const zpVector4f& b );
	friend void zpMath::AxisAngle( zpQuaternion4f& s, const zpVector4f& a, const zpScalar& b );

	friend void zpMath::ToMatrix( zpMatrix4f& s, const zpQuaternion4f& a );


private:
	zp_vec4 m_xyzw;
};
#endif

#endif
