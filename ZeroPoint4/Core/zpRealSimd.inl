
ZP_FORCE_INLINE zp_float zpScalarToFloat( zpScalar a ) {
	zp_float v;
	_mm_store_ss( &v, a );
	return v;
}
ZP_FORCE_INLINE zpScalar zpScalarFromFloat( zp_float a ) {
	return _mm_set_ps1( a );
}

ZP_FORCE_INLINE zpScalar zpScalarZero() {
	return _mm_setzero_ps();
}

ZP_FORCE_INLINE zpScalar zpScalarAdd( zpScalar a, zpScalar b ) {
	return _mm_add_ps( a, b );
}
ZP_FORCE_INLINE zpScalar zpScalarSub( zpScalar a, zpScalar b ) {
	return _mm_sub_ps( a, b );
}
ZP_FORCE_INLINE zpScalar zpScalarMul( zpScalar a, zpScalar b ) {
	return _mm_mul_ps( a, b );
}
ZP_FORCE_INLINE zpScalar zpScalarDiv( zpScalar a, zpScalar b ) {
	return _mm_div_ps( a, b );
}

ZP_FORCE_INLINE zpScalar zpScalarNeg( zpScalar a ) {
	return _mm_mul_ps( a, _mm_set_ps1( -1 ) );
}
ZP_FORCE_INLINE zpScalar zpScalarRcp( zpScalar a ) {
	return _mm_rcp_ps( a );
}
ZP_FORCE_INLINE zpScalar zpScalarAbs( zpScalar a ) {
	return _mm_and_ps( a, *(zpScalar*)&_mm_set1_epi32( 0x7fffffff ) );
}

ZP_FORCE_INLINE zpScalar zpScalarSqrt( zpScalar a ) {
	return _mm_sqrt_ps( a );
}
ZP_FORCE_INLINE zpScalar zpScalarRsqrt( zpScalar a ) {
	return _mm_rsqrt_ps( a );
}

ZP_FORCE_INLINE zpScalar zpScalarMod( zpScalar a, zpScalar b ) {
	zpScalar tmp = _mm_div_ps ( a, b );
	tmp = _mm_cvtepi32_ps( _mm_cvtps_epi32( tmp ) );
	tmp = _mm_mul_ps( tmp, b );
	return _mm_sub_ps( a, tmp );
}

ZP_FORCE_INLINE zpScalar zpScalarMin( zpScalar a, zpScalar b ) {
	return _mm_min_ps( a, b );
}
ZP_FORCE_INLINE zpScalar zpScalarMax( zpScalar a, zpScalar b ) {
	return _mm_max_ps( a, b );
}

ZP_FORCE_INLINE zpScalar zpScalarClamp( zpScalar real, zpScalar low, zpScalar high ) {
	return _mm_max_ps( _mm_min_ps( real, high ), low );
}
ZP_FORCE_INLINE zpScalar zpScalarSaturate( zpScalar real ) {
	return _mm_max_ps( _mm_min_ps( real, _mm_set_ps1( 1 ) ), _mm_set_ps1( 0 ) );
}

ZP_FORCE_INLINE zpScalar zpScalarSin( zpScalar theta ) {
	zpScalar upper = _mm_set_ps1( 4.f / ZP_PI );
	zpScalar lower = _mm_set_ps1( -4.f / ( ZP_PI * ZP_PI ) );
	zpScalar amask = *(zpScalar*)&_mm_set1_epi32( 0x7fffffff );
	zpScalar scale = _mm_set_ps1( 0.225f );
	
	zpScalar tmp = _mm_and_ps( amask, theta );
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
ZP_FORCE_INLINE zpScalar zpScalarCos( zpScalar theta ) {
	zpScalar negPi = _mm_set_ps1( -ZP_PI );
	zpScalar twoPi = _mm_set_ps1( ZP_2PI );
	zpScalar piOver2 = _mm_set_ps1( ZP_PIOVER2 );

	theta = _mm_sub_ps( theta, piOver2 );
	zpScalar tmp = _mm_cmplt_ps( theta, negPi );
	tmp = _mm_and_ps( tmp, twoPi );
	theta = _mm_add_ps( theta, tmp );
	
	return zpScalarSin( theta );
}
ZP_FORCE_INLINE zpScalar zpScalarTan( zpScalar theta ) {
	zpScalar c = zpScalarCos( theta );
	zpScalar s = zpScalarSin( theta );

	return _mm_div_ps( c, s );
}

ZP_FORCE_INLINE zpScalar zpScalarAsin( zpScalar theta ) {
	zpScalar scale = _mm_set_ps1( 0.391f );
	zpScalar x5 = theta;

	x5 = _mm_mul_ps( x5, theta );
	x5 = _mm_mul_ps( x5, x5 );
	x5 = _mm_mul_ps( x5, theta );
	x5 = _mm_mul_ps( x5, scale );
	return _mm_add_ps( x5, theta );
}
ZP_FORCE_INLINE zpScalar zpScalarAcos( zpScalar theta ) {
	zpScalar piOver2 = _mm_set_ps1( ZP_PIOVER2 );
	zpScalar x = zpScalarAsin( theta );
	return _mm_sub_ps( piOver2, x );
}
ZP_FORCE_INLINE zpScalar zpScalarAtan( zpScalar theta ) {
	return theta;
}

ZP_FORCE_INLINE zpScalar zpScalarCsc( zpScalar theta ) {
	theta = zpScalarSin( theta );
	return _mm_rcp_ps( theta );
}
ZP_FORCE_INLINE zpScalar zpScalarSec( zpScalar theta ) {
	theta = zpScalarCos( theta );
	return _mm_rcp_ps( theta );
}
ZP_FORCE_INLINE zpScalar zpScalarCot( zpScalar theta ) {
	theta = zpScalarTan( theta );
	return _mm_rcp_ps( theta );
}

ZP_FORCE_INLINE zp_bool zpScalarEq( zpScalar a, zpScalar b ) {
	return _mm_comieq_ss( a, b ) == 1;
}
ZP_FORCE_INLINE zp_bool zpScalarNeq( zpScalar a, zpScalar b ) {
	return _mm_comineq_ss( a, b ) == 1;
}

ZP_FORCE_INLINE zp_bool zpScalarLt( zpScalar a, zpScalar b ) {
	return _mm_comilt_ss( a, b ) == 1;
}
ZP_FORCE_INLINE zp_bool zpScalarLte( zpScalar a, zpScalar b ) {
	return _mm_comile_ss( a, b ) == 1;
}

ZP_FORCE_INLINE zp_bool zpScalarGt( zpScalar a, zpScalar b ) {
	return _mm_comigt_ss( a, b ) == 1;
}
ZP_FORCE_INLINE zp_bool zpScalarGte( zpScalar a, zpScalar b ) {
	return _mm_comige_ss( a, b ) == 1;
}

ZP_FORCE_INLINE zpScalar operator+( zpScalar a, zpScalar b ) {
	return zpScalarAdd( a, b );
}
ZP_FORCE_INLINE zpScalar operator-( zpScalar a, zpScalar b ) {
	return zpScalarSub( a, b );
}
ZP_FORCE_INLINE zpScalar operator*( zpScalar a, zpScalar b ) {
	return zpScalarMul( a, b );
}
ZP_FORCE_INLINE zpScalar operator/( zpScalar a, zpScalar b ) {
	return zpScalarDiv( a, b );
}