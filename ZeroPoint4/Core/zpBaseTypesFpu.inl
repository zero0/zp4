
typedef zp_float zp_real;

typedef zp_int zp_integer;

struct zp_vec4 {
	ZP_FORCE_INLINE zp_vec4() : 
		x( 0 ), y( 0 ), z( 0 ), w( 0 )
	{}
	ZP_FORCE_INLINE zp_vec4( zp_float _x, zp_float _y, zp_float _z, zp_float _w ) : 
		x( _x ), y( _y ), z( _z ), w( _w )
	{}

	template<int I>
	ZP_FORCE_INLINE zp_real get() const {
		return v[I];
	}
	ZP_FORCE_INLINE zp_real getX() const {
		return get<0>();
	}
	ZP_FORCE_INLINE zp_real getY() const {
		return get<1>();
	}
	ZP_FORCE_INLINE zp_real getZ() const {
		return get<2>();
	}
	ZP_FORCE_INLINE zp_real getW() const {
		return get<3>();
	}

	ZP_FORCE_INLINE operator const zp_float*() const { return &v[0]; };
	
	ZP_FORCE_INLINE void operator=( const zp_float* m ) { v[0] = m[0]; v[1] = m[1]; v[2] = m[2]; v[3] = m[3];};

	union {
		zp_float v[4];
		struct {
			zp_float x, y, z, w;
		};
	};
};

struct zp_ivec4 {
	union {
		zp_int v[4];
		struct {
			zp_int x, y, z, w;
		};
	};
};
