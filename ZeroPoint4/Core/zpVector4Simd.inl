/*
ZP_FORCE_INLINE void* zpVector4::operator new( zp_uint size ) {
	return _aligned_malloc( size, 16 );
};
ZP_FORCE_INLINE void* zpVector4::operator new[]( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void zpVector4::operator delete( void* ptr ) {
	_aligned_free( ptr );
}
*/
ZP_FORCE_INLINE zpVector4::zpVector4() : m_xyzw( _mm_setzero_ps() ) {}
ZP_FORCE_INLINE zpVector4::zpVector4( zp_float x, zp_float y, zp_float z, zp_float w ) : m_xyzw( _mm_set_ps( w, z, y, x ) ) {}
ZP_FORCE_INLINE zpVector4::zpVector4( const zpVector4& vector ) : m_xyzw( vector.m_xyzw ) {}
ZP_FORCE_INLINE zpVector4::zpVector4( zpVector4&& vector ) : m_xyzw( vector.m_xyzw ) {}
ZP_FORCE_INLINE zpVector4::zpVector4( const zp_vec4& vector ) : m_xyzw( vector ) {}
ZP_FORCE_INLINE zpVector4::~zpVector4() {}


ZP_FORCE_INLINE void zpVector4::mul( const zpVector4& vector ) {
	m_xyzw = _mm_mul_ps( m_xyzw, vector.m_xyzw );
}
ZP_FORCE_INLINE void zpVector4::div( const zpVector4& vector ) {
	m_xyzw = _mm_div_ps( m_xyzw, vector.m_xyzw );
}
ZP_FORCE_INLINE void zpVector4::add( const zpVector4& vector ) {
	m_xyzw = _mm_add_ps( m_xyzw, vector.m_xyzw );
}
ZP_FORCE_INLINE void zpVector4::sub( const zpVector4& vector ) {
	m_xyzw = _mm_sub_ps( m_xyzw, vector.m_xyzw );
}

ZP_FORCE_INLINE void zpVector4::mul( const zpReal& r ) {
	m_xyzw = _mm_mul_ps( m_xyzw, r );
}
ZP_FORCE_INLINE void zpVector4::div( const zpReal& r ) {
	m_xyzw = _mm_div_ps( m_xyzw, r );
}
ZP_FORCE_INLINE void zpVector4::add( const zpReal& r ) {
	m_xyzw = _mm_add_ps( m_xyzw, r );
}
ZP_FORCE_INLINE void zpVector4::sub( const zpReal& r ) {
	m_xyzw = _mm_sub_ps( m_xyzw, r );
}

ZP_FORCE_INLINE void zpVector4::zero() {
	m_xyzw = _mm_setzero_ps();
}
ZP_FORCE_INLINE zp_bool zpVector4::isZero() const {
	zp_vec4 zero = _mm_setzero_ps();
	zp_vec4 cmp = _mm_cmpeq_ps( m_xyzw, zero );
	return cmp.m128_i32[0] && cmp.m128_i32[1] && cmp.m128_i32[2] && cmp.m128_i32[3];
}

ZP_FORCE_INLINE zp_float zpVector4::dot3( const zpVector4& vector ) const {
	zp_vec4 m = _mm_mul_ps( m_xyzw, vector.m_xyzw );
	zp_vec4 x = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 0, 0, 0, 0 ) );
	zp_vec4 y = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	zp_vec4 z = _mm_shuffle_ps( m, m, _MM_SHUFFLE( 2, 2, 2, 2 ) );
	zp_vec4 dot = _mm_add_ps( x, _mm_add_ps( y, z ) );
	return dot.m128_f32[0];
}
ZP_FORCE_INLINE zp_float zpVector4::dot4( const zpVector4& vector ) const {
	zp_vec4 t0 = _mm_mul_ps( m_xyzw, vector.m_xyzw );
	zp_vec4 t1 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 1, 0, 3, 2 ) );
	zp_vec4 t2 = _mm_add_ps( t0, t1 );
	zp_vec4 t3 = _mm_shuffle_ps( t2, t2, _MM_SHUFFLE( 2, 3, 0, 1 ) );
	zp_vec4 dot = _mm_add_ps( t3, t2 );
	return dot.m128_f32[0];
}