
#include <immintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>

struct ZP_ALIGN16 zpScalar
{
	ZP_FORCE_INLINE zpScalar() {}
	ZP_FORCE_INLINE explicit zpScalar( const zp_float& x )
		: m_x( _mm_set_ps1( x ) )
	{}
	ZP_FORCE_INLINE explicit zpScalar( zp_float&& x )
		: m_x( _mm_set_ps1( x ) )
	{}
	ZP_FORCE_INLINE explicit zpScalar( const __m128& x )
		: m_x( x )
	{}
	ZP_FORCE_INLINE explicit zpScalar( __m128&& x )
		: m_x( x )
	{}
	ZP_FORCE_INLINE zpScalar( const zpScalar& x )
		: m_x( x.m_x )
	{}
	ZP_FORCE_INLINE zpScalar( zpScalar&& x )
		: m_x( x.m_x )
	{}

	ZP_FORCE_INLINE zp_float getFloat() const { return m_x.m128_f32[0]; }

	friend void zpMath::DegToRad( zpScalar& s, const zpScalar& a );
	friend void zpMath::RadToDeg( zpScalar& s, const zpScalar& a );

	friend void zpMath::Sin( zpScalar& s, const zpScalar& a );
	friend void zpMath::Cos( zpScalar& s, const zpScalar& a );
	friend void zpMath::Tan( zpScalar& s, const zpScalar& a );

	friend void zpMath::Add( zpScalar& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Sub( zpScalar& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Mul( zpScalar& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Div( zpScalar& s, const zpScalar& a, const zpScalar& b );

	friend void zpMath::Add( zpVector4f& s, const zpVector4f& a, const zpScalar& b );
	friend void zpMath::Sub( zpVector4f& s, const zpVector4f& a, const zpScalar& b );
	friend void zpMath::Mul( zpVector4f& s, const zpVector4f& a, const zpScalar& b );
	friend void zpMath::Div( zpVector4f& s, const zpVector4f& a, const zpScalar& b );

	friend void zpMath::Add( zpVector4f& s, const zpScalar& a, const zpVector4f& b );
	friend void zpMath::Sub( zpVector4f& s, const zpScalar& a, const zpVector4f& b );
	friend void zpMath::Mul( zpVector4f& s, const zpScalar& a, const zpVector4f& b );
	friend void zpMath::Div( zpVector4f& s, const zpScalar& a, const zpVector4f& b );

	friend void zpMath::Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );
	friend void zpMath::Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );
	friend void zpMath::Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );
	friend void zpMath::Div( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );

	friend void zpMath::Add( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );
	friend void zpMath::Sub( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );
	friend void zpMath::Mul( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );
	friend void zpMath::Div( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );

	friend void zpMath::Mul( zpMatrix4f& s, const zpScalar& a, const zpMatrix4f& b );

	friend void zpMath::Madd( zpVector4f& s, const zpVector4f& a, const zpVector4f& b, const zpScalar& c );
	friend void zpMath::Madd( zpVector4f& s, const zpVector4f& a, const zpScalar& b, const zpVector4f& c );

	friend void zpMath::Dot2( zpScalar& s, const zpVector4f& a, const zpVector4f& b );
	friend void zpMath::Dot3( zpScalar& s, const zpVector4f& a, const zpVector4f& b );
	friend void zpMath::Dot4( zpScalar& s, const zpVector4f& a, const zpVector4f& b );
	friend void zpMath::Dot4( zpScalar& s, const zpQuaternion4f& a, const zpQuaternion4f& b );

	friend void zpMath::LengthSquared2( zpScalar& s, const zpVector4f& a );
	friend void zpMath::LengthSquared3( zpScalar& s, const zpVector4f& a );
	friend void zpMath::LengthSquared4( zpScalar& s, const zpVector4f& a );
	friend void zpMath::LengthSquared4( zpScalar& s, const zpQuaternion4f& a );

	friend void zpMath::Length2( zpScalar& s, const zpVector4f& a );
	friend void zpMath::Length3( zpScalar& s, const zpVector4f& a );
	friend void zpMath::Length4( zpScalar& s, const zpVector4f& a );
	friend void zpMath::Length4( zpScalar& s, const zpQuaternion4f& a );

	friend void zpMath::Abs( zpScalar& s, const zpScalar& a );
	friend void zpMath::Neg( zpScalar& s, const zpScalar& a );
	friend void zpMath::Sqrt( zpScalar& s, const zpScalar& a );
	friend void zpMath::Rcp( zpScalar& s, const zpScalar& a );
	friend void zpMath::Max( zpScalar& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Min( zpScalar& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Cmp( zp_int& s, const zpScalar& a, const zpScalar& b );
	friend void zpMath::Cmp0( zp_int& s, const zpScalar& a );

	friend void zpMath::Determinant( zpScalar& s, const zpMatrix4f& a );

private:
	__m128 m_x;
};


struct ZP_ALIGN16 zp_vec4
{
	ZP_FORCE_INLINE zp_vec4()
	{}
	ZP_FORCE_INLINE zp_vec4( zp_float _x, zp_float _y, zp_float _z, zp_float _w )
		: m_xyxw( _mm_setr_ps( _x, _y, _z, _w ) )
	{}
	ZP_FORCE_INLINE zp_vec4( const __m128& v )
		: m_xyxw( v )
	{}

	ZP_FORCE_INLINE operator const __m128&() const
	{
		return m_xyxw;
	}

	template<zp_int I>
	ZP_FORCE_INLINE zpScalar get() const
	{
		return zpScalar( _mm_shuffle_ps( m_xyxw, m_xyxw, _MM_SHUFFLE( I, I, I, I ) ) );
	}
	ZP_FORCE_INLINE zpScalar getX() const
	{
		return get<0>();
	}
	ZP_FORCE_INLINE zpScalar getY() const
	{
		return get<1>();
	}
	ZP_FORCE_INLINE zpScalar getZ() const
	{
		return get<2>();
	}
	ZP_FORCE_INLINE zpScalar getW() const
	{
		return get<3>();
	} 

	template<zp_int I>
	ZP_FORCE_INLINE void set( const zpScalar& s )
	{
		m_xyxw.m128_f32[ I ] = s.getFloat();
	}
	ZP_FORCE_INLINE void setX( const zpScalar& s )
	{
		set<0>( s );
	}
	ZP_FORCE_INLINE void setY( const zpScalar& s )
	{
		set<1>( s );
	}
	ZP_FORCE_INLINE void setZ( const zpScalar& s )
	{
		set<2>( s );
	}
	ZP_FORCE_INLINE void setW( const zpScalar& s )
	{
		set<3>( s );
	}

private:
	__m128 m_xyxw;
};
