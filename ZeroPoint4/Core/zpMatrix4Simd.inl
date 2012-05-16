/*
ZP_FORCE_INLINE void* zpMatrix4::operator new( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void* zpMatrix4::operator new[]( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void zpMatrix4::operator delete( void* ptr ) {
	_aligned_free( ptr );
}
*/
ZP_FORCE_INLINE zpMatrix4::zpMatrix4() :
	m_m1( _mm_set_ps( 0, 0, 0, 1 ) ),
	m_m2( _mm_set_ps( 0, 0, 1, 0 ) ),
	m_m3( _mm_set_ps( 0, 1, 0, 0 ) ),
	m_m4( _mm_set_ps( 1, 0, 0, 0 ) )
{}
ZP_FORCE_INLINE zpMatrix4::zpMatrix4( zp_float* matrix ) {}
ZP_FORCE_INLINE zpMatrix4::zpMatrix4( zp_vec4* matrix ) {}
ZP_FORCE_INLINE zpMatrix4::zpMatrix4( const zpMatrix4& matrix ) {}
ZP_FORCE_INLINE zpMatrix4::zpMatrix4( zpMatrix4&& matrix ) {}
ZP_FORCE_INLINE zpMatrix4::~zpMatrix4() {}
