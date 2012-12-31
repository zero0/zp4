
#include <xmmintrin.h>
#include <emmintrin.h>

typedef __m128 zpScalar;

typedef __m128i zp_integer;

struct zp_vec4 {
	ZP_FORCE_INLINE zp_vec4() : 
		v( _mm_setzero_ps() ) 
	{}
	ZP_FORCE_INLINE zp_vec4( __m128 m ) : 
		v( m ) 
	{}
	ZP_FORCE_INLINE zp_vec4( zp_float _x, zp_float _y, zp_float _z, zp_float _w ) : 
		v( _mm_set_ps( _w, _z, _y, _x ) )
	{}

	template<int I>
	ZP_FORCE_INLINE zpScalar get() const {
		return _mm_shuffle_ps( v, v, _MM_SHUFFLE( I, I, I, I ) );
	}
	ZP_FORCE_INLINE zpScalar getX() const {
		return get<0>();
	}
	ZP_FORCE_INLINE zpScalar getY() const {
		return get<1>();
	}
	ZP_FORCE_INLINE zpScalar getZ() const {
		return get<2>();
	}
	ZP_FORCE_INLINE zpScalar getW() const {
		return get<3>();
	}

	ZP_FORCE_INLINE operator __m128() const { return v; };
	
	ZP_FORCE_INLINE void operator=( __m128 m ) { v = m; };

	__m128 v;
};

struct zp_ivec4 {
	__m128i v;
};

