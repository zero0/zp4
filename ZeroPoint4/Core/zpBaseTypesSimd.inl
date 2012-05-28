
#include <xmmintrin.h>

struct zp_vec4 {
	zp_vec4() : v( _mm_setzero_ps() ) {}
	zp_vec4( __m128 m ) : v( m ) {}
	zp_vec4( zp_float _x, zp_float _y, zp_float _z, zp_float _w ) : 
		v( _mm_set_ps( _w, _z, _y, _x ) ) {}

	template<int I>
	zp_vec4 get() const {
		return zp_vec4( _mm_shuffle_ps( v, v, _MM_SHUFFLE( I, I, I, I ) ) );
	}

	operator __m128() const { return v; };
	
	void operator=( __m128 m ) { v = m; };

	__m128 v;
};

typedef __m128 zp_ivec4;

typedef zp_vec4 zp_real;

typedef zp_ivec4 zp_integer;
