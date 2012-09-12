
ZP_FORCE_INLINE zp_float zp_real_to_float( zp_real a ) {
	zp_float v;
	_mm_store_ss( &v, a );
	return v;
}
ZP_FORCE_INLINE zp_real zp_real_from_float( zp_float a ) {
	return _mm_set_ps1( a );
}

ZP_FORCE_INLINE zp_real zp_real_zero() {
	return _mm_setzero_ps();
}

ZP_FORCE_INLINE zp_real zp_real_add( zp_real a, zp_real b ) {
	return _mm_add_ps( a, b );
}
ZP_FORCE_INLINE zp_real zp_real_sub( zp_real a, zp_real b ) {
	return _mm_sub_ps( a, b );
}
ZP_FORCE_INLINE zp_real zp_real_mul( zp_real a, zp_real b ) {
	return _mm_mul_ps( a, b );
}
ZP_FORCE_INLINE zp_real zp_real_div( zp_real a, zp_real b ) {
	return _mm_div_ps( a, b );
}

ZP_FORCE_INLINE zp_real zp_real_neg( zp_real a ) {
	return _mm_mul_ps( a, _mm_set_ps1( -1 ) );
}
ZP_FORCE_INLINE zp_real zp_real_rcp( zp_real a ) {
	return _mm_rcp_ps( a );
}
ZP_FORCE_INLINE zp_real zp_real_abs( zp_real a ) {
	return _mm_and_ps( a, *(zp_real*)&_mm_set1_epi32( 0x7fffffff ) );
}

ZP_FORCE_INLINE zp_real zp_real_sqrt( zp_real a ) {
	return _mm_sqrt_ps( a );
}
ZP_FORCE_INLINE zp_real zp_real_rsqrt( zp_real a ) {
	return _mm_rsqrt_ps( a );
}

ZP_FORCE_INLINE zp_real zp_real_mod( zp_real a, zp_real b ) {
	zp_real tmp = _mm_div_ps ( a, b );
	tmp = _mm_cvtepi32_ps( _mm_cvtps_epi32( tmp ) );
	tmp = _mm_mul_ps( tmp, b );
	return _mm_sub_ps( a, tmp );
}

ZP_FORCE_INLINE zp_real zp_real_min( zp_real a, zp_real b ) {
	return _mm_min_ps( a, b );
}
ZP_FORCE_INLINE zp_real zp_real_max( zp_real a, zp_real b ) {
	return _mm_max_ps( a, b );
}

ZP_FORCE_INLINE zp_real zp_real_clamp( zp_real real, zp_real low, zp_real high ) {
	return _mm_max_ps( _mm_min_ps( real, high ), low );
}
ZP_FORCE_INLINE zp_real zp_real_clamp01( zp_real real ) {
	return _mm_max_ps( _mm_min_ps( real, _mm_set_ps1( 1 ) ), _mm_set_ps1( 0 ) );
}

ZP_FORCE_INLINE zp_real zp_real_sin( zp_real theta ) {
	zp_real upper = _mm_set_ps1( 4.f / ZP_PI );
	zp_real lower = _mm_set_ps1( -4.f / ( ZP_PI * ZP_PI ) );
	zp_real amask = *(zp_real*)&_mm_set1_epi32( 0x7fffffff );
	zp_real scale = _mm_set_ps1( 0.225f );
	
	zp_real tmp = _mm_and_ps( amask, theta );
	theta = tmp;

	tmp = _mm_mul_ps( tmp, theta );
	tmp = _mm_mul_ps( tmp, lower );
	theta = _mm_mul_ps( theta, upper );
	theta = _mm_add_ps( theta, tmp );
	
	tmp = _mm_and_ps( theta, amask );
	tmp = _mm_mul_ps( tmp, theta );
	tmp = _mm_sub_ps( tmp, theta );
	tmp = _mm_mul_ps( tmp, scale );
	theta = _mm_add_ps( theta, tmp );

	return theta;
}
ZP_FORCE_INLINE zp_real zp_real_cos( zp_real theta ) {
	zp_real negPi = _mm_set_ps1( -ZP_PI );
	zp_real twoPi = _mm_set_ps1( ZP_2PI );
	zp_real piOver2 = _mm_set_ps1( ZP_PIOVER2 );

	theta = _mm_sub_ps( theta, piOver2 );
	zp_real tmp = _mm_cmplt_ps( theta, negPi );
	tmp = _mm_and_ps( tmp, twoPi );
	theta = _mm_add_ps( theta, tmp );
	
	return zp_real_sin( theta );
}
ZP_FORCE_INLINE zp_real zp_real_tan( zp_real theta ) {
	zp_real c = zp_real_cos( theta );
	zp_real s = zp_real_sin( theta );

	return _mm_div_ps( c, s );
}

ZP_FORCE_INLINE zp_real zp_real_asin( zp_real theta ) {
	zp_real scale = _mm_set_ps1( 0.391f );
	zp_real x5 = theta;

	x5 = _mm_mul_ps( x5, theta );
	x5 = _mm_mul_ps( x5, x5 );
	x5 = _mm_mul_ps( x5, theta );
	x5 = _mm_mul_ps( x5, scale );
	return _mm_add_ps( x5, theta );
}
ZP_FORCE_INLINE zp_real zp_real_acos( zp_real theta ) {
	zp_real piOver2 = _mm_set_ps1( ZP_PIOVER2 );
	zp_real x = zp_real_asin( theta );
	return _mm_sub_ps( piOver2, x );
}
ZP_FORCE_INLINE zp_real zp_real_atan( zp_real theta ) {
	return theta;
}

ZP_FORCE_INLINE zp_real zp_real_csc( zp_real theta ) {
	theta = zp_real_sin( theta );
	return _mm_rcp_ps( theta );
}
ZP_FORCE_INLINE zp_real zp_real_sec( zp_real theta ) {
	theta = zp_real_cos( theta );
	return _mm_rcp_ps( theta );
}
ZP_FORCE_INLINE zp_real zp_real_cot( zp_real theta ) {
	theta = zp_real_tan( theta );
	return _mm_rcp_ps( theta );
}

ZP_FORCE_INLINE zp_bool zp_real_eq( zp_real a, zp_real b ) {
	return _mm_comieq_ss( a, b ) == 1;
}
ZP_FORCE_INLINE zp_bool zp_real_neq( zp_real a, zp_real b ) {
	return _mm_comineq_ss( a, b ) == 1;
}

ZP_FORCE_INLINE zp_bool zp_real_lt( zp_real a, zp_real b ) {
	return _mm_comilt_ss( a, b ) == 1;
}
ZP_FORCE_INLINE zp_bool zp_real_lte( zp_real a, zp_real b ) {
	return _mm_comile_ss( a, b ) == 1;
}

ZP_FORCE_INLINE zp_bool zp_real_gt( zp_real a, zp_real b ) {
	return _mm_comigt_ss( a, b ) == 1;
}
ZP_FORCE_INLINE zp_bool zp_real_gte( zp_real a, zp_real b ) {
	return _mm_comige_ss( a, b ) == 1;
}

ZP_FORCE_INLINE zp_real operator+( zp_real a, zp_real b ) {
	return zp_real_add( a, b );
}
ZP_FORCE_INLINE zp_real operator-( zp_real a, zp_real b ) {
	return zp_real_sub( a, b );
}
ZP_FORCE_INLINE zp_real operator*( zp_real a, zp_real b ) {
	return zp_real_mul( a, b );
}
ZP_FORCE_INLINE zp_real operator/( zp_real a, zp_real b ) {
	return zp_real_div( a, b );
}