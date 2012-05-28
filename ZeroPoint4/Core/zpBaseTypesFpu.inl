
union zp_vec4 {
	zp_vec4() : 
		x( 0 ), y( 0 ), z( 0 ), w( 0 ) {}

	zp_vec4( zp_float _x, zp_float _y, zp_float _z, zp_float _w ) : 
		x( _x ), y( _y ), z( _z ), w( _w ) {}

	zp_float v[4];
	struct {
		zp_float x, y, z, w;
	};
};

union zp_ivec4 {
	zp_int v[4];
	struct {
		zp_int x, y, z, w;
	};
};

typedef zp_float zp_real;

typedef zp_int zp_integer;