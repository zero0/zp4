/*
ZP_FORCE_INLINE void* zpVector4f::operator new( zp_uint size ) {
	return _aligned_malloc( size, 16 );
};
ZP_FORCE_INLINE void* zpVector4f::operator new[]( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void zpVector4f::operator delete( void* ptr ) {
	_aligned_free( ptr );
}
*/

ZP_FORCE_INLINE zpVector4f::zpVector4f( zp_float x, zp_float y, zp_float z, zp_float w ) :
	m_xyzw( x, y, z, w )
{}
	
ZP_FORCE_INLINE zpVector4f zpVector4f::operator-() const {
	return zpVector4f( zp_real_neg( m_xyzw ) );
}

ZP_FORCE_INLINE void zpVector4f::set3( const zp_real& x, const zp_real& y, const zp_real& z ) {
	m_xyzw = _mm_set_ps( 
		zp_real_to_float( x ),
		zp_real_to_float( y ), 
		zp_real_to_float( z ), 
		m_xyzw.v.m128_f32[3] 
	);
}
ZP_FORCE_INLINE void zpVector4f::set4( const zp_real& x, const zp_real& y, const zp_real& z, const zp_real& w ) {
	m_xyzw = _mm_set_ps( 
		zp_real_to_float( x ),
		zp_real_to_float( y ), 
		zp_real_to_float( z ), 
		zp_real_to_float( w )
	);
}
ZP_FORCE_INLINE void zpVector4f::set3( const zpVector4f& vector ) {
	m_xyzw.v.m128_f32[0] = vector.m_xyzw.v.m128_f32[0];
	m_xyzw.v.m128_f32[1] = vector.m_xyzw.v.m128_f32[1];
	m_xyzw.v.m128_f32[2] = vector.m_xyzw.v.m128_f32[2];
}
ZP_FORCE_INLINE void zpVector4f::set4( const zpVector4f& vector ) {
	m_xyzw = vector.m_xyzw;
}

ZP_FORCE_INLINE void zpVector4f::setX( const zp_real& x ) {
	m_xyzw.v.m128_f32[0] = zp_real_to_float( x );
}
ZP_FORCE_INLINE void zpVector4f::setY( const zp_real& y ) {
	m_xyzw.v.m128_f32[1] = zp_real_to_float( y );
}
ZP_FORCE_INLINE void zpVector4f::setZ( const zp_real& z ) {
	m_xyzw.v.m128_f32[2] = zp_real_to_float( z );
}
ZP_FORCE_INLINE void zpVector4f::setW( const zp_real& w ) {
	m_xyzw.v.m128_f32[3] = zp_real_to_float( w );
}

ZP_FORCE_INLINE void zpVector4f::store3( zp_float* xyz ) const {
	xyz[0] = m_xyzw.v.m128_f32[0];
	xyz[1] = m_xyzw.v.m128_f32[1];
	xyz[2] = m_xyzw.v.m128_f32[2];
}
ZP_FORCE_INLINE void zpVector4f::store4( zp_float* xyzw ) const {
	xyzw[0] = m_xyzw.v.m128_f32[0];
	xyzw[1] = m_xyzw.v.m128_f32[1];
	xyzw[2] = m_xyzw.v.m128_f32[2];
	xyzw[3] = m_xyzw.v.m128_f32[3];
}

ZP_FORCE_INLINE void zpVector4f::load3( const zp_float* xyz ) {
	m_xyzw.v.m128_f32[0] = xyz[0];
	m_xyzw.v.m128_f32[1] = xyz[1];
	m_xyzw.v.m128_f32[2] = xyz[2];
}
ZP_FORCE_INLINE void zpVector4f::load4( const zp_float* xyzw ) {
	m_xyzw.v.m128_f32[0] = xyzw[0];
	m_xyzw.v.m128_f32[1] = xyzw[1];
	m_xyzw.v.m128_f32[2] = xyzw[2];
	m_xyzw.v.m128_f32[3] = xyzw[3];
}

ZP_FORCE_INLINE void zpVector4f::mul3( const zpVector4f& vector ) {
	zp_float w = m_xyzw.v.m128_f32[3];
	m_xyzw = _mm_mul_ps( m_xyzw, vector.m_xyzw );
	m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::mul4( const zpVector4f& vector ) {
	m_xyzw = _mm_mul_ps( m_xyzw, vector.m_xyzw );
}
ZP_FORCE_INLINE void zpVector4f::div3( const zpVector4f& vector ) {
	zp_float w = m_xyzw.v.m128_f32[3];
	m_xyzw = _mm_div_ps( m_xyzw, vector.m_xyzw );
	m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::div4( const zpVector4f& vector ) {
	m_xyzw = _mm_div_ps( m_xyzw, vector.m_xyzw );
}
ZP_FORCE_INLINE void zpVector4f::add3( const zpVector4f& vector ) {
	zp_float w = m_xyzw.v.m128_f32[3];
	m_xyzw = _mm_add_ps( m_xyzw, vector.m_xyzw );
	m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::add4( const zpVector4f& vector ) {
	m_xyzw = _mm_add_ps( m_xyzw, vector.m_xyzw );
}
ZP_FORCE_INLINE void zpVector4f::sub3( const zpVector4f& vector ) {
	zp_float w = m_xyzw.v.m128_f32[3];
	m_xyzw = _mm_sub_ps( m_xyzw, vector.m_xyzw );
	m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::sub4( const zpVector4f& vector ) {
	m_xyzw = _mm_sub_ps( m_xyzw, vector.m_xyzw );
}

ZP_FORCE_INLINE void zpVector4f::mul3( const zp_real& r ) {
	zp_float w = m_xyzw.v.m128_f32[3];
	m_xyzw = _mm_mul_ps( m_xyzw, r );
	m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::mul4( const zp_real& r ) {
	m_xyzw = _mm_mul_ps( m_xyzw, r );
}
ZP_FORCE_INLINE void zpVector4f::div3( const zp_real& r ) {
	zp_float w = m_xyzw.v.m128_f32[3];
	m_xyzw = _mm_div_ps( m_xyzw, r );
	m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::div4( const zp_real& r ) {
	m_xyzw = _mm_div_ps( m_xyzw, r );
}
ZP_FORCE_INLINE void zpVector4f::add3( const zp_real& r ) {
	zp_float w = m_xyzw.v.m128_f32[3];
	m_xyzw = _mm_add_ps( m_xyzw, r );
	m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::add4( const zp_real& r ) {
	m_xyzw = _mm_add_ps( m_xyzw, r );
}
ZP_FORCE_INLINE void zpVector4f::sub3( const zp_real& r ) {
	zp_float w = m_xyzw.v.m128_f32[3];
	m_xyzw = _mm_sub_ps( m_xyzw, r );
	m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::sub4( const zp_real& r ) {
	m_xyzw = _mm_sub_ps( m_xyzw, r );
}

ZP_FORCE_INLINE void zpVector4f::zero3() {
	zp_float w = m_xyzw.v.m128_f32[3];
	m_xyzw = _mm_setzero_ps();
	m_xyzw.v.m128_f32[3] = w;
}
ZP_FORCE_INLINE void zpVector4f::zero4() {
	m_xyzw = _mm_setzero_ps();
}
ZP_FORCE_INLINE zp_bool zpVector4f::isZero3() const {
	zp_real zero = _mm_setzero_ps();
	zp_real cmp = _mm_cmpeq_ps( m_xyzw, zero );
	return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2];
}
ZP_FORCE_INLINE zp_bool zpVector4f::isZero4() const {
	zp_real zero = _mm_setzero_ps();
	zp_real cmp = _mm_cmpeq_ps( m_xyzw, zero );
	return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2] && cmp.m128_i32[3];
}

ZP_FORCE_INLINE zp_real zpVector4f::dot3( const zpVector4f& vector ) const {
	zp_real m = _mm_mul_ps( m_xyzw, vector.m_xyzw );
	zp_real x = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 0, 0, 0, 0 ) );
	zp_real y = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	zp_real z = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 2, 2, 2, 2 ) );
	return _mm_add_ps( x, _mm_add_ps( y, z ) );
}
ZP_FORCE_INLINE zp_real zpVector4f::dot4( const zpVector4f& vector ) const {
	zp_real t0 = _mm_mul_ps( m_xyzw, vector.m_xyzw );
	zp_real t1 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 1, 0, 3, 2 ) );
	zp_real t2 = _mm_add_ps( t0, t1 );
	zp_vec4 t3 = _mm_shuffle_ps( t2, t2, _MM_SHUFFLE( 2, 3, 0, 1 ) );
	return _mm_add_ps( t3, t2 );
}

ZP_FORCE_INLINE zpVector4f zpVector4f::cross3( const zpVector4f& vector ) const {
	zp_real s0 = _mm_shuffle_ps( m_xyzw, m_xyzw, _MM_SHUFFLE( 3, 0, 2, 1 ) );
	zp_real s1 = _mm_shuffle_ps( vector.m_xyzw, vector.m_xyzw, _MM_SHUFFLE( 3, 1, 0, 2 ) );
	zp_real s2 = _mm_mul_ps( s0, s1 );

	zp_real s3 = _mm_shuffle_ps( m_xyzw, m_xyzw, _MM_SHUFFLE( 3, 1, 0, 2 ) );
	zp_real s4 = _mm_shuffle_ps( vector.m_xyzw, vector.m_xyzw, _MM_SHUFFLE( 3, 0, 2, 1 ) );
	zp_real s5 = _mm_mul_ps( s3, s4 );

	return zpVector4f( _mm_sub_ps( s2, s5 ) );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::cross4( const zpVector4f& vector ) const {
	// hmmm...
	return zpVector4f();
}

ZP_FORCE_INLINE zp_bool zpVector4f::equals3( const zpVector4f& vector ) const {
	zp_real cmp = _mm_cmpeq_ps( m_xyzw, vector.m_xyzw );
	return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2];
}
ZP_FORCE_INLINE zp_bool zpVector4f::equals4( const zpVector4f& vector ) const {
	zp_real cmp = _mm_cmpeq_ps( m_xyzw, vector.m_xyzw );
	return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2] && cmp.m128_i32[3];
}

ZP_FORCE_INLINE zp_bool zpVector4f::operator==( const zpVector4f& vector ) const {
	return equals4( vector );
}
ZP_FORCE_INLINE zp_bool zpVector4f::operator!=( const zpVector4f& vector ) const {
	zp_real cmp = _mm_cmpneq_ps( m_xyzw, vector.m_xyzw );
	return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2] && cmp.m128_i32[3];
}
