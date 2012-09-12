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
ZP_FORCE_INLINE void zpMatrix4f::scale( zp_real uniformScale ) {}
ZP_FORCE_INLINE void zpMatrix4f::scale( const zpVector4f& scale ) {}

ZP_FORCE_INLINE void zpMatrix4f::mul( const zpVector4f& vector, zpVector4f& outVector ) const {}
ZP_FORCE_INLINE void zpMatrix4f::mul( const zpMatrix4f& matrix, zpMatrix4f& outMatrix ) const {
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

	outMatrix.m_m1 = col1;
	outMatrix.m_m2 = col2;
	outMatrix.m_m3 = col3;
	outMatrix.m_m4 = col4;
}

ZP_FORCE_INLINE void zpMatrix4f::lookAt( const zpVector4f& eye, const zpVector4f& at, const zpVector4f& up ) {
	if( eye == at ) {
		(*this) = identity;
		return;
	}

	lookTo( eye, at - eye, up );
}
ZP_FORCE_INLINE void zpMatrix4f::lookTo( const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up ) {
	zpVector4f z( direction );
	z.normalize3();
	zp_uint i = 0;
	zpVector4f x( up.cross3( z ) );
	x.normalize3();
	i++;
	zpVector4f y( z.cross3( x ) );
	
	zpVector4f e( -eye );
	
	m_m1 = zpVector4f( zp_real_neg( x.getX() ), y.getX(), z.getX(), x.dot3( e ) ).toVec4();
	m_m2 = zpVector4f( zp_real_neg( x.getY() ), y.getY(), z.getY(), y.dot3( e ) ).toVec4();
	m_m3 = zpVector4f( zp_real_neg( x.getZ() ), y.getZ(), z.getZ(), z.dot3( e ) ).toVec4();
	m_m4 = zp_vec4( 0, 0, 0, 1 );
}
ZP_FORCE_INLINE void zpMatrix4f::perspective( zp_float fovy, zp_float aspect, zp_float nearDistance, zp_float farDistance ) {
	
	zp_float f = 1.f / zp_tan( ZP_DEG_TO_RAD( fovy ) * .5f );

	m_m1 = zp_vec4( f / aspect, 0, 0, 0 );
	m_m2 = zp_vec4( 0, f, 0, 0 );
	m_m3 = zp_vec4( 0, 0, -( farDistance + nearDistance ) / ( nearDistance - farDistance ), 2.f * nearDistance * farDistance / ( nearDistance - farDistance ) );
	m_m4 = zp_vec4( 0, 0, 1, 0 );
	//zp_float f = 1.f / zp_tan( ZP_DEG_TO_RAD( fovy ) * .5f );

	//m_11 = f/ aspect;	m_12 = 0;	m_13 = 0;	m_14 = 0;
	//m_21 = 0;	m_22 = f;	m_23 = 0;	m_24 = 0;
	//m_31 = 0;	m_32 = 0;	m_33 = ( farDistance + nearDistance ) / ( farDistance - nearDistance );	m_34 = ( -2.f * farDistance * nearDistance ) / ( farDistance - nearDistance );
	//m_41 = 0;	m_42 = 0;	m_43 = -1;	m_44 = 0;
}
ZP_FORCE_INLINE void zpMatrix4f::ortho( zp_float width, zp_float height, zp_float nearDistance, zp_float farDistance ) {
	zp_float fn = farDistance - nearDistance;
	zp_float pfn = farDistance + nearDistance;
	zp_float nf = nearDistance - farDistance;

	m_m1 = zp_vec4( 2.0f / width, 0, 0, 0 );
	m_m2 = zp_vec4( 0, 2.0f / height, 0, 0 );
	m_m3 = zp_vec4( 0, 0, 1.0f / fn, 0 );
	m_m4 = zp_vec4( 0, 0, -nearDistance / fn, 1 );
	//m_11 = 2.f / width;	m_12 = 0;			m_13 = 0;			m_14 = 0;
	//m_21 = 0;			m_22 = 2.f / height;m_23 = 0;			m_24 = 0;
	//m_31 = 0;			m_32 = 0;			m_33 = 1.f / fn;	m_34 = 0;
	//m_41 = 0;			m_42 = 0;			m_43 = -nearDistance / fn;	m_44 = 1;
}
ZP_FORCE_INLINE void zpMatrix4f::orthoOffset( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance ) {
	m_m1 = zp_vec4( 2.0f / ( right - left ), 0, 0, 0 );
	m_m2 = zp_vec4( 0,  2.0f / ( top - bottom ), 0, 0 );
	m_m3 = zp_vec4( 0, 0,  1.0f / ( farDistance - nearDistance ), 0 );
	m_m4 = zp_vec4( ( left + right ) / ( left - right ), ( top + bottom ) / ( bottom - top ), nearDistance / ( nearDistance - farDistance ), 1 );
	//m_11 = 2.f / ( right - left );	m_12 = 0;			m_13 = 0;			m_14 = 0;
	//m_21 = 0;			m_22 = 2.f / ( top - bottom );m_23 = 0;			m_24 = 0;
	//m_31 = 0;			m_32 = 0;			m_33 = 1.f / ( farDistance - nearDistance );	m_34 = 0;
	//m_41 = ( left + right ) / ( left - right );			m_42 = ( top + bottom ) / ( bottom - top );			m_43 = nearDistance / ( nearDistance - farDistance );	m_44 = 1;

}
ZP_FORCE_INLINE void zpMatrix4f::frustum( zp_float left, zp_float right, zp_float bottom, zp_float top, zp_float nearDistance, zp_float farDistance ) {}

ZP_FORCE_INLINE void zpMatrix4f::transpose() {
	_MM_TRANSPOSE4_PS( m_m1, m_m2, m_m3, m_m4 );
}

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