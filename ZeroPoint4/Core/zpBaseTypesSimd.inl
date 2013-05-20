
#include <immintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>

struct zpScalar
{
	ZP_FORCE_INLINE zpScalar() {}
	ZP_FORCE_INLINE explicit zpScalar( const zp_float x )
		: m_x( _mm_set_ss( x ) )
	{}
	ZP_FORCE_INLINE explicit zpScalar( const __m128& x )
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

	friend void zpMath::Mul( zpMatrix4f& s, const zpScalar& a, const zpMatrix4f& b );

	friend void zpMath::Madd( zpVector4f& s, const zpVector4f& a, const zpVector4f& b, const zpScalar& c );
	friend void zpMath::Madd( zpVector4f& s, const zpVector4f& a, const zpScalar& b, const zpVector4f& c );

	friend void zpMath::Dot2( zpScalar& s, const zpVector4f& a, const zpVector4f& b );
	friend void zpMath::Dot3( zpScalar& s, const zpVector4f& a, const zpVector4f& b );
	friend void zpMath::Dot4( zpScalar& s, const zpVector4f& a, const zpVector4f& b );

	friend void zpMath::LengthSquared2( zpScalar& s, const zpVector4f& a );
	friend void zpMath::LengthSquared3( zpScalar& s, const zpVector4f& a );

	friend void zpMath::Length2( zpScalar& s, const zpVector4f& a );
	friend void zpMath::Length3( zpScalar& s, const zpVector4f& a );

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


struct zp_vec4
{
	ZP_FORCE_INLINE zp_vec4()
	{}
	ZP_FORCE_INLINE zp_vec4( zp_float _x, zp_float _y, zp_float _z, zp_float _w )
		: m_xyxw( _mm_set_ps( _w, _z, _y, _x ) )
	{}
	ZP_FORCE_INLINE zp_vec4( const __m128& v )
		: m_xyxw( v )
	{}

	operator const __m128&() const
	{
		return m_xyxw;
	}

	template<int I>
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

	template<int I>
	ZP_FORCE_INLINE zpScalar set( const zpScalar& s )
	{
		m_xyxw.m128_f32[ I ] = s.m_x.m128_f32[ I ];
	}
	ZP_FORCE_INLINE zpScalar setX( const zpScalar& s )
	{
		return get<0>();
	}
	ZP_FORCE_INLINE zpScalar setY( const zpScalar& s )
	{
		return get<1>();
	}
	ZP_FORCE_INLINE zpScalar setZ( const zpScalar& s )
	{
		return get<2>();
	}
	ZP_FORCE_INLINE zpScalar setW( const zpScalar& s )
	{
		return get<3>();
	}

private:
	__m128 m_xyxw;
};
