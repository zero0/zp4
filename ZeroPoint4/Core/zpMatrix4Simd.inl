/*
ZP_FORCE_INLINE void* zpMatrix4f::operator new( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void* zpMatrix4f::operator new[]( zp_uint size ) {
	return _aligned_malloc( size, 16 );
}
ZP_FORCE_INLINE void zpMatrix4f::operator delete( void* ptr ) {
	_aligned_free( ptr );
}
*/

ZP_FORCE_INLINE void zpMatrix4f::operator*=( const zpMatrix4f& matrix ) {
	zp_vec4 col1, col2, col3, col4;

	col1 = _mm_mul_ps( matrix.m_m1.get<0>(), m_m1 );
	col2 = _mm_mul_ps( matrix.m_m2.get<0>(), m_m1 );
	col3 = _mm_mul_ps( matrix.m_m3.get<0>(), m_m1 );
	col4 = _mm_mul_ps( matrix.m_m4.get<0>(), m_m1 );

	col1 = _mm_add_ps( col1, _mm_mul_ps( matrix.m_m1.get<1>(), m_m2 ) );
	col2 = _mm_add_ps( col2, _mm_mul_ps( matrix.m_m2.get<1>(), m_m2 ) );
	col3 = _mm_add_ps( col3, _mm_mul_ps( matrix.m_m3.get<1>(), m_m2 ) );
	col4 = _mm_add_ps( col4, _mm_mul_ps( matrix.m_m4.get<1>(), m_m2 ) );

	col1 = _mm_add_ps( col1, _mm_mul_ps( matrix.m_m1.get<2>(), m_m3 ) );
	col2 = _mm_add_ps( col2, _mm_mul_ps( matrix.m_m2.get<2>(), m_m3 ) );
	col3 = _mm_add_ps( col3, _mm_mul_ps( matrix.m_m3.get<2>(), m_m3 ) );
	col4 = _mm_add_ps( col4, _mm_mul_ps( matrix.m_m4.get<2>(), m_m3 ) );

	col1 = _mm_add_ps( col1, _mm_mul_ps( matrix.m_m1.get<3>(), m_m4 ) );
	col2 = _mm_add_ps( col2, _mm_mul_ps( matrix.m_m2.get<3>(), m_m4 ) );
	col3 = _mm_add_ps( col3, _mm_mul_ps( matrix.m_m3.get<3>(), m_m4 ) );
	col4 = _mm_add_ps( col4, _mm_mul_ps( matrix.m_m4.get<3>(), m_m4 ) );

	m_m1 = col1;
	m_m2 = col2;
	m_m3 = col3;
	m_m4 = col4;
}

ZP_FORCE_INLINE void zpMatrix4f::translate( const zpVector4f& position ) {
	zp_real col, x, y, z, w;

	x = position.getX();
	y = position.getY();
	z = position.getZ();
	w = position.getW();

	col = _mm_mul_ps( x, m_m1 );
	col = _mm_add_ps( col, _mm_mul_ps( y, m_m2 ) );
	col = _mm_add_ps( col, _mm_mul_ps( z, m_m3 ) );
	col = _mm_add_ps( col, _mm_mul_ps( w, m_m4 ) );

	m_m4 = col;
}
ZP_FORCE_INLINE void zpMatrix4f::rotate( const zpVector4f& axis, zp_float angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::rotateX( zp_real angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::rotateY( zp_real angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::rotateZ( zp_real angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::scale( zp_float uniformScale ) {}
ZP_FORCE_INLINE void zpMatrix4f::scale( const zpVector4f& scale ) {}

ZP_FORCE_INLINE void zpMatrix4f::mul( const zpVector4f& vector, zpVector4f* outVector ) const {}

ZP_FORCE_INLINE void zpMatrix4f::lookAt( const zpVector4f& eye, const zpVector4f& point, const zpVector4f& up ) {}
ZP_FORCE_INLINE void zpMatrix4f::perspective( zp_float fovy, zp_float aspect, zp_float nearDistance, zp_float farDistance ) {}
ZP_FORCE_INLINE void zpMatrix4f::ortho( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance ) {}
ZP_FORCE_INLINE void zpMatrix4f::orthoOffset( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance ) {}
ZP_FORCE_INLINE void zpMatrix4f::frustum( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance ) {}

ZP_FORCE_INLINE zp_bool zpMatrix4f::operator==( const zpMatrix4f& matrix ) const {
	for( zp_uint i = 16; i --> 0; ) if( m_data[i] != matrix.m_data[i] ) return false;
	return true;
}
ZP_FORCE_INLINE zp_bool zpMatrix4f::operator!=( const zpMatrix4f& matrix ) const {
	for( zp_uint i = 16; i --> 0; ) if( m_data[i] == matrix.m_data[i] ) return false;
	return true;
}
#undef MM_
#undef MM