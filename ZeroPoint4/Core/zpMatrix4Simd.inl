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
#if 0
ZP_FORCE_INLINE void zpMatrix4f::translate( const zpVector4f& position ) {
	zp_vec4 col;

	col = _mm_mul_ps( position.getX(), m_m1 );
	col = _mm_add_ps( col, _mm_mul_ps( position.getY(), m_m2 ) );
	col = _mm_add_ps( col, _mm_mul_ps( position.getZ(), m_m3 ) );
	col = _mm_add_ps( col, _mm_mul_ps( position.getW(), m_m4 ) );

	m_m4 = col;
}
ZP_FORCE_INLINE void zpMatrix4f::rotate( const zpVector4f& axis, zp_float angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::rotateX( zpScalar angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::rotateY( zpScalar angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::rotateZ( zpScalar angle ) {}
ZP_FORCE_INLINE void zpMatrix4f::scale( zpScalar uniformScale ) {}
ZP_FORCE_INLINE void zpMatrix4f::scale( const zpVector4f& scale ) {}

ZP_FORCE_INLINE void zpMatrix4f::mul( const zpVector4f& vector, zpVector4f& outVector ) const {
	zp_vec4 col;

	col = _mm_mul_ps( vector.getX(), m_m1 );
	col = _mm_add_ps( col, _mm_mul_ps( vector.getY(), m_m2 ) );
	col = _mm_add_ps( col, _mm_mul_ps( vector.getZ(), m_m3 ) );
	col = _mm_add_ps( col, _mm_mul_ps( vector.getW(), m_m4 ) );

	outVector = col;
}
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

	zpVector4f x( up.cross3( z ) ); 
	x.normalize3();

	zpVector4f y( z.cross3( x ) );
	
	zpVector4f e( -eye );

	m_m1 = zpVector4f( zpScalarNeg( x.getX() ), y.getX(), z.getX(), x.dot3( e ) ).toVec4();
	m_m2 = zpVector4f( zpScalarNeg( x.getY() ), y.getY(), z.getY(), y.dot3( e ) ).toVec4();
	m_m3 = zpVector4f( zpScalarNeg( x.getZ() ), y.getZ(), z.getZ(), z.dot3( e ) ).toVec4();
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

ZP_FORCE_INLINE zp_float zpMatrix4f::determinant() const {
	// TODO: optimize for SIMD
	return 
		m_14*m_23*m_32*m_41 - m_13*m_24*m_32*m_41 - m_14*m_22*m_33*m_41 + m_12*m_24*m_33*m_41+
		m_13*m_22*m_34*m_41 - m_12*m_23*m_34*m_41 - m_14*m_23*m_31*m_42 + m_13*m_24*m_31*m_42+
		m_14*m_21*m_33*m_42 - m_11*m_24*m_33*m_42 - m_13*m_21*m_34*m_42 + m_11*m_23*m_34*m_42+
		m_14*m_22*m_31*m_43 - m_12*m_24*m_31*m_43 - m_14*m_21*m_32*m_43 + m_11*m_24*m_32*m_43+
		m_12*m_21*m_34*m_43 - m_11*m_22*m_34*m_43 - m_13*m_22*m_31*m_44 + m_12*m_23*m_31*m_44+
		m_13*m_21*m_32*m_44 - m_11*m_23*m_32*m_44 - m_12*m_21*m_33*m_44 + m_11*m_22*m_33*m_44;
}
ZP_FORCE_INLINE void zpMatrix4f::invert() {
	zp_float* src = m_data;

	//
	// From "Streaming SIMD Extension - Inverse of 4x4 Matrix" - http://download.intel.com/design/PentiumIII/sml/24504301.pdf
	//
	zp_vec4 minor0, minor1, minor2, minor3;
	zp_vec4 row0, row1, row2, row3;
	zp_vec4 det, tmp1;
	
	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src+ 4));
	row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));

	row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
	row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);

	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
	row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));

	row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
	row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);

	tmp1 = _mm_mul_ps(row2, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor0 = _mm_mul_ps(row1, tmp1);
	minor1 = _mm_mul_ps(row0, tmp1);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

	tmp1 = _mm_mul_ps(row1, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
	minor3 = _mm_mul_ps(row0, tmp1);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

	tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	row2 = _mm_shuffle_ps(row2, row2, 0x4E);

	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
	minor2 = _mm_mul_ps(row0, tmp1);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

	tmp1 = _mm_mul_ps(row0, row1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));

	tmp1 = _mm_mul_ps(row0, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));

	tmp1 = _mm_mul_ps(row0, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));

	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);

	det = _mm_mul_ps(row0, minor0);
	det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
	det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
	tmp1 = _mm_rcp_ss(det);

	det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
	det = _mm_shuffle_ps(det, det, 0x00);

	minor0 = _mm_mul_ps(det, minor0);
	_mm_storel_pi((__m64*)(src), minor0);
	_mm_storeh_pi((__m64*)(src+2), minor0);

	minor1 = _mm_mul_ps(det, minor1);
	_mm_storel_pi((__m64*)(src+4), minor1);
	_mm_storeh_pi((__m64*)(src+6), minor1);

	minor2 = _mm_mul_ps(det, minor2);
	_mm_storel_pi((__m64*)(src+ 8), minor2);
	_mm_storeh_pi((__m64*)(src+10), minor2);

	minor3 = _mm_mul_ps(det, minor3);
	_mm_storel_pi((__m64*)(src+12), minor3);
	_mm_storeh_pi((__m64*)(src+14), minor3);
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

#endif