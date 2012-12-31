
struct zp_vec4 {
	ZP_FORCE_INLINE zp_vec4() : 
		x( 0 ), y( 0 ), z( 0 ), w( 0 )
	{}
	ZP_FORCE_INLINE zp_vec4( zp_float _x, zp_float _y, zp_float _z, zp_float _w ) : 
		x( _x ), y( _y ), z( _z ), w( _w )
	{}

	template<int I>
	ZP_FORCE_INLINE zpScalar get() const {
		return v[I];
	}
	ZP_FORCE_INLINE zpScalar getX() const {
		return zpScalar( x );
	}
	ZP_FORCE_INLINE zpScalar getY() const {
		return zpScalar( y );
	}
	ZP_FORCE_INLINE zpScalar getZ() const {
		return zpScalar( z );
	}
	ZP_FORCE_INLINE zpScalar getW() const {
		return zpScalar( w );
	}

	ZP_FORCE_INLINE operator const zp_float*() const { return &v[0]; };
	
	ZP_FORCE_INLINE void operator=( const zp_float* m ) { v[0] = m[0]; v[1] = m[1]; v[2] = m[2]; v[3] = m[3];};

	ZP_FORCE_INLINE void operator=( const zp_vec4& m ) { x = m.x; y = m.y; z = m.z; w = m.w; };

	union {
		zp_float v[4];
		struct {
			zp_float x, y, z, w;
		};
	};
};
