
union zp_vec4 {
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